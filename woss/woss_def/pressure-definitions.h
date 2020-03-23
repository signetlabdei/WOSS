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
 * @file   pressure-definitions.h
 * @author Federico Guerra
 *
 * \brief  Definitions and library for woss::Pressure class 
 *
 * Definitions and library for complex pressure
 */


#ifndef PRESSURE_DEFINITIONS_H
#define PRESSURE_DEFINITIONS_H 


#include <complex>


namespace woss {
  
    
  class TimeArr;

  /**
  * \brief Complex attenuated pressure class
  *
  * The Pressure class stores a complex\<double\> acoustic pressure value for attenuation purposes. Pressure values are adimensional:
  * complex pressure represented has to be considered in conjunction with transmitted pressure.
  * Pressure also provides full arithmetic capability and a attenuation coherency check
  **/
  class Pressure {

    
    public:
      
      
    /**
    * Pressure constructor
    * @param real real part
    * @param imag imaginary part
    **/     
    Pressure( double real = 0.0, double imag = 0.0 );

    /**
    * Pressure constructor
    * @param complex_press complex\<double\> value
    **/   
    Pressure( const ::std::complex<double>& complex_press );

    /**
    * Constructs a Pressure from a TimeArr
    * @param time_arr valid TimeArr const reference
    **/
    Pressure( const TimeArr& time_arr );
    
    /**
    * Pressure copy constructor
    * @param copy Pressure to be copied
    **/
    Pressure( const Pressure& copy );

    
    /**
    * Pressure virtual factory method
    * @param real real part
    * @param imag imaginary part
    * @return a heap-created Pressure object
    **/     
    virtual Pressure* create( double real = 0.0, double imag = 0.0 ) const { return new Pressure( real, imag ); }

    /**
    * Pressure virtual factory method
    * @param complex_press complex\<double\> value
    * @return a heap-created Pressure object
    **/   
    virtual Pressure* create( const ::std::complex<double>& complex_press ) const { return new Pressure( complex_press ); }

    /**
    * Pressure virtual factory method
    * @param time_arr valid TimeArr const reference
    * @return a heap-created Pressure object
    **/
    virtual Pressure* create( const TimeArr& time_arr ) const { return new Pressure( time_arr ); }
    
    /**
    * Pressure virtual factory method
    * @param copy Pressure to be copied
    * @return a heap-created Pressure object
    **/
    virtual Pressure* create( const Pressure& copy ) const { return new Pressure( copy ); }
    
    /**
    * Pressure virtual factory method
    * @return a heap-created copy of <b>this</b> instance
    **/
    virtual Pressure* clone() const { return new Pressure(*this); }
    
    /**
    * Pressure virtual factory method
    * @param array_size size of array
    * @return a heap-created array of size array_size
    **/
    virtual Pressure* createArray( unsigned int array_size ) const { return new Pressure[array_size]; }
    
    
    virtual ~Pressure() { }


    /**
    * complex\<double\> operator for implicit casting
    * @return a copy of complex_pressure
    **/
    operator ::std::complex<double>() const { return complex_pressure; }


    /**
    * Creates an instance not valid
    * @return an instance not valid ( e.g. (+inf, +inf) )
    **/
    static const ::std::complex<double> createNotValid() { return( ::std::complex<double>( HUGE_VAL, HUGE_VAL ) ); } 
    
    
    /**
    * Gets the real part
    * @return double real part
    **/
    double real() const { return complex_pressure.real(); }

    /**
    * Gets the imaginary part
    * @return double imaginary part
    **/
    double imag() const { return complex_pressure.imag(); }
    
    
    /**
    * Gets the absolute value
    * @return double absolute value
    **/
    double abs() const { return ::std::abs( complex_pressure ); }

    /**
    * Gets the phase (arg) value
    * @return double phase (arg) value
    **/
    double phase() const { return ::std::arg( complex_pressure ); }

    /**
    * Gets the square root of a complex number
    * @return double square root value
    **/
    Pressure sqrt() const { return ::std::sqrt( complex_pressure ); }


    /**
    * Checks the validity of complex pressure provided
    * @return <i>true</i> if complex pressure is valid, <i>false</i> otherwise
    **/
    virtual bool isValid() const { return( complex_pressure != ::std::complex<double>( HUGE_VAL, HUGE_VAL ) ); }
    

    /**
    * Returns the Transmission Loss measured in db
    * @return the Transmission Loss [db]
    **/
    static double getTxLossDb( const ::std::complex<double>& val ) { if ( val == ::std::complex<double>( HUGE_VAL, HUGE_VAL ) ) return -HUGE_VAL;
                                else if( val == ::std::complex<double>( 0.0, 0.0 ) ) return HUGE_VAL; 
                                else return( -20.0 * log10( ::std::abs(val) ) ); }
  
  
    /**
    * Checks if the attuenuation provided by the complex pressure is truly an attenuation,
    * e.g. if abs < 1. If not, replace the pressure with the attenuation provided by Thorp absorption process at given frequency
    * along the given distance
    * @param distance distance value in <i>meters</i>
    * @param frequency frequency value in <i>hertz</i>
    * @return <i>true</i> if correction was applied, <i>false</i> otherwise
    **/  
    virtual bool checkAttenuation( double distance, double frequency );

    
    /**
    * Sets debug for the whole class
    * @param flag debug value
    **/
    static void setDebug( bool flag ) { debug = flag; } 
    
    
    /**
    * Assignment operator
    * @param copy const reference to a Pressure object to be copied
    * @return Pressure reference to <i>this</i>
    **/  
    Pressure& operator=( const Pressure& x );
    
    
    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const Pressure operator+( const Pressure& left, const Pressure& right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Pressure operator-( const Pressure& left, const Pressure& right );

    /**
    * Division operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Pressure operator/( const Pressure& left, const Pressure& right );
    
    /**
    * Multiplication operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Pressure operator*( const Pressure& left, const Pressure& right );


    /**
    * Compound assignment sum operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Pressure& operator+=( Pressure& left, const Pressure& right );

    /**
    * Compound assignment subtraction operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Pressure& operator-=( Pressure& left, const Pressure& right );

    /**
    * Compound assignment division operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Pressure& operator/=( Pressure& left, const Pressure& right );

    /**
    * Compound assignment multiplication operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Pressure& operator*=( Pressure& left, const Pressure& right );
    
    
    /**
    * Equality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left == right</i>, false otherwise
    **/  
    friend bool operator==( const Pressure& left, const Pressure& right );

    /**
    * Inequality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left != right</i>, false otherwise
    **/  
    friend bool operator!=( const Pressure& left, const Pressure& right );
    
    
    /**
    * << operator
    * @param os left operand ostream reference
    * @param instance right operand const Pressure reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend ::std::ostream& operator<<( ::std::ostream& os, const Pressure& instance );
    
      
    protected:
    
      
    /**
    * Debug flag
    **/
    static bool debug;
    
    
    /**
    * Complex acoustic pressure
    **/
    ::std::complex< double > complex_pressure;

    /**
    * Gets the linear acoustic attenuation incurred at given frequency along the given distance
    * @param dist distance in <i>meters</i>
    * @param freq frequency in <i>hertz</i>
    * @return linear attenuation
    **/ 
    virtual double getAttenuation( double dist, double freq );

    /**
    * Gets the acoustic attenuation (in db re uPa / m ) given by Thorp absorption process incurred at given frequency
    * @param freq frequency in <i>hertz</i>
    * @return attenuation in <i>db re uPa / m</i>
    **/ 
    double getThorpAtt( double frequency );
    
  };

  
  //inline functions 
  /////////////
  inline bool operator==( const Pressure& left, const Pressure& right ) { 
    if ( &left == &right ) return true; 
    return( left.complex_pressure == right.complex_pressure );
  }


  inline bool operator!=( const Pressure& left, const Pressure& right ) { 
    if ( &left == &right ) return false; 
    return( left.complex_pressure != right.complex_pressure );
  }


  inline const Pressure operator+( const Pressure& left, const Pressure& right ) { 
    return( Pressure( left.complex_pressure + right.complex_pressure ) );
  }
    
    
  inline const Pressure operator-( const Pressure& left, const Pressure& right ) { 
    return( Pressure( left.complex_pressure - right.complex_pressure ) );
  }
    
    
  inline const Pressure operator/( const Pressure& left, const Pressure& right ) { 
    return( Pressure( left.complex_pressure / right.complex_pressure ) );
  }
    
    
  inline const Pressure operator*( const Pressure& left, const Pressure& right ) { 
    return( Pressure( left.complex_pressure * right.complex_pressure ) );
  }

    
  inline Pressure& operator+=( Pressure& left, const Pressure& right ) { 
    left.complex_pressure += right.complex_pressure;
    return left;
  }
    
    
  inline Pressure& operator-=( Pressure& left, const Pressure& right ) { 
    left.complex_pressure -= right.complex_pressure;
    return left;
  }
    
    
  inline Pressure& operator/=( Pressure& left, const Pressure& right ) { 
    left.complex_pressure /= right.complex_pressure;
    return left;
  }
    
    
  inline Pressure& operator*=( Pressure& left, const Pressure& right ) { 
    left.complex_pressure *= right.complex_pressure;
    return left;
  }
    

  inline ::std::ostream& operator<<( ::std::ostream& os, const Pressure& instance ) { 
    os << instance.complex_pressure;
    return os; 
  }


}


#endif /* PRESSURE_DEFINITIONS_H */

