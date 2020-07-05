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
 * @file   ssp-woa2005-db.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::SspWoa2005Db class
 *
 * Provides the implementation of the woss::SspWoa2005Db class
 */

#ifdef WOSS_NETCDF_SUPPORT

#include <cmath>
#include <stdlib.h>
#include <definitions-handler.h>
#include "ssp-woa2005-db.h"


using namespace woss;

#define SSP_NOT_VALID (-1000)


SspWoa2005Db::SspWoa2005Db( const ::std::string& name ) 
: WossNetcdfDb(name),
  woa_db_type(WOA_DB_TYPE_2005),
#if defined (WOSS_NETCDF4_SUPPORT)
  ssp_var(),
  lat_var(),
  lon_var()
#else
  ssp_var(NULL)
#endif // defined (WOSS_NETCDF4_SUPPORT)
{ 
}

#if defined (WOSS_NETCDF4_SUPPORT)
SspWoa2005Db::SspWoa2005Db( const ::std::string& name, WOADbType db_type )
: WossNetcdfDb(name),
  woa_db_type(db_type),
  ssp_var(),
  lat_var(),
  lon_var()
{ 
}
#endif // defined (WOSS_NETCDF4_SUPPORT)


bool SspWoa2005Db::finalizeConnection() {
  if (woa_db_type == WOA_DB_TYPE_2005) {
#if defined (WOSS_NETCDF4_SUPPORT)
    ssp_var = netcdf_db->getVar("ssp");
    if (ssp_var.isNull()) {
      ::std::cout << "SspWoa2005Db::finalizeConnection() 2005 ssp_var is not valid" << ::std::endl;
      return false;
    }
    return true;
  #else
    ssp_var = netcdf_db->get_var( "ssp" );
    return (ssp_var != 0);
#endif // defined (WOSS_NETCDF4_SUPPORT)
  }
#if defined (WOSS_NETCDF4_SUPPORT)
  else if (woa_db_type == WOA_DB_TYPE_2013) {
    ssp_var = netcdf_db->getVar("ssp");
    if (ssp_var.isNull()) {
      ::std::cout << "SspWoa2005Db::finalizeConnection() 2013 ssp_var is not valid" << ::std::endl;
      return false;
    }

    lat_var = netcdf_db->getVar("latitude");
    if (lat_var.isNull()) {
      ::std::cout << "SspWoa2005Db::finalizeConnection() 2013 lat_var is not valid" << ::std::endl;
      return false;
    }

    lon_var = netcdf_db->getVar("longitude");
    if (lon_var.isNull()) {
      ::std::cout << "SspWoa2005Db::finalizeConnection() 2013 lon_var is not valid" << ::std::endl;
      return false;
    }

    return true;
  }
#endif // defined (WOSS_NETCDF4_SUPPORT)
  else {
    ::std::cout << "SspWoa2005Db::finalizeConnection() woa_db_type is not valid" << ::std::endl;
    return false;
  }
}


bool SspWoa2005Db::insertValue( const Coord& coordinates, const Time& time_value, const SSP& ssp_value ) {
  ::std::cerr << "SspWoa2005Db::insertValue() method is not implemented." << ::std::endl;
  return false;
}


SSPIndexes SspWoa2005Db::getSSPIndexes( const Coord& coordinates ) const {
  if (woa_db_type == WOA_DB_TYPE_2005) {
  double lat = floor(coordinates.getLatitude()) + 0.5;
  double lon = floor(coordinates.getLongitude()) + 0.5;

  int quantized_latitude = (int) ::std::abs(lat - SSP_WOA2005_STD_MAX_LAT);
  int quantized_longitude = (int) (lon + SSP_WOA2005_STD_MAX_LON);

  if (debug) ::std::cout << "SspWoa2005Db::getSSPIndexes() 2005 latitude = " << coordinates.getLatitude() << "; longitude = " 
                        << coordinates.getLongitude() << "; lat = " << lat << "; lon = " << lon 
                        << "; quant lat = " << quantized_latitude << "; quantized_longitude = " 
                        << quantized_longitude << ::std::endl;
//   debugWaitForUser();
 
  return ( ::std::make_pair(quantized_latitude, quantized_longitude) );
  }
#if defined (WOSS_NETCDF4_SUPPORT)
  else if (woa_db_type == WOA_DB_TYPE_2013) {
    double quantized_latitude = abs((coordinates.getLatitude() - SSP_WOA2013_STD_START_LAT) / SSP_WOA2013_STD_SPACING ); 
    if ( quantized_latitude - floor(quantized_latitude)  >= 0.5 ) quantized_latitude = ceil(quantized_latitude); 
    else quantized_latitude = floor(quantized_latitude);

    assert(quantized_latitude >= 0 && quantized_latitude <= SSP_WOA2013_STD_NLAT);

    double quantized_longitude = abs( ( coordinates.getLongitude() - SSP_WOA2013_STD_START_LON ) / SSP_WOA2013_STD_SPACING ); 
    if ( quantized_longitude - floor(quantized_longitude)  >= 0.5 ) quantized_longitude = ceil(quantized_longitude);
    else quantized_longitude = floor(quantized_longitude);

    assert(quantized_longitude >= 0 && quantized_longitude <= SSP_WOA2013_STD_NLON);

    if (debug)
      ::std::cout << "SspWoa2005Db::getSSPIndexes() 2013 q_lat = " << quantized_latitude << "; q_lon = " << quantized_longitude << ::std::endl;

    return ( ::std::make_pair(quantized_latitude, quantized_longitude) );
  }
#endif // defined (WOSS_NETCDF4_SUPPORT)
  else {
    ::std::cout << "SspWoa2005Db::getSSPIndexes() wrong woa_db_type" << ::std::endl;
    return ( ::std::make_pair(SSP_NOT_VALID, SSP_NOT_VALID) );
  }
} 

#if defined (WOSS_NETCDF4_SUPPORT)
void SspWoa2005Db::getSSPValue( const Coord& coords, const SSPIndexes& indexes, double ssp_values[] ) const
#else
void SspWoa2005Db::getSSPValue( const SSPIndexes& indexes, double* ssp_values ) const
#endif // defined (WOSS_NETCDF4_SUPPORT)
{
  if (woa_db_type == WOA_DB_TYPE_2005) {
#if defined (WOSS_NETCDF4_SUPPORT)
    ::std::vector<size_t> index_vector, count_vector;

    index_vector.push_back((size_t)indexes.first);
    index_vector.push_back((size_t)indexes.second);
    index_vector.push_back((size_t)0);

    count_vector.push_back((size_t)1);
    count_vector.push_back((size_t)1);
    count_vector.push_back((size_t)SSP_STD_NDEPTH);

    ssp_values[0] = SSP_NOT_VALID;

    ssp_var.getVar(index_vector, count_vector, ssp_values);
    if (ssp_values[0] == SSP_NOT_VALID) {
      ::std::cerr << "SspWoa2005Db::getSSPValue() 2005 Couldn't extract ssp value" << ::std::endl;
      exit(1);
    }
#else
    NcBool ret_value = ssp_var->set_cur(indexes.first, indexes.second, 0);
    if (!ret_value) {
      ::std::cerr << "SspWoa2005Db::getSSPValue() 2005 Couldn't set_cur() " << indexes.first << " , " << indexes.second << ::std::endl;
      exit(1);
    }
    
    ret_value = ssp_var->get(ssp_values, 1, 1, SSP_STD_NDEPTH);
    if (!ret_value) {
      ::std::cerr << "SspWoa2005Db::getSSPValue() 2005 Couldn't extract ssp value" << ::std::endl;
      exit(1);
    }
#endif // defined (WOSS_NETCDF4_SUPPORT)
  }
#if defined (WOSS_NETCDF4_SUPPORT)
  else if (woa_db_type == WOA_DB_TYPE_2013) {
    ::std::vector<size_t> index_vector, count_vector;
    double lat = SSP_NOT_VALID;
    double lon = SSP_NOT_VALID;

    index_vector.push_back((size_t)indexes.first);

    lat_var.getVar(index_vector, &lat);
    if (lat == SSP_NOT_VALID) {
      ::std::cout << "SspWoa2005Db::getSSPValue() 2013 Couldn't extract current lat" << ::std::endl;
      exit(1);
    }

    if (debug)
      ::std::cout << "SspWoa2005Db::getSSPValue() 2013 got lat = " << lat << ::std::endl;

    index_vector.clear();
    index_vector.push_back((size_t)indexes.second);

    lon_var.getVar(index_vector, &lon);
    if (lon == SSP_NOT_VALID) {
      ::std::cout << "SspWoa2005Db::getSSPValue() 2013 Couldn't extract current lon" << ::std::endl;
      exit(1);
    }

    if (debug)
      ::std::cout << "SspWoa2005Db::getSSPValue() 2013 got lon = " << lon << ::std::endl;

    assert(::std::abs(coords.getLatitude() - lat) <= SSP_WOA2013_STD_SPACING);
    assert(::std::abs(coords.getLongitude() - lon) <= SSP_WOA2013_STD_SPACING);

    index_vector.clear();

    index_vector.push_back((size_t)indexes.first);
    index_vector.push_back((size_t)indexes.second);
    index_vector.push_back((size_t)0);

    count_vector.push_back((size_t)1);
    count_vector.push_back((size_t)1);
    count_vector.push_back((size_t)SSP_STD_NDEPTH);

    ssp_values[0] = SSP_NOT_VALID;

    ssp_var.getVar(index_vector, count_vector, ssp_values);
    if (ssp_values[0] == SSP_NOT_VALID) {
      ::std::cerr << "SspWoa2005Db::getSSPValue() 2013 Couldn't extract ssp value" << ::std::endl;
      exit(1);
    }
  }
#endif // defined (WOSS_NETCDF4_SUPPORT)
  else {
    ::std::cerr << "SspWoa2005Db::getSSPValue() wrong woa_db_type" << ::std::endl;
    exit(1);
  }
}


SSP* SspWoa2005Db::getValue( const Coord& coordinates, const Time& time, long double ssp_depth_precision ) const {
  static double curr_ssp[SSP_STD_NDEPTH];

  DepthMap ssp_map;
  
  SSPIndexes curr_ind = getSSPIndexes(coordinates);
  
  if(debug) ::std::cout << "SspWoa2005Db::getValue() coordinates = " << coordinates << "; indexes = " << curr_ind.first << " , " 
                       << curr_ind.second << ::std::endl;

#if defined (WOSS_NETCDF4_SUPPORT)
  getSSPValue( coordinates, curr_ind, curr_ssp );
#else
  getSSPValue( curr_ind, curr_ssp );
#endif // defined (WOSS_NETCDF4_SUPPORT)
  
  for (int i = 0; i < SSP_STD_NDEPTH; i++) {
    
    if(debug) ::std::cout << "SspWoa2005Db::getValue() depth = " << ssp_std_depths[i] << "; ssp value = " << curr_ssp[i] << ::std::endl;

    // if ssp_values = 0.0 ===> coords are quantized on land
    if ( curr_ssp[i] == 0.0 ) {
      
      ::std::cerr << "SspWoa2005Db::getValue() coordinates = " << coordinates << " are quantized on land. " << ::std::endl; // trial no " << tries_counter << ::std::endl;

      return SDefHandler::instance()->getSSP()->create();
    }
    ssp_map.insert( ::std::make_pair( PDouble( ssp_std_depths[i], ssp_depth_precision ), curr_ssp[i] ) );
  }

  return( SDefHandler::instance()->getSSP()->create( ssp_map, ssp_depth_precision ) );
}

#endif // WOSS_NETCDF_SUPPORT

