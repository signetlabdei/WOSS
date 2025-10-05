/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2009 2025 Federico Guerra
 * and regents of the SIGNET lab, University of Padova
 *
 * Author: Federico Guerra - WOSS@guerra-tlc.com
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

 
#ifndef WOSS_SSP_NETCDF_DB_H 
#define WOSS_SSP_NETCDF_DB_H


/**
 * @file   ssp-woa2005-db.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::SspWoa2005Db class
 *
 * Provides the interface for the woss::SspWoa2005Db class
 */


#ifdef WOSS_NETCDF_SUPPORT

#include <utility>
#include <ssp-definitions.h>
#include "woss-db.h"
#if defined (WOSS_NETCDF4_SUPPORT)
#include <ncVar.h>
#endif // defined (WOSS_NETCDF4_SUPPORT)

namespace woss {
  
  static constexpr inline int SSP_WOA2005_STD_NLAT = 180; /**< Custom made NetCDF WOA2005 SSP total number of latitudes */

  static constexpr inline int SSP_WOA2005_STD_NLON = 360; /**< Custom made NetCDF WOA2005 SSP total number of longitudes */

  static constexpr inline double SSP_WOA2005_STD_SPACING = 1.0; /**< Spacing between coordinates value [decimal degree] */

  static constexpr inline double SSP_WOA2005_STD_MIN_LAT = -89.5; /**< Custom made NetCDF WOA2005 SSP mimimum latitude */

  static constexpr inline double SSP_WOA2005_STD_MAX_LAT = 89.5; /**< Custom made NetCDF WOA2005 SSP maximum latitude */

  static constexpr inline double SSP_WOA2005_STD_MIN_LON = -179.5; /**< Custom made NetCDF WOA2005 SSP mimimum longitude */

  static constexpr inline double SSP_WOA2005_STD_MAX_LON = 179.5; /**< Custom made NetCDF WOA2005 SSP maximum longitude */

  static constexpr inline double SSP_WOA2005_STD_START_LAT = 89.5; /**< Custom made NetCDF WOA2005 SSP start latitude */

  static constexpr inline double SSP_WOA2005_STD_START_LON = -179.5; /**< Custom made NetCDF WOA2005 SSP start longitude */

  static constexpr inline int SSP_WOA2013_STD_NLAT = 720; /**< Custom made NetCDF WOA2013 SSP total number of latitudes */

  static constexpr inline int SSP_WOA2013_STD_NLON = 1440; /**< Custom made NetCDF WOA2013 SSP total number of longitudes */

  static constexpr inline double SSP_WOA2013_STD_SPACING = 0.25; /**< Spacing between coordinates value [decimal degree] */

  static constexpr inline double SSP_WOA2013_STD_MIN_LAT = -89.875; /**< Custom made NetCDF WOA2013 SSP mimimum latitude */

  static constexpr inline double SSP_WOA2013_STD_MAX_LAT = 89.875; /**< Custom made NetCDF WOA2013 SSP maximum latitude */

  static constexpr inline double SSP_WOA2013_STD_MIN_LON = -179.875; /**< Custom made NetCDF WOA2013 SSP mimimum longitude */

  static constexpr inline double SSP_WOA2013_STD_MAX_LON = 179.875; /**< Custom made NetCDF WOA2013 SSP maximum longitude */

  static constexpr inline double SSP_WOA2013_STD_START_LAT = -89.875; /**< Custom made NetCDF WOA2013 SSP start latitude */

  static constexpr inline double SSP_WOA2013_STD_START_LON = -179.875; /**< Custom made NetCDF WOA2013 SSP start longitude */

  static constexpr inline int SSP_STD_NDEPTH = 33;  /**< Custom made NetCDF SSP total number of depth of a SSP */

  /**
  * Standard depths of any created SSP
  **/
  static constexpr inline short ssp_std_depths[SSP_STD_NDEPTH] = { 0, 10, 20, 30, 50, 75, 100, 125, 150, 200, 250, 300,
                                                                   400, 500, 600, 700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500,
                                                                   1750, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500 };

  /**
  * Pair representing latitute and longitude index for NetCDF value access
  **/
  using SSPIndexes = std::pair< int, int >;

  enum class WOADbType {
    WOA_DB_TYPE_2005 = 0, ///< 2005 and 2009 NetCDF Db type, 1 degree resolution
    WOA_DB_TYPE_2013 = 1, ///< 2013, 2001, 2018 and 2023 NetCDF4 Db type, 0.25 degree resolution
    WOA_DB_TYPE_INVALID ///< Must always be the last
  };

  /**
  * \brief WossDb for the custom made NetCDF WOA2005 SSP database
  *
  * WossDb for the custom made NetCDF WOA2005 SSP database
  */
  class SspWoa2005Db : public WossNetcdfDb, public WossSSPDb {

    public:

    /**
    * SspWoa2005Db constructor
    * @param name pathname of database
    **/
    SspWoa2005Db( const std::string& name );

#if defined (WOSS_NETCDF4_SUPPORT)
    /**
    * SspWoa2005Db constructor
    * @param name pathname of database
    * @param db_type WOADbType of the database
    **/
    SspWoa2005Db( const std::string& name, WOADbType db_type );
#endif // defined (WOSS_NETCDF4_SUPPORT)
    
    virtual ~SspWoa2005Db() override = default;


    /**
    * Post openConnection() actions, used to create and initialize NetCDF variables
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool finalizeConnection() override;

    /**
    * Inserts the given woss::SSP value in the database for given coordinates
    * @param coordinates const reference to a valid Coord object
    * @param time_value const reference to a valid Time object
    * @param ssp_value const Reference to SSP value to be inserted
    * @returns <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool insertValue( const Coord& coordinates, const Time& time_value, const SSP& ssp_value ) override;
    
    
    /**
    * Returns a std::unique_ptr to a heap-based SSP for given coordinates and date time if both present in the database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coords const reference to a valid Coord object
    * @param time const reference to a valid Time object
    * @param ssp_depth_precision ssp depth precision [m]
    * @return std::unique_ptr to a <i>valid</i> SSP if coordinates and time date are found, <i>not valid</i> otherwise
    **/  
    virtual std::unique_ptr<SSP> getValue( const Coord& coordinates, const Time& time, long double ssp_depth_precision ) const override;

    /**
    * Returns current WOADbType
    * @return current WOADbType
    **/
    WOADbType getWoaDbType() const { return woa_db_type; }


    protected:

    /**
    * WOA Db type
    **/
    WOADbType woa_db_type;

    /**
    * NetCDF variable representing SSP
    **/
#if defined (WOSS_NETCDF4_SUPPORT)
    netCDF::NcVar ssp_var;
#else
    NcVar* ssp_var;
#endif // defined (WOSS_NETCDF4_SUPPORT)
#if defined (WOSS_NETCDF4_SUPPORT)
    /**
    * NetCDF variable representing latitude
    **/
    netCDF::NcVar lat_var;
    /**
    * NetCDF variable representing longitude
    **/
    netCDF::NcVar lon_var;
#endif // defined (WOSS_NETCDF4_SUPPORT)

    /**
    * Returns the indexes used by the NetCDF variable to get the SSP values
    * @param coordinates const reference to a valid Coord object
    * @returns SSPIndexes value
    **/
    SSPIndexes getSSPIndexes( const Coord& coordinates ) const;

#if defined (WOSS_NETCDF4_SUPPORT)
    /**
    * Insert the SSP values taken from the given indexes into the given array
    * @param coordinates const reference to a valid Coord object
    * @param indexes const reference to a valid SSPIndexes object
    * @param ssp_values[] array that will hold SSP values
    **/
    void getSSPValue( const Coord& coordinates, const SSPIndexes& indexes, double ssp_values[] ) const;
#else    
    /**
    * Insert the SSP values taken from the given indexes into the given array 
    * @param indexes const reference to a valid SSPIndexes object
    * @param ssp_values[] array that will hold SSP values
    **/
    void getSSPValue( const SSPIndexes& indexes, double ssp_values[] ) const;
#endif // defined (WOSS_NETCDF4_SUPPORT)

  };

}

#endif // WOSS_NETCDF_SUPPORT

#endif /* WOSS_SSP_NETCDF_DB_H */
