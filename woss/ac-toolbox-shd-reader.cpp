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
 * @file   ac-toolbox-shd-reader.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::ShdResReader and woss::ShdData classes
 *
 * Provides the implementation of the woss::ShdResReader and woss::ShdData classes
 */


#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include "woss.h"
#include "bellhop-woss.h"
#include <definitions-handler.h>
#include "ac-toolbox-shd-reader.h"


#define SHD_RES_NOT_SET (-4000)


using namespace woss;


ShdData::ShdData()
: record_length(0),
  plot_type(NULL),
  frequency(0.0),
  Ntheta(0),
  theta(NULL),
  Nsd(0),
  tx_depths(NULL),
  Nrd(0),
  rx_depths(NULL),
  Nrr(0),
  rx_ranges(NULL),
  Nrx_per_range(0),
  press_values(NULL)
{

}


int ShdData::getIndex( float value, float* array, int32_t array_size ) const {
  if ( value <= array[0] || array_size == 1 ) {
    return 0;
  }
  else if ( value >= array[array_size - 1] ) {
    return( array_size - 1 );
  }
  else {
    double quantized_value = (value - array[0]) / ( ( array[array_size - 1] - array[0] ) / array_size ) ;
    if ( ( abs( quantized_value ) - floor( abs( quantized_value ) ) ) >= 0.5 ) return( ceil( quantized_value ) );
    else return( floor( quantized_value ) );
  }
}


int ShdData::getPressureIndex( double tx_depth, double rx_depth, double rx_range, double tx_theta ) const {
  int theta_index = getIndex( tx_theta, theta, Ntheta );
  int tx_depth_index = getIndex( tx_depth, tx_depths, Nsd );
  int rx_depth_index = getIndex( rx_depth, rx_depths, Nrx_per_range );
  int rx_range_index = getIndex( rx_range/1000.0, rx_ranges, Nrr );

  return( theta_index * Nsd * Nrx_per_range * Nrr + tx_depth_index * Nrx_per_range * Nrr + rx_depth_index * Nrr + rx_range_index );
}


ShdResReader::ShdResReader() 
:  ResReader(),
   shd_header_collected(false),
   shd_file_collected(false),
   file_reader(),
   shd_file()
{ 

}


ShdResReader::ShdResReader( const Woss* const woss ) 
:  ResReader(woss),
   shd_header_collected(false),
   shd_file_collected(false),
   file_reader(),
   shd_file()
{ 

}


ShdResReader::~ShdResReader()  { 
  
}


bool ShdResReader::initialize() {
  assert(woss_ptr != NULL );
  assert(file_name.size() > 0);

  return( getShdHeader() && getShdFile() );
}


bool ShdResReader::getShdHeader() {
  if (shd_header_collected == true) return true;
  
  file_reader.open(file_name.c_str() , ::std::ios::binary|::std::ios::in);

  if(!file_reader) {
      if (woss_ptr->usingDebug()) ::std::cout << "ShdResReader(" << woss_ptr->getWossId() << ")::getShdHeader() WARNING, results file " << file_name 
				<< " non existant" << ::std::endl;
      return false;
  }

  file_reader.read(reinterpret_cast<char*>(&shd_file.record_length),sizeof(int32_t));

  if (woss_ptr->usingDebug()) ::std::cout << "ShdResReader(" << woss_ptr->getWossId() << ")::getShdHeader() record length = " << shd_file.record_length << ::std::endl;

  file_reader.seekg(4*shd_file.record_length, ::std::ios_base::beg); //reposition to end of first record
  shd_file.plot_type = new char[10*sizeof(char*)];
  file_reader.read(shd_file.plot_type, 10*sizeof(char*));

  if (woss_ptr->usingDebug()) ::std::cout << "ShdResReader(" << woss_ptr->getWossId() << ")::getShdHeader() plot type : " << shd_file.plot_type << ::std::endl;

  file_reader.seekg(2*4*shd_file.record_length, ::std::ios_base::beg); //reposition to end of record 2
  file_reader.read(reinterpret_cast<char*>(&shd_file.frequency),sizeof(float));
  file_reader.read(reinterpret_cast<char*>(&shd_file.Ntheta),sizeof(int32_t));
  file_reader.read(reinterpret_cast<char*>(&shd_file.Nsd),sizeof(int32_t));   
  file_reader.read(reinterpret_cast<char*>(&shd_file.Nrd),sizeof(int32_t)); 
  file_reader.read(reinterpret_cast<char*>(&shd_file.Nrr),sizeof(int32_t)); 

  if (woss_ptr->usingDebug()) ::std::cout << "ShdResReader(" << woss_ptr->getWossId() << ")::getShdHeader() Ntheta = " << shd_file.Ntheta 
				   << "; Nsd = " << shd_file.Nsd << "; Nrd = " << shd_file.Nrd << "; Nrr = " << shd_file.Nrr << ::std::endl;

  shd_file.theta = new float[shd_file.Ntheta];
  shd_file.tx_depths = new float[shd_file.Nsd];
  shd_file.rx_depths = new float[shd_file.Nrd];
  shd_file.rx_ranges = new float[shd_file.Nrr];
  shd_file.press_values = new ::std::complex<double>[(int)(shd_file.Ntheta * shd_file.Nsd * shd_file.Nrd * shd_file.Nrr)];

  file_reader.seekg(3*4*shd_file.record_length, ::std::ios_base::beg); //reposition to end of record 3
  file_reader.read(reinterpret_cast<char*>(shd_file.theta),shd_file.Ntheta*sizeof(float));

  file_reader.seekg(4*4*shd_file.record_length, ::std::ios_base::beg); //reposition to end of record 4
  file_reader.read(reinterpret_cast<char*>(shd_file.tx_depths),shd_file.Nsd*sizeof(float));

  file_reader.seekg(5*4*shd_file.record_length, ::std::ios_base::beg); //reposition to end of record 5
  file_reader.read(reinterpret_cast<char*>(shd_file.rx_depths),shd_file.Nrd*sizeof(float));

  file_reader.seekg(6*4*shd_file.record_length, ::std::ios_base::beg); //reposition to end of record 6
  file_reader.read(reinterpret_cast<char*>(shd_file.rx_ranges),shd_file.Nrr*sizeof(float));

  file_reader.close();

  if (woss_ptr->usingDebug()) {
     for (int i = 0; i < shd_file.Ntheta; i++) {
        ::std::cout << "ShdResReader(" << woss_ptr->getWossId() << ")::getShdHeader() Ntheta(" << i << ") = " << shd_file.theta[i] << ::std::endl;
     }

     for (int i = 0; i < shd_file.Nsd; i++) {
        ::std::cout << "ShdResReader(" << woss_ptr->getWossId() << ")::getShdHeader() Nsd(" << i << ") = " << shd_file.tx_depths[i] << ::std::endl;
     }

     for (int i = 0; i < shd_file.Nrd; i++) {
        ::std::cout << "ShdResReader(" << woss_ptr->getWossId() << ")::getShdHeader() Nrd(" << i << ") = " << shd_file.rx_depths[i] << ::std::endl;
     }

     for (int i = 0; i < shd_file.Nrr; i++) {
        ::std::cout << "ShdResReader(" << woss_ptr->getWossId() << ")::getShdHeader() Nrr(" << i << ") = " << shd_file.rx_ranges[i] << ::std::endl;
     }
  }

  if ( strcmp( shd_file.plot_type, "rectilin  " ) == 0) {
      shd_file.Nrx_per_range = shd_file.Nrd;
  }
  else if ( strcmp( shd_file.plot_type, "irregular " ) == 0) {
      shd_file.Nrx_per_range = 1;
  }
  else {
      shd_file.Nrx_per_range = shd_file.Nrd;
  }

  shd_header_collected = true;

  return shd_header_collected;
}


bool ShdResReader::getShdFile()
{
  file_reader.open(file_name.c_str() , ::std::ios::binary|::std::ios::in);
  
  if(!file_reader) return false;

  const BellhopWoss* bwoss_ptr = dynamic_cast<const BellhopWoss*>(woss_ptr);
  assert( NULL != bwoss_ptr );

  float* press = new float[2*shd_file.Nrr];

  if (woss_ptr->usingDebug())
    ::std::cout << "ShdResReader(" << woss_ptr->getWossId() << ")::getShdFile() syntax " 
                << (int)bwoss_ptr->getBellhopShdSyntax() << ::std::endl;

  for (int itheta = 0; itheta < shd_file.Ntheta; itheta++) {

    for (int isd = 0; isd < shd_file.Nsd; isd++) {

      if (woss_ptr->usingDebug()) 
        ::std::cout << "ShdResReader(" << woss_ptr->getWossId() << ")::getShdFile() Reading data for source " << isd << " of " 
                                       << shd_file.Nsd << ::std::endl;

      for (int ird = 0; ird < shd_file.Nrd; ird++) {

        int recnum = itheta * shd_file.Nsd * shd_file.Nrx_per_range + isd * shd_file.Nrx_per_range + ird;

        if (bwoss_ptr->getBellhopShdSyntax() == BELLHOP_CREATOR_SHD_FILE_SYNTAX_0) {
          recnum += 7;
        }
        else if (bwoss_ptr->getBellhopShdSyntax() == BELLHOP_CREATOR_SHD_FILE_SYNTAX_1) {
          recnum += 10;
        }

        file_reader.seekg( recnum * 4 * shd_file.record_length, ::std::ios_base::beg ); //Move to end of previous record
        if ( !file_reader.good()) {
          ::std::cout << "ShdResReader(" << woss_ptr->getWossId() << ")::getShdFile() Seek to specified record failed in " 
               << file_name << ::std::endl;
          exit(1);
        }
        file_reader.read(reinterpret_cast<char*>(press), 2*shd_file.Nrr*sizeof(float));

        for (int i = 0; i < 2*shd_file.Nrr; i += 2 ) {
          int press_index = itheta * shd_file.Nsd * shd_file.Nrx_per_range * shd_file.Nrr + isd * shd_file.Nrx_per_range * shd_file.Nrr + ird * shd_file.Nrr + (i/2);

          double real_part = press[i];
          double imag_part = press[i+1];

          if ( !::std::isnan(real_part) && !::std::isnan(imag_part) && !::std::isinf(real_part) && !::std::isinf(imag_part) ) {

            shd_file.press_values[press_index] += ::std::complex<double>(real_part, imag_part) ;

            if (woss_ptr->usingDebug()) ::std::cout << "ShdResReader(" << woss_ptr->getWossId() << ")::getShdFile() tx_depth = " << shd_file.tx_depths[isd] 
                                             << "; rx depth = " << shd_file.rx_depths[ird] << "; rx range = " << shd_file.rx_ranges[(i/2)] 
                                             << "; pressure = " << shd_file.press_values[press_index] << "; tx loss db = " 
                                             << Pressure::getTxLossDb( ::std::complex<double>( real_part, imag_part ) ) << ::std::endl;
          }
          else {
            shd_file.press_values[press_index] += ::std::complex<double>( 0.0, 0.0 ) ;
          }
        }
      }
    }
  }
  file_reader.close();

  delete[] press;
  press = NULL;
  
  shd_file_collected = true;
  return shd_file_collected;
}


Pressure* ShdResReader::readAvgPressure( double tx_depth, double start_rx_depth, double start_rx_range, double end_rx_depth, double end_rx_range ) const {
  if (!shd_file_collected) return SDefHandler::instance()->getPressure()->create( Pressure::createNotValid() );
  return( SDefHandler::instance()->getPressure()->create( readMapAvgPressure( tx_depth, start_rx_depth, start_rx_range, end_rx_depth, end_rx_range ) ) ); 
}


Pressure* ShdResReader::readPressure( double tx_depth, double rx_depth, double rx_range ) const {
  if (!shd_file_collected) return SDefHandler::instance()->getPressure()->create( Pressure::createNotValid() );
  return( SDefHandler::instance()->getPressure()->create( accessMap(tx_depth, rx_depth, rx_range) ) );
}


TimeArr* ShdResReader::readTimeArr( double tx_depth, double rx_depth, double rx_range ) const {
  if (!shd_file_collected ) return SDefHandler::instance()->getTimeArr()->create( Pressure::createNotValid() );
  return( SDefHandler::instance()->getTimeArr()->create( accessMap( tx_depth, rx_depth, rx_range) ) ); 
} 


::std::complex<double> ShdResReader::readMapAvgPressure( double tx_depth, double start_rx_depth, double start_rx_range, double end_rx_depth, double end_rx_range, double theta ) const {

  static double last_tx_depth = SHD_RES_NOT_SET;
  static double last_start_rx_depth = SHD_RES_NOT_SET;
  static double last_start_rx_range = SHD_RES_NOT_SET;
  static double last_end_rx_depth = SHD_RES_NOT_SET;
  static double last_end_rx_range = SHD_RES_NOT_SET;
  static ::std::complex<double> last_ret_value = Pressure::createNotValid();

  if ( ( last_tx_depth == tx_depth ) && ( last_start_rx_depth == start_rx_depth ) && ( last_start_rx_range == start_rx_range ) 
   &&  ( last_end_rx_depth == end_rx_depth ) && ( last_end_rx_range == end_rx_range ) )
     return last_ret_value;
  else {
     ::std::complex<double> sum_press ( 0.0 , 0.0 );
     double sum_cnt = 0;
  
     int start_index = shd_file.getPressureIndex( tx_depth, start_rx_depth, start_rx_range, theta );
     int end_index = shd_file.getPressureIndex( tx_depth, end_rx_depth, end_rx_range, theta );

     for ( int i = start_index; i <= end_index; i++ ) {
       sum_press += shd_file.press_values[i];
       sum_cnt++;
     }
     
     last_tx_depth = tx_depth;
     last_start_rx_range = start_rx_range;
     last_end_rx_range = end_rx_range;
     last_start_rx_depth = start_rx_depth;
     last_end_rx_depth = end_rx_depth;

     if (woss_ptr->usingDebug()) ::std::cout << "ShdResReader(" << woss_ptr->getWossId() << ")::getMapAvgPressure() sd = " << tx_depth 
                                      << "; rd_s = " << start_rx_depth << "; rd_e = " << end_rx_depth << "; rr_s = " << start_rx_range << "; rr_e = " << end_rx_range  
                                      << "; final tx loss avg = " << sum_press / sum_cnt << ::std::endl;

     last_ret_value = sum_press / sum_cnt;

     return(last_ret_value);
  }
}

