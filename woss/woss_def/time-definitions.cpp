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
 * @file   time-definitions.cpp
 * @author Federico Guerra
 *
 * \brief  Implementation of woss::Time class 
 *
 * Implementation of woss::Time class 
 */


#include <iostream>
#include <cmath>
#include <cstdlib>
#include "time-definitions.h"


using namespace woss;


bool Time::debug = false;


Time::Time() 
  : timeinfo(),
    raw_time(TIME_NOT_SET_VALUE)
{ 
  time( &raw_time );
  timeinfo = *( localtime( &raw_time ) );
}


Time::Time(int d, int mth, int y, int h, int m, int s) 
  : timeinfo(),
    raw_time(TIME_NOT_SET_VALUE) 
{ 
  assert( d >= 1 && d <= 31 );
  assert( mth >= 1 && mth <= 12 );
  assert( y > 1900 );
  assert( h >= 0 && h <= 23 );
  assert( m >= 0 && m <= 59 );
  assert( s >= 0 && s <= 59 );

//    ::std::cout << "Time::Time() d " << d << "; mth " << mth
//                << "; y " << y << "; h " << h
//                << "; m " << m << "; s " << s
//                << ::std::endl; 
  
  time( &raw_time );
  timeinfo = *( localtime( &raw_time ) );
 
  timeinfo.tm_mon = mth - 1;
  timeinfo.tm_mday = d;
  timeinfo.tm_year = y - 1900;
  timeinfo.tm_hour = h;
  timeinfo.tm_min = m;
  timeinfo.tm_sec = s;

  raw_time = mktime(&timeinfo);
  
//   ::std::cout << "Time::Time() raw_time " << raw_time << "; tm mon " << timeinfo.tm_mon
//               << "; tm mday " << timeinfo.tm_mday << "; tm year " << timeinfo.tm_year
//               << "; tm hour " << timeinfo.tm_hour << "; tm min " << timeinfo.tm_min
//               << "; tm sec " << timeinfo.tm_sec << ::std::endl;
}


Time::Time(const Time& time) {
  timeinfo = time.timeinfo;
  raw_time = time.raw_time;
}


Time::Time(struct tm* time) {
  timeinfo = *time;
  raw_time = mktime(&timeinfo);
}


Time& Time::operator=( const Time& time ) {
  if (this == &time) return *this;
  timeinfo=time.timeinfo;
  raw_time = time.raw_time;
  return *this;
}


Time& woss::operator+=( Time& left, time_t right ) {
  time_t sum_time = mktime( &left.timeinfo ) + right;
  left.timeinfo = *( localtime( &sum_time ) );
  left.raw_time = mktime(&(left.timeinfo));
  return left;
}


Time& woss::operator-=( Time& left, time_t right ) {
  time_t diff_time = mktime( &left.timeinfo ) - right;
  left.timeinfo = *( localtime( &diff_time ) );
  left.raw_time = mktime(&(left.timeinfo));
  return left;
}


double woss::operator-( const Time& left, const Time& right ) {
  time_t time_x = mktime( &left.timeinfo );
  time_t time_y = mktime( &right.timeinfo );
 
  if (time_x > time_y) return( difftime(time_x, time_y) );
  else return( difftime( time_y, time_x) );
}



