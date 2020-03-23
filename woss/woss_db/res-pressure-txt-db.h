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
 * @file   res-pressure-txt-db.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::ResPressureTxtDb class
 *
 * Provides the interface for the woss::ResPressureTxtDb class
 */


#ifndef WOSS_RES_PRESSURE_TXT_DB_H
#define WOSS_RES_PRESSURE_TXT_DB_H


#include <map>
#include <complex>
#include <custom-precision-double.h>
#include "woss-db.h"


namespace woss {


  /**
  * \brief Textual WossDb for Pressure
  *
  * ResPressureTxtDb implements WossTextualDb and WossResPressDb for storing calculated Pressure into a text file
  *
  **/
  class ResPressureTxtDb : public WossTextualDb, public WossResPressDb {

    
    public:


    /**
    * ResPressureTxtDb constructor
    * @param name pathname of database
    **/
    ResPressureTxtDb( const ::std::string& name );

    virtual ~ResPressureTxtDb() { }


    /**
    * Closes the connection to the text file provided
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool closeConnection();


    /**
    * Post openConnection() actions
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool finalizeConnection();


    /**
    * Returns a pointer to a heap-created Pressure for given frequency, 
    * transmitter and receiver coordinates if present in the database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coord_tx const reference to a valid CoordZ object
    * @param coord_rx const reference to a valid CoordZ object
    * @param frequency used frequency [hz]
    * @param time_value const reference to a valid Time object
    * @return <i>valid</i> Pressure if parameters are found, <i>not valid</i> otherwise
    **/  
    virtual Pressure* getValue( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value ) const;


    /**
    * Inserts the given Pressure value in the pressure_map at given frequency, transmitter and receiver coordinates
    * @param coord_tx const reference to a valid CoordZ object
    * @param coord_rx const reference to a valid CoordZ object
    * @param frequency used frequency [hz]
    * @param time_value const reference to a valid Time object
    * @param pressure computed Pressure
    **/
    virtual bool insertValue( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value, const Pressure& pressure );


    static void setSpaceSampling( double value ) { space_sampling = value; }
    
    static double getSpaceSampling() { return space_sampling; }
    
    
    protected:
  
      
    typedef ::std::map< time_t, ::std::complex< double > > TimeMap;
    typedef TimeMap::iterator TMIter;
    typedef TimeMap::const_iterator TMCIter;
    typedef TimeMap::reverse_iterator TMRIter;
    
    typedef ::std::map< PDouble, TimeMap > FreqMap;
    typedef FreqMap::iterator FMIter;
    typedef FreqMap::reverse_iterator FMRIter;
    
    typedef ::std::map< CoordZ, FreqMap, CoordComparator< ResPressureTxtDb, CoordZ > > RxMap;
    typedef RxMap::iterator RxMIter;
    typedef RxMap::reverse_iterator RxMRIter;
       
    /**
    * Multidimensional map that links a transmitter CoordZ to a receiver CoordZ to a frequency PDouble value 
    * and finally to a Pressure value
    **/
    typedef ::std::map< CoordZ, RxMap, CoordComparator< ResPressureTxtDb, CoordZ > > PressureMatrix;
    typedef PressureMatrix::iterator PMIter;
    typedef PressureMatrix::const_iterator PMCIter;
    typedef PressureMatrix::reverse_iterator PMRIter;
    typedef PressureMatrix::const_reverse_iterator PMCRIter;
  
  
    static double space_sampling;
    
    
    /**
    * PressureMatrix map for storing imported and user inserted Pressure values
    **/
    PressureMatrix pressure_map;

    /**
    * pressure_map's initial size. If pressure_map's size is greater on closeConnection() the map will be written to disk
    **/
    int initial_pressmap_size;

    bool has_been_modified;
    
    
    /**
    * Prints pressure_map to screen. The columns format is the following: \n
    * <b> tx latitude, tx longitude, tx depth, rx latitude, rx longitude, rx depth, frequency, real pressure, imag pressure</b>
    * @returns <i>true</i> if operation succeeds, <i>false</i> otherwise
    **/  
    void printScreenMap();

    /**
    * Writes pressure_map to textual files. The columns format is the following: \n
    * <b> tx latitude, tx longitude, tx depth, rx latitude, rx longitude, rx depth, frequency, real pressure, imag pressure</b>
    * @returns <i>true</i> if operation succeeds, <i>false</i> otherwise
    **/  
    virtual bool writeMap();

    /**
    * Imports the formatted textual files into pressure_map. The column format is the following: \n
    * <b> tx latitude, tx longitude, tx depth, rx latitude, rx longitude, rx depth, frequency, real pressure, imag pressure</b>
    * @returns <i>true</i> if operation succeeds, <i>false</i> otherwise
    **/  
    virtual bool importMap();

    
    /**
    * Reads given values from pressure_map
    * @param tx valid transmitter coordinates
    * @param rx valid receiver coordinates
    * @param frequency frequency [hz]
    * @param time_value const reference to a valid time_value
    * @return valid Pressure if parameters are found, not valid otherwise
    **/
    ::std::complex<double> readMap( const CoordZ& tx, const CoordZ& rx, const double frequency, const Time& time_value ) const;

    
  };


}
#endif /* WOSS_RES_PRESSURE_TXT_DB_H */ 


 
