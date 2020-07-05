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
 * @file   sediment-deck41-db-creator-tcl.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::SedimDeck41DbCreatorTcl class
 *
 * Provides the implementation of the woss::SedimDeck41DbCreatorTcl class
 */



#ifdef WOSS_NS_MIRACLE_SUPPORT


#include <iostream>
#include "sediment-deck41-db-creator-tcl.h"


#ifdef WOSS_NETCDF_SUPPORT
using namespace woss;

static class SedimDeck41DbCreatorClass : public TclClass {
public:
  SedimDeck41DbCreatorClass() : TclClass("WOSS/Creator/Database/NetCDF/Sediment/DECK41") {}
  TclObject* create(int, const char*const*) {
    return( new SedimDeck41DbCreatorTcl() );  
  }
} class_SedimDeck41DbCreator;


SedimDeck41DbCreatorTcl::SedimDeck41DbCreatorTcl()
: SedimDeck41DbCreator()
{
  bind("debug", &debug_);
  bind("woss_db_debug", &woss_db_debug_);
  
  debug = (bool) debug_;
  woss_db_debug = (bool) woss_db_debug_;
}


int SedimDeck41DbCreatorTcl::command(int argc, const char*const* argv) {
  if ( argc == 3 ) {
    if(strcasecmp(argv[1], "setDbPathName") == 0) {
      
      if (debug) ::std::cout << "SedimDeck41DbCreatorTcl::command() setDbPathName " << argv[2] << " called. error use other commands!"  << ::std::endl;

      pathname = argv[2];
      
      return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setUpDeck41CoordinatesDb") == 0) {
      if (debug) ::std::cout << "SedimDeck41DbCreatorTcl::command() setUpDeck41CoordinatesDb " << argv[2] << " called"  << ::std::endl;

      db_coord_name = argv[2];

      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setUpDeck41MarsdenDb") == 0) {
      if (debug) ::std::cout << "SedimDeck41DbCreatorTcl::command() setUpDeck41MarsdenDb " << argv[2] << " called"  << ::std::endl;

      db_marsden_name = argv[2];

      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setUpDeck41MarsdenOneDb") == 0) {
      if (debug) ::std::cout << "SedimDeck41DbCreatorTcl::command() setUpDeck41MarsdenOneDb " << argv[2] << " called"  << ::std::endl;

      db_marsden_one_name = argv[2];

      return TCL_OK;
    }
  }
  else if (argc == 2) {
    if(strcasecmp(argv[1], "setDeck41DbTypeV1") == 0) {
      if (debug) ::std::cout << "SedimDeck41DbCreatorTcl::command() setDeck41DbTypeV1 called"  << ::std::endl;

#if defined (WOSS_NETCDF4_SUPPORT)
      deck41_db_type = DECK41_DB_V1_TYPE;

      return TCL_OK;
#else
      ::std::cerr << "setDeck41DbTypeV2 can't be executed. NETCDF4 support was not enabled!" 
                  << ::std::endl;

      return TCL_ERROR;
#endif // defined (WOSS_NETCDF4_SUPPORT)
    }
    else if(strcasecmp(argv[1], "setDeck41DbTypeV2") == 0) {
      if (debug) ::std::cout << "SedimDeck41DbCreatorTcl::command() setDeck41DbTypeV2 called"  << ::std::endl;

#if defined (WOSS_NETCDF4_SUPPORT)
      deck41_db_type = DECK41_DB_V2_TYPE;

      return TCL_OK;
#else
      ::std::cerr << "setDeck41DbTypeV2 can't be executed. NETCDF4 support was not enabled!" 
                  << ::std::endl;

      return TCL_ERROR;
#endif // defined (WOSS_NETCDF4_SUPPORT)
    }
  }
  return( TclObject::command(argc,argv) );
}

#else 
#include <cstdlib>

static class SedimDeck41DbCreatorClass : public TclClass {
public:
  SedimDeck41DbCreatorClass() : TclClass("WOSS/Creator/Database/NetCDF/Sediment/DECK41") {}
  TclObject* create(int, const char*const*) {
    ::std::cerr << "WOSS/Creator/Database/NetCDF/Sediment/DECK41 can't be instantiated. NETCDF support was not enabled!" 
                << ::std::endl;
    exit(1);    
    return NULL;  
  }
} class_SedimDeck41DbCreator;

#endif // WOSS_NETCDF_SUPPORT


#endif // WOSS_NS_MIRACLE_SUPPORT


