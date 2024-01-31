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
 * @file   bellhop-woss.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::BellhopWoss class
 *
 * Provides the implementation of woss::BellhopWoss class
 */


#include <cstdlib>
#include <altimetry-definitions.h>
#include "bellhop-woss.h"


#define BELLHOP_NOT_SET (-3000)

static const char* WOSS_BELLHOP_PROGRAM = "bellhop.exe"; /**< Bellhop program name */

static const char* WOSS_BELLHOP_NAME = "bellhop";  /**< Bellhop configuration file name */

static const char* WOSS_BELLHOP_ATI = ".ati"; /**< Bellhop altimetry file extension */

static const char* WOSS_BELLHOP_BTY = ".bty"; /**< Bellhop batimetry file extension */

//  static const char* WOSS_BELLHOP_TRC = ".trc"; /**< Bellhop top reflection coefficients file extension */

//  static const char* WOSS_BELLHOP_BRC = ".brc"; /**< Bellhop bottom reflection coefficients file extension */

static const char* WOSS_BELLHOP_SBP = ".sbp"; /**< Bellhop source beam pattern extension */

static const char* WOSS_BELLHOP_SSP = ".ssp"; /**< Bellhop SSP matrix file extension */

static const char* WOSS_BELLHOP_SHD = ".shd"; /**< Bellhop binary SHD file extension */

static const char* WOSS_BELLHOP_ARR = ".arr";  /**< Bellhop ARR file extension */

static const char* WOSS_BELLHOP_ENV = ".env";  /**< Bellhop configuration file extension */

//static const char* WOSS_BATHYMETRY_MODE_SLOPE = "S"; /**< Bathymetry write method: Slope */

static const char* WOSS_BATHYMETRY_MODE_DISCRETE = "D"; /**< Bathymetry write method: Discrete */

static const double BELLHOP_QUAD_SSP_RANGE_FACTOR = 1.05; /**< Bellhop range factor multiplier, used in SSP quad file */

using namespace woss;


BellhopWoss::BellhopWoss() 
: use_thorpe_att(true),
  beam_options(),
  bathymetry_type(),
  bathymetry_method(),
  altimetry_type(),
  bellhop_op_mode(),
  bellhop_env_file(),
  bathymetry_file(),
  altimetry_file(),
  beam_pattern_file(),
  ssp_file(),
  shd_file(),
  arr_file(),
  bellhop_path(""),
  curr_path(),
  tx_min_depth_offset(0.0),
  tx_max_depth_offset(0.0),
  total_transmitters(BELLHOP_NOT_SET),
  total_rx_depths(BELLHOP_NOT_SET),
  rx_min_depth_offset(0.0),
  rx_max_depth_offset(0.0),
  total_rx_ranges(BELLHOP_NOT_SET),
  rx_min_range_offset(0.0),
  rx_max_range_offset(0.0),
  total_rays(BELLHOP_NOT_SET),
  min_angle(BELLHOP_NOT_SET),
  max_angle(BELLHOP_NOT_SET),
  min_normalized_ssp_depth(HUGE_VAL),
  max_normalized_ssp_depth(0.0),
  curr_norm_ssp_depth_steps(BELLHOP_NOT_SET),
  transform_ssp_depth_steps(BELLHOP_NOT_SET),
  transducer(NULL),
  bp_initial_bearing(0.0),
  bp_vertical_rotation(0.0),
  bp_horizontal_rotation(0.0),
  bp_mult_costant(1.0),
  bp_add_costant(0.0),
  using_ssp_file(false),
  using_press_mode(false),
  using_time_arrival_mode(false),
  normalized_ssp_map(),
  randomized_ssp_map(),
  box_depth(BELLHOP_NOT_SET),
  box_range(BELLHOP_NOT_SET),
  f_out()
{

}


BellhopWoss::BellhopWoss(const CoordZ& tx, const CoordZ& rx, const Time& start_t, const Time& end_t, double start_freq, double end_freq, double freq_step )  
: ACToolboxWoss( tx, rx, start_t, end_t, start_freq, end_freq, freq_step ), 
  beam_options(),
  bathymetry_type(),
  bathymetry_method(),
  altimetry_type(),
  bellhop_op_mode(),
  bellhop_env_file(),
  bathymetry_file(),
  altimetry_file(),
  beam_pattern_file(),
  ssp_file(),
  shd_file(),
  arr_file(),
  bellhop_path(""),
  curr_path(),
  tx_min_depth_offset(0.0),
  tx_max_depth_offset(0.0),
  total_transmitters(BELLHOP_NOT_SET),
  total_rx_depths(BELLHOP_NOT_SET),
  rx_min_depth_offset(0.0),
  rx_max_depth_offset(0.0),
  total_rx_ranges(BELLHOP_NOT_SET),
  rx_min_range_offset(0.0),
  rx_max_range_offset(0.0),
  total_rays(BELLHOP_NOT_SET),
  min_angle(BELLHOP_NOT_SET),
  max_angle(BELLHOP_NOT_SET),
  min_normalized_ssp_depth(HUGE_VAL),
  max_normalized_ssp_depth(0.0),
  curr_norm_ssp_depth_steps(BELLHOP_NOT_SET),
  transform_ssp_depth_steps(BELLHOP_NOT_SET),
  transducer(NULL),
  bp_initial_bearing(0.0),
  bp_vertical_rotation(0.0),
  bp_horizontal_rotation(0.0),
  bp_mult_costant(1.0),
  bp_add_costant(0.0),
  using_ssp_file(false),
  using_press_mode(false),
  using_time_arrival_mode(false),
  normalized_ssp_map(),
  randomized_ssp_map(),
  box_depth(BELLHOP_NOT_SET),
  box_range(BELLHOP_NOT_SET),
  f_out()
{

}


BellhopWoss::~BellhopWoss() {
//   rmWorkDir();
  for ( NSMIter it = normalized_ssp_map.begin(); it != normalized_ssp_map.end(); it++ ) {
    delete it->second;
    it->second = NULL;
  }
  for ( NSMIter it = randomized_ssp_map.begin(); it != randomized_ssp_map.end(); it++ ) {
    delete it->second;
    it->second = NULL;
  }
}


bool BellhopWoss::isValid() const {
  return ( ACToolboxWoss::isValid() && isValidBhMode(bellhop_op_mode) && total_transmitters != BELLHOP_NOT_SET && total_rx_depths != BELLHOP_NOT_SET 
        && total_range_steps != BELLHOP_NOT_SET && total_rx_ranges != BELLHOP_NOT_SET && total_rays != BELLHOP_NOT_SET 
        && min_angle != BELLHOP_NOT_SET && max_angle != BELLHOP_NOT_SET );
}


BellhopWoss& BellhopWoss::setBhMode( const ::std::string& mode ) {
  if (isValidBhMode(mode) == true) {
    bellhop_op_mode = mode;

    if ( mode == "A" || mode == "a" ) {
      using_time_arrival_mode = true;
      using_press_mode = false;
    }
    else if (mode == "C" || mode == "I" || mode == "S") {
      using_press_mode = true;
      using_time_arrival_mode = false;
    }
  }
  else {
    ::std::cout << "BellhopWoss(" << woss_id << ")::setBhMode() ERROR wrong Bellhop Mode!" << ::std::endl;
    exit(1);
  }

  return *this;
}


void BellhopWoss::resetNormalizedDbSSP()
{
  for( NSMIter it = normalized_ssp_map.begin(); it != normalized_ssp_map.end(); it++ ) {
    if ( it->second != NULL ) delete it->second;
  }
  
  normalized_ssp_map.clear();
}


void BellhopWoss::normalizeDbSSP() {
  double max_bathy_depth;
  max_bathy_depth = max_bathymetry_depth;

  bool is_normalized_matrix = ( min_ssp_depth_set.size() == 1 && max_ssp_depth_set.size() == 1 
                             && min_ssp_depth_steps == max_ssp_depth_steps );

  if (debug) ::std::cout << "BellhopWoss::normalizeDbSSP() is_ssp_vector_transformable = " << is_ssp_vector_transformable
                         << "; transform_ssp_depth_steps = " << transform_ssp_depth_steps 
                         << "; is_normalized_matrix = " << is_normalized_matrix << ::std::endl;

  if ((is_ssp_vector_transformable == true) && (transform_ssp_depth_steps > 0)) {  
    min_normalized_ssp_depth = ::std::min( min_altimetry_depth, *( min_ssp_depth_set.begin() ) );

    if (max_bathy_depth <= *( max_ssp_depth_set.rbegin()))
      max_normalized_ssp_depth = max_bathy_depth;
    else
      max_normalized_ssp_depth = *( max_ssp_depth_set.rbegin());

    if (debug) ::std::cout << "BellhopWoss::normalizeDbSSP() min norm ssp depth = " << min_normalized_ssp_depth
                           << "; min alt depth = " << min_altimetry_depth 
                           << "; max_normalized_ssp_depth " << max_normalized_ssp_depth 
                           << "; max_bathy_depth = " << max_bathy_depth << ::std::endl;
    
    for ( ::std::set<int>::iterator it = ssp_unique_indexes.begin(); it != ssp_unique_indexes.end(); it++) {
      normalized_ssp_map[range_vector[*it]] = ssp_vector[*it]->transform( tx_coordz, min_normalized_ssp_depth, max_normalized_ssp_depth, transform_ssp_depth_steps);
      assert( normalized_ssp_map[range_vector[*it]]->isValid() );
    }

    curr_norm_ssp_depth_steps = normalized_ssp_map.begin()->second->size(); 

    return;
  }

  if ( is_normalized_matrix ) {
    for ( ::std::set<int>::iterator it = ssp_unique_indexes.begin(); it != ssp_unique_indexes.end(); it++) {
      normalized_ssp_map[range_vector[*it]] = ssp_vector[*it]->truncate(ceil(max_bathy_depth));
    }
    min_normalized_ssp_depth = *( min_ssp_depth_set.begin() );
    max_normalized_ssp_depth = *( max_ssp_depth_set.rbegin() );
    if (max_bathy_depth <= *( max_ssp_depth_set.rbegin()))
      max_normalized_ssp_depth = ceil(max_bathy_depth);
    else
      max_normalized_ssp_depth = *( max_ssp_depth_set.rbegin());

    curr_norm_ssp_depth_steps = normalized_ssp_map.begin()->second->size(); 
    return;
  }

  normalized_ssp_map[0.0] = ssp_vector[0]->clone();
  min_normalized_ssp_depth = ssp_vector[0]->getMinDepthValue();
  max_normalized_ssp_depth = ssp_vector[0]->getMaxDepthValue();
  curr_norm_ssp_depth_steps = ssp_vector[0]->size();
}


void BellhopWoss::writeNormalizedSSP( int curr_run ) {
  bool using_ssp_file;

  if (normalized_ssp_map.size() > 1) 
    using_ssp_file = true;
  else 
    using_ssp_file = false;
    
  if (using_ssp_file == false) { 
    f_out << "\'CVW";
  }
  else {
    f_out << "\'QVW";
  }
    
  if (use_thorpe_att == true)
    f_out << "T";
  
  f_out << ( ( (altimetry_value != NULL) && (altimetry_value->isValid()) ) ? "*" : "" )  
        << "\'" << ::std::setw(30) << "! OPTIONS1" << ::std::endl;
  
  f_out << "0 0.0 " << max_normalized_ssp_depth << ::std::setw(30) << "! NMESH SIGMAS Z(NSSP)" << ::std::endl;
 
  NSMIter it1 = normalized_ssp_map.begin();

  /////////////////////////////////////////////
  DConstIter it2;
  DConstIter it3;
  bool has_to_delete = false;
  SSP* curr_ssp = NULL;
  if ( curr_run > 0 ) {
    curr_ssp = it1->second->randomize( 0.0001 );
    has_to_delete = true;
  }
  else {
    curr_ssp = it1->second;
  }
  ////////////////////
  DConstIter it4 = curr_ssp->begin();
  f_out << ::std::min( (double)it4->first, min_normalized_ssp_depth ) << "  " << it4->second << "  / "<< ::std::endl;
  it4++;
  
  for ( ; it4 != curr_ssp->end(); it4++) { 
     f_out << it4->first << "  " << it4->second << "  / "<< ::std::endl;
  }  

  if ( has_to_delete ) {
    delete curr_ssp;
    curr_ssp = NULL;
  }
  if (using_ssp_file == true) {
    if ( curr_run > 0 ) {
      for ( NSMIter it = randomized_ssp_map.begin(); it != randomized_ssp_map.end(); it++ ) {
        delete it->second;
        it->second = NULL;
      }
      randomized_ssp_map.clear();
    }
     
    ::std::ofstream ssp_out ( ssp_file.c_str() );
    assert ( ssp_out.is_open() );

    ssp_out.precision(WOSS_DECIMAL_PRECISION);

    /*
     * Bellhop does NOT extrapolate SSP
     * Therefore we need to add two additional SSPs
     * - the first one at -box_range * BELLHOP_QUAD_SSP_RANGE_FACTOR
     * - the last one at box_range * BELLHOP_QUAD_SSP_RANGE_FACTOR
     */
    ssp_out << normalized_ssp_map.size() + 2 << ::std::endl;

    // first box range
    ssp_out << ::std::setw(30) << (-box_range * BELLHOP_QUAD_SSP_RANGE_FACTOR) / 1000.0;

    //write actual ranges
    for (NSMIter it = normalized_ssp_map.begin(); it != normalized_ssp_map.end(); it++) {
      ssp_out << ::std::setw(30) << ((double) it->first / 1000.0); // write ranges
       
      if ( curr_run > 0 ) {
        randomized_ssp_map[it->first] = it->second->randomize( 0.0001 );
      }
    }
    
    // last box range
    ssp_out << ::std::setw(30) << (box_range * BELLHOP_QUAD_SSP_RANGE_FACTOR) / 1000.0 << ::std::endl;

 
    NormSSPMap* write_ssp_map;
    if ( curr_run > 0 ) {
      write_ssp_map = &randomized_ssp_map;
    }
    else write_ssp_map = &normalized_ssp_map;

    NSMIter first_it = write_ssp_map->begin();
    NSMRIter last_it = write_ssp_map->rbegin();

    assert (first_it != write_ssp_map->end());
    assert (last_it != write_ssp_map->rend());

    for (int i = 0; i < curr_norm_ssp_depth_steps; i++) {
      // write first SSP at first box range
      DConstIter first_it2 = first_it->second->at(i);
      assert( first_it2 != first_it->second->end() );
      ssp_out << ::std::setw(30) << first_it2->second;

      // write actual SSPs
      for (NSMIter it = write_ssp_map->begin(); it != write_ssp_map->end(); it++) {
        DConstIter it2 = it->second->at(i);
        assert( it2 != it->second->end() );
        ssp_out << ::std::setw(30) << it2->second;
      }

      // write last SSP at last box range
      DConstIter last_it2 = last_it->second->at(i);
      assert( last_it2 != last_it->second->end() );
      ssp_out << ::std::setw(30) << last_it2->second;

      ssp_out << ::std::endl;
    }

    ssp_out.close();
  }
}


void BellhopWoss::checkBoundaries( double& frequency, double& tx_depth, double& rx_start_depth, double& rx_start_range, double& rx_end_depth, double& rx_end_range ) const {
  if ( frequency <= *(frequencies.begin()) ) 
    frequency = *(frequencies.begin());
  
  if ( frequency >= *(frequencies.rbegin()) ) 
    frequency = *(frequencies.rbegin());

  if (tx_depth <= tx_coordz.getDepth() + tx_min_depth_offset) 
    tx_depth = tx_coordz.getDepth() + tx_min_depth_offset;
  
  if (tx_depth >= tx_coordz.getDepth() + tx_max_depth_offset) 
    tx_depth = tx_coordz.getDepth() + tx_min_depth_offset; 
  
  if (rx_start_depth <= rx_coordz.getDepth() + rx_min_depth_offset) 
    rx_start_depth = rx_coordz.getDepth() + rx_min_depth_offset;
  
  if (rx_start_depth >= rx_coordz.getDepth() + rx_max_depth_offset) 
    rx_start_depth = rx_coordz.getDepth() + rx_max_depth_offset;
  
  if (rx_end_depth <= rx_coordz.getDepth() + rx_min_depth_offset) 
    rx_end_depth = rx_coordz.getDepth() + rx_min_depth_offset; 
 
  if (rx_end_depth >= rx_coordz.getDepth() + rx_max_depth_offset) 
    rx_end_depth = rx_coordz.getDepth() + rx_max_depth_offset;
  
  if (rx_start_range <= total_great_circle_distance + rx_min_range_offset) 
    rx_start_range = total_great_circle_distance + rx_min_range_offset;
  
  if (rx_start_range >= total_great_circle_distance + rx_max_range_offset) 
    rx_start_range = total_great_circle_distance + rx_max_range_offset;
  
  if (rx_end_range <= total_great_circle_distance + rx_min_range_offset) 
    rx_end_range = total_great_circle_distance + rx_min_range_offset;
  
  if (rx_end_range >= total_great_circle_distance + rx_max_range_offset) 
    rx_end_range = total_great_circle_distance + rx_max_range_offset;
}


void BellhopWoss::checkDepthOffsets(const CoordZ& coordinates, double& min_depth_offset, double& max_depth_offset, double min_depth_value, double max_depth_value) {
  double total_depth = max_depth_value - min_depth_value; 
  double total_depth_offset = abs( max_depth_offset - min_depth_offset );

  if ( total_depth_offset >= total_depth ) total_depth_offset = total_depth - total_depth/50.0;
    
  if ( (coordinates.getDepth() + min_depth_offset) <= min_depth_value ) {
    min_depth_offset = min_depth_value + min_depth_value/50.0 - coordinates.getDepth();
  }

  if ( (coordinates.getDepth() + max_depth_offset) <= min_depth_value ) {
    max_depth_offset = (min_depth_value + min_depth_value/50.0 + total_depth_offset) - coordinates.getDepth();
  }

  if ( (coordinates.getDepth() + min_depth_offset) >= max_depth_value ) {
    min_depth_offset = max_depth_value - max_depth_value/50.0 - total_depth_offset - coordinates.getDepth();
  }

  if ( (coordinates.getDepth() + max_depth_offset) >= max_depth_value ) {
    max_depth_offset = max_depth_value - max_depth_value/50.0 - coordinates.getDepth();
  }

  if (debug) ::std::cerr << "BellhopWoss(" << woss_id << ")::checkDepthOffsets() WARNING, latitude = " 
                            << coordinates.getLatitude() << "; longitude = " << coordinates.getLongitude() 
                            << "; depth = " << (coordinates.getDepth())
                            << "; min depth value = " << min_depth_value << "; max depth value = " << max_depth_value 
                            << "; total depth offset = " << total_depth_offset << "; total depth = " << total_depth 
                            << "; new min depth offset = " << min_depth_offset << "; new max depth offset = " 
                            << max_depth_offset << ::std::endl;
  
}


void BellhopWoss::checkDepthOffsets() {
  double min_depth = min_normalized_ssp_depth;
  if (min_depth <= 0.0) min_depth = 1.0e-1;

  checkDepthOffsets(tx_coordz, tx_min_depth_offset, tx_max_depth_offset, min_depth, coordz_vector[0].getDepth() );
  checkDepthOffsets(rx_coordz, rx_min_depth_offset, rx_max_depth_offset, min_depth, ::std::min(max_bathymetry_depth, max_normalized_ssp_depth) );
}


void BellhopWoss::checkRangeOffsets() {
  double total_range_offset = abs( rx_max_range_offset - rx_min_range_offset );

  if ( total_range_offset >= 1.1*total_great_circle_distance ) total_range_offset = 1.1*total_great_circle_distance - total_great_circle_distance/50.0;

  if ( rx_min_range_offset <= -total_great_circle_distance ) {
    double new_value = -total_great_circle_distance;
    rx_min_range_offset = new_value - new_value/1000.0;
  }

  if ( rx_max_range_offset <= -total_great_circle_distance ) {
    double new_value = (-total_great_circle_distance + total_range_offset);
    rx_max_range_offset = new_value - new_value/1000.0;
  }

  if ( rx_min_range_offset >= 0.1*total_great_circle_distance ) {
    double new_value = (1.1*total_great_circle_distance - total_range_offset);
    rx_min_range_offset = new_value - new_value/1000.0;
  }

  if ( rx_max_range_offset >= 0.1*total_great_circle_distance ) {
    double new_value = 0.1*total_great_circle_distance;
    rx_max_range_offset = new_value - new_value/1000.0;
  }

  if (debug) ::std::cerr << "BellhopWoss(" << woss_id << ")::checkRangeOffsets() WARNING, tx latitude = " 
                            << tx_coordz.getLatitude() << "; tx longitude = " << tx_coordz.getLongitude() 
                            << "; rx latitude = " << rx_coordz.getLatitude() << "; rx longitude = " << rx_coordz.getLongitude() 
                            << "; total range offset = " << total_range_offset << "; total great circle distance = " << total_great_circle_distance
                            << ::std::endl << "new min rx range offset = " << rx_min_range_offset << "; new rx max range offset = " 
                            << rx_max_range_offset << ::std::endl;

}


void BellhopWoss::checkAngles() {
  if ( total_great_circle_distance == 0.0 && total_distance != 0.0 ) {

    if (debug)
      ::std::cout << "BellhopWoss(" << woss_id << ")::checkAngles() vertical channel: old angles = " << min_angle << "; " << max_angle << ::std::endl; 
    
    max_angle = ::std::max( ::std::abs( min_angle ), ::std::abs( max_angle ) );
    min_angle = -max_angle; 

    if (debug)
      ::std::cout << "BellhopWoss(" << woss_id << ")::checkAngles() vertical channel: new angles = " << min_angle << "; " << max_angle << ::std::endl; 
  }
}


void BellhopWoss::writeBathymetryFile() {
  ::std::ofstream baty_out (bathymetry_file.c_str());
  assert ( baty_out.is_open() );

  baty_out.precision(WOSS_DECIMAL_PRECISION);

  ::std::stringstream temp_buffer;
  temp_buffer.precision(WOSS_DECIMAL_PRECISION);

  double prev_depth = BELLHOP_NOT_SET;

  // if Bathymetry write method D - discrete jumps
  if (bathymetry_method.compare(WOSS_BATHYMETRY_MODE_DISCRETE) == 0) {
    if (debug) std::cout << "BellhopWoss(" << woss_id << ")::writeBathymetryFile(): bathymetry_method = DISCRETE" << std::endl;

    int total_same_values = 0;
    bool has_last_value_to_write = false;
    
    for (int i = 0; i < (int) coordz_vector.size(); i++) {
      double curr_depth = coordz_vector[i].getDepth();

      if ( curr_depth == prev_depth ) {
        total_same_values++;
        has_last_value_to_write = true;
        continue;
      }
      
      if (has_last_value_to_write) {
        total_same_values--;
        has_last_value_to_write = false;
        
        temp_buffer << ::std::setw(WOSS_STREAM_TAB_SPACE) << (range_vector[i-1]/1000.0) 
                    << ::std::setw(WOSS_STREAM_TAB_SPACE) 
                    << ::std::min( prev_depth, max_normalized_ssp_depth ) << ::std::endl;
      
        if (debug) 
          ::std::cout << "BellhopWoss(" << woss_id << ")::writeBathymetryFile() range = " << range_vector[i-1]
                      << "; depth = " << ::std::min( prev_depth, max_normalized_ssp_depth ) << ::std::endl;
      }
      prev_depth = curr_depth;

      temp_buffer << ::std::setw(WOSS_STREAM_TAB_SPACE) << (range_vector[i]/1000.0) << ::std::setw(WOSS_STREAM_TAB_SPACE) 
                  << ::std::min( curr_depth, max_normalized_ssp_depth ) << ::std::endl;

      if (debug) 
        ::std::cout << "BellhopWoss(" << woss_id << ")::writeBathymetryFile() range = " << range_vector[i]
                    << "; depth = " << ::std::min( curr_depth, max_normalized_ssp_depth ) << ::std::endl;

    }
      
    baty_out << "\'"<< bathymetry_type << "\'" << ::std::endl; 
    baty_out << range_vector.size() - total_same_values << ::std::endl;
  }
  else { // Bathymetry write method S - Slope
    if (debug) std::cout << "BellhopWoss(" << woss_id << ")::writeBathymetryFile(): bathymetry_method = SLOPE" << std::endl;

    int written_values = 0;

    for (int i = 0; i < (int) coordz_vector.size(); i++) {
      double curr_depth = coordz_vector[i].getDepth();

      if (curr_depth != prev_depth && i > 0) {
        written_values++;

        temp_buffer << ::std::setw(WOSS_STREAM_TAB_SPACE) << (range_vector[i]/1000.0) 
                    << ::std::setw(WOSS_STREAM_TAB_SPACE) 
                    << ::std::min( (prev_depth+curr_depth)/2.0, max_normalized_ssp_depth ) << ::std::endl;

        if (debug) 
          ::std::cout << "BellhopWoss(" << woss_id << ")::writeBathymetryFile() value change: i = " << i 
                      << "; range = " << range_vector[i]
                      << "; depth = " << ::std::min( (prev_depth+curr_depth)/2.0, max_normalized_ssp_depth ) << ::std::endl;

      }
      else if (i == 0 || i == ((int) coordz_vector.size())-1) { //On first and last point, use the actual value
        written_values++;

        temp_buffer << ::std::setw(WOSS_STREAM_TAB_SPACE) << (range_vector[i]/1000.0) 
                    << ::std::setw(WOSS_STREAM_TAB_SPACE)
                    << ::std::min( curr_depth, max_normalized_ssp_depth ) << ::std::endl;

        if (debug) 
          ::std::cout << "BellhopWoss(" << woss_id << ")::writeBathymetryFile() first/last: i = " << i 
                      << "; range = " << range_vector[i]
                      << "; depth = " << ::std::min( curr_depth, max_normalized_ssp_depth ) << ::std::endl;

      }

      prev_depth = curr_depth;

    }

    baty_out << "\'"<< bathymetry_type << "\'" << ::std::endl; 
    baty_out << written_values << ::std::endl;
  }
  
  baty_out << temp_buffer.str();
  baty_out.close();
}


void BellhopWoss::writeBeamPatternFile() {
  ::std::ofstream beam_out ( beam_pattern_file.c_str() );
  assert ( beam_out.is_open() );
  beam_out.precision(WOSS_DECIMAL_PRECISION);
  transducer->writeVertBeamPattern( beam_out, tx_coordz, rx_coordz, bp_initial_bearing, bp_vertical_rotation, bp_horizontal_rotation, bp_mult_costant, bp_add_costant );
  beam_out.close();
}


void BellhopWoss::writeAltimetryFile( int curr_run ) { 
  ::std::ofstream aty_out (altimetry_file.c_str());
  assert ( aty_out.is_open() );
  aty_out.precision(WOSS_DECIMAL_PRECISION);
  
  ::std::stringstream temp_buffer;
  temp_buffer.precision(WOSS_DECIMAL_PRECISION);
  
  double prev_depth = BELLHOP_NOT_SET;
  int total_same_values = 0;
  bool has_last_value_to_write = false;
  
  int range_counter = 0;
  Altimetry* curr_alt = NULL;
  bool has_to_delete = false;
  
  if ( curr_run > 0 ) {
    curr_alt = altimetry_value->randomize( 0.0001 );
    min_altimetry_depth = curr_alt->getMinAltimetryValue();
    max_altimetry_depth = curr_alt->getMaxAltimetryValue();
    has_to_delete = true;
  }
  else curr_alt = altimetry_value;
  
  if (debug) ::std::cout << "BellhopWoss(" << woss_id << ")::writeAltimetryFile() altimetry size " 
                         << altimetry_value->size() << ::std::endl;
  
//   debugWaitForUser();
  
  for (AltCIt it = curr_alt->begin(); it != curr_alt->end(); it++) {
    double curr_depth = it->second;
    
    if ( curr_depth == prev_depth ) {
      total_same_values++;
      range_counter++;
      has_last_value_to_write = true;
      continue;
    }
    
    if (has_last_value_to_write) {
      total_same_values--;
      has_last_value_to_write = false;
      
      temp_buffer << ::std::setw(WOSS_STREAM_TAB_SPACE) << (range_vector[range_counter-1]/1000.0) 
                  << ::std::setw(WOSS_STREAM_TAB_SPACE) << prev_depth << ::std::endl;

      if (debug) ::std::cout << "BellhopWoss(" << woss_id << ")::writeAltimetryFile() range = " 
                             << range_vector[range_counter-1]
                             << "; depth = " << prev_depth << ::std::endl;
    }                   
    prev_depth = curr_depth;

    temp_buffer << ::std::setw(WOSS_STREAM_TAB_SPACE) << (range_vector[range_counter]/1000.0) 
                << ::std::setw(WOSS_STREAM_TAB_SPACE) << curr_depth << ::std::endl;

//     ::std::cout.precision(WOSS_DECIMAL_PRECISION);
    if (debug) ::std::cout << "BellhopWoss(" << woss_id << ")::writeAltimetryFile() range = " 
                           << range_vector[range_counter]
                           << "; depth = " << curr_depth << ::std::endl;
    
    range_counter++;
  }
    
  aty_out << "\'"<< altimetry_type << "\'" << ::std::endl; 
  aty_out << curr_alt->size() - total_same_values << ::std::endl;
  aty_out << temp_buffer.str();
  aty_out.close();
  
  if ( has_to_delete )
    delete curr_alt;
}


bool BellhopWoss::initialize() {

  bool is_valid = ACToolboxWoss::initialize();

  assert( isValid() && is_valid );

  resetNormalizedDbSSP();
  normalizeDbSSP();

  checkDepthOffsets();

  checkRangeOffsets();

  checkAngles();
  
  initBox( ::std::min(max_bathymetry_depth, max_normalized_ssp_depth), ::std::max(total_great_circle_distance,total_distance) + rx_max_range_offset );
  
  writeAllCfgFiles();
  
  return ( is_valid && true );
}


bool BellhopWoss::initPressResReader( double curr_frequency ) {
  ResReader* ptr = new ShdResReader(this);
  RRMPair ret_pair = res_reader_map.insert( ::std::make_pair( curr_frequency, ptr ) );
  
  if ( ret_pair.second == false) {
    delete ptr;
  }
  
  if ( res_reader_map[curr_frequency] != NULL ) {
    res_reader_map[curr_frequency]->setFileName(shd_file);
    return true;
  }
  
  return false;
}


bool BellhopWoss::initTimeArrResReader( double curr_frequency ) {
  ResReader* ptr = NULL;
  
  if ( bellhop_op_mode == "A" ) {
    ptr = new ArrAscResReader( this ); 
  }
  else if ( bellhop_op_mode == "a" ) {
    ptr = new ArrBinResReader( this );
  }

  RRMPair ret_pair = res_reader_map.insert( ::std::make_pair( curr_frequency, ptr ) );

  if ( ret_pair.second == false) {
    delete ptr;
  }

  if ( res_reader_map[curr_frequency] != NULL ) {
    res_reader_map[curr_frequency]->setFileName(arr_file);
    return true;
  }
  
  return false;
}


bool BellhopWoss::run() { 
  is_running = true;
  
  assert((bellhop_arr_syntax != BELLHOP_CREATOR_ARR_FILE_INVALID) && (bellhop_shd_syntax != BELLHOP_CREATOR_SHD_FILE_INVALID));

  ::std::stringstream str_out;
  
  for ( FreqSIt it = frequencies.begin(); it != frequencies.end(); it++ ) {
    double curr_frequency = *it;
    
    for (int i = 0; i < total_runs; i++ ) {

      if (debug) ::std::cout << "BellhopWoss(" << woss_id << ")::run() frequency = " << curr_frequency << "; run = " << i << ::std::endl;
    
      initCfgFiles( curr_frequency, i );
      
      if (debug) str_out << "cd " << curr_path << " && " << bellhop_path << WOSS_BELLHOP_PROGRAM << " " << WOSS_BELLHOP_NAME << " > " << WOSS_BELLHOP_NAME << ".prt2";
      else str_out <<"cd " << curr_path << " && " << bellhop_path << WOSS_BELLHOP_PROGRAM << " " << WOSS_BELLHOP_NAME << " > " << "/dev/null";
      ::std::string command = str_out.str();
      str_out.str("");
      
      int ret_value = -1;
      if (system(NULL)) ret_value = system(command.c_str());
      if (ret_value != 0) {
        ::std::cerr << "BellhopWoss(" << woss_id << ")::run() error! bellhop.exe aborted!" << ::std::endl;

        is_running = false;
        return false;
      }
      bool is_ok = initResReader( curr_frequency ); 
      assert(is_ok);
    }
  }
  is_running = false;
  if (!has_run_once) has_run_once = true;
  return true;
}


void BellhopWoss::initCfgFiles( double curr_frequency, int curr_run ) {
  ::std::stringstream str_out;

  str_out << work_dir_path << "woss" << woss_id 
          << "/freq" << curr_frequency << "/time" << (time_t)current_time << "/run" << curr_run << "/";
          
  curr_path = str_out.str();
  str_out.str("");
  
  bellhop_env_file = curr_path + WOSS_BELLHOP_NAME + WOSS_BELLHOP_ENV;
  bathymetry_file = curr_path + WOSS_BELLHOP_NAME + WOSS_BELLHOP_BTY;
  altimetry_file = curr_path + WOSS_BELLHOP_NAME + WOSS_BELLHOP_ATI;
  beam_pattern_file = curr_path + WOSS_BELLHOP_NAME + WOSS_BELLHOP_SBP;
  
  ssp_file = curr_path + WOSS_BELLHOP_NAME + WOSS_BELLHOP_SSP;
  shd_file = curr_path + WOSS_BELLHOP_NAME + WOSS_BELLHOP_SHD;
  arr_file = curr_path + WOSS_BELLHOP_NAME + WOSS_BELLHOP_ARR;
}


void BellhopWoss::writeCfgFiles( double curr_frequency, int curr_run ) {
  
  bool is_ok = mkWorkDir( curr_frequency, curr_run );
  assert(is_ok);
  
  initCfgFiles( curr_frequency, curr_run );
  
  f_out.open(bellhop_env_file.c_str());
  assert( f_out.is_open() );
  f_out.precision(WOSS_DECIMAL_PRECISION);
  
  writeBathymetryFile();
  
//   ::std::cout << "BellhopWoss::writeCfgFiles() altimetry_value " << altimetry_value 
//               << "; altim is valid " << altimetry_value->isValid() << ::std::endl;
       
//   debugWaitForUser();
  
  if ( altimetry_value != NULL && altimetry_value->isValid() ) writeAltimetryFile( curr_run );
  
  if ( transducer != NULL && transducer->isValid() ) writeBeamPatternFile();
  
  writeHeader( curr_frequency, curr_run );
  
  if ( curr_run > 0 ) {
    resetNormalizedDbSSP();
    normalizeDbSSP();
  }

  writeNormalizedSSP( curr_run );
 
  writeSediment();

  writeTransmitter();
  
  writeReceiver();
  
  writeRayOptions();
  
  writeBox();
  
  f_out.close();
}


void BellhopWoss::writeAllCfgFiles() {
  for ( FreqSIt it = frequencies.begin(); it != frequencies.end(); it++ ) {   
    for (int i = 0; i < total_runs; i++ ) {
      writeCfgFiles( *it, i );
    }
  }
}


void BellhopWoss::removeAllCfgFiles() {
  for ( FreqSIt it = frequencies.begin(); it != frequencies.end(); it++ ) {   
    for (int i = 0; i < total_runs; i++ ) {
      removeCfgFiles( *it, i );
    }
  }
}


void BellhopWoss::removeCfgFiles( double curr_frequency, int curr_run ) {
    rmWorkDir( curr_frequency, curr_run );
}


bool BellhopWoss::initResReader( double curr_frequency )  {
  assert( !(using_press_mode == false && using_time_arrival_mode == false) );
  
  if ( using_press_mode ) return( initPressResReader(curr_frequency) && res_reader_map[curr_frequency]->initialize() );
  if ( using_time_arrival_mode ) return( initTimeArrResReader(curr_frequency) && res_reader_map[curr_frequency]->initialize() );
  return false;
}


Pressure* BellhopWoss::getAvgPressure( double frequency, double tx_depth, double start_rx_depth, double start_rx_range, 
                                       double end_rx_depth, double end_rx_range ) const {
  assert( using_press_mode == true && res_reader_map.size() > 0 );

  checkBoundaries( frequency, tx_depth, start_rx_depth, start_rx_range, end_rx_depth, end_rx_range );
  
  Pressure* ret_val = res_reader_map.find(frequency)->second->readAvgPressure( tx_depth, start_rx_depth, start_rx_range, end_rx_depth, end_rx_range );
  *ret_val /= (double) total_runs;
  return ( ret_val );
}


Pressure* BellhopWoss::getPressure( double frequency, double tx_depth, double rx_depth, double rx_range ) const {
  assert( res_reader_map.size() > 0 );

  checkBoundaries( frequency, tx_depth, rx_depth, rx_range, rx_depth, rx_range );
  Pressure* ret_val = res_reader_map.find(frequency)->second->readPressure( tx_depth, rx_depth, rx_range );
  *ret_val /= (double) total_runs;
  return ( ret_val );
}


TimeArr* BellhopWoss::getTimeArr( double frequency, double tx_depth, double rx_depth, double rx_range ) const {
  assert( res_reader_map.size() > 0 );

  checkBoundaries( frequency, tx_depth, rx_depth, rx_range, rx_depth, rx_range );
  TimeArr* ret_val = res_reader_map.find(frequency)->second->readTimeArr( tx_depth, rx_depth, rx_range );
  
   if ( debug ) ::std::cout << "BellhopWoss(" << woss_id << ")::getTimeArr() ret_val = " 
                            << *ret_val << ::std::endl;

  *ret_val /= (double) total_runs;

   if ( debug ) ::std::cout << "BellhopWoss(" << woss_id << ")::getTimeArr() total_runs = " 
                            << total_runs << "; ret_val/total_runs = " << *ret_val << ::std::endl;
   
//   debugWaitForUser();
  
  return ( ret_val );
}


bool BellhopWoss::timeEvolve( const Time& time_value ) {
  if ( evolution_time_quantum < 0.0 ) {
    if (!has_run_once) return true;
    else return false;
  }
  
  if ( !time_value.isValid() )
    return false;
  
  Time t_value = time_value;
  
  if ( t_value > end_time ) t_value = end_time;
  else if ( t_value < start_time ) t_value = start_time;
 
   if ( debug ) ::std::cout << "BellhopWoss(" << woss_id << ")::timeEvolve() current_time = "
                            << current_time << "; time_value = " 
                            << t_value << "; evolution_time_quantum = " << evolution_time_quantum << ::std::endl;
  
  if ( t_value == current_time ) {
    if ( !has_run_once )
      return true;
    else 
      return false;
  }
  
  double time_difference = ::std::abs(current_time - t_value);
  
  if ( debug ) ::std::cout << "BellhopWoss(" << woss_id << ")::timeEvolve() time_difference = " << time_difference << ::std::endl;
  
  if ( ( evolution_time_quantum == 0.0 ) || ( time_difference >= evolution_time_quantum ) ) {

    if ( debug ) ::std::cout << "BellhopWoss(" << woss_id << ")::timeEvolve() has to run" << ::std::endl; 
   
    current_time = t_value; 
    //removeAllCfgFiles();
    initialize();
    
    if ( altimetry_value != NULL && altimetry_value->isValid() ) {
      Altimetry* new_value = altimetry_value->timeEvolve( time_value );
      delete altimetry_value;
      altimetry_value = NULL;
      altimetry_value = new_value;
    }
    return true;
  } 
  
  if ( !has_run_once )
    return true;
  else 
    return false;
}

