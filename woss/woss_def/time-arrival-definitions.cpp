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
 * @file   time-arrival-definitions.cpp
 * @author Federico Guerra
 *
 * \brief  Implementations and library for woss::TimeArr class 
 *
 * Implementations and library for woss::TimeArr class
 */


#include <iterator>
#include "time-arrival-definitions.h"


using namespace woss;


bool TimeArr::debug = false;


TimeArr::TimeArr( long double custom_delay_prec )
: delay_precision(custom_delay_prec),
  time_arr_map()
{

}


TimeArr::TimeArr( const TimeArr& copy ) {
  delay_precision = copy.delay_precision;
  time_arr_map = copy.time_arr_map;
}


TimeArr::TimeArr( TimeArrMap& map, long double custom_delay_prec )
: delay_precision(custom_delay_prec),
  time_arr_map()
{
  time_arr_map.swap(map);
}


TimeArr::TimeArr( const Pressure& pressure, double delay, long double custom_delay_prec ) 
: delay_precision(custom_delay_prec),
  time_arr_map()
{
  if ( !pressure.isValid() ) time_arr_map[0.0] = Pressure::createNotValid();
  else time_arr_map.insert( std::make_pair( delay, pressure ) );
}


TimeArr::operator std::complex<double>() const { 
  std::complex<double> value;
  for ( TimeArrCIt it = time_arr_map.begin(); it != time_arr_map.end(); it++) {
    value += it->second;
  }
  return value;
}


bool TimeArr::isValid() const { 
  if ( time_arr_map.size() < 1 ) return false; 
  if ( time_arr_map.find(0.0) != time_arr_map.end() && time_arr_map.find(0.0)->second == Pressure::createNotValid() ) return false; 
  return true;
}


TimeArr& TimeArr::operator=( const TimeArr& copy ) {
  if (this == &copy) return *this;
  delay_precision = copy.delay_precision;
  time_arr_map = copy.time_arr_map;
  return( *this );
}


TimeArrCIt TimeArr::at( const int position ) const {
  if ( position >= (int) time_arr_map.size() || position < 0 ) return time_arr_map.end();
  if ( position == 0 ) return time_arr_map.begin();

  TimeArrCIt ret_val = time_arr_map.begin();
  ::std::advance( ret_val, position );

  return ret_val;
}


TimeArrCIt TimeArr::lowerBoundTxLoss( double threshold_db ) const {
	TimeArrCIt it;
	
	for ( it = time_arr_map.begin(); it != time_arr_map.end(); it++ ) {
    if ( Pressure::getTxLossDb(it->second) <= threshold_db ) {
			
// 			std::cout << "TimeArr::lowerBoundTxLoss() threshold_db curr tx loss val "
// 			          << Pressure::getTxLossDb(it->second) << " <=  threshold_db " << threshold_db 
// 			          << "; time " << it->first << ::std::endl;
								
			return it;
		}
  }
  
  return time_arr_map.end();
}


// keeps the last value of each overridden PDouble
woss::TimeArr& TimeArr::setDelayPrecision( long double precision ) {
  TimeArrMap temp_map;
  
  for ( TimeArrIt it = time_arr_map.begin(); it != time_arr_map.end(); it++ ) {
    temp_map[ PDouble( it->first.getValue(), precision) ] = it->second;
  }
  time_arr_map.swap(temp_map);
  
  return *this;
}


bool TimeArr::checkPressureAttenuation( double distance, double frequency ) {
  bool ret_val = false;
  
  for ( TimeArrIt it = time_arr_map.begin(); it != time_arr_map.end(); it++) {
    Pressure temp = Pressure(it->second);
    ret_val = ret_val || temp.checkAttenuation( distance, frequency ) ;
    it->second = temp;
  }
  
  return ret_val;
}


TimeArr* TimeArr::coherentSumSample( double time_resolution ) {
  TimeArrMap temp_map;
  
  PDouble curr_ch_time = time_arr_map.begin()->first;
  for ( TimeArrIt it = time_arr_map.begin(); it != time_arr_map.end(); it++) {
    if ( it->first > (curr_ch_time + PDouble(time_resolution, delay_precision) ) ) curr_ch_time = it->first;
    temp_map[curr_ch_time] += it->second;
  }
  return( create(temp_map) );
}
  

TimeArr* TimeArr::incoherentSumSample( double time_resolution ) {
  TimeArrMap temp_map;
  
  PDouble curr_ch_time = time_arr_map.begin()->first;
  for ( TimeArrIt it = time_arr_map.begin(); it != time_arr_map.end(); it++) {
    if ( it->first > (curr_ch_time + PDouble(time_resolution, delay_precision) ) ) curr_ch_time = it->first;
    temp_map[curr_ch_time] += pow( abs( it->second ), 2.0 );
  }
  
  for ( TimeArrIt it = temp_map.begin(); it != temp_map.end(); it++) {
    temp_map[it->first] = sqrt( it->second ) ;
  }
  return( create(temp_map) );
}


TimeArr* TimeArr::crop( double time_start, double time_end ) {
	TimeArrMap temp_map;
	
// 	int i = 0;
  for ( TimeArrIt it = time_arr_map.begin(); it != time_arr_map.end(); it++) {
    if ( it->first >= PDouble(time_start, delay_precision) 
			&& it->first < PDouble(time_end, delay_precision) ) {
				
// 			std::cout << "TimeArr::crop() index " << i << "; time_start " << time_start << "; time_end " << time_end 
// 			          << "; time value " << it->first << "; att " << it->second << std::endl;

			temp_map[it->first] = it->second;
		}
// 		++i;
	}
  return( create(temp_map) );	
}

  
const TimeArr woss::operator+( const TimeArr& left, const TimeArr& right ) { 
  TimeArr ret_val ( left );
  ret_val += right;
  return ret_val;
}


const TimeArr woss::operator-( const TimeArr& left, const TimeArr& right ) { 
  TimeArr ret_val ( left );
  ret_val -= right;
  return ret_val;
}


const TimeArr woss::operator+( const TimeArr& left, const double right ) { 
  TimeArr ret_val ( left );
  ret_val += right;
  return ret_val;
}


const TimeArr woss::operator-( const TimeArr& left, const double right ) { 
  TimeArr ret_val ( left );
  ret_val -= right;
  return ret_val;
}


const TimeArr woss::operator/( const TimeArr& left, const double right ) { 
  TimeArr ret_val ( left );
  ret_val /= right;
  return ret_val;
}


const TimeArr woss::operator*( const TimeArr& left, const double right ) { 
  TimeArr ret_val ( left );
  ret_val *= right;
  return ret_val;
}


const TimeArr woss::operator+( const double left, const TimeArr& right ) { 
  TimeArr ret_val ( right );
  ret_val += left;
  return ret_val;
}


const TimeArr woss::operator-( const double left, const TimeArr& right ) { 
  TimeArr ret_val ( right );
  ret_val -= left;
  return ret_val;
}


const TimeArr woss::operator/( const double left, const TimeArr& right ) { 
  TimeArr ret_val ( right );
  ret_val /= left;
  return ret_val;
}


const TimeArr woss::operator*( const double left, const TimeArr& right ) { 
  TimeArr ret_val ( right );
  ret_val *= left;
  return ret_val;
}


TimeArr& woss::operator+=( TimeArr& left, const TimeArr& right ) { 
  for ( TimeArrCIt it = right.time_arr_map.begin(); it != right.time_arr_map.end(); it++ ) {
    left.time_arr_map[it->first] += it->second;
  }
  return left;
}


TimeArr& woss::operator-=( TimeArr& left, const TimeArr& right ) { 
  for ( TimeArrCIt it = right.time_arr_map.begin(); it != right.time_arr_map.end(); it++ ) {
    left.time_arr_map[it->first] -= it->second;
  }
  return left;
}


TimeArr& woss::operator+=( TimeArr& left, double right ) { 
  for ( TimeArrCIt it = left.time_arr_map.begin(); it != left.time_arr_map.end(); it++ ) {
    left.time_arr_map[it->first] += right;
  }
  return left;
}


TimeArr& woss::operator-=( TimeArr& left, double right ) { 
  for ( TimeArrCIt it = left.time_arr_map.begin(); it != left.time_arr_map.end(); it++ ) {
    left.time_arr_map[it->first] -= right;
  }
  return left;
}


TimeArr& woss::operator/=( TimeArr& left, double right ) { 
  for ( TimeArrCIt it = left.time_arr_map.begin(); it != left.time_arr_map.end(); it++ ) {
    left.time_arr_map[it->first] /= right;
  }
  return left;
}


TimeArr& woss::operator*=( TimeArr& left, double right ) { 
  for ( TimeArrCIt it = left.time_arr_map.begin(); it != left.time_arr_map.end(); it++ ) {
    left.time_arr_map[it->first] *= right;
  }
  return left;
}



