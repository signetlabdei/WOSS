/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2009 Federico Guerra
 * and regents of the SIGNET lab, University of Padova
 *
 * Author: Federico Guerra - WOSS@guerra-tlc.com
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
#include <singleton-definitions.h>
#include "woss-controller-tcl.h"


using namespace woss;

/**
* \brief Singleton implementation of WossControllerTcl class
*
* Singleton implementation of WossControllerTcl class
*/
using SWossControllerTcl = woss::Singleton< WossControllerTcl >;

static class WossControllerClass : public TclClass {
  public:
  WossControllerClass() : TclClass("WOSS/Controller") {}
  TclObject* create(int, const char*const*) {
    return( &SWossControllerTcl::instance() );
  }
} class_WossController;


WossControllerTcl::WossControllerTcl()
{
  bind("debug", &debug);
    
  if ( debug > 0.0 ) 
    woss::SWossController::instance().setDebug(true);
}


int WossControllerTcl::command( int argc, const char*const* argv ) {
  Tcl& tcl = Tcl::instance();

  if ( argc == 3) {
    if(strcasecmp(argv[1], "setBathymetryDbCreator") == 0) {
      
      if (debug) 
        std::cout << "WossControllerTcl::command() setBathymetryDbCreator called"  << std::endl;

      auto bathymetry_db_creator = dynamic_cast< WossDbCreator* >( tcl.lookup(argv[2]) );

      if ( bathymetry_db_creator ) {
        // we supply a custom deleter that does nothing since ownership is within TCL object
        auto bathy_db_creator_shared_ptr = std::shared_ptr<WossDbCreator>(bathymetry_db_creator, [](WossDbCreator* ptr) {});
        woss::SWossController::instance().setBathymetryDbCreator(bathy_db_creator_shared_ptr);

        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setSedimentDbCreator") == 0) {
      if (debug) 
        std::cout << "WossControllerTcl::command() setSedimentDbCreator called"  << std::endl;

      auto sediment_db_creator = dynamic_cast< WossDbCreator* >( tcl.lookup(argv[2]) );

      if ( sediment_db_creator ) {
        // we supply a custom deleter that does nothing since ownership is within TCL object
        auto sediment_db_creator_shared_ptr = std::shared_ptr<WossDbCreator>(sediment_db_creator, [](WossDbCreator* ptr) {});
        woss::SWossController::instance().setSedimentDbCreator(sediment_db_creator_shared_ptr);

        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setSSPDbCreator") == 0) {
      if (debug) 
        std::cout << "WossControllerTcl::command() setSSPDbCreator called"  << std::endl;

      auto ssp_db_creator = dynamic_cast< WossDbCreator* >( tcl.lookup(argv[2]) );

      if ( ssp_db_creator ) {
        // we supply a custom deleter that does nothing since ownership is within TCL object
        auto ssp_db_creator_shared_ptr = std::shared_ptr<WossDbCreator>(ssp_db_creator, [](WossDbCreator* ptr) {});
        woss::SWossController::instance().setSSPDbCreator(ssp_db_creator_shared_ptr);

        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setPressureResultsDbCreator") == 0) {
      if (debug) 
        std::cout << "WossControllerTcl::command() setPressureResultsDbCreator called"  << std::endl;

      auto pressure_result_db_creator = dynamic_cast< WossDbCreator* >( tcl.lookup(argv[2]) );

      if ( pressure_result_db_creator ) {
        // we supply a custom deleter that does nothing since ownership is within TCL object
        auto pressure_result_db_creator_shared_ptr = std::shared_ptr<WossDbCreator>(pressure_result_db_creator, [](WossDbCreator* ptr) {});
        woss::SWossController::instance().setPressureDbCreator(pressure_result_db_creator_shared_ptr);

        return TCL_OK;
      }
      else return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setTimeArrResultsDbCreator") == 0) {
      if (debug) 
        std::cout << "WossControllerTcl::command() setTimeArrResultsDbCreator called"  << std::endl;

      auto timearr_result_db_creator = dynamic_cast< WossDbCreator* >( tcl.lookup(argv[2]) );

      if ( timearr_result_db_creator ) {
        // we supply a custom deleter that does nothing since ownership is within TCL object
        static auto timearr_result_db_creator_shared_ptr = std::shared_ptr<WossDbCreator>(timearr_result_db_creator, [](WossDbCreator* ptr) {});
        woss::SWossController::instance().setTimeArrDbCreator(timearr_result_db_creator_shared_ptr);
        
        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setWossCreator") == 0) {
      if (debug) 
        std::cout << "WossControllerTcl::command() setWossCreator called"  << std::endl;

      auto woss_creator = dynamic_cast< WossCreator* >( tcl.lookup(argv[2]) );

      if ( woss_creator ) {
        // we supply a custom deleter that does nothing since ownership is within TCL object
        auto woss_creator_shared_ptr = std::shared_ptr<WossCreator>(woss_creator, [](WossCreator* ptr) {});
        woss::SWossController::instance().setWossCreator(woss_creator_shared_ptr);
        
        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setWossDbManager") == 0) {
      if (debug) 
        std::cout << "WossControllerTcl::command() setWossDbManager called"  << std::endl;

      auto woss_db_manager = dynamic_cast< WossDbManager* >( tcl.lookup(argv[2]) );

      if ( woss_db_manager ) {
        // we supply a custom deleter that does nothing since ownership is within TCL object
        auto woss_db_manager_shared_ptr = std::shared_ptr<WossDbManager>(woss_db_manager, [](WossDbManager* ptr) {});
        woss::SWossController::instance().setWossDbManager(woss_db_manager_shared_ptr);

        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setWossManager") == 0) {
      if (debug) 
        std::cout << "WossControllerTcl::command() setWossManager called"  << std::endl;

      auto woss_manager = dynamic_cast< WossManager* >( tcl.lookup(argv[2]) );

      if ( woss_manager ) {
        // we supply a custom deleter that does nothing since ownership is within TCL object
        auto woss_manager_shared_ptr = std::shared_ptr<WossManager>(woss_manager, [](WossManager* ptr) {});
        woss::SWossController::instance().setWossManager(woss_manager_shared_ptr);

        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setTransducerHandler") == 0) {
      if (debug) 
        std::cout << "WossControllerTcl::command() setTransducerHandler called"  << std::endl;

      auto transducer_handler = dynamic_cast< TransducerHandler* >( tcl.lookup(argv[2]) );

      if ( transducer_handler ) {
        // we supply a custom deleter that does nothing since ownership is within TCL object
        auto transducer_handler_shared_ptr = std::shared_ptr<TransducerHandler>(transducer_handler, [](TransducerHandler* ptr) {});
        woss::SWossController::instance().setTransducerHandler(transducer_handler_shared_ptr);

        return TCL_OK;
      }
      else 
        return TCL_ERROR;
    }  
  }
  else if ( argc == 2) {
    if(strcasecmp(argv[1], "initialize") == 0) {
      if (debug) 
        std::cout << "WossControllerTcl::command() initialize called"  << std::endl;

      assert( woss::SWossController::instance().initialize() );
      return TCL_OK;
    }
  }
  return TclObject::command(argc, argv);
}

#endif // WOSS_NS_MIRACLE_SUPPORT
