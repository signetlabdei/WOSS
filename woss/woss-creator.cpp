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
 * @file   woss-creator.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::WossCreator class
 *
 * Provides the implementation of woss::WossCreator class
 */


#include <cstdlib>
#include "woss-creator.h"


using namespace woss;


const Woss* WossCreator::woss_not_valid = NULL;


WossCreator::WossCreator() 
: woss_db_manager(NULL),
  transducer_handler(NULL),
  work_dir_path(),
  ccevolution_time_quantum(),
  cctotal_runs(),
  ccfrequency_step(), 
  ccsimtime_map(),
  debug(false),
  woss_debug(false),
  woss_clean_workdir(false)
{ 
  if ( ccfrequency_step.accessAllLocations() <= 0.0 ) ccfrequency_step.accessAllLocations() = WOSS_CREATOR_MAX_FREQ_STEP;
  
  WossCreator::updateDebugFlag();
}

void WossCreator::updateDebugFlag() {
  ccsimtime_map.setDebug(debug);
  ccevolution_time_quantum.setDebug(debug);
  cctotal_runs.setDebug(debug);
  ccfrequency_step.setDebug(debug);
}

bool WossCreator::initializeWoss( Woss* const woss_ptr ) const {
  assert( work_dir_path.size() > 0 );
  assert( woss_db_manager != NULL );
  
  const CoordZ& tx = woss_ptr->getTxCoordZ(); 
  const CoordZ& rx = woss_ptr->getRxCoordZ();
  
  woss_ptr->setWorkDirPath( work_dir_path );
  woss_ptr->setDebug( woss_debug );
  woss_ptr->setCleanWorkDir( woss_clean_workdir );
  woss_ptr->setTotalRuns( cctotal_runs.get( tx, rx ) );
  woss_ptr->setEvolutionTimeQuantum( ccevolution_time_quantum.get( tx, rx ) );
  woss_ptr->setWossDbManager( woss_db_manager );
  return true;
}


const Woss& WossCreator::getWossNotValid() const {
  if ( woss_not_valid == NULL ) woss_not_valid = createNotValidWoss();
  return *woss_not_valid;
}

