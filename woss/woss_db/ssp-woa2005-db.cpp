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

static const int SSP_WOA2005_STD_NLAT = 180; /**< Custom made NetCDF WOA2005 SSP total number of latitudes */

static const int SSP_WOA2005_STD_NLON = 360; /**< Custom made NetCDF WOA2005 SSP total number of longitudes */

static const double SSP_WOA2005_STD_SPACING = 1.0; /**< Spacing between coordinates value [decimal degree] */


static const double SSP_WOA2005_STD_MIN_LAT = -89.5; /**< Custom made NetCDF WOA2005 SSP mimimum latitude */

static const double SSP_WOA2005_STD_MAX_LAT = 89.5; /**< Custom made NetCDF WOA2005 SSP maximum latitude */


static const double SSP_WOA2005_STD_MIN_LONG = -179.5; /**< Custom made NetCDF WOA2005 SSP mimimum longitude */

static const double SSP_WOA2005_STD_MAX_LONG = 179.5; /**< Custom made NetCDF WOA2005 SSP maximum longitude */


static const double SSP_WOA2005_STD_START_LAT = 89.5; /**< Custom made NetCDF WOA2005 SSP start latitude */

static const double SSP_WOA2005_STD_START_LONG = -179.5; /**< Custom made NetCDF WOA2005 SSP start longitude */


static const int SSP_WOA2005_STD_NDEPTH = 33;  /**< Custom made NetCDF WOA2005 SSP total number of depth of a SSP */

/**
* Standard depths of any created SSP
**/
static int ssp_woa2005_std_depths[SSP_WOA2005_STD_NDEPTH] = { 0, 10, 20, 30, 50, 75, 100, 125, 150, 200, 250, 300,
                                                              400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500,
                                                              1750, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500 };


SspWoa2005Db::SspWoa2005Db( const ::std::string& name ) 
: WossNetcdfDb(name),
#if defined (WOSS_NETCDF4_SUPPORT)
  ssp_var()
#else
  ssp_var(NULL)
#endif // defined (WOSS_NETCDF4_SUPPORT)
{ 
}


bool SspWoa2005Db::finalizeConnection() {
#if defined (WOSS_NETCDF4_SUPPORT)
  ssp_var = netcdf_db->getVar("ssp");
  if (ssp_var.isNull()) {
    ::std::cout << "SspWoa2005Db::finalizeConnection() ssp_var is not valid" << ::std::endl;
    return false;
  }
  return true;
#else
  ssp_var = netcdf_db->get_var( "ssp" );
  return (ssp_var != 0);
#endif // defined (WOSS_NETCDF4_SUPPORT)
}


bool SspWoa2005Db::insertValue( const Coord& coordinates, const Time& time_value, const SSP& ssp_value ) {
  ::std::cerr << "SspWoa2005Db::insertValue() method is not implemented." << ::std::endl;
  return false;
}


SSPIndexes SspWoa2005Db::getSSPIndexes( const Coord& coordinates ) const {
 
  double lat = floor(coordinates.getLatitude()) + 0.5;
  double lon = floor(coordinates.getLongitude()) + 0.5;

  int quantized_latitude = (int) ::std::abs(lat - SSP_WOA2005_STD_MAX_LAT);
  int quantized_longitude = (int) (lon + SSP_WOA2005_STD_MAX_LONG);

  if (debug) ::std::cout << "SspWoa2005Db::getSSPIndexes() latitude = " << coordinates.getLatitude() << "; longitude = " 
                        << coordinates.getLongitude() << "; lat = " << lat << "; lon = " << lon 
                        << "; quant lat = " << quantized_latitude << "; quantized_longitude = " 
                        << quantized_longitude << ::std::endl;
//   debugWaitForUser();
 
  return ( ::std::make_pair(quantized_latitude, quantized_longitude) );
} 


void SspWoa2005Db::getSSPValue( const SSPIndexes& indexes, double* ssp_values ) const {
#if defined (WOSS_NETCDF4_SUPPORT)
  ::std::vector<size_t> index_vector, count_vector;

  index_vector.push_back((size_t)indexes.first);
  index_vector.push_back((size_t)indexes.second);
  index_vector.push_back((size_t)0);

  count_vector.push_back((size_t)1);
  count_vector.push_back((size_t)1);
  count_vector.push_back((size_t)SSP_WOA2005_STD_NDEPTH);

  ssp_values[0] = SSP_NOT_VALID;

  ssp_var.getVar(index_vector, count_vector, ssp_values);
  if (ssp_values[0] == SSP_NOT_VALID) {
    ::std::cerr << "SspWoa2005Db::getSSPValue() Couldn't extract ssp value" << ::std::endl;
    exit(1);
  }
#else
  NcBool ret_value = ssp_var->set_cur(indexes.first, indexes.second, 0);
  if (!ret_value) {
    ::std::cerr << "SspWoa2005Db::getSSPValue() Couldn't set_cur() " << indexes.first << " , " << indexes.second << ::std::endl;
    exit(1);
  }
  
  ret_value = ssp_var->get(ssp_values, 1, 1, SSP_WOA2005_STD_NDEPTH);
  if (!ret_value) {
    ::std::cerr << "SspWoa2005Db::getSSPValue() Couldn't extract ssp value" << ::std::endl;
    exit(1);
  }
#endif // defined (WOSS_NETCDF4_SUPPORT)
}


SSP* SspWoa2005Db::getValue( const Coord& coordinates, const Time& time, long double ssp_depth_precision ) const {
  static double curr_ssp[SSP_WOA2005_STD_NDEPTH];

  DepthMap ssp_map;
  
  SSPIndexes curr_ind = getSSPIndexes(coordinates);
  
  if(debug) ::std::cout << "SspWoa2005Db::getValue() coordinates = " << coordinates << "; indexes = " << curr_ind.first << " , " 
                       << curr_ind.second << ::std::endl;

  getSSPValue( curr_ind, curr_ssp );

  for (int i = 0; i < SSP_WOA2005_STD_NDEPTH; i++) {
    
    if(debug) ::std::cout << "SspWoa2005Db::getValue() depth = " << ssp_woa2005_std_depths[i] << "; ssp value = " << curr_ssp[i] << ::std::endl;

    // if ssp_values = 0.0 ===> coords are quantized on land
    if ( curr_ssp[i] == 0.0 ) {
      
      ::std::cerr << "SspWoa2005Db::getValue() coordinates = " << coordinates << " are quantized on land. " << ::std::endl; // trial no " << tries_counter << ::std::endl;

      return SDefHandler::instance()->getSSP()->create();
    }
    ssp_map.insert( ::std::make_pair( PDouble( ssp_woa2005_std_depths[i], ssp_depth_precision ), curr_ssp[i] ) );
  }

  return( SDefHandler::instance()->getSSP()->create( ssp_map, ssp_depth_precision ) );
}

#endif // WOSS_NETCDF_SUPPORT

