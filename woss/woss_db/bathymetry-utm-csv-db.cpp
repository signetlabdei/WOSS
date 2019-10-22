/*
 * WOSS - World Ocean Simulation System -
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

#include <cassert>
#include <cstdlib>
#include <cmath>
#include <definitions.h>
#include "bathymetry-utm-csv-db.h"
#include <sstream>

using namespace woss;

const double BathyUtmCsvDb::land_approximation_depth = 0.000000001;

BathyUtmCsvDb::BathyUtmCsvDb( const ::std::string& name ) 
: WossTextualDb(name),
  bathy_vec(),
  separator(','),
  db_spacing(1.0),
  total_northing_values(0),
  total_easting_values(0),
  range_easting_start(0.0),
  range_easting_end(0.0),
  range_northing_start(0.0),
  range_northing_end(0.0),
  approx_land_to_sea_surface(false)
{

}


bool BathyUtmCsvDb::finalizeConnection() {
  return importData();
}


bool BathyUtmCsvDb::insertValue( const Coord& coordinates, const Bathymetry& bathymetry_value ) {
  ::std::cerr << "BathyUtmCsvDb::insertValue() method is not implemented." << ::std::endl;
  return false;
}


double BathyUtmCsvDb::getValue( const Coord& coords ) const {
  double depth = HUGE_VAL;
  int index = getBathyIndex(coords);

  if (index < 0 || index >= (int)bathy_vec.size()) {
    ::std::cerr << "BathyUtmCsvDb::getValue() WARNING current coordinates"
      << " are outside CSV bathymetry: " << coords  << ::std::endl;
    return (HUGE_VAL);
  }
  depth = bathy_vec[index];
  //std::cout << "Position " << coords << " depth= " << depth << std::endl;
  if ( depth != HUGE_VAL && depth <= 0.0 ) 
    return(::std::abs(depth));
  else {
    if  (debug)
      ::std::cout << "BathyUtmCsvDb::getValue() WARNING current"
                  << " coordinates are on land : " << coords << "; altitude = " 
                  << depth << ::std::endl;
    if (true == approx_land_to_sea_surface) {
      return land_approximation_depth;
    }
    else {
      return HUGE_VAL;
    }
  }
}
 

int BathyUtmCsvDb::getBathyIndex( const Coord& coords ) const {
  UtmWgs84 utm = UtmWgs84::getUtmWgs84FromCoord(coords);

  if (debug)
    std::cout << "utm " << utm << std::endl;

  if (debug)
    std::cout << "range_easting_start = " << range_easting_start
              << " range_northing_start = " << range_northing_start
              << " range_easting_end = " << range_easting_end
              << " range_northing_end = " << range_northing_end
              << std::endl;

              
  if (utm.isValid() == false)
  {
    std::cerr << "UTM not valid" << std::endl;
    return -1;
  }

  if (utm.getEasting() < range_easting_start || 
      utm.getNorthing() < range_northing_start || 
      utm.getEasting() > range_easting_end ||
      utm.getNorthing() > range_northing_end)
  {
    std::cerr << "UTM out of db ranges" << std::endl;

    return -1;
  }

  double x_index = std::floor((utm.getEasting() - range_easting_start)/db_spacing);
  double y_index = std::floor((utm.getNorthing() - range_northing_start)/db_spacing);

  if (debug)
    std::cout << "x_index = " << x_index << " y_index = " << y_index << std::endl;

  return (int)(y_index*total_easting_values) + (int)x_index;
}


bool BathyUtmCsvDb::importData() {
  textual_db.close();
  textual_db.open( db_name.c_str() , ::std::fstream::in );
  if (textual_db.is_open()) {
    if (debug) 
      std::cout << "file opened" << std::endl;

    std::string line;
    int i = 0;
    int j = 0;
    int n = 0;
    while (std::getline(textual_db,line)) {
      j = 0;
      std::stringstream ss(line);
      std::string number;
      while(std::getline(ss,number,separator)) {
        if (debug) 
          std::cout << "i = " << i << " j = " << j 
                    << " n = " << n << " number = " << atof(number.c_str()) << std::endl;

        bathy_vec.push_back(atof(number.c_str()));
        j++;
        n++;
      }
        i++;
    }

    if (debug) 
      std::cout << "number of rows = " << i << " number of columns = " << j << std::endl
                << "size of vector = " << bathy_vec.size() << " number of elements inserted = " << n << std::endl;

    textual_db.close();
    return true;
  } else {
    std::cerr << "file not opened" << std::endl;
    return false;
  }

}
