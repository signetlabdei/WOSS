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
 * @file   res-time-arr-txt-db.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::ResTimeArrTxtDb class
 *
 * Provides the interface for the woss::ResTimeArrTxtDb class
 */


#ifndef WOSS_RES_TIME_ARR_TXT_DB_H
#define WOSS_RES_TIME_ARR_TXT_DB_H


#include <coordinates-definitions.h>
#include <time-arrival-definitions.h>
#include "woss-db.h"


namespace woss {


  /**
  * \brief Textual WossDb for TimeArr
  *
  * ResTimeArrTxtDb implements WossTextualDb and WossResTimeArrDb for storing calculated TimeArr into a text file
  *
  **/
  class ResTimeArrTxtDb : public WossTextualDb, public WossResTimeArrDb {
    
    
    public:


    /**
    * ResTimeArrTxtDb constructor
    * @param name pathname of database
    **/
    ResTimeArrTxtDb( const ::std::string& name );

    virtual ~ResTimeArrTxtDb() { }
    

    /**
    * Post openConnection() actions
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool finalizeConnection();

    
    /**
    * Closes the connection to the text file provided
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool closeConnection();

    
    /**
    * Returns a pointer to a heap-created TimeArr value for given frequency, 
    * transmitter and receiver coordinates if present in the database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coord_tx const reference to a valid CoordZ object
    * @param coord_rx const reference to a valid CoordZ object
    * @param frequency used frequency [hz]
    * @param time_value const reference to a valid Time object
    * @return <i>valid</i> TimeArr if parameters are found, <i>not valid</i> otherwise
    **/  
    virtual TimeArr* getValue( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value ) const ;

    /**
    * Inserts the given TimeArr value in the arrivals_map at given frequency, transmitter and receiver coordinates
    * @param coord_tx const reference to a valid CoordZ object
    * @param coord_rx const reference to a valid CoordZ object
    * @param frequency used frequency [hz]
    * @param time_value const reference to a valid Time& object
    * @param channel computed TimeArr
    **/
    virtual bool insertValue( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency,const Time& time_value, const TimeArr& channel ) ;


    static void setSpaceSampling( double value ) { space_sampling = value; }
    
    static double getSpaceSampling() { return space_sampling; }
    
    
    protected:
  
      
    typedef ::std::map< time_t, TimeArr > TimeMap;
    typedef TimeMap::iterator TMIter;
    typedef TimeMap::const_iterator TMCIter;
    typedef TimeMap::reverse_iterator TMRIter;
    
    typedef ::std::map< PDouble, TimeMap > FreqMap;
    typedef FreqMap::iterator FMIter;
    typedef FreqMap::const_iterator FMCIter;
    typedef FreqMap::reverse_iterator FMRIter;
    
    typedef ::std::map< CoordZ, FreqMap, CoordComparator< ResTimeArrTxtDb, CoordZ > > RxMap;
//     typedef ::std::map< CoordZ, FreqMap > RxMap;
    typedef RxMap::iterator RxMIter;
    typedef RxMap::const_iterator RxMCIter;
    typedef RxMap::reverse_iterator RxMRIter;
    
    /**
    * Multidimensional map that links a transmitter CoordZ to a receiver CoordZ to a frequency PDouble value 
    * and finally to a TimeArr value
    **/
    typedef ::std::map< CoordZ, RxMap, CoordComparator< ResTimeArrTxtDb, CoordZ > > ArrMatrix;
//     typedef ::std::map< CoordZ, RxMap > ArrMatrix;
    typedef ArrMatrix::iterator AMXIter;
    typedef ArrMatrix::const_iterator AMXCIter;
    typedef ArrMatrix::reverse_iterator AMXRIter;
    typedef ArrMatrix::const_reverse_iterator AMXCRIter;
  
    
    static double space_sampling;
    
    
    /**
    * ArrMatrix map for storing imported and user inserted TimeArr values
    **/
    ArrMatrix arrivals_map;

    /**
    * arrivals_map's initial size. If pressure_map's size is greater on closeConnection() the map will be written to disk
    **/
    int initial_arrmap_size;
    
    
    bool has_been_modified;


    /**
    * Prints arrivals_map to screen. The columns format is the following: \n
    * <b> tx latitude, tx longitude, tx depth, rx latitude, rx longitude, rx depth, frequency, total channel taps, 
    * delay-<i>i-th</i> real pressure-<i>i-th</i>, imag pressure-<i>i-th</i></b>
    * @returns <i>true</i> if operation succeeds, <i>false</i> otherwise
    **/  
    void printScreenMap();

    
    /**
    * Writes arrivals_map into the textual file. The columns format is the following: \n
    * <b> tx latitude, tx longitude, tx depth, rx latitude, rx longitude, rx depth, frequency, total channel taps, 
    * delay-<i>i-th</i> real pressure-<i>i-th</i>, imag pressure-<i>i-th</i></b>
    * @returns <i>true</i> if operation succeeds, <i>false</i> otherwise
    **/  
    virtual bool writeMap();

    
    /**
    * Imports the formatted textual files into arrivals_map. The column format is the following: \n
    * <b> tx latitude, tx longitude, tx depth, rx latitude, rx longitude, rx depth, frequency, total channel taps, 
    * delay-<i>i-th</i> real pressure-<i>i-th</i>, imag pressure-<i>i-th</i></b>
    * @returns <i>true</i> if operation succeeds, <i>false</i> otherwise
    **/
    virtual bool importMap();


    /**
    * Reads given values from arrivals_map
    * @param tx valid transmitter coordinates
    * @param rx valid receiver coordinates
    * @param frequency frequency [hz]
    * @param time_value const reference to a valid Time object
    * @return valid TimeArr if parameters are found, not valid otherwise
    **/
    const TimeArr* readMap( const CoordZ& tx, const CoordZ& rx, const double frequency, const Time& time_value ) const;

    
  };


}


#endif /* WOSS_RES_TIME_ARR_TXT_DB_H */ 


