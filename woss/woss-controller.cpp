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
 * @file   woss-controller.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation for woss::WossController class
 *
 * Provides the implementation for the woss::WossController class
 */


#include "woss-creator.h"
#include "woss-manager.h"
#include <woss-db.h>
#include <woss-db-creator.h>
#include <woss-db-manager.h>
#include <transducer-handler.h>
#include "woss-controller.h"


using namespace woss;



WossController::WossController()
: debug(false),
  initialized(false),
  bathymetry_db_creator(NULL),
  sediment_db_creator(NULL),
  ssp_db_creator(NULL),
  pressure_result_db_creator(NULL), 
  timearr_result_db_creator(NULL), 
  woss_creator(NULL),
  woss_db_manager(NULL),
  woss_manager(NULL),
  transducer_handler(NULL)
{
 
}


WossController::~WossController() {
//   delete bathymetry_db_creator;
//   delete sediment_db_creator;
//   delete ssp_db_creator;
//   delete pressure_result_db_creator;
//   delete timearr_result_db_creator;
//   delete woss_creator;
//   delete woss_db_manager;
//   delete woss_manager;
//   delete transducer_handler;
}


WossController::WossController( WossController& copy )
: debug(copy.debug), 
  initialized(copy.initialized),
  bathymetry_db_creator(copy.bathymetry_db_creator),
  sediment_db_creator(copy.sediment_db_creator),
  ssp_db_creator(copy.ssp_db_creator),
  pressure_result_db_creator(copy.pressure_result_db_creator),
  timearr_result_db_creator(copy.timearr_result_db_creator),
  woss_creator(copy.woss_creator),
  woss_db_manager(copy.woss_db_manager),
  woss_manager(copy.woss_manager),
  transducer_handler(copy.transducer_handler)
{
  copy.initialized = false;
  copy.bathymetry_db_creator = NULL;
  copy.sediment_db_creator = NULL;
  copy.ssp_db_creator = NULL;
  copy.pressure_result_db_creator = NULL;
  copy.timearr_result_db_creator = NULL;
  copy.woss_creator = NULL;
  copy.woss_db_manager = NULL;
  copy.woss_manager = NULL;
  copy.transducer_handler = NULL;
}


WossController& WossController::operator=( WossController& copy ) {
  if (this == &copy) return *this;

  debug = copy.debug;
  initialized = copy.initialized;
  bathymetry_db_creator = copy.bathymetry_db_creator;
  sediment_db_creator = copy.sediment_db_creator;
  ssp_db_creator = copy.ssp_db_creator;
  pressure_result_db_creator = copy.pressure_result_db_creator;
  timearr_result_db_creator = copy.timearr_result_db_creator;
  woss_creator = copy.woss_creator;
  woss_db_manager = copy.woss_db_manager;
  woss_manager = copy.woss_manager;
  transducer_handler = copy.transducer_handler;
  
  copy.initialized = false;
  copy.bathymetry_db_creator = NULL;
  copy.sediment_db_creator = NULL;
  copy.ssp_db_creator = NULL;
  copy.pressure_result_db_creator = NULL;
  copy.timearr_result_db_creator = NULL;
  copy.woss_creator = NULL;
  copy.woss_db_manager = NULL;
  copy.woss_manager = NULL;
  copy.transducer_handler = NULL;
  
  return( *this );
}


bool WossController::initialize() {
  if (initialized) return true;
  
  assert( woss_creator != NULL );
  assert( woss_db_manager != NULL );
  assert( woss_manager != NULL );
  assert( transducer_handler != NULL );
  
  if ( bathymetry_db_creator ) woss_db_manager->setBathymetryDb( dynamic_cast<WossBathymetryDb*>( bathymetry_db_creator->createWossDb() ) );
  
  if ( sediment_db_creator ) woss_db_manager->setSedimentDb( dynamic_cast<WossSedimentDb*>( sediment_db_creator->createWossDb() ) );
 
  if ( ssp_db_creator ) woss_db_manager->setSSPDb( dynamic_cast<WossSSPDb*>( ssp_db_creator->createWossDb() ) );
  
  if ( timearr_result_db_creator ) woss_db_manager->setResTimeArrDb( dynamic_cast<WossResTimeArrDb*>( timearr_result_db_creator->createWossDb() ) );
  
  if ( pressure_result_db_creator ) woss_db_manager->setResPressureDb( dynamic_cast<WossResPressDb*>( pressure_result_db_creator->createWossDb() ) );

  woss_creator->setWossDbManager( woss_db_manager );
  woss_creator->setTransducerHandler( transducer_handler );
  woss_manager->setWossCreator( woss_creator );
  
  WossManagerResDb* ptr = dynamic_cast< WossManagerResDb* >( woss_manager );
  if (ptr) ptr->setWossDbManager( woss_db_manager );
  
  initialized = true;
  return ( initialized );
}





