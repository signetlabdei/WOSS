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
 * @file   bathymetry-gebco-db-creator.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::BathyGebcoDbCreator class
 *
 * Provides the interface for the woss::BathyGebcoDbCreator class
 */


#ifndef WOSS_BATHYMETRY_GEBCO_DB_CREATOR_H 
#define WOSS_BATHYMETRY_GEBCO_DB_CREATOR_H

#ifdef WOSS_NETCDF_SUPPORT

#include <memory>
#include "woss-db-creator.h"
#include "bathymetry-gebco-db.h"


namespace woss {

  /**
  * \brief WossDbCreator for the GEBCO bathymetry database
  *
  * Specialization of WossDbCreator for the GEBCO bathymetry database.
  * 
  * It also provides a Tcl interpreter for NS-2 implementation.
  *
  **/
  class BathyGebcoDbCreator : public WossDbCreator {

    public:

    BathyGebcoDbCreator() = default;

    BathyGebcoDbCreator(const BathyGebcoDbCreator& copy) = default;

    BathyGebcoDbCreator(BathyGebcoDbCreator&& tmp) = default;

    virtual ~BathyGebcoDbCreator() override = default;

    BathyGebcoDbCreator& operator=( const BathyGebcoDbCreator& copy ) = default;

    BathyGebcoDbCreator& operator=( BathyGebcoDbCreator&& tmp ) = default;

    /**
    * Abstract method. Virtual factory method. 
    * It is called to clone this WossDbCreator instance. 
    * The caller will be the <b>owner</b> of created object,
    * therefore object destruction is his responsibility
    * @return a std::unique_ptr to a properly initialized WossDbCreator object
    **/
    virtual std::unique_ptr<WossDbCreator> clone() const override { return std::make_unique<BathyGebcoDbCreator>(*this); }

    /**
    * Creates and initialize a BathyGebcoDb object
    * @return a std::unique_ptr to a properly initialized BathyGebcoDb object
    **/
    virtual std::unique_ptr<WossDb> createWossDb() const override;

    /**
    * Sets the GEBCO_BATHY_TYPE related to the netcdf db that will be opened
    * @param bathy_type netcdf file format
    * @return reference to <b>*this</b>
    */
    BathyGebcoDbCreator& setGebcoBathyType( GEBCO_BATHY_TYPE bathy_type ) { 
      gebco_type = bathy_type; return *this; }

    /**
    * Gets the GEBCO_BATHY_TYPE related to the netcdf db that will be opened
    * @return gebco_type
    */
    GEBCO_BATHY_TYPE getGebcoBathyType() const { return gebco_type; }

    protected:

    /**
    * GEBCO version in use
    * @see GEBCO_BATHY_TYPE
    **/
    GEBCO_BATHY_TYPE gebco_type = GEBCO_BATHY_TYPE::GEBCO_2D_30_SECONDS_BATHY_TYPE; 

    /**
    * Initializes the pointed object
    * @param woss_db reference to a recently created BathyGebcoDb
    * @return <i>true</i> if the method succeed, <i>false</i> otherwise
    **/
    virtual bool initializeDb( WossDb& woss_db ) const override;

  };

}

#endif // WOSS_NETCDF_SUPPORT

#endif /* WOSS_BATHYMETRY_GEBCO_DB_CREATOR_H */

