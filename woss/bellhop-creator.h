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
 * @file   bellhop-creator.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::BellhopCreator class
 *
 * Provides the interface for the woss::BellhopCreator class
 */


#ifndef WOSS_BELLHOP_CREATOR_DEFINITIONS_H
#define WOSS_BELLHOP_CREATOR_DEFINITIONS_H


#include "woss-creator.h"
#include "bellhop-woss.h"


namespace woss {
  
    
  /**
  * \brief Bellhop min max angles
  *
  * Struct that stores Bellhop minimum and maximum ray launching angle
  **/
  struct CustomAngles {
  
    
    /**
    * Default constructor
    * @param min minimum angle [signed degrees]
    * @param max maximum angle [signed degrees]
    **/
    CustomAngles( double min = double(), double max = double() ) : min_angle(min), max_angle(max) { }
   
   
    friend std::ostream& operator<<( std::ostream& os, const CustomAngles& instance ) {
      os << "min angle = " << instance.min_angle << "; max_angle = " << instance.max_angle;
      return os;
    }   
    
    /**
    * minimum angle [signed degrees]
    **/
    double min_angle;

    /**
    * maximum angle [signed degrees]
    **/    
    double max_angle;
   
    
  };

    
  /**
  * \brief class that provides correctly initialized BellhopWoss objects
  *
  * BellhopCreator implements WossCreator and provides interface for creation and initializiation of BellhopWoss object
  */
  class BellhopCreator : public WossCreator {

    
    public:

    /**
    * BellhopCreator default constructor
    */
    BellhopCreator();
    
    virtual ~BellhopCreator() { }
    

    /**
    * Returns a pointer to valid BellhopWoss for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @returns pointer to properly initialized BellhopWoss object
    **/
    virtual BellhopWoss* const createWoss( const CoordZ& tx, const CoordZ& rx, double start_frequency, double end_frequency ) const;
    
    
    /**
    * Sets the Thorpe attenuation flag for all bellhop instances
    * @param flag boolean flag
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setThorpeAttFlag( bool flag ) { use_thorpe_att = flag; return *this; }
    
    /**
    * Gets the Thorpe attenuation flag for all bellhop instances
    * @return boolean flag
    */
    bool getThorpeAttFlag() { return use_thorpe_att; }
    
    
  /**
    * Sets the total range steps of bellhop simulation for given transmitter, receiver woss::CoordZ
    * @param steps total range steps
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setTotalRangeSteps( int steps, const CoordZ& tx, const CoordZ& rx ) { 
      cctotal_range_steps.replace(steps, tx, rx); return *this; }

    /**
    * Sets the total range steps of bellhop simulation for given transmitter, receiver woss::Location
    * @param steps total range steps
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setTotalRangeSteps( int steps, Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      cctotal_range_steps.replace(steps, tx, rx); return *this; }

    /**
    * Erases the total range steps of bellhop simulation for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseTotalRangeSteps( const CoordZ& tx, const CoordZ& rx ) { 
      cctotal_range_steps.erase(tx, rx); return *this; }

    /**
    * Erases the total range steps of bellhop simulation for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseTotalRangeSteps( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      cctotal_range_steps.erase(tx, rx); return *this; }

    /**
    * Returns the total range steps of bellhop simulation for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance
    * @return int value
    */
    double getTotalRangeSteps( const CoordZ& tx, const CoordZ& rx ) { 
      return cctotal_range_steps.get(tx, rx); }

    /**
    * Returns the total range steps of bellhop simulation for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance
    * @return int value
    */
    double getTotalRangeSteps( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      return cctotal_range_steps.get(tx, rx); }
      
    
    /**
    * Sets mimimum depth offset [m] for given transmitter, receiver woss::CoordZ
    * @param offset 0 <= depth offset <= 0 [m]
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setTxMinDepthOffset( double offset, const CoordZ& tx, const CoordZ& rx ) { 
      cctx_min_depth_offset.replace(offset, tx, rx); return *this; }

    /**
    * Sets mimimum depth offset [m] for given transmitter, receiver woss::Location
    * @param offset 0 <= depth offset <= 0 [m]
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setTxMinDepthOffset( double offset, Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      cctx_min_depth_offset.replace(offset, tx, rx); return *this; }

    /**
    * Erases mimimum depth offset [m] for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseTxMinDepthOffset( const CoordZ& tx, const CoordZ& rx ) { 
      cctx_min_depth_offset.erase(tx, rx); return *this; }

    /**
    * Erases mimimum depth offset [m] for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseTxMinDepthOffset( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      cctx_min_depth_offset.erase(tx, rx); return *this; }

    /**
    * Returns mimimum depth offset [m] for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance
    * @return value in meters
    */
    double getTxMinDepthOffset( const CoordZ& tx, const CoordZ& rx ) { 
      return cctx_min_depth_offset.get(tx, rx); }

    /**
    * Returns mimimum depth offset [m] for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance
    * @return value in meters
    */
    double getTxMinDepthOffset( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      return cctx_min_depth_offset.get(tx, rx); }
      

    /**
    * Sets the transmitter maximum depth offset [m] for given transmitter, receiver woss::CoordZ
    * @param offset 0 <= depth offset <= 0 [m]
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setTxMaxDepthOffset( double offset, const CoordZ& tx, const CoordZ& rx ) { 
      cctx_max_depth_offset.replace(offset, tx, rx); return *this; }

    /**
    * Sets the transmitter maximum depth offset [m] for given transmitter, receiver woss::Location
    * @param offset 0 <= depth offset <= 0 [m]
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setTxMaxDepthOffset( double offset, Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      cctx_max_depth_offset.replace(offset, tx, rx); return *this; }

    /**
    * Erases the transmitter maximum depth offset [m] for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseTxMaxDepthOffset( const CoordZ& tx, const CoordZ& rx ) { 
      cctx_max_depth_offset.erase(tx, rx); return *this; }

    /**
    * Erases the transmitter maximum depth offset [m] for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseTxMaxDepthOffset( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      cctx_max_depth_offset.erase(tx, rx); return *this; }

    /**
    * Gets the transmitter maximum depth offset [m] for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance
    * @return value in meters
    */
    double getTxMaxDepthOffset( const CoordZ& tx, const CoordZ& rx ) { 
      return cctx_max_depth_offset.get(tx, rx); }

    /**
    * Gets the transmitter maximum depth offset [m] for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance
    * @return value in meters
    */
    double getTxMaxDepthOffset( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      return cctx_max_depth_offset.get(tx, rx); }


    /**
    * Sets the number of transmitters for given transmitter, receiver woss::CoordZ
    * @param sources number of transmitters
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setTotalTransmitters( int sources, const CoordZ& tx, const CoordZ& rx ) { 
      cctotal_transmitters.replace(sources, tx, rx); return *this; }

    /**
    * Sets the number of transmitters for given transmitter, receiver woss::Location
    * @param sources number of transmitters
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setTotalTransmitters( int sources, Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      cctotal_transmitters.replace(sources, tx, rx); return *this; }

    /**
    * Erases the number of transmitters for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseTotalTransmitters( const CoordZ& tx, const CoordZ& rx ) { 
      cctotal_transmitters.erase(tx, rx); return *this; }

    /**
    * Erases the number of transmitters for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseTotalTransmitters( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      cctotal_transmitters.erase(tx, rx); return *this; }

    /**
    * Returns the number of transmitters for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance
    * @return total number of transmitter sources
    */
    int getTotalTransmitters( const CoordZ& tx, const CoordZ& rx ) { 
      return cctotal_transmitters.get(tx, rx); }

    /**
    * Returns the number of transmitters for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance
    * @return total number of transmitter sources
    */
    int getTotalTransmitters( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      return cctotal_transmitters.get(tx, rx); }


    /**
    * Sets the receiver mimimum depth offset [m] for given transmitter, receiver woss::CoordZ
    * @param offset 0 <= depth offset <= 0 [m]
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setRxMinDepthOffset( double offset, const CoordZ& tx, const CoordZ& rx ) { 
      ccrx_min_depth_offset.replace(offset, tx, rx); return *this; }

    /**
    * Sets the receiver mimimum depth offset [m] for given transmitter, receiver woss::Location
    * @param offset 0 <= depth offset <= 0 [m]
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setRxMinDepthOffset( double offset, Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      ccrx_min_depth_offset.replace(offset, tx, rx); return *this; }

    /**
    * Erases the receiver mimimum depth offset [m] for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseRxMinDepthOffset( const CoordZ& tx, const CoordZ& rx ) { 
      ccrx_min_depth_offset.erase(tx, rx); return *this; }

    /**
    * Erases the receiver mimimum depth offset [m] for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseRxMinDepthOffset( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      ccrx_min_depth_offset.erase(tx, rx); return *this; }

    /**
    * Returns the receiver mimimum depth offset [m] for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance    
    * @return value in meters
    */
    double getRxMinDepthOffset( const CoordZ& tx, const CoordZ& rx ) { 
      return ccrx_min_depth_offset.get(tx, rx); }

    /**
    * Sets the receiver mimimum depth offset [m] for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance    
    * @return value in meters
    */
    double getRxMinDepthOffset( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      return ccrx_min_depth_offset.get(tx, rx); }


    /**
    * Sets the receiver maximum depth offset [m] for given transmitter, receiver woss::CoordZ
    * @param offset 0 <= depth offset <= 0 [m]
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setRxMaxDepthOffset( double offset, const CoordZ& tx, const CoordZ& rx ) { 
      ccrx_max_depth_offset.replace(offset, tx, rx); return *this; }

    /**
    * Sets the receiver maximum depth offset [m] for given transmitter, receiver woss::Location
    * @param offset 0 <= depth offset <= 0 [m]
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setRxMaxDepthOffset( double offset, Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      ccrx_max_depth_offset.replace(offset, tx, rx); return *this; }

    /**
    * Erases the receiver maximum depth offset [m] for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseRxMaxDepthOffset( const CoordZ& tx, const CoordZ& rx ) { 
      ccrx_max_depth_offset.erase(tx, rx); return *this; }

    /**
    * Erases the receiver maximum depth offset [m] for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseRxMaxDepthOffset( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      ccrx_max_depth_offset.erase(tx, rx); return *this; }

    /**
    * Returns the receiver maximum depth offset [m] for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance    
    * @return value in meters
    */
    double getRxMaxDepthOffset( const CoordZ& tx, const CoordZ& rx ) { 
      return ccrx_max_depth_offset.get(tx, rx); }

    /**
    * Returns the receiver maximum depth offset [m] for given transmitter, receiver woss::Location
    * @param offset 0 <= depth offset <= 0 [m]
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance    
    * @return value in meters
    */
    double getRxMaxDepthOffset( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      return ccrx_max_depth_offset.get(tx, rx); }


    /**
    * Sets the receiver mimimum range offset [m] for given transmitter, receiver woss::CoordZ
    * @param offset -total_distance <= range offset <= total_distance [m]
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance    
    * @return reference to <b>*this</b>
    */   
    BellhopCreator& setRxMinRangeOffset( double offset, const CoordZ& tx, const CoordZ& rx ) { 
      ccrx_min_range_offset.replace(offset, tx, rx); return *this; }

    /**
    * Sets the receiver mimimum range offset [m] for given transmitter, receiver woss::Location
    * @param offset -total_distance <= range offset <= total_distance [m]
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance    
    * @return reference to <b>*this</b>
    */   
    BellhopCreator& setRxMinRangeOffset( double offset, Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      ccrx_min_range_offset.replace(offset, tx, rx); return *this; }

    /**
    * Erases the receiver mimimum range offset [m] for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance    
    * @return reference to <b>*this</b>
    */   
    BellhopCreator& eraseRxMinRangeOffset( const CoordZ& tx, const CoordZ& rx ) { 
      ccrx_min_range_offset.erase(tx, rx); return *this; }

    /**
    * Erases the receiver mimimum range offset [m] for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance    
    * @return reference to <b>*this</b>
    */   
    BellhopCreator& eraseRxMinRangeOffset( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      ccrx_min_range_offset.erase(tx, rx); return *this; }

    /**
    * Returns the receiver mimimum range offset [m] for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance    
    * @return value in meters
    */   
    double getRxMinRangeOffset( const CoordZ& tx, const CoordZ& rx ) { 
      return ccrx_min_range_offset.get(tx, rx); }

    /**
    * Returns the receiver mimimum range offset [m] for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance    
    * @return value in meters
    */   
    double getRxMinRangeOffset( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      return ccrx_min_range_offset.get(tx, rx); }       
    
    
    /**
    * Sets the receiver maximum depth offset [m] for given transmitter, receiver woss::CoordZ
    * @param offset -total_distance <= range offset <= total_distance [m]
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance    
    * @return value in meters
    */
    BellhopCreator& setRxMaxRangeOffset( double offset, const CoordZ& tx, const CoordZ& rx ) { 
      ccrx_max_range_offset.replace(offset, tx, rx); return *this; }

    /**
    * Sets the receiver maximum depth offset [m] for given transmitter, receiver woss::Location
    * @param offset -total_distance <= range offset <= total_distance [m]
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setRxMaxRangeOffset( double offset, Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      ccrx_max_range_offset.replace(offset, tx, rx); return *this; }

    /**
    * Erases the receiver maximum depth offset [m] for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param tx const reference to a valid woss::CoordZ instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseRxMaxRangeOffset( const CoordZ& tx, const CoordZ& rx ) { 
      ccrx_max_range_offset.erase(tx, rx); return *this; }

    /**
    * Erases the receiver maximum depth offset [m] for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param tx const pointer to a valid woss::Location instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseRxMaxRangeOffset( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      ccrx_max_range_offset.erase(tx, rx); return *this; }

    /**
    * Returns the receiver maximum depth offset [m] for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param tx const reference to a valid woss::CoordZ instance    
    * @return value in meters
    */   
    double getRxMaxRangeOffset( const CoordZ& tx, const CoordZ& rx ) { 
      return ccrx_max_range_offset.get(tx, rx); }

    /**
    * Returns the receiver maximum depth offset [m] for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param tx const pointer to a valid woss::Location instance    
    * @return value in meters
    */
    double getRxMaxRangeOffset( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      return ccrx_max_range_offset.get(tx, rx); }       
    
    
    /**
    * Sets the number of receiver depths for given transmitter, receiver woss::CoordZ
    * @param number number of receiver depths
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setRxTotalDepths( int number, const CoordZ& tx, const CoordZ& rx ) { 
      cctotal_rx_depths.replace(number, tx, rx); return *this; }

    /**
    * Sets the number of receiver depths for given transmitter, receiver woss::Location
    * @param number number of receiver depths
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setRxTotalDepths( int number, Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      cctotal_rx_depths.replace(number, tx, rx); return *this; }

    /**
    * Erases the number of receiver depths for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseRxTotalDepths( const CoordZ& tx, const CoordZ& rx ) { 
      cctotal_rx_depths.erase(tx, rx); return *this; }

    /**
    * Erases the number of receiver depths for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseRxTotalDepths( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      cctotal_rx_depths.erase(tx, rx); return *this; }

    /**
    * Gets the number of receiver depths for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return number of receiver depths
    */
    int getRxTotalDepths( const CoordZ& tx, const CoordZ& rx ) { 
      return cctotal_rx_depths.get(tx, rx); }

    /**
    * Sets the number of receiver depths for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return number of receiver depths
    */
    int getRxTotalDepths( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      return cctotal_rx_depths.get(tx, rx); }   
      
      
    /**
    * Sets the number of receiver ranges for given transmitter, receiver woss::CoordZ
    * @param number number of receiver ranges
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setRxTotalRanges( int number, const CoordZ& tx, const CoordZ& rx ) { 
      cctotal_rx_ranges.replace(number, tx, rx); return *this; }

    /**
    * Sets the number of receiver ranges for given transmitter, receiver woss::Location
    * @param number number of receiver ranges
    * @param tx const reference to a valid woss::Location instance
    * @param rx const reference to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setRxTotalRanges( int number, Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      cctotal_rx_ranges.replace(number, tx, rx); return *this; }

    /**
    * Erases the number of receiver ranges for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseRxTotalRanges( const CoordZ& tx, const CoordZ& rx ) { 
      cctotal_rx_ranges.erase(tx, rx); return *this; }

    /**
    * Erases the number of receiver ranges for given transmitter, receiver woss::Location
    * @param tx const reference to a valid woss::Location instance
    * @param rx const reference to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseRxTotalRanges( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      cctotal_rx_ranges.erase(tx, rx); return *this; }

    /**
    * Returns the number of receiver ranges for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return number of receiver ranges
    */
    int getRxTotalRanges( const CoordZ& tx, const CoordZ& rx ) { 
      return cctotal_rx_ranges.get(tx, rx); }

    /**
    * Returns the number of receiver ranges for given transmitter, receiver woss::Location
    * @param tx const reference to a valid woss::Location instance
    * @param rx const reference to a valid woss::Location instance 
    * @return number of receiver ranges
    */
    int getRxTotalRanges( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      return cctotal_rx_ranges.get(tx, rx); }  
      

    /**
    * Sets the number of launched rays for given transmitter, receiver woss::CoordZ
    * @param number number of launched rays (>= 0)
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setRaysNumber( int number, const CoordZ& tx, const CoordZ& rx ) { 
      cctotal_rays.replace(number, tx, rx); return *this; }

    /**
    * Sets the number of launched rays for given transmitter, receiver woss::Location
    * @param number number of launched rays (>= 0)
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setRaysNumber( int number, Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      cctotal_rays.replace(number, tx, rx); return *this; }

    /**
    * Erases the number of launched rays for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseRaysNumber( const CoordZ& tx, const CoordZ& rx ) { 
      cctotal_rays.erase(tx, rx); return *this; }

    /**
    * Erases the number of launched rays for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseRaysNumber( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      cctotal_rays.erase(tx, rx); return *this; }

    /**
    * Returns the number of launched rays for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return number of launched rays
    */
    int getRaysNumber( const CoordZ& tx, const CoordZ& rx ) { 
      return cctotal_rays.get(tx, rx); }

    /**
    * Returns the number of launched rays for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return number of launched rays
    */
    int getRaysNumber( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      return cctotal_rays.get(tx, rx); }  


    /**
    * Sets the path of bellhop program
    * @param path filesystem path
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setBellhopPath( const ::std::string& path ) { bellhop_path = path; return *this; }

    /**
    * Gets the path of bellhop program
    * @return path string
    */
    ::std::string getBellhopPath() { return bellhop_path; }

    /**
    * Sets the .arr file syntax to be used during file parsing
    * @param syntax .arr file syntax
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setBellhopArrSyntax( BellhopArrSyntax syntax ) { 
      bellhop_arr_syntax = syntax; return *this; }

    /**
    * Gets the .arr file syntax
    * @return bellhop_arr_syntax
    */
    BellhopArrSyntax getBellhopArrSyntax() { return bellhop_arr_syntax; }

    /**
    * Sets the .shd file syntax to be used during file parsing
    * @param syntax .arr file syntax
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setBellhopShdSyntax( BellhopShdSyntax syntax ) { 
      bellhop_shd_syntax = syntax; return *this; }

    /**
    * Gets the .shd file syntax
    * @return bellhop_shd_syntax
    */
    BellhopShdSyntax getBellhopShdSyntax() { return bellhop_shd_syntax; }

    /**
    * Sets the beam option string for given transmitter, receiver woss::CoordZ. 
    * See Bellhop documentation for more info
    * @param options beam options ( G, C, R , B)
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setBeamOptions( const ::std::string& options, const CoordZ& tx, const CoordZ& rx ) { 
      ccbeam_options.replace(options, tx, rx); return *this; }

    /**
    * Sets the beam option string for given transmitter, receiver woss::Location. 
    * See Bellhop documentation for more info
    * @param options beam options ( G, C, R , B)
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setBeamOptions( const ::std::string& options, Location* const tx = CCString::ALL_LOCATIONS, Location* const rx = CCString::ALL_LOCATIONS ) { 
      ccbeam_options.replace(options, tx, rx); return *this; }

    /**
    * Erases the beam option string for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseBeamOptions( const CoordZ& tx, const CoordZ& rx ) { 
      ccbeam_options.erase(tx, rx); return *this; }

    /**
    * Erases the beam option string for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseBeamOptions( Location* const tx = CCString::ALL_LOCATIONS, Location* const rx = CCString::ALL_LOCATIONS ) { 
      ccbeam_options.erase(tx, rx); return *this; }

    /**
    * Returns the beam option string for given transmitter, receiver woss::CoordZ
    * @param rx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return beam options
    */
    ::std::string getBeamOptions( const CoordZ& tx, const CoordZ& rx ) { 
      return ccbeam_options.get(tx, rx); }

    /**
    * Returns the beam option string for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return beam options
    */
    ::std::string getBeamOptions( Location* const tx = CCString::ALL_LOCATIONS, Location* const rx = CCString::ALL_LOCATIONS ) { 
      return ccbeam_options.get(tx, rx); }  
      

    /**
    * Sets the Bellhop run mode string for given transmitter, receiver woss::CoordZ. 
    * See Bellhop documentation for more info
    * @param mode Bellhop run mode ( A, a , C, I , S) 
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setBhMode( const ::std::string& options, const CoordZ& tx, const CoordZ& rx ) { 
      ccbellhop_mode.replace(options, tx, rx); return *this; }

    /**
    * Sets the Bellhop run mode string for given transmitter, receiver woss::Location. 
    * See Bellhop documentation for more info
    * @param mode Bellhop run mode ( A, a , C, I , S) 
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setBhMode( const ::std::string& options, Location* const tx = CCString::ALL_LOCATIONS, Location* const rx = CCString::ALL_LOCATIONS ) { 
      ccbellhop_mode.replace(options, tx, rx); return *this; }

    /**
    * Erases the Bellhop run mode string for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseBhMode( const CoordZ& tx, const CoordZ& rx ) { 
      ccbellhop_mode.erase(tx, rx); return *this; }

    /**
    * Erases the Bellhop run mode string for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseBhMode( Location* const tx = CCString::ALL_LOCATIONS, Location* const rx = CCString::ALL_LOCATIONS ) { 
      ccbellhop_mode.erase(tx, rx); return *this; }

    /**
    * Returns the Bellhop run mode string for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return Bellhop run mode
    */
    ::std::string getBhMode( const CoordZ& tx, const CoordZ& rx ) { 
      return ccbellhop_mode.get(tx, rx); }

    /**
    * Returns the Bellhop run mode string for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return Bellhop run mode
    */
    ::std::string getBhMode( Location* const tx = CCString::ALL_LOCATIONS, Location* const rx = CCString::ALL_LOCATIONS ) { 
      return ccbellhop_mode.get(tx, rx); } 


    /**
    * Sets the bathymetry type ( L, C ) for given transmitter, receiver woss::CoordZ. 
    * See Bellhop documentation for more info
    * @param type bathymetry type
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */   
    BellhopCreator& setBathymetryType( const ::std::string& options, const CoordZ& tx, const CoordZ& rx ) { 
      ccbathymetry_type.replace(options, tx, rx); return *this; }

    /**
    * Sets the bathymetry type ( L, C ) for given transmitter, receiver woss::Location. 
    * See Bellhop documentation for more info
    * @param type  bathymetry types
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setBathymetryType( const ::std::string& options, Location* const tx = CCString::ALL_LOCATIONS, Location* const rx = CCString::ALL_LOCATIONS ) { 
      ccbathymetry_type.replace(options, tx, rx); return *this; }

    /**
    * Erases the bathymetry type ( L, C ) for given transmitter, receiver woss::CoordZ 
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */   
    BellhopCreator& eraseBathymetryType( const CoordZ& tx, const CoordZ& rx ) { 
      ccbathymetry_type.erase(tx, rx); return *this; }

    /**
    * Erases the bathymetry type ( L, C ) for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseBathymetryType( Location* const tx = CCString::ALL_LOCATIONS, Location* const rx = CCString::ALL_LOCATIONS ) { 
      ccbathymetry_type.erase(tx, rx); return *this; }

    /**
    * Returns the bathymetry type ( L, C ) for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return bathymetry type
    */   
    ::std::string getBathymetryType( const CoordZ& tx, const CoordZ& rx ) { 
      return ccbathymetry_type.get(tx, rx); }

    /**
    * Sets the bathymetry type ( L, C ) for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return  bathymetry type
    */
    ::std::string getBathymetryType( Location* const tx = CCString::ALL_LOCATIONS, Location* const rx = CCString::ALL_LOCATIONS ) { 
      return ccbathymetry_type.get(tx, rx); }    

    /**
    * Sets the bathymetry write method ( S, D ) for given transmitter, receiver woss::CoordZ. 
    * See Bellhop documentation for more info
    * @param type bathymetry write method
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */   
    BellhopCreator& setBathymetryMethod( const ::std::string& options, const CoordZ& tx, const CoordZ& rx ) { 
      ccbathymetry_method.replace(options, tx, rx); return *this; }

    /**
    * Sets the bathymetry write method ( S, D ) for given transmitter, receiver woss::Location. 
    * See Bellhop documentation for more info
    * @param type  bathymetry types
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setBathymetryMethod( const ::std::string& options, Location* const tx = CCString::ALL_LOCATIONS, Location* const rx = CCString::ALL_LOCATIONS ) { 
      ccbathymetry_method.replace(options, tx, rx); return *this; }

    /**
    * Erases the bathymetry write method ( S, D ) for given transmitter, receiver woss::CoordZ 
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */   
    BellhopCreator& eraseBathymetryMethod( const CoordZ& tx, const CoordZ& rx ) { 
      ccbathymetry_method.erase(tx, rx); return *this; }

    /**
    * Erases the bathymetry write method ( S, D ) for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseBathymetryMethod( Location* const tx = CCString::ALL_LOCATIONS, Location* const rx = CCString::ALL_LOCATIONS ) { 
      ccbathymetry_method.erase(tx, rx); return *this; }

    /**
    * Returns the bathymetry write method ( S, D ) for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return bathymetry type
    */   
    ::std::string getBathymetryMethod( const CoordZ& tx, const CoordZ& rx ) { 
      return ccbathymetry_method.get(tx, rx); }

    /**
    * Sets the bathymetry write method ( S, D ) for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return  bathymetry type
    */
    ::std::string getBathymetryMethod( Location* const tx = CCString::ALL_LOCATIONS, Location* const rx = CCString::ALL_LOCATIONS ) { 
      return ccbathymetry_method.get(tx, rx); }   

    /**
    * Sets the altimetry type ( L, C ) for given transmitter, receiver woss::CoordZ. 
    * See Bellhop documentation for more info
    * @param type altimetry type
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */   
    BellhopCreator& setAltimetryType( const ::std::string& options, const CoordZ& tx, const CoordZ& rx ) { 
      ccaltimetry_type.replace(options, tx, rx); return *this; }
    
    /**
    * Sets the altimetry type ( L, C ) for given transmitter, receiver woss::Location. 
    * See Bellhop documentation for more info
    * @param type  altimetry types
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setAltimetryType( const ::std::string& options, Location* const tx = CCString::ALL_LOCATIONS, Location* const rx = CCString::ALL_LOCATIONS ) { 
      ccaltimetry_type.replace(options, tx, rx); return *this; }
      
    /**
     * Erases the altimetry type ( L, C ) for given transmitter, receiver woss::CoordZ 
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    */   
    BellhopCreator& eraseAltimetryType( const CoordZ& tx, const CoordZ& rx ) { 
      ccaltimetry_type.erase(tx, rx); return *this; }
      
    /**
     * Erases the altimetry type ( L, C ) for given transmitter, receiver woss::Location
      * @param tx const pointer to a valid woss::Location instance
      * @param rx const pointer to a valid woss::Location instance 
      * @return reference to <b>*this</b>
      */
    BellhopCreator& eraseAltimetryType( Location* const tx = CCString::ALL_LOCATIONS, Location* const rx = CCString::ALL_LOCATIONS ) { 
      ccaltimetry_type.erase(tx, rx); return *this; }
        
    /**
     * Returns the altimetry type ( L, C ) for given transmitter, receiver woss::CoordZ
      * @param tx const reference to a valid woss::CoordZ instance
      * @param rx const reference to a valid woss::CoordZ instance 
      * @return altimetry type
      */   
    ::std::string getAltimetryType( const CoordZ& tx, const CoordZ& rx ) { 
      return ccaltimetry_type.get(tx, rx); }
      
    /**
     * Sets the altimetry type ( L, C ) for given transmitter, receiver woss::Location
      * @param tx const pointer to a valid woss::Location instance
      * @param rx const pointer to a valid woss::Location instance 
      * @return  altimetry type
      */
    ::std::string getAltimetryType( Location* const tx = CCString::ALL_LOCATIONS, Location* const rx = CCString::ALL_LOCATIONS ) { 
      return ccaltimetry_type.get(tx, rx); }    


    /**
    * Sets the CustomAngles for given transmitter CoordZ, receiver woss::CoordZ
    * @param angles const reference to a valid CustomAngles
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    **/
    BellhopCreator& setAngles( const CustomAngles& angles, const CoordZ& tx, const CoordZ& rx ) { 
      ccangles_map.replace(angles, tx, rx); return *this; }

    /**
    * Sets the CustomAngles for given transmitter CoordZ, receiver woss::Location
    * @param angles const reference to a valid CustomAngles
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance  
    * @return reference to <b>*this</b>
    **/
    BellhopCreator& setAngles( const CustomAngles& angles, Location* const tx = CCAngles::ALL_LOCATIONS, Location* const rx = CCAngles::ALL_LOCATIONS ) { 
      ccangles_map.replace(angles, tx, rx); return *this; }    
   
    /**
    * Returns the CustomAngles for given transmitter CoordZ, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return a valid CustomAngles
    **/
    CustomAngles getAngles( const CoordZ& tx, const CoordZ& rx ) const { return ccangles_map.get(tx, rx); }

    /**
    * Returns the CustomAngles for given transmitter CoordZ, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance  
    * @return a valid CustomAngles
    **/
    CustomAngles getAngles( Location* const tx = CCAngles::ALL_LOCATIONS, Location* const rx = CCAngles::ALL_LOCATIONS ) const { 
      return ccangles_map.get(tx, rx); }
          
    /**
    * Erases the CustomAngles for given transmitter CoordZ, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    **/
    BellhopCreator& eraseAngles( const CoordZ& tx, const CoordZ& rx ) { ccangles_map.erase(tx, rx); return *this; }
    
    /**
    * Erases the CustomAngles for given transmitter CoordZ, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance  
    * @return reference to <b>*this</b>
    **/    
    BellhopCreator& eraseAngles( Location* const tx = CCAngles::ALL_LOCATIONS, Location* const rx = CCAngles::ALL_LOCATIONS ) {
      ccangles_map.erase(tx, rx); return *this;
    }  
    
	/**
	* Sets the maximum Bellhop ray depth [m] for given transmitter, receiver woss::CoordZ
	* @param box_depth [m]
	* @param tx const reference to a valid woss::CoordZ instance
	* @param rx const reference to a valid woss::CoordZ instance
	* @return reference to <b>*this</b>
	*/
	BellhopCreator& setBoxDepth( double box_depth, const CoordZ& tx, const CoordZ& rx ) {
	  ccbox_depth.replace(box_depth, tx, rx); return *this; }

    /**
      * Sets the maximum Bellhop ray depth [m] for given transmitter, receiver woss::Location
      * @param box_depth [m]
      * @param tx const pointer to a valid woss::Location instance
      * @param rx const pointer to a valid woss::Location instance
      * @return value in meters
      */
      BellhopCreator& setBoxDepth( double box_depth, Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) {
        ccbox_depth.replace(box_depth, tx, rx); return *this; }


      /**
      * Erases the maximum box depth [m] for given transmitter, receiver woss::CoordZ
      * @param tx const reference to a valid woss::CoordZ instance
      * @param tx const reference to a valid woss::CoordZ instance
      * @return reference to <b>*this</b>
      */
      BellhopCreator& eraseBoxDepth( const CoordZ& tx, const CoordZ& rx ) {
        ccbox_depth.erase(tx, rx); return *this; }

	  /**
	  * Erases the receiver box depth [m] for given transmitter, receiver woss::Location
	  * @param tx const pointer to a valid woss::Location instance
	  * @param rx const pointer to a valid woss::Location instance
	  * @return reference to <b>*this</b>
	  */
	  BellhopCreator& eraseBoxDepth( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) {
		ccbox_depth.erase(tx, rx); return *this; }

      /**
      * Returns the maximum box depth [m] for given transmitter, receiver woss::CoordZ
      * @param tx const reference to a valid woss::CoordZ instance
      * @param tx const reference to a valid woss::CoordZ instance
      * @return value in meters
      */
      double getBoxDepth( const CoordZ& tx, const CoordZ& rx ) {
        return ccbox_depth.get(tx, rx); }

      /**
      * Returns the maximum box depth [m] for given transmitter, receiver woss::Location
      * @param tx const pointer to a valid woss::Location instance
      * @param tx const pointer to a valid woss::Location instance
      * @return value in meters
      */
      double getBoxDepth( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) {
        return ccbox_depth.get(tx, rx); }

      /**
      	* Sets the maximum Bellhop ray range [m] for given transmitter, receiver woss::CoordZ
      	* @param box_depth [m]
      	* @param tx const reference to a valid woss::CoordZ instance
      	* @param rx const reference to a valid woss::CoordZ instance
      	* @return reference to <b>*this</b>
      	*/
      	BellhopCreator& setBoxRange( double box_range, const CoordZ& tx, const CoordZ& rx ) {
      	  ccbox_range.replace(box_range, tx, rx); return *this; }

	  /**
		* Sets the maximum Bellhop ray depth [m] for given transmitter, receiver woss::Location
		* @param box_depth [m]
		* @param tx const pointer to a valid woss::Location instance
		* @param rx const pointer to a valid woss::Location instance
		* @return value in meters
		*/
		BellhopCreator& setBoxRange( double box_range, Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) {
		  ccbox_range.replace(box_range, tx, rx); return *this; }


		/**
		* Erases the maximum box depth [m] for given transmitter, receiver woss::CoordZ
		* @param tx const reference to a valid woss::CoordZ instance
		* @param tx const reference to a valid woss::CoordZ instance
		* @return reference to <b>*this</b>
		*/
		BellhopCreator& eraseBoxRange( const CoordZ& tx, const CoordZ& rx ) {
		  ccbox_range.erase(tx, rx); return *this; }

	  /**
	  * Erases the receiver box depth [m] for given transmitter, receiver woss::Location
	  * @param tx const pointer to a valid woss::Location instance
	  * @param rx const pointer to a valid woss::Location instance
	  * @return reference to <b>*this</b>
	  */
	  BellhopCreator& eraseBoxRange( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) {
		ccbox_range.erase(tx, rx); return *this; }

		/**
		* Returns the maximum box depth [m] for given transmitter, receiver woss::CoordZ
		* @param tx const reference to a valid woss::CoordZ instance
		* @param tx const reference to a valid woss::CoordZ instance
		* @return value in meters
		*/
		double getBoxRange( const CoordZ& tx, const CoordZ& rx ) {
		  return ccbox_range.get(tx, rx); }

		/**
		* Returns the maximum box depth [m] for given transmitter, receiver woss::Location
		* @param tx const pointer to a valid woss::Location instance
		* @param tx const pointer to a valid woss::Location instance
		* @return value in meters
		*/
		double getBoxRange( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) {
		  return ccbox_range.get(tx, rx); }
    
  /**
    * Sets the SSP depth precision [m] for given transmitter, receiver woss::CoordZ
    * @param ssp_precision depth_precision [m]
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance    
    * @return value in meters
    */
    BellhopCreator& setSspDepthPrecision( double ssp_precision, const CoordZ& tx, const CoordZ& rx ) { 
      ccssp_depth_precision.replace(ssp_precision, tx, rx); return *this; }

    /**
    * Sets the SSP depth precision [m] for given transmitter, receiver woss::Location
    * @param ssp_precision depth_precision [m]
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setSspDepthPrecision( double ssp_precision, Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      ccssp_depth_precision.replace(ssp_precision, tx, rx); return *this; }

    /**
    * Erases the SSP depth precision [m] for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param tx const reference to a valid woss::CoordZ instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseSspDepthPrecision( const CoordZ& tx, const CoordZ& rx ) { 
      ccssp_depth_precision.erase(tx, rx); return *this; }

    /**
    * Erases the SSP depth precision [m] for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param tx const pointer to a valid woss::Location instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseSspDepthPrecision( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      ccssp_depth_precision.erase(tx, rx); return *this; }

    /**
    * Returns the SSP depth precision [m] for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param tx const reference to a valid woss::CoordZ instance    
    * @return value in meters
    */   
    double getSspDepthPrecision( const CoordZ& tx, const CoordZ& rx ) { 
      return ccssp_depth_precision.get(tx, rx); }

    /**
    * Returns the SSP depth precision [m] for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param tx const pointer to a valid woss::Location instance    
    * @return value in meters
    */
    double getSspDepthPrecision( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      return ccssp_depth_precision.get(tx, rx); }       
        
    
  /**
    * Sets the SSP depth steps for given transmitter, receiver woss::CoordZ
    * @param ssp_depth_steps depth steps
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance    
    * @return value in meters
    */
    BellhopCreator& setSspDepthSteps( int ssp_depth_steps, const CoordZ& tx, const CoordZ& rx ) { 
      ccnormalized_ssp_depth_steps.replace(ssp_depth_steps, tx, rx); return *this; }

    /**
    * Sets the SSP depth steps for given transmitter, receiver woss::Location
    * @param ssp_depth_steps depth_precision [m]
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& setSspDepthSteps( int ssp_depth_steps, Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      ccnormalized_ssp_depth_steps.replace(ssp_depth_steps, tx, rx); return *this; }

    /**
    * Erases depth steps for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param tx const reference to a valid woss::CoordZ instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseSspDepthSteps( const CoordZ& tx, const CoordZ& rx ) { 
      ccnormalized_ssp_depth_steps.erase(tx, rx); return *this; }

    /**
    * Erases depth steps for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param tx const pointer to a valid woss::Location instance    
    * @return reference to <b>*this</b>
    */
    BellhopCreator& eraseSspDepthSteps( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      ccnormalized_ssp_depth_steps.erase(tx, rx); return *this; }

    /**
    * Returns depth steps for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param tx const reference to a valid woss::CoordZ instance    
    * @return integer value
    */   
    int getSspDepthSteps( const CoordZ& tx, const CoordZ& rx ) { 
      return ccnormalized_ssp_depth_steps.get(tx, rx); }

    /**
    * Returns depth steps for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param tx const pointer to a valid woss::Location instance    
    * @return integer value
    */
    int getSspDepthSteps( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      return ccnormalized_ssp_depth_steps.get(tx, rx); }      
    
    
    /**
    * Sets the woss::Transducer for given transmitter woss::CoordZ, receiver woss::CoordZ
    * @param angles const reference to a valid woss::Transducer
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    **/
    BellhopCreator& setCustomTransducer( const CustomTransducer& type, const CoordZ& tx, const CoordZ& rx ) { 
      cctransducer.replace(type, tx, rx); return *this; }

    /**
    * Sets the woss::Transducer for given transmitter woss::CoordZ, receiver woss::Location
    * @param angles const reference to a valid woss::Transducer
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance  
    * @return reference to <b>*this</b>
    **/
    BellhopCreator& setCustomTransducer( const CustomTransducer& type, Location* const tx = CCTransducer::ALL_LOCATIONS, Location* const rx = CCTransducer::ALL_LOCATIONS ) { 
      cctransducer.replace(type, tx, rx); return *this; }    
   
    /**
    * Returns the woss::Transducer for given transmitter CoordZ, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return a valid woss::Transducer
    **/
    CustomTransducer getCustomTransducer( const CoordZ& tx, const CoordZ& rx ) const { 
      return cctransducer.get(tx, rx); }

    /**
    * Returns the woss::Transducer for given transmitter woss::CoordZ, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance  
    * @return a valid woss::Transducer
    **/
    CustomTransducer getCustomTransducer( Location* const tx = CCTransducer::ALL_LOCATIONS, Location* const rx = CCTransducer::ALL_LOCATIONS ) const { 
      return cctransducer.get(tx, rx); }
          
    /**
    * Erases the woss::Transducer for given transmitter woss::CoordZ, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    **/
    BellhopCreator& eraseCustomTransducer( const CoordZ& tx, const CoordZ& rx ) { ccangles_map.erase(tx, rx); return *this; }
    
    /**
    * Erases the woss::Transducer for given transmitter woss::CoordZ, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance  
    * @return reference to <b>*this</b>
    **/    
    BellhopCreator& eraseCustomTransducer( Location* const tx = CCTransducer::ALL_LOCATIONS, Location* const rx = CCTransducer::ALL_LOCATIONS ) {
      cctransducer.erase(tx, rx); return *this;
    }      
    

    /**
    * CustomAngles container. A map that links a valid CoordZ (transmitter) with its CustomAngles
    */
    typedef WossCreatorContainer< CustomAngles > CCAngles;
    
    typedef WossCreatorContainer< CustomTransducer > CCTransducer;

    typedef WossCreatorContainer< ::std::string > CCString;
        
		
    protected:
    
    /**
     * flag that control bellhop's thorpe calculation
     */
    bool use_thorpe_att;
    
      
    /**
    * Pathname of Bellhop program
    **/
    ::std::string bellhop_path;
    
    /**
     * Bellhop .arr file syntax to be used during parsing, factory value = invalid
     */
    BellhopArrSyntax bellhop_arr_syntax;

    /**
     * Bellhop .shd file syntax to be used during parsing, factory value = invalid
     */
    BellhopShdSyntax bellhop_shd_syntax;

    /**
    * Bellhop run mode ( A, a, C, I, S )
    **/
    CCString ccbellhop_mode;

    /**
    * Bellhop beam options ( G, C, R, B )
    **/
    CCString ccbeam_options; 

    /**
    * Bathymetry type ( L, C )
    **/
    CCString ccbathymetry_type;
    
    /**
     * Bathymetry write method ( S (slope), D (discrete )
     */
    CCString ccbathymetry_method;
       
    /**
     * Altimetry type ( L, C )
     **/
    CCString ccaltimetry_type;
    
    
    /**
    * CustomAngles containter for user-given transmitter CoordZ
    **/
    CCAngles ccangles_map;
    
    
    /**
    * Number ot range steps
    **/
    CCInt cctotal_range_steps;
  
    
    /**
    * Number of transmitter
    **/  
    CCInt cctotal_transmitters;
  
    /**
    * Transmitter minimum depth offset [m]
    **/  
    CCDouble cctx_min_depth_offset;

    /**
    * Transmitter maximum depth offset [m]
    **/ 
    CCDouble cctx_max_depth_offset;


    /**
    * Number of receiver depths
    **/  
    CCInt cctotal_rx_depths;

    /**
    * Receiver minimum depth offset [m]
    **/
    CCDouble ccrx_min_depth_offset;

    /**
    * Receiver maximum depth offset [m]
    **/
    CCDouble ccrx_max_depth_offset;


    /**
    * Number of receiver ranges.
    *
    * On some configuration (linux distribution / cpu ) bellhop will output an empty file
    * with a value of <i>total_rx_ranges</i> = 1.
    **/  
    CCInt cctotal_rx_ranges;

    /**
    * Receiver minimum range offset [m]
    **/
    CCDouble ccrx_min_range_offset;

    /**
    * Receiver maximum range offset [m]
    **/
    CCDouble ccrx_max_range_offset;


    /**
    * Number of launched rays
    **/ 
    CCInt cctotal_rays;
    

    /**
    * SSP depth precision [m]
    **/ 
    CCDouble ccssp_depth_precision;

    /**
    * Depth steps of all SSP involved
    **/ 
    CCInt ccnormalized_ssp_depth_steps;
    
    /**
    * Transmitter beam pattern
    **/    
    CCTransducer cctransducer;
    
    /**
    * Maximum Bellhop ray depth [m]
    **/
    CCDouble ccbox_depth;

    /**
    * Maximum Bellhop ray range [m]
    **/
    CCDouble ccbox_range;
    
    /**
    * Initializes given BellhopWoss object
    * @param woss_ptr const pointer to an unitialized BellhopWoss
    * @returns <i>true</i> if method succeeded, <i>false</i> otherwise
    **/
    virtual bool initializeWoss( Woss* const woss_ptr ) const ;

    /**
    * Initializes given BellhopWoss object
    * @param woss_ptr const pointer to an unitialized BellhopWoss
    * @returns <i>true</i> if method succeeded, <i>false</i> otherwise
    **/
    bool initializeBhWoss( BellhopWoss* const woss_ptr ) const ;
        

    virtual const BellhopWoss* createNotValidWoss() const;

    virtual void updateDebugFlag();
    
  };

  
}


#endif /* WOSS_BELLHOP_CREATOR_DEFINITIONS_H */

