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
 * @file   woss-db-manager-tcl.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::WossDbManagerTcl class
 *
 * Provides the implementation of the woss::WossDbManagerTcl class
 */


#ifdef WOSS_NS_MIRACLE_SUPPORT    


#include <singleton-definitions.h>
#include "woss-db-manager-tcl.h"


using namespace woss;
   

static class WossDbManagerClass : public TclClass {
public:
  WossDbManagerClass() : TclClass("WOSS/Database/Manager") {}
  TclObject* create(int, const char*const*) {
    return( Singleton<WossDbManagerTcl>::instance() );
  }
} class_WossDbManagerDb;

    
WossDbManagerTcl::WossDbManagerTcl() 
: WossDbManager()
{ 
  bind("debug", &debug_);
  
  debug = (bool) debug_;
  
  updateDebugFlag();
}


int WossDbManagerTcl::command( int argc, const char*const* argv ) {
  Tcl& tcl = Tcl::instance();
  
  if ( argc == 13 ) {
    if(strcasecmp(argv[1], "setCustomSediment") == 0) { // setCustomSediment tx_lat tx_long bearing range depth sedim_name v_p v_s dens a_p a_s
      
      double range = atof( argv[5] );
      double bearing = atof( argv[4] );
      if ( range <= 0.0 ) range = CCSediment::DB_CDATA_ALL_INNER_KEYS;
      if ( bearing > 180.0 || bearing < -180.0 ) bearing = CCSediment::DB_CDATA_ALL_MEDIUM_KEYS;
      else bearing = bearing * M_PI / 180.0;
      
      Sediment* ptr = SDefHandler::instance()->getSediment()->create( argv[6], atof(argv[7]), atof(argv[8]), atof(argv[9]), 
                                                                         atof(argv[10]), atof(argv[11]), atof(argv[12]) );
                          
      bool done = setCustomSediment( ptr, Coord(atof(argv[2]), atof(argv[3])), bearing, range );
        
      if (debug) ::std::cout << "WossDbManagerTcl::command() setCustomSediment called, tx latitude = " 
                           << atof(argv[2]) << "; tx longitude = " << atof(argv[3])  
                           << "; bearing = " << bearing << "; range = " << range 
                           << "; type name = " << ::std::string(argv[6]) << " v_p = " << atof(argv[7]) 
                           << "; v_s = " << atof(argv[8]) << "; dens = " << atof(argv[9]) 
                           << "; a_p = " << atof(argv[10]) << "; a_s = " <<  atof(argv[11]) 
                           << "; depth = " << atof(argv[12]) << ::std::endl;

      if ( done ) return TCL_OK;
      else return TCL_ERROR;
    }
  }
  else if ( argc == 9 ) {
    if(strcasecmp(argv[1], "setCustomSediment") == 0) { // setCustomSediment sedim_name v_p v_s dens a_p a_s   
      Sediment* ptr = SDefHandler::instance()->getSediment()->create( argv[2], atof(argv[3]), atof(argv[4]), atof(argv[5]), atof(argv[6]), atof(argv[7]), atof(argv[8]) );

      
      if (debug) ::std::cout << "WossDbManagerTcl::command() setCustomSediment called, ptr address = " << ptr
                           << "; Sediment = " << *ptr << ::std::endl;
      
      bool done = setCustomSediment( ptr );

     
      if ( done ) return TCL_OK;
      else return TCL_ERROR;
    }
  }
  else if ( argc == 7 ) {
    if(strcasecmp(argv[1], "setCustomAltimetry") == 0) { // setCustomAltimetry lat long bearing range ssp_filename_path

      double bearing = atof( argv[4] );

      if ( bearing > 180.0 || bearing < -180.0 ) bearing = CCAltimetry::DB_CDATA_ALL_MEDIUM_KEYS;
      else bearing = bearing * M_PI / 180.0;

      if (debug) ::std::cout << "WossDbManagerTcl::command() setCustomAltimetry called, tx_lat = " 
                             << atof(argv[2]) << "; tx_long = " << atof(argv[3]) << "; bearing = " << bearing 
                             << "; range " << atof(argv[5]) << ::std::endl;

      Altimetry* altim_val = dynamic_cast< Altimetry* >( tcl.lookup(argv[2]) );
      
      if (altim_val) {
        //Altimetry* cloned_alt = altim_val->clone();
        setCustomAltimetry( altim_val->clone(), Coord(atof(argv[2]), atof(argv[3])), atof(argv[4]), atof(argv[5]) );
        return TCL_OK;
      }
      else return TCL_ERROR;
    }        
  }
  else if ( argc == 7 ) {
    if(strcasecmp(argv[1], "setCustomSSP") == 0) { // setCustomSSP lat long bearing Time ssp_filename_path

      double bearing = atof( argv[4] );

      if ( bearing > 180.0 || bearing < -180.0 ) bearing = CCSSP::DB_CDATA_ALL_MEDIUM_KEYS;
      else bearing = bearing * M_PI / 180.0;
      const Time* time = dynamic_cast< const Time*>( tcl.lookup(argv[5]) );
      if ( ( time == NULL ) || (!time->isValid()) ) time = &CCSSP::DB_CDATA_ALL_TIME_KEYS;
        
      if (debug) ::std::cout << "WossDbManagerTcl::command() setCustomSSP called, tx_lat = " 
                             << atof(argv[2]) << "; tx_long = " << atof(argv[3]) << "; bearing = " << bearing 
                             << "; time = " << time << "; filename = " << argv[6] << ::std::endl;        
      
      bool valid = importCustomSSP( ::std::string(argv[5]), *time, Coord(atof(argv[2]), atof(argv[3])), bearing );

      if ( valid ) return TCL_OK;
      else return TCL_ERROR;
    }
  }
  else if (argc==4) {
    if(strcasecmp(argv[1], "setCustomSSP") == 0) { // setCustomSSP filename

      const Time* time = dynamic_cast< const Time*>( tcl.lookup(argv[2]) );
      if ( ( time == NULL ) || (!time->isValid()) ) time = &CCSSP::DB_CDATA_ALL_TIME_KEYS;

      if (debug) ::std::cout << "WossDbManagerTcl::command() setCustomSSP called, time = " << *time
                             << "; filename = " << argv[3] << ::std::endl;

      bool valid = importCustomSSP( ::std::string(argv[3]), *time );
      
      if (valid) return TCL_OK;
      else return TCL_ERROR;
    }    
    else if(strcasecmp(argv[1], "getBathymetry") == 0) { // lat long (decimal degrees) 

      double ret_bathy = getBathymetry(  Coord(atof(argv[2]) , atof(argv[3])), Coord(atof(argv[2]) , atof(argv[3])) );

      if (debug) ::std::cout << "WossDbManager::command() getBathymetry called, latitude = " << atof(argv[2]) 
                            << "; longitude = " << atof(argv[3]) << "; bathymetry = " << ret_bathy << ::std::endl;

      tcl.resultf("%.17g",ret_bathy);
      return TCL_OK;
    }
  }
  else if (argc == 3) {
    if(strcasecmp(argv[1], "setCustomAltimetry") == 0) { // setCustomAltimetry filename
          
      Altimetry* altim_val = dynamic_cast< Altimetry* >( tcl.lookup(argv[2]) );
      
      if ( altim_val != NULL ) {

        if (debug) ::std::cout << "WossDbManagerTcl::command() setCustomAltimetry called" << *altim_val << ::std::endl;
        
        setCustomAltimetry(altim_val->clone());
        return TCL_OK;
      }
      
      if (debug) ::std::cout << "WossDbManagerTcl::command() setCustomAltimetry null pointer"  << ::std::endl;
      
      else return TCL_ERROR;
    }    
  }
  else if (argc==2) {
    if(strcasecmp(argv[1], "closeAllConnections") == 0) {

      if (debug) ::std::cout << "WossDbManager::command() closeAllConnections called"  << ::std::endl;

      bool done = closeAllConnections();

      if (done) return TCL_OK;
      else return TCL_ERROR;
    }
  }
  if(strcasecmp(argv[1], "setCustomBathymetry") == 0) { // setCustomBathymetry lat long bearing total_ranges range bathy_value ...

    Coord tx_coord ( atof(argv[2]), atof(argv[3]) );
    double bearing = atof( argv[4] );
    if ( bearing > 180.0 || bearing < -180.0 ) bearing = CCBathymetry::DB_CDATA_ALL_MEDIUM_KEYS; 
    else bearing = bearing * M_PI / 180.0;
    
    int total_values = atoi(argv[5]);

    if (debug) ::std::cout << "WossDbManagerTcl::command() setCustomBathymetry called, tx latitude = " << atof(argv[2]) 
                           << "; tx longitude = " << atof(argv[3]) << "; bearing = " << bearing << "; total values = " << total_values << ::std::endl;
   
    if ( total_values == 0 ) {
      bool valid = importCustomBathymetry( ::std::string(argv[6]), tx_coord, bearing );
      if (valid ) return TCL_OK;
      else return TCL_ERROR;
    }
    else {
      for ( int i = 0; i < 2*total_values; i += 2 ) {
        assert(i < argc);
        int range = 6 + i;
        int depth = 6 + i + 1;
        
        ccbathy_map[tx_coord][bearing][atof(argv[range])] = atof(argv[depth]);
        
        if (debug) ::std::cout << "WossDbManagerTcl::command() setCustomBathymetry range = " << atof(argv[range]) << "; bathymetry = "
                               << atof(argv[depth]) << ::std::endl;

      }
    }

    return TCL_OK; 
  }
  return TclObject::command(argc,argv);
}


#endif // WOSS_NS_MIRACLE_SUPPORT

 
