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
 * @file   bathymetry-hamburg-port-db.h
 * @author Alberto Signori, Federico Guerra
 * 
 * \brief Provides the interface for woss::BathyUtmCsvDb class
 *
 * Provides the interface for the woss::BathyUtmCsvDb class
 */


#ifndef WOSS_BATHYMETRY_UTM_CSV_DB_H 
#define WOSS_BATHYMETRY_UTM_CSV_DB_H

#include "woss-db.h"
#include <vector>
#include <utility>

namespace woss {

 
  /**
  * \brief Specialization of WossTextualDb for UTM CSV database
  *
  * Specialization of WossTextualDb for UTM CSV database. 
  * It creates a vector used to  get requested bathymetry values.
  **/
  class BathyUtmCsvDb : public WossTextualDb, public WossBathymetryDb {    
    public:

    /**
    * BathyUtmCsvDb constructor
    * 
    * @param name pathname of database
    **/
    BathyUtmCsvDb( const ::std::string& name );

    virtual ~BathyUtmCsvDb() { }


    /**
    * Inserts the given woss::Bathymetry value in the database for given coordinates
    * 
    * @param coordinates const reference to a valid Coord object
    * @param bathymetry_value const reference to woss::Bathymetry value to be inserted
    * @returns <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool insertValue( const Coord& coordinates, const Bathymetry& bathymetry_value );

    /**
    * Returns the positive depth value ( bathymetry ) of given coordinates, if present in the database.
    * If given coordinates are on land ( original retrieved value is positive ) HUGE_VAL is returned.
    * 
    * @param coords const reference to a valid Coord object
    * @return <i>positive</i> depth value [m] if coordinates are found, <i>HUGE_VAL</i> otherwise
    **/
    virtual double getValue( const Coord& coords ) const ;

    
    /**
    * Post openConnection() actions. It create and initializes a NetCDF variable
    * 
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool finalizeConnection();

    /**
    * Sets the char separator used in CSV textual db
    * 
    * @param new_separator separator char
    **/
    void setCSVSeparator( const char new_separator ) { separator = new_separator; }
    
    /**
    * Returns the current char separator
    * 
    * @return the char separator
    **/
    const char getCSVSeparator() const { return separator; }
    
  /**
    * Sets db resolution, in meters
    * 
    * @param spacing space resolution in meters
    **/
    void setDbSpacing( double spacing ) { db_spacing = spacing; }
    
   /**
    * Returns the current db spatial resolution
    * 
    * @return the db spatial resolution in meters
    **/
    double getDbSpacing() const { return db_spacing; }

    /**
    * Sets the db total northing and easting values
    * 
    * @param nnorth db total northing points. should be >= 0
    * @param neast db total easting points. should be >= 0
    **/
    void setDbTotalValues(int nnorth, int neast) { 
      total_northing_values = nnorth; 
      total_easting_values = neast;
    }

    /**
    * Returns the total northing and easting values
    * 
    * @return pair of total northing and easting values
    **/
    std::pair<int, int> getDbTotalValues() const {
      return std::make_pair(total_northing_values, total_easting_values);
    }
    
    /**
    * Sets db range easting start and end
    * 
    * @param start easting range start
    * @param end easting range end
    **/
    void setDbRangeEasting(double start, double end) {
      range_easting_start = start;
      range_easting_end = end;
    }
    
    /**
    * Returns the current db range easting
    * 
    * @return the db easting range start and end
    **/
    std::pair<double, double> getDbRangeEasting() const {
      return std::make_pair(range_easting_start, range_easting_end); 
    }
    
    /**
    * Sets db northing range start and end
    * 
    * @param start northing range start
    * @param end northing range end
    **/
    void setDbRangeNorthing(double start, double end) {
      range_northing_start = start;
      range_northing_end = end;
    }

    /**
    * Returns the current db range northing
    * 
    * @return the db easting range start and end
    **/
    std::pair<double, double> getDbRangeNorthing() const {
      return std::make_pair( range_northing_start, range_northing_end);
    }

    /** Sets land approximation flag
    * 
    * @param flag land approximation flag
    **/
    void setLandApproximationFlag(bool flag) {
      approx_land_to_sea_surface = flag;
    }

    /**
    * Returns the current land approximation flag
    * 
    * @return the land approximation flag
    **/
    bool getLandApproximationFlag() {
      return approx_land_to_sea_surface;
    }
    
    protected:

    /**
    * Returns the index corresponding the given coordinates. This index will be used to access the vector variable
    * and thus retrieving the bathymetry value
    * 
    * @param coords const reference to a valid Coord object
    * @return index value 
    **/
    int getBathyIndex( const Coord& coords ) const ;

    /**
     * Imports data from .csv file and store depth values in the bathy_vec 
     * 
     * @returns <i>true</i> if operation succeeds, <i>false</i> otherwise
     */
    virtual bool importData();


    std::vector<double> bathy_vec; /**< Vector with bathymetry values*/

    char separator; /**< Separator used in the csv file */

    double db_spacing; /**< spatial spacing of the db, in meters */
    
    int total_northing_values; /**< Separator used in the csv file */
    
    int total_easting_values; /**< Separator used in the csv file */
    
    double range_easting_start; /**< Separator used in the csv file */
    
    double range_easting_end; /**< Separator used in the csv file */
    
    double range_northing_start; /**< Separator used in the csv file */
    
    double range_northing_end; /**< Separator used in the csv file */ 

    bool approx_land_to_sea_surface; /**< Approximate land db points to sea surface */

    static const double land_approximation_depth; /**< Depth value for land approximation */
  };

}

#endif /* WOSS_BATHYMETRY_UTM_CSV_DB_H */


 
