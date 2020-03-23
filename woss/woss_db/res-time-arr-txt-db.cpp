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
 * @file   res-time-arr-txt-db.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::ResTimeArrTxtDb class
 *
 * Provides the interface for the woss::ResTimeArrTxtDb class
 */


#include <iomanip>
#include <cassert>
#include <cstdlib>
#include <sstream>
#include <definitions.h>
#include <definitions-handler.h>
#include "res-time-arr-txt-db.h"


using namespace woss;


#define WOSS_CMM_WRITE_GAIN_MARGIN (4)
#define RES_NOT_SET (-2000)
#define RES_TIME_ARR_FREQ_PRECISION (1e-5)


double ResTimeArrTxtDb::space_sampling = 0.0;


ResTimeArrTxtDb::ResTimeArrTxtDb( const ::std::string& name )
 : WossTextualDb( name ), 
   arrivals_map(),
   initial_arrmap_size(0),
   has_been_modified(false)
{

}


bool ResTimeArrTxtDb::importMap() {

  textual_db.close();
  textual_db.open( db_name.c_str() , ::std::fstream::in );
  textual_db.precision(WOSS_DECIMAL_PRECISION); 
  
  while ( textual_db.good() ) {
      double tx_lat = RES_NOT_SET;
      double tx_long = RES_NOT_SET;
      double tx_z = RES_NOT_SET;
  
      double rx_lat = RES_NOT_SET;
      double rx_long = RES_NOT_SET;
      double rx_z = RES_NOT_SET;

      double frequency;
      time_t time;
      
      int total_taps = RES_NOT_SET;
      double delay = RES_NOT_SET;
      double press_real = RES_NOT_SET;
      double press_imag = RES_NOT_SET;
      
   
      textual_db >> tx_lat;     textual_db >> tx_long;     textual_db >> tx_z;
      textual_db >> rx_lat;     textual_db >> rx_long;     textual_db >> rx_z;
      textual_db >> frequency;  textual_db >> time;        textual_db >> total_taps;

      ::std::cout.precision(WOSS_DECIMAL_PRECISION);

//       if (debug) ::std::cout << ::std::setw(22) << tx_lat << ::std::setw(22) << tx_long << ::std::setw(22) << tx_z
//                              << ::std::setw(22) << rx_lat << ::std::setw(22) << rx_long << ::std::setw(22) << rx_z
//                              << ::std::setw(22) << frequency << ::std::setw(22) << time << ::std::setw(22) << total_taps;

      bool exit = (tx_lat == RES_NOT_SET) || (tx_long == RES_NOT_SET) || (tx_z == RES_NOT_SET) ||
                  (rx_lat == RES_NOT_SET) || (rx_long == RES_NOT_SET) || (rx_z == RES_NOT_SET);

      if (exit) break;

      TimeArr value;

      for (int i = 0; i < total_taps; i++) {
          textual_db >> delay; textual_db >> press_real;  textual_db >> press_imag;

//           if (debug) ::std::cout << ::std::setw(22) << delay << ::std::setw(22) << press_real << ::std::setw(22) << press_imag;

          assert(delay != RES_NOT_SET); assert(press_real != RES_NOT_SET); assert(press_imag != RES_NOT_SET);

          value.insertValue(delay, Pressure(press_real, press_imag));
      }
//       if (debug) ::std::cout << ::std::endl;

      arrivals_map[CoordZ(tx_lat, tx_long, tx_z)][CoordZ(rx_lat, rx_long, rx_z)][PDouble(frequency, RES_TIME_ARR_FREQ_PRECISION)][time] = value;
//       insertValue(CoordZ(tx_lat, tx_long, tx_z), CoordZ(rx_lat, rx_long, rx_z), frequency, value );

      initial_arrmap_size++;
  } 

  textual_db.close();

  if ( debug ) printScreenMap();
   
  return( initial_arrmap_size > 0 );
}


const TimeArr* ResTimeArrTxtDb::readMap( const CoordZ& tx, const CoordZ& rx, const double freq, const Time& time_value ) const {
  const TimeArr* ret_val = NULL;

  if(debug) ::std::cout << "ResTimeArrTxtDb::readMap() tx coords " << tx << "; rx coords " << rx 
                        << "; frequency = " << freq << "; time = " << time_value << ::std::endl;

  // searching for first position;
  AMXCIter it = arrivals_map.find( tx );

  // searching for second position;
  if ( it != arrivals_map.end() ) {

    if(debug) ::std::cout << "ResTimeArrTxtDb::readMap() tx coords found." << ::std::endl;
    
    RxMCIter it2 = it->second.find( rx );

    if ( it2 != it->second.end() ) {
      
      if(debug) ::std::cout << "ResTimeArrTxtDb::readMap() rx coords found." << ::std::endl;
      
      FMCIter it3 = it2->second.find( PDouble( freq, RES_TIME_ARR_FREQ_PRECISION ) );
      
      if ( it3 != it2->second.end() ) {
        
        if (debug) ::std::cout << "ResTimeArrTxtDb::readMap() freq found." << ::std::endl;
    
        TMCIter it4 = it3->second.find( time_value );
        
        if ( it4 != it3->second.end() ) {
          
          if (debug) ::std::cout << "ResTimeArrTxtDb::readMap() time found." << ::std::endl; 
          
          ret_val = &it4->second;
        } 
      }
    }
  }
  
  if (debug && ret_val == NULL) ::std::cout << "ResTimeArrTxtDb::readMap() an input parameter was not found" << ::std::endl;

  return ret_val;
}


bool ResTimeArrTxtDb::writeMap() {

  textual_db.close();
  textual_db.open( db_name.c_str() , ::std::ios::out );

  if ( !textual_db.is_open() ) return false;

  ::std::stringstream ss;

  ss.precision(WOSS_DECIMAL_PRECISION);
  textual_db.precision(WOSS_DECIMAL_PRECISION);

  for (AMXIter it = arrivals_map.begin(); it != arrivals_map.end(); it++) {
    double tx_lat = it->first.getLatitude();
    double tx_long = it->first.getLongitude();
    double tx_z = it->first.getDepth();

    ss << tx_lat;
    int tx_lat_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
    ss.str("");

    ss << tx_long;
    int tx_long_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
    ss.str("");

    ss << tx_z;
    int tx_z_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
    ss.str("");

    for ( RxMap::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {

      double rx_lat = it2->first.getLatitude();
      double rx_long = it2->first.getLongitude();
      double rx_z = it2->first.getDepth();

      ss << rx_lat;
      int rx_lat_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
      ss.str("");

      ss << rx_long;
      int rx_long_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
      ss.str("");

      ss << rx_z;
      int rx_z_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
      ss.str("");
      
      for ( FreqMap::iterator it3 = it2->second.begin(); it3 != it2->second.end(); it3++ ) {        
        double frequency = it3->first;

        ss << frequency;
        int freq_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
        ss.str("");        

        for ( TimeMap::iterator it4 = it3->second.begin(); it4 != it3->second.end(); it4++ ) {
          
          time_t time = it4->first;
          
          ss << time;
          int time_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");
          
          TimeArr& arr = it4->second;
          int arr_taps = arr.size();
          
          ss << arr_taps;
          int arr_taps_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");

          textual_db << ::std::setw(tx_lat_size) << tx_lat << ::std::setw(tx_long_size) << tx_long << ::std::setw(tx_z_size) << tx_z
                     << ::std::setw(rx_lat_size) << rx_lat << ::std::setw(rx_long_size) << rx_long << ::std::setw(rx_z_size) << rx_z
                     << ::std::setw(freq_size) << frequency << ::std::setw(time_size) << time << ::std::setw(arr_taps_size) 
                     << arr_taps << ::std::flush;

          for ( TimeArrCIt it5 = arr.begin(); it5 != arr.end(); it5++ ) {

            ss << it5->first;
            int arr_delay_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
            ss.str("");

            ss << it5->second.real();
            int press_real_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
            ss.str("");

            ss << it5->second.imag();
            int press_imag_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
            ss.str("");
            
            textual_db << ::std::setw(arr_delay_size) << it5->first << ::std::setw(press_real_size) << it5->second.real() 
                       << ::std::setw(press_imag_size) << it5->second.imag() << ::std::flush;
          }
          textual_db << ::std::endl;
        }
      }
    }
  }
  return true;
}


void ResTimeArrTxtDb::printScreenMap() {

  ::std::stringstream ss;

  ss.precision(WOSS_DECIMAL_PRECISION);
  textual_db.precision(WOSS_DECIMAL_PRECISION);

  for (AMXIter it = arrivals_map.begin(); it != arrivals_map.end(); it++) {
    double tx_lat = it->first.getLatitude();
    double tx_long = it->first.getLongitude();
    double tx_z = it->first.getDepth();

    ss << tx_lat;
    int tx_lat_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
    ss.str("");

    ss << tx_long;
    int tx_long_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
    ss.str("");

    ss << tx_z;
    int tx_z_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
    ss.str("");

    for ( RxMap::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {

      double rx_lat = it2->first.getLatitude();
      double rx_long = it2->first.getLongitude();
      double rx_z = it2->first.getDepth();

      ss << rx_lat;
      int rx_lat_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
      ss.str("");

      ss << rx_long;
      int rx_long_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
      ss.str("");

      ss << rx_z;
      int rx_z_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
      ss.str("");
      
      for ( FreqMap::iterator it3 = it2->second.begin(); it3 != it2->second.end(); it3++ ) {        
        double frequency = it3->first;

        ss << frequency;
        int freq_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
        ss.str("");        

        for ( TimeMap::iterator it4 = it3->second.begin(); it4 != it3->second.end(); it4++ ) {
          
          time_t time = it4->first;
          
          ss << time;
          int time_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");
          
          TimeArr& arr = it4->second;
          int arr_taps = arr.size();
          
          ss << arr_taps;
          int arr_taps_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
          ss.str("");

          ::std::cout << ::std::setw(tx_lat_size) << tx_lat << ::std::setw(tx_long_size) << tx_long << ::std::setw(tx_z_size) << tx_z
                      << ::std::setw(rx_lat_size) << rx_lat << ::std::setw(rx_long_size) << rx_long << ::std::setw(rx_z_size) << rx_z
                      << ::std::setw(freq_size) << frequency << ::std::setw(arr_taps_size) << time << ::std::setw(time_size)
                      << arr_taps << ::std::flush;

          for ( TimeArrCIt it5 = arr.begin(); it5 != arr.end(); it5++ ) {

            ss << it5->first;
            int arr_delay_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
            ss.str("");

            ss << it5->second.real();
            int press_real_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
            ss.str("");

            ss << it5->second.imag();
            int press_imag_size = ss.str().length() + WOSS_CMM_WRITE_GAIN_MARGIN;
            ss.str("");
            
            ::std::cout << ::std::setw(arr_delay_size) << it5->first << ::std::setw(press_real_size) << it5->second.real() 
                        << ::std::setw(press_imag_size) << it5->second.imag() << ::std::flush;
          }
          ::std::cout << ::std::endl;
        }
      }
    }
  }
}


bool ResTimeArrTxtDb::finalizeConnection() {
  importMap();
  return true;
}


bool ResTimeArrTxtDb::closeConnection() {
  bool ok = true;
  if (has_been_modified) ok = writeMap();
  assert(ok);
  return ok && WossTextualDb::closeConnection();
}


TimeArr* ResTimeArrTxtDb::getValue( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value ) const {
  if ( arrivals_map.size() > 0 && time_value.isValid() ) {
    const TimeArr* ptr = readMap( coord_tx, coord_rx, frequency, time_value);
    if (ptr == NULL) return SDefHandler::instance()->getTimeArr()->create( TimeArr::createNotValid() );
    else return ptr->create( *ptr );
  }
  else return( SDefHandler::instance()->getTimeArr()->create( TimeArr::createNotValid() ) );
}


bool ResTimeArrTxtDb::insertValue( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value, const TimeArr& channel ) {
  
  AMXIter it1 = arrivals_map.find( coord_tx );

  if (it1 == arrivals_map.end() ) { // no tx CoordZ found

    if (debug) ::std::cout << "ResTimeArrTxtDb::insertValue() no tx CoordZ found" << ::std::endl;
    
    arrivals_map[coord_tx][coord_rx][PDouble(frequency, RES_TIME_ARR_FREQ_PRECISION)][time_value] = channel;
    
//     debugWaitForUser();
  }
  else { // start CoordZ found
    RxMIter it2 = (it1->second).find( coord_rx );

    if ( it2 == it1->second.end() ) { // no rx CoordZ foundZ

      if (debug) ::std::cout << "ResTimeArrTxtDb::insertValue() no rx CoordZ found" << ::std::endl;

      (it1->second)[coord_rx][PDouble(frequency, RES_TIME_ARR_FREQ_PRECISION)][time_value] = channel;
      
//       debugWaitForUser();
    }
    else {
      FMIter it3 = it2->second.find( PDouble(frequency, RES_TIME_ARR_FREQ_PRECISION) );
      
      if ( it3 == it2->second.end() ) { // no freq found
        
        if (debug) ::std::cout << "ResTimeArrTxtDb::insertValue() no frequency found" << ::std::endl;

        (it2->second)[PDouble(frequency, RES_TIME_ARR_FREQ_PRECISION)][time_value] = channel;
        
//         debugWaitForUser();
      }
      else {
        TMIter it4 = it3->second.find( time_value );
        
        if ( it4 == it3->second.end() ) { // no time found

          if (debug) ::std::cout << "ResTimeArrTxtDb::insertValue() no time found" << ::std::endl;
          
          (it3->second)[time_value] = channel;
        }
        else (it4->second) = channel; // freq found
      }
    }
  }
    
  has_been_modified = true;
  return true;
}


