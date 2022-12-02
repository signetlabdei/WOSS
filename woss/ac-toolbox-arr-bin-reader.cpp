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
 * @file   ac-toolbox-arr-bin-reader.cpp
 * @author Federico Guerra
 *
 * \brief Provides the implementation of woss::ArrBinResReader class
 *
 * Provides the implementation of the woss::ArrBinResReader class
 */


#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include "bellhop-woss.h"
#include <definitions-handler.h>
#include "ac-toolbox-arr-bin-reader.h"


#define ARR_BIN_RES_NOT_SET (-4000)


using namespace woss;


ArrBinResReader::ArrBinResReader()
:  ResReader(),
   arr_bin_header_collected(false),
   arr_bin_file_collected(false),
   file_reader(),
   skip_header(0),
   arr_file(),
   last_tx_depth(ARR_BIN_RES_NOT_SET),
   last_start_rx_depth(ARR_BIN_RES_NOT_SET),
   last_start_rx_range(ARR_BIN_RES_NOT_SET),
   last_end_rx_depth(ARR_BIN_RES_NOT_SET),
   last_end_rx_range(ARR_BIN_RES_NOT_SET),
   last_ret_value(Pressure::createNotValid())
{

}


ArrBinResReader::ArrBinResReader( const Woss* const woss )
: ResReader(woss),
   arr_bin_header_collected(false),
   arr_bin_file_collected(false),
   file_reader(),
   skip_header(0),
   arr_file(),
   last_tx_depth(ARR_BIN_RES_NOT_SET),
   last_start_rx_depth(ARR_BIN_RES_NOT_SET),
   last_start_rx_range(ARR_BIN_RES_NOT_SET),
   last_end_rx_depth(ARR_BIN_RES_NOT_SET),
   last_end_rx_range(ARR_BIN_RES_NOT_SET),
   last_ret_value(Pressure::createNotValid())
{

}


ArrBinResReader::~ArrBinResReader()  {

}


bool ArrBinResReader::initialize() {
  assert( woss_ptr != NULL );
  assert( file_name.size() > 0 );

  assert( getArrBinHeader() );
  assert( getArrBinFile() );

  return true;
}


bool ArrBinResReader::getArrBinHeader() {
  if (arr_bin_header_collected == true) return true;

  file_reader.open(file_name.c_str() , ::std::ios::binary|::std::ios::in);
  if(!file_reader) {
      if (woss_ptr->usingDebug()) ::std::cout << "ArrBinResReader(" << woss_ptr->getWossId()
                                              << ")::getArrBinHeader() WARNING, can't open " << file_name << ::std::endl;
      return false;
  }

  const BellhopWoss* bwoss_ptr = dynamic_cast<const BellhopWoss*>(woss_ptr);

  assert( NULL != bwoss_ptr );

  if (woss_ptr->usingDebug())
    ::std::cout << "ArrBinResReader(" << woss_ptr->getWossId() << ")::getArrBinHeader() syntax = "
                << (int) bwoss_ptr->getBellhopArrSyntax() << std::endl;

  file_reader.seekg(4, ::std::ios_base::beg);

  if (bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_2)
  {
    char sim_type[4]; // '2D' or '3D'
    file_reader.read(sim_type, 4);

    if (woss_ptr->usingDebug())
      std::cout << "ArrBinResReader(" << woss_ptr->getWossId()
                << ")::getArrBinHeader() sim_type = " << sim_type << std::endl;

    file_reader.ignore(8); // ::std::ios_base::beg;
    file_reader.read(reinterpret_cast<char*>(&arr_file.frequency),sizeof(float));
    file_reader.ignore(8);

    if(strcmp(sim_type, "'2D'") == 0)
    {
      //Read in num sources and source depths
      file_reader.read(reinterpret_cast<char*>(&arr_file.Nsd),sizeof(int32_t));
      arr_file.tx_depths = new float[arr_file.Nsd];
      file_reader.read( reinterpret_cast<char*>(arr_file.tx_depths), arr_file.Nsd * sizeof(float) );
      file_reader.ignore(8);

      //Read in num receivers depths and receiver depths
      file_reader.read(reinterpret_cast<char*>(&arr_file.Nrd),sizeof(int32_t));
      arr_file.rx_depths = new float[arr_file.Nrd];
      file_reader.read( reinterpret_cast<char*>(arr_file.rx_depths), arr_file.Nrd * sizeof(float) );
      file_reader.ignore(8);

      //Read in num receivers ranges and receiver ranges
      file_reader.read(reinterpret_cast<char*>(&arr_file.Nrr),sizeof(int32_t));
      arr_file.rx_ranges = new float[arr_file.Nrr];
      file_reader.read( reinterpret_cast<char*>(arr_file.rx_ranges), arr_file.Nrr * sizeof(float) );
    } else
    {
      ::std::cout << "ArrBinResReader(" << woss_ptr->getWossId()
                  << ")::getArrBinHeader() 3D arrival file provided: 3D case not yet coded!" << ::std::endl;

      arr_bin_header_collected = false;
      return arr_bin_header_collected;
    }
  }
  else
  {
    file_reader.read(reinterpret_cast<char*>(&arr_file.Nsd),sizeof(int32_t));
    file_reader.read(reinterpret_cast<char*>(&arr_file.Nrd),sizeof(int32_t));
    file_reader.read(reinterpret_cast<char*>(&arr_file.Nrr),sizeof(int32_t));

    arr_file.tx_depths = new float[arr_file.Nsd];
    arr_file.rx_depths = new float[arr_file.Nrd];
    arr_file.rx_ranges = new float[arr_file.Nrr];

    file_reader.seekg(8, ::std::ios_base::cur);
    file_reader.read( reinterpret_cast<char*>(arr_file.tx_depths), arr_file.Nsd * sizeof(float) );

    file_reader.seekg(8, ::std::ios_base::cur);
    file_reader.read( reinterpret_cast<char*>(arr_file.rx_depths), arr_file.Nrd * sizeof(float) );

    file_reader.seekg(8, ::std::ios_base::cur);
    file_reader.read( reinterpret_cast<char*>(arr_file.rx_ranges), arr_file.Nrr * sizeof(float) );
  }

  if (woss_ptr->usingDebug())
    ::std::cout << "ArrBinResReader(" << woss_ptr->getWossId() << ")::getArrBinHeader() freq = " << arr_file.frequency << "; Nsd = " << arr_file.Nsd << "; Nrd = "
                                      << arr_file.Nrd << "; Nrr = " << arr_file.Nrr << ::std::endl;

  file_reader.seekg(8, ::std::ios_base::cur);

  if (woss_ptr->usingDebug()) {
    for (int i = 0; i < arr_file.Nsd; i++) {
      ::std::cout << "ArrBinResReader(" << woss_ptr->getWossId() << ")::getArrBinHeader() Nsd(" << i << ") = " << arr_file.tx_depths[i] << ::std::endl;
    }

    for (int i = 0; i < arr_file.Nrd; i++) {
      ::std::cout << "ArrBinResReader(" << woss_ptr->getWossId() << ")::getArrBinHeader() Nrd(" << i << ") = " << arr_file.rx_depths[i] << ::std::endl;
    }

    for (int i = 0; i < arr_file.Nrr; i++) {
      ::std::cout << "ArrBinResReader(" << woss_ptr->getWossId() << ")::getArrBinHeader() Nrr(" << i << ") = " << arr_file.rx_ranges[i] << ::std::endl;
    }
  }

  arr_file.arr_values = SDefHandler::instance()->getTimeArr()->createArray(arr_file.Nsd * arr_file.Nrd * arr_file.Nrr);

  skip_header = file_reader.tellg();
  file_reader.close();

  arr_bin_header_collected = true;
  return arr_bin_header_collected;
}


bool ArrBinResReader::getArrBinFile() {

  if ( !arr_bin_header_collected ) return false;

  assert(skip_header > 0);

  if (woss_ptr->usingDebug())
      ::std::cout << "ArrBinResReader(" << woss_ptr->getWossId() << ")::getArrBinFile() skip_header = "
                  << skip_header << ::std::endl;

  file_reader.open(file_name.c_str() , ::std::ios::binary|::std::ios::in);
  file_reader.seekg(skip_header);

  const BellhopWoss* bwoss_ptr = dynamic_cast<const BellhopWoss*>(woss_ptr);

  for (int isd = 0; isd < arr_file.Nsd ; isd++) {

    int32_t max_arrivals;

    file_reader.read( reinterpret_cast<char*>(&max_arrivals) , sizeof(int32_t));
    file_reader.seekg(8, ::std::ios_base::cur);

    if (woss_ptr->usingDebug())
      ::std::cout << "ArrBinResReader(" << woss_ptr->getWossId() << ")::getArrBinFile() Reading data for source "
                  << isd << " of " << arr_file.Nsd << "; max arrivals = " << max_arrivals << ::std::endl;

    for (int ird = 0; ird < arr_file.Nrd; ird++) {

      for (int irr = 0; irr < arr_file.Nrr; irr++ ) {

        int32_t curr_arrivals;

        file_reader.read( reinterpret_cast<char*>(&curr_arrivals) , sizeof(int32_t));
        file_reader.seekg(8, ::std::ios_base::cur);

        if ( woss_ptr->usingDebug() )
          ::std::cout << "ArrBinResReader(" << woss_ptr->getWossId() << ")::getArrBinFile() curr_arrivals = "
                      << curr_arrivals << ::std::endl;

        if (curr_arrivals > 0) {

          float curr_amplitude;
          float curr_phase;
          float curr_delay;
          float curr_delay_imag;
          float curr_src_angle;
          float curr_rx_angle;
          float curr_top_bounces;
          float curr_bottom_bounces;
          Pressure* curr_pressure_ptr = NULL;

          for (int i = 0; i < curr_arrivals; i++) {

            file_reader.read( reinterpret_cast<char*>(&curr_amplitude), sizeof(float) );
            file_reader.read( reinterpret_cast<char*>(&curr_phase), sizeof(float) );
            file_reader.read( reinterpret_cast<char*>(&curr_delay), sizeof(float) );
            if (bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_1 ||
                bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_2)
              file_reader.read( reinterpret_cast<char*>(&curr_delay_imag), sizeof(float) );
            file_reader.read( reinterpret_cast<char*>(&curr_src_angle), sizeof(float) );
            file_reader.read( reinterpret_cast<char*>(&curr_rx_angle), sizeof(float) );
            file_reader.read( reinterpret_cast<char*>(&curr_top_bounces), sizeof(float) );
            file_reader.read( reinterpret_cast<char*>(&curr_bottom_bounces), sizeof(float) );
            file_reader.seekg(2*sizeof(float), ::std::ios_base::cur);

            assert(curr_amplitude >= 0.0);

            if ( woss_ptr->usingDebug() ) {
              assert(!::std::isnan(curr_amplitude));      assert(!::std::isinf(curr_amplitude));
              assert(!::std::isnan(curr_phase));          assert(!::std::isinf(curr_phase));
              assert(!::std::isnan(curr_delay));          assert(!::std::isinf(curr_delay));       //assert(curr_delay >= 0.0);
              if (bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_1 || bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_2) {
                assert(!::std::isnan(curr_delay_imag));   assert(!::std::isinf(curr_delay_imag));  //assert(curr_delay_imag >= 0.0);
              }
              assert(!::std::isnan(curr_src_angle));      assert(!::std::isinf(curr_src_angle));
              assert(!::std::isnan(curr_rx_angle));       assert(!::std::isinf(curr_rx_angle));
              assert(!::std::isnan(curr_top_bounces));    assert(!::std::isinf(curr_top_bounces)); assert(curr_top_bounces >= 0.0);
              assert(!::std::isnan(curr_bottom_bounces)); assert(!::std::isinf(curr_top_bounces)); assert(curr_top_bounces >= 0.0);
            }

            // GLITCH RECOVERY FROM VERTICAL CHANNEL SIMULATIONS ( HORIZ RANGE == 0, ONLY VERTICAL DEPTH )
            if ( curr_delay <= 0.0 || arr_file.rx_ranges[irr] <= 0.0 )
              curr_delay = ( abs( arr_file.rx_depths[ird] - arr_file.tx_depths[isd] ) ) / 1500.0;
            if ( curr_delay <= 0.0 ) curr_delay = abs( curr_delay );

            if (bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_0) {
              curr_pressure_ptr = new Pressure( (curr_amplitude * cos(2.0 * M_PI * arr_file.frequency + curr_phase * M_PI / 180.0) ) ,
                                                (curr_amplitude * sin(2.0 * M_PI * arr_file.frequency + curr_phase * M_PI / 180.0) ) );
            }
            else if (bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_1 ||
                     bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_2) {
              curr_pressure_ptr = new Pressure( (curr_amplitude * ::std::exp(2.0 * M_PI * arr_file.frequency * curr_delay_imag)
                                                  * cos(2.0 * M_PI * arr_file.frequency + curr_phase * M_PI / 180.0) ) ,
                                                (-1.0 * curr_amplitude * ::std::exp(2.0 * M_PI * arr_file.frequency * curr_delay_imag)
                                                  * sin(2.0 * M_PI * arr_file.frequency + curr_phase * M_PI / 180.0) ) );
            }
            else
            {
              // syntax must be defined
              assert(0);
            }

            arr_file.arr_values[isd * arr_file.Nrd * arr_file.Nrr + ird * arr_file.Nrr + irr].sumValue( curr_delay,  *curr_pressure_ptr);

            if (woss_ptr->usingDebug())
              ::std::cout << "ArrBinResReader(" << woss_ptr->getWossId() << ")::getArrBinFile() s_depth = " << arr_file.tx_depths[isd]
                          << "; rx depth = " << arr_file.rx_depths[ird] << "; rx range = " << arr_file.rx_ranges[irr]
                          << " freq = " << arr_file.frequency << "; delay = " << curr_delay
                          << "; delay_imag = " << (bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_1 ? curr_delay_imag : 0.0)
                          << "; amplitude = " << curr_amplitude << "; phase = " << curr_phase
                          << "; exp = " << (bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_1 ? ::std::exp(-2.0 * M_PI * arr_file.frequency * curr_delay_imag) : 1.0)
                          << "; Press = " << *curr_pressure_ptr
                          << "; tx loss db = " << Pressure::getTxLossDb(*curr_pressure_ptr)
                          << "; src angle = " << curr_src_angle << "; rx angle = " << curr_rx_angle
                          << "; top bounces = " << curr_top_bounces << "; bottom bounces = "
                          << curr_bottom_bounces << ::std::endl;

            delete curr_pressure_ptr;
          }
        }
        else {
          arr_file.arr_values[isd * arr_file.Nrd * arr_file.Nrr + ird * arr_file.Nrr + irr].sumValue( 0.0 , Pressure( 0.0, 0.0 ) ) ;

          if ( woss_ptr->usingDebug() )
            ::std::cout << "ArrBinResReader(" << woss_ptr->getWossId() << ")::getArrBinFile() no arrivals, inserted zero value"
                        << ::std::endl;

        }
      }
    }
  }
  file_reader.close();

  arr_bin_file_collected = true;
  return arr_bin_file_collected;
}


Pressure* ArrBinResReader::readPressure( double tx_depth, double rx_depth, double rx_range ) const {
  return( SDefHandler::instance()->getPressure()->create( *readTimeArr( tx_depth, rx_depth, rx_range ) ) );
}


Pressure* ArrBinResReader::readAvgPressure( double tx_depth, double start_rx_depth, double start_rx_range, double end_rx_depth, double end_rx_range ) {
  if ( !arr_bin_file_collected ) return SDefHandler::instance()->getPressure()->create( Pressure::createNotValid() );
  return( SDefHandler::instance()->getPressure()->create( readMapAvgPressure( tx_depth, start_rx_depth, start_rx_range, end_rx_depth, end_rx_range ) ) );
}


TimeArr* ArrBinResReader::readTimeArr(double source_depth, double rx_depth, double rx_range) const {
  if ( !arr_bin_file_collected ) SDefHandler::instance()->getTimeArr()->create( TimeArr::createNotValid() );
  return( SDefHandler::instance()->getTimeArr()->create( *accessMap( source_depth,rx_depth,rx_range ) ) );
}


::std::complex<double> ArrBinResReader::readMapAvgPressure( double tx_depth, double start_rx_depth, double start_rx_range, double end_rx_depth, double end_rx_range ) {
  if ( ( last_tx_depth == tx_depth ) && ( last_start_rx_depth == start_rx_depth ) && ( last_start_rx_range == start_rx_range )
   &&  ( last_end_rx_depth == end_rx_depth ) && ( last_end_rx_range == end_rx_range ) )
     return last_ret_value;
  else {
     ::std::complex<double> sum_press;
     double sum_cnt = 0;

     int start_index = arr_file.getTimeArrIndex( tx_depth, start_rx_depth, start_rx_range );
     int end_index = arr_file.getTimeArrIndex( tx_depth, end_rx_depth, end_rx_range );

     for ( int i = start_index; i <= end_index; i++ ) {
       ::std::complex<double> curr_press = arr_file.arr_values[i];
       sum_press += curr_press;
       sum_cnt++;
     }

     last_tx_depth = tx_depth;
     last_start_rx_range = start_rx_range;
     last_end_rx_range = end_rx_range;
     last_start_rx_depth = start_rx_depth;
     last_end_rx_depth = end_rx_depth;

     if (woss_ptr->usingDebug()) ::std::cout << "ArrBinResReader(" << woss_ptr->getWossId() << ")::getMapAvgPressure() sd = " << tx_depth
                                      << "; rd_s = " << start_rx_depth << "; rd_e = " << end_rx_depth << "; rr_s = " << start_rx_range << "; rr_e = " << end_rx_range
                                      << "; final tx loss avg = " << sum_press / sum_cnt << ::std::endl;

     last_ret_value = sum_press/sum_cnt;

     return(last_ret_value);
  }
}

