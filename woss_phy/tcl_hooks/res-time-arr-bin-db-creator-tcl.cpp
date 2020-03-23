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
 * @file   res-time-arr-bin-db-creator-tcl.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of ResTimeArrBinDbCreatorTcl class
 *
 * Provides the implementation of the ResTimeArrBinDbCreatorTcl class
 */


#ifdef WOSS_NS_MIRACLE_SUPPORT


#include <iostream>
#include "res-time-arr-bin-db-creator-tcl.h"


using namespace woss;


static class ResTimeArrBinDbCreatorClass : public TclClass {
public:
  ResTimeArrBinDbCreatorClass() : TclClass("WOSS/Creator/Database/Binary/Results/TimeArr") {}
  TclObject* create(int, const char*const*) {
    return( new ResTimeArrBinDbCreatorTcl() );
  }
} class_ResTimeArrBinDbCreator;


ResTimeArrBinDbCreatorTcl::ResTimeArrBinDbCreatorTcl()
: ResTimeArrBinDbCreator()
{
  bind("space_sampling",&space_sampling);
  bind("debug", &debug_);
  bind("woss_db_debug", &woss_db_debug_);
  
  debug = (bool) debug_;
  woss_db_debug = (bool) woss_db_debug_;
}

int ResTimeArrBinDbCreatorTcl::command(int argc, const char*const* argv) {
  if ( argc == 3 ) {
    if(strcasecmp(argv[1], "setDbPathName") == 0) {
      
      if (debug) ::std::cout << "ResTimeArrBinDbCreatorTcl::command() setDbPathName " << argv[2] << " called"  << ::std::endl;

      pathname = argv[2];
      
      return TCL_OK;
    }
  }
  return( TclObject::command(argc,argv) );
}


#endif // WOSS_NS_MIRACLE_SUPPORT

