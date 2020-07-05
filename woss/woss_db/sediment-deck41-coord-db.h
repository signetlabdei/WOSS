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
 * @file   sediment-deck41-coord-db.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::SedimDeck41CoordDb class
 *
 * Provides the interface for the woss::SedimDeck41CoordDb class
 */


#ifndef WOSS_SEDIMENT_DECK41_COORD_DB_H 
#define WOSS_SEDIMENT_DECK41_COORD_DB_H


#ifdef WOSS_NETCDF_SUPPORT

#include <sediment-definitions.h>
#include "woss-db.h"
#if defined (WOSS_NETCDF4_SUPPORT)
#include <ncVar.h>
#endif // defined (WOSS_NETCDF4_SUPPORT)

namespace woss {

    
  static const int DECK41_MINUTES_SEDIM_NLAT = 10801; /**< Custom made NetCDF DECK41 total number of latitudes */

  static const int DECK41_MINUTES_SEDIM_NLON = 21601; /**< Custom made NetCDF DECK41 total number of longitudes */

  static const double DECK41_MINUTES_SEDIM_SPACING = 0.0166666666666667; /**< Spacing between coordinates value [decimal degree] */

  static const int DECK41_V2_SEDIM_NLAT = 10800; /**< Custom made NetCDF DECK41 total number of latitudes */

  static const int DECK41_V2_SEDIM_NLON = 21600; /**< Custom made NetCDF DECK41 total number of longitudes */

  static const double DECK41_V2_SEDIM_MIN_LAT = -89.9833333333333333; /**< Custom made NetCDF DECK41 mimimum latitude */

  static const double DECK41_V2_SEDIM_MAX_LAT = 89.9833333333333333; /**< Custom made NetCDF DECK41 maximum latitude */

  static const double DECK41_V2_SEDIM_MIN_LON = -179.9833333333333333; /**< Custom made NetCDF DECK41 mimimum longitude */

  static const double DECK41_V2_SEDIM_MAX_LON = 179.9833333333333333; /**< Custom made NetCDF DECK41 maximum longitude */

  static const double DECK41_V2_SEDIM_START_LAT = -89.9833333333333333; /**< Custom made NetCDF DECK41 start latitude */

  static const double DECK41_V2_SEDIM_START_LON = -179.9833333333333333; /**< Custom made NetCDF DECK41 start longitude */

  static const double DECK41_V2_SEDIM_SPACING = 0.0166666666666667; /**< Spacing between coordinates value [decimal degree] */

  /**
  * DECK41 db version in use
  **/
  enum DECK41DbType {
    DECK41_DB_V1_TYPE       = 0,  ///< DECK41 V1 NetCDF Legacy db
    DECK41_DB_V2_TYPE       = 1,  ///< DECK41 V2 NetCDF4 db
    DECK41_DB_INVALID_TYPE        ///< INVALID, must be last
  };

  
  /**
  * \brief WossDb for custom made NetCDF DECK41 Sediment database
  *
  * SedimDeck41Db implements WossNetcdfDb for the custom made NetCDF DECK41 Sediment database
  **/
  class SedimDeck41CoordDb : public WossNetcdfDb {


    public:

    /**
    * SedimDeck41CoordDb default constructor. Default constructed object are not valid
    * @param name pathname of database
    **/
    SedimDeck41CoordDb( const ::std::string& name = DB_NAME_NOT_SET );

#if defined (WOSS_NETCDF4_SUPPORT)
    /**
    * SedimDeck41CoordDb default constructor. Default constructed object are not valid
    * @param name pathname of database
    * @param db_type DECK41 db type
    **/
    SedimDeck41CoordDb( const ::std::string& name, DECK41DbType db_type );
#endif // defined (WOSS_NETCDF4_SUPPORT)

    virtual ~SedimDeck41CoordDb() { }

    /**
    * Returns a Deck41Types for the given coordinates
    * @param coordinates const reference to a valid Coord object
    * @returns a Deck41Types value ( main type value, secondary type value )
    **/
    Deck41Types getSeaFloorType( const Coord& coordinates ) const ;


    /**
    * Post openConnection() actions, used to create and initialize NetCDF variables
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool finalizeConnection();

    
    /**
    * Checks the validity of SedimDeck41CoordDb
    * @return <i>true</i> if pathname is valid, <i>false</i> otherwise
    **/
    virtual bool isValid() { return( WossNetcdfDb::isValid() 
                                    && db_name != DB_NAME_NOT_SET && deck41_db_type != DECK41_DB_INVALID_TYPE ); }

#if defined (WOSS_NETCDF4_SUPPORT)
    /**
    * Sets the current DECK41 Db type
    * @param db_type DECK41DbType 
    **/
    void setDeck41DbType( DECK41DbType db_type ) { deck41_db_type = db_type; }
#endif // defined (WOSS_NETCDF4_SUPPORT)

    /**
     * Returns the current DECK41 DB type
     * @returns current deck41 db type
     */
    DECK41DbType getDeck41DbType() const { return deck41_db_type; }

    
    protected:


    /**
    * NetCDF variable representing main DECK41 floortype 
    **/
#if defined (WOSS_NETCDF4_SUPPORT)
    netCDF::NcVar main_sedim_var_coord;
#else
    NcVar* main_sedim_var_coord;
#endif // 
    
    /**
    * NetCDF variable representing secondary DECK41 floortype 
    **/
#if defined (WOSS_NETCDF4_SUPPORT)
    netCDF::NcVar sec_sedim_var_coord;
#else
    NcVar* sec_sedim_var_coord;
#endif // defined (WOSS_NETCDF4_SUPPORT)

    /**
    * NetCDF latitude variable
    **/
#if defined(WOSS_NETCDF4_SUPPORT)
    netCDF::NcVar lat_var;
#else
    NcVar* lat_var;
#endif // defined(WOSS_NETCDF4_SUPPORT)

    /**
    * NetCDF longitude variable
    **/
#if defined(WOSS_NETCDF4_SUPPORT)
    netCDF::NcVar lon_var;
#else
    NcVar* lon_var;
#endif // defined(WOSS_NETCDF4_SUPPORT)

    /**
     * DECK41 database type
     */
    DECK41DbType deck41_db_type;

    /**
    * Returns the index used by a NetCDF variable to get the DECK41 floortype integer value
    * @param coordinates const reference to a valid Coord object
    * @returns index value
    **/
    int getSedimIndex( const Coord& coords ) const ;

#if defined (WOSS_NETCDF4_SUPPORT)
   /**
    * Returns the index pair used by a NetCDF variable to get the DECK41 floortype integer value
    * @param coordinates const reference to a valid Coord object
    * @returns index pair
    **/
    ::std::pair< int, int > getSedimIndexes( const Coord& coordinates ) const;
#endif // defined (WOSS_NETCDF4_SUPPORT)
  };


}

#endif // WOSS_NETCDF_SUPPORT

#endif /* WOSS_SEDIMENT_DECK41_COORD_DB_H */

  
 
