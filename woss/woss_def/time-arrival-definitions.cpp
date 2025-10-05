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
 * @file   time-arrival-definitions.cpp
 * @author Federico Guerra
 *
 * \brief  Implementations and library for woss::TimeArr class 
 *
 * Implementations and library for woss::TimeArr class
 */


#include "time-arrival-definitions.h"


using namespace woss;


bool TimeArr::debug = false;


TimeArr::TimeArr( long double custom_delay_prec )
: delay_precision(custom_delay_prec),
  time_arr_map()
{

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
  if ( !pressure.isValid() ) 
    time_arr_map[0.0] = Pressure::createNotValid();
  else 
    time_arr_map.emplace( std::make_pair( delay, pressure ) );
}


TimeArr::operator std::complex<double>() const { 
  std::complex<double> value;
  for ( const auto& it : time_arr_map ) {
    value += it.second;
  }
  return value;
}


bool TimeArr::isValid() const { 
  if ( time_arr_map.size() < 1 ) 
    return false; 
  if ( time_arr_map.find(0.0) != time_arr_map.end() && time_arr_map.find(0.0)->second == Pressure::createNotValid() ) 
    return false; 
  return true;
}


TimeArrCIt TimeArr::at( const int position ) const {
  if ( position >= static_cast<int>( time_arr_map.size() ) || position < 0 ) 
    return time_arr_map.cend();
  if ( position == 0 ) 
    return time_arr_map.cbegin();

  auto ret_val = time_arr_map.cbegin();
  std::advance( ret_val, position );

  return ret_val;
}


TimeArrCIt TimeArr::lowerBoundTxLoss( double threshold_db ) const {
  for ( auto it = time_arr_map.cbegin(); it != time_arr_map.cend(); it++ ) {
    if ( Pressure::getTxLossDb(it->second) <= threshold_db ) {
      
      if (debug)
        std::cout << "TimeArr::lowerBoundTxLoss() threshold_db curr tx loss val "
                  << Pressure::getTxLossDb(it->second) << " <=  threshold_db " << threshold_db 
                  << "; time " << it->first << std::endl;

      return it;
    }
  }
  
  return time_arr_map.cend();
}


// keeps the last value of each overridden PDouble
woss::TimeArr& TimeArr::setDelayPrecision( long double precision ) {
  TimeArrMap temp_map;
  
  for ( const auto& it : time_arr_map ) {
    temp_map[ PDouble( it.first.getValue(), precision) ] = it.second;
  }
  time_arr_map.swap(temp_map);
  
  return *this;
}


bool TimeArr::checkPressureAttenuation( double distance, double frequency ) {
  bool ret_val = false;
  
  for ( auto& it : time_arr_map ) {
    Pressure temp = Pressure(it.second);
    ret_val = ret_val || temp.checkAttenuation( distance, frequency ) ;
    it.second = temp;
  }
  
  return ret_val;
}


std::unique_ptr<TimeArr> TimeArr::coherentSumSample( double time_resolution ) const {
  TimeArrMap temp_map;
  
  PDouble curr_ch_time = time_arr_map.begin()->first;
  for ( const auto& it : time_arr_map ) {
    if ( it.first > (curr_ch_time + PDouble(time_resolution, delay_precision) ) ) 
      curr_ch_time = it.first;
    temp_map[curr_ch_time] += it.second;
  }
  return( create(temp_map) );
}


std::unique_ptr<TimeArr> TimeArr::incoherentSumSample( double time_resolution ) const {
  TimeArrMap temp_map;
  
  PDouble curr_ch_time = time_arr_map.begin()->first;
  for ( const auto& it : time_arr_map ) {
    if ( it.first > (curr_ch_time + PDouble(time_resolution, delay_precision) ) ) 
      curr_ch_time = it.first;
    temp_map[curr_ch_time] += pow( abs( it.second ), 2.0 );
  }
  
  for ( auto& it : temp_map ) {
    temp_map[it.first] = sqrt( it.second ) ;
  }
  return( create(temp_map) );
}


std::unique_ptr<TimeArr> TimeArr::crop( double time_start, double time_end ) const {
  TimeArrMap temp_map;

  for ( const auto& it : time_arr_map ) {
    if ( it.first >= PDouble(time_start, delay_precision) 
        && it.first < PDouble(time_end, delay_precision) ) {
      if (debug)
        std::cout << "TimeArr::crop() time_start " << time_start << "; time_end " << time_end 
                  << "; time value " << it.first << "; att " << it.second << std::endl;

			temp_map[it.first] = it.second;
		}
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
  for ( const auto& it : right.time_arr_map ) {
    left.time_arr_map[it.first] += it.second;
  }
  return left;
}


TimeArr& woss::operator-=( TimeArr& left, const TimeArr& right ) { 
  for ( const auto& it : right.time_arr_map ) {
    left.time_arr_map[it.first] -= it.second;
  }
  return left;
}


TimeArr& woss::operator+=( TimeArr& left, double right ) { 
  for ( const auto& it : left.time_arr_map ) {
    left.time_arr_map[it.first] += right;
  }
  return left;
}


TimeArr& woss::operator-=( TimeArr& left, double right ) { 
  for ( const auto& it : left.time_arr_map ) {
    left.time_arr_map[it.first] -= right;
  }
  return left;
}


TimeArr& woss::operator/=( TimeArr& left, double right ) { 
  for ( const auto& it : left.time_arr_map ) {
    left.time_arr_map[it.first] /= right;
  }
  return left;
}


TimeArr& woss::operator*=( TimeArr& left, double right ) { 
  for ( const auto& it : left.time_arr_map ) {
    left.time_arr_map[it.first] *= right;
  }
  return left;
}



