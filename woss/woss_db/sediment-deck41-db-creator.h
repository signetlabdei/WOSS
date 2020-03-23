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

#include "woss-db-creator.h"


namespace woss {
  
    
  class SedimDeck41Db;
  
  
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
    
    virtual ~SedimDeck41DbCreator();
    
 
    /**
    * This method is called to create and initialize a SedimDeck41Db
    * @return a pointer to a properly initialized SedimDeck41Db object
    **/
    virtual WossDb* const createWossDb();


    void setDeck41CoordPathName( const ::std::string& name ) { db_coord_name = name; }
    
    void setDeck41MarsdenPathName( const ::std::string& name ) { db_marsden_name = name; }

    void setDeck41MarsdenOnePathName( const ::std::string& name ) { db_marsden_one_name = name; }


    ::std::string getDeck41CoordPathName() { return db_coord_name; }
    
    ::std::string getDeck41MarsdenPathName() { return db_marsden_name; }

    ::std::string getDeck41MarsdenOnePathName() { return db_marsden_one_name; }
    
    
    protected:
    
      
    ::std::string db_coord_name;
    
    ::std::string db_marsden_name;
    
    ::std::string db_marsden_one_name;
    
    
    /**
    * Initializes the pointed object
    * @param woss_db pointer to a recently created SedimDeck41Db
    * @return <i>true</i> if the method succeed, <i>false</i> otherwise
    **/   
    virtual bool initializeDb( WossDb* woss_db );
    
    
    /**
    * Initializes the pointed object
    * @param woss_db pointer to a recently created SedimDeck41Db
    * @return <i>true</i> if the method succeed, <i>false</i> otherwise
    **/ 
    bool initializeSedimDb( SedimDeck41Db* const woss_db );
    

  };

  
}

#endif // WOSS_NETCDF_SUPPORT

#endif /* WOSS_SEDIMENT_DECK41_DB_CREATOR_H */

