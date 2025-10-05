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
 * @file   res-pressure-txt-db.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::ResPressureTxtDb class
 *
 * Provides the implementation of the woss::ResPressureTxtDb class
 */


#include <iomanip>
#include <cassert>
#include <sstream>
#include <definitions.h>
#include <pressure-definitions.h>
#include <definitions-handler.h>
#include "res-pressure-txt-db.h"


using namespace woss;


static constexpr inline int WOSS_CMM_WRITE_GAIN_MARGIN = 4;
static constexpr inline double RES_NOT_SET = (-2000);
static constexpr inline double RES_PRESSURE_FREQ_PRECISION = (1e-5);


double ResPressureTxtDb::space_sampling = 0.0;


ResPressureTxtDb::ResPressureTxtDb( const std::string& name )
: WossTextualDb( name ),
  pressure_map(),
  initial_pressmap_size(0),
  has_been_modified(false)
{

}


bool ResPressureTxtDb::importMap() {
  textual_db.close();
  textual_db.open( db_name.c_str() , std::fstream::in );
  textual_db.precision(WOSS_DECIMAL_PRECISION); 

  while ( textual_db.good() ) {
    double tx_lat = RES_NOT_SET;
    double tx_long = RES_NOT_SET;
    double tx_z = RES_NOT_SET;

    double rx_lat = RES_NOT_SET;
    double rx_long = RES_NOT_SET;
    double rx_z = RES_NOT_SET;
  
    double frequency = RES_NOT_SET;
    time_t time = 0;
    
    double press_real = RES_NOT_SET;
    double press_imag = RES_NOT_SET;
    
    textual_db >> tx_lat;     textual_db >> tx_long;    textual_db >> tx_z;
    textual_db >> rx_lat;     textual_db >> rx_long;    textual_db >> rx_z;
    textual_db >> frequency;  textual_db >> time;
    textual_db >> press_real; textual_db >> press_imag;

    if (!textual_db.good()) 
      break;

    if (debug) {
      std::cout.precision(WOSS_DECIMAL_PRECISION);

      std::cout << std::setw(22) << tx_lat << std::setw(22) << tx_long << std::setw(22) << tx_z
                << std::setw(22) << rx_lat << std::setw(22) << rx_long << std::setw(22) << rx_z
                << std::setw(22) << frequency << std::setw(22) << time << std::setw(22) 
                << press_real << std::setw(22) << press_imag << std::endl;
    }

    assert(tx_lat != RES_NOT_SET); assert(tx_long != RES_NOT_SET); assert(tx_z != RES_NOT_SET);
    assert(rx_lat != RES_NOT_SET); assert(rx_long != RES_NOT_SET); assert(rx_z != RES_NOT_SET);
    assert(press_real != RES_NOT_SET); assert(press_imag != RES_NOT_SET); assert(frequency != RES_NOT_SET);
    assert(time != 0);

    pressure_map[CoordZ(tx_lat, tx_long, std::abs(tx_z))][CoordZ(rx_lat, rx_long, std::abs(rx_z))][PDouble(frequency, RES_PRESSURE_FREQ_PRECISION)][time] = 
              std::complex<double> (press_real, press_imag);
              
    initial_pressmap_size++;
  } 

  textual_db.close();

//   initial_pressmap_size = pressure_map.size();

  if (debug) 
    printScreenMap();

  return( initial_pressmap_size > 0 );
}


std::complex<double> ResPressureTxtDb::readMap(const CoordZ& tx, const CoordZ& rx, const double freq, const Time& time_value ) const {
  std::complex<double> ret_val = Pressure::createNotValid();

  // searching for first position;
  PMCIter it = pressure_map.find( tx );

  // searching for second position;
  if ( it != pressure_map.end() ) {
    RxMap::const_iterator it2 = it->second.find( rx );

    if ( it2 != it->second.end() ) {
      FreqMap::const_iterator it3 = it2->second.find( PDouble(freq,RES_PRESSURE_FREQ_PRECISION) );
      
      if ( it3 != it2->second.end() ) {
        TimeMap::const_iterator it4 = it3->second.find( time_value );
        
        if ( it4 != it3->second.end() ) {
          ret_val = it4->second;
        }
      } 
    }
  }
  return ret_val;
}


bool ResPressureTxtDb::writeMap() {
  if ( debug ) 
    printScreenMap();

  textual_db.close();
  textual_db.open( db_name.c_str() , std::fstream::out );

  if ( !textual_db.is_open() ) 
    return false;

  std::stringstream ss;

  ss.precision(WOSS_DECIMAL_PRECISION);
  textual_db.precision(WOSS_DECIMAL_PRECISION);

  for ( const auto& it : pressure_map ) {
    double tx_lat = it.first.getLatitude();
    double tx_long = it.first.getLongitude();
    double tx_z = it.first.getDepth();

    ss << tx_lat;
    int tx_lat_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
    ss.str("");

    ss << tx_long;
    int tx_long_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
    ss.str("");

    ss << tx_z;
    int tx_z_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
    ss.str("");

    for ( const auto& it2 : it.second ) {
      double rx_lat = it2.first.getLatitude();
      double rx_long = it2.first.getLongitude();
      double rx_z = it2.first.getDepth();
      
      for( const auto& it3 : it2.second ) {
        double frequency = it3.first;

        for ( const auto& it4 : it3.second ) {
          time_t time = it4.first;

          std::complex<double> press = it4.second;

          ss << rx_lat;
          int rx_lat_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");

          ss << rx_long;
          int rx_long_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");

          ss << rx_z;
          int rx_z_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");
          
          ss << frequency;
          int freq_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");

          ss << time;
          int time_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");
          
          ss << press.real();
          int press_real_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");

          ss << press.imag();
          int press_imag_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");

          textual_db << std::setw(tx_lat_size) << tx_lat << std::setw(tx_long_size) << tx_long << std::setw(tx_z_size) << tx_z
                     << std::setw(rx_lat_size) << rx_lat << std::setw(rx_long_size) << rx_long << std::setw(rx_z_size) << rx_z
                     << std::setw(freq_size) << frequency << std::setw(time_size) << time 
                     << std::setw(press_real_size) << press.real() << std::setw(press_imag_size) << press.imag() << std::endl;
        }
      }
    }
  } 
  return true;
}


void ResPressureTxtDb::printScreenMap() {
  std::stringstream ss;

  ss.precision(WOSS_DECIMAL_PRECISION);
  textual_db.precision(WOSS_DECIMAL_PRECISION);

  for (const auto& it : pressure_map ) {
    double tx_lat = it.first.getLatitude();
    double tx_long = it.first.getLongitude();
    double tx_z = it.first.getDepth();

    ss << tx_lat;
    int tx_lat_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
    ss.str("");

    ss << tx_long;
    int tx_long_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
    ss.str("");

    ss << tx_z;
    int tx_z_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
    ss.str("");

    for ( const auto& it2 : it.second ) {
      double rx_lat = it2.first.getLatitude();
      double rx_long = it2.first.getLongitude();
      double rx_z = it2.first.getDepth();
      
      for( const auto& it3 : it2.second ) {
        double frequency = it3.first;
        
        for ( auto it4 : it3.second ) {
          time_t time = it4.first;
          auto press = it4.second;

          ss << rx_lat;
          int rx_lat_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");

          ss << rx_long;
          int rx_long_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");

          ss << rx_z;
          int rx_z_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");
          
          ss << frequency;
          int freq_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");

          ss << time;
          int time_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");
          
          ss << press.real();
          int press_real_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");

          ss << press.imag();
          int press_imag_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");

          std::cout << std::setw(tx_lat_size) << tx_lat << std::setw(tx_long_size) << tx_long << std::setw(tx_z_size) << tx_z
                    << std::setw(rx_lat_size) << rx_lat << std::setw(rx_long_size) << rx_long << std::setw(rx_z_size) << rx_z
                    << std::setw(freq_size) << frequency << std::setw(press_real_size) << time 
                    << std::setw(time_size) << press.real() << std::setw(press_imag_size) << press.imag() << std::endl;      
        }
      }
    }
  } 
}


bool ResPressureTxtDb::finalizeConnection() {
  importMap();
  return true;
}


bool ResPressureTxtDb::closeConnection() {
  bool ok = true;
  if (has_been_modified) 
    ok = writeMap();
  assert(ok);
  return ok && WossTextualDb::closeConnection();
}


std::unique_ptr<Pressure> ResPressureTxtDb::getValue( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value ) const {
  if ( pressure_map.size() > 0 && time_value.isValid() ) {
    return( SDefHandler::instance().createPressure( readMap(coord_tx, coord_rx, frequency, time_value) ) );
  }
  else {
    return( SDefHandler::instance().createPressure( Pressure::createNotValid() ) );
  }
}


bool ResPressureTxtDb::insertValue( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value, const Pressure& pressure ) {
  auto it1 = pressure_map.find( coord_tx );

  if (it1 == pressure_map.end() ) { // no tx CoordZ found
    if (debug)
      std::cout << "ResPressureTxtDb::insertValue() no tx CoordZ found" << std::endl;

    pressure_map[coord_tx][coord_rx][PDouble(frequency, RES_PRESSURE_FREQ_PRECISION)][time_value] = pressure;
  }
  else { // start CoordZ found
    auto it2 = (it1->second).find( coord_rx );

    if ( it2 == it1->second.end() ) { // no rx CoordZ foundZ
      if (debug) 
        std::cout << "ResPressureTxtDb::insertValue() no rx CoordZ found" << std::endl;

      (it1->second)[coord_rx][PDouble(frequency, RES_PRESSURE_FREQ_PRECISION)][time_value] = pressure;
    }
    else {
      auto it3 = it2->second.find( PDouble(frequency, RES_PRESSURE_FREQ_PRECISION) );

      if ( it3 == it2->second.end() ) {
        if (debug) 
          std::cout << "ResPressureTxtDb::insertValue() no frequency found" << std::endl;

        (it2->second)[PDouble(frequency, RES_PRESSURE_FREQ_PRECISION)][time_value] = pressure;
      }
      else {
        auto it4 = it3->second.find( time_value );

        if ( it4 == it3->second.end() ) {
          if (debug) 
            std::cout << "ResPressureTxtDb::insertValue() no time found" << std::endl;
          
          (it3->second)[time_value] = pressure;
        }
        else 
          (it4->second) = pressure;
      }
    }
  }

  has_been_modified = true;
  return true;
}
