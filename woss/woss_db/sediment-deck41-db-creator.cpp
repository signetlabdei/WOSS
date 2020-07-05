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
 * @file   sediment-deck41-db-creator.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::SedimDeck41DbCreator class
 *
 * Provides the implementation of the woss::SedimDeck41DbCreator class
 */

#ifdef WOSS_NETCDF_SUPPORT

#include <cassert>
#include "sediment-deck41-db-creator.h"
#include "sediment-deck41-db.h"


using namespace woss;


SedimDeck41DbCreator::SedimDeck41DbCreator()
: db_coord_name(),
  db_marsden_name(),
  db_marsden_one_name(),
  deck41_db_type(DECK41_DB_V1_TYPE)
{

}


SedimDeck41DbCreator::~SedimDeck41DbCreator() {

}


WossDb* const SedimDeck41DbCreator::createWossDb() {
  SedimDeck41Db* woss_db = new SedimDeck41Db( "" );
  
  assert( initializeSedimDb( woss_db ) );
  
  return( woss_db );
}


bool SedimDeck41DbCreator::initializeSedimDb( SedimDeck41Db* const woss_db ) {
  assert( db_coord_name.length() > 0 && db_marsden_name.length() > 0 && db_marsden_one_name.length() > 0 );
  bool valid = true;
  
  woss_db->sediment_coord_db.setDbName( db_coord_name );
  woss_db->sediment_coord_db.setDeck41DbType( deck41_db_type );
  valid = valid && initializeDb( & woss_db->sediment_coord_db );
  
  woss_db->sediment_marsden_db.setDbName( db_marsden_name );
  woss_db->sediment_marsden_db.setDeck41DbType( deck41_db_type );
  valid = valid && initializeDb( & woss_db->sediment_marsden_db );
  
  woss_db->sediment_marsden_one_db.setDbName( db_marsden_one_name );
  woss_db->sediment_marsden_one_db.setDeck41DbType( deck41_db_type );
  valid = valid && initializeDb( & woss_db->sediment_marsden_one_db );
  
  return valid;
}


bool SedimDeck41DbCreator::initializeDb( WossDb* woss_db ) {
  return( WossDbCreator::initializeDb(woss_db) );
}


#endif // WOSS_NETCDF_SUPPORT

