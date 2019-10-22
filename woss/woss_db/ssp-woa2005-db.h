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


#include <ssp-definitions.h>
#include "woss-db.h"
#if defined (WOSS_NETCDF4_SUPPORT)
#include <ncVar.h>
#endif // defined (WOSS_NETCDF4_SUPPORT)

namespace woss {
  

  /**
  * Pair representing latitute and longitude index for NetCDF value access
  **/
  typedef ::std::pair< int, int > SSPIndexes;


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
    SspWoa2005Db( const ::std::string& name );

    virtual ~SspWoa2005Db() { }


    /**
    * Post openConnection() actions, used to create and initialize NetCDF variables
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool finalizeConnection();

    
    /**
    * Inserts the given woss::SSP value in the database for given coordinates
    * @param coordinates const reference to a valid Coord object
    * @param time_value const reference to a valid Time object
    * @param ssp_value const Reference to SSP value to be inserted
    * @returns <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool insertValue( const Coord& coordinates, const Time& time_value, const SSP& ssp_value );
    
    
    /**
    * Returns a pointer to a heap-based SSP for given coordinates and date time if both present in the database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coords const reference to a valid Coord object
    * @param time const reference to a valid Time object
    * @param ssp_depth_precision ssp depth precision [m]
    * @return <i>valid</i> SSP if coordinates and time date are found, <i>not valid</i> otherwise
    **/  
    virtual SSP* getValue( const Coord& coordinates, const Time& time, long double ssp_depth_precision ) const ;


    protected:


  /**
    * NetCDF variable representing SSP
    **/
#if defined (WOSS_NETCDF4_SUPPORT)
    netCDF::NcVar ssp_var;
#else
    NcVar* ssp_var;
#endif // defined (WOSS_NETCDF4_SUPPORT)
    
    /**
    * Returns the indexes used by the NetCDF variable to get the SSP values
    * @param coordinates const reference to a valid Coord object
    * @returns SSPIndexes value
    **/
    SSPIndexes getSSPIndexes( const Coord& coordinates ) const;

    
    /**
    * Insert the SSP values taken from the given indexes into the given array 
    * @param indexes const reference to a valid SSPIndexes object
    * @param ssp_values[] array that will hold SSP values
    **/
    void getSSPValue( const SSPIndexes& indexes, double ssp_values[] ) const;

    
  };

  
}


#endif // WOSS_NETCDF_SUPPORT

#endif /* WOSS_SSP_NETCDF_DB_H */


 
