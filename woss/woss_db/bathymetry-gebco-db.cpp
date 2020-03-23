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


#define GEBCO_NOT_FOUND (-HUGE_VAL)


using namespace woss;


BathyGebcoDb::BathyGebcoDb( const ::std::string& name ) 
: WossNetcdfDb(name),
  gebco_type(GEBCO_SECONDS_BATHY_TYPE),
  bathy_var(NULL)
{ 

}


bool BathyGebcoDb::finalizeConnection() {
  bathy_var = netcdf_db->get_var( "z" );
  return (bathy_var != 0);
}


bool BathyGebcoDb::insertValue( const Coord& coordinates, const Bathymetry& bathymetry_value ) {
  ::std::cerr << "BathyGebcoDb::insertValue() method is not implemented." << ::std::endl;
  return false;
}


double BathyGebcoDb::getValue( const Coord& coords ) const {
  double depth = GEBCO_NOT_FOUND;
  int index = getBathyIndex(coords);

  NcBool ret_val = bathy_var->set_cur(index);
  if (!ret_val) {
     ::std::cout << "BathyGebcoDb::getValue() Couldn't set_cur() current index = " << index << ::std::endl;
     exit(1);
  }
  
  ret_val = bathy_var->get(&depth,1);
  if (!ret_val) {
     ::std::cout << "BathyGebcoDb::getValue() Couldn't extract current depth" << ::std::endl;
     exit(1);
  }

  if ( depth != GEBCO_NOT_FOUND && depth <= 0.0 ) return(::std::abs(depth));
  else {
    ::std::cout << "BathyGebcoDb::getValue() WARNING current coordinates are on land : " << coords << "; altitude = " << depth << ::std::endl;
    return (HUGE_VAL);
  }
}
 

int BathyGebcoDb::getBathyIndex( const Coord& coords ) const {
  if (gebco_type == GEBCO_SECONDS_BATHY_TYPE) {

    double quantized_latitude = ::std::abs( ( coords.getLatitude() - GEBCO_SECONDS_BATHY_START_LAT ) / GEBCO_SECONDS_BATHY_SPACING ); 
    if ( quantized_latitude - floor(quantized_latitude)  >= 0.5 ) quantized_latitude = ceil(quantized_latitude); 
    else quantized_latitude = floor(quantized_latitude);

    assert(quantized_latitude >= 0 && quantized_latitude <= GEBCO_SECONDS_BATHY_NLAT);
    
    double quantized_longitude = abs( ( coords.getLongitude() - GEBCO_SECONDS_BATHY_START_LONG ) / GEBCO_SECONDS_BATHY_SPACING ); 
    if ( quantized_longitude - floor(quantized_longitude)  >= 0.5 ) quantized_longitude = ceil(quantized_longitude);
    else quantized_longitude = floor(quantized_longitude);

    assert(quantized_longitude >= 0 && quantized_longitude <= GEBCO_SECONDS_BATHY_NLON);

    return( (int)(quantized_latitude * GEBCO_SECONDS_BATHY_NLON) + (int)quantized_longitude );
  }
  else {
    double quantized_latitude = ::std::abs( coords.getLatitude()  / GEBCO_MINUTES_BATHY_SPACING ); 
    if ( ( quantized_latitude - floor(quantized_latitude) ) >= 0.5) quantized_latitude = ceil(quantized_latitude); 
    else quantized_latitude = floor(quantized_latitude);
    quantized_latitude -= (GEBCO_MINUTES_BATHY_NLAT - 1) / 2; // from +90.0 to -90.0

    assert(quantized_latitude >= 0 && quantized_latitude <= GEBCO_MINUTES_BATHY_NLAT);

    double quantized_longitude = ::std::abs( coords.getLongitude()  / GEBCO_MINUTES_BATHY_SPACING ); 
    if ( ( quantized_longitude - floor(quantized_longitude) ) >= 0.5) quantized_longitude = ceil(quantized_longitude);
    else quantized_longitude = floor(quantized_longitude);
    quantized_longitude += (GEBCO_MINUTES_BATHY_NLON - 1) / 2; // from -180.0 to 180.0

    assert(quantized_longitude >= 0 && quantized_longitude <= GEBCO_MINUTES_BATHY_NLON);

    return( (int)(quantized_latitude * GEBCO_MINUTES_BATHY_NLON) + (int)quantized_longitude );
  }
}

#endif // WOSS_NETCDF_SUPPORT

