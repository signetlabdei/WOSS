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

#include <memory>
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
    SspWoa2005DbCreator() = default;

#if defined (WOSS_NETCDF4_SUPPORT)
    /**
    * SspWoa2005DbCreator constructor
    * @param db_type WOADbType of the database
    **/
    SspWoa2005DbCreator( WOADbType db_type );
#endif // defined (WOSS_NETCDF4_SUPPORT)

    SspWoa2005DbCreator(const SspWoa2005DbCreator& copy) = default;

    SspWoa2005DbCreator(SspWoa2005DbCreator&& tmp) = default;

    virtual ~SspWoa2005DbCreator() override = default;

    SspWoa2005DbCreator& operator=( const SspWoa2005DbCreator& copy ) = default;

    SspWoa2005DbCreator& operator=( SspWoa2005DbCreator&& tmp ) = default;

    /**
    * Abstract method. Virtual factory method. 
    * It is called to clone this WossDbCreator instance. 
    * The caller will be the <b>owner</b> of created object,
    * therefore object destruction is his responsibility
    * @return a std::unique_ptr to a properly initialized WossDbCreator object
    **/
    virtual std::unique_ptr<WossDbCreator> clone() const override { return std::make_unique<SspWoa2005DbCreator>(*this); }

    /**
    * This method is called to create and initialize a SspWoa2005Db
    * @return a std::unique_ptr to a properly initialized SspWoa2005Db object
    **/
    virtual std::unique_ptr<WossDb> createWossDb() const override;

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
    * Initializes the referenced object
    * @param woss_db reference to a recently created SspWoa2005Db
    * @return <i>true</i> if the method succeed, <i>false</i> otherwise
    **/           
    virtual bool initializeDb( WossDb& woss_db ) const override;

    WOADbType woa_db_type = WOADbType::WOA_DB_TYPE_2005;
  };

}

#endif // WOSS_NETCDF_SUPPORT


#endif /* WOSS_SSP_NETCDF_DB_CREATOR_H */

