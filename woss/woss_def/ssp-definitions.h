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
 * @file   ssp-definitions.h
 * @author Federico Guerra
 *
 * \brief  Definitions and library for Sound Speed Profiles 
 *
 * Definitions and library for woss::SSP (Sound Speed profile) class
 */


#ifndef WOSS_SSP_DEFINITIONS_H
#define WOSS_SSP_DEFINITIONS_H 


#include <cassert>
#include <cstdlib>
#include <map>
#include "coordinates-definitions.h"
#include "pressure-definitions.h"
#include "custom-precision-double.h"
#include "definitions-handler.h"


namespace woss {
  
    
  // UNESCO CHEN AND MILLERO EQUATIONS COEFFICIENTS
  #define cf_C00 (1402.388)
  #define cf_A02 (7.166e-5)
  #define cf_C01 (5.03830)
  #define cf_A03 (2.008e-6)
  #define cf_C02 (-5.81090e-2)
  #define cf_A04 (-3.21e-8)
  #define cf_C03 (3.3432e-4)
  #define cf_A10 (9.4742e-5)
  #define cf_C04 (-1.47797e-6)
  #define cf_A11 (-1.2583e-5)
  #define cf_C05 (3.1419e-9)
  #define cf_A12 (-6.4928e-8)
  #define cf_C10 (0.153563)
  #define cf_A13 (1.0515e-8)
  #define cf_C11 (6.8999e-4)
  #define cf_A14 (-2.0142e-10)
  #define cf_C12 (-8.1829e-6)
  #define cf_A20 (-3.9064e-7)
  #define cf_C13 (1.3632e-7)
  #define cf_A21 (9.1061e-9)
  #define cf_C14 (-6.1260e-10)
  #define cf_A22 (-1.6009e-10)
  #define cf_C20 (3.1260e-5)
  #define cf_A23 (7.994e-12)
  #define cf_C21 (-1.7111e-6)
  #define cf_A30 (1.100e-10)
  #define cf_C22 (2.5986e-8)
  #define cf_A31 (6.651e-12)
  #define cf_C23 (-2.5353e-10)
  #define cf_A32 (-3.391e-13)
  #define cf_C24 (1.0415e-12)
  #define cf_B00 (-1.922e-2)
  #define cf_C30 (-9.7729e-9)
  #define cf_B01 (-4.42e-5)
  #define cf_C31 (3.8513e-10)
  #define cf_B10 (7.3637e-5)
  #define cf_C32 (-2.3654e-12)
  #define cf_B11 (1.7950e-7)
  #define cf_A00 (1.389)
  #define cf_D00 (1.727e-3)
  #define cf_A01 (-1.262e-2)
  #define cf_D10 (-7.9836e-6)


  /**
  * Multipurpose map that links a depth [m] to a value ( temperature [C°], pressure [bar], salinity [ppu], 
  * or sound speed profile [m/s] )
  **/
  typedef ::std::map< PDouble, double > DepthMap;
  typedef DepthMap::const_iterator DConstIter;
  typedef DepthMap::iterator DIter;
  typedef DepthMap::reverse_iterator DRIter;
  typedef DepthMap::const_reverse_iterator DConstRIter;


  /**
  * Precision in meters of woss::PDouble objects representing depth of a default created woss::SSP
  * @see woss::SSP
  **/
  #define SSP_CUSTOM_DEPTH_PRECISION (1.0e-6)

  /**
  * Total depth steps of default transformation
  * @see SSP::transform
  **/
  #define SSP_CUSTOM_DEPTH_STEPS (20.0)


  /**
  * \brief SSP class offers multiple creation and manipulation capabilities for sound speed profile
  *
  * SSP can store all information related to a sound speed profile: temperature [C°], pressure [bar], salinity [psu] and sound speed [m/s].
  * It offers capabilities for arithmetic computations, sound speed calculations (Chen and Millero equations), depth to pressure conversions 
  * (and viceversa) with coordinates corrections, sound speed profile transformations and random perturbation.
  **/
  class SSP {

    
    public:

      
    /**
    * SSP default constructor. The object created is not valid
    * @param depth_precision precision of PDouble objects representing depth.
    **/  
    SSP( long double depth_precision = SSP_CUSTOM_DEPTH_PRECISION );

    /**
    * SSP constructor. 
    * @param ssp_map map linking a PDouble depth [m] (with precision depth_precision) to a sound speed [m/s]
    * @param temp_map map linking a PDouble depth [m] (with precision depth_precision) to a temperature [C°]
    * @param sal_map map linking a PDouble depth [m] (with precision depth_precision) to a salinity [ppu]
    * @param press_map map linking a PDouble depth [m] (with precision depth_precision) to a pressure [bar]
    * @param depth_precision depth precision of given maps [m] 
    **/  
    SSP( DepthMap& ssp_map, DepthMap& temp_map, DepthMap& sal_map, DepthMap& press_map, long double depth_precision = SSP_CUSTOM_DEPTH_PRECISION );

    /**
    * SSP constructor. 
    * @param ssp_map map linking a PDouble depth [m] (with precision depth_precision) to a sound speed [m/s]
    * @param depth_precision depth precision of given maps [m] 
    **/  
    SSP( DepthMap& ssp_map, long double depth_precision = SSP_CUSTOM_DEPTH_PRECISION );

    /**
    * SSP copy constructor
    * @param copy SSP to be copied
    **/
    SSP( const SSP& copy );

    virtual ~SSP() { }
    
    
    /**
    * SSP virtual factory method
    * @param depth_precision precision of PDouble objects representing depth.
    * @return a heap-created SSP object
    **/  
    virtual SSP* create( long double depth_precision = SSP_CUSTOM_DEPTH_PRECISION ) const { return new SSP(depth_precision); }

    /**
    * SSP virtual factory method
    * @param ssp_map map linking a PDouble depth [m] (with precision depth_precision) to a sound speed [m/s]
    * @param temp_map map linking a PDouble depth [m] (with precision depth_precision) to a temperature [C°]
    * @param sal_map map linking a PDouble depth [m] (with precision depth_precision) to a salinity [ppu]
    * @param press_map map linking a PDouble depth [m] (with precision depth_precision) to a pressure [bar]
    * @param depth_precision depth precision of given maps [m] 
    * @return a heap-created SSP object
    **/  
    virtual SSP* create( DepthMap& ssp_map, DepthMap& temp_map, DepthMap& sal_map, DepthMap& press_map, long double depth_precision = SSP_CUSTOM_DEPTH_PRECISION ) const {
      return new SSP( ssp_map, temp_map, sal_map, press_map, depth_precision); }

    /**
    * SSP virtual factory method
    * @param ssp_map map linking a PDouble depth [m] (with precision depth_precision) to a sound speed [m/s]
    * @param depth_precision depth precision of given maps [m] 
    * @return a heap-created SSP object
    **/  
    virtual SSP* create( DepthMap& ssp_map, long double depth_precision = SSP_CUSTOM_DEPTH_PRECISION ) const {
      return new SSP( ssp_map, depth_precision); }

    /**
    * SSP virtual factory method
    * @param copy SSP to be copied
    * @return a heap-created SSP object
    **/
    virtual SSP* create( const SSP& copy ) const { return new SSP( copy ); }
      
    /**
    * SSP virtual factory method
    * @return a heap-created copy of <b>this</b> instance
    **/
    virtual SSP* clone() const { return new SSP(*this); }
    
    
    /**
    * Returns the number of sound speed values stored
    * @return number of sound speed values stored
    **/
    int size() const { return( ssp_map.size() ); }


    /**
    * Checks if the instance has stored values
    * @return <i>true</i> if condition applies, <i>false</i> otherwise
    **/
    bool empty() const { return ssp_map.empty(); } 
    
    
    /**
    * Erase all values of sound speed, temperature, pressure and salinity. The object therefore is not valid
    **/
    void clear() { ssp_map.clear(); temperature_map.clear(); pressure_map.clear(); salinity_map.clear(); }


    /**
    * Returns a const iterator to the beginning of the sound speed map
    * @returns const iterator 
    **/
    DConstIter begin() const { return( ssp_map.begin() ); }
    
    /**
    * Returns a const iterator to the end of the sound speed map
    * @returns const iterator 
    **/
    DConstIter end() const { return( ssp_map.end() ); }

    /**
    * Returns a const reverse iterator to the reverse beginning of the sound speed map
    * @returns const reverse iterator 
    **/
    DConstRIter rbegin() const { return( ssp_map.rbegin() ); }

    /**
    * Returns a const reverse iterator to the reverse end of the sound speed map
    * @returns const reverse iterator 
    **/
    DConstRIter rend() const { return( ssp_map.rend() ); }


    /**
    * Returns a const iterator to the sound speed value with key >= of <i>depth</i> parameter
    * @param depth const reference to a PDouble depth value
    * @returns const iterator to end() if <i>depth</i> is not found
    **/
    DConstIter lower_bound( const PDouble& depth ) const { return( ssp_map.lower_bound(depth) ); }

    /**
    * Returns a const iterator to the sound speed value with key > of <i>depth</i> parameter 
    * @param depth const reference to a PDouble depth value
    * @returns const iterator to end() if <i>depth</i> is not found
    **/
    DConstIter upper_bound( const PDouble& depth ) const { return( ssp_map.upper_bound(depth) ); }
    
    /**
    * Returns a const iterator to the sound speed value at i-th position 
    * @param i integer should be between 0 and size() 
    * @returns const iterator to end() if position <i>i</i> is not found
    **/
    DConstIter at( const int i ) const ;
    

    /**
    * Returns a const iterator to the begin of the pressure map
    * @returns const iterator 
    **/
    DConstIter pressure_begin() const { return( pressure_map.begin() ); }
    
    /**
    * Returns a const iterator to the end of the pressure map
    * @returns const iterator 
    **/
    DConstIter pressure_end() const { return( pressure_map.end() ); }

    /**
    * Returns a const reverse iterator to the reverse begin of the pressure map
    * @returns const reverse iterator 
    **/
    DConstRIter pressure_rbegin() const { return( pressure_map.rbegin() ); }

    /**
    * Returns a const reverse iterator to the reverse end of the pressure map
    * @returns const reverse iterator 
    **/
    DConstRIter pressure_rend() const { return( pressure_map.rend() ); }


    /**
    * Returns a const iterator to the pressure value with key >= of <i>depth</i> parameter
    * @param depth const reference to a PDouble depth value
    * @returns const iterator to pressure_end() if <i>depth</i> is not found
    **/
    DConstIter pressure_lower_bound( const PDouble& depth ) const { return( pressure_map.lower_bound(depth) ); }

    /**
    * Returns a const iterator to the pressure value with key > of <i>depth</i> parameter
    * @param depth const reference to a PDouble depth value
    * @returns const iterator to pressure_end() if <i>depth</i> is not found
    **/
    DConstIter pressure_upper_bound( const PDouble& depth ) const { return( pressure_map.upper_bound(depth) ); }
    
    /**
    * Returns a const iterator to the pressure value with key == <i>depth</i> parameter
    * @param depth const reference to a PDouble depth value
    * @returns const iterator to pressure_end() if <i>depth</i> is not found
    **/
    DConstIter pressure_find( const PDouble& depth ) const { return( pressure_map.find(depth) ); }
    
    
    /**
    * Returns a const iterator to the beginning of the temperature map
    * @returns const reverse iterator 
    **/
    DConstIter temperature_begin() const { return( temperature_map.begin() ); }

    /**
    * Returns a const iterator to the end of the temperature map
    * @returns const reverse iterator 
    **/
    DConstIter temperature_end() const { return( temperature_map.end() ); }

    /**
    * Returns a const reverse iterator to the reverse beginning of the temperature map
    * @returns const reverse iterator 
    **/
    DConstRIter temperature_rbegin() const { return( temperature_map.rbegin() ); }

    /**
    * Returns a const reverse iterator to the reverse end of the temperature map
    * @returns const reverse iterator 
    **/
    DConstRIter temperature_rend() const { return( temperature_map.rend() ); }


    /**
    * Returns a const iterator to the temperature value with key >= of <i>depth</i> parameter
    * @param depth const reference to a PDouble depth value
    * @returns const iterator to temperature_end() if <i>depth</i> is not found
    **/
    DConstIter temperature_lower_bound( const PDouble& depth ) const { return( temperature_map.lower_bound(depth) ); }

    /**
    * Returns a const iterator to the temperature value with key > of <i>depth</i> parameter
    * @param depth const reference to a PDouble depth value
    * @returns const iterator to temperature_end() if <i>depth</i> is not found
    **/
    DConstIter temperature_upper_bound( const PDouble& depth ) const { return( temperature_map.upper_bound(depth) ); }

    /**
    * Returns a const iterator to the temperature value with key == of <i>depth</i> parameter
    * @param depth const reference to a PDouble depth value
    * @returns const iterator to temperature_end() if <i>depth</i> is not found
    **/
    DConstIter temperature_find( const PDouble& depth ) const { return( temperature_map.find(depth) ); }
    

    /**
    * Returns a const iterator to the beginning of the salinity map
    * @returns const reverse iterator 
    **/
    DConstIter salinity_begin() const { return( salinity_map.begin() ); }

    /**
    * Returns a const iterator to the end of the salinity map
    * @returns const reverse iterator 
    **/
    DConstIter salinity_end() const { return( salinity_map.end() ); }
      
    /**
    * Returns a const reverse iterator to the reverse beginning of the salinity map
    * @returns const reverse iterator 
    **/
    DConstRIter salinity_rbegin() const { return( salinity_map.rbegin() ); }

    /**
    * Returns a const reverse iterator to the reverse end of the salinity map
    * @returns const reverse iterator 
    **/
    DConstRIter salinity_rend() const { return( salinity_map.rend() ); }


    /**
    * Returns a const iterator to the salinity value with key >= of <i>depth</i> parameter
    * @param depth const reference to a PDouble depth value
    * @returns const iterator to salinity_end() if <i>depth</i> is not found
    **/
    DConstIter salinity_lower_bound( const PDouble& depth ) const { return( salinity_map.lower_bound(depth) ); }

    /**
    * Returns a const iterator to the salinity value with key > of <i>depth</i> parameter
    * @param depth const reference to a PDouble depth value
    * @returns const iterator to salinity_end() if <i>depth</i> is not found
    **/
    DConstIter salinity_upper_bound( const PDouble& depth ) const { return( salinity_map.upper_bound(depth) ); }

    /**
    * Returns a const iterator to the salinity value with key > of <i>depth</i> parameter
    * @param depth const reference to a PDouble depth value
    * @returns const iterator to salinity_end() if <i>depth</i> is not found
    **/
    DConstIter salinity_find( const PDouble& depth ) const { return( salinity_map.find(depth) ); }
    
    
    /**
    * Checks the validity of sound speed profile provided
    * @return <i>true</i> if it has at least one value, <i>false</i> otherwise
    **/
    virtual bool isValid() const { return( !ssp_map.empty() ); }

    /**
    * Checks if the sound speed profile provided can be transformed
    * @return <i>true</i> if it can, <i>false</i> otherwise
    **/
    virtual bool isTransformable() const { return( isValid() /*&& !temperature_map.empty() && !pressure_map.empty() && !salinity_map.empty() */ ); }

    /**
    * Checks if the sound speed profile provided can be randomly perturbated
    * @return <i>true</i> if it can, <i>false</i> otherwise
    **/
    virtual bool isRandomizable() const { return( isValid() && !temperature_map.empty() && !pressure_map.empty() && !salinity_map.empty() ); }
    

    /**
    * Transform the sound speed profile. If the current ssp can't be transformed it returns a not valid SSP.
    * If both depth are not changed while depth steps is increased, all data will be linearly interpolated.
    * If the SSP is extended in depth, sound speed will be calculated from last known temperature and salinity, 
    * while pressure will be calculated from depth conversion.
    * @param coordinates coordinates for pressure, depth conversion corrections
    * @param new_min_depth miminum depth [m] of the new SSP
    * @param new_max_depth maximum depth [m] of the new SSP
    * @param total_depth_steps number of depths of the new SSP
    * @return a <i>new</i> SSP object
    **/
    virtual SSP* transform( const Coord& coordinates, double new_min_depth = -HUGE_VAL, double new_max_depth = HUGE_VAL, int total_depth_steps = SSP_CUSTOM_DEPTH_STEPS ) const;

    /**
    * Performs a random perturbation of temperature, salinity, pressure with given ratio
    * Sound speed values are then calculated from this new data
    * @param ratio_incr_value perturbation ratio
    * @return a <i>new</i> SSP object
    **/
    virtual SSP* fullRandomize( double ratio_incr_value ) const;
    
    /**
    * Performs a random perturbation of sound speed values with given ratio
    * @param ratio_incr_value perturbation ratio
    * @return a <i>new</i> SSP object
    **/
    virtual SSP* randomize( double ratio_incr_value ) const;

    
    /**
    * Imports values in from the given stream
    * @param stream_in const reference to an istream instance
    * @return true if method was successful, false otherwise
    **/
    virtual bool import( ::std::istream& stream_in );
    
    /**
    * Write values out to the given stream
    * @param stream_out const reference to an ostream instance
    * @return true if method was successful, false otherwise
    **/
    virtual bool write( ::std::ostream& stream_out ) const;
    
    
    /**
    * Inserts and doesn't replace a sound speed value at given depth
    * @param depth depth value [m]. The corresponding PDouble will take SSP::depth_precision as precision
    * @param ssp_value sound speed value [m/s]
    * @return reference to <b>*this</b>
    **/
    SSP& insertValue( double depth, double ssp_value );

    /**
    * Inserts and doesn't replace a sound speed value at given depth
    * @param depth depth value [m]. The corresponding PDouble will take SSP::depth_precision as precision
    * @param temperature temperature value [C°]
    * @param salinity salinity value [ppu]
    * @param pressure pressure value [bar]
    * @param ssp_value sound speed value [m/s]
    * @return reference to <b>*this</b>
    **/
    SSP& insertValue( double depth, double temperature, double salinity, const ::std::complex<double>& pressure, double ssp_value );

    /**
    * Calculates and doesn't replace sound speed from given temperature, pressure and salinity value at depth converted from pressure.
    * @param coordinates coordinates for pressure-to-depth conversion corrections (defaults to CanonOcean)
    * @param temperature temperature value [C°]
    * @param salinity salinity value [ppu]
    * @param pressure pressure value [bar]
    * @return reference to <b>*this</b>
    **/
    SSP& insertValue( double temperature, double salinity, const ::std::complex<double>& pressure, const Coord& coordinates = Coord( 0.0, 0.0) );

    /**
    * Calculates and doesn't replace sound speed from given temperature, pressure and salinity value at given depth, with pressure converted from depth.
    * @param coordinates coordinates for depth-to-pressure conversion corrections (defaults to CanonOcean)
    * @param depth depth value [m]
    * @param temperature temperature value [C°]
    * @param salinity salinity value [ppu]
    * @return reference to <b>*this</b>
    **/
    SSP& insertValue( double depth, double temperature, double salinity, const Coord& coordinates = Coord(0.0, 0.0) );


    /**
    * Returns a const iterator to the sound speed value with key == of <i>depth</i> parameter
    * @param depth const reference to a PDouble depth value
    * @returns const iterator to end() if <i>depth</i> is not found
    **/
    DConstIter findValue( const double& depth ) const { return( ssp_map.find(depth) ); }
    

    /**
    * Erase the sound speed value with key == of <i>depth</i> parameter
    * @param depth const reference to a PDouble depth value
    * @return reference to <b>*this</b>
    **/
    SSP& eraseValue( const double& depth ) { ssp_map.erase(depth); return *this; }

  
    /**
    * Returns the maximum depth value
    * @returns maximum depth [m]
    **/
    double getMaxDepthValue() const { if ( ssp_map.rbegin() != ssp_map.rend() ) return ssp_map.rbegin()->first; 
                                      else return 0.0; }

    /**
    * Returns the minimum depth value
    * @returns minimum depth [m]
    **/
    double getMinDepthValue() const { if ( ssp_map.begin() != ssp_map.end() ) return ssp_map.begin()->first; 
                                      else return 0.0; }


    /**
    * Returns the maximum sound speed value
    * @returns maximum sound speed [m/s]
    **/
    double getMaxSSPValue() const { return( max_ssp_value ); }

    /**
    * Returns the maximum depth value
    * @returns maximum sound speed [m/s]
    **/
    double getMinSSPValue() const { return( min_ssp_value ); }
    

    /**
    * Sets the sound speed profile depth precision for all PDouble depth values. If the given precision is different from
    * current value, the profile will be modified accordingly
    * @param prec depth precision [m]
    **/
    virtual void setDepthPrecision( long double prec ); 

    /**
    * Returns the sound speed profile depth precision
    * @returns depth precision [m]
    **/
    long double getDepthPrecision() const { return depth_precision; }
    
    
    /**
    * Assignment operator
    * @param copy const reference to a SSP object to be copied
    * @return SSP reference to <i>this</i>
    **/
    SSP& operator=( const SSP& x ) ;


    /**
    * Equality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left == right</i>, false otherwise
    **/ 
    friend bool operator==( const SSP& left, const SSP& right );

    /**
    * Inequality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left == right</i>, false otherwise
    **/ 
    friend bool operator!=( const SSP& left, const SSP& right );


    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a const instance holding the operation result  
    **/
    friend const SSP operator+( const SSP& left, const SSP& right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const SSP operator-( const SSP& left, const SSP& right );
    
    /**
    * Multiplication operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const SSP operator*( const SSP& left, const SSP& right );

    /**
    * Division operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const SSP operator/( const SSP& left, const SSP& right );
    

    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const SSP operator+( const SSP& left, const double right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const SSP operator-( const SSP& left, const double right );

    /**
    * Division operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const SSP operator/( const SSP& left, const double right );

    /**
    * Multiplication operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const SSP operator*( const SSP& left, const double right );


    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const SSP operator+( const double left, const SSP& right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const SSP operator-( const double left, const SSP& right );

    /**
    * Division operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const SSP operator/( const double left, const SSP& right );

    /**
    * Multiplication operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const SSP operator*( const double left, const SSP& right );
    

    /**
    * Compound assignment sum operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend SSP& operator+=( SSP& left, const SSP& right );

    /**
    * Compound assignment subtraction operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend SSP& operator-=( SSP& left, const SSP& right );
    
    /**
    * Compound assignment multiplication operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend SSP& operator*=( SSP& left, const SSP& right );

    /**
    * Compound assignment division operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend SSP& operator/=( SSP& left, const SSP& right );
    

    /**
    * Compound assignment sum operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend SSP& operator+=( SSP& left, const double right );

    /**
    * Compound assignment subtraction operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend SSP& operator-=( SSP& left, const double right );
    
    /**
    * Compound assignment division operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend SSP& operator/=( SSP& left, const double right );

    /**
    * Compound assignment multiplication operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend SSP& operator*=( SSP& left, const double right );

    
    /**
    * << operator
    * @param os left operand ostream reference
    * @param instance right operand const SSP reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend ::std::ostream& operator<<( ::std::ostream& os, const SSP& instance );
    
    /**
    * >> operator
    * @param os left operand ostream reference
    * @param instance right operand const SSP reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend ::std::ostream& operator>>( ::std::istream& is, const SSP& instance );
    
    
    /**
    * Sets debug flag for all instances
    * @param flag debug bool
    **/
    static void setDebug( bool flag ) { debug = flag; }
        
    
    protected:

      
    /**
    * Stores the miminum sound speed value [m/s]
    **/    
    double min_ssp_value;

    /**
    * Stores the maximum sound speed value [m/s]
    **/ 
    double max_ssp_value;

    /**
    * Stores the precision of all PDouble depth instances [m]
    **/ 
    long double depth_precision;

    
    /**
    * Stores the common debug flag
    **/ 
    static bool debug;
       

    /**
    * Sound speed values map 
    **/ 
    DepthMap ssp_map;

    /**
    * Pressure values map
    **/ 
    DepthMap pressure_map;

    /**
    * Salinity values map
    **/ 
    DepthMap salinity_map;

    /**
    * Temperature values map
    **/ 
    DepthMap temperature_map;
  

    /**
    * Calculates sound speed from temperature, salinity, pressure with UNESCO Chen and Millero equations and with Wong and Zu
    * corrections
    * @param temperature temperature value [C°]
    * @param salinity salinity value [ppu]
    * @param pressure pressure value [bar]
    **/ 
    virtual double calculateSSP( double temperature, double salinity, double pressure ) const; 


    private:


    /**
    * Checks if coordinates provided are in canon ocean
    * @param coordinates coordinates value 
    * @return <i>true</i> if condition holds, <i>false</i> otherwise
    **/ 
    bool isCanonOcean( const Coord& coordinates ) const { return( coordinates.getLatitude() >= -40.0 && coordinates.getLatitude() <= 60.0 ); }

    /**
    * Checks if coordinates provided are in north eastern Atlantic Ocean
    * @param coordinates coordinates value 
    * @return <i>true</i> if condition holds, <i>false</i> otherwise
    **/ 
    bool isNEAtlanticOcean( const Coord& coordinates ) const { return( coordinates.getLatitude() >= 30.0 && coordinates.getLatitude() <= 60.0 
                                                                    && coordinates.getLongitude() >= -43.0 && coordinates.getLongitude() <= -5.0 ); }

    /**
    * Checks if coordinates provided are in Antarctic Ocean
    * @param coordinates coordinates value 
    * @return <i>true</i> if condition holds, <i>false</i> otherwise
    **/ 
    bool isAntarcticOcean( const Coord& coordinates ) const { return( coordinates.getLatitude() <= -55.0 ); }

    /**
    * Checks if coordinates provided are in Mediterranean Sea
    * @param coordinates coordinates value 
    * @return <i>true</i> if condition holds, <i>false</i> otherwise
    **/ 
    bool isMediterraneanSea( const Coord& coordinates ) const { return( coordinates.getLatitude() >= 30.1 && coordinates.getLatitude() <= 46.0 
                                                                    && coordinates.getLongitude() >= -4.0 && coordinates.getLongitude() <= 37.0 ); }

    /**
    * Checks if coordinates provided are in Red Sea
    * @param coordinates coordinates value 
    * @return <i>true</i> if condition holds, <i>false</i> otherwise
    **/ 
    bool isRedSea( const Coord& coordinates ) const { return( coordinates.getLatitude() >= 12.0 && coordinates.getLatitude() <= 30.0 
                                                          && coordinates.getLongitude() >= 32.0 && coordinates.getLongitude() <= 44.0 ); }

    /**
    * Checks if coordinates provided are in Arctic Ocean 
    * @param coordinates coordinates value 
    * @return <i>true</i> if condition holds, <i>false</i> otherwise
    **/ 
    bool isArcticOcean( const Coord& coordinates ) const { return( coordinates.getLatitude() >= 65.0 ); }
    
    /**
    * Checks if coordinates provided are in Japan Sea
    * @param coordinates coordinates value 
    * @return <i>true</i> if condition holds, <i>false</i> otherwise
    **/ 
    bool isJapanSea( const Coord& coordinates ) const { return( coordinates.getLatitude() >= 33.0 && coordinates.getLatitude() <= 52.0 
                                                            && coordinates.getLongitude() >= 127.0 && coordinates.getLongitude() <= 142.0 ); }

    /**
    * Checks if coordinates provided are in Sulu Sea
    * @param coordinates coordinates value 
    * @return <i>true</i> if condition holds, <i>false</i> otherwise
    **/ 
    bool isSuluSea( const Coord& coordinates ) const { return( coordinates.getLatitude() >= 5.0 && coordinates.getLatitude() <= 13.0 
                                                            && coordinates.getLongitude() >= 117.0 && coordinates.getLongitude() <= 123.0 ); }

    /**
    * Checks if coordinates provided are in Halmahera Sea
    * @param coordinates coordinates value 
    * @return <i>true</i> if condition holds, <i>false</i> otherwise
    **/ 
    bool isHalmaheraSea( const Coord& coordinates ) const { return( coordinates.getLatitude() >= -2.0 && coordinates.getLatitude() <= 1.0 
                                                                && coordinates.getLongitude() >= 127.0 && coordinates.getLongitude() <= 130.0 ); }

    /**
    * Checks if coordinates provided are in Celebes Sea
    * @param coordinates coordinates value 
    * @return <i>true</i> if condition holds, <i>false</i> otherwise
    **/ 
    bool isCelebesSea( const Coord& coordinates ) const { return( coordinates.getLatitude() >= 0.0 && coordinates.getLatitude() <= 7.0 
                                                              && coordinates.getLongitude() >= 115.0 && coordinates.getLongitude() <= 125.0 ); }

    /**
    * Checks if coordinates provided are in Black Sea
    * @param coordinates coordinates value 
    * @return <i>true</i> if condition holds, <i>false</i> otherwise
    **/ 
    bool isBlackSea( const Coord& coordinates ) const { return( coordinates.getLatitude() >= 41.0 && coordinates.getLatitude() <= 48.0 
                                                            && coordinates.getLongitude() >= 28.0 && coordinates.getLongitude() <= 42.0 ); }

    /**
    * Checks if coordinates provided are in Baltic Sea
    * @param coordinates coordinates value 
    * @return <i>true</i> if condition holds, <i>false</i> otherwise
    **/ 
    bool isBalticSea( const Coord& coordinates ) const { return( coordinates.getLatitude() >= 53.0 && coordinates.getLatitude() <= 66.0 
                                                              && coordinates.getLongitude() >= 20.0 && coordinates.getLongitude() <= 26.0 ); }

    /**
    * Equation for pressure from depth conversion ( Leroy and Parthiot )
    * @param z depth [m]
    **/ 
    double thyh( double z ) const ; 

    /**
    * Equation for pressure from depth conversion ( Leroy and Parthiot )
    * @param lat latitude [decimal degree]
    **/ 
    double g( double lat ) const ;

    /**
    * Equation for pressure from depth conversion ( Leroy and Parthiot )
    * @param z depth [m]
    * @param lat latitude [decimal degree]
    **/ 
    double k( double z, double lat ) const ; 

    /**
    * Equation for pressure from depth conversion ( Leroy and Parthiot )
    * @param z depth [m]
    **/ 
    double hq( double z ) const ;

    /**
    * Equation for pressure from depth conversion ( Leroy and Parthiot )
    * @param z depth [m]
    * @param lat latitude [decimal degree]
    **/ 
    double h( double z, double lat ) const ;

    /**
    * Returns pressure correction for given coordinates and depth ( Leroy and Parthiot )
    * @param coordinates coordinates provided
    * @param depth depth provided [m]
    **/ 
    double getPressureCorreptions( const Coord& coordinates, double depth ) const ;
    
    /**
    * Returns pressure for given coordinates and depth ( Leroy and Parthiot )
    * @param coordinates coordinates provided
    * @param depth depth provided [m]
    * @return pressure [bar]
    **/  
    double getPressureFromDepth( const Coord& coordinates, double depth ) const ;


    /**
    * Equation for depth from pressure conversion ( Leroy and Parthiot )
    * @param lat latitude [decimal degree]
    **/ 
    double g_z( double lat ) const ; 

    /**
    * Returns depth correction for given coordinates and pressure ( Leroy and Parthiot )
    * @param coordinates coordinates provided
    * @param pressure pressure provided [bar]
    **/ 
    double getDepthCorreptions( const Coord& coordinates, double pressure ) const ;

    /**
    * Returns depth for given coordinates and pressure ( Leroy and Parthiot )
    * @param coordinates coordinates provided
    * @param pressure pressure provided [bar]
    * @return depth [m]
    **/  
    double getDepthfromPressure( const Coord& coordinates, double pressure ) const ;
    

    /**
    * UNESCO Chen and Millero Equation with Wong and Zu corrections for sound speed calculations
    * @param t temperature provided [C°]
    * @param p pressure provided [bar]
    **/ 
    double d( double t, double p ) const ;

    /**
    * UNESCO Chen and Millero Equation with Wong and Zu corrections for sound speed calculations
    * @param t temperature provided [C°]
    * @param p pressure provided [bar]
    **/ 
    double b( double t, double p ) const ;

    /**
    * UNESCO Chen and Millero Equation with Wong and Zu corrections for sound speed calculations
    * @param t temperature provided [C°]
    * @param p pressure provided [bar]
    **/ 
    double a( double t, double p  ) const ; 

    /**
    * UNESCO Chen and Millero Equation with Wong and Zu corrections for sound speed calculations
    * @param t temperature provided [C°]
    * @param p pressure provided [bar]
    **/ 
    double cw( double t, double p ) const ;
    

  };

  //non-inline operator declarations
  /////////////
  
  SSP& operator+=( SSP& left, const SSP& right );

  SSP& operator-=( SSP& left, const SSP& right );

  SSP& operator*=( SSP& left, const SSP& right );

  SSP& operator/=( SSP& left, const SSP& right );


  SSP& operator+=( SSP& left, const double right );

  SSP& operator-=( SSP& left, const double right );

  SSP& operator/=( SSP& left, const double right );

  SSP& operator*=( SSP& left, const double right );

  
  //inline functions
  ////
  inline bool operator==( const SSP& left, const SSP& right ) {
    if ( &left == &right ) return true; 
    return( left.ssp_map == right.ssp_map && left.temperature_map == right.temperature_map && left.pressure_map == right.pressure_map 
        && left.salinity_map == right.salinity_map );
  }


  inline bool operator!=( const SSP& left, const SSP& right ) {
    if ( &left == &right ) return false; 
    return( left.ssp_map != right.ssp_map || left.temperature_map != right.temperature_map || left.pressure_map != right.pressure_map 
        || left.salinity_map == right.salinity_map ); }


  inline const SSP operator+( const SSP& left, const SSP& right ) {
    SSP ret_val ( left );
    ret_val += right;
    return ret_val;
  }


  inline const SSP operator-( const SSP& left, const SSP& right ) {
    SSP ret_val ( left );
    ret_val -= right;
    return ret_val;
  }


  inline const SSP operator*( const SSP& left, const SSP& right ) {
    SSP ret_val ( left );
    ret_val *= right;

    return ret_val;
  }


  inline const SSP operator/( const SSP& left, const SSP& right ) {
    SSP ret_val ( left );
    ret_val /= right;
    return ret_val;
  }


  inline const SSP operator+( const SSP& left, const double right ) {
    SSP ret_val ( left );
    ret_val += right;
    return ret_val;
  }


  inline const SSP operator-( const SSP& left, const double right ) {
    SSP ret_val ( left );
    ret_val -= right;
    return ret_val;
  }


  inline const SSP operator/( const SSP& left, const double right ) {
    SSP ret_val ( left );
    ret_val /= right;
    return ret_val;
  }


  inline const SSP operator*( const SSP& left, const double right ) {
    SSP ret_val ( left );
    ret_val *= right;
    return ret_val;
  }


  inline const SSP operator+( const double left, const SSP& right ) {
    SSP ret_val ( right );
    ret_val += left;
    return ret_val;
  }


  inline const SSP operator-( const double left, const SSP& right ) {
    SSP ret_val ( right );
    ret_val -= left;
    return ret_val;
  }


  inline const SSP operator/( const double left, const SSP& right ) {
    SSP ret_val ( right );
    ret_val /= left;
    return ret_val;
  }


  inline const SSP operator*( const double left, const SSP& right ) {
    SSP ret_val ( right );
    ret_val *= left;
    return ret_val;
  }


  inline ::std::ostream& operator<<( ::std::ostream& os, const SSP& instance ) {
    if ( !instance.write( os ) ) os.setstate( ::std::ios_base::failbit );
    return os;
  }


  inline ::std::istream& operator>>( ::std::istream& is, SSP& instance ) {
    if ( !instance.import( is ) ) is.setstate( ::std::ios_base::failbit );
    return is;
  }


  inline double SSP::thyh( double z ) const {
    return (1.0e-2 * z/(z+100) + 6.2e-6 * z);
  }


  inline double SSP::g( double lat ) const {
    return( 9.7803*(1.0 + 5.3e-3 * pow(sin(lat),2.0)) ); 
  }


  inline double SSP::k( double z, double lat ) const { 
    return( (g(lat) - 2e-5 * z) / (9.80612 - 2e-5 * z) ); 
  }


  inline double SSP::hq( double z ) const { 
    return( 1.00818e-2 * z + 2.465e-8 * pow(z,2.0) - 1.25e-13 * pow(z,3.0) + 2.8e-19 * pow(z,4.0) ); 
  }


  inline double SSP::h( double z, double lat ) const { 
    return( hq(z) * k(z,lat) ); 
  }


  inline double SSP::getPressureFromDepth( const Coord& coordinates, double depth ) const { 
    // conversion in bar from MPa
    return( 10.0 * ( h(depth,coordinates.getLatitude()) - thyh(depth) - getPressureCorreptions(coordinates, depth) ) );  
  }


  inline double SSP::g_z( double lat ) const { 
    return( 9.780318 * ( 1.0 + 5.2788 * 10e-3 * pow(sin(lat), 2.0) + 2.36 * 10e-5 * pow(sin(lat),4.0) ) ); 
  }


  inline double SSP::getDepthfromPressure( const Coord& coordinates, double pressure ) const {
    // conversion from bar to MPa
    pressure /= 10.0;
    return( ( (9.72659e2 * pressure - 2.2512e-1 * pow(pressure, 2.0) + 2.279e-4 * pow(pressure, 3.0) - 1.82e-7 * pow(pressure, 4.0) ) 
    / ( g_z(coordinates.getLatitude()) + 1.092e-4 * pressure ) ) + getDepthCorreptions(coordinates, pressure)  );
  }



  inline double SSP::calculateSSP(double temperature, double salinity, double pressure) const {
    return( cw(temperature, pressure) + a(temperature, pressure)*salinity 
      + b(temperature, pressure) * ::std::sqrt(salinity*salinity*salinity) 
      + d(temperature, pressure) * (salinity*salinity) );
  }


  inline double SSP::d(double t, double p) const { 
    return( cf_D00 + cf_D10*p ); 
  }


  inline double SSP::b( double t, double p) const { 
    return( cf_B00 + cf_B01*t + (cf_B10 + cf_B11*t)*p ); 
  }


  inline double SSP::a(double t, double p) const {
    return( (cf_A00 + cf_A01*t + cf_A02*pow(t,2.0) + cf_A03*pow(t,3.0) + cf_A04*pow(t,4.0)) 
    + (cf_A10 + cf_A11*t + cf_A12*pow(t,2.0) + cf_A13*pow(t,3.0) + cf_A14*pow(t,4.0))*p 
    + (cf_A20 + cf_A21*t + cf_A22*pow(t,2.0) + cf_A23*pow(t,3.0))*pow(p,2.0) 
    + (cf_A30 + cf_A31*t + cf_A32*pow(t,2.0))*pow(p,3.0) );
  }


  inline double SSP::cw( double t, double p) const {
    return( (cf_C00 + cf_C01*t + cf_C02*pow(t,2.0) + cf_C03*pow(t,3.0) + cf_C04*pow(t,4.0) + cf_C05*pow(t,5.0)) 
    + (cf_C10 + cf_C11*t + cf_C12*pow(t,2.0) + cf_C13*pow(t,3.0) + cf_C14*pow(t,4.0))*p       
    + (cf_C20 + cf_C21*t + cf_C22*pow(t,2.0) + cf_C23*pow(t,3.0) + cf_C24*pow(t,4.0))*pow(p,2.0)   
    + (cf_C30 + cf_C31*t + cf_C32*pow(t,2.0))*pow(p,3.0) );
  }

}


#endif /* WOSS_SSP_DEFINITIONS_H */ 


