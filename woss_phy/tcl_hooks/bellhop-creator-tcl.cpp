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


#ifdef WOSS_NS_MIRACLE_SUPPORT  


#include "bellhop-creator-tcl.h"


using namespace woss;


static class BellhopCreatorClass : public TclClass {
public:
  BellhopCreatorClass() : TclClass("WOSS/Creator/Bellhop") {}
  TclObject* create(int, const char*const*) {
    return (new BellhopCreatorTcl());
  }
} class_BellhopCreatorClass;


BellhopCreatorTcl::BellhopCreatorTcl() 
: BellhopCreator()
{ 
  bind( "total_range_steps", &cctotal_range_steps.accessAllLocations() );
  bind( "tx_min_depth_offset", &cctx_min_depth_offset.accessAllLocations() );
  bind( "tx_max_depth_offset", &cctx_max_depth_offset.accessAllLocations() );
  bind( "total_transmitters", &cctotal_transmitters.accessAllLocations() );
  bind( "total_rx_depths", &cctotal_rx_depths.accessAllLocations() );
  bind( "rx_min_depth_offset", &ccrx_min_depth_offset.accessAllLocations() );
  bind( "rx_max_depth_offset", &ccrx_max_depth_offset.accessAllLocations() );
  bind( "total_rx_ranges", &cctotal_rx_ranges.accessAllLocations() );
  bind( "rx_min_range_offset", &ccrx_min_range_offset.accessAllLocations() );
  bind( "rx_max_range_offset", &ccrx_max_range_offset.accessAllLocations() );
  bind( "total_rays", &cctotal_rays.accessAllLocations() );
  bind( "min_angle", &ccangles_map.accessAllLocations().min_angle );
  bind( "max_angle", &ccangles_map.accessAllLocations().max_angle );
  bind( "ssp_depth_precision", &ccssp_depth_precision.accessAllLocations() );
  bind( "normalized_ssp_depth_steps", &ccnormalized_ssp_depth_steps.accessAllLocations() );
  bind( "box_depth", &ccbox_depth.accessAllLocations() );
  bind( "box_range", &ccbox_range.accessAllLocations() );
  bind( "woss_debug", &woss_debug_);  
  bind( "debug", &debug_);  
  bind( "woss_clean_workdir", &woss_clean_workdir_);  
  bind( "frequency_step", &ccfrequency_step.accessAllLocations());
  bind( "evolution_time_quantum", &ccevolution_time_quantum.accessAllLocations());
  bind( "total_runs", &cctotal_runs.accessAllLocations());
  bind( "bellhop_arr_syntax", &bellhop_arr_syntax_); //!< 1 new syntax, 0 old syntax
  
  if ( ccfrequency_step.accessAllLocations() <= 0.0 ) ccfrequency_step.accessAllLocations() = WOSS_CREATOR_MAX_FREQ_STEP;
  
  woss_debug = (bool) woss_debug_;
  debug = (bool) debug_;
  woss_clean_workdir = (bool) woss_clean_workdir_;
  bellhop_arr_syntax = (BellhopArrSyntax) bellhop_arr_syntax_;
  
  updateDebugFlag();  
}


int BellhopCreatorTcl::command( int argc, const char*const* argv ) {
  Tcl& tcl = Tcl::instance();

 if ( argc == 16 ) {
    if(strcasecmp(argv[1], "setSimulationTimes") == 0) {
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCSimTime::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCSimTime::ALL_LOCATIONS;
      
      ccsimtime_map.replace( SimTime( Time( atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atoi(argv[7]), atoi(argv[8]), atoi(argv[9]) ),
                                      Time( atoi(argv[10]), atoi(argv[11]), atoi(argv[12]), atoi(argv[13]), atoi(argv[14]), atoi(argv[15]) ) ), 
                             tx, rx);
      
      if (debug) ::std::cout << "BellhopCreatorTcl::command() setSimulationTimes called, " << "; start day = " << atoi(argv[4]) << "; start month = "
                             << atoi(argv[5]) << "; start year = " << atoi(argv[6]) << "; start hours = " << atoi(argv[7]) << "; start mins = " 
                             << atoi(argv[8]) << "; start secs = " << atoi(argv[9]) << "; end day = " << atoi(argv[10]) << "; end month = " 
                             << atoi(argv[11]) << "; end year = " << atoi(argv[12]) << "; end hours = " << atoi(argv[13]) << "; end mins = "
                             << atoi(argv[14]) << "; end secs = " << atoi(argv[15]) << ::std::endl;

      return TCL_OK;
    }    
  }
  if ( argc == 9 ) {
      if(strcasecmp(argv[1], "setCustomTransducerType") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCTransducer::ALL_LOCATIONS;

      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCTransducer::ALL_LOCATIONS;     

      
      cctransducer.replace( CustomTransducer(argv[4], 0.0, atof(argv[5]), atof(argv[6]), atof(argv[7]), atof(argv[8]) ) , tx, rx );
    
      return TCL_OK; 
    }
  }
  else if ( argc == 6 ) {

    if(strcasecmp(argv[1], "setCustomAngles") == 0) {  
      
      if (debug) ::std::cout << "BellhopCreatorTcl::command() setCustomAngles called "
                             << "; min angle = " << atof(argv[4]) << "; max_angle = " << atof(argv[5]) 
                             << ::std::endl;
           
      Location* tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if ( tx == NULL ) tx = CCAngles::ALL_LOCATIONS;
 
      Location* rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if ( rx == NULL ) rx = CCAngles::ALL_LOCATIONS;
                 
      ccangles_map.replace( CustomAngles(atof(argv[4]), atof(argv[5])), tx, rx );

      return TCL_OK;
    }
  }  
  else if ( argc == 5 ) {
    
    if(strcasecmp(argv[1], "setBellhopMode") == 0) {
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCString::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCString::ALL_LOCATIONS;
      
      ccbellhop_mode.replace( argv[4], tx, rx );
      
      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setBeamOptions") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCString::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCString::ALL_LOCATIONS;
      
      ccbeam_options.replace( argv[4], tx, rx );
      
      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setBathymetryType") == 0) { 

      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCString::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCString::ALL_LOCATIONS;
      
      ccbathymetry_type.replace( argv[4], tx, rx );
      
      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setBathymetryMethod") == 0) { 

      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCString::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCString::ALL_LOCATIONS;
      
      ccbathymetry_method.replace( argv[4], tx, rx );
      
      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setAltimetryType") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCString::ALL_LOCATIONS;
      
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCString::ALL_LOCATIONS;
      
      ccaltimetry_type.replace( argv[4], tx, rx );
      
      return TCL_OK;
    }    
    else if(strcasecmp(argv[1], "setRangeSteps") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCInt::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCInt::ALL_LOCATIONS;
      
      cctotal_range_steps.replace( atoi(argv[4]), tx, rx );
      
      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setTotalTransmitters") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCInt::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCInt::ALL_LOCATIONS;
      
      cctotal_transmitters.replace( atoi(argv[4]), tx, rx );
      
      return TCL_OK;
    }    
    else if(strcasecmp(argv[1], "setTxMinDepthOffset") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCDouble::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCDouble::ALL_LOCATIONS;
      
      cctx_min_depth_offset.replace( atof(argv[4]), tx, rx );
      
      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setTxMaxDepthOffset") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCDouble::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCDouble::ALL_LOCATIONS;
      
      cctx_max_depth_offset.replace( atof(argv[4]), tx, rx );
      
      return TCL_OK;
    } 
    else if(strcasecmp(argv[1], "setRxTotalDepths") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCInt::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCInt::ALL_LOCATIONS;
      
      cctotal_rx_depths.replace( atoi(argv[4]), tx, rx );
      
      return TCL_OK;
    }    
    else if(strcasecmp(argv[1], "setRxMinDepthOffset") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCDouble::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCDouble::ALL_LOCATIONS;
      
      ccrx_min_depth_offset.replace( atof(argv[4]), tx, rx );
      
      return TCL_OK;
    } 
    else if(strcasecmp(argv[1], "setRxMaxDepthOffset") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCDouble::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCDouble::ALL_LOCATIONS;
      
      ccrx_max_depth_offset.replace( atof(argv[4]), tx, rx );
      
      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setRxTotalRanges") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCInt::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCInt::ALL_LOCATIONS;
      
      cctotal_rx_ranges.replace( atoi(argv[4]), tx, rx );
      
      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setRxMinRangeOffset") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCDouble::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCDouble::ALL_LOCATIONS;
      
      ccrx_min_range_offset.replace( atof(argv[4]), tx, rx );
      
      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setRxMaxRangeOffset") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCDouble::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCDouble::ALL_LOCATIONS;
      
      ccrx_max_range_offset.replace( atof(argv[4]), tx, rx );
      
      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setRaysNumber") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCInt::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCInt::ALL_LOCATIONS;
      
      cctotal_rays.replace( atoi(argv[4]), tx, rx );
      
      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setSSPDepthPrecision") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCDouble::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCDouble::ALL_LOCATIONS;
      
      ccssp_depth_precision.replace( atof(argv[4]), tx, rx );
      
      return TCL_OK;
    }    
    else if(strcasecmp(argv[1], "setNormalizedSSPDepthSteps") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCInt::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCInt::ALL_LOCATIONS;
      
      ccnormalized_ssp_depth_steps.replace( atoi(argv[4]), tx, rx );
      
      return TCL_OK;
    } 
    else if(strcasecmp(argv[1], "setBoxDepth") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCInt::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCInt::ALL_LOCATIONS;
      
      ccbox_depth.replace( atoi(argv[4]), tx, rx );
      
      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setBoxRange") == 0) { 
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCInt::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCInt::ALL_LOCATIONS;
      
      ccbox_range.replace( atoi(argv[4]), tx, rx );
      
      return TCL_OK;
    }       
    else if(strcasecmp(argv[1], "setTotalRuns") == 0) {
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCInt::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCInt::ALL_LOCATIONS;
      
      cctotal_runs.replace( atoi(argv[4]), tx, rx );
      
      if (debug) ::std::cout << "BellhopCreatorTcl::command() setWorkDirPath called, value = " << argv[4] << ::std::endl;
      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setEvolutionTimeQuantum") == 0) {
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCInt::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCInt::ALL_LOCATIONS;
      
      ccevolution_time_quantum.replace( atof(argv[4]), tx, rx );

      if (debug) ::std::cout << "BellhopCreatorTcl::command() setEvolutionTimeQuantum called, value = " << argv[4] << ::std::endl;

      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setFrequencyStep") == 0) {
      
      Location* tx = NULL; 
      Location* rx = NULL; 
      
      tx = dynamic_cast< Location* >( tcl.lookup(argv[2]) );
      if (!tx ) tx = CCDouble::ALL_LOCATIONS;
 
      rx = dynamic_cast< Location* >( tcl.lookup(argv[3]) );
      if (!rx ) rx = CCDouble::ALL_LOCATIONS;
      
      ccfrequency_step.replace( atoi(argv[4]), tx, rx  );
      
      if (debug) ::std::cout << "BellhopCreatorTcl::command() setFrequencyStep called, value = " << argv[4] << ::std::endl;

      return TCL_OK;
    }
  }
  else if ( argc == 3 ) {
    if(strcasecmp(argv[1], "setWorkDirPath") == 0) {
      
      work_dir_path = argv[2];
      
      if (debug) ::std::cout << "BellhopCreatorTcl::command() setWorkDirPath called, work dir path = " << work_dir_path << ::std::endl;

      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setBellhopPath") == 0) { 

      bellhop_path = argv[2];
      if (bellhop_path != "" ) bellhop_path += "/";
      
      if (debug) ::std::cout << "BellhopCreatorTcl::command() setBellhopPath called, path = " << argv[2] << ::std::endl;

      return TCL_OK;
    }
  }
  return( TclObject::command(argc,argv) );
}


#endif // WOSS_NS_MIRACLE_SUPPORT

