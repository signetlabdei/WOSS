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
 * @file   bellhop-creator.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::BellhopCreator class
 *
 * Provides the implementation of the woss::BellhopCreator class
 */




#include <definitions-handler.h>
#include <transducer-handler.h>
#include "bellhop-creator.h"


using namespace woss;


BellhopCreator::BellhopCreator() 
: WossCreator(),
  use_thorpe_att(true),
  bellhop_path(),  
  bellhop_arr_syntax(BELLHOP_CREATOR_ARR_FILE_INVALID),
  bellhop_shd_syntax(BELLHOP_CREATOR_SHD_FILE_INVALID),
  ccbellhop_mode(),
  ccbeam_options(),
  ccbathymetry_type(),
  ccbathymetry_method(),
  ccaltimetry_type(),
  ccangles_map(),
  cctotal_range_steps(),
  cctotal_transmitters(),
  cctx_min_depth_offset(),
  cctx_max_depth_offset(),
  cctotal_rx_depths(),
  ccrx_min_depth_offset(),
  ccrx_max_depth_offset(),
  cctotal_rx_ranges(),
  ccrx_min_range_offset(),
  ccrx_max_range_offset(),
  cctotal_rays(),
  ccssp_depth_precision(),
  ccnormalized_ssp_depth_steps(),
  cctransducer(),
  ccbox_depth(),
  ccbox_range()
{ 
  BellhopCreator::updateDebugFlag();
  cctransducer.accessAllLocations() = CustomTransducer();
}


void BellhopCreator::updateDebugFlag() {
  ccangles_map.setDebug(debug);
  ccbellhop_mode.setDebug(debug);
  ccbeam_options.setDebug(debug);
  ccbathymetry_type.setDebug(debug);
  ccbathymetry_method.setDebug(debug);
  ccaltimetry_type.setDebug(debug);
  cctotal_range_steps.setDebug(debug);
  cctotal_transmitters.setDebug(debug);
  cctx_min_depth_offset.setDebug(debug);
  cctx_max_depth_offset.setDebug(debug);
  cctotal_rx_depths.setDebug(debug);
  ccrx_min_depth_offset.setDebug(debug);
  ccrx_max_depth_offset.setDebug(debug);
  cctotal_rx_ranges.setDebug(debug);
  ccrx_min_range_offset.setDebug(debug);
  ccrx_max_range_offset.setDebug(debug);
  cctotal_rays.setDebug(debug);
  ccssp_depth_precision.setDebug(debug);
  ccnormalized_ssp_depth_steps.setDebug(debug);
  cctransducer.setDebug(debug);  
    
  WossCreator::updateDebugFlag();
}


BellhopWoss* const BellhopCreator::createWoss( const CoordZ& tx, const CoordZ& rx, double start_frequency, double end_frequency ) const {
  SimTime time = getSimTime( tx, rx );
  assert( time.start_time.isValid() && time.end_time.isValid() );
                               
  BellhopWoss* ret_value = new BellhopWoss( tx, rx, time.start_time, time.end_time, start_frequency, end_frequency, getFrequencyStep() );
  assert( initializeBhWoss(ret_value) );
  return ret_value;
}


bool BellhopCreator::initializeWoss( Woss* const woss_ptr ) const {
  assert( WossCreator::initializeWoss(woss_ptr) );
  return( woss_ptr->initialize() );
}


bool BellhopCreator::initializeBhWoss( BellhopWoss* const woss_ptr ) const {
  assert( !ccbellhop_mode.isEmpty() && !ccbathymetry_type.isEmpty() && !ccbeam_options.isEmpty() 
          && !ccaltimetry_type.isEmpty() );

  const CoordZ& tx = woss_ptr->getTxCoordZ(); 
  const CoordZ& rx = woss_ptr->getRxCoordZ();
  CustomTransducer transducer_params = cctransducer.get( tx, rx );
      
  woss_ptr->setThorpeAttFlag(use_thorpe_att)
           .setTotalTransmitters(cctotal_transmitters.get( tx, rx ))
           .setTxMinDepthOffset(cctx_min_depth_offset.get( tx, rx ))
           .setTxMaxDepthOffset(cctx_max_depth_offset.get( tx, rx ))
           .setRxTotalDepths(cctotal_rx_depths.get( tx, rx ))
           .setRxMinDepthOffset(ccrx_min_depth_offset.get( tx, rx ))
           .setRxMaxDepthOffset(ccrx_max_depth_offset.get( tx, rx ))
           .setRxTotalRanges(cctotal_rx_ranges.get( tx, rx ))
           .setRxMinRangeOffset(ccrx_min_range_offset.get( tx, rx ))
           .setRxMaxRangeOffset(ccrx_max_range_offset.get( tx, rx ))
           .setRaysNumber(cctotal_rays.get( tx, rx ))
		   .setBoxDepth(ccbox_depth.get(tx, rx))
		   .setBoxRange(ccbox_range.get(tx, rx))
           .setMinAngle(ccangles_map.get( tx, rx ).min_angle)
           .setMaxAngle(ccangles_map.get( tx, rx ).max_angle)
           .setTransducer(transducer_handler->getValue(transducer_params.type))
           .setBeamPatternParam(transducer_params.initial_bearing, transducer_params.initial_vert_rotation,
                                transducer_params.initial_horiz_rotation,
                                transducer_params.multiply_costant, transducer_params.add_costant )
           .setTransformSSPDepthSteps(ccnormalized_ssp_depth_steps.get( tx, rx ))
           .setBellhopPath(bellhop_path)
           .setBellhopArrSyntax(bellhop_arr_syntax)
           .setBellhopShdSyntax(bellhop_shd_syntax)
           .setBathymetryType(ccbathymetry_type.get( tx, rx ))
           .setBathymetryMethod(ccbathymetry_method.get( tx, rx))
           .setAltimetryType(ccaltimetry_type.get( tx, rx ))
           .setBhMode(ccbellhop_mode.get( tx, rx ))
           .setBeamOptions(ccbeam_options.get( tx, rx ))
           .setSSPDepthPrecision(ccssp_depth_precision.get( tx, rx ))
           .setRangeSteps(cctotal_range_steps.get( tx, rx ));

  return( initializeWoss( woss_ptr ) );
}


const BellhopWoss* BellhopCreator::createNotValidWoss() const {
  return new BellhopWoss();
}
