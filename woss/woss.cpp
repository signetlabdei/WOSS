/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2009 2025 Federico Guerra
 * and regents of the SIGNET lab, University of Padova
 *
 * Author: Federico Guerra - WOSS@guerra-tlc.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */ 


/**
 * @file   woss.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::Woss and woss::WossResReader class
 *
 * Provides the implementation of the woss::Woss and woss::WossResReader classes
 */


#include <cstdlib>
#include <sstream>
#include <filesystem>
#include "woss.h"


using namespace woss;

#if defined (WOSS_MULTITHREAD)
std::atomic<int> Woss::woss_counter = 0;
#else
int Woss::woss_counter = 0;
#endif // defined (WOSS_MULTITHREAD)

Woss::Woss() 
: woss_id(0),
  work_dir_path(),
  db_manager(nullptr),
  start_time(),
  current_time(),
  end_time(),
  evolution_time_quantum(-1.0),
  tx_coordz(),
  rx_coordz(),
  frequencies(),
  bearing(0.0),
  total_great_circle_distance(0.0),
  total_distance(0.0),
  total_runs(1),
  debug(false),
  has_run_once(false),
  is_running(false),
  clean_workdir(false)
{
  woss_id = woss_counter++; 
}


Woss::Woss( const CoordZ& tx, const CoordZ& rx, const Time& start_t, const Time& end_t, double start_freq, double end_freq, double freq_step )
: woss_id(0),
  work_dir_path(),
  db_manager(nullptr),
  start_time(start_t),
  current_time(start_t),
  end_time(end_t),
  evolution_time_quantum(-1.0),
  tx_coordz(tx),
  rx_coordz(rx),
  frequencies(),
  bearing(0.0),
  total_great_circle_distance(0.0),
  total_distance(0.0),
  total_runs(1),
  debug(false),
  has_run_once(false),
  is_running(false),
  clean_workdir(false)
{
  assert( tx.isValid() && rx.isValid() );
  assert( start_time.isValid() && end_t.isValid() );
  assert( end_time >= start_time );

  woss_id = woss_counter++; 

  insertFrequencies( start_freq, end_freq, freq_step );
}


Woss::~Woss() {
  woss_counter--; 

  if ( clean_workdir ) 
    rmWorkDir();
}


Woss& Woss::insertFrequencies( double freq_start, double freq_end, double freq_step ) {
  assert( freq_start > 0 && freq_end > 0 && freq_step > 0 && freq_end >= freq_start );
  
  for( int i = 0; i <= floor( (freq_end - freq_start) / freq_step ); i++ ) {
    frequencies.emplace( freq_start + ((double)i) * freq_step );
  }
  
  return *this;
}


bool Woss::rmWorkDir() {
  assert(work_dir_path.size() > 0);
  std::stringstream str_out;
  std::error_code error;

  str_out << work_dir_path << "woss" << woss_id << "/";
  std::string path = str_out.str();
  str_out.str("");

  if (debug) 
    std::cout << "Woss(" << woss_id << ")::rmWorkDir() path = " << path << std::endl;

  if (std::filesystem::exists(path) == false)
  {
    // The folder does not exist
    std::cout << "Woss(" << woss_id << ")::rmWorkDir() folder does not exist" << std::endl;
    return false;
  }
  if (std::filesystem::remove_all(path, error) == false) {
    std::cout << "Woss(" << woss_id << ")::rmWorkDir() FAILED, err: " << error.message() << std::endl;
    return false;
  }
  return true;
}


bool Woss::mkWorkDir( double curr_frequency, int curr_run ) {
  assert(work_dir_path.size() > 0);
  std::stringstream str_out;
  std::error_code error;

  str_out << work_dir_path << "woss" << woss_id 
          << "/freq" << curr_frequency << "/time" << (time_t)current_time 
          << "/run" << curr_run;
          
  std::string path = str_out.str();
  str_out.str("");

  if (debug) 
    std::cout << "Woss(" << woss_id << ")::mkWorkDir() path = " << path << std::endl;

  if (std::filesystem::create_directories(path, error) == false) {
    if (std::filesystem::exists(path))
    {
      // The folder already exists
      error.clear();
      return true;
    }
    else {
      std::cout << "Woss(" << woss_id << ")::mkWorkDir() FAILED, err: " << error.message() << std::endl;
      return false;
    }
  }
  return true;
}


bool Woss::rmWorkDir( double curr_frequency, int curr_run ) {
  assert(work_dir_path.size() > 0);
  std::stringstream str_out;
  std::error_code error;

  str_out << work_dir_path << "woss" << woss_id 
          << "/freq" << curr_frequency << "/time" << (time_t)current_time 
          << "/run" << curr_run;
          
  std::string path = str_out.str();
  str_out.str("");

  if (debug) 
    std::cout << "Woss(" << woss_id << ")::rmWorkDir() path = " << path << std::endl;

  if (std::filesystem::exists(path) == false)
  {
    // The folder does not exist
    std::cout << "Woss(" << woss_id << ")::rmWorkDir() folder does not exist" << std::endl;
    return false;
  }
  if (std::filesystem::remove_all(path, error) == false) {
    std::cout << "Woss(" << woss_id << ")::rmWorkDir() FAILED, err: " << error.message() << std::endl;
    return false;
  }
  return true;
}


bool Woss::initialize() {
  total_great_circle_distance = tx_coordz.getGreatCircleDistance(rx_coordz);
  total_distance = tx_coordz.getCartDistance(rx_coordz);
  bearing = tx_coordz.getInitialBearing(rx_coordz);
  
  if (debug) 
    std::cout << "Woss(" << woss_id << ")::initialize() tx coords = " << tx_coordz << "; rx coords = " 
              << rx_coordz << "; total_great_circle_distance = " << total_great_circle_distance 
              << "; total_distance = " << total_distance << "; bearing = " << bearing << std::endl;

  return true;
}


bool Woss::isRunning() const {
  return is_running;
}


void WossResReader::clearResReaderMap() {
  res_reader_map.clear();
}

