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
 * @file   ac-toolbox-arr-asc-reader.cpp
 * @author Federico Guerra
 *
 * \brief Provides the implementation of woss::ArrAscResReader and woss::ArrData classes
 *
 * Provides the implementation of the woss::ArrAscResReader and woss::ArrData classes
 */

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <definitions.h>
#include "ac-toolbox-arr-asc-reader.h"
#include "bellhop-woss.h"


#define ARR_ASC_RES_NOT_SET (-4000)
#include <definitions-handler.h>

using namespace woss;


ArrData::ArrData()
: frequency(0.0),
  Nsd(0),
  tx_depths(NULL),
  Nrd(0),
  rx_depths(NULL),
  Nrr(0),
  rx_ranges(NULL),
  arr_values(NULL)
{

}


int ArrData::getIndex( float value, float* array, int array_size ) const {

//   ::std::cout << "ArrData::getIndex() value = " << value << "; array size = " << array_size << ::std::endl;

  if ( value <= array[0] || array_size == 1 ) {
    return 0;
  }
  else if ( value >= array[array_size - 1] ) {
    return( array_size - 1 );
  }
  else {
    double quantized_value = (value - array[0]) / ( ( array[array_size - 1] - array[0] ) / array_size ) ;

//     ::std::cout << "ArrData::getIndex() quantized value = " << quantized_value
//                 << "; array[ end ] = " <<  ( array[array_size - 1] ) << "; array[ start ] = " << (array[0])
//                 << "; array[ end ] - array[ start ] = " << ( array[array_size - 1] - array[0] ) << ::std::endl;

    if ( ( abs( quantized_value ) - floor( abs( quantized_value ) ) ) >= 0.5 ) return( ceil( quantized_value ) );
    else return( floor( quantized_value ) );
  }
}


int ArrData::getTimeArrIndex( double tx_depth, double rx_depth, double rx_range ) const {
  int tx_depth_index = getIndex( tx_depth, tx_depths, Nsd );
  int rx_depth_index = getIndex( rx_depth, rx_depths, Nrd );
  int rx_range_index = getIndex( rx_range, rx_ranges, Nrr );

//   ::std::cout << "ArrData::getTimeArrIndex() tx depth = " << tx_depth
//               << "; rx depth = " << rx_depth << "; rx range = " << rx_range
//               << "; tx depth index = " << tx_depth_index << "; rx depth index = " << rx_depth_index
//               << "; rx range index = " << rx_range_index << "; final index = " <<  tx_depth_index * Nrd * Nrr + rx_depth_index * Nrr + rx_range_index
//               << "; max index = " << Nsd * Nrd * Nrr << ::std::endl;

  return( tx_depth_index * Nrd * Nrr + rx_depth_index * Nrr + rx_range_index );
}


ArrAscResReader::ArrAscResReader()
: ResReader(),
  arr_asc_header_collected(false),
  arr_asc_file_collected(false),
  file_reader(),
  skip_header(0),
  arr_file(),
  last_tx_depth(ARR_ASC_RES_NOT_SET),
  last_start_rx_depth(ARR_ASC_RES_NOT_SET),
  last_start_rx_range(ARR_ASC_RES_NOT_SET),
  last_end_rx_depth(ARR_ASC_RES_NOT_SET),
  last_end_rx_range(ARR_ASC_RES_NOT_SET),
  last_ret_value(Pressure::createNotValid())
{

}


ArrAscResReader::ArrAscResReader( const Woss* const woss )
: ResReader(woss),
  arr_asc_header_collected(false),
  arr_asc_file_collected(false),
  file_reader(),
  skip_header(0),
  arr_file(),
  last_tx_depth(ARR_ASC_RES_NOT_SET),
  last_start_rx_depth(ARR_ASC_RES_NOT_SET),
  last_start_rx_range(ARR_ASC_RES_NOT_SET),
  last_end_rx_depth(ARR_ASC_RES_NOT_SET),
  last_end_rx_range(ARR_ASC_RES_NOT_SET),
  last_ret_value(Pressure::createNotValid())
{

}


ArrAscResReader::~ArrAscResReader()  {

}


bool ArrAscResReader::initialize() {
  assert( woss_ptr != NULL );
  assert( file_name.size() > 0 );

  return( getArrAscHeader() && getArrAscFile() );
}


bool ArrAscResReader::getArrAscHeader() {
  if ( arr_asc_header_collected == true ) return arr_asc_header_collected;

  file_reader.open(file_name.c_str() , ::std::ios::in);

  if(!file_reader) {
    if (woss_ptr->usingDebug()) ::std::cout << "ArrAscResReader(" << woss_ptr->getWossId() << ")::getArrAscHeader() WARNING, can't open " << file_name << ::std::endl;
    return false;
  }

  file_reader.precision(WOSS_DECIMAL_PRECISION);
  ::std::cout.precision(WOSS_DECIMAL_PRECISION);

  const BellhopWoss* bwoss_ptr = dynamic_cast<const BellhopWoss*>(woss_ptr);

  assert( NULL != bwoss_ptr );

  if (woss_ptr->usingDebug())
    ::std::cout << "ArrAscResReader(" << woss_ptr->getWossId() << ")::getArrAscHeader() syntax = "
                << (int) bwoss_ptr->getBellhopArrSyntax() << std::endl;

  if (bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_2)
  {
    std::string sim_type; // '2D' or '3D'
    file_reader >> sim_type;

    if(sim_type.compare("'2D'") == 0)
    {
      file_reader >> arr_file.frequency;

      //Read in num sources and source depths
      file_reader >> arr_file.Nsd;
      arr_file.tx_depths = new float[arr_file.Nsd];

      for (int i = 0; i < arr_file.Nsd; i++) {
          file_reader >> arr_file.tx_depths[i];

          if (woss_ptr->usingDebug()) ::std::cout << "ArrAscResReader(" << woss_ptr->getWossId()
                                                  << ")::getArrAscHeader() Reading source depth(" << i << "/"
                                                  << i << ") = " << arr_file.tx_depths[i] << ::std::endl;
      }

      //Read in num receivers depths and receiver depths
      file_reader >> arr_file.Nrd;
      arr_file.rx_depths = new float[arr_file.Nrd];

      for (int i = 0; i < arr_file.Nrd; i++) {
        file_reader >> arr_file.rx_depths[i];

        if (woss_ptr->usingDebug()) ::std::cout << "ArrAscResReader(" << woss_ptr->getWossId() << ")::getArrAscHeader() Reading rx depth(" << i << "/"
                                                << i << ") = " << arr_file.rx_depths[i] << ::std::endl;
      }

      //Read in num receivers ranges and receiver ranges
      file_reader >> arr_file.Nrr;
      arr_file.rx_ranges = new float[arr_file.Nrr];

      for (int i = 0; i < arr_file.Nrr; i++) {
        file_reader >> arr_file.rx_ranges[i];

        if (woss_ptr->usingDebug()) ::std::cout << "ArrAscResReader(" << woss_ptr->getWossId() << ")::getArrAscHeader() Reading rx range(" << i << "/"
                                                << i << ") = " << arr_file.rx_ranges[i] << " [m]" << ::std::endl;
      }
    }
    else
    {
      ::std::cout << "ArrAscResReader(" << woss_ptr->getWossId()
                  << ")::getArrAscHeader() 3D arrival file provided: 3D case not yet coded!" << ::std::endl;

      arr_asc_header_collected = false;
      return arr_asc_header_collected;
    }
  }
  else
  {
    file_reader >> arr_file.frequency;
    file_reader >> arr_file.Nsd;
    file_reader >> arr_file.Nrd;
    file_reader >> arr_file.Nrr;


    arr_file.tx_depths = new float[arr_file.Nsd];
    arr_file.rx_depths = new float[arr_file.Nrd];
    arr_file.rx_ranges = new float[arr_file.Nrr];
//   arr_file.arr_values = new TimeArr*[arr_file.Nsd*arr_file.Nrd*arr_file.Nrr];

    if (woss_ptr->usingDebug()) ::std::cout << "ArrAscResReader(" << woss_ptr->getWossId() << ")::getArrAscHeader() freq = " << arr_file.frequency << "; Nsd = " << arr_file.Nsd
                    << "; Nrd = " << arr_file.Nrd << "; Nrr = " << arr_file.Nrr << ::std::endl;

    for (int i = 0; i < arr_file.Nsd; i++) {
       file_reader >> arr_file.tx_depths[i];

       if (woss_ptr->usingDebug()) ::std::cout << "ArrAscResReader(" << woss_ptr->getWossId() << ")::getAsciiArrMap() Reading source depth(" << i << "/"
                                        << i << ") = " << arr_file.tx_depths[i] << ::std::endl;
    }

    for (int i = 0; i < arr_file.Nrd; i++) {
       file_reader >> arr_file.rx_depths[i];

       if (woss_ptr->usingDebug()) ::std::cout << "ArrAscResReader(" << woss_ptr->getWossId() << ")::getAsciiArrMap() Reading rx depth(" << i << "/"
                                        << i << ") = " << arr_file.rx_depths[i] << ::std::endl;
    }

    for (int i = 0; i < arr_file.Nrr; i++) {
       file_reader >> arr_file.rx_ranges[i];

       if (woss_ptr->usingDebug()) ::std::cout << "ArrAscResReader(" << woss_ptr->getWossId() << ")::getAsciiArrMap() Reading rx range(" << i << "/"
                                        << i << ") = " << arr_file.rx_ranges[i] << " [m]" << ::std::endl;
    }
  }

  arr_file.arr_values = SDefHandler::instance()->getTimeArr()->createArray( arr_file.Nsd*arr_file.Nrd*arr_file.Nrr );

  skip_header = file_reader.tellg();
  file_reader.close();

  arr_asc_header_collected = true;
  return arr_asc_header_collected;
}


bool ArrAscResReader::getArrAscFile() {
  if ( !arr_asc_header_collected ) return false;

  assert(skip_header > 0);
  file_reader.open(file_name.c_str() , ::std::ios::in);
  file_reader.seekg(skip_header);

  file_reader.precision(WOSS_DECIMAL_PRECISION);
  ::std::cout.precision(WOSS_DECIMAL_PRECISION);

  const BellhopWoss* bwoss_ptr = dynamic_cast<const BellhopWoss*>(woss_ptr);

  for (int isd = 0; isd < arr_file.Nsd ; isd++) {

    int max_arrivals = -1;

    file_reader >> max_arrivals;

    if (woss_ptr->usingDebug())
      ::std::cout << "ArrAscResReader(" << woss_ptr->getWossId() << ")::getAsciiArrMap() Reading data for source " << isd << " of "
                  << isd << "; max arrivals = " << max_arrivals << ::std::endl;

    for (int ird = 0; ird < arr_file.Nrd; ird++) {

      for (int irr = 0; irr < arr_file.Nrr; irr++ ) {

        int curr_arrivals = -1;

        file_reader >> curr_arrivals;

        if ( woss_ptr->usingDebug() )
          ::std::cout << "ArrAscResReader(" << woss_ptr->getWossId() << ")::getAsciiArrMap() curr_arrivals = "
                      << curr_arrivals << ::std::endl;

        if (curr_arrivals > 0) {

          double curr_amplitude = ARR_ASC_RES_NOT_SET;
          double curr_phase = ARR_ASC_RES_NOT_SET;
          double curr_delay = ARR_ASC_RES_NOT_SET;
          double curr_delay_imag = ARR_ASC_RES_NOT_SET;
          double curr_src_angle = ARR_ASC_RES_NOT_SET;
          double curr_rx_angle = ARR_ASC_RES_NOT_SET;
          double curr_top_bounces = ARR_ASC_RES_NOT_SET;
          double curr_bottom_bounces = ARR_ASC_RES_NOT_SET;
          Pressure* curr_pressure_ptr = NULL;

          for (int i = 0; i < curr_arrivals; i++) {

            file_reader >> curr_amplitude;
            file_reader >> curr_phase;
            file_reader >> curr_delay;

            if (bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_1 ||
                bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_2) {
              file_reader >> curr_delay_imag;
            }

            file_reader >> curr_src_angle;
            file_reader >> curr_rx_angle;
            file_reader >> curr_top_bounces;
            file_reader >> curr_bottom_bounces;

            assert(curr_amplitude >= 0.0);

            if ( woss_ptr->usingDebug() ) {
              assert(!::std::isnan(curr_amplitude));      assert(!::std::isinf(curr_amplitude));
              assert(!::std::isnan(curr_phase));          assert(!::std::isinf(curr_phase));
              assert(!::std::isnan(curr_delay));          assert(!::std::isinf(curr_delay));       //assert(curr_delay >= 0.0);
              if (bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_1 
                 || bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_2) {
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
                     bwoss_ptr->getBellhopArrSyntax() == BELLHOP_CREATOR_ARR_FILE_SYNTAX_2 ) {
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
              ::std::cout << "ArrAscResReader(" << woss_ptr->getWossId() << ")::getAsciiArrMap() s_depth = " << arr_file.tx_depths[isd]
                          << "; rx depth = " << arr_file.rx_depths[ird] << "; rx range = " << arr_file.rx_ranges[irr]
                          << " freq = " << arr_file.frequency << "; delay = " << curr_delay << "; delay_imag = " << curr_delay_imag
                          << "; amplitude = " << curr_amplitude << "; phase = " << curr_phase
                          << "; exp = " << (bwoss_ptr->getBellhopArrSyntax() != BELLHOP_CREATOR_ARR_FILE_SYNTAX_0 ? ::std::exp(-2.0 * M_PI * arr_file.frequency * curr_delay_imag) : 1.0)
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
            ::std::cout << "ArrAscResReader(" << woss_ptr->getWossId() << ")::getAsciiArrMap() no arrivals, inserted zero value"
                        << ::std::endl;

        }
      }
    }
  }
  file_reader.close();
  arr_asc_file_collected = true;
  return arr_asc_file_collected;
}


Pressure* ArrAscResReader::readPressure(double frequency, double tx_depth, double rx_depth, double rx_range ) const {
  return( SDefHandler::instance()->getPressure()->create( *readTimeArr( frequency, tx_depth, rx_depth, rx_range ) ) );
}


Pressure* ArrAscResReader::readAvgPressure( double frequency, double tx_depth, double start_rx_depth, double start_rx_range, double end_rx_depth, double end_rx_range ) {
  if ( !arr_asc_file_collected ) return SDefHandler::instance()->getPressure()->create( Pressure::createNotValid() );
  return( SDefHandler::instance()->getPressure()->create( readMapAvgPressure( frequency, tx_depth, start_rx_depth, start_rx_range, end_rx_depth, end_rx_range ) ) );
}


TimeArr* ArrAscResReader::readTimeArr( double frequency, double source_depth, double rx_depth, double rx_range ) const {
  if ( !arr_asc_file_collected ) return( SDefHandler::instance()->getTimeArr()->create( TimeArr::createNotValid() ) );
  return( SDefHandler::instance()->getTimeArr()->create( *accessMap(frequency, source_depth,rx_depth,rx_range ) ) );
}


::std::complex<double> ArrAscResReader::readMapAvgPressure( double frequency, double tx_depth, double start_rx_depth, double start_rx_range, double end_rx_depth, double end_rx_range ) {
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

     if (woss_ptr->usingDebug()) ::std::cout << "ArrAscResReader(" << woss_ptr->getWossId() << ")::getMapAvgPressure() sd = " << tx_depth
                                      << "; rd_s = " << start_rx_depth << "; rd_e = " << end_rx_depth << "; rr_s = " << start_rx_range << "; rr_e = " << end_rx_range
                                      << "; final tx loss avg = " << sum_press / sum_cnt << ::std::endl;

     last_ret_value = sum_press/sum_cnt;

     return(last_ret_value);
  }
}
