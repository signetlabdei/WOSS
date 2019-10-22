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
 * @file   bathymetry-umt-csv-db-creator.h
 * @author Alberto Signori, Federico Guerra
 * 
 * \brief Provides the interface for woss::BathyUtmCsvDbCreator class
 *
 * Provides the interface for the woss::BathyUtmCsvDbCreator class
 */


#ifndef WOSS_BATHYMETRY_UMT_CSV_DB_CREATOR_H 
#define WOSS_BATHYMETRY_UMT_CSV_DB_CREATOR_H

#include <utility>
#include "woss-db-creator.h"


namespace woss {
  

  /**
  * \brief WossDbCreator for the UMT CSV bathymetry database
  *
  * Specialization of WossDbCreator for the UMT CSV bathymetry database.
  * 
  **/
  class BathyUtmCsvDbCreator : public WossDbCreator {

    
    public:
    

    /**
    * Default BathyUtmCsvDbCreator constructor
    **/
    BathyUtmCsvDbCreator();
    
    virtual ~BathyUtmCsvDbCreator();
      
    
    /**
    * Creates and initialize a BathyUtmCsvDb object
    * @return a pointer to a properly initialized BathyUtmCsvDb object
    **/
    virtual WossDb* const createWossDb();

   /**
    * Sets the char separator used in CSV textual db
    * 
    * @param new_separator separator char
    * @return reference to <b>*this</b>
    **/
    BathyUtmCsvDbCreator& setCSVSeparator( const char new_separator ) { separator = new_separator; return *this; }
    
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
    * @return reference to <b>*this</b>
    **/
    BathyUtmCsvDbCreator& setDbSpacing( double spacing ) { db_spacing = spacing; return *this; }
    
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
    * @return reference to <b>*this</b>
    **/
    BathyUtmCsvDbCreator& setDbTotalValues(int nnorth, int neast) { 
      total_northing_values = nnorth; 
      total_easting_values = neast;
      return *this;
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
    * @return reference to <b>*this</b>
    **/
    BathyUtmCsvDbCreator& setDbRangeEasting(double start, double end) {
      range_easting_start = start;
      range_easting_end = end;
      return *this;
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
    * @return reference to <b>*this</b>
    **/
    BathyUtmCsvDbCreator& setDbRangeNorthing(double start, double end) {
      range_northing_start = start;
      range_northing_end = end;
      return *this;
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
    * @return reference to <b>*this</b>
    **/
    BathyUtmCsvDbCreator& setLandApproximationFlag(bool flag) {
      approx_land_to_sea_surface = flag;
      return *this;
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

    char separator; /**< Separator used in the csv file */

    double db_spacing; /**< spatial spacing of the db, in meters */
    
    int total_northing_values; /**< Separator used in the csv file */
    
    int total_easting_values; /**< Separator used in the csv file */
    
    double range_easting_start; /**< Separator used in the csv file */
    
    double range_easting_end; /**< Separator used in the csv file */
    
    double range_northing_start; /**< Separator used in the csv file */
    
    double range_northing_end; /**< Separator used in the csv file */ 

    bool approx_land_to_sea_surface; /**< Approximate land db points to sea surface */
    
    /**
    * Initializes the pointed object
    * @param woss_db pointer to a recently created BathyUtmCsvDb
    * @return <i>true</i> if the method succeed, <i>false</i> otherwise
    **/
    virtual bool initializeDb( WossDb* const woss_db );

    
  };

}


#endif /* WOSS_BATHYMETRY_UMT_CSV_DB_CREATOR_H */

