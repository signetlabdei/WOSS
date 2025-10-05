/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2009 2025 Federico Guerra
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
 * @file   woss-controller.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation for woss::WossController class
 *
 * Provides the implementation for the woss::WossController class
 */


#include <woss-db.h>
#include "woss-controller.h"


using namespace woss;


bool WossController::initialize() {
  if (initialized) 
    return true;
  
  assert( woss_creator != nullptr );
  assert( woss_db_manager != nullptr );
  assert( woss_manager != nullptr );
  assert( transducer_handler != nullptr );
  
  if ( bathymetry_db_creator ) {
    auto base_ptr = bathymetry_db_creator->createWossDb();
    if (auto derived_ptr = dynamic_cast<WossBathymetryDb*>(base_ptr.get()))
    {
      base_ptr.release();
      auto derived_uniq_ptr = std::unique_ptr<WossBathymetryDb>( derived_ptr );
      woss_db_manager->setBathymetryDb( std::move(derived_uniq_ptr) );
    }
  }
  if ( sediment_db_creator ) {
    auto base_ptr = sediment_db_creator->createWossDb();
    if (auto derived_ptr = dynamic_cast<WossSedimentDb*>(base_ptr.get()))
    {
      base_ptr.release();
      auto derived_uniq_ptr = std::unique_ptr<WossSedimentDb>( derived_ptr );
      woss_db_manager->setSedimentDb( std::move(derived_uniq_ptr) );
    }
  }
  if ( ssp_db_creator ) {
    auto base_ptr = ssp_db_creator->createWossDb();
    if (auto derived_ptr = dynamic_cast<WossSSPDb*>(base_ptr.get()))
    {
      base_ptr.release();
      auto derived_uniq_ptr = std::unique_ptr<WossSSPDb>( derived_ptr );
      woss_db_manager->setSSPDb( std::move(derived_uniq_ptr) );
    }
  }
  if ( timearr_result_db_creator ) {
    auto base_ptr = timearr_result_db_creator->createWossDb();
    if (auto derived_ptr = dynamic_cast<WossResTimeArrDb*>(base_ptr.get()))
    {
      base_ptr.release();
      auto derived_uniq_ptr = std::unique_ptr<WossResTimeArrDb>( derived_ptr );
      woss_db_manager->setResTimeArrDb( std::move(derived_uniq_ptr) );
    }
  }
  if ( pressure_result_db_creator ) {
    auto base_ptr = pressure_result_db_creator->createWossDb();
    if (auto derived_ptr = dynamic_cast<WossResPressDb*>(base_ptr.get()))
    {
      base_ptr.release();
      auto derived_uniq_ptr = std::unique_ptr<WossResPressDb>( derived_ptr );
      woss_db_manager->setResPressureDb( std::move(derived_uniq_ptr) );
    }
  }
  woss_creator->setWossDbManager( woss_db_manager );
  woss_creator->setTransducerHandler( transducer_handler );
  woss_manager->setWossCreator( woss_creator );
  
  if (auto ptr = dynamic_cast<WossManagerResDb*>(woss_manager.get())) {
    ptr->setWossDbManager( woss_db_manager );
  }

  initialized = true;
  return ( initialized );
}
