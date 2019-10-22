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
 * @file   sediment-deck41-coord-db.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::SedimDeck41CoordDb class
 *
 * Provides the implementation of the woss::SedimDeck41CoordDb class
 */

#ifdef WOSS_NETCDF_SUPPORT

#include <cmath>
#include <cstdlib>
#include "sediment-deck41-coord-db.h" 

#define SEDIMENT_NOT_FOUND (-100000000)

using namespace woss;


SedimDeck41CoordDb::SedimDeck41CoordDb( const ::std::string& name ) 
: WossNetcdfDb( name ),
#if defined (WOSS_NETCDF4_SUPPORT)
  main_sedim_var_coord(),
  sec_sedim_var_coord()
#else
  main_sedim_var_coord(NULL),
  sec_sedim_var_coord(NULL)
#endif // defined (WOSS_NETCDF4_SUPPORT)
{
  
}

int SedimDeck41CoordDb::getSedimIndex( const Coord& coords ) const {
  double quantized_latitude = (double) coords.getLatitude() / DECK41_MINUTES_SEDIM_SPACING; 
  if ( (::std::abs(quantized_latitude) - floor(::std::abs(quantized_latitude))) >= 0.5) 
       quantized_latitude = ceil(quantized_latitude); 
  else quantized_latitude = floor(quantized_latitude);
  quantized_latitude -= (DECK41_MINUTES_SEDIM_NLAT - 1) / 2; // from +90.0 to -90.0
  quantized_latitude = ::std::abs(quantized_latitude);

  double quantized_longitude = (double) coords.getLongitude() / DECK41_MINUTES_SEDIM_SPACING; 
  if ( (::std::abs(quantized_longitude) - floor(::std::abs(quantized_longitude))) >= 0.5) 
      quantized_longitude = ceil(quantized_longitude);
  else quantized_longitude = floor(quantized_longitude);
  quantized_longitude += (DECK41_MINUTES_SEDIM_NLON - 1) / 2; // from -180.0 to 180.0

  return((int)quantized_latitude * DECK41_MINUTES_SEDIM_NLON + (int)quantized_longitude);
}

bool SedimDeck41CoordDb::finalizeConnection() {
#if defined (WOSS_NETCDF4_SUPPORT)
  main_sedim_var_coord = netcdf_db->getVar("seafloor_main_type");
  if (main_sedim_var_coord.isNull()) {
    ::std::cout << "SedimDeck41CoordDb::finalizeConnection() main_sedim_var_coord is not valid" << ::std::endl;
    return false;
  }
  sec_sedim_var_coord = netcdf_db->getVar("seafloor_secondary_type");
  if (sec_sedim_var_coord.isNull()) {
    ::std::cout << "SedimDeck41CoordDb::finalizeConnection() sec_sedim_var_coord is not valid" << ::std::endl;
    return false;
  }
  return true;
#else
  main_sedim_var_coord = netcdf_db->get_var("seafloor_main_type");
  sec_sedim_var_coord = netcdf_db->get_var("seafloor_secondary_type");
  return ( (main_sedim_var_coord != 0) && (sec_sedim_var_coord != 0) );
#endif // defined (WOSS_NETCDF4_SUPPORT)
}

Deck41Types SedimDeck41CoordDb::getSeaFloorType( const Coord& coordinates ) const {
  int index = getSedimIndex(coordinates);
  int main_type = SEDIMENT_NOT_FOUND;
  int secondary_type = SEDIMENT_NOT_FOUND;

#if defined (WOSS_NETCDF4_SUPPORT)
  ::std::vector<size_t> index_vector(1, index);

  main_sedim_var_coord.getVar(index_vector, &main_type);
  if (main_type == SEDIMENT_NOT_FOUND) {
    ::std::cout << "SedimDeck41CoordDb::getSeaFloorType() Couldn't extract current main_type" << ::std::endl;
    exit(1);
  }

  sec_sedim_var_coord.getVar(index_vector, &secondary_type);
  if (secondary_type == SEDIMENT_NOT_FOUND) {
    ::std::cout << "SedimDeck41CoordDb::getSeaFloorType() Couldn't extract current secondary_type" << ::std::endl;
    exit(1);
  }
#else
  NcBool ret_val = main_sedim_var_coord->set_cur(index);
  if (!ret_val) {
    ::std::cout << "SedimDeck41CoordDb::getSeaFloorType() Couldn't set_cur() of main_sedim_var_coord;" 
	      << " current index = " << index << ::std::endl;
    exit(1);
  }
  
  ret_val = sec_sedim_var_coord->set_cur(index);
  if (!ret_val) {
    ::std::cout << "SedimDeck41CoordDb::getSeaFloorType() Couldn't set_cur() of sec_sedim_var_coord;"
	      << " current index = " << index << ::std::endl;
    exit(1);
  }

  ret_val = main_sedim_var_coord->get(&main_type,1);
  if (!ret_val) {
    ::std::cout << "SedimDeck41CoordDb::getSeaFloorType() Couldn't extract current main_type" << ::std::endl;
    exit(1);
  }
  
  ret_val = sec_sedim_var_coord->get(&secondary_type,1);
  if (!ret_val) {
    ::std::cout << "SedimDeck41CoordDb::getSeaFloorType() Couldn't extract current secondary_type" << ::std::endl;
    exit(1);
  }
#endif // defined (WOSS_NETCDF4_SUPPORT)

  if (debug) ::std::cout << "SedimDeck41CoordDb::getSeaFloorType() lat = " << coordinates.getLatitude() << "; long = " 
		        << coordinates.getLongitude() << "; main type = " << main_type 
		        << "; secondary type = " << secondary_type << ::std::endl;

  return( ::std::make_pair( main_type, secondary_type ) );     
}

#endif // WOSS_NETCDF_SUPPORT




