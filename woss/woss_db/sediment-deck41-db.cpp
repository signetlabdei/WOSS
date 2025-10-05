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
 * @file   sediment-deck41-db.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::SedimDeck41Db class
 *
 * Provides the implementation of the woss::SedimDeck41Db class
 */

#ifdef WOSS_NETCDF_SUPPORT

#include <cassert> 
#include <cstdlib>
#include <definitions-handler.h>
#include "sediment-deck41-db.h" 


using namespace woss;


SedimWeightMap SedimDeck41Db::sediment_weight_map = SedimDeck41Db::initSedimWeightMap();
    
    
SedimDeck41Db::SedimDeck41Db( const std::string& name )
: WossDb(name),
  sediment_coord_db(),
  sediment_marsden_db(),
  sediment_marsden_one_db(),
  curr_tests_state(),
  prev_tests_state()
{

}


bool SedimDeck41Db::closeConnection() {
  return ( sediment_coord_db.closeConnection() && sediment_marsden_db.closeConnection() && 
           sediment_marsden_one_db.closeConnection() );
}


bool SedimDeck41Db::insertValue( const Coord& coordinates, const Sediment& sediment_value ) {
  std::cerr << "SedimDeck41Db::insertValue() method is not implemented." << std::endl;
  return false;
}


SedimWeightMap SedimDeck41Db::initSedimWeightMap() {
  SedimWeightMap map;

  map['E'] = 0.65;
  map['F'] = 0.4;
 
  return(map);
}


std::unique_ptr<Sediment> SedimDeck41Db::createSediment( int deck41_type, double depth ) const {
  switch(deck41_type) {
    case(DECK41_FLOORTYPE_GRAVEL): {
      return std::make_unique<SedimentGravel>(depth);
    }
    break;

    case(DECK41_FLOORTYPE_SAND): {
      return std::make_unique<SedimentSand>();
    }
    break;

    case(DECK41_FLOORTYPE_SILT): {
      return std::make_unique<SedimentSilt>(depth);
    }
    break;

    case(DECK41_FLOORTYPE_CLAY): {
      return std::make_unique<SedimentClay>();
    }
    break;

    case(DECK41_FLOORTYPE_OOZE): {
      return std::make_unique<SedimentOoze>();
    }
    break;

    case(DECK41_FLOORTYPE_MUD): {
      return std::make_unique<SedimentMud>();
    }
    break;

    case(DECK41_FLOORTYPE_ROCKS): {
      return std::make_unique<SedimentRocks>();
    }
    break;

    case(DECK41_FLOORTYPE_ORGANIC): {
      return std::make_unique<SedimentOrganic>();
    }
    break;

    case(DECK41_FLOORTYPE_NODULES): {
      return std::make_unique<SedimentNodules>();
    }
    break;

    case(DECK41_FLOORTYPE_HARDBOTTOM): {
      return std::make_unique<SedimentHardBottom>();
    }
    break;

    default: {
      std::cerr << "SedimDeck41Db::createSediment() ERROR, Deck41Type not found or not valid!" << std::endl;
      return SDefHandler::instance().createSediment();
    }
    
  }
  
}


double SedimDeck41Db::calculateAvgDepth( const CoordZVector& coordz_vector ) const {
  assert ( coordz_vector.size() != 0);

  double ret_value = 0.0;

  for ( const auto& coord : coordz_vector ) {
    ret_value += coord.getDepth();
  }
 
  return ( ret_value / coordz_vector.size() ); 
}  


int SedimDeck41Db::getMaxAppereanceFrequencyValue( const FrequencyMap& frequency_map ) const {
  int max = 0;
  int max_type = DECK41_FLOORTYPE_NODATA;

  for ( const auto& it : frequency_map ) {
     if (it.first == DECK41_FLOORTYPE_NODATA) 
      continue;
     if ( (it.second) > max) {
        max = it.second;
        max_type = it.first;
     }
  }

  return max_type;
}


Deck41Types SedimDeck41Db::getDeck41TypesFromCoords( const CoordZVector& coordz_vector ) const {
  FrequencyMap main_map; // main_type, appereance times
  FrequencyMap secondary_map; //secondary_type, appereance times

  for ( const auto& coord : coordz_vector ) {
     Deck41Types curr_type = sediment_coord_db.getSeaFloorType(coord);

     main_map[curr_type.first]++;
     secondary_map[curr_type.second]++;
  }

  int max_main_type = getMaxAppereanceFrequencyValue( main_map );
  int max_secondary_type = getMaxAppereanceFrequencyValue ( secondary_map );

  if (debug) 
    std::cout << "SedimDeck41Db::getDeck41Types(CoordVector& ) max main type = " 
              <<  max_main_type << "; secondary type = " << max_secondary_type << std::endl;

  return(std::make_pair(max_main_type, max_secondary_type));
}


Deck41Types SedimDeck41Db::getDeck41TypesFromMarsdenCoords( const CoordZVector& coordz_vector ) const {
  FrequencyMap main_map; // main_type, appereance times
  FrequencyMap secondary_map; //secondary_type, appereance times

  for ( const auto& coord : coordz_vector ) {
    Deck41Types curr_type = sediment_marsden_one_db.getSeaFloorType ( coord.getMarsdenCoord() );

    main_map[curr_type.first]++;
    secondary_map[curr_type.second]++;
  }

  int max_main_type = getMaxAppereanceFrequencyValue( main_map );
  int max_secondary_type = getMaxAppereanceFrequencyValue ( secondary_map );

  if (debug) 
    std::cout << "SedimDeck41Db::getDeck41Types(MarsdenCoordVector& ) max main type = " 
              <<  max_main_type << "; secondary type = " << max_secondary_type << std::endl;

  return(std::make_pair(max_main_type, max_secondary_type));
}


Deck41Types SedimDeck41Db::getDeck41TypesFromMarsdenSquare( const CoordZVector& coordz_vector ) const {
  FrequencyMap main_map; // main_type, appereance times
  FrequencyMap secondary_map; //secondary_type, appereance times

  for ( const auto& coord : coordz_vector ) {
    Deck41Types curr_type = sediment_marsden_db.getSeaFloorType(coord.getMarsdenSquare() );

    main_map[curr_type.first]++;
    secondary_map[curr_type.second]++;
  }

  int max_main_type = getMaxAppereanceFrequencyValue( main_map );
  int max_secondary_type = getMaxAppereanceFrequencyValue ( secondary_map );

  if (debug) 
    std::cout << "SedimDeck41Db::getDeck41Types(MarsdenVector& ) max main type = " 
              <<  max_main_type << "; secondary type = " << max_secondary_type << std::endl;

  return(std::make_pair(max_main_type, max_secondary_type));
}


Deck41Types SedimDeck41Db::calculateDeck41Types( const CoordZVector& coordz_vector ) const {
  Deck41Types floor_types = getDeck41TypesFromCoords(coordz_vector);

  if (debug) 
    std::cout << "SedimDeck41Db::calculateDeck41Types() coord main type = " << floor_types.first << "; second type = "
              << floor_types.second << std::endl;

  curr_tests_state.updateAllConditions(floor_types);

  if ( doTestA(curr_tests_state) )
    return floor_types;
  else if ( doTestB(curr_tests_state) ) {
    floor_types = getDeck41TypesFromMarsdenCoords(coordz_vector);  

    if (debug) 
      std::cout << "SedimDeck41Db::calculateDeck41Types() marsden one main type = " << floor_types.first << "; second type = " 
                << floor_types.second << std::endl;

    prev_tests_state = curr_tests_state;
    curr_tests_state.updateAllConditions(floor_types);

    if ( doTestA(curr_tests_state) ) 
      return floor_types;
    else if ( doTestB(curr_tests_state) ) { 
      Deck41Types prev_types = floor_types;
      floor_types = getDeck41TypesFromMarsdenSquare( coordz_vector );

      if (debug) 
        std::cout << "SedimDeck41Db::calculateDeck41Types() marsden square main type = " << floor_types.first << "; second type = " 
                  << floor_types.second << std::endl;

      prev_tests_state = curr_tests_state;
      curr_tests_state.updateAllConditions(floor_types);

      if ( doTestA(curr_tests_state) ) 
        return floor_types;
      else if ( doTestC(curr_tests_state) ) { 
        if (debug) 
          std::cout << "SedimDeck41Db::calculateDeck41Types() Failed all 3 steps,"
                    << " reverting to the last one" << std::endl;

        floor_types = prev_types;
        prev_tests_state = curr_tests_state;
        
        return floor_types;
      }
      else {
        std::cerr << "SedimDeck41Db::calculateDeck41Types() ERROR, can't find a sea floor type!" 
                  << std::endl << "Please provide a custom Sediment for this transmitter: lat = " 
                  << coordz_vector.front().getLatitude() << "; long = " << coordz_vector.front().getLongitude() << std::endl;

        return( Deck41Types( DECK41_FLOORTYPE_NODATA, DECK41_FLOORTYPE_NODATA ) );
      }
    }
  }
  
  std::cerr << "SedimDeck41Db::calculateDeck41Types() ERROR, unexpected test failed!" 
            << std::endl << "Please provide a custom Sediment for this transmitter: lat = " 
            << coordz_vector.front().getLatitude() << "; long = " << coordz_vector.front().getLongitude() << std::endl;

  return( Deck41Types( DECK41_FLOORTYPE_NODATA, DECK41_FLOORTYPE_NODATA ) );
}


std::unique_ptr<Sediment> SedimDeck41Db::calculateSediment( const Deck41Types& floor_types, double avg_depth ) const {
  if ( floor_types.first == DECK41_FLOORTYPE_NODATA && floor_types.second == DECK41_FLOORTYPE_NODATA ) {
    std::cerr << "SedimDeck41Db::calculateSediment() no valid floor types found, returning a Sediment not valid" << std::endl;

    return SDefHandler::instance().createSediment();
  }
  if ( ( curr_tests_state.getConditionA() == true ) || ( curr_tests_state.getConditionC() == true ) )  {
    if (debug) 
      std::cout << "SedimDeck41Db::calculateSediment() condition A or C called" << std::endl;

    return( createSediment( floor_types.first, avg_depth ) );
  }
  if ( ( curr_tests_state.getConditionB() == true ) || ( curr_tests_state.getConditionD() == true ) ) {
    if (debug) 
      std::cout << "SedimDeck41Db::calculateSediment() condition B or D called" << std::endl;

    return( createSediment( floor_types.second, avg_depth ) );
  }
  if ( curr_tests_state.getConditionE() == true ) {
    if (debug) 
      std::cout << "SedimDeck41Db::calculateSediment() condition E called" << std::endl;

    auto first = createSediment(floor_types.first, avg_depth);
    auto second = createSediment(floor_types.second, avg_depth);
    
    *first *= sediment_weight_map['E'];
    *second *= 1.0 - sediment_weight_map['E'];
    *first += *second;

    return first;
  }
  if ( curr_tests_state.getConditionF() == true ) {
    if (debug) 
      std::cout << "SedimDeck41Db::calculateSediment() condition F called" << std::endl;

    auto first = createSediment( floor_types.first, avg_depth );
    auto second = createSediment( floor_types.second, avg_depth );
    
    *first *= sediment_weight_map['F'];
    *second *= 1.0 - sediment_weight_map['F'];    
    *first += *second;
    
    return first;
  }

  std::cerr << "SedimDeck41Db::calculateSediment(), unknown state condition occured, returning a Sediment not valid" << std::endl;

  return SDefHandler::instance().createSediment();
}


std::unique_ptr<Sediment> SedimDeck41Db::getValue( const CoordZVector& coordz_vector ) const {
  return( calculateSediment( calculateDeck41Types( coordz_vector ) , calculateAvgDepth( coordz_vector ) ) );
}


std::unique_ptr<Sediment> SedimDeck41Db::getValue( const CoordZ& coordz ) const {
  CoordZVector coordz_vector;
  coordz_vector.emplace_back(coordz);
  return( getValue(coordz_vector) );
}

#endif // WOSS_NETCDF_SUPPORT
