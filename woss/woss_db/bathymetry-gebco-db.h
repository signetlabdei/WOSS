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
 * @file   bathymetry-gebco-db.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::BathyGebcoDb class
 *
 * Provides the interface for the woss::BathyGebcoDb class
 */


#ifndef WOSS_BATHYMETRY_GEBCO_DB_H 
#define WOSS_BATHYMETRY_GEBCO_DB_H

#ifdef WOSS_NETCDF_SUPPORT

#include "woss-db.h"

namespace woss {


  static const double GEBCO_BATHY_MIN_LAT = -90.0;      /**< GEBCO One minute NetCDF mimimum latitude value [decimal degree] */

  static const double GEBCO_BATHY_MAX_LAT = 90.0;       /**< GEBCO One minute NetCDF maximum latitude value [decimal degree] */

  static const double GEBCO_BATHY_MIN_LONG = -180.0;    /**< GEBCO One minute NetCDF mimimum longitude value [decimal degree] */

  static const double GEBCO_BATHY_MAX_LONG = 180.0;     /**< GEBCO One minute NetCDF maximum longitude value [decimal degree] */


  static const double GEBCO_BATHY_START_LAT = 90.0;     /**< GEBCO One minute NetCDF starting latitude value [decimal degree] */

  static const double GEBCO_BATHY_START_LONG = -180.0;  /**< GEBCO One minute NetCDF starting longitude value [decimal degree] */


  static const int GEBCO_MINUTES_BATHY_NLAT = 10801; /**< GEBCO One minute NetCDF total number of latitudes */

  static const int GEBCO_MINUTES_BATHY_NLON = 21601; /**< GEBCO One minute NetCDF total number of longitudes */

  static const double GEBCO_MINUTES_BATHY_SPACING = 0.0166666666666667;   /**< GEBCO One minute NetCDF spacing between coordinates value [decimal degree] */


  static const int GEBCO_SECONDS_BATHY_NLAT = 21600;                      /**< GEBCO Thirty seconds NetCDF total number of latitudes */

  static const int GEBCO_SECONDS_BATHY_NLON = 43200;                      /**< GEBCO Thirty seconds NetCDF total number of longitudes */

  static const double GEBCO_SECONDS_BATHY_SPACING = 0.00833333333333333;  /**< GEBCO Thirty seconds NetCDF spacing between coordinates value [decimal degree] */


  static const double GEBCO_SECONDS_BATHY_MIN_LAT = -89.995833333333333335;     /**< GEBCO Thirty seconds NetCDF mimimum latitude value [decimal degree] */

  static const double GEBCO_SECONDS_BATHY_MAX_LAT = 89.995833333333333335;      /**< GEBCO Thirty seconds NetCDF maximum latitude value [decimal degree] */

  static const double GEBCO_SECONDS_BATHY_MIN_LONG = -179.995833333333333335;   /**< GEBCO Thirty seconds NetCDF mimimum longitude value [decimal degree] */

  static const double GEBCO_SECONDS_BATHY_MAX_LONG = 179.995833333333333335;    /**< GEBCO Thirty seconds NetCDF maximum longitude value [decimal degree] */

  static const double GEBCO_SECONDS_BATHY_START_LAT = 89.995833333333333335;    /**< GEBCO Thirty seconds NetCDF start latitude value [decimal degree] */

  static const double GEBCO_SECONDS_BATHY_START_LONG = -179.995833333333333335; /**< GEBCO Thirty seconds NetCDF start longitude value [decimal degree] */


  /**
  * GEBCO version in use
  **/
  enum GEBCO_BATHY_TYPE {
    GEBCO_MINUTES_BATHY_TYPE = 1, GEBCO_SECONDS_BATHY_TYPE
  };


  /**
  * \brief NetCDF specialization of WossNetcdfDb for GEBCO database
  *
  * NetCDF specialization of WossNetcdfDb for GEBCO database. It creates a NetCDF variable used to get requested bathymetry 
  * values
  **/
  class BathyGebcoDb : public WossNetcdfDb, public WossBathymetryDb {

    
    public:


    /**
    * BathyGebcoDb constructor
    * @param name pathname of database
    **/
    BathyGebcoDb( const ::std::string& name );

    virtual ~BathyGebcoDb() { }

  
    /**
    * Inserts the given woss::Bathymetry value in the database for given coordinates
    * @param coordinates const reference to a valid Coord object
    * @param bathymetry_value const reference to woss::Bathymetry value to be inserted
    * @returns <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool insertValue( const Coord& coordinates, const Bathymetry& bathymetry_value );
    
    
    /**
    * Returns the positive depth value ( bathymetry ) of given coordinates, if present in the database.
    * If given coordinates are on land ( original retrieved value is positive ) HUGE_VAL is returned.
    * @param coords const reference to a valid Coord object
    * @return <i>positive</i> depth value [m] if coordinates are found, <i>HUGE_VAL</i> otherwise
    **/
    virtual double getValue( const Coord& coords ) const ;

    
    /**
    * Post openConnection() actions. It create and initializes a NetCDF variable
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool finalizeConnection();


    /**
    * Notify the database which GEBCO version is in use
    * @param type GEBCO_BATHY_TYPE instance
    **/
    void setGebcoType( const GEBCO_BATHY_TYPE& type ) { gebco_type = type; }
    
    /**
    * Returns which GEBCO version is in use
    * #return GEBCO_BATHY_TYPE instance
    **/
    GEBCO_BATHY_TYPE getGebcoType() { return gebco_type; }


    protected:


    /**
    * GEBCO version in use
    **/
    GEBCO_BATHY_TYPE gebco_type;

    
    /**
    * NetCDF bathymetry variable
    **/
    NcVar* bathy_var;

    
    /**
    * Returns the GEBCO index corresponding the given coordinates. This index will be used to access the NetCDF variable
    * and thus retrieving the bathymetry value
    * @param coords const reference to a valid Coord object
    * @return index value 
    **/
    int getBathyIndex( const Coord& coords ) const ;

    
  };

}

#endif // WOSS_NETCDF_SUPPORT

#endif /* WOSS_BATHYMETRY_GEBCO_DB_H */


 
