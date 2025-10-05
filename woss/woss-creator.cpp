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
 * @file   woss-creator.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::WossCreator class
 *
 * Provides the implementation of woss::WossCreator class
 */


#include "woss-creator.h"


using namespace woss;


WossCreator::WossCreator() 
: woss_db_manager(nullptr),
  transducer_handler(nullptr),
  work_dir_path(),
  ccevolution_time_quantum(),
  cctotal_runs(),
  ccfrequency_step(), 
  ccsimtime_map(),
  debug(false),
  woss_debug(false),
  woss_clean_workdir(false)
{ 
  if ( ccfrequency_step.accessAllLocations() <= 0.0 ) 
    ccfrequency_step.accessAllLocations() = WOSS_CREATOR_MAX_FREQ_STEP;
  
  WossCreator::updateDebugFlag();
}

void WossCreator::updateDebugFlag() {
  ccsimtime_map.setDebug(debug);
  ccevolution_time_quantum.setDebug(debug);
  cctotal_runs.setDebug(debug);
  ccfrequency_step.setDebug(debug);
}

bool WossCreator::initializeWoss( Woss& woss_ref ) const {
  assert( work_dir_path.size() > 0 );
  assert( woss_db_manager != nullptr );
  
  const CoordZ& tx = woss_ref.getTxCoordZ(); 
  const CoordZ& rx = woss_ref.getRxCoordZ();
  
  woss_ref.setWorkDirPath( work_dir_path );
  woss_ref.setDebug( woss_debug );
  woss_ref.setCleanWorkDir( woss_clean_workdir );
  woss_ref.setTotalRuns( cctotal_runs.get( tx, rx ) );
  woss_ref.setEvolutionTimeQuantum( ccevolution_time_quantum.get( tx, rx ) );
  woss_ref.setWossDbManager( woss_db_manager );

  return true;
}

