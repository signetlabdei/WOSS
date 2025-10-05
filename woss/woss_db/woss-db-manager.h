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
 * @file   woss-db-manager.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossDbManager class
 *
 * Provides the interface for the woss::WossDbManager class
 */


#ifndef WOSS_DB_MANAGER_IMPLEMENT_H 
#define WOSS_DB_MANAGER_IMPLEMENT_H


#include <memory>
#include <string>
#include <definitions.h>
#include <definitions-handler.h>
#include "woss-db-custom-data-container.h"
#include "woss-db.h"


namespace woss {

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

    using CCBathymetry = CustomDataContainer< Coord, BearingOperator, RangeOperator, Bathymetry >;

    using CCSSP = CustomDataTimeContainer< Coord, BearingOperator, RangeOperator, std::shared_ptr<SSP> >;

    using CCSediment = CustomDataContainer< Coord, BearingOperator, RangeOperator, std::shared_ptr<Sediment> >;

    using CCAltimetry = CustomDataContainer< Coord, BearingOperator, RangeOperator, std::shared_ptr<Altimetry> >;

    /**
    * Default constructor
    **/
    WossDbManager();

    WossDbManager( const WossDbManager& instance ) = default;

    WossDbManager( WossDbManager&& instance ) = default;

    WossDbManager& operator=( const WossDbManager& instance ) = default;

    WossDbManager& operator=( WossDbManager&& instance ) = default;

    /**
    * Destructor. It deletes all pointers involved. Derived classes 
    * <b>don't have to do it</b>
    **/ 
    virtual ~WossDbManager() = default;

    /**
    * Returns a std::unique_ptr to a heap-created Altimetry value for given coordinates and depth, 
    * if present in the Altimetry database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coords const reference to a valid CoordZ object
    * @return std::unique_ptr to a <i>valid</i> Altimetry if coordinates are found, <i>not valid</i> otherwise
    **/
    virtual std::unique_ptr<Altimetry> getAltimetry( const CoordZ& tx, const CoordZ& rx ) const;
    
    
    /**
    * Returns a std::unique_ptr to a heap-created Sediment value for given coordinates and depth, 
    * if present in the Sediment database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coords const reference to a valid CoordZ object
    * @return std::unique_ptr to a <i>valid</i> Sediment if coordinates are found, <i>not valid</i> otherwise
    **/
    virtual std::unique_ptr<Sediment> getSediment( const CoordZ& tx, const CoordZ& rx ) const;

    /**
    * Returns the std::unique_ptr to a Sediment value of given coordinates and depth vector, 
    * if at least one set of coordinates is present in the Sediment database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coordz_vector const reference to a valid CoordZ vector
    * @return std::unique_ptr to a <i>valid</i> Sediment if at least one set of coordinates is found, <i>not valid</i> otherwise
    **/
    virtual std::unique_ptr<Sediment> getSediment( const CoordZ& tx, const CoordZVector& rx_coordz_vector ) const;


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
    * @return a std::unique_ptr to a <i>valid</i> SSP if coordinates are found, <i>not valid</i> otherwise
    **/  
    virtual std::unique_ptr<SSP> getSSP( const Coord& tx, const Coord& rx, const Time& time, long double ssp_depth_precision = SSP_CUSTOM_DEPTH_PRECISION ) const;

    /**
    * Returns a std::unique_ptr to a heap-created average SSP for given coordinates, start and end time date 
    * if they are present in the database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coords const reference to a valid Coord object
    * @param time_start const reference to a valid Time object
    * @param time_end const reference to a valid Time object
    * @param max_time_values total number of Time to take between time_start and time_end
    * @param ssp_depth_precision returned SSP's depth precision
    * @return a std::unique_ptr to a <i>valid</i> SSP if coordinates and both time date are found, <i>not valid</i> otherwise
    **/  
    virtual std::unique_ptr<SSP> getAverageSSP( const Coord& tx, const Coord& rx, const Time& time_start, const Time& time_end, int max_time_values, long double ssp_depth_precision = SSP_CUSTOM_DEPTH_PRECISION ) const;


    /**
    * Returns a std::unique_ptr to a heap-created TimeArr value of given frequency, transmitter and receiver 
    * coordinates if present in the database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coord_tx const reference to a valid CoordZ object
    * @param coord_rx const reference to a valid CoordZ object
    * @param frequency used frequency [hz]
    * @param time_value const reference to a valid Time object
    * @return std::unique_ptr to a <i>valid</i> TimeArr if parameters are found, <i>not valid</i> otherwise
    **/  
    virtual std::unique_ptr<TimeArr> getTimeArr( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value ) const;

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
    * Returns a std::unique_ptr to a heap-created Pressure value of given frequency, 
    * transmitter and receiver coordinates if present in the database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coord_tx const reference to a valid CoordZ object
    * @param coord_rx const reference to a valid CoordZ object
    * @param frequency used frequency [hz]
    * @param time_value const reference to a valid Time object
    * @return std::unique_ptr to a <i>valid</i> Pressure if parameters are found, <i>not valid</i> otherwise
    **/  
    virtual std::unique_ptr<Pressure> getPressure( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value ) const;

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
    * Initializes the bathymetry database pointer. 
    * WossDbManager takes the ownership of the Db.
    * @param ptr valid std::unique_ptr WossBathymetryDb
    * @return reference to <b>*this</b>
    **/
    WossDbManager& setBathymetryDb( std::unique_ptr<WossBathymetryDb> ptr ) { bathymetry_db = std::move(ptr); return *this; }

    /**
    * Initializes the Sediment database pointer.
    * WossDbManager takes the ownership of the Db.
    * @param ptr std::unique_ptr to a valid WossSedimentDb
    * @return reference to <b>*this</b>
    **/
    WossDbManager& setSedimentDb( std::unique_ptr<WossSedimentDb> ptr ) { sediment_db = std::move(ptr); return *this; }

    /**
    * Initializes the SSP database pointer
    * WossDbManager takes the ownership of the Db.
    * @param ptr std::unique_ptr to a valid WossSSPDb
    * @return reference to <b>*this</b>
    **/
    WossDbManager& setSSPDb( std::unique_ptr<WossSSPDb> ptr ) { ssp_db = std::move(ptr); return *this; }

    /**
    * Initializes the TimeArr database pointer
    * WossDbManager takes the ownership of the Db.
    * @param ptr std::unique_ptr to a valid WossResTimeArrDb
    * @return reference to <b>*this</b>
    **/
    WossDbManager& setResTimeArrDb( std::unique_ptr<WossResTimeArrDb> ptr ) { results_arrivals_db = std::move(ptr); return *this; }
  
    /**
    * Initializes the Pressure database pointer
    * WossDbManager takes the ownership of the Db.
    * @param ptr std::unique_ptr to a valid WossResPressDb
    * @return reference to <b>*this</b>
    **/
    WossDbManager& setResPressureDb( std::unique_ptr<WossResPressDb> ptr ) { results_pressure_db = std::move(ptr); return *this; }

    /**
    * Sets the custom Altimetry for given generator Coord
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @returns <i>true</i> if method succed, <i>false</i> otherwise
    **/
    bool setCustomAltimetry( const Altimetry& altimetry, const Coord& tx_coord = CCAltimetry::DB_CDATA_ALL_OUTER_KEYS, 
                            double bearing = CCAltimetry::DB_CDATA_ALL_MEDIUM_KEYS, 
                            double range = CCAltimetry::DB_CDATA_ALL_INNER_KEYS );
      
    /**
    * Gets the custom Altimetry for given generator Coord
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @returns std::unique_ptr to an Altimetry value. The object will be not valid if the input is not found
    **/
    std::unique_ptr<Altimetry> getCustomAltimetry( const Coord& tx_coord = CCAltimetry::DB_CDATA_ALL_OUTER_KEYS, 
                                 double bearing = CCAltimetry::DB_CDATA_ALL_MEDIUM_KEYS, 
                                 double range = CCAltimetry::DB_CDATA_ALL_INNER_KEYS ) const;
    
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
    * @returns if insertion succeed, the function returns a nullptr pointer, that needs to be initialized.
    *          if an object was already instantiated, the function returns the pointer to it;
    **/
    bool setCustomSediment( const Sediment& sediment, const Coord& tx_coord = CCSediment::DB_CDATA_ALL_OUTER_KEYS, 
                            double bearing = CCSediment::DB_CDATA_ALL_MEDIUM_KEYS, 
                            double range = CCSediment::DB_CDATA_ALL_INNER_KEYS );

    /**
    * Converts the input :std::string into a woss::Sediment object and binds it to the designed geometry.
    * \param sediment_string SSP in string format. syntax:
    *  - Sed_typename|SSP[m/s]|vel_c[m/s]|vel_s[m/s]|dens[g/cm^3]|att_c[db/wavelength]|att_s[db/wavelength]|bottom_depth[m]
    * \param txCoord the geometry originating geographical coordinates
    * \param bearing the geometry bearing [radians]
    * \param range the geometry range
    * \returns true if successful, false otherwise
    */
    bool setCustomSediment( const std::string& sediment_string, 
                            const Coord& txCoord = CCSediment::DB_CDATA_ALL_OUTER_KEYS,
                            double bearing = CCSediment::DB_CDATA_ALL_MEDIUM_KEYS,
                            double range = CCSediment::DB_CDATA_ALL_INNER_KEYS );

    /**
    * Gets the custom Sediment for given generator Coord
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @returns std::unique_ptr to a Sediment value. The object will be not valid if the input is not found
    **/
    std::unique_ptr<Sediment> getCustomSediment( const Coord& tx_coord = CCSediment::DB_CDATA_ALL_OUTER_KEYS, 
                                 double bearing = CCSediment::DB_CDATA_ALL_MEDIUM_KEYS, 
                                 double range = CCSediment::DB_CDATA_ALL_INNER_KEYS ) const;

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
    * @returns if insertion succeed, the function returns a nullptr pointer, that needs to be initialized.
    *          if an object was already instantiated, the function returns the pointer to it;
    **/
    bool setCustomSSP( const SSP& ssp, const Coord& tx_coord = CCSSP::DB_CDATA_ALL_OUTER_KEYS, 
                                       double bearing = CCSSP::DB_CDATA_ALL_MEDIUM_KEYS, 
                                       double range = CCSSP::DB_CDATA_ALL_INNER_KEYS,
                                       const Time& time_value = CCSSP::DB_CDATA_ALL_TIME_KEYS );

    /**
    * Converts the input std::string into a custom woss::SSP object and assign it to the designed geometry
    * and simulation time validity.
    * \param sspString SSP in string format. syntax:
    *  - tot_values|depth_1[m]|SSP[m/s]|....|depth_final[m]|SSP_final[m/s]
    * \param txCoord the geometry originating geographical coordinates
    * \param bearing the geometry bearing [radians]
    * \param range the geometry range
    * \param time_value the geometry time evolution step
    * \returns true if successful, false otherwise
    */
    bool setCustomSSP( const std::string& sspString, const Coord& txCoord = CCSSP::DB_CDATA_ALL_OUTER_KEYS,
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
    virtual bool importCustomSSP( const std::string& filename, const Time& = CCSSP::DB_CDATA_ALL_TIME_KEYS,
	                                const Coord& tx_coord = CCSSP::DB_CDATA_ALL_OUTER_KEYS, double bearing = CCSSP::DB_CDATA_ALL_MEDIUM_KEYS );

    /**
    * Gets the custom SSP for given generator Coord
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @param time_value const reference to a valid Time object
    * @returns std::unique_ptr to an SSP value. The object will be not valid if the input is not found
    **/
    std::unique_ptr<SSP> getCustomSSP( const Coord& tx_coord = CCSSP::DB_CDATA_ALL_OUTER_KEYS, 
                                      double bearing = CCSSP::DB_CDATA_ALL_MEDIUM_KEYS, 
                                      double range = CCSSP::DB_CDATA_ALL_INNER_KEYS,
                                      const Time& time_value = CCSSP::DB_CDATA_ALL_TIME_KEYS ) const;

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
    * @returns if insertion succeed, the function returns a nullptr pointer, that needs to be initialized.
    *          if an object was already instantiated, the function returns the pointer to it;
    **/
    bool setCustomBathymetry( const Bathymetry& bathymetry, const Coord& tx_coord = CCBathymetry::DB_CDATA_ALL_OUTER_KEYS, 
                                   double bearing = CCBathymetry::DB_CDATA_ALL_MEDIUM_KEYS, 
                                   double range = CCBathymetry::DB_CDATA_ALL_INNER_KEYS );

  /**
   * Reads the input std::string and converts it into a custom woss::Bathymetry object and binds it to
   * the input geometry
   * \param bathyLine bathymetry in string format. syntax:
   *  - tot_ranges|range_1[m]|dept_1[n]|....|range_final[m]|depth_final[m]
   * \param txCoord the geometry originating geographical coordinates
   * \param bearing the geometry bearing [radians]
   * \returns true if successful, false otherwise
   */
    bool setCustomBathymetry (const std::string &bathyLine, const Coord& tx_coord = CCBathymetry::DB_CDATA_ALL_OUTER_KEYS, 
                              double bearing = CCBathymetry::DB_CDATA_ALL_MEDIUM_KEYS);

    /**
    * Imports a CustomBathymetry from file. The file has to be a two column format: Range [m] Depth [m]
    * @param filename const reference to a string
		* @param tx_coord const reference to a valid Coord (generator coordinates)
		* @param bearing bearing value [radians]
    * @returns true if import was successful, false otherwise
    **/
    virtual bool importCustomBathymetry( const std::string& filename, const Coord& tx_coord = CCBathymetry::DB_CDATA_ALL_OUTER_KEYS, 
                                         double bearing = CCBathymetry::DB_CDATA_ALL_MEDIUM_KEYS );
   
    /**
    * Gets the custom Bathymetry for given generator Coord
    * @param tx_coord const reference to a valid Coord (generator coordinates)
    * @param bearing bearing value [radians]
    * @param range range value [m]
    * @returns std::unique_ptr to an Altimetry value. The object will be not valid if the input is not found
    **/
    Bathymetry getCustomBathymetry( const Coord& tx_coord = CCBathymetry::DB_CDATA_ALL_OUTER_KEYS, 
                                     double bearing = CCBathymetry::DB_CDATA_ALL_MEDIUM_KEYS, 
                                     double range = CCBathymetry::DB_CDATA_ALL_INNER_KEYS ) const;

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

    bool getDebug() const { return debug; }

    protected:

    /**
    * Bathymetry database pointer
    **/
    std::unique_ptr<WossBathymetryDb> bathymetry_db;

    /**
    * Sediment database pointer
    **/
    std::unique_ptr<WossSedimentDb> sediment_db;

    /**
    * SSP database pointer
    **/
    std::unique_ptr<WossSSPDb> ssp_db;

    /**
    * TimeArr database pointer
    **/
    std::unique_ptr<WossResTimeArrDb> results_arrivals_db;
  
    /**
    * Pressure database pointer
    **/
    std::unique_ptr<WossResPressDb> results_pressure_db;

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
  inline bool WossDbManager::setCustomAltimetry( const Altimetry& altimetry, const Coord& tx_coord, double bearing, double range ) {
    return ccaltimetry_map.insert( altimetry.clone(), tx_coord, bearing, range );
  }


  inline std::unique_ptr<Altimetry> WossDbManager::getCustomAltimetry( const Coord& tx_coord, double bearing, double range ) const {
    auto ret = ccaltimetry_map.get( tx_coord, bearing, range );
    if (ret)
      return (*ret)->clone();
    else
      return( SDefHandler::instance().createAltimetry( Altimetry::createNotValid() ) );
  }


  inline WossDbManager& WossDbManager::eraseCustomAltimetry( const Coord& tx_coord, double bearing, double range ) {
    ccaltimetry_map.erase( tx_coord, bearing, range );
    return *this;
  }


  inline bool WossDbManager::setCustomSediment( const Sediment& sediment, const Coord& tx_coord, double bearing, double range ) {
    return ccsediment_map.insert( sediment.clone(), tx_coord, bearing, range );
  }


  inline std::unique_ptr<Sediment> WossDbManager::getCustomSediment( const Coord& tx_coord, double bearing, double range ) const {
    auto ret = ccsediment_map.get( tx_coord, bearing, range );
    if (ret)
      return (*ret)->clone();
    else
      return( SDefHandler::instance().createSediment() );
  }


  inline WossDbManager& WossDbManager::eraseCustomSediment( const Coord& tx_coord, double bearing, double range ) {
    ccsediment_map.erase( tx_coord, bearing, range );
    return *this;
  }


  inline bool WossDbManager::setCustomSSP( const SSP& ssp, const Coord& tx_coord, double bearing, double range, const Time& time_key ) {
    return ccssp_map.insert( ssp.clone(), tx_coord, bearing, range, time_key );
  }
      
      
  inline std::unique_ptr<SSP> WossDbManager::getCustomSSP( const Coord& tx_coord, double bearing, double range, const Time& time_key ) const {
    auto ret = ccssp_map.get( tx_coord, bearing, range, time_key);
    if (ret)
      return (*ret)->clone();
    else
      return( SDefHandler::instance().createSSP() );
  }


  inline WossDbManager& WossDbManager::eraseCustomSSP( const Coord& tx_coord, double bearing, double range, const Time& time_key ) {
    ccssp_map.erase( tx_coord, bearing, range, time_key );
    return *this;
  }


  inline bool WossDbManager::setCustomBathymetry( const Bathymetry& bathy, const Coord& tx_coord, double bearing, double range ) {
    return ccbathy_map.insert(bathy, tx_coord, bearing, range );
  }
      
      
  inline Bathymetry WossDbManager::getCustomBathymetry( const Coord& tx_coord, double bearing, double range ) const {
    auto ret = ccbathy_map.get( tx_coord, bearing, range);
    if (ret)
      return (*ret);
    else
      return( Bathymetry() );
  }


  inline WossDbManager& WossDbManager::eraseCustomBathymetry( const Coord& tx_coord, double bearing, double range ) {
    ccbathy_map.erase( tx_coord, bearing, range );
    return *this;
  }

}

#endif /* WOSS_DB_MANAGER_IMPLEMENT_H */

