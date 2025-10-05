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
 * @file   res-pressure-txt-db-creator.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::ResPressureTxtDbCreator class
 *
 * Provides the interface for the woss::ResPressureTxtDbCreator class
 */


#ifndef WOSS_RES_PRESSURE_TXT_DB_CREATOR_H 
#define WOSS_RES_PRESSURE_TXT_DB_CREATOR_H


#include "woss-db-creator.h"


namespace woss {
  
    
  /**
  * \brief DbCreator for textual Pressure database
  *
  * ResPressureTxtDbCreator implements WossDbCreator for textual Pressure database
  **/
  class ResPressureTxtDbCreator : public virtual WossDbCreator {

    public:

    ResPressureTxtDbCreator() = default;

    ResPressureTxtDbCreator(const ResPressureTxtDbCreator& copy) = default;

    ResPressureTxtDbCreator(ResPressureTxtDbCreator&& tmp) = default;

    virtual ~ResPressureTxtDbCreator() override = default;

    ResPressureTxtDbCreator& operator=( const ResPressureTxtDbCreator& copy ) = default;

    ResPressureTxtDbCreator& operator=( ResPressureTxtDbCreator&& tmp ) = default;

    /**
    * Abstract method. Virtual factory method. 
    * It is called to clone this WossDbCreator instance. 
    * The caller will be the <b>owner</b> of created object,
    * therefore object destruction is his responsibility
    * @return a std::unique_ptr to a properly initialized WossDbCreator object
    **/
    virtual std::unique_ptr<WossDbCreator> clone() const override { return std::make_unique<ResPressureTxtDbCreator>(*this); }

    /**
    * This method is called to create and initialize a ResPressureTxtDb
    * @return a std::unique_ptr to a properly initialized ResPressureTxtDb object
    **/
    virtual std::unique_ptr<WossDb> createWossDb() const override;

    void setSpaceSampling( double value ) { space_sampling = value; }
    
    double getSpaceSampling() const { return space_sampling; }
    
    protected:

    double space_sampling = 0.0;
    
    
    /**
    * Initializes the pointed object
    * @param woss_db reference to a recently created ResPressureTxtDb
    * @return <i>true</i> if the method succeed, <i>false</i> otherwise
    **/
    virtual bool initializeDb( WossDb& woss_db ) const override;

  };

  
}


#endif /* WOSS_RES_PRESSURE_TXT_DB_CREATOR_H */

