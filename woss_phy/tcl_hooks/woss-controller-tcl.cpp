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
 * @file   woss-controller-tcl.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation for woss::WossControllerTcl class
 *
 * Provides the implementation for the woss::WossControllerTcl class
 */


#ifdef WOSS_NS_MIRACLE_SUPPORT


#include <cassert>
#include <iostream>
#include <woss-creator.h>
#include <woss-manager.h>
#include <woss-db-creator.h>
#include <woss-db-manager.h>
#include <transducer-handler.h>
#include "woss-controller-tcl.h"


using namespace woss;


static class WossControllerClass : public TclClass {
  public:
  WossControllerClass() : TclClass("WOSS/Controller") {}
  TclObject* create(int, const char*const*) {
    return( SWossControllerTcl::instance() );
  }
} class_WossController;


WossControllerTcl::WossControllerTcl()
{
  bind("debug", &debug);
    
  if ( debug > 0.0 ) SWossController::instance()->setDebug(true);
}


int WossControllerTcl::command( int argc, const char*const* argv ) {
  Tcl& tcl = Tcl::instance();

  if ( argc == 3) {
    if(strcasecmp(argv[1], "setBathymetryDbCreator") == 0) {
      
      if (debug) ::std::cout << "WossControllerTcl::command() setBathymetryDbCreator called"  << ::std::endl;

      woss::WossDbCreator* bathymetry_db_creator = dynamic_cast< WossDbCreator* >( tcl.lookup(argv[2]) );
            
      if ( bathymetry_db_creator ) {
        woss::SWossController::instance()->setBathymetryDbCreator(bathymetry_db_creator);

        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setSedimentDbCreator") == 0) {

      if (debug) ::std::cout << "WossControllerTcl::command() setSedimentDbCreator called"  << ::std::endl;

      woss::WossDbCreator* sediment_db_creator = dynamic_cast< WossDbCreator* >( tcl.lookup(argv[2]) );

      if ( sediment_db_creator ) {
        woss::SWossController::instance()->setSedimentDbCreator(sediment_db_creator);

        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setSSPDbCreator") == 0) {

      if (debug) ::std::cout << "WossControllerTcl::command() setSSPDbCreator called"  << ::std::endl;

      woss::WossDbCreator* ssp_db_creator = dynamic_cast< WossDbCreator* >( tcl.lookup(argv[2]) );

      if ( ssp_db_creator ) {
        woss::SWossController::instance()->setSSPDbCreator(ssp_db_creator);

        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setPressureResultsDbCreator") == 0) {
      
      if (debug) ::std::cout << "WossControllerTcl::command() setPressureResultsDbCreator called"  << ::std::endl;

      woss::WossDbCreator* pressure_result_db_creator = dynamic_cast< WossDbCreator* >( tcl.lookup(argv[2]) );

      if ( pressure_result_db_creator ) {
        woss::SWossController::instance()->setPressureDbCreator(pressure_result_db_creator);

        return TCL_OK;
      }
      else return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setTimeArrResultsDbCreator") == 0) {
      
      if (debug) ::std::cout << "WossControllerTcl::command() setTimeArrResultsDbCreator called"  << ::std::endl;

      woss::WossDbCreator* timearr_result_db_creator = dynamic_cast< WossDbCreator* >( tcl.lookup(argv[2]) );

      if ( timearr_result_db_creator ) {
        woss::SWossController::instance()->setTimeArrDbCreator(timearr_result_db_creator);
        
        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setWossCreator") == 0) {
      
      if (debug) ::std::cout << "WossControllerTcl::command() setWossCreator called"  << ::std::endl;

      woss::WossCreator* woss_creator = dynamic_cast< WossCreator* >( tcl.lookup(argv[2]) );

      if ( woss_creator ) {
        woss::SWossController::instance()->setWossCreator(woss_creator);
        
        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setWossDbManager") == 0) {
      
      if (debug) ::std::cout << "WossControllerTcl::command() setWossDbManager called"  << ::std::endl;

      WossDbManager* woss_db_manager = dynamic_cast< WossDbManager* >( tcl.lookup(argv[2]) );

      if ( woss_db_manager ) {
        woss::SWossController::instance()->setWossDbManager(woss_db_manager);

        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setWossManager") == 0) {
      
      if (debug) ::std::cout << "WossControllerTcl::command() setWossManager called"  << ::std::endl;

      WossManager* woss_manager = dynamic_cast< WossManager* >( tcl.lookup(argv[2]) );

      if ( woss_manager ) {
         woss::SWossController::instance()->setWossManager(woss_manager);
       
        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setTransducerHandler") == 0) {
      
      if (debug) ::std::cout << "WossControllerTcl::command() setTransducerHandler called"  << ::std::endl;

      TransducerHandler* transducer_handler = dynamic_cast< TransducerHandler* >( tcl.lookup(argv[2]) );

      if ( transducer_handler ) {
        woss::SWossController::instance()->setTransducerHandler(transducer_handler);
       
        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }  
  }
  else if ( argc == 2) {
    if(strcasecmp(argv[1], "initialize") == 0) {
      
      if (debug) ::std::cout << "WossControllerTcl::command() initialize called"  << ::std::endl;

      assert( woss::SWossController::instance()->initialize() );
      return TCL_OK;
    }
  }
  return TclObject::command(argc, argv);
}


#endif // WOSS_NS_MIRACLE_SUPPORT


