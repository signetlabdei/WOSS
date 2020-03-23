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
 * @file   res-time-arr-bin-db.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::ResTimeArrBinDb class
 *
 * Provides the interface for the woss::ResTimeArrBinDb class
 */


#ifndef WOSS_RES_TIME_ARR_BIN_DB_H
#define WOSS_RES_TIME_ARR_BIN_DB_H


#include "res-time-arr-txt-db.h"


namespace woss {


  /**
  * \brief Binary WossDb for TimeArr
  *
  * ResTimeArrBinDb implements WossTextualDb and WossResTimeArrDb for storing calculated TimeArr into a binary file
  *
  **/
  class ResTimeArrBinDb : public ResTimeArrTxtDb {
    
    
    public:


    /**
    * ResTimeArrBinDb constructor
    * @param name pathname of database
    **/
    ResTimeArrBinDb( const ::std::string& name ) : ResTimeArrTxtDb(name) { }

    virtual ~ResTimeArrBinDb() { }
    

    static void setSpaceSampling( double value ) { space_sampling = value; }
    
    static double getSpaceSampling() { return space_sampling; }
    
    
    protected:
     
   
    /**
    * Writes arrivals_map into the binary file. The format is the following: \n
    * <b> tx latitude, tx longitude, tx depth, rx latitude, rx longitude, rx depth, frequency, total channel taps, 
    * delay-<i>i-th</i> real pressure-<i>i-th</i>, imag pressure-<i>i-th</i></b>
    * @returns <i>true</i> if operation succeeds, <i>false</i> otherwise
    **/  
    virtual bool writeMap();

    
    /**
    * Imports the binary file into arrivals_map. The format is the following: \n
    * <b> tx latitude, tx longitude, tx depth, rx latitude, rx longitude, rx depth, frequency, total channel taps, 
    * delay-<i>i-th</i> real pressure-<i>i-th</i>, imag pressure-<i>i-th</i></b>
    * @returns <i>true</i> if operation succeeds, <i>false</i> otherwise
    **/
    virtual bool importMap();

    
  };


}


#endif /* WOSS_RES_TIME_ARR_BIN_DB_H */ 


