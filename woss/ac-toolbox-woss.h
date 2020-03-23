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
 * @file   ac-toolbox-woss.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::ACToolboxWoss class
 *
 * Provides the interface for the woss::ACToolboxWoss class
 */


#ifndef AC_TOOLBOX_WOSS_DEFINITIONS_H
#define AC_TOOLBOX_WOSS_DEFINITIONS_H


#include <ssp-definitions.h>
#include "woss.h"


namespace woss {
  
  
  class Sediment;
  class Altimetry;
  
  /**
  * A vector of SSP
  **/  
  typedef ::std::vector< SSP* > SSPVector;


  /**
  * \brief base class for implementing acoustic-toolbox channel simulators (Bellhop, Kraken, etc...) 
  *
  * Class ACToolboxWoss should be the base class of any Woss that wants to implement any acoustic-toolbox channel simulator
  * (Bellhop, Kraken, Fields, Sparc, etc...)
  */
  class ACToolboxWoss : public WossResReader {


    public:


    /**
    * ACToolboxWoss default constructor. Default constructed objects are not valid
    **/
    ACToolboxWoss();

    /**
    * ACToolboxWoss constructor
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_t const reference to a valid Time object for SSP's averaging purposes ( start date time )
    * @param end_t const reference to a valid Time object for SSP's averaging purposes ( end date time )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param freq_step frequency step [Hz]
    **/
    ACToolboxWoss( const CoordZ& tx, const CoordZ& rx, const Time& start_t, const Time& end_t, double start_freq, double end_freq, double freq_step ) ;

    /**
    * Destructor. It properly delete all pointers involved
    **/
    virtual ~ACToolboxWoss();


    /**
    * Initializes the enviroment for acoustic-toolbox channel simulator
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool initialize();


    /**
    * Checks the validity of ACToolboxWoss
    * @return <i>true</i> if it's valid, <i>false</i> otherwise
    **/
    virtual bool isValid() const ;

  
    /**
    * Sets the total number of range steps
    * @param steps total number of range steps
    **/
    ACToolboxWoss& setRangeSteps( int steps ) { total_range_steps = steps; coordz_vector.reserve(steps + 2); 
                                      range_vector.reserve(steps + 2); ssp_vector.reserve(steps + 2); return *this; }

    /**
    * Sets the depth precision for all SSP that will be created
    * @param precision depth precision [m]
    **/
    ACToolboxWoss& setSSPDepthPrecision( long double precision ) { ssp_depth_precision = precision; return *this; }


    /**
    * Gets the total number of range steps
    * @returns total number of range steps
    **/
    int getRangeSteps() const { return total_range_steps; }

    /**
    * Gets the depth precision for all SSP that will be created
    * @returns depth precision [m]
    **/
    long double getSSPDepthPrecision() const { return ssp_depth_precision; }

    /**
    * Gets the minimum number depth steps of all SSP currently in use
    * @returns mimimum SSP depth steps
    **/
    int getMinSSPDepthSteps() const { return min_ssp_depth_steps; }

    /**
    * Gets the maximum number depth steps of all SSP currently in use
    * @returns maximum SSP depth steps
    **/
    int getMaxSSPDepthSteps() const { return max_ssp_depth_steps; }

    /**
    * Gets the minimum depth of all SSP currently in use
    * @returns mimimum SSP depth [m]
    **/
    double getMinSSPDepth() const { return( *(min_ssp_depth_set.begin()) ); }

    /**
    * Gets the maximum depth of all SSP currently in use
    * @returns maximum SSP depth [m]
    **/
    double getMaxSSPDepth() const { return( *(max_ssp_depth_set.rbegin()) ); }

    /**
    * Gets the minimum depth of bathymetry in use
    * @returns mimimum bathymetry depth [m]
    **/
    double getMinBathymteryDepth() const { return min_bathymetry_depth; }

    /**
    * Gets the maximum depth of bathymetry in use
    * @returns maximum bathymetry depth [m]
    **/
    double getMaxBathymetryDepth() const { return max_bathymetry_depth; }


    protected:


    /**
    * Depth precision of all SSP that will be created [m]
    **/
    long double ssp_depth_precision;


    /**
    * Minimum bathymetry depth [m]
    **/
    double min_bathymetry_depth;

    /**
    * Maximum bathymetry depth [m]
    **/
    double max_bathymetry_depth;

    
    /**
    * Minimum altimetry depth [m]
    **/    
    double min_altimetry_depth;
    
    /**
    * Maximum altimetry depth [m]
    **/    
    double max_altimetry_depth;
    
    
    /**
    * Set of all minimum SSP depth [m] currently in use
    **/
    ::std::set< double > min_ssp_depth_set;

    /**
    * Set of all maximum SSP depth [m] currently in use
    **/
    ::std::set< double > max_ssp_depth_set;


    /**
    * Miminum number of currently in use SSP depth steps 
    **/
    int min_ssp_depth_steps;

    /**
    * Maximum number of currently in use SSP depth steps 
    **/
    int max_ssp_depth_steps;

    /**
    * Number of range steps
    **/
    int total_range_steps;


    /**
    * Vector of all CoordZ involved. Its size is equal to total_range_steps
    **/  
    CoordZVector coordz_vector;

    /**
    * Vector of all ranges [m] involved. Its size is equal to total_range_steps
    **/
    RangeVector range_vector;

    /**
    * Vector of all SSP involved. Its size is less or equal to total_range_steps
    **/
    SSPVector ssp_vector;

    /**
    * Set of of ssp_vector indexes that link to unique SSP
    **/
    ::std::set< int > ssp_unique_indexes;

    /**
    * Sediment in use
    **/
    Sediment* sediment_value;

    /**
    * Altimetry in use
    **/
    Altimetry* altimetry_value;
    

    /**
    * <i>True</i> if ssp_vector is transformable
    * @see SSP::isTransformable()
    **/
    bool is_ssp_vector_transformable;


    /**
    * Checks if the given SSP is not equal to previous values
    * @param value pointer to a valid SSP object
    **/
    virtual void checkSSPUnicity( SSP*& ptr );


    /**
    * Initializes range_vector
    * @returns <i>true</i> if method succeeded, <i>false</i> otherwise
    **/
    virtual bool initRangeVector();

    /**
    * Initializes coordz_vector
    * @returns <i>true</i> if method succeeded, <i>false</i> otherwise
    **/
    virtual bool initCoordZVector();
    
    /**
    * Initializes sediment_value
    * @returns <i>true</i> if method succeeded, <i>false</i> otherwise
    **/
    virtual bool initSediment();

    /**
    * Initializes altimetry_value
    * @returns <i>true</i> if method succeeded, <i>false</i> otherwise
    **/    
    virtual bool initAltimetry();
        
    /**
    * Initializes ssp_vector
    * @returns <i>true</i> if method succeeded, <i>false</i> otherwise
    **/
    virtual bool initSSPVector();

    /**
    * Resets ssp_vector
    **/    
    virtual void resetSSPVector(); 
    
  };

  
}


#endif /* AC_TOOLBOX_WOSS_DEFINITIONS_H */

 
