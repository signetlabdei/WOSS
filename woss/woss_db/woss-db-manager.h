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
 * @file   woss-db-manager.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossDbManager class
 *
 * Provides the interface for the woss::WossDbManager class
 */


#ifndef WOSS_DB_MANAGER_IMPLEMENT_H 
#define WOSS_DB_MANAGER_IMPLEMENT_H


#include "woss-db-custom-data-container.h"
#include <coordinates-definitions.h>
#include <sediment-definitions.h>
#include <altimetry-definitions.h>
#include <ssp-definitions.h>
#include <definitions.h>


namespace woss {


  class WossBathymetryDb;
  
  class WossSedimentDb;
  
  class WossSSPDb;
  
  class WossResTimeArrDb;
  
  class WossResPressDb;
  
  class Time;
  
  class Pressure;
  
  class TimeArr;
  
  
  /**
  * \brief Abstraction layer for database and data manipulation
  *
  * WossDbManager has the exclusive handling of <b>all</b> databases involved. 
  * No other object can access directly to a WossDb entity.
  * Thanks to the abstraction provided, WossDbManager can perform arithmetic and other data manipulation. 
  * It should be used with woss::Singleton for safety reasons. ( e.g. woss::Singleton\<woss::WossDbManager> )
  * WossDbManager also provides a way to generate environmental data on the fly. 
  * Providing a generator coordinate and a set of bearing and ranges, the user can create all sort of polygon 
  * where the given environmental data is valid.
  * @see setCustom* methods, command
  **/
  class WossDbManager {

		
    protected:
   
    
    /**
    * \brief Bearing operator function object
    *
    * Function object that returns the bearing between two valid woss::Coord
    **/
    class BearingOperator {
      
      
      public:
      
        
      /**
      * Function that compares to woss::Coord instances. 
      * If WossDbManager::cust_bathymetry_coord_resolution is valid (>=0)
      * two valid CoordZ are considered equivalent if their great circle distance 
      * is less or equal to the space sampling value 
      * @param tx const reference to a valid Coord object
      * @param rx const reference to a valid Coord object
      * @returns <i>true</i> if x less than y, <i>false</i> otherwise
      **/       
      double operator()( const Coord& x, const Coord& y ) const {                     
        return( x.getInitialBearing(y) );
      }
      
    };
    

    /**
    * \brief Range operator function object
    *
    * Function object that returns the great circle distance between two valid woss::Coord
    **/    
    class RangeOperator {
      
      
      public:
      
        
      /**
      * Function that compares to woss::Coord instances. 
      * If WossDbManager::cust_bathymetry_coord_resolution is valid (>=0)
      * two valid CoordZ are considered equivalent if their great circle distance 
      * is less or equal to the space sampling value 
      * @param tx const reference to a valid Coord object
      * @param rx const reference to a valid Coord object
      * @returns <i>true</i> if x less than y, <i>false</i> otherwise
      **/       
      double operator()( const Coord& x, const Coord& y ) const {                     
        return( x.getGreatCircleDistance(y) );
      }
      
    };
    
   
    public:


    typedef CustomDataContainer< Coord, BearingOperator, RangeOperator, Bathymetry > CCBathymetry;

    typedef CustomDataTimeContainer< Coord, BearingOperator, RangeOperator, SSP* > CCSSP;

    typedef CustomDataContainer< Coord, BearingOperator, RangeOperator, Sediment* > CCSediment;

    typedef CustomDataContainer< Coord, BearingOperator, RangeOperator, Altimetry* > CCAltimetry;
         
      
    /**
    * Default constructor
    **/
    WossDbManager();
    
    /**
    * Copy constructor ( no const here, we have to modify the copy )
    **/
    WossDbManager( WossDbManager& instance );
    
    /**
    * Assignment operator ( no const here, we have to modify the copy )
    **/
    WossDbManager& operator=( WossDbManager& instance );
    
    /**
    * Destructor. It deletes all pointers involved. Derived classes 
    * <b>don't have to do it</b>
    **/ 
    virtual ~WossDbManager();


    /**
    * Returns a pointer to a heap-created Altimetry value for given coordinates and depth, 
    * if present in the Altimetry database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coords const reference to a valid CoordZ object
    * @return <i>valid</i> Altimetry if coordinates are found, <i>not valid</i> otherwise
    **/
    virtual Altimetry* getAltimetry( const CoordZ& tx, const CoordZ& rx ) const;    
    
    
    /**
    * Returns a pointer to a heap-created Sediment value for given coordinates and depth, 
    * if present in the Sediment database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coords const reference to a valid CoordZ object
    * @return <i>valid</i> Sediment if coordinates are found, <i>not valid</i> otherwise
    **/
    virtual Sediment* getSediment( const CoordZ& tx, const CoordZ& rx ) const;

    /**
    * Returns the representative sediment value of given coordinates and depth vector, 
    * if at least one set of coordinates is present in the Sediment database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coordz_vector const reference to a valid CoordZ vector
    * @return <i>valid</i> Sediment if at least one set of coordinates is found, <i>not valid</i> otherwise
    **/
    virtual Sediment* getSediment( const CoordZ& tx, const CoordZVector& rx_coordz_vector ) const;


    /**
    * Returns the positive depth value ( bathymetry ) of given coordinates, if present in the database
    * @param coords const reference to a valid Coord object
    * @return <i>positive</i> depth value [m] if coordinates are found, <i>HUGE_VAL</i> otherwise
    **/
    virtual Bathymetry getBathymetry( const Coord& tx, const Coord& rx ) const;

    /**
    * Sets the positive depth for each CoordZ present in the vector, HUGE_VAL is set if coordinates are not present in the database
    * @param coords reference to a CoordZVector
    **/
    virtual void getBathymetry( const Coord& tx, CoordZVector& rx_coordz_vector ) const;


    /**
    * Returns the SSP value of given coordinates and date time if both present in the database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coords const reference to a valid Coord object
    * @param time const reference to a valid Time object
    * @param ssp_depth_precision ssp depth precision [m]
    * @return <i>valid</i> SSP if coordinates are found, <i>not valid</i> otherwise
    **/  
    virtual SSP* getSSP( const Coord& tx, const Coord& rx, const Time& time, long double ssp_depth_precision = SSP_CUSTOM_DEPTH_PRECISION ) const;

    /**
    * Returns a pointer a heap-created average SSP for given coordinates, start and end time date 
    * if they are present in the database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coords const reference to a valid Coord object
    * @param time_start const reference to a valid Time object
    * @param time_end const reference to a valid Time object
    * @param max_time_values total number of Time to take between time_start and time_end
    * @param ssp_depth_precision returned SSP's depth precision
    * @return <i>valid</i> SSP if coordinates and both time date are found, <i>not valid</i> otherwise
    **/  
    virtual SSP* getAverageSSP( const Coord& tx, const Coord& rx, const Time& time_start, const Time& time_end, int max_time_values, long double ssp_depth_precision = SSP_CUSTOM_DEPTH_PRECISION ) const;


    /**
    * Returns a pointer to a heap-created TimeArr value of given frequency, transmitter and receiver 
    * coordinates if present in the database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coord_tx const reference to a valid CoordZ object
    * @param coord_rx const reference to a valid CoordZ object
    * @param frequency used frequency [hz]
    * @param time_value const reference to a valid Time object
    * @return <i>valid</i> TimeArr if parameters are found, <i>not valid</i> otherwise
    **/  
    virtual TimeArr* getTimeArr( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value ) const;

    /**
    * Inserts a given TimeArr value in the database at given frequency, transmitter and receiver coordinates
    * @param coord_tx const reference to a valid CoordZ object
    * @param coord_rx const reference to a valid CoordZ object
    * @param frequency used frequency [hz]
    * @param time_value const reference to a valid Time object
    * @param channel computed TimeArr
    **/
    virtual void insertTimeArr( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value, const TimeArr& channel ) const;


    /**
    * Returns a pointer to a heap-created Pressure value of given frequency, 
    * transmitter and receiver coordinates if present in the database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coord_tx const reference to a valid CoordZ object
    * @param coord_rx const reference to a valid CoordZ object
    * @param frequency used frequency [hz]
    * @param time_value const reference to a valid Time object
    * @return <i>valid</i> TimeArr if parameters are found, <i>not valid</i> otherwise
    **/  
    virtual Pressure* getPressure( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value ) const;

    /**
    * Inserts the given Pressure value in the database at given frequency, transmitter and receiver coordinates
    * @param coord_tx const reference to a valid CoordZ object
    * @param coord_rx const reference to a valid CoordZ object
    * @param frequency used frequency [hz]
    * @param time_value const reference to a valid Time object
    * @param channel computed Pressure
    **/
    virtual void insertPressure( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value, const Pressure& pressure ) const;


    /**
    * Initializes the bathymetry database pointer
    * @param ptr valid WossBathymetryDb*
    * @return reference to <b>*this</b>
    **/
    WossDbManager& setBathymetryDb( WossBathymetryDb* ptr ) { bathymetry_db = ptr; return *this; }

    /**
    * Initializes the Sediment database pointer
    * @param ptr valid WossSedimentDb*
    * @return reference to <b>*this</b>
    **/
    WossDbManager& setSedimentDb( WossSedimentDb* ptr ) { sediment_db = ptr; return *this; }

    /**
    * Initializes the SSP database pointer
    * @param ptr valid WossSSPDb*
    * @return reference to <b>*this</b>
    **/
    WossDbManager& setSSPDb( WossSSPDb* ptr ) { ssp_db = ptr; return *this; }

    /**
    * Initializes the TimeArr database pointer
    * @param ptr valid WossResTimeArrDb*
    * @return reference to <b>*this</b>
    **/
    WossDbManager& setResTimeArrDb( WossResTimeArrDb* ptr ) { results_arrivals_db = ptr; return *this; }
  
    /**
    * Initializes the Pressure database pointer
    * @param ptr valid WossResPressDb*
    * @return reference to <b>*this</b>
    **/
    WossDbManager& setResPressureDb( WossResPressDb* ptr ) { results_pressure_db = ptr; return *this; }
    

    /**
    * Sets the custom Altimetry for given generator Coord
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @returns <i>true</i> if method succed, <i>false</i> otherwise
    **/
    bool setCustomAltimetry( Altimetry* const altimetry, const Coord& tx_coord = CCAltimetry::DB_CDATA_ALL_OUTER_KEYS, 
                            double bearing = CCAltimetry::DB_CDATA_ALL_MEDIUM_KEYS, 
                            double range = CCAltimetry::DB_CDATA_ALL_INNER_KEYS );
      
    /**
    * Gets the custom Altimetry for given generator Coord
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @returns NULL pointer if parameters are not found.
    **/
    Altimetry* getCustomAltimetry( const Coord& tx_coord = CCAltimetry::DB_CDATA_ALL_OUTER_KEYS, 
                                 double bearing = CCAltimetry::DB_CDATA_ALL_MEDIUM_KEYS, 
                                 double range = CCAltimetry::DB_CDATA_ALL_INNER_KEYS );
    
    /**
     * Erases the custom Altimetry for given parameters
     * @param tx_coord const reference to a valid Coord (generator coordinates)
     * @param bearing bearing value [radians]
     * @param range range value [m]
     * @return reference to <b>*this</b>
     **/
    WossDbManager& eraseCustomAltimetry( const Coord& tx_coord = CCAltimetry::DB_CDATA_ALL_OUTER_KEYS, 
                                         double bearing = CCAltimetry::DB_CDATA_ALL_MEDIUM_KEYS, 
                                         double range = CCAltimetry::DB_CDATA_ALL_INNER_KEYS );
    
    
    /**
    * Sets the custom Sediment for given generator Coord
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @returns if insertion succeed, the function returns a NULL pointer, that needs to be initialized.
    *          if an object was already instantiated, the function returns the pointer to it;
    **/
    bool setCustomSediment( Sediment* const sediment, const Coord& tx_coord = CCSediment::DB_CDATA_ALL_OUTER_KEYS, 
                            double bearing = CCSediment::DB_CDATA_ALL_MEDIUM_KEYS, 
                            double range = CCSediment::DB_CDATA_ALL_INNER_KEYS );
      
    /**
    * Gets the custom Sediment for given generator Coord
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @returns NULL pointer if parameters are not found.
    **/
    Sediment* getCustomSediment( const Coord& tx_coord = CCSediment::DB_CDATA_ALL_OUTER_KEYS, 
                                 double bearing = CCSediment::DB_CDATA_ALL_MEDIUM_KEYS, 
                                 double range = CCSediment::DB_CDATA_ALL_INNER_KEYS );

    /**
    * Erases the custom Sediment for given parameters
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @return reference to <b>*this</b>
    **/
    WossDbManager& eraseCustomSediment( const Coord& tx_coord = CCSediment::DB_CDATA_ALL_OUTER_KEYS, 
                                        double bearing = CCSediment::DB_CDATA_ALL_MEDIUM_KEYS, 
                                        double range = CCSediment::DB_CDATA_ALL_INNER_KEYS );
    
    
    /**
    * Sets the custom SSP for given generator Coord
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @param time_value const reference to a valid Time object
    * @returns if insertion succeed, the function returns a NULL pointer, that needs to be initialized.
    *          if an object was already instantiated, the function returns the pointer to it;
    **/
    bool setCustomSSP( SSP* const ssp, const Coord& tx_coord = CCSSP::DB_CDATA_ALL_OUTER_KEYS, 
                                       double bearing = CCSSP::DB_CDATA_ALL_MEDIUM_KEYS, 
                                       double range = CCSSP::DB_CDATA_ALL_INNER_KEYS,
                                       const Time& time_value = CCSSP::DB_CDATA_ALL_TIME_KEYS );
      
    /**
    * Imports a CustomSSP from file. \n
    * File format: 
    * <ul>
    *   <li> first line: type name.
    *   <li> second line: latitude for depth and pressure correction purposes
    *   <li> third line: longitude for depth and pressure correction purposes
    * </ul>
    * Following format depends on type name:
    * <ul>
    *  <li><b>"SSP"</b> : three columns.  range [m] | depth [m] | sound speed [m/s]
    *  <li><b>"FULL"</b> : six columns.   range [m] | depth [m] | temperature [C°] | salinity [ppu] | pressure [bar] | sound speed [m/s] 
    *  <li><b>"TEMPERATURE_SALINITY_PRESSURE"</b> : four columns. range [m] | temperature [C°] | salinity [ppu] | pressure [bar]
    *  <li><b>"DEPTH_TEMPERATURE_SALINITY"</b> : four columns. range [m] | depth [m] | temperature [C°] | salinity [ppu]
    * </ul>
    * @param filename const reference to a string
    * @param time const reference to a valid Time object
    * @param tx_coord const reference to a valid Coord object (originator coordinates)
    * @param bearing bearing value [radians]
    * @returns true is import was completed succesfully, false otherwise
    **/
    virtual bool importCustomSSP( const ::std::string& filename, const Time& = CCSSP::DB_CDATA_ALL_TIME_KEYS,
	                                const Coord& tx_coord = CCSSP::DB_CDATA_ALL_OUTER_KEYS, double bearing = CCSSP::DB_CDATA_ALL_MEDIUM_KEYS );

    /**
    * Gets the custom SSP for given generator Coord
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @param time_value const reference to a valid Time object
    * @returns NULL pointer if parameters are not found.
    **/
    SSP* getCustomSSP( const Coord& tx_coord = CCSSP::DB_CDATA_ALL_OUTER_KEYS, 
                       double bearing = CCSSP::DB_CDATA_ALL_MEDIUM_KEYS, 
                       double range = CCSSP::DB_CDATA_ALL_INNER_KEYS,
                       const Time& time_value = CCSSP::DB_CDATA_ALL_TIME_KEYS );

    /**
    * Erases the custom SSP for given parameters
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @param time_value const reference to a valid Time object
    * @return reference to <b>*this</b>
    **/
    WossDbManager& eraseCustomSSP( const Coord& tx_coord = CCSSP::DB_CDATA_ALL_OUTER_KEYS, 
                                   double bearing = CCSSP::DB_CDATA_ALL_MEDIUM_KEYS, 
                                   double range = CCSSP::DB_CDATA_ALL_INNER_KEYS,
                                   const Time& time_value = CCSSP::DB_CDATA_ALL_TIME_KEYS );
                                     
    /**
    * Sets the custom Bathymetry for given generator Coord
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @returns if insertion succeed, the function returns a NULL pointer, that needs to be initialized.
    *          if an object was already instantiated, the function returns the pointer to it;
    **/
    bool setCustomBathymetry( Bathymetry* const bathymetry, const Coord& tx_coord = CCBathymetry::DB_CDATA_ALL_OUTER_KEYS, 
                                   double bearing = CCBathymetry::DB_CDATA_ALL_MEDIUM_KEYS, 
                                   double range = CCBathymetry::DB_CDATA_ALL_INNER_KEYS );
       
    /**
    * Imports a CustomBathymetry from file. The file has to be a two column format: Range [m] Depth [m]
    * @param filename const reference to a string
		* @param tx_coord const reference to a valid Coord (generator coordinates)
		* @param bearing bearing value [radians]
    * @returns true if import was successful, false otherwise
    **/
    virtual bool importCustomBathymetry( const ::std::string& filename, const Coord& tx_coord = CCBathymetry::DB_CDATA_ALL_OUTER_KEYS, 
                                         double bearing = CCBathymetry::DB_CDATA_ALL_MEDIUM_KEYS );      
   
    /**
    * Gets the custom Bathymetry for given generator Coord
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @returns NULL pointer if parameters are not found.
    **/
    Bathymetry* getCustomBathymetry( const Coord& tx_coord = CCBathymetry::DB_CDATA_ALL_OUTER_KEYS, 
                                     double bearing = CCBathymetry::DB_CDATA_ALL_MEDIUM_KEYS, 
                                     double range = CCBathymetry::DB_CDATA_ALL_INNER_KEYS );

    /**
    * Erases the custom Bathymetry for given parameters
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @return reference to <b>*this</b>
    **/
    WossDbManager& eraseCustomBathymetry( const Coord& tx_coord = CCBathymetry::DB_CDATA_ALL_OUTER_KEYS, 
                                          double bearing = CCBathymetry::DB_CDATA_ALL_MEDIUM_KEYS, 
                                          double range = CCBathymetry::DB_CDATA_ALL_INNER_KEYS );
    

    WossDbManager& setDebug( bool flag ) { debug = flag; updateDebugFlag(); return *this; }
       
    bool getDebug() { return debug; }
    
    
    protected:
   
    
    /**
    * Bathymetry database pointer
    **/
    WossBathymetryDb* bathymetry_db;

    /**
    * Sediment database pointer
    **/
    WossSedimentDb* sediment_db;

    /**
    * SSP database pointer
    **/
    WossSSPDb* ssp_db;

    /**
    * TimeArr database pointer
    **/
    WossResTimeArrDb* results_arrivals_db;
  
    /**
    * Pressure database pointer
    **/
    WossResPressDb* results_pressure_db;


    /**
    * Debug flag
    **/
    bool debug;

    
    /**
    * CustomBathymetry containter for user-given generator CoordZ
    **/
    CCBathymetry ccbathy_map;
    
    /**
    * custom Sediment containter for user-given generator CoordZ
    **/
    CCSediment ccsediment_map;
    
    /**
    * CustomSSP containter for user-given generator CoordZ
    **/
    CCSSP ccssp_map;

    /**
    * CustomAltimetry containter for user-given generator CoordZ
    **/
    CCAltimetry ccaltimetry_map;
    
    /**
    * Closes all connections of owned databases
    **/
    virtual bool closeAllConnections();   
    
    virtual void updateDebugFlag();
    
  };

  
  //inline functions
  /////////////  
  inline bool WossDbManager::setCustomAltimetry( Altimetry* const altimetry, const Coord& tx_coord, double bearing, double range ) {
    return ccaltimetry_map.insert( altimetry, tx_coord, bearing, range );
  }
      
      
  inline Altimetry* WossDbManager::getCustomAltimetry( const Coord& tx_coord, double bearing, double range ) {
    return ccaltimetry_map.get( tx_coord, bearing, range );
  }
  

  inline WossDbManager& WossDbManager::eraseCustomAltimetry( const Coord& tx_coord, double bearing, double range ) {
    ccaltimetry_map.erase( tx_coord, bearing, range );
    return *this;
  }


  inline bool WossDbManager::setCustomSediment( Sediment* const sediment, const Coord& tx_coord, double bearing, double range ) {
    return ccsediment_map.insert( sediment, tx_coord, bearing, range );
  }
      
      
  inline Sediment* WossDbManager::getCustomSediment( const Coord& tx_coord, double bearing, double range ) {
    return ccsediment_map.get( tx_coord, bearing, range );
  }


  inline WossDbManager& WossDbManager::eraseCustomSediment( const Coord& tx_coord, double bearing, double range ) {
    ccsediment_map.erase( tx_coord, bearing, range );
    return *this;
  }


  inline bool WossDbManager::setCustomSSP( SSP* const ssp, const Coord& tx_coord, double bearing, double range, const Time& time_key ) {
    return ccssp_map.insert( ssp, tx_coord, bearing, range, time_key );
  }
      
      
  inline SSP* WossDbManager::getCustomSSP( const Coord& tx_coord, double bearing, double range, const Time& time_key ) {
    return ccssp_map.get( tx_coord, bearing, range, time_key );
  }


inline WossDbManager& WossDbManager::eraseCustomSSP( const Coord& tx_coord, double bearing, double range, const Time& time_key ) {
    ccssp_map.erase( tx_coord, bearing, range, time_key );
    return *this;
  }


  inline bool WossDbManager::setCustomBathymetry( Bathymetry* const bathy, const Coord& tx_coord, double bearing, double range ) {
    return ccbathy_map.insert(*bathy, tx_coord, bearing, range );
  }
      
      
  inline Bathymetry* WossDbManager::getCustomBathymetry( const Coord& tx_coord, double bearing, double range ) {
    return const_cast< Bathymetry* >( ccbathy_map.get( tx_coord, bearing, range ) );
  }


  inline WossDbManager& WossDbManager::eraseCustomBathymetry( const Coord& tx_coord, double bearing, double range ) {
    ccbathy_map.erase( tx_coord, bearing, range );
    return *this;
  }
  

}


#endif /* WOSS_DB_MANAGER_IMPLEMENT_H */

 
 
