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
 * @file   bathymetry-gebco-db-creator-tcl.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::BathyGebcoDbCreatorTcl class
 *
 * Provides the implementation of the woss::BathyGebcoDbCreatorTcl class
 */

 
#ifdef WOSS_NS_MIRACLE_SUPPORT

#include <iostream>
#include <cstdlib>
#include "bathymetry-gebco-db-creator-tcl.h"


#ifdef WOSS_NETCDF_SUPPORT  
using namespace woss;

static class BathyGebcoDbCreatorClass : public TclClass {
public:
  BathyGebcoDbCreatorClass() : TclClass("WOSS/Creator/Database/NetCDF/Bathymetry/GEBCO") {}
  TclObject* create(int, const char*const*) {
    return( new BathyGebcoDbCreatorTcl() );
  }
} class_BathyGebcoDbCreator;


BathyGebcoDbCreatorTcl::BathyGebcoDbCreatorTcl()
: BathyGebcoDbCreator()
{
  bind("debug", &debug_);
  bind("woss_db_debug", &woss_db_debug_);
  
  debug = (bool) debug_;
  woss_db_debug = (bool) woss_db_debug_;
}


int BathyGebcoDbCreatorTcl::command(int argc, const char*const* argv) {
  if ( argc == 3 ) {
    if(strcasecmp(argv[1], "setDbPathName") == 0) {
      
      if (debug) ::std::cout << "BathyGebcoDbCreatorTcl::command() setDbPathName " << argv[2] << " called"  << ::std::endl;

      pathname = argv[2];
      
      return TCL_OK;
    }
  }  
  else if (argc == 2) {
    if(strcasecmp(argv[1], "useOneMinutePrecision") == 0) {
      if (debug) ::std::cout << "BathyGebcoDbCreatorTcl::command() useOneMinutePrecision called"  << ::std::endl;

      gebco_type = GEBCO_1D_1_MINUTE_BATHY_TYPE;

      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "useThirtySecondsPrecision") == 0) {
      if (debug) ::std::cout << "BathyGebcoDbCreatorTcl::command() useThirtySecondsPrecision called"  << ::std::endl;

      gebco_type = GEBCO_1D_30_SECONDS_BATHY_TYPE;

      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "use2DOneMinutePrecision") == 0) {
      if (debug) ::std::cout << "BathyGebcoDbCreatorTcl::command() use2DOneMinutePrecision called"  << ::std::endl;

      gebco_type = GEBCO_2D_1_MINUTE_BATHY_TYPE;

      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "use2DThirtySecondsPrecision") == 0) {
      if (debug) ::std::cout << "BathyGebcoDbCreatorTcl::command() use2DThirtySecondsPrecision called"  << ::std::endl;

      gebco_type = GEBCO_2D_30_SECONDS_BATHY_TYPE;

      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "use2DFifteenSecondsPrecision") == 0) {
      if (debug) ::std::cout << "BathyGebcoDbCreatorTcl::command() use2DFifteenSecondsPrecision called"  << ::std::endl;

      gebco_type = GEBCO_2D_15_SECONDS_BATHY_TYPE;

      return TCL_OK;
    }
  }
  return TclObject::command(argc,argv);
}

#else 
#include <cstdlib>

static class BathyGebcoDbCreatorClass :public TclClass {
public:
  BathyGebcoDbCreatorClass() : TclClass("WOSS/Creator/Database/NetCDF/Bathymetry/GEBCO") {}
  TclObject* create(int, const char*const*) {
    ::std::cerr << "WOSS/Creator/Database/NetCDF/Bathymetry/GEBCO can't be instantiated. NETCDF support was not enabled!" 
                << ::std::endl;
    exit(1);    
    return NULL;  
  }
} class_BathyGebcoDbCreatorClass;
#endif // WOSS_NETCDF_SUPPORT

#endif // WOSS_NS_MIRACLE_SUPPORT
