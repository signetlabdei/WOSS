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

  

#include <cassert>
#include "bathymetry-utm-csv-db-creator.h"
#include "bathymetry-utm-csv-db.h"


using namespace woss;


BathyUtmCsvDbCreator::BathyUtmCsvDbCreator()
:
  separator(','),
  db_spacing(1.0),
  total_northing_values(0),
  total_easting_values(0),
  range_easting_start(0.0),
  range_easting_end(0.0),
  range_northing_start(0.0),
  range_northing_end(0.0),
  approx_land_to_sea_surface(false)
{
}


BathyUtmCsvDbCreator::~BathyUtmCsvDbCreator() {

}


WossDb* const BathyUtmCsvDbCreator::createWossDb() {
  assert( pathname.length() > 0 );

  BathyUtmCsvDb* woss_db = new BathyUtmCsvDb( pathname );

  woss_db->setCSVSeparator( separator );
  woss_db->setDbSpacing( db_spacing );
  woss_db->setDbTotalValues( total_northing_values, total_easting_values );
  woss_db->setDbRangeEasting( range_easting_start, range_easting_end );
  woss_db->setDbRangeNorthing( range_northing_start, range_northing_end );
  woss_db->setLandApproximationFlag( approx_land_to_sea_surface );

  assert( initializeDb( woss_db ) );
  return( woss_db );
}


bool BathyUtmCsvDbCreator::initializeDb( WossDb* const woss_db ) {
  return( WossDbCreator::initializeDb( woss_db ) );
}
