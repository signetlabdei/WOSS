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
 * @file   definitions-handler-tcl.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::DefHandlerTcl class
 *
 * Provides the implementation of the woss::DefHandlerTcl class
 */


#ifdef WOSS_NS_MIRACLE_SUPPORT


#include <sediment-definitions.h>
#include <time-definitions.h>
#include <time-arrival-definitions.h>
#include <ssp-definitions.h>
#include <transducer-definitions.h>
#include <altimetry-definitions.h>
#include "definitions-handler-tcl.h"


using namespace woss;


static class DefHandlerClass : public TclClass {
  public:
  DefHandlerClass() : TclClass("WOSS/Definitions/Handler") {}
  TclObject* create(int, const char*const*) {
    return( SDefHandlerTcl::instance() );
  }
} class_DefHandler;


DefHandlerTcl::DefHandlerTcl()
: debug(0)
{ 
  bind("debug", &debug);
  
  if ( debug > 0 ) SDefHandler::instance()->setDebug(true);
}


int DefHandlerTcl::command( int argc, const char*const* argv ) {
  Tcl& tcl = Tcl::instance();

  if ( argc == 3) {
    if(strcasecmp(argv[1], "setPressureCreator") == 0) {
      if (debug) ::std::cout << "DefHandlerTcl::command() setPressureCreator called"  << ::std::endl;

      Pressure* pressure_creator = dynamic_cast< Pressure* >( tcl.lookup(argv[2]) )->clone();

      if ( pressure_creator ) {
        SDefHandler::instance()->setPressure(pressure_creator);
        
        return TCL_OK;
      }
      else return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setTimeArrCreator") == 0) {
      if (debug) ::std::cout << "DefHandlerTcl::command() setTimeArrCreator called"  << ::std::endl;

      TimeArr* time_arr_creator = dynamic_cast< TimeArr* >( tcl.lookup(argv[2]) )->clone();

      if ( time_arr_creator ) {
        SDefHandler::instance()->setTimeArr(time_arr_creator);
        
        return TCL_OK;
      }
      else return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setSedimentCreator") == 0) {
      if (debug) ::std::cout << "DefHandlerTcl::command() setSedimentCreator called"  << ::std::endl;

      Sediment* sediment_creator = dynamic_cast< Sediment* >( tcl.lookup(argv[2]) )->clone();

      if ( sediment_creator ) { 
        SDefHandler::instance()->setSediment(sediment_creator); 
        
        return TCL_OK;
      }
      else return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setSSPCreator") == 0) {
      if (debug) ::std::cout << "DefHandlerTcl::command() setSSPCreator called"  << ::std::endl;

      SSP* ssp_creator = dynamic_cast< SSP* >( tcl.lookup(argv[2]) )->clone();

      if ( ssp_creator ) { 
        SDefHandler::instance()->setSSP(ssp_creator); 
        
        return TCL_OK;
      }
      else return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setAltimetryCreator") == 0) {
      if (debug) ::std::cout << "DefHandlerTcl::command() setAltimetryCreator called"  << ::std::endl;

      Altimetry* altimetry_creator = dynamic_cast< Altimetry* >( tcl.lookup(argv[2]) )->clone();

      if ( altimetry_creator ) {
        SDefHandler::instance()->setAltimetry(altimetry_creator);
        
        return TCL_OK;
      }
      else return TCL_ERROR;
    }    
    else if(strcasecmp(argv[1], "setTimeReference") == 0) {
      if (debug) ::std::cout << "DefHandlerTcl::command() setTimeReference called"  << ::std::endl;

      TimeReference* time_reference = dynamic_cast< TimeReference* >( tcl.lookup(argv[2]) )->clone();

      if ( time_reference ) {
        SDefHandler::instance()->setTimeReference(time_reference);
        
        return TCL_OK;
      }
      else return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setRandomGenerator") == 0) {
      if (debug) ::std::cout << "DefHandlerTcl::command() setRandomGenerator called"  << ::std::endl;

      RandomGenerator* rand_generator = dynamic_cast< RandomGenerator* >( tcl.lookup(argv[2]) )->clone();

      if ( rand_generator ) {
        SDefHandler::instance()->setRandGenerator(rand_generator);
        
        return TCL_OK;
      }
      else return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setTransducerCreator") == 0) {
      if (debug) ::std::cout << "DefHandlerTcl::command() setTransducerCreator called"  << ::std::endl;

      Transducer* transducer = dynamic_cast< Transducer* >( tcl.lookup(argv[2]) )->clone();

      if ( transducer ) {
        SDefHandler::instance()->setTransducer(transducer);
        
        return TCL_OK;
      }
      else return TCL_ERROR;
    }
  }
  return TclObject::command(argc, argv);
}


#endif // WOSS_NS_MIRACLE_SUPPORT
 
