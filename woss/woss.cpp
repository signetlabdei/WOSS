/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2009 Federico Guerra
 * and regents of the SIGNET lab, University of Padova
 *
 * Author: Federico Guerra - federico@guerra-tlc.com
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

/*
 * This software has been developed by Federico Guerra and SIGNET lab,
 * University of Padova, in collaboration with the NATO Centre for 
 * Maritime Research and Experimentation (http://www.cmre.nato.int ; 
 * E-mail: pao@cmre.nato.int), whose support is gratefully acknowledged.
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
#include "woss.h"


using namespace woss;


int Woss::woss_counter = 0;


#ifdef WOSS_MULTITHREAD

namespace woss {

  
  void destroyWossSpinlock() {  
    pthread_spin_destroy( &Woss::woss_mutex );
  }
  
  
  static pthread_spinlock_t& initWossSpinlock( pthread_spinlock_t* spin, int value ) {
    pthread_spin_init( spin, value );
    atexit(destroyWossSpinlock);
    return *spin;
  }
  
  
}

pthread_spinlock_t Woss::woss_mutex = woss::initWossSpinlock( &Woss::woss_mutex , 0 ) ;

#endif // WOSS_MULTITHREAD


Woss::Woss() 
: woss_id(0),
  work_dir_path(),
  db_manager(NULL),
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
#ifdef WOSS_MULTITHREAD
  pthread_spin_lock( &woss_mutex );
#endif // WOSS_MULTITHREAD
  
  woss_id = woss_counter++; 

#ifdef WOSS_MULTITHREAD
  pthread_spin_unlock( &woss_mutex );
#endif // WOSS_MULTITHREAD
}


Woss::Woss( const CoordZ& tx, const CoordZ& rx, const Time& start_t, const Time& end_t, double start_freq, double end_freq, double freq_step )
: woss_id(0),
  work_dir_path(),
  db_manager(NULL),
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
  
#ifdef WOSS_MULTITHREAD
  pthread_spin_lock( &woss_mutex );
#endif // WOSS_MULTITHREAD
  
  woss_id = woss_counter++; 

#ifdef WOSS_MULTITHREAD
  pthread_spin_unlock( &woss_mutex );
#endif // WOSS_MULTITHREAD

  insertFrequencies( start_freq, end_freq, freq_step );  
}


Woss::~Woss() {
#ifdef WOSS_MULTITHREAD
  pthread_spin_lock( &woss_mutex );
#endif // WOSS_MULTITHREAD
  
  woss_counter--; 
  
  if ( clean_workdir ) rmWorkDir();
  
#ifdef WOSS_MULTITHREAD
  pthread_spin_unlock( &woss_mutex );
#endif // WOSS_MULTITHREAD
}


Woss& Woss::insertFrequencies( double freq_start, double freq_end, double freq_step ) {
  assert( freq_start > 0 && freq_end > 0 && freq_step > 0 && freq_end >= freq_start );
  
  for( int i = 0; i <= floor( (freq_end - freq_start) / freq_step ); i++ ) {
    frequencies.insert( freq_start + ((double)i) * freq_step );
  }
  
  return *this;
}


bool Woss::rmWorkDir() {
  assert(work_dir_path.size() > 0);

  static ::std::stringstream str_out;
  str_out << "rm -fr " << work_dir_path << "woss" << woss_id << "/";
  ::std::string command = str_out.str();
  str_out.str("");

  if (debug) ::std::cout << "Woss(" << woss_id << ")::rmWorkDir() command = " << command << ::std::endl;

  int ret_value = -1;
  if (system(NULL)) ret_value = system(command.c_str());

  return( ret_value == 0 );
}


bool Woss::mkWorkDir( double curr_frequency, int curr_run ) {

  assert(work_dir_path.size() > 0);

  static ::std::stringstream str_out;
  str_out << "mkdir -p " << work_dir_path << "woss" << woss_id 
          << "/freq" << curr_frequency << "/time" << (time_t)current_time 
          << "/run" << curr_run;
          
  ::std::string command = str_out.str();
  str_out.str("");

  if (debug) ::std::cout << "Woss(" << woss_id << ")::mkWorkDir() command = " << command << ::std::endl;

  int ret_value = -1;
  if (system(NULL)) ret_value = system(command.c_str());

  return( ret_value == 0 );
}


bool Woss::rmWorkDir( double curr_frequency, int curr_run ) {
  static ::std::stringstream str_out;
  int ret_value = -1;
  
  str_out << "rm -fr " << work_dir_path << "woss" << woss_id 
          << "/freq" << curr_frequency << "/time" << (time_t)current_time
          << "/run" << curr_run;
          
  ::std::string command = str_out.str();
  str_out.str("");
  if ( system(NULL) ) ret_value = system(command.c_str());
  if( ret_value != 0 ) return false;

  return true;
}


bool Woss::initialize() {
  total_great_circle_distance = tx_coordz.getGreatCircleDistance(rx_coordz);
  total_distance = tx_coordz.getCartDistance(rx_coordz);
  bearing = tx_coordz.getInitialBearing(rx_coordz);
  
  if (debug) ::std::cout << "Woss(" << woss_id << ")::initialize() tx coords = " << tx_coordz << "; rx coords = " 
                       << rx_coordz << "; total_great_circle_distance = " << total_great_circle_distance 
                       << "; total_distance = " << total_distance << "; bearing = " << bearing << ::std::endl;

  return true;
}


bool Woss::isRunning() const {
#ifdef WOSS_MULTITHREAD
  bool ret_value;
  
  pthread_spin_lock( &woss_mutex );
  
  ret_value = is_running;

  pthread_spin_unlock( &woss_mutex );
  
  return ret_value;
#else 
  return is_running;
#endif // WOSS_MULTITHREAD  
}


void WossResReader::clearResReaderMap() {
  for ( RRMIter it = res_reader_map.begin() ; it != res_reader_map.end(); it++ ) {
    delete (it->second);
    it->second = NULL;
  }
}

