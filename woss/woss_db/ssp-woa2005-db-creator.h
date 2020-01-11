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
 * @file   ssp-woa2005-db-creator.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::SspWoa2005DbCreator class
 *
 * Provides the interface for the woss::SspWoa2005DbCreator class
 */


#ifndef WOSS_SSP_NETCDF_DB_CREATOR_H 
#define WOSS_SSP_NETCDF_DB_CREATOR_H


#ifdef WOSS_NETCDF_SUPPORT


#include "woss-db-creator.h"
#include "ssp-woa2005-db.h"

namespace woss {
  
    
  /**
  * \brief WossDbCreator for the custom made NetCDF WOA2005 SSP database
  *
  * Specialization of WossDbCreator for the  custom made NetCDF WOA2005 SSP database
  *
  **/
  class SspWoa2005DbCreator : public WossDbCreator {

    
    public:
    
      
    /**
    * Default SspWoa2005DbCreator constructor
    **/    
    SspWoa2005DbCreator();

#if defined (WOSS_NETCDF4_SUPPORT)
    /**
    * SspWoa2005DbCreator constructor
    * @param db_type WOADbType of the database
    **/
    SspWoa2005DbCreator( WOADbType db_type );
#endif // defined (WOSS_NETCDF4_SUPPORT)
    
    virtual ~SspWoa2005DbCreator();
    
    
    /**
    * This method is called to create and initialize a SspWoa2005Db
    * @return a pointer to a properly initialized SspWoa2005Db object
    **/
    virtual WossDb* const createWossDb();


    /**
    * Returns current WOADbType
    * @return current WOADbType
    **/
    WOADbType getWoaDbType() const { return woa_db_type; }

#if defined (WOSS_NETCDF4_SUPPORT)
    /**
    * Set current WOADbType
    * @param type valid WOADbType
    **/
    SspWoa2005DbCreator& setWoaDbType(WOADbType type) { woa_db_type = type; return *this; }
#endif // defined (WOSS_NETCDF4_SUPPORT)

    protected:
    
    
    /**
    * Initializes the pointed object
    * @param woss_db pointer to a recently created SspWoa2005Db
    * @return <i>true</i> if the method succeed, <i>false</i> otherwise
    **/           
    virtual bool initializeDb( WossDb* const woss_db );

    WOADbType woa_db_type;
  };

}

#endif // WOSS_NETCDF_SUPPORT


#endif /* WOSS_SSP_NETCDF_DB_CREATOR_H */

