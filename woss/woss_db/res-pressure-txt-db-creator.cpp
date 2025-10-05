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
 * @file   res-pressure-txt-db-creator.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::ResPressureTxtDbCreator class
 *
 * Provides the implementation of the woss::ResPressureTxtDbCreator class
 */


#include <cassert>
#include "res-pressure-txt-db-creator.h"
#include "res-pressure-txt-db.h"


using namespace woss;


std::unique_ptr<WossDb> ResPressureTxtDbCreator::createWossDb() const {
  assert( pathname.length() > 0 );

  if ( debug ) 
    std::cout << "ResPressureTxtDbCreator::createWossDb() pathname = " << pathname << std::endl;

  auto woss_db = std::make_unique<ResPressureTxtDb>( pathname );

  woss_db->setSpaceSampling(space_sampling);

  bool ok = initializeDb( *woss_db );
  assert( ok );
  
  return( woss_db );
}


bool ResPressureTxtDbCreator::initializeDb( WossDb& woss_db ) const {
  return( WossDbCreator::initializeDb( woss_db ) );
}



