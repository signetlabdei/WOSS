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
 * @file   ac-toolbox-woss.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::ACToolboxWoss class
 *
 * Provides the implementation of the woss::ACToolboxWoss class
 */


#include <cstdlib>
#include <cmath>
#include "ac-toolbox-woss.h"
#include <woss-db-manager.h>


using namespace woss;


ACToolboxWoss::ACToolboxWoss() 
  : WossResReader(),
    ssp_depth_precision(SSP_CUSTOM_DEPTH_PRECISION),
    min_bathymetry_depth(HUGE_VAL),
    max_bathymetry_depth(0.0),
    min_altimetry_depth(HUGE_VAL),
    max_altimetry_depth(-HUGE_VAL),    
    min_ssp_depth_set(),
    max_ssp_depth_set(),
    min_ssp_depth_steps(INT_MAX),
    max_ssp_depth_steps(0),
    total_range_steps(0),
    coordz_vector(),
    range_vector(),
    ssp_vector(),
    ssp_unique_indexes(),
    sediment_value(NULL),
    altimetry_value(NULL),
    is_ssp_vector_transformable(false)
{

}
   

ACToolboxWoss::ACToolboxWoss( const CoordZ& tx, const CoordZ& rx, const Time& start_t, const Time& end_t, double start_freq, double end_freq, double freq_step ) 
  : WossResReader( tx, rx, start_t, end_t, start_freq, end_freq, freq_step ),
    ssp_depth_precision(SSP_CUSTOM_DEPTH_PRECISION),
    min_bathymetry_depth(HUGE_VAL),
    max_bathymetry_depth(0.0),
    min_altimetry_depth(HUGE_VAL),
    max_altimetry_depth(-HUGE_VAL),        
    min_ssp_depth_set(),
    max_ssp_depth_set(),
    min_ssp_depth_steps(INT_MAX),
    max_ssp_depth_steps(0),
    total_range_steps(0),
    coordz_vector(),
    range_vector(),
    ssp_vector(),
    ssp_unique_indexes(),
    sediment_value(NULL),
    altimetry_value(NULL),
    is_ssp_vector_transformable(false)
{

}

ACToolboxWoss::~ACToolboxWoss() {
  delete sediment_value;
  delete altimetry_value;
  
  sediment_value = NULL;
  altimetry_value = NULL;
  
  resetSSPVector();
}


bool ACToolboxWoss::isValid() const {
  return( start_time.isValid() && end_time.isValid() && tx_coordz.isValid() && rx_coordz.isValid() && frequencies.size() > 0);
}


void ACToolboxWoss::resetSSPVector() {
  for( SSPVector::iterator it = ssp_vector.begin(); it != ssp_vector.end(); it++) {
    if ( *it != NULL ) delete *it;
  }
  ssp_vector.clear();  
}


bool ACToolboxWoss::initialize() {

  bool is_ok = WossResReader::initialize();
  assert(is_ok);
  
  range_vector.clear();
  is_ok = initRangeVector();
  assert(is_ok);

  coordz_vector.clear();
  is_ok = initCoordZVector();
  assert(is_ok);

  if ( sediment_value != NULL ) {
    delete sediment_value;
    sediment_value = NULL;
  }
  is_ok = initSediment();
  assert(is_ok);

  if ( altimetry_value != NULL ) {
    delete altimetry_value;
    altimetry_value = NULL;
  }
  is_ok = initAltimetry();
  if (is_ok) assert( (min_bathymetry_depth > min_altimetry_depth) && (max_altimetry_depth < max_bathymetry_depth) );
  
//   ::std::cout << "ACToolboxWoss(" << woss_id << ")::initialize() altimetry_value " << altimetry_value 
//               << "; is valid = " << altimetry_value->isValid() << ::std::flush << ::std::endl;
  
//   debugWaitForUser();
  
  resetSSPVector();
  is_ok = initSSPVector();
  assert(is_ok);
  
  return true;
}


bool ACToolboxWoss::initRangeVector() {
  for (int i = 0; i <= total_range_steps; i++) {
    range_vector.push_back( ( total_great_circle_distance / (total_range_steps) ) * i );
    
    if (debug) ::std::cout << "ACToolboxWoss(" << woss_id << ")::initRangeVector() i = " << i;
    if (debug) ::std::cout << "; curr range = " << range_vector[i] << ::std::endl; 
    
  }
  return( range_vector.size() > 0 );
}


bool ACToolboxWoss::initCoordZVector() {
  if (debug) ::std::cout << "ACToolboxWoss(" << woss_id << ")::initCoordZVector() tx coord = " << tx_coordz << "; rx coord = " << rx_coordz << " ; bearing = " << bearing << ::std::endl;

  bool valid = true;
  CoordZ curr_coord;
  double curr_bathy;
  
  for (int i = 0; i <= total_range_steps; i++) {

    if ( i == 0 ) curr_coord = tx_coordz;
    else if ( i == total_range_steps ) curr_coord = rx_coordz;
    else curr_coord = CoordZ( Coord::getCoordFromBearing(tx_coordz, bearing, range_vector[i]) );
      
    curr_bathy = db_manager->getBathymetry( tx_coordz, curr_coord );

    assert( curr_bathy != HUGE_VAL && curr_bathy >= 0);
    
    if (curr_bathy > max_bathymetry_depth) max_bathymetry_depth = curr_bathy;
    if (curr_bathy < min_bathymetry_depth) min_bathymetry_depth = curr_bathy;

    curr_coord.setDepth( curr_bathy );
    
    valid = valid && curr_coord.isValid();

    if (debug) ::std::cout << "ACToolboxWoss(" << woss_id << ")::initCoordZVector() i = " << i << " coordinate " << curr_coord << ::std::endl;

    coordz_vector.push_back(curr_coord);
  }

  return valid;
}


bool ACToolboxWoss::initSediment() {
  sediment_value = db_manager->getSediment( tx_coordz, coordz_vector );
  
  if (debug) ::std::cout << "ACToolboxWoss(" << woss_id << ")::initSediment() Sediment = " << *sediment_value << ::std::endl;
  
  return( sediment_value->isValid() );
}


bool ACToolboxWoss::initAltimetry() {
  if ( altimetry_value == NULL ) 
    altimetry_value = db_manager->getAltimetry( tx_coordz, rx_coordz );
  
  bool ret_value = false;

  altimetry_value->setRange(tx_coordz.getGreatCircleDistance(rx_coordz));
  altimetry_value->setTotalRangeSteps(total_range_steps);
  altimetry_value->setDepth( max_bathymetry_depth );
  
  if (debug) ::std::cout << "ACToolboxWoss(" << woss_id << ")initAltimetry() altimetry_value " 
                         << altimetry_value << "; is valid = " << altimetry_value->isValid() << ::std::flush << ::std::endl;
            
//   debugWaitForUser();
  
  if ( altimetry_value->isValid() == true ) {
  
    if (debug) ::std::cout << "ACToolboxWoss(" << woss_id << ")::initAltimetry() range =  " << altimetry_value->getRange() 
                           << "; total range steps = " << altimetry_value->getTotalRangeSteps() << ::std::endl;
              
    ret_value = altimetry_value->initialize();

    if ( ret_value == true ) {   
      min_altimetry_depth = altimetry_value->getMinAltimetryValue();
      max_altimetry_depth = altimetry_value->getMaxAltimetryValue();

  
      if (debug) ::std::cout << "ACToolboxWoss(" << woss_id << ")::initAltimetry() Altimetry = " 
                             << altimetry_value << "; " << *altimetry_value 
                             << "; min altim depth = " << min_altimetry_depth 
                             << "; max altim depth = " << max_altimetry_depth << ::std::endl;    
      
//       debugWaitForUser();
      
    }
  }
  else {
    //delete altimetry_value;
    //altimetry_value = SDefHandler::instance()->getAltimetry()->create( Altimetry::createFlat() );
    //altimetry_value = NULL;
    ret_value = false;
  }
  return( ret_value );
}


bool ACToolboxWoss::initSSPVector() {
  is_ssp_vector_transformable = true;
  
  for( int i = 0; i <= total_range_steps; i++ ) {
    SSP* curr_ssp;
   
    curr_ssp = db_manager->getSSP( tx_coordz, coordz_vector[i], current_time );

    if ( curr_ssp->isValid() ) {
      is_ssp_vector_transformable = is_ssp_vector_transformable && curr_ssp->isTransformable();

      min_ssp_depth_set.insert( curr_ssp->getMinDepthValue() );
      max_ssp_depth_set.insert( curr_ssp->getMaxDepthValue() );

      if ( curr_ssp->size() > max_ssp_depth_steps ) max_ssp_depth_steps = curr_ssp->size();
      if ( curr_ssp->size() < min_ssp_depth_steps ) min_ssp_depth_steps = curr_ssp->size();

      checkSSPUnicity( curr_ssp ); 
      ssp_vector.push_back( curr_ssp );
    }
  }
  
  if (debug) ::std::cout << "ACToolboxWoss(" << woss_id << ")::initSSPVector() ssp vector size = " << ssp_vector.size() 
                         << "; unique ssp = " << ssp_unique_indexes.size() << ::std::endl;
  
  return( ssp_vector.size() > 0 );
}


void ACToolboxWoss::checkSSPUnicity( SSP*& ptr ) {
  for( SSPVector::iterator it = ssp_vector.begin(); it != ssp_vector.end(); it++) {
    if( *ptr == **it ) {
      delete ptr;
      ptr = NULL;
      return;
    }
  }
  ssp_unique_indexes.insert( ssp_vector.size() );
}


