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
 * @file   time-arrival-definitions.h
 * @author Federico Guerra
 *
 * \brief  Definitions and library for woss::TimeArr class 
 *
 * Definitions and library for woss::TimeArr class
 */


#ifndef TIME_ARRIVAL_DEFINITIONS_H
#define TIME_ARRIVAL_DEFINITIONS_H


#include <cassert>
#include <climits>
#include <map>
#include <memory>
#include "pressure-definitions.h"
#include "custom-precision-double.h"


namespace woss {

  /**
  * Map that links a PDouble delay [s] to a complex Pressure
  **/ 
  using TimeArrMap = std::map < PDouble , std::complex<double> >; 
  using TimeArrIt = TimeArrMap::iterator;
  using TimeArrCIt = TimeArrMap::const_iterator;
  using TimeArrRIt = TimeArrMap::reverse_iterator;
  using TimeArrCRIt = TimeArrMap::const_reverse_iterator;

  /**
  * Delay precision for default constructed objects [s]
  **/ 
  static constexpr inline double TIMEARR_CUSTOM_DELAY_PRECISION = (1.0e-7);
  /**
  * Not valid delay value for Pressure to TimeArr conversion
  **/ 
  static constexpr inline int TIMEARR_PRESSURE_CONVERSION_DELAY = (-INT_MAX);

  /**
  * \brief Channel power delay profile class
  *
  * TimeArr class offers the possibility to store and manipulate channel power delay profiles, e.g. a collection of time delay 
  * values associated to a Pressure attenuation value.
  **/ 
  class TimeArr {

    public:

    /**
    * Default TimeArr constructor
    * @param custom_delay_prec delay precision [s]
    **/ 
    TimeArr( long double custom_delay_prec = TIMEARR_CUSTOM_DELAY_PRECISION );

    /**
    * TimeArr constructor
    * @param map custom time arrival map
    * @param custom_delay_prec delay precision [s]
    **/ 
    TimeArr( TimeArrMap& map, long double custom_delay_prec = TIMEARR_CUSTOM_DELAY_PRECISION );

    /**
    * TimeArr constructor. Needed for Pressure to TimeArr conversion
    * @param pressure Pressure value
    * @param delay delay value [s]
    * @param custom_delay_prec delay precision [s]
    **/ 
    explicit TimeArr( const Pressure& pressure, double delay = TIMEARR_PRESSURE_CONVERSION_DELAY, long double custom_delay_prec = TIMEARR_CUSTOM_DELAY_PRECISION );

    TimeArr( const TimeArr& copy ) = default;

    TimeArr( TimeArr&& tmp ) = default;

    virtual ~TimeArr() = default;

    /**
    * complex\<double\> operator for implicit casting
    * @return the coherent sum 
    **/
    virtual operator std::complex<double>() const;
    
    /**
    * TimeArr virtual factory method
    * @param custom_delay_prec delay precision [s]
    * @return a std::unique_ptr to a heap allocated woss::TimeArr object
    **/ 
    virtual std::unique_ptr<TimeArr> create( long double custom_delay_prec = TIMEARR_CUSTOM_DELAY_PRECISION ) const { return std::make_unique<TimeArr>(custom_delay_prec); }

    /**
    * TimeArr virtual factory method
    * @param map custom time arrival map
    * @param custom_delay_prec delay precision [s]
    * @return a std::unique_ptr to a heap allocated woss::TimeArr object
    **/ 
    virtual std::unique_ptr<TimeArr> create( TimeArrMap& map, long double custom_delay_prec = TIMEARR_CUSTOM_DELAY_PRECISION ) const {
      return std::make_unique<TimeArr>( map, custom_delay_prec); }

    /**
    * TimeArr virtual factory method
    * @param pressure Pressure value
    * @param delay delay value [s]
    * @param custom_delay_prec delay precision [s]
    * @return a std::unique_ptr to a heap allocated woss::TimeArr object
    **/ 
    virtual std::unique_ptr<TimeArr> create( const Pressure& pressure, double delay = TIMEARR_PRESSURE_CONVERSION_DELAY, long double custom_delay_prec = TIMEARR_CUSTOM_DELAY_PRECISION ) const {
      return std::make_unique<TimeArr>(pressure, delay, custom_delay_prec ); } 
    
    /**
    * TimeArr virtual factory method
    * @return a std::unique_ptr to a heap-created copy of <b>this</b> instance
    **/
    virtual std::unique_ptr<TimeArr> clone() const { return std::make_unique<TimeArr>(*this); }
  
    /**
    * TimeArr virtual factory method
    * @param array_size size of array
    * @return a std::unique_ptr to a heap-created array of size array_size
    **/
    virtual std::unique_ptr<TimeArr[]> createArray( unsigned int array_size ) const { return std::make_unique<TimeArr[]>(array_size); }
    
    /**
    * Creates an instance not valid
    * @return a new instance not valid ( e.g. delay 0.0 = (+inf, +inf) )
    **/
    static TimeArrMap& createNotValid();

    /**
    * Creates an impulsive instance
    * @return a new impulsive instance ( e.g. delay 0.0 = (+1.0, 0.0 ) )
    **/
    static TimeArrMap& createImpulse();

    /**
    * Inserts and replace a Pressure value at given delay
    * @param delay delay value [s]
    * @param pressure Pressure value
    * @return reference to <b>*this</b>
    **/
    TimeArr& insertValue( double delay, const Pressure& pressure );

    /**
    * Inserts and (complex) sums a Pressure value at given delay
    * @param delay delay value [s]
    * @param pressure Pressure value
    **/
    void sumValue( double delay, const Pressure& pressure );

    /**
    * Returns a const iterator to the Pressure with key == <i>delay</i> parameter
    * @param delay delay value [s]
    * @returns const iterator to end() if <i>delay</i> is not found
    **/
    TimeArrCIt findValue( double delay ) const { return( time_arr_map.find(delay) ); }

    /**
    * Erase the Pressure value with key == <i>delay</i> parameter
    * @param delay delay value [s]
    * @return reference to <b>*this</b>
    **/
    TimeArr& eraseValue( double delay ) { time_arr_map.erase(delay); return *this; }

    /**
    * Sample the TimeArr with sample-time <i>delay</i>. Pressure of the new TimeArr are the coherent sum of previous 
    * object with equal sampled delay.
    * @param delay delay value [s]
    * @return a std::unique_ptr to a heap allocated woss::TimeArr object
    **/
    virtual std::unique_ptr<TimeArr> coherentSumSample( double time_delay ) const;

    /**
    * Sample the TimeArr with sample-time <i>delay</i>. Pressure of the new TimeArr are the incoherent sum of previous 
    * object with equal sampled delay.
    * @param delay delay value [s]
    * @return a std::unique_ptr to a heap allocated woss::TimeArr object
    **/
    virtual std::unique_ptr<TimeArr> incoherentSumSample( double time_delay ) const;

    /**
    * Crops the TimeArr between given time values and returns a new heap-allocated object. 
		* The new object will have time values in [time_start, time_end)
    * @param time_start start time value [s]
		* @param time_end end time value [s]
    * @return a std::unique_ptr to a heap allocated woss::TimeArr object
    **/
    virtual std::unique_ptr<TimeArr> crop( double time_start, double time_end ) const;

    /**
    * Check the Pressure value of each delay. If amplitude is >= 1, it replaces with a new Pressure with same phase and 
    * new amplitude given by the Thorp absorption process at given frequency and along given distance
    * @param distance given distance [m]
    * @param frequency given frequency [hz]
    * @return <i>true</i> if correction was applied, <i>false</i> otherwise
    **/
    virtual bool checkPressureAttenuation( double distance, double frequency );

    /**
    * Returns a const iterator to the beginning of the time arrival map
    * @returns const iterator 
    **/
    TimeArrCIt begin() const { return time_arr_map.begin(); }
    
    /**
    * Returns a const iterator to the end of the time arrival map
    * @returns const iterator 
    **/
    TimeArrCIt end() const { return time_arr_map.end(); }
    
    /**
    * Returns a const reverse iterator to the reverse beginning of the time arrival map
    * @returns const iterator 
    **/
    TimeArrCRIt rbegin() const { return time_arr_map.rbegin(); }
    
    /**
    * Returns a const reverse iterator to the reverse end of the time arrival map
    * @returns const iterator 
    **/
    TimeArrCRIt rend() const { return time_arr_map.rend(); }

    /**
    * Returns a const iterator to the Pressure value at i-th position 
    * @param i integer should be between 0 and size() 
    * @returns const iterator to end() if position <i>i</i> is not found
    **/
    TimeArrCIt at( const int i ) const ;

    /**
    * Returns a const iterator to the Pressure that has txLoss >= threshold_db 
    * @param i threshold in db re upa 
    * @returns const iterator to end() if position <i>i</i> is not found
    **/
    TimeArrCIt lowerBoundTxLoss( double threshold_db ) const;

    /**
    * Returns the number of Pressure stored
    * @return number of Pressure values stored
    **/
    int size() const { return time_arr_map.size(); }

    /**
    * Checks if the instance has stored values
    * @return <i>true</i> if condition applies, <i>false</i> otherwise
    **/
    bool empty() const { return time_arr_map.empty(); } 

    /**
    * Erase all values of Pressure
    **/
    void clear() { time_arr_map.clear(); }

    /**
    * Sets the delay precision for all PDouble delay values. If the given precision is different from
    * current value, the profile will be modified accordingly
    * @param precision delay precision [m]
    * @return reference to <b>*this</b>
    **/  
    TimeArr& setDelayPrecision( long double precision );

    /**
    * Sets debug flag for all instances
    * @param flag debug bool
    **/
    static void setDebug( bool flag ) { debug = flag; }

    /**
    * Returns the maximum delay value
    * @returns maximum delay [s]
    **/
    double getMaxDelayValue() const { return( time_arr_map.rbegin()->first ); }

    /**
    * Returns the maximum delay value
    * @returns maximum delay [s]
    **/
    double getMinDelayValue() const { return( time_arr_map.begin()->first ); }

    /**
    * Returns the delay precision
    * @returns delay precision [s]
    **/
    long double getDelayPrecision() const { return delay_precision; }

    /**
    * Checks the validity of TimeArr
    * @return <i>true</i> if it has at least one value, <i>false</i> otherwise
    **/
    virtual bool isValid() const; 

    /**
    * Checks if the TimeArr was constructed from a Pressure value, therefore not carrying a valid delay information
    * @return <i>true</i> if it has at least one value, <i>false</i> otherwise
    **/
    virtual bool isConvertedFromPressure() const { return( time_arr_map.size() == 1 && ( time_arr_map.begin()->first == PDouble(TIMEARR_PRESSURE_CONVERSION_DELAY) ) ); }

    TimeArr& operator=( const TimeArr& copy ) = default;

    TimeArr& operator=( TimeArr&& tmp ) = default;

    /**
    * Equality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left == right</i>, false otherwise
    **/  
    friend bool operator==( const TimeArr& left, const TimeArr& right );

    /**
    * Inequality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left != right</i>, false otherwise
    **/  
    friend bool operator!=( const TimeArr& left, const TimeArr& right );

    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const TimeArr operator+( const TimeArr& left, const TimeArr& right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const TimeArr operator-( const TimeArr& left, const TimeArr& right );

    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const TimeArr operator+( const TimeArr& left, const double right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const TimeArr operator-( const TimeArr& left, const double right );  

    /**
    * Division operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const TimeArr operator/( const TimeArr& left, const double right );

    /**
    * Multiplication operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const TimeArr operator*( const TimeArr& left, const double right );

    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const TimeArr operator+( const double left, const TimeArr& right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const TimeArr operator-( const double left, const TimeArr& right );  

    /**
    * Division operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const TimeArr operator/( const double left, const TimeArr& right );

    /**
    * Multiplication operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const TimeArr operator*( const double left, const TimeArr& right );  

    /**
    * Compound assignment sum operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend TimeArr& operator+=( TimeArr& left, const TimeArr& right );

    /**
    * Compound assignment subtraction operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend TimeArr& operator-=( TimeArr& left, const TimeArr& right );

    /**
    * Compound assignment sum operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend TimeArr& operator+=( TimeArr& left, double right );

    /**
    * Compound assignment subtraction operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend TimeArr& operator-=( TimeArr& left, double right );

    /**
    * Compound assignment division operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend TimeArr& operator/=( TimeArr& left, double right );

    /**
    * Compound assignment multiplication operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend TimeArr& operator*=( TimeArr& left, double right );

    /**
    * << operator
    * @param os left operand ostream reference
    * @param instance right operand const Pressure reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend std::ostream& operator<<( std::ostream& os, const TimeArr& instance );

    protected:

    /**
    * Debug flag
    **/
    static bool debug;

    /**
    * Stores the precision of all PDouble delay instances [s]
    **/ 
    long double delay_precision;

    /**
    * TimArr values map 
    **/ 
    TimeArrMap time_arr_map;

  };

  //non-inline operator declarations
  /////////////////
  const TimeArr operator+( const TimeArr& left, const TimeArr& right ); 

  const TimeArr operator-( const TimeArr& left, const TimeArr& right ); 

  const TimeArr operator+( const TimeArr& left, const double right ); 

  const TimeArr operator-( const TimeArr& left, const double right ); 

  const TimeArr operator/( const TimeArr& left, const double right ); 

  const TimeArr operator*( const TimeArr& left, const double right ); 

  const TimeArr operator+( const double left, const TimeArr& right ); 

  const TimeArr operator-( const double left, const TimeArr& right );

  const TimeArr operator/( const double left, const TimeArr& right ); 

  const TimeArr operator*( const double left, const TimeArr& right ); 

  TimeArr& operator+=( TimeArr& left, const TimeArr& right ); 

  TimeArr& operator-=( TimeArr& left, const TimeArr& right ); 

  TimeArr& operator+=( TimeArr& left, double right );

  TimeArr& operator-=( TimeArr& left, double right ); 

  TimeArr& operator/=( TimeArr& left, double right );

  TimeArr& operator*=( TimeArr& left, double right );

  //////////
  //inline functions
  inline TimeArrMap& TimeArr::createNotValid() {
    static TimeArrMap time_arr_map;
    time_arr_map.clear();
    time_arr_map[0.0] = Pressure::createNotValid();
    return time_arr_map;
  }

  inline TimeArrMap& TimeArr::createImpulse() {
    static TimeArrMap time_arr_map;
    time_arr_map.clear();
    time_arr_map[0.0] = std::complex<double> (1.0 , 0.0);
    return time_arr_map;
  }

  inline TimeArr& TimeArr::insertValue( double delay, const Pressure& pressure ) { 
    assert( pressure.isValid() ); 
    assert( delay >= 0.0 );
    
    time_arr_map.emplace( std::make_pair( delay, pressure ) ); 
    
    return *this;
  }

  inline void TimeArr::sumValue( double delay, const Pressure& pressure ) {
    assert( pressure.isValid() ); 
    assert( delay >= 0.0 );
    
    TimeArrIt it = time_arr_map.find( delay );
    if ( it == time_arr_map.end() ) 
      time_arr_map.emplace( std::make_pair( delay, pressure ) );
    else 
      it->second += std::complex<double>( pressure ); 
  }

  inline std::ostream& operator<<( std::ostream& os, const TimeArr& instance ) {
    os << "size = " << instance.time_arr_map.size() << "; min time_arr = " << instance.time_arr_map.begin()->first 
                    << "; pressure db = " << Pressure::getTxLossDb(instance.begin()->second)  
                    << "; max time_arr = " << instance.time_arr_map.rbegin()->first 
                    << "; pressure db = " << Pressure::getTxLossDb(instance.time_arr_map.rbegin()->second);
    return os;
  }

  inline bool operator==( const TimeArr& left, const TimeArr& right ) { 
    if ( &left == &right ) 
      return true; 
    return( left.time_arr_map == right.time_arr_map );
  }

  inline bool operator!=( const TimeArr& left, const TimeArr& right ) { 
    if ( &left == &right ) 
      return false; 
    return( left.time_arr_map != right.time_arr_map );
  }

}

#endif /* TIME_ARRIVAL_DEFINITIONS_H */

