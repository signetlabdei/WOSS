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
 * @file   res-time-arr-bin-db.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::ResTimeArrBinDb class
 *
 * Provides the interface for the woss::ResTimeArrBinDb class
 */


#include <cassert>
#include "res-time-arr-bin-db.h"


using namespace woss;


#define RES_NOT_SET (-2000)
#define RES_TIME_ARR_FREQ_PRECISION (1e-5)


bool ResTimeArrBinDb::importMap() {

  textual_db.close();
  textual_db.open( db_name.c_str(), ::std::fstream::in | ::std::fstream::binary );
//   textual_db.precision(WOSS_DECIMAL_PRECISION); 
  
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
   
      textual_db.read( reinterpret_cast< char* > (&tx_lat), sizeof(double) );
      textual_db.read( reinterpret_cast< char* > (&tx_long), sizeof(double) );
      textual_db.read( reinterpret_cast< char* > (&tx_z), sizeof(double) );
      textual_db.read( reinterpret_cast< char* > (&rx_lat), sizeof(double) );
      textual_db.read( reinterpret_cast< char* > (&rx_long), sizeof(double) );
      textual_db.read( reinterpret_cast< char* > (&rx_z), sizeof(double) );
      textual_db.read( reinterpret_cast< char* > (&frequency), sizeof(double) );
      textual_db.read( reinterpret_cast< char* > (&time), sizeof(time_t) );
      textual_db.read( reinterpret_cast< char* > (&total_taps), sizeof(int) );
        
//       ::std::cout.precision(WOSS_DECIMAL_PRECISION);
//       ::std::cout << tx_lat << " " << tx_long << " " << tx_z << " " << rx_lat << " " 
//                   << rx_long << " " << rx_z << " " << " " << frequency << " " << time << " " << total_taps;

      bool exit = (tx_lat == RES_NOT_SET) || (tx_long == RES_NOT_SET) || (tx_z == RES_NOT_SET) ||
                  (rx_lat == RES_NOT_SET) || (rx_long == RES_NOT_SET) || (rx_z == RES_NOT_SET);

      if ( !textual_db.good() || exit ) break;

      TimeArr value;

      for (int i = 0; i < total_taps; i++) {
        textual_db.read( reinterpret_cast< char* > (&delay), sizeof(double) );          
        textual_db.read( reinterpret_cast< char* > (&press_real), sizeof(double) );
        textual_db.read( reinterpret_cast< char* > (&press_imag), sizeof(double) );   
               
//         ::std::cout << " " << delay << " " << press_real << " " << press_imag;

        assert(delay != RES_NOT_SET); assert(press_real != RES_NOT_SET); assert(press_imag != RES_NOT_SET);

        value.insertValue(delay, Pressure(press_real, press_imag));
      }
//       ::std::cout << ::std::endl;

      arrivals_map[CoordZ(tx_lat, tx_long, tx_z)][CoordZ(rx_lat, rx_long, rx_z)][PDouble(frequency, RES_TIME_ARR_FREQ_PRECISION)][time] = value;
//       insertValue(CoordZ(tx_lat, tx_long, tx_z), CoordZ(rx_lat, rx_long, rx_z), frequency, value );

      initial_arrmap_size++;
  } 

  textual_db.close();

  if ( debug ) printScreenMap();
   
  return( initial_arrmap_size > 0 );
}


bool ResTimeArrBinDb::writeMap() {

  textual_db.close();
  textual_db.open( db_name.c_str(), ::std::ios::out | ::std::fstream::binary );

  if ( !textual_db.is_open() ) return false;

//   textual_db.precision(WOSS_DECIMAL_PRECISION);

  for (AMXIter it = arrivals_map.begin(); it != arrivals_map.end(); it++) {
    double tx_lat = it->first.getLatitude();
    double tx_long = it->first.getLongitude();
    double tx_z = it->first.getDepth();

    for ( RxMap::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
      double rx_lat = it2->first.getLatitude();
      double rx_long = it2->first.getLongitude();
      double rx_z = it2->first.getDepth();
     
      for ( FreqMap::iterator it3 = it2->second.begin(); it3 != it2->second.end(); it3++ ) {        
        double frequency = it3->first;
        
        for ( TimeMap::iterator it4 = it3->second.begin(); it4 != it3->second.end(); it4++ ) {
          time_t time = it4->first;
          
          TimeArr& arr = it4->second;
          int arr_taps = arr.size();

          textual_db.write( reinterpret_cast< char* > (&tx_lat), sizeof(double) );
          textual_db.write( reinterpret_cast< char* > (&tx_long), sizeof(double) );
          textual_db.write( reinterpret_cast< char* > (&tx_z), sizeof(double) );
          textual_db.write( reinterpret_cast< char* > (&rx_lat), sizeof(double) );
          textual_db.write( reinterpret_cast< char* > (&rx_long), sizeof(double) );
          textual_db.write( reinterpret_cast< char* > (&rx_z), sizeof(double) );
          textual_db.write( reinterpret_cast< char* > (&frequency), sizeof(double) );
          textual_db.write( reinterpret_cast< char* > (&time), sizeof(time_t) );
          textual_db.write( reinterpret_cast< char* > (&arr_taps), sizeof(int) );

          for ( TimeArrCIt it5 = arr.begin(); it5 != arr.end(); it5++ ) {
            double delay = it5->first;
            double real = it5->second.real();
            double imag = it5->second.imag();
            
            textual_db.write( reinterpret_cast< char* > (&delay), sizeof(double) );          
            textual_db.write( reinterpret_cast< char* > (&real), sizeof(double) );
            textual_db.write( reinterpret_cast< char* > (&imag), sizeof(double) );          
          }
        }
      }
    }
  }
  return true;
}

