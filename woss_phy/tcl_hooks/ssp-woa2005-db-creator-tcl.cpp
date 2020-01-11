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
 * @file   ssp-woa2005-db-creator-tcl.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::SspWoa2005DbCreatorTcl class
 *
 * Provides the implementation of the woss::SspWoa2005DbCreatorTcl class
 */


#ifdef WOSS_NS_MIRACLE_SUPPORT

#include <iostream>
#include "ssp-woa2005-db-creator-tcl.h"

#ifdef WOSS_NETCDF_SUPPORT
using namespace woss;


#if defined (WOSS_NETCDF4_SUPPORT)
static class SspWoa2013DbCreatorClass : public TclClass {
public:
  SspWoa2013DbCreatorClass() : TclClass("WOSS/Creator/Database/NetCDF/SSP/WOA2013/MonthlyAverage") {}
  TclObject* create(int, const char*const*) {    
    return( new SspWoa2005DbCreatorTcl(WOA_DB_TYPE_2013) );
  }
} class_SspWoa2013DbCreator;
#else
#include <cstdlib>

static class SspWoa2013DbCreatorClass : public TclClass {
public:
  SspWoa2013DbCreatorClass() : TclClass("WOSS/Creator/Database/NetCDF/SSP/WOA2013/MonthlyAverage") {}
  TclObject* create(int, const char*const*) {
    ::std::cerr << "WOSS/Creator/Database/NetCDF/SSP/WOA2013 can't be instantiated. NETCDF4 support was not enabled!" 
                << ::std::endl;
    exit(1);    
  }
} class_SspWoa2013DbCreator;
#endif // defined (WOSS_NETCDF4_SUPPORT)

static class SspWoa2005DbCreatorClass : public TclClass {
public:
  SspWoa2005DbCreatorClass() : TclClass("WOSS/Creator/Database/NetCDF/SSP/WOA2005/MonthlyAverage") {}
  TclObject* create(int, const char*const*) {    
    return( new SspWoa2005DbCreatorTcl() );
  }
} class_SspWoa2005DbCreator;


SspWoa2005DbCreatorTcl::SspWoa2005DbCreatorTcl() 
: SspWoa2005DbCreator()
{
  bind("debug", &debug_);
  bind("woss_db_debug", &woss_db_debug_);
  
  debug = (bool) debug_;
  woss_db_debug = (bool) woss_db_debug_;
}


#if defined (WOSS_NETCDF4_SUPPORT)
SspWoa2005DbCreatorTcl::SspWoa2005DbCreatorTcl( WOADbType db_type )
: SspWoa2005DbCreator(db_type)
{
  bind("debug", &debug_);
  bind("woss_db_debug", &woss_db_debug_);

  debug = (bool) debug_;
  woss_db_debug = (bool) woss_db_debug_;
}
#endif // defined (WOSS_NETCDF4_SUPPORT)

int SspWoa2005DbCreatorTcl::command(int argc, const char*const* argv) {
  if ( argc == 3 ) {
    if(strcasecmp(argv[1], "setDbPathName") == 0) {
      
      if (debug) ::std::cout << "SspWoa2005DbCreatorTcl::command() setDbPathName " << argv[2] << " called"  << ::std::endl;

      pathname = argv[2];
      
      return TCL_OK;
    }
  }
  return( TclObject::command(argc,argv) );
}

#else 
#include <cstdlib>

static class SspWoa2005DbCreatorClass : public TclClass {
public:
  SspWoa2005DbCreatorClass() : TclClass("WOSS/Creator/Database/NetCDF/SSP/WOA2005/MonthlyAverage") {}
  TclObject* create(int, const char*const*) {    
    ::std::cerr << "WOSS/Creator/Database/NetCDF/SSP/WOA2005/MonthlyAverage can't be instantiated. NETCDF support was not enabled!" 
                << ::std::endl;
    exit(1);    
  }
} class_SspWoa2005DbCreator;

#endif // WOSS_NETCDF_SUPPORT

#endif // WOSS_NS_MIRACLE_SUPPORT
