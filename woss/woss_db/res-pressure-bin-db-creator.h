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
 * @file   res-pressure-bin-db-creator.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::ResPressureBinDbCreator class
 *
 * Provides the interface for the woss::ResPressureBinDbCreator class
 */


#ifndef WOSS_RES_PRESSURE_BIN_DB_CREATOR_H 
#define WOSS_RES_PRESSURE_BIN_DB_CREATOR_H


#include "woss-db-creator.h"


namespace woss {
  
    
  /**
  * \brief DbCreator for binary Pressure database
  *
  * ResPressureBinDbCreator implements WossDbCreator for binary file Pressure database
  **/
  class ResPressureBinDbCreator : public WossDbCreator {

    
    public:
    

    /**
    * ResPressureBinDbCreator default constructor
    **/
    ResPressureBinDbCreator();
    
    virtual ~ResPressureBinDbCreator();
    

    /**
    * This method is called to create and initialize a ResPressureTxtDb
    * @return a pointer to a properly initialized ResPressureTxtDb object
    **/
    virtual WossDb* const createWossDb();

    
    void setSpaceSampling( double value ) { space_sampling = value; }
    
    double getSpaceSampling() { return space_sampling; }
    

    protected:
        

    double space_sampling;   
    
    
    /**
    * Initializes the pointed object
    * @param woss_db pointer to a recently created ResPressureTxtDb
    * @return <i>true</i> if the method succeed, <i>false</i> otherwise
    **/
    virtual bool initializeDb( WossDb* const woss_db );
    
    
  };

  
}


#endif /* WOSS_RES_PRESSURE_BIN_DB_CREATOR_H */

