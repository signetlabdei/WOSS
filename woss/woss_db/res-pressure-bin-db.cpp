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
 * @file   res-pressure-bin-db.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::ResPressureBinDb class
 *
 * Provides the implementation of the woss::ResPressureBinDb class
 */


#include <cassert>
#include <pressure-definitions.h>
#include "res-pressure-bin-db.h"


using namespace woss;


#define RES_NOT_SET (-2000)
#define RES_PRESSURE_FREQ_PRECISION (1e-5)



bool ResPressureBinDb::importMap() {
  textual_db.close();
  textual_db.open( db_name.c_str() , ::std::fstream::in | ::std::fstream::binary );

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
      
      textual_db.read( reinterpret_cast< char* > (&tx_lat), sizeof(double) );
      textual_db.read( reinterpret_cast< char* > (&tx_long), sizeof(double) );
      textual_db.read( reinterpret_cast< char* > (&tx_z), sizeof(double) );
      textual_db.read( reinterpret_cast< char* > (&rx_lat), sizeof(double) );
      textual_db.read( reinterpret_cast< char* > (&rx_long), sizeof(double) );
      textual_db.read( reinterpret_cast< char* > (&rx_z), sizeof(double) );
      textual_db.read( reinterpret_cast< char* > (&frequency), sizeof(double) );
      textual_db.read( reinterpret_cast< char* > (&time), sizeof(time_t) );
      textual_db.read( reinterpret_cast< char* > (&press_real), sizeof(double) );
      textual_db.read( reinterpret_cast< char* > (&press_imag), sizeof(double) );

      if (!textual_db.good()) break;
 
//       if (debug) ::std::cout.precision(WOSS_DECIMAL_PRECISION);
  
//       if (debug) ::std::cout << ::std::setw(22) << tx_lat << ::std::setw(22) << tx_long << ::std::setw(22) << tx_z
//                              << ::std::setw(22) << rx_lat << ::std::setw(22) << rx_long << ::std::setw(22) << rx_z
//                              << ::std::setw(22) << frequency << ::std::setw(22) << time << ::std::setw(22) << press_real 
//                              << ::std::setw(22) << press_imag << ::std::endl;

      assert(tx_lat != RES_NOT_SET); assert(tx_long != RES_NOT_SET); assert(tx_z != RES_NOT_SET);
      assert(rx_lat != RES_NOT_SET); assert(rx_long != RES_NOT_SET); assert(rx_z != RES_NOT_SET);
      assert(press_real != RES_NOT_SET); assert(press_imag != RES_NOT_SET); assert(frequency != RES_NOT_SET);
      assert(time != 0); 
      
      pressure_map[CoordZ(tx_lat, tx_long, ::std::abs(tx_z))][CoordZ(rx_lat, rx_long, ::std::abs(rx_z))][PDouble(frequency, RES_PRESSURE_FREQ_PRECISION)][time] = 
               ::std::complex<double> (press_real, press_imag);
               
      initial_pressmap_size++;
  } 

  textual_db.close();

  if (debug) printScreenMap();

  return( initial_pressmap_size > 0 );
}


bool ResPressureBinDb::writeMap() {
  textual_db.close();
  textual_db.open( db_name.c_str() , ::std::fstream::out | ::std::fstream::binary );

  if ( !textual_db.is_open() ) return false;

  for (PMIter it = pressure_map.begin(); it != pressure_map.end(); it++) {
    double tx_lat = it->first.getLatitude();
    double tx_long = it->first.getLongitude();
    double tx_z = it->first.getDepth();

    for ( RxMap::iterator it2 = it->second.begin(); it2 != it->second.end(); it2++) {
      double rx_lat = it2->first.getLatitude();
      double rx_long = it2->first.getLongitude();
      double rx_z = it2->first.getDepth();
      
      for( FreqMap::iterator it3 = it2->second.begin(); it3 != it2->second.end(); it3++ ) {
        double frequency = it3->first;
        
        for ( TimeMap::iterator it4 = it3->second.begin(); it4 != it3->second.end(); it4++ ) {
            
          time_t time = it4->first;
          double real = it4->second.real();
          double imag = it4->second.imag();
          
          textual_db.write( reinterpret_cast< char* > (&tx_lat), sizeof(double) );
          textual_db.write( reinterpret_cast< char* > (&tx_long), sizeof(double) );
          textual_db.write( reinterpret_cast< char* > (&tx_z), sizeof(double) );
          textual_db.write( reinterpret_cast< char* > (&rx_lat), sizeof(double) );
          textual_db.write( reinterpret_cast< char* > (&rx_long), sizeof(double) );
          textual_db.write( reinterpret_cast< char* > (&rx_z), sizeof(double) );
          textual_db.write( reinterpret_cast< char* > (&frequency), sizeof(double) );
          textual_db.write( reinterpret_cast< char* > (&time), sizeof(time_t) );
          textual_db.write( reinterpret_cast< char* > (&real), sizeof(double) );
          textual_db.write( reinterpret_cast< char* > (&imag), sizeof(double) );
        }
      }
    }
  } 
  return true;
}

