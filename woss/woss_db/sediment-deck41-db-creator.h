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
 * @file   sediment-deck41-db-creator.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::SedimDeck41DbCreator class
 *
 * Provides the interface for the woss::SedimDeck41DbCreator class
 */


#ifndef WOSS_SEDIMENT_DECK41_DB_CREATOR_H 
#define WOSS_SEDIMENT_DECK41_DB_CREATOR_H


#ifdef WOSS_NETCDF_SUPPORT

#include <string>
#include "woss-db-creator.h"
#include "sediment-deck41-db.h"


namespace woss {


  /**
  * \brief DbCreator for NetCDF Deck41 Sediment database
  *
  * SedimDeck41DbCreator implements WossDbCreator for NetCDF Deck41 Sediment database
  **/
  class SedimDeck41DbCreator : public WossDbCreator {

    public:

    /**
    * SedimDeck41DbCreator default constructor
    **/
    SedimDeck41DbCreator();

    SedimDeck41DbCreator(const SedimDeck41DbCreator& copy) = default;

    SedimDeck41DbCreator(SedimDeck41DbCreator&& tmp) = default;

    virtual ~SedimDeck41DbCreator() override = default;

    SedimDeck41DbCreator& operator=( const SedimDeck41DbCreator& copy ) = default;

    SedimDeck41DbCreator& operator=( SedimDeck41DbCreator&& tmp ) = default;

    /**
    * Abstract method. Virtual factory method. 
    * It is called to clone this WossDbCreator instance. 
    * The caller will be the <b>owner</b> of created object,
    * therefore object destruction is his responsibility
    * @return a std::unique_ptr to a properly initialized WossDbCreator object
    **/
    virtual std::unique_ptr<WossDbCreator> clone() const override { return std::make_unique<SedimDeck41DbCreator>(*this); }

    /**
    * This method is called to create and initialize a SedimDeck41Db
    * @return a std::unique_ptr to a properly initialized SedimDeck41Db object
    **/
    virtual std::unique_ptr<WossDb> createWossDb() const override;


    void setDeck41CoordPathName( const std::string& name ) { db_coord_name = name; }

    void setDeck41MarsdenPathName( const std::string& name ) { db_marsden_name = name; }

    void setDeck41MarsdenOnePathName( const std::string& name ) { db_marsden_one_name = name; }

#if defined (WOSS_NETCDF4_SUPPORT)
    void setDeck41DbType( DECK41DbType db_type ) { deck41_db_type = db_type; }
#endif // defined (WOSS_NETCDF4_SUPPORT)

    std::string getDeck41CoordPathName() const { return db_coord_name; }

    std::string getDeck41MarsdenPathName() const { return db_marsden_name; }

    std::string getDeck41MarsdenOnePathName() const { return db_marsden_one_name; }

    DECK41DbType getDeck41DbType() const { return deck41_db_type; }


    protected:

    std::string db_coord_name;

    std::string db_marsden_name;

    std::string db_marsden_one_name;

    /**
     * DECK41 database type
     */
    DECK41DbType deck41_db_type;

    /**
    * Initializes the referenced object
    * @param woss_db reference to a recently created SedimDeck41Db
    * @return <i>true</i> if the method succeed, <i>false</i> otherwise
    **/
    virtual bool initializeDb( WossDb& woss_db ) const override;

    /**
    * Initializes the referenced object
    * @param woss_db reference to a recently created SedimDeck41Db
    * @return <i>true</i> if the method succeed, <i>false</i> otherwise
    **/ 
    bool initializeSedimDb( SedimDeck41Db& woss_db ) const;
  };

}

#endif // WOSS_NETCDF_SUPPORT

#endif /* WOSS_SEDIMENT_DECK41_DB_CREATOR_H */

