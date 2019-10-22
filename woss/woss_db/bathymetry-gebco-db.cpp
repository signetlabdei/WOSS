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
 * @file   bathymetry-gebco-db.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::BathyGebcoDb class
 *
 * Provides the implementation of the woss::BathyGebcoDb class
 */

#ifdef WOSS_NETCDF_SUPPORT

#include <cassert>
#include <cstdlib>
#include <cmath>
#include <definitions.h>
#include "bathymetry-gebco-db.h"


using namespace woss;

#define GEBCO_1D_INVALID_INDEX (-1)
#define GEBCO_2D_INVALID_INDEXES woss::Gebco2DIndexes( GEBCO_1D_INVALID_INDEX, GEBCO_1D_INVALID_INDEX )
#define GEBCO_NOT_FOUND (-HUGE_VAL)


BathyGebcoDb::BathyGebcoDb( const ::std::string& name ) 
: WossNetcdfDb(name),
  gebco_type(GEBCO_2D_30_SECONDS_BATHY_TYPE),
#if defined(WOSS_NETCDF4_SUPPORT)
  bathy_var(),
  lat_var(),
  lon_var()
#else
  bathy_var(NULL),
  lat_var(NULL),
  lon_var(NULL)
#endif // defined(WOSS_NETCDF4_SUPPORT)
{

}


bool BathyGebcoDb::finalizeConnection() {
#if defined (WOSS_NETCDF4_SUPPORT)
    if (debug) {
      ::std::cout << "BathyGebcoDb::finalizeConnection() name = " << netcdf_db->getName() << "; num_dim = " << netcdf_db->getDimCount() 
                  << "; num_vars = " << netcdf_db->getVarCount() << "; num_atts = " << netcdf_db->getAttCount() << ::std::endl;
    }

    if ((gebco_type == GEBCO_1D_1_MINUTE_BATHY_TYPE) || (gebco_type == GEBCO_1D_30_SECONDS_BATHY_TYPE)) {
      bathy_var = netcdf_db->getVar("z");
      if (bathy_var.isNull()) {
        ::std::cout << "BathyGebcoDb::finalizeConnection() bathy_var is not valid" << ::std::endl;
        return false;
      }
    }
    else {
      bathy_var = netcdf_db->getVar("elevation");
      if (bathy_var.isNull()) {
        ::std::cout << "BathyGebcoDb::finalizeConnection() bathy_var is not valid" << ::std::endl;
        return false;
      }

      lat_var = netcdf_db->getVar("lat");
      if (lat_var.isNull()) {
        ::std::cout << "BathyGebcoDb::finalizeConnection() lat_var is not valid" << ::std::endl;
        return false;
      }

      lon_var = netcdf_db->getVar("lon");
      if (lon_var.isNull()) {
        ::std::cout << "BathyGebcoDb::finalizeConnection() lon_var is not valid" << ::std::endl;
        return false;
      }

      if (debug) {
        ::std::cout << "BathyGebcoDb::finalizeConnection() bathy name = " << bathy_var.getName() << "; num_dim = " << bathy_var.getDimCount() 
                    << "; num_atts = " << bathy_var.getAttCount() << ::std::endl;
        ::std::cout << "BathyGebcoDb::finalizeConnection() lat name = " << lat_var.getName() << "; num_dim = " << lat_var.getDimCount() 
                    << "; num_atts = " << lat_var.getAttCount() << ::std::endl;
        ::std::cout << "BathyGebcoDb::finalizeConnection() lon name = " << lon_var.getName() << "; num_dim = " << lon_var.getDimCount() 
                    << "; num_atts = " << lon_var.getAttCount() << ::std::endl;
      }
    }
    return true;
#else
  bool is_valid = (bool) netcdf_db->is_valid();

  if (is_valid == true) {
    if (debug) {
      ::std::cout << "BathyGebcoDb::finalizeConnection() num_dim = " << netcdf_db->num_dims() 
                  << "; num_vars = " << netcdf_db->num_vars() << "; num_atts = " << netcdf_db->num_atts() << ::std::endl;
    }

    if ((gebco_type == GEBCO_1D_1_MINUTE_BATHY_TYPE) || (gebco_type == GEBCO_1D_30_SECONDS_BATHY_TYPE)) {
      bathy_var = netcdf_db->get_var("z");
    }
    else {
      bathy_var = netcdf_db->get_var("elevation");
      lat_var = netcdf_db->get_var("lat");
      lon_var = netcdf_db->get_var("lon");

      if (debug) {
        ::std::cout << "BathyGebcoDb::finalizeConnection() bathy valid = " << bathy_var->is_valid() << "; num_dim = " << bathy_var->num_dims() 
                    << "; num_vals = " << bathy_var->num_vals() << "; num_atts = " << bathy_var->num_atts() << ::std::endl;
        ::std::cout << "BathyGebcoDb::finalizeConnection() lat valid = " << lat_var->is_valid() << "; num_dim = " << lat_var->num_dims() 
                    << "; num_vals = " << lat_var->num_vals() << "; num_atts = " << lat_var->num_atts() << ::std::endl;
        ::std::cout << "BathyGebcoDb::finalizeConnection() lon valid = " << lon_var->is_valid() << "; num_dim = " << lon_var->num_dims() 
                    << "; num_vals = " << lon_var->num_vals() << "; num_atts = " << lon_var->num_atts() << ::std::endl;
      }
    }
    return (bathy_var != 0);
  }

  ::std::cout << "BathyGebcoDb::finalizeConnection() db is not valid" << ::std::endl;

  return false;
#endif // defined (WOSS_NETCDF4_SUPPORT)
}


bool BathyGebcoDb::insertValue( const Coord& coordinates, const Bathymetry& bathymetry_value ) {
  ::std::cerr << "BathyGebcoDb::insertValue() method is not implemented." << ::std::endl;
  return false;
}


double BathyGebcoDb::getValue( const Coord& coords ) const {
  double depth = GEBCO_NOT_FOUND;

  if ((gebco_type == GEBCO_1D_1_MINUTE_BATHY_TYPE) || (gebco_type == GEBCO_1D_30_SECONDS_BATHY_TYPE)) {
    int index = get1DBathyIndex(coords);
    
    if (index == GEBCO_1D_INVALID_INDEX) {
      ::std::cout << "BathyGebcoDb::getValue() WARNING invalid 1D index returned for coords = " << coords << ::std::endl;
      return (HUGE_VAL);
    }

#if defined (WOSS_NETCDF4_SUPPORT)
    ::std::vector<size_t> index_vector (1, (size_t)index);

    bathy_var.getVar(index_vector, &depth);
    if (depth == GEBCO_NOT_FOUND) {
      ::std::cout << "BathyGebcoDb::getValue() Couldn't extract current depth" << ::std::endl;
      return (HUGE_VAL);
    }
#else
    NcBool ret_val = bathy_var->set_cur(index);
    if (!ret_val) {
      ::std::cout << "BathyGebcoDb::getValue() Couldn't set_cur() current index = " << index << ::std::endl;
      return (HUGE_VAL);
    }

    ret_val = bathy_var->get(&depth,1);
    if (!ret_val) {
      ::std::cout << "BathyGebcoDb::getValue() Couldn't extract current depth" << ::std::endl;
      return (HUGE_VAL);
    }
#endif // defined (WOSS_NETCDF4_SUPPORT)
  }
  else if ((gebco_type == GEBCO_2D_1_MINUTE_BATHY_TYPE) || (gebco_type == GEBCO_2D_30_SECONDS_BATHY_TYPE) || (gebco_type == GEBCO_2D_15_SECONDS_BATHY_TYPE)) {
    Gebco2DIndexes indexes = get2DBathyIndexes(coords);
    double lat = GEBCO_NOT_FOUND;
    double lon = GEBCO_NOT_FOUND;

    if (indexes == GEBCO_2D_INVALID_INDEXES) {
      ::std::cout << "BathyGebcoDb::getValue() WARNING invalid 2D indexes returned for coords = " << coords << ::std::endl;
      return (HUGE_VAL);
    }

#if defined (WOSS_NETCDF4_SUPPORT)
    ::std::vector<size_t> index_vector;
    index_vector.push_back((size_t)indexes.first);

    lat_var.getVar(index_vector, &lat);
    if (lat == GEBCO_NOT_FOUND) {
      ::std::cout << "BathyGebcoDb::getValue() Couldn't extract current lat" << ::std::endl;
      return (HUGE_VAL);
    }

    if (debug)
      ::std::cout << "BathyGebcoDb::getValue() got lat = " << lat << ::std::endl;

    index_vector.clear();
    index_vector.push_back((size_t)indexes.second);

    lon_var.getVar(index_vector, &lon);
    if (lon == GEBCO_NOT_FOUND) {
      ::std::cout << "BathyGebcoDb::getValue() Couldn't extract current lon" << ::std::endl;
      return (HUGE_VAL);
    }

    if (debug)
      ::std::cout << "BathyGebcoDb::getValue() got lon = " << lon << ::std::endl;

    if (gebco_type == GEBCO_2D_1_MINUTE_BATHY_TYPE) {
      assert(::std::abs(coords.getLatitude() - lat) <= GEBCO_1_MINUTE_BATHY_SPACING);
      assert(::std::abs(coords.getLongitude() - lon) <= GEBCO_1_MINUTE_BATHY_SPACING);
    }
    else if (gebco_type == GEBCO_2D_30_SECONDS_BATHY_TYPE) {
      assert(::std::abs(coords.getLatitude() - lat) <= GEBCO_30_SECONDS_BATHY_SPACING);
      assert(::std::abs(coords.getLongitude() - lon) <= GEBCO_30_SECONDS_BATHY_SPACING);
    }
    else {
      assert(::std::abs(coords.getLatitude() - lat) <= GEBCO_15_SECONDS_BATHY_SPACING);
      assert(::std::abs(coords.getLongitude() - lon) <= GEBCO_15_SECONDS_BATHY_SPACING);
    }

    index_vector.clear();
    index_vector.push_back((size_t)indexes.first);
    index_vector.push_back((size_t)indexes.second);

    bathy_var.getVar(index_vector, &depth);
    if (lon == GEBCO_NOT_FOUND) {
      ::std::cout << "BathyGebcoDb::getValue() Couldn't extract current depth" << ::std::endl;
      return (HUGE_VAL);
    }
#else
    NcBool ret_val = lat_var->set_cur(indexes.first);
    if (!ret_val) {
      ::std::cout << "BathyGebcoDb::getValue() Couldn't lat_var set_cur() current index = " << indexes.first << ::std::endl;
      return (HUGE_VAL);
    }

    ret_val = lat_var->get(&lat,1);
    if (!ret_val) {
      ::std::cout << "BathyGebcoDb::getValue() Couldn't extract current lat" << ::std::endl;
      return (HUGE_VAL);
    }

    if (debug)
      ::std::cout << "BathyGebcoDb::getValue() got lat = " << lat << ::std::endl;

    ret_val = lon_var->set_cur(indexes.second);
    if (!ret_val) {
      ::std::cout << "BathyGebcoDb::getValue() Couldn't lon_var set_cur() current index = " << indexes.second << ::std::endl;
      return (HUGE_VAL);
    }

    ret_val = lon_var->get(&lon,1);
    if (!ret_val) {
      ::std::cout << "BathyGebcoDb::getValue() Couldn't extract current lon" << ::std::endl;
      return (HUGE_VAL);
    }

    if (debug)
      ::std::cout << "BathyGebcoDb::getValue() got lon = " << lon << ::std::endl;

    if (gebco_type == GEBCO_2D_1_MINUTE_BATHY_TYPE) {
      assert(::std::abs(coords.getLatitude() - lat) <= GEBCO_1_MINUTE_BATHY_SPACING);
      assert(::std::abs(coords.getLongitude() - lon) <= GEBCO_1_MINUTE_BATHY_SPACING);
    }
    else if (gebco_type == GEBCO_2D_30_SECONDS_BATHY_TYPE) {
      assert(::std::abs(coords.getLatitude() - lat) <= GEBCO_30_SECONDS_BATHY_SPACING);
      assert(::std::abs(coords.getLongitude() - lon) <= GEBCO_30_SECONDS_BATHY_SPACING);
    }
    else {
      assert(::std::abs(coords.getLatitude() - lat) <= GEBCO_15_SECONDS_BATHY_SPACING);
      assert(::std::abs(coords.getLongitude() - lon) <= GEBCO_15_SECONDS_BATHY_SPACING);
    }

    ret_val = bathy_var->set_cur(indexes.first, indexes.second);
    if (!ret_val) {
      ::std::cout << "BathyGebcoDb::getValue() Couldn't set_cur() current indexes = " << indexes.first << "," << indexes.second << ::std::endl;
      return (HUGE_VAL);
    }

    ret_val = bathy_var->get(&depth,1,1);
    if (!ret_val) {
      ::std::cout << "BathyGebcoDb::getValue() Couldn't extract current depth" << ::std::endl;
      return (HUGE_VAL);
    }
#endif // defined (WOSS_NETCDF4_SUPPORT)
  }
  else {
    ::std::cout << "BathyGebcoDb::getValue() invalid gebco_type = " << gebco_type << ::std::endl;
    return (HUGE_VAL);
  }

  if (debug)
    ::std::cout << "BathyGebcoDb::getValue() coordinates = " << coords << "; altitude = " << depth << ::std::endl;

  if ( depth != GEBCO_NOT_FOUND && depth <= 0.0 ) return(::std::abs(depth));
  else {
    ::std::cout << "BathyGebcoDb::getValue() WARNING current coordinates are on land : " << coords << "; altitude = " << depth << ::std::endl;
    return (HUGE_VAL);
  }
}
 

long BathyGebcoDb::get1DBathyIndex( const Coord& coords ) const {
  if (gebco_type == GEBCO_1D_1_MINUTE_BATHY_TYPE) {
    double quantized_latitude = ::std::abs( coords.getLatitude() / GEBCO_1_MINUTE_BATHY_SPACING ); 
    if ( ( quantized_latitude - floor(quantized_latitude) ) >= 0.5) quantized_latitude = ceil(quantized_latitude); 
    else quantized_latitude = floor(quantized_latitude);
    quantized_latitude -= (GEBCO_1_MINUTE_BATHY_NLAT - 1) / 2; // from +90.0 to -90.0

    assert(quantized_latitude >= 0 && quantized_latitude <= GEBCO_1_MINUTE_BATHY_NLAT);

    double quantized_longitude = ::std::abs( coords.getLongitude() / GEBCO_1_MINUTE_BATHY_SPACING ); 
    if ( ( quantized_longitude - floor(quantized_longitude) ) >= 0.5) quantized_longitude = ceil(quantized_longitude);
    else quantized_longitude = floor(quantized_longitude);
    quantized_longitude += (GEBCO_1_MINUTE_BATHY_NLON - 1) / 2; // from -180.0 to 180.0

    assert(quantized_longitude >= 0 && quantized_longitude <= GEBCO_1_MINUTE_BATHY_NLON);

    if (debug) 
      ::std::cout << "BathyGebcoDb::get1DBathyIndex() minutes q_lat = " << quantized_latitude << "; q_lon = " << quantized_longitude << ::std::endl;

    return( (long)(quantized_latitude * GEBCO_1_MINUTE_BATHY_NLON) + (long)quantized_longitude );
  }
  else if (gebco_type == GEBCO_1D_30_SECONDS_BATHY_TYPE) {
    double quantized_latitude = ::std::abs( ( coords.getLatitude() - GEBCO_1D_30_SECONDS_BATHY_START_LAT ) / GEBCO_30_SECONDS_BATHY_SPACING ); 
    if ( quantized_latitude - floor(quantized_latitude)  >= 0.5 ) quantized_latitude = ceil(quantized_latitude); 
    else quantized_latitude = floor(quantized_latitude);

    assert(quantized_latitude >= 0 && quantized_latitude <= GEBCO_30_SECONDS_BATHY_NLAT);
    
    double quantized_longitude = abs( ( coords.getLongitude() - GEBCO_1D_30_SECONDS_BATHY_START_LONG ) / GEBCO_30_SECONDS_BATHY_SPACING ); 
    if ( quantized_longitude - floor(quantized_longitude)  >= 0.5 ) quantized_longitude = ceil(quantized_longitude);
    else quantized_longitude = floor(quantized_longitude);

    assert(quantized_longitude >= 0 && quantized_longitude <= GEBCO_30_SECONDS_BATHY_NLON);

    if (debug) 
      ::std::cout << "BathyGebcoDb::get1DBathyIndex() thirty seconds q_lat = " << quantized_latitude << "; q_lon = " << quantized_longitude << ::std::endl;

    return( (long)(quantized_latitude * GEBCO_30_SECONDS_BATHY_NLON) + (long)quantized_longitude );
  }
  else {
    ::std::cout << "BathyGebcoDb::get1DBathyIndex() ERROR wrong GEBCO type " << gebco_type << ::std::endl;
    return GEBCO_1D_INVALID_INDEX;
  }
}

Gebco2DIndexes BathyGebcoDb::get2DBathyIndexes( const Coord& coords ) const
{
  if (gebco_type == GEBCO_2D_1_MINUTE_BATHY_TYPE) {
    double quantized_latitude = ::std::abs( ( coords.getLatitude() - GEBCO_2D_1_MINUTE_BATHY_START_LAT ) / GEBCO_1_MINUTE_BATHY_SPACING ); 
    if ( quantized_latitude - floor(quantized_latitude)  >= 0.5 ) quantized_latitude = ceil(quantized_latitude); 
    else quantized_latitude = floor(quantized_latitude);

    assert(quantized_latitude >= 0 && quantized_latitude <= GEBCO_1_MINUTE_BATHY_NLAT);

    double quantized_longitude = abs( ( coords.getLongitude() - GEBCO_2D_1_MINUTE_BATHY_START_LONG ) / GEBCO_1_MINUTE_BATHY_SPACING ); 
    if ( quantized_longitude - floor(quantized_longitude)  >= 0.5 ) quantized_longitude = ceil(quantized_longitude);
    else quantized_longitude = floor(quantized_longitude);

    assert(quantized_longitude >= 0 && quantized_longitude <= GEBCO_1_MINUTE_BATHY_NLON);

    if (debug)
      ::std::cout << "BathyGebcoDb::get2DBathyIndex() minutes seconds q_lat = " << quantized_latitude << "; q_lon = " << quantized_longitude << ::std::endl;

    return Gebco2DIndexes((long)quantized_latitude, (long)quantized_longitude);
  }
  else if (gebco_type == GEBCO_2D_30_SECONDS_BATHY_TYPE) {
    double quantized_latitude = ::std::abs( ( coords.getLatitude() - GEBCO_2D_30_SECONDS_BATHY_START_LAT ) / GEBCO_30_SECONDS_BATHY_SPACING ); 
    if ( quantized_latitude - floor(quantized_latitude)  >= 0.5 ) quantized_latitude = ceil(quantized_latitude); 
    else quantized_latitude = floor(quantized_latitude);

    assert(quantized_latitude >= 0 && quantized_latitude <= GEBCO_30_SECONDS_BATHY_NLAT);

    double quantized_longitude = abs( ( coords.getLongitude() - GEBCO_2D_30_SECONDS_BATHY_START_LONG ) / GEBCO_30_SECONDS_BATHY_SPACING ); 
    if ( quantized_longitude - floor(quantized_longitude)  >= 0.5 ) quantized_longitude = ceil(quantized_longitude);
    else quantized_longitude = floor(quantized_longitude);

    assert(quantized_longitude >= 0 && quantized_longitude <= GEBCO_30_SECONDS_BATHY_NLON);

    if (debug)
      ::std::cout << "BathyGebcoDb::get2DBathyIndex() thirty seconds q_lat = " << quantized_latitude << "; q_lon = " << quantized_longitude << ::std::endl;

    return Gebco2DIndexes((long)quantized_latitude, (long)quantized_longitude);
  }
  else if (gebco_type == GEBCO_2D_15_SECONDS_BATHY_TYPE) {
    double quantized_latitude = ::std::abs( ( coords.getLatitude() - GEBCO_2D_15_SECONDS_BATHY_START_LAT ) / GEBCO_15_SECONDS_BATHY_SPACING ); 
    if ( quantized_latitude - floor(quantized_latitude)  >= 0.5 ) quantized_latitude = ceil(quantized_latitude); 
    else quantized_latitude = floor(quantized_latitude);

    assert(quantized_latitude >= 0 && quantized_latitude <= GEBCO_15_SECONDS_BATHY_NLAT);

    double quantized_longitude = abs( ( coords.getLongitude() - GEBCO_2D_15_SECONDS_BATHY_START_LONG ) / GEBCO_15_SECONDS_BATHY_SPACING ); 
    if ( quantized_longitude - floor(quantized_longitude)  >= 0.5 ) quantized_longitude = ceil(quantized_longitude);
    else quantized_longitude = floor(quantized_longitude);

    assert(quantized_longitude >= 0 && quantized_longitude <= GEBCO_15_SECONDS_BATHY_NLON);

    if (debug)
      ::std::cout << "BathyGebcoDb::get2DBathyIndex() fifteen seconds q_lat = " << quantized_latitude << "; q_lon = " << quantized_longitude << ::std::endl;

    return Gebco2DIndexes((long)quantized_latitude, (long)quantized_longitude);
  }
  else {
    ::std::cout << "BathyGebcoDb::get2DBathyIndexes() ERROR wrong GEBCO type " << gebco_type << ::std::endl;
    return GEBCO_2D_INVALID_INDEXES;
  }
}


#endif // WOSS_NETCDF_SUPPORT

