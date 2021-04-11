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

  // TEOS-10 constants
  #define TEO_10_c000  -6.07991438090e-5
  #define TEO_10_c001  1.99712338438e-5
  #define TEO_10_c002  -3.39280843110e-6
  #define TEO_10_c003  4.21246123200e-7
  #define TEO_10_c004  -6.32363064300e-8
  #define TEO_10_c005  1.17681023580e-8
  #define TEO_10_c010  1.85057654290e-5
  #define TEO_10_c011  -2.34727734620e-6
  #define TEO_10_c012  -1.09581019659e-6
  #define TEO_10_c013  1.25816399608e-6
  #define TEO_10_c020  -1.17166068530e-5
  #define TEO_10_c021  4.26100574800e-6
  #define TEO_10_c022  8.60877154770e-7
  #define TEO_10_c030  7.92796561730e-6
  #define TEO_10_c031  -9.22650800740e-7
  #define TEO_10_c040  -3.41021874820e-6
  #define TEO_10_c041  -1.26705833028e-7
  #define TEO_10_c050  5.07367668140e-7
  #define TEO_10_c100  2.42624687470e-5
  #define TEO_10_c101  -1.16968865968e-6
  #define TEO_10_c102  1.08930565545e-6
  #define TEO_10_c103  -4.45885016920e-7
  #define TEO_10_c110  -9.56770881560e-6
  #define TEO_10_c111  -1.11398309114e-5
  #define TEO_10_c112  -8.18870887110e-7
  #define TEO_10_c120  -2.36783083610e-7
  #define TEO_10_c121  7.82747741600e-7
  #define TEO_10_c130  -3.45587736550e-6
  #define TEO_10_c131  1.55237776184e-8
  #define TEO_10_c140  1.29567177830e-6
  #define TEO_10_c200  -3.47924609740e-5
  #define TEO_10_c201  -9.62445031940e-6
  #define TEO_10_c202  5.02389113400e-8
  #define TEO_10_c210  1.11008347650e-5
  #define TEO_10_c211  1.09241497668e-5
  #define TEO_10_c220  2.92833462950e-6
  #define TEO_10_c221  -1.31462208134e-6
  #define TEO_10_c230  3.16553060780e-7
  #define TEO_10_c300  3.74707773050e-5
  #define TEO_10_c301  9.85262139960e-6
  #define TEO_10_c310  -9.84471178440e-6
  #define TEO_10_c311  -2.70883712540e-6
  #define TEO_10_c320  -4.88261392000e-7
  #define TEO_10_c400  -1.73222186120e-5
  #define TEO_10_c401  -3.56239494540e-6
  #define TEO_10_c410  2.59092252600e-6
  #define TEO_10_c500  3.09274272530e-6
  #define TEO_10_v000  1.0769995862e-3
  #define TEO_10_v001  -6.0799143809e-5
  #define TEO_10_v002  9.9856169219e-6
  #define TEO_10_v003  -1.1309361437e-6
  #define TEO_10_v004  1.0531153080e-7
  #define TEO_10_v005  -1.2647261286e-8
  #define TEO_10_v006  1.9613503930e-9
  #define TEO_10_v010  -3.1038981976e-4
  #define TEO_10_v011  2.4262468747e-5
  #define TEO_10_v012  -5.8484432984e-7
  #define TEO_10_v013  3.6310188515e-7
  #define TEO_10_v014  -1.1147125423e-7
  #define TEO_10_v020  6.6928067038e-4
  #define TEO_10_v021  -3.4792460974e-5
  #define TEO_10_v022  -4.8122251597e-6
  #define TEO_10_v023  1.6746303780e-8
  #define TEO_10_v030  -8.5047933937e-4
  #define TEO_10_v031  3.7470777305e-5
  #define TEO_10_v032  4.9263106998e-6
  #define TEO_10_v040  5.8086069943e-4
  #define TEO_10_v041  -1.7322218612e-5
  #define TEO_10_v042  -1.7811974727e-6
  #define TEO_10_v050  -2.1092370507e-4
  #define TEO_10_v051  3.0927427253e-6
  #define TEO_10_v060  3.1932457305e-5
  #define TEO_10_v100  -1.5649734675e-5
  #define TEO_10_v101  1.8505765429e-5
  #define TEO_10_v102  -1.1736386731e-6
  #define TEO_10_v103  -3.6527006553e-7
  #define TEO_10_v104  3.1454099902e-7
  #define TEO_10_v110  3.5009599764e-5
  #define TEO_10_v111  -9.5677088156e-6
  #define TEO_10_v112  -5.5699154557e-6
  #define TEO_10_v113  -2.7295696237e-7
  #define TEO_10_v120  -4.3592678561e-5
  #define TEO_10_v121  1.1100834765e-5
  #define TEO_10_v122  5.4620748834e-6
  #define TEO_10_v130  3.4532461828e-5
  #define TEO_10_v131  -9.8447117844e-6
  #define TEO_10_v132  -1.3544185627e-6
  #define TEO_10_v140  -1.1959409788e-5
  #define TEO_10_v141  2.5909225260e-6
  #define TEO_10_v150  1.3864594581e-6
  #define TEO_10_v200  2.7762106484e-5
  #define TEO_10_v201  -1.1716606853e-5
  #define TEO_10_v202  2.1305028740e-6
  #define TEO_10_v203  2.8695905159e-7
  #define TEO_10_v210  -3.7435842344e-5
  #define TEO_10_v211  -2.3678308361e-7
  #define TEO_10_v212  3.9137387080e-7
  #define TEO_10_v220  3.5907822760e-5
  #define TEO_10_v221  2.9283346295e-6
  #define TEO_10_v222  -6.5731104067e-7
  #define TEO_10_v230  -1.8698584187e-5
  #define TEO_10_v231  -4.8826139200e-7
  #define TEO_10_v240  3.8595339244e-6
  #define TEO_10_v300  -1.6521159259e-5
  #define TEO_10_v301  7.9279656173e-6
  #define TEO_10_v302  -4.6132540037e-7
  #define TEO_10_v310  2.4141479483e-5
  #define TEO_10_v311  -3.4558773655e-6
  #define TEO_10_v312  7.7618888092e-9
  #define TEO_10_v320  -1.4353633048e-5
  #define TEO_10_v321  3.1655306078e-7
  #define TEO_10_v330  2.2863324556e-6
  #define TEO_10_v400  6.9111322702e-6
  #define TEO_10_v401  -3.4102187482e-6
  #define TEO_10_v402  -6.3352916514e-8
  #define TEO_10_v410  -8.7595873154e-6
  #define TEO_10_v411  1.2956717783e-6
  #define TEO_10_v420  4.3703680598e-6
  #define TEO_10_v500  -8.0539615540e-7
  #define TEO_10_v501  5.0736766814e-7
  #define TEO_10_v510  -3.3052758900e-7
  #define TEO_10_v600  2.0543094268e-7

  #define TEO_10_gsw_sfac (0.0248826675584615)
  #define TEO_10_offset  (5.971840214030754e-1)

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
     * SSP EQ 
     */
    enum SSPEqType {
      SSP_EQ_CHEN_MILLERO = 0, ///< UNESCO Chen & Miller, Wong Zu Eq
      SSP_EQ_TEOS_10 = 1, ///< TEOS-10 Eq (@see reference [10])
      SSP_EQ_TEOS_10_EXACT = 2, ///< TEOS-10 Exact Eq (@see reference [10])
      SSP_EQ_INVALID ///< Must always be the last
    };

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
    * It returns a new SSP object, truncated at the input depth. 
    * If the current ssp can't be truncated it returns a not valid SSP.
    * @param max_depth truncation depth [m]
    * @return a <i>new</i> SSP object
    **/
    virtual SSP* truncate( double max_depth ) const;

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
    * Sets the SSP EQ that should be used by when inserting data
    * @param eq_type valid SSPEqType
    * @return reference to <b>*this</b>
    **/
    SSP& setSSPEqType(SSPEqType eq_type) { ssp_eq_type = eq_type; return *this; }

    /**
    * Gets the current SSPEqType
    * @return current SSPEqType
    **/
    SSPEqType getSSPEqType() const {return ssp_eq_type; }

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
    * Stores the SSP eq type that should be used
    **/
    SSPEqType ssp_eq_type;
    
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

    /**
    * Seawater specific Gibbs free energy and derivatives up to order 2
    * @param ns order of salinity derivative
    * @param nt order of temperature derivative
    * @param np order of pressure derivative
    * @param sa salinity [ppu]
    * @param t temperature [C°]
    * @param p pressure [dbar]
    **/ 
    double gibbs(int ns, int nt, int np, double sa, double t, double p) const ;

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
    if (ssp_eq_type == SSP_EQ_CHEN_MILLERO)
    {
      return( cw(temperature, pressure) + a(temperature, pressure)*salinity 
        + b(temperature, pressure) * ::std::sqrt(salinity*salinity*salinity) 
        + d(temperature, pressure) * (salinity*salinity) );
    }
    else if (ssp_eq_type == SSP_EQ_TEOS_10)
    {
      double  v, v_p, xs, ys, z;

      pressure *= 10; // formula requires dbar instead of bar

      xs  = ::std::sqrt(TEO_10_gsw_sfac*salinity + TEO_10_offset);
      ys  = temperature*0.025;
      z = pressure*1e-4;

      v = TEO_10_v000
        + xs*(TEO_10_v010 + xs*(TEO_10_v020 + xs*(TEO_10_v030 + xs*(TEO_10_v040 + xs*(TEO_10_v050
        + TEO_10_v060*xs))))) + ys*(TEO_10_v100 + xs*(TEO_10_v110 + xs*(TEO_10_v120 + xs*(TEO_10_v130 + xs*(TEO_10_v140
        + TEO_10_v150*xs)))) + ys*(TEO_10_v200 + xs*(TEO_10_v210 + xs*(TEO_10_v220 + xs*(TEO_10_v230 + TEO_10_v240*xs)))
        + ys*(TEO_10_v300 + xs*(TEO_10_v310 + xs*(TEO_10_v320 + TEO_10_v330*xs)) + ys*(TEO_10_v400 + xs*(TEO_10_v410
        + TEO_10_v420*xs) + ys*(TEO_10_v500 + TEO_10_v510*xs + TEO_10_v600*ys))))) + z*(TEO_10_v001 + xs*(TEO_10_v011
        + xs*(TEO_10_v021 + xs*(TEO_10_v031 + xs*(TEO_10_v041 + TEO_10_v051*xs)))) + ys*(TEO_10_v101 + xs*(TEO_10_v111
        + xs*(TEO_10_v121 + xs*(TEO_10_v131 + TEO_10_v141*xs))) + ys*(TEO_10_v201 + xs*(TEO_10_v211 + xs*(TEO_10_v221
        + TEO_10_v231*xs)) + ys*(TEO_10_v301 + xs*(TEO_10_v311 + TEO_10_v321*xs) + ys*(TEO_10_v401 + TEO_10_v411*xs
        + TEO_10_v501*ys)))) + z*(TEO_10_v002 + xs*(TEO_10_v012 + xs*(TEO_10_v022 + xs*(TEO_10_v032 + TEO_10_v042*xs)))
        + ys*(TEO_10_v102 + xs*(TEO_10_v112 + xs*(TEO_10_v122 + TEO_10_v132*xs)) + ys*(TEO_10_v202 + xs*(TEO_10_v212
        + TEO_10_v222*xs) + ys*(TEO_10_v302 + TEO_10_v312*xs + TEO_10_v402*ys))) + z*(TEO_10_v003 + xs*(TEO_10_v013
        + TEO_10_v023*xs) + ys*(TEO_10_v103 + TEO_10_v113*xs + TEO_10_v203*ys) + z*(TEO_10_v004 + TEO_10_v014*xs + TEO_10_v104*ys
        + z*(TEO_10_v005 + TEO_10_v006*z)))));

      v_p = TEO_10_c000
        + xs*(TEO_10_c100 + xs*(TEO_10_c200 + xs*(TEO_10_c300 + xs*(TEO_10_c400 + TEO_10_c500*xs))))
        + ys*(TEO_10_c010 + xs*(TEO_10_c110 + xs*(TEO_10_c210 + xs*(TEO_10_c310 + TEO_10_c410*xs))) + ys*(TEO_10_c020
        + xs*(TEO_10_c120 + xs*(TEO_10_c220 + TEO_10_c320*xs)) + ys*(TEO_10_c030 + xs*(TEO_10_c130 + TEO_10_c230*xs)
        + ys*(TEO_10_c040 + TEO_10_c140*xs + TEO_10_c050*ys)))) + z*(TEO_10_c001 + xs*(TEO_10_c101 + xs*(TEO_10_c201
        + xs*(TEO_10_c301 + TEO_10_c401*xs))) + ys*(TEO_10_c011 + xs*(TEO_10_c111 + xs*(TEO_10_c211 + TEO_10_c311*xs))
        + ys*(TEO_10_c021 + xs*(TEO_10_c121 + TEO_10_c221*xs) + ys*(TEO_10_c031 + TEO_10_c131*xs + TEO_10_c041*ys)))
        + z*( TEO_10_c002 + xs*(TEO_10_c102 + TEO_10_c202*xs) + ys*(TEO_10_c012 + TEO_10_c112*xs + TEO_10_c022*ys)
        + z*(TEO_10_c003 + TEO_10_c103*xs + TEO_10_c013*ys + z*(TEO_10_c004 + TEO_10_c005*z))));

      return (10000.0*::std::sqrt(-v*v/v_p));
    }
    else if (ssp_eq_type == SSP_EQ_TEOS_10_EXACT)
    {
      int n0=0, n1=1, n2=2;
      double  g_tt, g_tp;

      pressure *= 10; // formulas require dbar instead of bar

      g_tt  = gibbs(n0, n2, n0, salinity, temperature, pressure);
      g_tp  = gibbs(n0, n1, n1, salinity, temperature, pressure);

      return (gibbs(n0, n0, n1, salinity, temperature, pressure) * ::std::sqrt(g_tt/(g_tp*g_tp - g_tt*gibbs(n0, n0, n2, salinity, temperature, pressure))));
    }
    else return -HUGE_VAL;
  }

  inline double SSP::gibbs(int ns, int nt, int np, double sa, double t, double p) const
  {
    double x2, x, y, z, g03, g08, return_value = 0.0;

    x2	= TEO_10_gsw_sfac*sa;
    x = sqrt(x2);
    y = t*0.025;
    z = p*1e-4;

    if (ns == 0  && nt == 0  && np == 0) {
      g03 = 101.342743139674 + z*(100015.695367145 +
          z*(-2544.5765420363 + z*(284.517778446287 +
          z*(-33.3146754253611 + (4.20263108803084 -
              0.546428511471039*z)*z)))) +
          y*(5.90578347909402 + z*(-270.983805184062 +
          z*(776.153611613101 + z*(-196.51255088122 +
              (28.9796526294175 - 2.13290083518327*z)*z))) +
          y*(-12357.785933039 + z*(1455.0364540468 +
          z*(-756.558385769359 + z*(273.479662323528 +
              z*(-55.5604063817218 + 4.34420671917197*z)))) +
          y*(736.741204151612 + z*(-672.50778314507 +
          z*(499.360390819152 + z*(-239.545330654412 +
              (48.8012518593872 - 1.66307106208905*z)*z))) +
          y*(-148.185936433658 + z*(397.968445406972 +
          z*(-301.815380621876 + (152.196371733841 -
              26.3748377232802*z)*z)) +
          y*(58.0259125842571 + z*(-194.618310617595 +
          z*(120.520654902025 + z*(-55.2723052340152 +
              6.48190668077221*z))) +
          y*(-18.9843846514172 + y*(3.05081646487967 -
              9.63108119393062*z) +
          z*(63.5113936641785 + z*(-22.2897317140459 +
              8.17060541818112*z))))))));

      g08 = x2*(1416.27648484197 + z*(-3310.49154044839 +
          z*(384.794152978599 + z*(-96.5324320107458 +
            (15.8408172766824 - 2.62480156590992*z)*z))) +
          x*(-2432.14662381794 + x*(2025.80115603697 +
          y*(543.835333000098 + y*(-68.5572509204491 +
          y*(49.3667694856254 + y*(-17.1397577419788 +
              2.49697009569508*y))) - 22.6683558512829*z) +
          x*(-1091.66841042967 - 196.028306689776*y +
          x*(374.60123787784 - 48.5891069025409*x +
              36.7571622995805*y) + 36.0284195611086*z) +
          z*(-54.7919133532887 + (-4.08193978912261 -
              30.1755111971161*z)*z)) +
          z*(199.459603073901 + z*(-52.2940909281335 +
              (68.0444942726459 - 3.41251932441282*z)*z)) +
          y*(-493.407510141682 + z*(-175.292041186547 +
              (83.1923927801819 - 29.483064349429*z)*z) +
          y*(-43.0664675978042 + z*(383.058066002476 +
              z*(-54.1917262517112 + 25.6398487389914*z)) +
          y*(-10.0227370861875 - 460.319931801257*z +
              y*(0.875600661808945 + 234.565187611355*z))))) +
          y*(168.072408311545 + z*(729.116529735046 +
          z*(-343.956902961561 + z*(124.687671116248 +
              z*(-31.656964386073 + 7.04658803315449*z)))) +
          y*(880.031352997204 + y*(-225.267649263401 +
          y*(91.4260447751259 + y*(-21.6603240875311 +
              2.13016970847183*y) +
          z*(-297.728741987187 + (74.726141138756 -
              36.4872919001588*z)*z)) +
          z*(694.244814133268 + z*(-204.889641964903 +
              (113.561697840594 - 11.1282734326413*z)*z))) +
          z*(-860.764303783977 + z*(337.409530269367 +
          z*(-178.314556207638 + (44.2040358308 -
              7.92001547211682*z)*z))))));

      if (sa > 0.0)
        g08 = g08 + x2*(5812.81456626732 +
        851.226734946706*y)*log(x);

      return_value	= g03 + g08;
    }
    else if (ns == 1  && nt == 0  && np == 0) {
      g08	= 8645.36753595126 + z*(-6620.98308089678 +
          z*(769.588305957198 + z*(-193.0648640214916 +
              (31.6816345533648 - 5.24960313181984*z)*z))) +
          x*(-7296.43987145382 + x*(8103.20462414788 +
          y*(2175.341332000392 + y*(-274.2290036817964 +
          y*(197.4670779425016 + y*(-68.5590309679152 +
              9.98788038278032*y))) - 90.6734234051316*z) +
          x*(-5458.34205214835 - 980.14153344888*y +
          x*(2247.60742726704 - 340.1237483177863*x +
              220.542973797483*y) + 180.142097805543*z) +
          z*(-219.1676534131548 + (-16.32775915649044 -
              120.7020447884644*z)*z)) +
          z*(598.378809221703 + z*(-156.8822727844005 +
              (204.1334828179377 - 10.23755797323846*z)*z)) +
          y*(-1480.222530425046 + z*(-525.876123559641 +
              (249.57717834054571 - 88.449193048287*z)*z) +
          y*(-129.1994027934126 + z*(1149.174198007428 +
              z*(-162.5751787551336 + 76.9195462169742*z)) +
          y*(-30.0682112585625 - 1380.9597954037708*z +
              y*(2.626801985426835 + 703.695562834065*z))))) +
          y*(1187.3715515697959 + z*(1458.233059470092 +
          z*(-687.913805923122 + z*(249.375342232496 +
              z*(-63.313928772146 + 14.09317606630898*z)))) +
          y*(1760.062705994408 + y*(-450.535298526802 +
          y*(182.8520895502518 + y*(-43.3206481750622 +
              4.26033941694366*y) +
          z*(-595.457483974374 + (149.452282277512 -
              72.9745838003176*z)*z)) +
          z*(1388.489628266536 + z*(-409.779283929806 +
              (227.123395681188 - 22.2565468652826*z)*z))) +
          z*(-1721.528607567954 + z*(674.819060538734 +
          z*(-356.629112415276 + (88.4080716616 -
              15.84003094423364*z)*z)))));

      if (sa > 0.0)
        g08 = g08 + (11625.62913253464 + 1702.453469893412*y)*log(x);
      else
        g08 = 0.0;

      return_value	= 0.5*TEO_10_gsw_sfac*g08;
    }
    else if (ns == 0  && nt == 1  && np == 0) {
      g03 = 5.90578347909402 + z*(-270.983805184062 +
          z*(776.153611613101 + z*(-196.51255088122 +
              (28.9796526294175 - 2.13290083518327*z)*z))) +
          y*(-24715.571866078 + z*(2910.0729080936 +
          z*(-1513.116771538718 + z*(546.959324647056 +
              z*(-111.1208127634436 + 8.68841343834394*z)))) +
          y*(2210.2236124548363 + z*(-2017.52334943521 +
          z*(1498.081172457456 + z*(-718.6359919632359 +
              (146.4037555781616 - 4.9892131862671505*z)*z))) +
          y*(-592.743745734632 + z*(1591.873781627888 +
          z*(-1207.261522487504 + (608.785486935364 -
              105.4993508931208*z)*z)) +
          y*(290.12956292128547 + z*(-973.091553087975 +
          z*(602.603274510125 + z*(-276.361526170076 +
              32.40953340386105*z))) +
          y*(-113.90630790850321 + y*(21.35571525415769 -
              67.41756835751434*z) +
          z*(381.06836198507096 + z*(-133.7383902842754 +
              49.023632509086724*z)))))));

      g08 = x2*(168.072408311545 + z*(729.116529735046 +
          z*(-343.956902961561 + z*(124.687671116248 +
              z*(-31.656964386073 + 7.04658803315449*z)))) +
          x*(-493.407510141682 + x*(543.835333000098 +
              x*(-196.028306689776 + 36.7571622995805*x) +
          y*(-137.1145018408982 + y*(148.10030845687618 +
              y*(-68.5590309679152 + 12.4848504784754*y))) -
              22.6683558512829*z) + z*(-175.292041186547 +
              (83.1923927801819 - 29.483064349429*z)*z) +
          y*(-86.1329351956084 + z*(766.116132004952 +
              z*(-108.3834525034224 + 51.2796974779828*z)) +
          y*(-30.0682112585625 - 1380.9597954037708*z +
              y*(3.50240264723578 + 938.26075044542*z)))) +
          y*(1760.062705994408 + y*(-675.802947790203 +
          y*(365.7041791005036 + y*(-108.30162043765552 +
              12.78101825083098*y) +
          z*(-1190.914967948748 + (298.904564555024 -
              145.9491676006352*z)*z)) +
          z*(2082.7344423998043 + z*(-614.668925894709 +
              (340.685093521782 - 33.3848202979239*z)*z))) +
          z*(-1721.528607567954 + z*(674.819060538734 +
          z*(-356.629112415276 + (88.4080716616 -
              15.84003094423364*z)*z)))));

      if (sa > 0.0)
        g08 = g08 + 851.226734946706*x2*log(x);

      return_value	= (g03 + g08)*0.025;
    }
    else if (ns == 0  && nt == 0  && np == 1) {
      g03 = 100015.695367145 + z*(-5089.1530840726 +
          z*(853.5533353388611 + z*(-133.2587017014444 +
              (21.0131554401542 - 3.278571068826234*z)*z))) +
          y*(-270.983805184062 + z*(1552.307223226202 +
          z*(-589.53765264366 + (115.91861051767 -
              10.664504175916349*z)*z)) +
          y*(1455.0364540468 + z*(-1513.116771538718 +
          z*(820.438986970584 + z*(-222.2416255268872 +
              21.72103359585985*z))) +
          y*(-672.50778314507 + z*(998.720781638304 +
          z*(-718.6359919632359 + (195.2050074375488 -
              8.31535531044525*z)*z)) +
          y*(397.968445406972 + z*(-603.630761243752 +
              (456.589115201523 - 105.4993508931208*z)*z) +
          y*(-194.618310617595 + y*(63.5113936641785 -
              9.63108119393062*y +
          z*(-44.5794634280918 + 24.511816254543362*z)) +
          z*(241.04130980405 + z*(-165.8169157020456 +
          25.92762672308884*z)))))));

      g08 = x2*(-3310.49154044839 + z*(769.588305957198 +
          z*(-289.5972960322374 + (63.3632691067296 -
              13.1240078295496*z)*z)) +
          x*(199.459603073901 + x*(-54.7919133532887 +
              36.0284195611086*x - 22.6683558512829*y +
          (-8.16387957824522 - 90.52653359134831*z)*z) +
          z*(-104.588181856267 + (204.1334828179377 -
              13.65007729765128*z)*z) +
          y*(-175.292041186547 + (166.3847855603638 -
              88.449193048287*z)*z +
          y*(383.058066002476 + y*(-460.319931801257 +
              234.565187611355*y) +
          z*(-108.3834525034224 + 76.9195462169742*z)))) +
          y*(729.116529735046 + z*(-687.913805923122 +
          z*(374.063013348744 + z*(-126.627857544292 +
              35.23294016577245*z))) +
          y*(-860.764303783977 + y*(694.244814133268 +
          y*(-297.728741987187 + (149.452282277512 -
              109.46187570047641*z)*z) +
          z*(-409.779283929806 + (340.685093521782 -
              44.5130937305652*z)*z)) +
          z*(674.819060538734 + z*(-534.943668622914 +
              (176.8161433232 - 39.600077360584095*z)*z)))));

      return_value	= (g03 + g08)*1.0e-8;
    }
    else if (ns == 0  && nt == 2  && np == 0) {
      g03 = -24715.571866078 + z*(2910.0729080936 + z*
          (-1513.116771538718 + z*(546.959324647056 +
            z*(-111.1208127634436 + 8.68841343834394*z)))) +
          y*(4420.4472249096725 + z*(-4035.04669887042 +
          z*(2996.162344914912 + z*(-1437.2719839264719 +
              (292.8075111563232 - 9.978426372534301*z)*z))) +
          y*(-1778.231237203896 + z*(4775.621344883664 +
          z*(-3621.784567462512 + (1826.356460806092 -
              316.49805267936244*z)*z)) +
          y*(1160.5182516851419 + z*(-3892.3662123519 +
          z*(2410.4130980405 + z*(-1105.446104680304 +
              129.6381336154442*z))) +
          y*(-569.531539542516 + y*(128.13429152494615 -
              404.50541014508605*z) +
          z*(1905.341809925355 + z*(-668.691951421377 +
              245.11816254543362*z))))));

      g08 = x2*(1760.062705994408 + x*(-86.1329351956084 +
          x*(-137.1145018408982 + y*(296.20061691375236 +
              y*(-205.67709290374563 + 49.9394019139016*y))) +
          z*(766.116132004952 + z*(-108.3834525034224 +
              51.2796974779828*z)) +
          y*(-60.136422517125 - 2761.9195908075417*z +
              y*(10.50720794170734 + 2814.78225133626*z))) +
          y*(-1351.605895580406 + y*(1097.1125373015109 +
              y*(-433.20648175062206 + 63.905091254154904*y) +
          z*(-3572.7449038462437 + (896.713693665072 -
              437.84750280190565*z)*z)) +
          z*(4165.4688847996085 + z*(-1229.337851789418 +
              (681.370187043564 - 66.7696405958478*z)*z))) +
          z*(-1721.528607567954 + z*(674.819060538734 +
          z*(-356.629112415276 + (88.4080716616 -
              15.84003094423364*z)*z))));

      return_value = (g03 + g08)*0.000625;
    }
    else if (ns == 1  && nt == 0  && np == 1) {
      g08 = -6620.98308089678 + z*(1539.176611914396 +
          z*(-579.1945920644748 + (126.7265382134592 -
              26.2480156590992*z)*z)) +
          x*(598.378809221703 + x*(-219.1676534131548 +
              180.142097805543*x - 90.6734234051316*y +
          (-32.65551831298088 - 362.10613436539325*z)*z) +
          z*(-313.764545568801 + (612.4004484538132 -
              40.95023189295384*z)*z) +
          y*(-525.876123559641 + (499.15435668109143 -
              265.347579144861*z)*z +
          y*(1149.174198007428 + y*(-1380.9597954037708 +
              703.695562834065*y) +
          z*(-325.1503575102672 + 230.7586386509226*z)))) +
          y*(1458.233059470092 + z*(-1375.827611846244 +
          z*(748.126026697488 + z*(-253.255715088584 +
              70.4658803315449*z))) +
          y*(-1721.528607567954 + y*(1388.489628266536 +
          y*(-595.457483974374 + (298.904564555024 -
              218.92375140095282*z)*z) +
          z*(-819.558567859612 + (681.370187043564 -
              89.0261874611304*z)*z)) +
          z*(1349.638121077468 + z*(-1069.887337245828 +
              (353.6322866464 - 79.20015472116819*z)*z))));

      return_value = g08*TEO_10_gsw_sfac*0.5e-8;
    }
    else if (ns == 0  && nt == 1  && np == 1) {
      g03 = -270.983805184062 + z*(1552.307223226202 +
          z*(-589.53765264366 + (115.91861051767 -
              10.664504175916349*z)*z)) +
          y*(2910.0729080936 + z*(-3026.233543077436 +
          z*(1640.877973941168 + z*(-444.4832510537744 +
              43.4420671917197*z))) +
          y*(-2017.52334943521 + z*(2996.162344914912 +
          z*(-2155.907975889708 + (585.6150223126464 -
              24.946065931335752*z)*z)) +
          y*(1591.873781627888 + z*(-2414.523044975008 +
              (1826.356460806092 - 421.9974035724832*z)*z) +
          y*(-973.091553087975 + z*(1205.20654902025 +
              z*(-829.084578510228 + 129.6381336154442*z)) +
          y*(381.06836198507096 - 67.41756835751434*y +
              z*(-267.4767805685508 + 147.07089752726017*z))))));

      g08 = x2*(729.116529735046 + z*(-687.913805923122 +
          z*(374.063013348744 + z*(-126.627857544292 +
              35.23294016577245*z))) +
          x*(-175.292041186547 - 22.6683558512829*x +
              (166.3847855603638 - 88.449193048287*z)*z +
          y*(766.116132004952 + y*(-1380.9597954037708 +
              938.26075044542*y) +
          z*(-216.7669050068448 + 153.8390924339484*z))) +
          y*(-1721.528607567954 + y*(2082.7344423998043 +
          y*(-1190.914967948748 + (597.809129110048 -
              437.84750280190565*z)*z) +
          z*(-1229.337851789418 + (1022.055280565346 -
              133.5392811916956*z)*z)) +
          z*(1349.638121077468 + z*(-1069.887337245828 +
              (353.6322866464 - 79.20015472116819*z)*z))));

      return_value = (g03 + g08)*2.5e-10;
    }
    else if (ns == 1  && nt == 1  && np == 0) {
      g08 = 1187.3715515697959 + z*(1458.233059470092 +
          z*(-687.913805923122 + z*(249.375342232496 +
          z*(-63.313928772146 + 14.09317606630898*z)))) +
          x*(-1480.222530425046 + x*(2175.341332000392 +
          x*(-980.14153344888 + 220.542973797483*x) +
          y*(-548.4580073635929 + y*(592.4012338275047 +
          y*(-274.2361238716608 + 49.9394019139016*y))) -
          90.6734234051316*z) +
          z*(-525.876123559641 + (249.57717834054571 -
          88.449193048287*z)*z) +
                y*(-258.3988055868252 + z*(2298.348396014856 +
          z*(-325.1503575102672 + 153.8390924339484*z)) +
          y*(-90.2046337756875 - 4142.8793862113125*z +
          y*(10.50720794170734 + 2814.78225133626*z)))) +
          y*(3520.125411988816 + y*(-1351.605895580406 +
          y*(731.4083582010072 + y*(-216.60324087531103 +
          25.56203650166196*y) +
          z*(-2381.829935897496 + (597.809129110048 -
          291.8983352012704*z)*z)) +
          z*(4165.4688847996085 + z*(-1229.337851789418 +
          (681.370187043564 - 66.7696405958478*z)*z))) +
          z*(-3443.057215135908 + z*(1349.638121077468 +
          z*(-713.258224830552 + (176.8161433232 -
          31.68006188846728*z)*z))));

      if (sa > 0.0)
        g08 = g08 + 1702.453469893412*log(x);

      return_value = 0.5*TEO_10_gsw_sfac*0.025*g08;
    }
    else if (ns == 2  && nt == 0  && np == 0) {
      g08 = 2.0*(8103.20462414788 +
          y*(2175.341332000392 + y*(-274.2290036817964 +
          y*(197.4670779425016 + y*(-68.5590309679152 +
          9.98788038278032*y))) - 90.6734234051316*z) +
          1.5*x*(-5458.34205214835 - 980.14153344888*y +
          (4.0/3.0)*x*(2247.60742726704 - 340.1237483177863*1.25*x +
          220.542973797483*y) + 180.142097805543*z) +
          z*(-219.1676534131548 + (-16.32775915649044 -
          120.7020447884644*z)*z));

      if (x > 0.0) {
        g08 += (-7296.43987145382 + z*(598.378809221703 +
            z*(-156.8822727844005 + (204.1334828179377 -
            10.23755797323846*z)*z)) +
            y*(-1480.222530425046 + z*(-525.876123559641 +
            (249.57717834054571 - 88.449193048287*z)*z) +
            y*(-129.1994027934126 + z*(1149.174198007428 +
            z*(-162.5751787551336 + 76.9195462169742*z)) +
            y*(-30.0682112585625 - 1380.9597954037708*z +
            y*(2.626801985426835 + 703.695562834065*z)))))/x +
            (11625.62913253464 + 1702.453469893412*y)/x2;
      } else
        g08 = 0.0;

      return_value = 0.25*TEO_10_gsw_sfac*TEO_10_gsw_sfac*g08;
    }
    else if (ns == 0  && nt == 0  && np == 2) {
      g03 = -5089.1530840726 + z*(1707.1066706777221 +
          z*(-399.7761051043332 + (84.0526217606168 -
              16.39285534413117*z)*z)) +
          y*(1552.307223226202 + z*(-1179.07530528732 +
              (347.75583155301 - 42.658016703665396*z)*z) +
          y*(-1513.116771538718 + z*(1640.877973941168 +
              z*(-666.7248765806615 + 86.8841343834394*z)) +
          y*(998.720781638304 + z*(-1437.2719839264719 +
              (585.6150223126464 - 33.261421241781*z)*z) +
          y*(-603.630761243752 + (913.178230403046 -
              316.49805267936244*z)*z +
          y*(241.04130980405 + y*(-44.5794634280918 +
              49.023632509086724*z) +
          z*(-331.6338314040912 + 77.78288016926652*z))))));

      g08 = x2*(769.588305957198 + z*(-579.1945920644748 +
          (190.08980732018878 - 52.4960313181984*z)*z) +
          x*(-104.588181856267 + x*(-8.16387957824522 -
              181.05306718269662*z) +
          (408.2669656358754 - 40.95023189295384*z)*z +
          y*(166.3847855603638 - 176.898386096574*z +
              y*(-108.3834525034224 + 153.8390924339484*z))) +
          y*(-687.913805923122 + z*(748.126026697488 +
              z*(-379.883572632876 + 140.9317606630898*z)) +
          y*(674.819060538734 + z*(-1069.887337245828 +
              (530.4484299696 - 158.40030944233638*z)*z) +
          y*(-409.779283929806 + y*(149.452282277512 -
              218.92375140095282*z) +
          (681.370187043564 - 133.5392811916956*z)*z))));

      return_value = (g03 + g08)*1e-16 ;
    }
    else
      return_value = -HUGE_VAL;

    return (return_value);
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


