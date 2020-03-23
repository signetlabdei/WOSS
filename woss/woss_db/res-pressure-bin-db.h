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
 * @file   res-pressure-bin-db.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::ResPressureBinDb class
 *
 * Provides the interface for the woss::ResPressureBinDb class
 */


#ifndef WOSS_RES_PRESSURE_BIN_DB_H
#define WOSS_RES_PRESSURE_BIN_DB_H


#include "res-pressure-txt-db.h"


namespace woss {


  /**
  * \brief Binary WossDb for Pressure
  *
  * ResPressureBinDb implements WossTextualDb and WossResPressDb for storing calculated Pressure into a binary file
  *
  **/
  class ResPressureBinDb : public ResPressureTxtDb {

    
    public:


    /**
    * ResPressureBinDb constructor
    * @param name pathname of database
    **/
    ResPressureBinDb( const ::std::string& name ) : ResPressureTxtDb(name) { }

    virtual ~ResPressureBinDb() { }
   
    
    protected:
  
      
    /**
    * Writes pressure_map to binary file. The format is the following: \n
    * <b> tx latitude, tx longitude, tx depth, rx latitude, rx longitude, rx depth, frequency, real pressure, imag pressure</b>
    * @returns <i>true</i> if operation succeeds, <i>false</i> otherwise
    **/  
    virtual bool writeMap();

    /**
    * Imports the formatted binary file into pressure_map. The format is the following: \n
    * <b> tx latitude, tx longitude, tx depth, rx latitude, rx longitude, rx depth, frequency, real pressure, imag pressure</b>
    * @returns <i>true</i> if operation succeeds, <i>false</i> otherwise
    **/  
    virtual bool importMap();

    
  };


}
#endif /* WOSS_RES_PRESSURE_BIN_DB_H */ 


 
