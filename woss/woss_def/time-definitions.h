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
 * @file   time-definitions.h
 * @author Federico Guerra
 *
 * \brief  Definitions and library for woss::Time, woss::SimTime, woss::TimeReference and woss::TimeReferenceTcl classes
 *
 * Definitions and library for woss::Time, woss::SimTime, woss::TimeReference and woss::TimeReferenceTcl classes
 */


#ifndef WOSS_TIME_DEFINITIONS_H
#define WOSS_TIME_DEFINITIONS_H


#include <iostream>
#include <ctime>
#include <cassert>
#include <cmath>
#include <climits>


namespace woss {

    
  #define TIME_NOT_SET_VALUE (LONG_MIN)


  /**
  * \brief Class for simulation time reference purposes
  *
  * Class for simulation time reference purposes
  *
  **/ 
  class TimeReference {
  
    
    public: 


    virtual ~TimeReference() = 0;

    /**
    * woss::TimeReference virtual factory method
    * @return a heap-allocated copy of <b>this</b> instance
    **/      
    virtual TimeReference* clone() = 0;

    /**
    * Retuns simulation time
    * @return value in seconds
    **/          
    virtual double getTimeReference() const = 0;
  
    
  };

  inline TimeReference::~TimeReference() { }

  /**
  * \brief a class for time date manipulation
  *
  * Time class offers the possibility to store and manipulate date time.
  * A time date consists of a day, month, year, hours,
  * minutes and seconds.
  **/ 
  class Time {


    public:

      
    /**
    * Default Time constructor. Object is not valid
    **/ 
    Time();

    /**
    * Time constructor
    * @param time <b>struct tm</b> from ctime library
    **/
    Time( struct tm* time );

    /**
    * Time constructor
    * @param day day value. Should be between 1 and 31
    * @param month month value. Should be between 1 and 12
    * @param year year value.
    * @param hours hours value. Should be between 0 and 23
    * @param mins minutes value. Should be between 0 and 59
    * @param seconds seconds value. Should be between 0 and 59
    **/
    Time( int day, int month, int year, int hours = 0, int mins = 0, int seconds = 1 ); 

    /**
    * Time copy constructor
    * @param copy TimeArr to be copied
    **/
    Time( const Time& copy );
    
    /**
    * Time destructor.
    * It is not <b>virtual</b>, since this class is not meant to be inherited from
    **/
    ~Time() { }


    /**
    * Sets month 
    * @param m month value. Should be between 1 and 12
    * @return reference to <b>*this</b>
    **/
    Time& setMonth( int m ) { assert( m >= 1 && m <= 12 ); timeinfo.tm_mon = m - 1; raw_time = mktime(&timeinfo); 
                              return *this; }

    /**
    * Sets day 
    * @param d day value. Should be between 1 and 31
    * @return reference to <b>*this</b>
    **/
    Time& setDay( int d ) { assert( d >= 1 && d <= 31 ); timeinfo.tm_mday = d; raw_time = mktime(&timeinfo); return *this; }

    /**
    * Sets year 
    * @param y year value
    * @return reference to <b>*this</b>
    **/
    Time& setYear( int y ) { assert( y >= 1900 ); timeinfo.tm_year = y - 1900; raw_time = mktime(&timeinfo); return *this; }

    /**
    * Sets hours 
    * @param m hours value. Should be between 0 and 23
    * @return reference to <b>*this</b>
    **/
    Time& setHours( int h ) { assert( h >= 0 && h <= 23 ); timeinfo.tm_hour = h; raw_time = mktime(&timeinfo); return *this; }

    /**
    * Sets minutes 
    * @param m minutes value. Should be between 0 and 59
    * @return reference to <b>*this</b>
    **/
    Time& setMinutes( int m ) { assert( m >= 0 && m <= 59 ); timeinfo.tm_min = m; raw_time = mktime(&timeinfo); return *this; }

    /**
    * Sets seconds 
    * @param s seconds value. Should be between 0 and 59
    * @return reference to <b>*this</b>
    **/
    Time& setSeconds( int s ) { assert( s >= 0 && s <= 59 ); timeinfo.tm_sec = s; raw_time = mktime(&timeinfo); return *this; }


    /**
    * Sets debug flag for all instances
    * @param flag debug bool
    **/
    static void setDebug(bool flag) { debug = flag; }
    

    /**
    * Checks the validity of Time
    * @return <i>true</i> if it has a initialized date time, <i>false</i> otherwise
    **/
    bool isValid() const { return ( raw_time != TIME_NOT_SET_VALUE ); }


    /**
    * Returns month value
    * @return month value between 1 and 12
    **/
    int getMonth() const { return timeinfo.tm_mon; }

    /**
    * Returns day value
    * @return day value between 1 and 31
    **/
    int getDay() const { return timeinfo.tm_mday; }

    /**
    * Returns hours value
    * @return hours value between 0 and 23
    **/
    int getHours() const { return timeinfo.tm_hour; }

    /**
    * Returns year value
    * @return year value
    **/
    int getYear() const { return timeinfo.tm_year; }

    /**
    * Returns minutes value
    * @return minutes value between 0 and 59
    **/
    int getMinutes() const { return timeinfo.tm_min; }

    /**
    * Returns seconds value
    * @return seconds value between 0 and 59
    **/
    int getSeconds() const { return timeinfo.tm_sec; }


    /**
    * time_t operator
    * @return the time_t
    **/
    operator time_t() const;

    /**
    * Assignment operator
    * @param copy const reference to a Time object to be copied
    * @return Time reference to <i>this</i>
    **/ 
    Time& operator=( const Time& copy );


    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const Time operator+( const Time& left, const time_t right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const Time operator-( const Time& left, const time_t right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return time difference in seconds 
    **/  
    friend double operator-( const Time& left, const Time& right );


    /**
    * Compound assignment sum operator
    * @param left left operand reference
    * @param right right operand const time_t representing seconds
    * @return <i>left</i> reference after the operation
    **/
    friend Time& operator+=( Time& left, time_t right );

    /**
    * Compound assignment subtraction operator
    * @param left left operand reference
    * @param right right operand const time_t representing seconds
    * @return <i>left</i> reference after the operation
    **/
    friend Time& operator-=( Time& left, time_t right );


    /**
    * Equality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left == right</i>, false otherwise
    **/ 
    friend bool operator==( const Time& left, const Time& right );

    /**
    * Inequality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left != right</i>, false otherwise
    **/ 
    friend bool operator!=( const Time& left, const Time& right );

    
    /**
    * Greater than operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left > right</i>, false otherwise
    **/ 
    friend bool operator>( const Time& left, const Time& right );

    /**
    * Less than operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left < right</i>, false otherwise
    **/ 
    friend bool operator<( const Time& left, const Time& right );

    /**
    * Less than or equal to operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left <= right</i>, false otherwise
    **/ 
    friend bool operator<=( const Time& left, const Time& right );

    /**
    * Greater than or equal to operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left >= right</i>, false otherwise
    **/ 
    friend bool operator>=( const Time& left, const Time& right );


    /**
    * << operator
    * @param os left operand ostream reference
    * @param instance right operand const Time reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend ::std::ostream& operator<<( ::std::ostream& os, const Time& instance );


    protected:


    /**
    * Struct tm from ctime library
    **/
    mutable struct tm timeinfo;

    /**
    * Number of seconds corresponding to date time <i>timeinfo</i>
    **/
    time_t raw_time;
    

    /**
    * Debug flag
    **/
    static bool debug;

  };

  
  /**
  * \brief Struct that stores start and end Time
  *
  * woss::SimeTime is a pair of woss::Time objects for simulation purposes. A start Time and a end Time
  **/ 
  struct SimTime {
    
    
    SimTime( Time time1 = Time(), Time time2 = Time() ) : start_time(time1), end_time(time2) { }
    
    
    friend std::ostream& operator<<( std::ostream& os, const SimTime& instance ) {
      os << "start time = " << instance.start_time << "; end time = " << instance.end_time;
      return os;
    }   
    
    
    Time start_time;
    
    Time end_time;
  
    
  };
  

  // non-inline operator declarations
  double operator-( const Time& left, const Time& right );

  
  //inline functions
  //////////
  inline Time::operator time_t() const {
    return( mktime(&timeinfo) );
  }
  
  
  inline bool operator==( const Time& left, const Time& right ) {
    if ( &left == &right ) return true; 
    return( mktime(const_cast<tm*>(&left.timeinfo)) == mktime(const_cast<tm*>(&right.timeinfo)) );
  }


  inline bool operator!=( const Time& left, const Time& right ) {
    if ( &left == &right ) return false; 
    return( mktime(const_cast<tm*>(&left.timeinfo)) != mktime(const_cast<tm*>(&right.timeinfo)) );
  }


  inline bool operator>( const Time& left, const Time& right ) {
    if ( &left == &right ) return false; 
    return( mktime(const_cast<tm*>(&left.timeinfo)) > mktime(const_cast<tm*>(&right.timeinfo)) );
  }


  inline bool operator<( const Time& left, const Time& right ) {
    if ( &left == &right ) return false; 
    return( mktime(const_cast<tm*>(&left.timeinfo)) < mktime(const_cast<tm*>(&right.timeinfo)) );
  }


  inline bool operator<=( const Time& left, const Time& right ) {
    return( mktime(const_cast<tm*>(&left.timeinfo)) <= mktime(const_cast<tm*>(&right.timeinfo)) );
  }


  inline bool operator>=( const Time& left, const Time& right ) {
    return( mktime(const_cast<tm*>(&left.timeinfo)) >= mktime(const_cast<tm*>(&right.timeinfo)) );
  }


  inline std::ostream& operator<<( std::ostream& os, const Time& time ) {
    os << asctime(const_cast<tm*>(&time.timeinfo)) ;
    return os;
  }


  inline const Time operator+( const Time& left, const time_t right ) {
    time_t sum_time = mktime(const_cast<tm*>(&left.timeinfo)) + right;
    return( Time( localtime( &sum_time ) ) );
  }


  inline const Time operator-( const Time& left, const time_t right ) {
    time_t diff_time = mktime(const_cast<tm*>(&left.timeinfo)) - right;
    return( Time( localtime( &diff_time ) ) );
  }


  Time& operator+=( Time& left, time_t right );


  Time& operator-=( Time& left, time_t right );

}


#endif /* WOSS_TIME_DEFINITIONS_H */


