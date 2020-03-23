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
 * @file   custom-precision-double.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for the woss::PDouble class
 *
 * Provides the interface for the PDouble class. PDouble stands for PrecisionDouble: the value and the precision 
 * provided are used for arithmetic computations and comparisons. This class is not intented for inheritance: 
 * all containers used in WOSS are based on PDouble <i>objects</i>, not pointers to objects. Therefore, inheritance requires 
 * modification in other classes, to prevent object slicing when inserting in containers PDouble-derived 
 * objects instead of PDouble objects. Moreover, due to intensive use of PDouble objects, using heap-based pointers
 * adds another layer of indirection, therefore adding an efficiency penalty.
 */


#ifndef CUSTOM_PRECISION_DOUBLE_H
#define CUSTOM_PRECISION_DOUBLE_H


#include <iostream>
#include <cmath>



namespace woss {
    
    
  /**
  * Default precision of any instance if no precision is specified.
  **/
  #define PDOUBLE_DEFAULT_PRECISION (1.0e-17)

  /**
  * \brief Custom precision long double class.
  *
  * The PDouble class stores a long double value and a long double precision for arithmetic computation and comparison purposes.
  **/
  class PDouble {

    
    public:
    
    /**
    * PDouble constructor
    * @param input value to store
    * @param precision custom precision
    **/     
    PDouble( const long double input = 0.0, const long double precision = PDOUBLE_DEFAULT_PRECISION );
    
    /**
    * PDouble copy constructor
    * @param copy PDouble to be copied
    **/
    PDouble( const PDouble& copy );
    
    /**
    * PDouble destructor.
    * It is not <b>virtual</b>, since this class is not meant to be inherited from
    **/
    ~PDouble();
    
    
    /**
    * Sets debug for all instances
    * @param flag debug boolean
    **/
    static void setDebug( bool flag ) { debug = flag; }

    /**
    * Sets a custom precison
    * @param value desired precision
    **/
    void setPrecision( double value ) { precision = value; }
    
    
    /**
    * Returns the custom precison
    * @return long double precision
    **/
    long double getPrecision() const { return precision; }

    /**
    * Returns the custom value
    * @return long double value
    **/
    long double getValue() const { return value; }


    /**
    * Assignment operator
    * @param copy const reference to a PDouble object to be copied
    * @return PDouble reference to <i>this</i>
    **/
    PDouble& operator=( const PDouble& copy );

    
    /**
    * int cast operator
    * @return a copy of <i>value</i> casted to int
    **/
    operator int() const;
    
    /**
    * float cast operator
    * @return a copy of <i>value</i> casted to float
    **/
    operator float() const;

    /**
    * double cast operator
    * @return a copy of <i>value</i> casted to double
    **/
    operator double() const;
    
    /**
    * long double cast operator
    * @return a copy of <i>value</i> casted to long double
    **/
    operator long double() const;
    
    
    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const PDouble operator+( const PDouble& left, const PDouble& right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const PDouble operator-( const PDouble& left, const PDouble& right );

    /**
    * Division operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const PDouble operator/( const PDouble& left, const PDouble& right );

    /**
    * Multiplication operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const PDouble operator*( const PDouble& left, const PDouble& right );

    /**
    * Modulo operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const PDouble operator%( const PDouble& left, const PDouble& right );
    

    /**
    * Compound assignment sum operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend PDouble& operator+=( PDouble& left, const PDouble& right );
    
    /**
    * Compound assignment subtraction operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend PDouble& operator-=( PDouble& left, const PDouble& right );

    /**
    * Compound assignment division operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend PDouble& operator/=( PDouble& left, const PDouble& right );

    /**
    * Compound assignment multiplication operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend PDouble& operator*=( PDouble& left, const PDouble& right );
    
    /**
    * Compound assignment modulo operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/   
    friend PDouble& operator%=( PDouble& left, const PDouble& right );

    
    /**
    * Equality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left == right</i>, false otherwise
    **/  
    friend bool operator==( const PDouble& left, const PDouble& right );

    /**
    * Inequality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left != right</i>, false otherwise
    **/  
    friend bool operator!=( const PDouble& left, const PDouble& right );
    
    
    /**
    * Greater than operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left > right</i>, false otherwise
    **/ 
    friend bool operator>( const PDouble& left, const PDouble& right );

    /**
    * Less than operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left < right</i>, false otherwise
    **/ 
    friend bool operator<( const PDouble& left, const PDouble& right );

    /**
    * Greater than or equal to operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left >= right</i>, false otherwise
    **/    
    friend bool operator>=( const PDouble& left, const PDouble& right );

    /**
    * Less than or equal to operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left <= right</i>, false otherwise
    **/ 
    friend bool operator<=( const PDouble& left, const PDouble& right );
    
    
    /**
    * << operator
    * @param os left operand ostream reference
    * @param instance right operand const PDouble reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend ::std::ostream& operator<<( ::std::ostream& os, const PDouble& instance );

    /**
    * >> operator
    * @param is left operand istream reference
    * @param instance right operand PDouble reference. It will take the value provided by <i>left</i> with default precision
    * @return <i>is</i> reference after the operation
    **/ 
    friend ::std::istream& operator>>( ::std::istream& is, PDouble& instance );
    

    
    protected:

    /**
    * stored value
    **/ 
    long double value;

    /**
    * stored precision
    **/ 
    long double precision;

    
    /**
    * debug status
    **/ 
    static bool debug;

  };


  ////////////
  // inline functions

  inline PDouble::operator double() const {
    return( (double) value) ;
  }


  inline PDouble::operator int() const {
    return( (int) value );
  }


  inline PDouble::operator float() const {
    return( (float) value );
  }


  inline PDouble::operator long double() const {
    return( (long double) value );
  }


  inline const PDouble operator+( const PDouble& left, const PDouble& right ) {
    return( PDouble(left.value + right.value, ::std::max(left.precision, right.precision)) );
  }


  inline const PDouble operator-( const PDouble& left, const PDouble& right ) {
    return( PDouble(left.value - right.value, ::std::max(left.precision, right.precision)) );
  }


  inline const PDouble operator/( const PDouble& left, const PDouble& right ) {
    return( PDouble(left.value / right.value, ::std::max(left.precision, right.precision)) );
  }


  inline const PDouble operator*( const PDouble& left, const PDouble& right ) {
    return( PDouble(left.value * right.value, ::std::max(left.precision, right.precision)) );
  }


  inline const PDouble operator%( const PDouble& left, const PDouble& right ) {
    return( PDouble( ::std::fmod(left.value, right.value), ::std::max(left.precision, right.precision) ) );
  }


  inline PDouble& operator+=( PDouble& left, const PDouble& right ) {
  //   if ( &left == &right ) { /* self assignment code here */ }
    left.value += right.value;
    return left;
  }


  inline PDouble& operator-=( PDouble& left, const PDouble& right ) {
  //   if ( &left == &right ) { /* self assignment code here */ }
    left.value -= right.value;
    return left;
  }


  inline PDouble& operator/=( PDouble& left, const PDouble& right ) {
  //   if ( &left == &right ) { /* self assignment code here */ }
    left.value /= right.value;
    return left;
  }


  inline PDouble& operator*=( PDouble& left, const PDouble& right ) {
  //   if ( &left == &right ) { /* self assignment code here */ }
    left.value *= right.value;
    return left;
  }


  inline PDouble& operator%=( PDouble& left, const PDouble& right ) {
  //   if ( &left == &right ) { /* self assignment code here */ }
    left.value = ::std::fmod(left.value, right.value);
    return left;
  }


  inline bool operator==( const PDouble& left, const PDouble& right ) {
    if ( &left == &right ) { return true; }
    return( ::std::abs(left.value - right.value) <= left.precision );
  }


  inline bool operator!=( const PDouble& left, const PDouble& right ) {
    if ( &left == &right ) { return false; }
    return( ::std::abs(left.value - right.value) > left.precision );
  }


  inline bool operator>( const PDouble& left, const PDouble& right ) {
    if ( left == right ) return false;
    return( left.value > right.value );
  }


  inline bool operator<( const PDouble& left, const PDouble& right ) {
    if ( left == right ) return false;
    return( left.value < right.value );
  }


  inline bool operator>=( const PDouble& left, const PDouble& right ) {
    if ( left == right ) return true;
    return( left.value > right.value );
  }


  inline bool operator<=( const PDouble& left, const PDouble& right ) {
    if ( left == right ) return true;
    return( left.value < right.value );
  }


  inline ::std::ostream& operator<<( ::std::ostream& os, const PDouble& instance ) {
    os << instance.value;
    return os;
  }


  inline ::std::istream& operator>>( ::std::istream& is, PDouble& instance ) {
    is >> instance.value;
    return is;
  }

}

#endif /* CUSTOM_PRECISION_DOUBLE_H */

