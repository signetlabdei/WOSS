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
 * @file   woss-db-manager.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::WossDbManager class
 *
 * Provides the implementation of the woss::WossDbManager class
 */


#include <cassert>
#include <cstdlib>
#include <time-definitions.h>
#include <time-arrival-definitions.h>
#include <altimetry-definitions.h>
#include "woss-db.h"
#include <definitions-handler.h>
#include <singleton-definitions.h>
#include "woss-db-manager.h"


using namespace woss;
   
   
WossDbManager::WossDbManager() 
: bathymetry_db(NULL),
  sediment_db(NULL),
  ssp_db(NULL),
  results_arrivals_db(NULL),
  results_pressure_db(NULL),
  debug(false),
  ccbathy_map(),
  ccsediment_map(),
  ccssp_map(),
  ccaltimetry_map()
{  
  WossDbManager::updateDebugFlag();
}


WossDbManager::WossDbManager( WossDbManager& copy )
: bathymetry_db(copy.bathymetry_db),
  sediment_db(copy.sediment_db),
  ssp_db(copy.ssp_db),
  results_arrivals_db(copy.results_arrivals_db),
  results_pressure_db(copy.results_pressure_db),
  debug(copy.debug),
  ccbathy_map(copy.ccbathy_map),
  ccsediment_map(copy.ccsediment_map),
  ccssp_map(copy.ccssp_map),
  ccaltimetry_map(copy.ccaltimetry_map)
{    
  copy.bathymetry_db = NULL;
  copy.sediment_db = NULL;
  copy.ssp_db = NULL;
  copy.results_arrivals_db = NULL;
  copy.results_pressure_db = NULL;
}


WossDbManager& WossDbManager::operator=( WossDbManager& copy ) {
  if (this == &copy) return *this;
 
  bathymetry_db = copy.bathymetry_db;
  sediment_db = copy.sediment_db;
  ssp_db = copy.ssp_db;
  results_arrivals_db = copy.results_arrivals_db;
  results_pressure_db = copy.results_pressure_db;
  debug = copy.debug;
  ccbathy_map = copy.ccbathy_map;
  ccsediment_map = copy.ccsediment_map;
  ccssp_map = copy.ccssp_map;
  ccaltimetry_map = copy.ccaltimetry_map;
  
  copy.bathymetry_db = NULL;
  copy.sediment_db = NULL;
  copy.ssp_db = NULL;
  copy.results_arrivals_db = NULL;
  copy.results_pressure_db = NULL;
  
  return *this;
}


WossDbManager::~WossDbManager() { 
  closeAllConnections(); 
  
  delete bathymetry_db;
  delete sediment_db;
  delete ssp_db;
  delete results_arrivals_db;
  delete results_pressure_db;
  
  bathymetry_db = NULL;
  sediment_db = NULL;
  ssp_db = NULL;
  results_arrivals_db = NULL;
  results_pressure_db = NULL;
}


void WossDbManager::updateDebugFlag() {
  ccsediment_map.setDebug(debug);
  ccbathy_map.setDebug(debug);
  ccssp_map.setDebug(debug);
  ccaltimetry_map.setDebug(debug);
}


bool WossDbManager::closeAllConnections() {
  bool arrivals_ok = true;
  bool pressure_ok = true;
  bool bathy_ok = true;
  bool sedim_ok = true;
  bool ssp_ok = true;
  
  if ( bathymetry_db) bathy_ok = dynamic_cast< WossDb* >(bathymetry_db)->closeConnection();
  if ( sediment_db ) sedim_ok = dynamic_cast< WossDb* >(sediment_db)->closeConnection();
  if ( ssp_db ) ssp_ok = dynamic_cast< WossDb* >(ssp_db)->closeConnection();
  if(results_arrivals_db) arrivals_ok = dynamic_cast< WossDb* >(results_arrivals_db)->closeConnection();
  if(results_pressure_db) pressure_ok = dynamic_cast< WossDb* >(results_pressure_db)->closeConnection();
 
  return ( bathy_ok && sedim_ok && arrivals_ok && pressure_ok && ssp_ok );
}


Altimetry* WossDbManager::getAltimetry( const CoordZ& tx_coord, const CoordZ& rx_coord ) const {
  if( debug ) 
    ::std::cout << "WossDbManager::getAltimetry() tx_coord = " << tx_coord << "; rx_coord = " << rx_coord 
                << "; ccaltimetry_map.size() = " << ccaltimetry_map.size() << ::std::endl;
  
  Altimetry* ptr = ccaltimetry_map.get( tx_coord, rx_coord );
  
  if( debug && ptr != NULL && ptr->isValid() ) 
    ::std::cout << "WossDbManager::getAltimetry() altimetry found " << *ptr << ::std::endl;

  if ( ptr != NULL ) return ptr;
  
  return SDefHandler::instance()->getAltimetry()->create( Altimetry::createNotValid() );  
}


Sediment* WossDbManager::getSediment( const CoordZ& tx_coord, const CoordZ& rx_coord ) const {

  if(debug) 
    ::std::cout << "WossDbManager::getSediment() tx_coord = " << tx_coord << "; rx_coord = " << rx_coord 
                << "; ccsediment_map.size() = " << ccsediment_map.size() << ::std::endl;
  
  if (ccsediment_map.empty() == false) {
    Sediment* ptr = ccsediment_map.get( tx_coord, rx_coord );

    if(debug && ptr != NULL) 
      ::std::cout << "WossDbManager::getSediment() sed found = " << ptr << "; " << *ptr << ::std::endl;

    if ( ptr != NULL ) return ptr;
  }

  if ( !sediment_db ) {
    ::std::cerr << "WossDbManager::getSediment() WARNING, no database nor custom Sediment was found for tx = " << tx_coord 
                << "; rx_coord = " << rx_coord << ::std::endl;
                
    return SDefHandler::instance()->getSediment()->create();
  }

  return sediment_db->getValue( rx_coord ); 
}


Sediment* WossDbManager::getSediment( const CoordZ& tx_coord, const CoordZVector& rx_coordz_vector ) const { 
  Sediment* ptr = NULL;
  
  ::std::map< ::std::string, ::std::vector<Sediment*> > sedim_map;
  
  if ( ccsediment_map.empty() ) 
    goto db_sediment;

  for ( int i = 0; i < (int) rx_coordz_vector.size(); i++ ) {
    ptr = getSediment( tx_coord, rx_coordz_vector[i] );
    if ( ptr != NULL ) { 
      sedim_map[ptr->getType()].push_back(ptr);
    }
  }
  
  if ( !sedim_map.empty() ) {
    if (debug) 
      ::std::cout << "WossDbManager::getSediment() sedim_map.size() = " << sedim_map.size() << ::std::endl;
    
    int max_size = -1;
    ptr = NULL;
    
    for (  ::std::map< ::std::string, ::std::vector<Sediment*> >::iterator it = sedim_map.begin(); it != sedim_map.end(); it++ ) {
      if (debug) 
        ::std::cout << "WossDbManager::getSediment() it->second.size() = " << it->second.size() 
                    <<"; max = " << max_size << ::std::endl;

      if ( (int)it->second.size() > max_size ) {
        max_size = it->second.size();
        ptr = it->second.back();
        it->second.pop_back();
        
        if (debug) 
          ::std::cout << "WossDbManager::getSediment() it->second.size() = " << it->second.size() 
                      << " > max = " << max_size << "; ptr = " << ptr << "; sed = " << *ptr << ::std::endl;
      }   
      for ( int i = 0; i < (int) it->second.size(); i++ ) {
        delete (it->second)[i];
        it->second[i] = NULL;
      }

      if (debug) 
        ::std::cout << "WossDbManager::getSediment() after delete it->second.size() = " << it->second.size()   
                    << ::std::endl;
    }
    sedim_map.clear();

    return ptr;
  } 
  else goto db_sediment;
  
  db_sediment:
  
  if ( !sediment_db ) {
    ::std::cerr << "WossDbManager::getSediment() WARNING, no database nor custom Sediment was found for tx = " << tx_coord 
                << "; CoordZVector size = " << rx_coordz_vector.size() << ::std::endl;
                
    return SDefHandler::instance()->getSediment()->create();
  }
  return( sediment_db->getValue( rx_coordz_vector ) );
}


Bathymetry WossDbManager::getBathymetry( const Coord& tx_coord, const Coord& rx_coord ) const {
  
  if(debug) 
    ::std::cout << "WossDbManager::getBathymetry() tx_coord = " << tx_coord << "; rx_coord = " << rx_coord 
                << "; ccbathy_map.size() = " << ccbathy_map.size() << ::std::endl;
  
  if ( !ccbathy_map.empty() ) {
      const Bathymetry* ptr = ccbathy_map.get( tx_coord, rx_coord );

      if ( ptr != NULL ) return *ptr;
  }
 
  if( !bathymetry_db ) {
    ::std::cerr << "WossDbManager::getBathymetry() WARNING, no database nor custom Bathymetry was found for tx = " << tx_coord 
                << "; rx_coord = " << rx_coord << ::std::endl;

    return HUGE_VAL;
  }

  return bathymetry_db->getValue( rx_coord ); 
}


void WossDbManager::getBathymetry( const Coord& tx_coord, CoordZVector& rx_coordz_vector ) const {
  for (int i = 0; i < (int) rx_coordz_vector.size(); i++) {
      rx_coordz_vector[i].setDepth( getBathymetry( tx_coord, rx_coordz_vector[i] ) ) ;
  }
}


SSP* WossDbManager::getSSP( const Coord& tx_coord, const Coord& rx_coord, const Time& time, long double ssp_depth_precision ) const {
  if ( !ccssp_map.empty() ) {
    SSP* ptr = ccssp_map.get( tx_coord, rx_coord, time );
    if ( ptr != NULL ) return ptr;
  }
  if ( !ssp_db ) {
    ::std::cerr << "WossDbManager::getSSP() WARNING, no database nor custom SSP was found for tx = " << tx_coord 
                << "; rx_coord = " << rx_coord << "; time = " << time;
                
    return SDefHandler::instance()->getSSP()->create();
  }
  return( ssp_db->getValue( rx_coord, time, ssp_depth_precision ) );
}


SSP* WossDbManager::getAverageSSP( const Coord& tx_coord, const Coord& rx_coord, const Time& time_start, const Time& time_end, 
				  int max_time_values, long double ssp_depth_precision ) const {

  if ( !ccssp_map.empty() ) {
    SSP* ptr = ccssp_map.get( tx_coord, rx_coord );
    if ( ptr != NULL ) return ptr;
  }
  if ( !ssp_db ) {
    ::std::cerr << "WossDbManager::getAverageSSP() WARNING, no database nor custom SSP was found for tx = " << tx_coord 
                << "; rx_coord = " << rx_coord << "; start time = " << time_start << "; end time = " << time_end;
                
    return SDefHandler::instance()->getSSP()->create();
  }
  assert( time_end > time_start );
  
  SSP* ret_value = SDefHandler::instance()->getSSP()->create( ssp_depth_precision );
  
  double time_quantum = ( time_end - time_start ) / (double) max_time_values;
  Time curr_time = time_start;
  
  SSP* curr_ssp = NULL;
  for ( int i = 0; i < max_time_values; i++ ) {
    curr_ssp = ssp_db->getValue( rx_coord, curr_time, ssp_depth_precision );
    *ret_value += *curr_ssp;
    curr_time += time_quantum;
    delete curr_ssp;
    curr_ssp = NULL;
  }
  *ret_value /= max_time_values;
  return ret_value;
}


TimeArr* WossDbManager::getTimeArr( const CoordZ& tx_coord, const CoordZ& rx_coord, const double frequency, const Time& time_value ) const {
  if ( !results_arrivals_db ) return( SDefHandler::instance()->getTimeArr()->create( TimeArr::createNotValid() ) ); 
  return( results_arrivals_db->getValue( tx_coord, rx_coord, frequency, time_value ) );
}


void WossDbManager::insertTimeArr( const CoordZ& tx_coord, const CoordZ& rx_coord, const double frequency, const Time& time_value, const TimeArr& channel ) const {
  if ( results_arrivals_db ) results_arrivals_db->insertValue( tx_coord, rx_coord, frequency, time_value, channel );
}


Pressure* WossDbManager::getPressure( const CoordZ& tx_coord, const CoordZ& rx_coord, const double frequency, const Time& time_value ) const {
  if ( !results_pressure_db ) return( SDefHandler::instance()->getPressure()->create( Pressure::createNotValid() ) );
  return( results_pressure_db->getValue( tx_coord, rx_coord, frequency, time_value ) );
}


void WossDbManager::insertPressure( const CoordZ& tx_coord, const CoordZ& rx_coord, const double frequency, const Time& time_value, const Pressure& pressure ) const {
  if ( results_pressure_db ) results_pressure_db->insertValue( tx_coord, rx_coord, frequency, time_value, pressure );
}


bool WossDbManager::setCustomSSP(const ::std::string &sspString, const Coord& txCoord, double bearing,
                          double range, const Time& timeValue)
{
  SSP* ssp = SDefHandler::instance()->getSSP()->create();
  ::std::string sspTmp = sspString;
  ::std::string::size_type tmp;

  int totalDepths = 0;

  tmp = sspTmp.find ("|");
  if (tmp == std::string::npos) {
    ::std::cerr << "WossDbManager::setCustomSSP() separator | not found, string parsed = " << sspTmp << ::std::endl;
    return false;
  }

  std::string totDepths = sspTmp.substr (0, tmp);
  sspTmp = sspTmp.substr (tmp + 1, sspTmp.npos);
  totalDepths = ::std::atoi (totDepths.c_str ());

  if(debug) {
    ::std::cout << "WossDbManager::setCustomSSP() totalDepths = " << totalDepths << ::std::endl;
  }

  if (totalDepths <= 0) {
    ::std::cerr << "WossDbManager::setCustomSSP() totalDepths < 0 " << totalDepths << ::std::endl;
    return false;
  }

  for (int cnt = 0; cnt < totalDepths; ++cnt) {
    double depth;
    double sspValue;

    tmp = sspTmp.find ("|");
    if (tmp == std::string::npos) {
    ::std::cerr << "WossDbManager::setCustomSSP() cnt = " << cnt 
                << "; separator | not found, string parsed = " << sspTmp << ::std::endl;
      return false;
    }

    std::string depthStr = sspTmp.substr (0, tmp);
    sspTmp = sspTmp.substr (tmp + 1, sspTmp.npos);
    depth = ::std::atof (depthStr.c_str ());

    tmp = sspTmp.find ("|");
    if ((tmp == std::string::npos) && (cnt != totalDepths - 1)) {
    ::std::cerr << "WossDbManager::setCustomSSP() cnt = " << cnt 
                << "; separator | not found, string parsed = " << sspTmp << ::std::endl;
      return false;
    }

    std::string sspValueStr = sspTmp.substr (0, tmp);
    sspTmp = sspTmp.substr (tmp + 1, sspTmp.npos);
    sspValue = ::std::atof (sspValueStr.c_str ());

    if(debug) {
      ::std::cout << "WossDbManager::setCustomSSP() cnt = " << cnt 
                  << "; depth = " << depth << "; sspValue = " << sspValue << ::std::endl;
    }

    if (sspValue < 0.0) {
    ::std::cerr << "WossDbManager::setCustomSSP() cnt = " << cnt 
                << "; depth = " << depth << "< 0 " << ::std::endl;
      return false;
    }
    else {
      ssp->insertValue(depth, sspValue);
    }
  }

  return setCustomSSP(ssp, txCoord, bearing, range, timeValue);
}


bool WossDbManager::importCustomSSP( const ::std::string& filename, const Time& time, const Coord& tx_coord, double bearing ) {
  ::std::ifstream ssp_in ( filename.c_str() );
  if ( ! ssp_in.is_open() ) {
    ::std::cerr << "WossDbManager::importCustomSSP() WARNING, no filename = " << filename << " was found for tx = " 
                << tx_coord << "; bearing = " << bearing << ::std::endl;
                
    return false;
  }
  ::std::string ssp_type;
  double range;
  double curr_depth;
  double curr_temp;
  double curr_press;
  double curr_sal;
  double curr_ssp;
  double curr_latitude;
  double curr_longitude;
  SSP* curr_ptr = NULL;
  
  ssp_in >> ssp_type;  
  ssp_in >> curr_latitude;
  ssp_in >> curr_longitude;
  
  if (debug) 
    ::std::cout << "WossDbManager::importCustomSSP() ssp type = " << ssp_type 
                << "; latitude = " << curr_latitude <<"; longitude = " << curr_longitude << ::std::endl;

  if ( ssp_type == "SSP" ) {
    double last_range = -HUGE_VAL;
    
    while( ssp_in.good() ) {
      ssp_in >> range;
      if ( range <= 0.0 ) range = CCSSP::DB_CDATA_ALL_INNER_KEYS;
      
      if ( last_range != range ) {
        last_range = range;
        curr_ptr = SDefHandler::instance()->getSSP()->create();
        ccssp_map.insert( curr_ptr, tx_coord, bearing, range, time ); 
      }
      
      ssp_in >> curr_depth;
      ssp_in >> curr_ssp;
      
      if (debug) 
        ::std::cout << "WossDbManager::importCustomSSP() range = " << range << "; depth = " << curr_depth << "; ssp value = " << curr_ssp << ::std::endl;
        
      curr_ptr->insertValue( curr_depth, curr_ssp );
    }
  }
  else if ( ssp_type == "FULL" ) {
    double last_range = -HUGE_VAL;
    
    while( ssp_in.good() ) {
      ssp_in >> range;
      if ( range <= 0.0 ) range = CCSSP::DB_CDATA_ALL_INNER_KEYS;

      if ( last_range != range ) {
        curr_ptr = SDefHandler::instance()->getSSP()->create();
        last_range = range;
        ccssp_map.insert( curr_ptr, tx_coord, bearing, range, time );
      }

      ssp_in >> curr_depth;
      ssp_in >> curr_temp;
      ssp_in >> curr_sal;
      ssp_in >> curr_press;
      ssp_in >> curr_ssp;
      
      if (debug) 
        ::std::cout << "WossDbManager::importCustomSSP() range = " << range << "; depth = " << curr_depth << "; temperature = " << curr_temp
                    << "; salinity = " << curr_sal << "; pressure = " << curr_press << "; ssp value = " << curr_ssp << ::std::endl;

      curr_ptr->insertValue( curr_depth, curr_temp, curr_sal, ::std::complex<double>(curr_press), curr_ssp );
    }
  } 
  else if ( ssp_type == "TEMPERATURE_SALINITY_PRESSURE" ) { 
    double last_range = -HUGE_VAL;
    
    while( ssp_in.good() ) {
      ssp_in >> range;
      if ( range <= 0.0 ) range = CCSSP::DB_CDATA_ALL_INNER_KEYS;

      if ( last_range != range ) {
        curr_ptr = SDefHandler::instance()->getSSP()->create();
        last_range = range;
        ccssp_map.insert( curr_ptr, tx_coord, bearing, range, time );
      }

      ssp_in >> curr_temp;
      ssp_in >> curr_sal;
      ssp_in >> curr_press;
      
      if (debug) 
        ::std::cout << "WossDbManager::importCustomSSP() range = " << range << "; temperature = " << curr_temp << "; salinity = " << curr_sal
                    << "; pressure = " << curr_press << ::std::endl;
      
      curr_ptr->insertValue( curr_temp, curr_sal, ::std::complex<double>(curr_press), Coord(curr_latitude, curr_longitude) );
    }
  }
  else if ( ssp_type == "DEPTH_TEMPERATURE_SALINITY" ) {
    double last_range = -HUGE_VAL;
    
    while( ssp_in.good() ) {   
      ssp_in >> range;
      if ( range <= 0.0 ) range = CCSSP::DB_CDATA_ALL_INNER_KEYS;

      if ( last_range != range ) {
        curr_ptr = SDefHandler::instance()->getSSP()->create();
        last_range = range;
        ccssp_map.insert( curr_ptr, tx_coord, bearing, range, time );
      } 

      ssp_in >> curr_depth;
      ssp_in >> curr_temp;
      ssp_in >> curr_sal;
      
      if (debug) 
        ::std::cout << "WossDbManager::importCustomSSP() range = " << range << "; depth = " << curr_depth << "; temperature = " << curr_temp 
                    << "; salinity = " << curr_sal << ::std::endl;
      
      curr_ptr->insertValue( curr_depth, curr_temp, curr_sal, Coord(curr_latitude, curr_longitude) );
    }
  }
  return true;
}

bool
WossDbManager::setCustomBathymetry(const ::std::string &bathyLine, const Coord& txCoord, double bearing)
{
  ::std::string bathyTmp = bathyLine;
  ::std::string::size_type tmp;

  int totalRanges = 0;

  if (debug) {
    ::std::cout << "WossDbManager::setCustomBathymetry() txCoord = " << txCoord 
                << "; bearing = " << bearing << ::std::endl;
  }

  tmp = bathyTmp.find ("|");
  if (tmp == std::string::npos) {
    ::std::cerr << "WossDbManager::setCustomBathymetry() " 
                << "separator | not found, string parsed = " << bathyTmp << ::std::endl;
    return false;
  }

  std::string totRanges = bathyTmp.substr (0, tmp);
  bathyTmp = bathyTmp.substr (tmp + 1, bathyTmp.npos);
  totalRanges = ::std::atoi (totRanges.c_str ());

  if (debug) {
    ::std::cout << "WossDbManager::setCustomBathymetry() " 
                << "totalRanges = " << totalRanges << ::std::endl;
  }

  if (totalRanges <= 0) {
    return false;
  }

  for (int cnt = 0; cnt < totalRanges; ++cnt) {
    double range;
    double depth;

    tmp = bathyTmp.find ("|");
    if (tmp == std::string::npos) {
      ::std::cerr << "WossDbManager::setCustomBathymetry() " 
                  << "cnt = " << cnt 
                  << "; separator | not found, string parsed:" << bathyTmp << ::std::endl;

      return false;
    }

    std::string rangeStr = bathyTmp.substr (0, tmp);
    bathyTmp = bathyTmp.substr (tmp + 1, bathyTmp.npos);
    range = ::std::atof (rangeStr.c_str ());

    tmp = bathyTmp.find ("|");
    if ((tmp == std::string::npos) && (cnt != totalRanges - 1)) {
      ::std::cerr << "WossDbManager::setCustomBathymetry() " 
                  << "cnt = " << cnt 
                  << "; separator | not found, string parsed:" << bathyTmp << ::std::endl;
      return false;
    }

    std::string depthStr = bathyTmp.substr (0, tmp);
    bathyTmp = bathyTmp.substr (tmp + 1, bathyTmp.npos);
    depth = ::std::atof (depthStr.c_str ());

    if (debug) {
      ::std::cout << "WossDbManager::setCustomBathymetry() " 
                  << " cnt = " << cnt << "; range = " << range 
                  << "; depth = " << depth << ::std::endl;
    }

    if (depth < 0.0) {
        return false;
    }
    else {
        setCustomBathymetry(&depth, txCoord, bearing, range);
    }
  }
  return true;
}

bool WossDbManager::importCustomBathymetry( const ::std::string& filename, const Coord& tx_coord, double bearing ) {
  ::std::ifstream bathy_in ( filename.c_str() );
  if ( ! bathy_in.is_open() ) {
    ::std::cerr << "WossDbManager::importCustomBathymetry() WARNING, no filename = " << filename << " was found for tx = " 
                << tx_coord << "; bearing = " << bearing << ::std::endl;
                    
    return false;
  }
  double curr_range;
  double curr_bathy;
  
  while( bathy_in.good() ) {
    bathy_in >> curr_range;
    bathy_in >> curr_bathy;
    
    if (debug) 
      ::std::cout << "WossDbManager::importCustomBathymetry() range = " << curr_range << "; bathymetry = " << curr_bathy << ::std::endl;
    
    ccbathy_map[tx_coord][bearing][curr_range] = curr_bathy;
  }
  return true;
}

bool WossDbManager::setCustomSediment(const ::std::string &sedimentString, const Coord& txCoord, double bearing, double range) {

  ::std::string sedimentType;
  ::std::string sedimTmp = sedimentString;
  ::std::string::size_type tmp;
  double param[5];

  tmp = sedimTmp.find ("|");
  if (tmp == std::string::npos) {
    ::std::cout << "WossDbManager::setCustomSediment() " 
                << "separator | not found, string parsed = " << sedimTmp << ::std::endl;
    return false;
  }

  sedimentType = sedimTmp.substr (0, tmp);
  sedimTmp = sedimTmp.substr (tmp + 1, sedimTmp.npos);

  if (debug) {
    ::std::cout << "WossDbManager::setCustomSediment() " 
                << "sedimentType = " << sedimentType << ::std::endl;
  }

  for (int cnt = 0; cnt < 5; ++cnt) {
    tmp = sedimTmp.find ("|");
    if ((tmp == std::string::npos) && (cnt != 5 - 1)) {
      ::std::cerr << "WossDbManager::setCustomSediment() " 
                  << "cnt = " << cnt << "; separator | not found, string parsed = " 
                  << sedimTmp << ::std::endl;

      return false;
    }

    std::string paramStr = sedimTmp.substr (0, tmp);
    sedimTmp = sedimTmp.substr (tmp + 1, sedimTmp.npos);
    param[cnt] = ::std::atof (paramStr.c_str ());

    if (debug) {
      ::std::cout << "WossDbManager::setCustomSediment() " 
                  << "cnt = " << cnt << "; param = " << param[cnt] << ::std::endl;
    }
  }

  Sediment* sediment = SDefHandler::instance()->getSediment()->create(sedimentType, param[0], param[1], param[2], param[3], param[4]);

  return setCustomSediment(sediment, txCoord, bearing, range);
}
