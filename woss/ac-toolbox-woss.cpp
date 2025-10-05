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
 * @file   ac-toolbox-woss.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::ACToolboxWoss class
 *
 * Provides the implementation of the woss::ACToolboxWoss class
 */


#include <cstdlib>
#include <cmath>
#include <woss-db-manager.h>
#include "ac-toolbox-woss.h"


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
    ssp_map(),
    sediment_map(),
    altimetry_value(nullptr),
    is_ssp_map_transformable(false)
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
    ssp_map(),
    sediment_map(),
    altimetry_value(nullptr),
    is_ssp_map_transformable(false)
{

}


bool ACToolboxWoss::isValid() const {
  return( start_time.isValid() && end_time.isValid() && tx_coordz.isValid() 
          && rx_coordz.isValid() && frequencies.size() > 0);
}


void ACToolboxWoss::resetSSPMap() {
  ssp_map.clear();
}


void ACToolboxWoss::resetSedimentMap() {
  sediment_map.clear();
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

  resetSedimentMap();
  is_ok = initSedimentMap();
  assert(is_ok);

  if ( altimetry_value != nullptr ) {
    altimetry_value.reset();
  }

  is_ok = initAltimetry();
  if (is_ok) 
    assert( (min_bathymetry_depth > min_altimetry_depth) && (max_altimetry_depth < max_bathymetry_depth) );
  
  resetSSPMap();
  is_ok = initSSPMap();
  assert(is_ok);
  
  return true;
}


bool ACToolboxWoss::initRangeVector() {
  for (int i = 0; i <= total_range_steps; i++) {
    range_vector.push_back( ( total_great_circle_distance / (total_range_steps) ) * i );
    
    if (debug)
      std::cout << "ACToolboxWoss(" << woss_id << ")::initRangeVector() i = " << i
                << "; curr range = " << range_vector[i] << std::endl; 
  }
  return( range_vector.size() > 0 );
}


bool ACToolboxWoss::initCoordZVector() {
  if (debug) 
    std::cout << "ACToolboxWoss(" << woss_id << ")::initCoordZVector() tx coord = " << tx_coordz << "; rx coord = " << rx_coordz << " ; bearing = " << bearing << std::endl;

  bool valid = true;
  CoordZ curr_coord;
  double curr_bathy;

  for (const auto& it : range_vector) {
    if ( it == 0.0 ) { // first range
      curr_coord = tx_coordz;
    }
    else if ( it == total_great_circle_distance ) { // last range
      curr_coord = rx_coordz;
    }
    else {
      curr_coord = CoordZ( Coord::getCoordFromBearing(tx_coordz, bearing, it) );
    }

    curr_bathy = db_manager->getBathymetry( tx_coordz, curr_coord );

    assert( curr_bathy != HUGE_VAL && curr_bathy >= 0);
    
    if (curr_bathy > max_bathymetry_depth) max_bathymetry_depth = curr_bathy;
    if (curr_bathy < min_bathymetry_depth) min_bathymetry_depth = curr_bathy;

    curr_coord.setDepth( curr_bathy );
    
    valid = valid && curr_coord.isValid();

    if (debug)
      std::cout << "ACToolboxWoss(" << woss_id << ")::initCoordZVector() coordinate = " << curr_coord << std::endl;

    coordz_vector.push_back(curr_coord);
  }

  return valid;
}


bool ACToolboxWoss::initAltimetry() {
  if ( altimetry_value == nullptr ) 
    altimetry_value = db_manager->getAltimetry( tx_coordz, rx_coordz );
  
  bool ret_value = false;

  altimetry_value->setRange(tx_coordz.getGreatCircleDistance(rx_coordz));
  altimetry_value->setTotalRangeSteps(total_range_steps);
  altimetry_value->setDepth( max_bathymetry_depth );
  
  if (debug) 
    std::cout << "ACToolboxWoss(" << woss_id << ")initAltimetry() altimetry_value " 
              << *altimetry_value << "; is valid = " << altimetry_value->isValid() << std::flush << std::endl;

  if ( altimetry_value->isValid() == true ) {
    if (debug) 
      std::cout << "ACToolboxWoss(" << woss_id << ")::initAltimetry() range =  " << altimetry_value->getRange() 
                << "; total range steps = " << altimetry_value->getTotalRangeSteps() << std::endl;

    ret_value = altimetry_value->initialize();

    if ( ret_value == true ) {
      min_altimetry_depth = altimetry_value->getMinAltimetryValue();
      max_altimetry_depth = altimetry_value->getMaxAltimetryValue();

      if (debug) 
        std::cout << "ACToolboxWoss(" << woss_id << ")::initAltimetry() Altimetry = " 
                  << *altimetry_value
                  << "; min altim depth = " << min_altimetry_depth 
                  << "; max altim depth = " << max_altimetry_depth << std::endl;
    }
  }
  else {
    ret_value = false;
  }
  return( ret_value );
}


bool ACToolboxWoss::initSedimentMap() {
  for (auto it = coordz_vector.cbegin() ; it != coordz_vector.cend(); ++it) {
    auto curr_sediment = db_manager->getSediment( tx_coordz, *it );

    if ( curr_sediment->isValid() ) {
      if (debug)
        std::cout << "ACToolboxWoss(" << woss_id << ")::initSedimentMap() i = " 
                  << std::distance(coordz_vector.cbegin(), it) 
                  << "; sediment = " << *curr_sediment << std::endl;

      if (true == checkSedimentUnicity( *curr_sediment )) {
        auto key = std::distance(coordz_vector.cbegin(), it);
        auto ret = sediment_map.try_emplace( key, std::move(curr_sediment) );
        
        assert(ret.second == true);

        if (debug)
          std::cout << "ACToolboxWoss(" << woss_id << ")::initSedimentMap() unique sedim found, i = " 
                    << ret.first->first << "; range = " << range_vector[ret.first->first]
                    << "; sediment map size = " << sediment_map.size() << std::endl;
      }
    }
    else {
      if (debug) 
        std::cout << "ACToolboxWoss(" << woss_id << ")::initSedimentMap() invalid sediment at range step i = " 
                  << std::distance(coordz_vector.cbegin(), it) << std::endl;
    }
  }

  if (debug) 
    std::cout << "ACToolboxWoss(" << woss_id << ")::initSedimentMap() sediment map size = " << sediment_map.size() 
              << std::endl;

  return( sediment_map.size() > 0 );
}


bool ACToolboxWoss::checkSedimentUnicity( const Sediment& ref ) const {
  for( const auto& it : sediment_map) {
    if( ref == (*it.second) ) {
      return false;
    }
  }
  return true;
}


bool ACToolboxWoss::initSSPMap() {
  is_ssp_map_transformable = true;

  for( auto it = coordz_vector.cbegin(); it != coordz_vector.cend(); ++it ) {
    auto curr_ssp = db_manager->getSSP( tx_coordz, *it, current_time );

    if ( curr_ssp->isValid() ) {
      is_ssp_map_transformable = is_ssp_map_transformable && curr_ssp->isTransformable();

      min_ssp_depth_set.emplace( curr_ssp->getMinDepthValue() );
      max_ssp_depth_set.emplace( curr_ssp->getMaxDepthValue() );

      if ( curr_ssp->size() > max_ssp_depth_steps ) 
        max_ssp_depth_steps = curr_ssp->size();
      if ( curr_ssp->size() < min_ssp_depth_steps ) 
        min_ssp_depth_steps = curr_ssp->size();

      if (debug) 
        std::cout << "ACToolboxWoss(" << woss_id << ")::initSSPMap() i = " 
                  << std::distance(coordz_vector.cbegin(), it)
                  << "; ssp = " << *curr_ssp << std::endl;

      if (true == checkSSPUnicity( *curr_ssp ))
      {
        auto key = std::distance(coordz_vector.cbegin(), it);
        auto ret = ssp_map.try_emplace( key, std::move(curr_ssp) );

        assert(ret.second == true);

        if (debug) 
          std::cout << "ACToolboxWoss(" << woss_id << ")::initSSPMap() unique SSP found, i = " << ret.first->first
                    << "; range = " << range_vector[ret.first->first] 
                    << "; ssp map size = " << ssp_map.size() << std::endl;
      }
    }
    else {
      if (debug)
        std::cout << "ACToolboxWoss(" << woss_id << ")::initSSPMap() invalid ssp at range step i = " 
                  << std::distance(coordz_vector.cbegin(), it) << std::endl;
    }
  }

  if (debug)
    std::cout << "ACToolboxWoss(" << woss_id << ")::initSSPMap() ssp vector size = " 
              << ssp_map.size() << std::endl;

  return( ssp_map.size() > 0 );
}


bool ACToolboxWoss::checkSSPUnicity( const SSP& ref ) const {
  for( const auto& it : ssp_map ) {
    if( ref == *(it.second) ) {
      return false;
    }
  }

  return true;
}
