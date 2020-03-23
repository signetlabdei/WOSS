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
 * @file   woss.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::Woss and woss::WossResReader classes
 *
 * Provides the interface for the woss::Woss and woss::WossResReader classes
 */


#ifndef WOSS_PROGRAM_DEFINITIONS_H
#define WOSS_PROGRAM_DEFINITIONS_H


#include <set>
#include <vector>
#include <map>
#include <climits>
#include <coordinates-definitions.h>
#include <time-definitions.h>
#include "res-reader.h"


#ifdef WOSS_MULTITHREAD
#include <pthread.h>
#endif // WOSS_MULTITHREAD


namespace woss {
  
  
  class WossDbManager;

  /**
  * Vector of range values [m]
  **/
  typedef ::std::vector< double > RangeVector;


  /**
  * Set of frequencies [Hz]
  **/
  typedef ::std::set< double > FreqSet;
  typedef FreqSet::iterator FreqSIt;
  typedef FreqSet::const_iterator FreqSCIt;
  typedef FreqSet::reverse_iterator FreqSRIt;
  typedef FreqSet::const_reverse_iterator FreqSCRIt;


  static const int WOSS_MIN_DEPTH = 0;            /**< Default miminum depth value [m] */

  static const int WOSS_MAX_DEPTH = INT_MAX;      /**< Default maximum depth value [m] */

  static const int WOSS_MIN_RANGE = -INT_MAX;     /**< Default miminum range value [m] */

  static const int WOSS_MAX_RANGE = INT_MAX;      /**< Default maximum range value [m] */


  /**
  * \brief Abstract class that provides the interface for initializing and running a channel simulator
  *
  * Woss class has the task to properly initialize and run a channel simulator and to provide its results 
  * ( with the optional aid of ResReader class ).
  **/
  class Woss {

    
    public:


    /**
    * Woss default constructor. Default constructed objects are not valid
    **/
    Woss();

    /**
    * Woss constructor
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_t const reference to a valid Time object for SSP's averaging purposes ( start date time )
    * @param end_t const reference to a valid Time object for SSP's averaging purposes ( end date time )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param freq_step frequency step [Hz]
    **/
    Woss( const CoordZ& tx, const CoordZ& rx, const Time& start_t, const Time& end_t, double start_freq, double end_freq, double freq_step ) ;

    virtual ~Woss();


    /**
    * Initializes the channel simulator
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool initialize() = 0;

    
    /**
    * Runs the channel simulator. It is mandatory to set <b>is_running</b> to <i>true</i> at the beginning
    * of this function and set it to <i>false</i> before returning.
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/   
    virtual bool run() = 0;

    
    /**
    * Performs a time evoulion of all time-dependant parameters
    * @param time_value constant reference to a valid Time object ( between start_time and end_time)
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/   
    virtual bool timeEvolve( const Time& time_value ) = 0;
    

    /**
    * Checks the validity of Woss
    * @return <i>true</i> if it's valid, <i>false</i> otherwise
    **/
    virtual bool isValid() const = 0;

    
    /**
    * Gets the average Pressure value in given rx range-depth box
    * @param frequency frequency [Hz]
    * @param tx_depth transmitter depth [m]
    * @param start_rx_depth start receiver depth [m]
    * @param start_rx_range start receiver range [m]
    * @param end_rx_depth end receiver depth [m]
    * @param end_rx_range end receiver range [m]
    * @return a valid Pressure value 
    **/
    virtual Pressure* getAvgPressure( double frequency, double tx_depth, double start_rx_depth = WOSS_MIN_DEPTH, double start_rx_range = WOSS_MIN_RANGE, double end_rx_depth = WOSS_MAX_DEPTH , double end_rx_range = WOSS_MAX_RANGE ) const = 0;

    /**
    * Gets a Pressure value of given range, depths
    * @param frequency frequency [Hz]
    * @param tx_depth transmitter depth [m]
    * @param rx_depth receiver depth [m]
    * @param rx_range receiver range [m]
    * @return a valid Pressure value 
    **/
    virtual Pressure* getPressure( double frequency, double tx_depth, double rx_depth, double rx_range ) const = 0;

  
    /**
    * Gets a TimeArr value of given range, depths
    * @param frequency frequency [Hz]
    * @param tx_depth transmitter depth [m]
    * @param rx_depth receiver depth [m]
    * @param rx_range receiver range [m]
    * @return a valid Pressure value 
    **/
    virtual TimeArr* getTimeArr( double frequency, double tx_depth, double rx_depth, double rx_range ) const = 0;


    /**
    * Sets debug flag
    * @param flag debug flag
    * @return reference to <b>*this</b>
    **/
    Woss& setDebug( bool flag ) { debug = flag; return *this; }

    /**
    * Sets clean work dir flag
    * @param flag debug flag
    * @return reference to <b>*this</b>
    **/
    Woss& setCleanWorkDir( bool flag ) { clean_workdir = flag; return *this; }
    
    
    /**
    * Sets the work pathname
    * @param path valid pathname
    * @return reference to <b>*this</b>
    **/
    Woss& setWorkDirPath( const ::std::string& path ) { work_dir_path = path; return *this; }

    /**
    * Sets the WossDbManager pointer
    * @param path const pointer to a const WossDbManager object
    * @return reference to <b>*this</b>
    **/
    Woss& setWossDbManager( const WossDbManager* const ptr ) { db_manager = ptr; return *this; }


    /**
    * Insert a frequency value into the FreqSet
    * @param freq a valid frequency value [Hz]
    * @return reference to <b>*this</b>
    **/
    Woss& insertFrequency( double freq ) { frequencies.insert(freq); return *this; } 

    /**
    * Insert a range of valid frequencies
    * @param freq_start a valid frequency value [Hz]
    * @param freq_end a valid frequency value [Hz] >= freq_start
    * @param freq_step a valid frequency value [Hz]
    * @return reference to <b>*this</b>
    **/
    Woss& insertFrequencies( double freq_start, double freq_end, double freq_step );

    /**
    * Copys the given FreqSet  
    * @param freq_set const reference to a FreqSet
    * @return reference to <b>*this</b>
    **/
    Woss& setFrequencies( const FreqSet& freq_set ) { frequencies = freq_set; return *this; }

    /**
    * Erases the given frequency from the FreqSet  
    * @param freq frequency value [Hz]
    * @return reference to <b>*this</b>
    **/
    Woss& eraseFrequency( double freq ) { frequencies.erase(freq); return *this; }

    /**
    * Erases all frequencies
    * @return reference to <b>*this</b>
    **/
    Woss& clearFrequencies() { frequencies.clear(); return *this; }


    /**
    * Sets the total number of channel simulator's runs 
    * @param runs number of runs
    * @return reference to <b>*this</b>
    **/
    Woss& setTotalRuns( int runs ) { total_runs = runs; return *this; }

    /**
    * Sets transmitter CoordZ
    * @param coordz const reference to a valid CoordZ object
    * @return reference to <b>*this</b>
    **/
    Woss& setTxCoordZ( const CoordZ& coordz ) { tx_coordz = coordz; return *this; }

    /**
    * Sets receiver CoordZ
    * @param coordz const reference to a valid CoordZ object
    * @return reference to <b>*this</b>
    **/
    Woss& setRxCoordZ( const CoordZ& coordz ) { rx_coordz = coordz; return *this; }


    /**
    * Sets start date time
    * @param coordz const reference to a valid Time object
    * @return reference to <b>*this</b>
    **/
    Woss& setStartTime( const Time& start_t ) { start_time = start_t; return *this; }

    /**
    * Sets end date time
    * @param coordz const reference to a valid Time object
    * @return reference to <b>*this</b>
    **/
    Woss& setEndTime( const Time& end_t ) { end_time = end_t; return *this; }  

    /**
    * Sets time evolution threshold
    * @param value time threshold in seconds.
    * @return reference to <b>*this</b>
    **/
    Woss& setEvolutionTimeQuantum( double value ) { evolution_time_quantum = value; return *this; } 
    
    
    /**
    * Returns the instance identifier
    * @returns id number
    **/
    int getWossId() const { return woss_id; }

    /**
    * Returns the work pathname
    * @returns string 
    **/
    ::std::string getWorkDirPath() const { return work_dir_path; }


    /**
    * Returns the FreqSet in use
    * @returns const reference to frequencies
    **/
    const FreqSet& getFrequencies() const { return frequencies; }

    /**
    * Returns the minimum frequency in use
    * @returns frequency value [Hz]
    **/
    double getMinFrequency() const { return( *( frequencies.begin() ) ); }
    
    /**
    * Returns the maximum frequency in use
    * @returns frequency value [Hz]
    **/
    double getMaxFrequency() const { return( *( frequencies.rbegin() ) ); } 
    

    /**
    * Returns a const iterator to the beginning of FreqSet in use
    * @returns const FreqSet iterator
    **/
    FreqSCIt freq_begin() const { return( frequencies.begin() ); } 

    /**
    * Returns a const iterator to the end of FreqSet in use
    * @returns const FreqSet iterator
    **/
    FreqSCIt freq_end() const { return( frequencies.end() ); } 
    
    /**
    * Returns a const iterator to the reverse beginning of FreqSet in use
    * @returns const FreqSet reverse iterator
    **/
    FreqSCRIt freq_rbegin() const { return( frequencies.rbegin() ); }

    /**
    * Returns a const iterator to the reverse of FreqSet in use
    * @returns const FreqSet reverse iterator
    **/
    FreqSCRIt freq_rend() const { return( frequencies.rend() ); }

    /**
    * Returns a const iterator to the value >= <i>frequency</i> parameter
    * @param frequency const reference to a frequency value [Hz]
    * @returns const iterator to end() if <i>frequency</i> is not found
    **/
    FreqSCIt freq_lower_bound( double frequency ) const { return( frequencies.lower_bound( frequency ) ); } 

    /**
    * Returns a const iterator to the value > <i>frequency</i> parameter
    * @param frequency const reference to a frequency value [Hz]
    * @returns const iterator to end() if <i>frequency</i> is not found
    **/
    FreqSCIt freq_upper_bound( double frequency ) const { return( frequencies.upper_bound( frequency ) ); } 


    /**
    * Gets the total number of channel simulator's runs 
    * @returns total number of runs
    **/
    int getTotalRuns() const { return total_runs; }
    
    
    /**
    * Gets transmitter CoordZ
    * @returns valid CoordZ object
    **/
    CoordZ getTxCoordZ() const { return tx_coordz; }
  
    /**
    * Gets receiver CoordZ
    * @returns valid CoordZ object
    **/
    CoordZ getRxCoordZ() const { return rx_coordz; }


    /**
    * Gets start date time
    * @returns a valid Time object
    **/
    Time getStartTime() const { return start_time; }

    /**
    * Gets start date time
    * @returns a valid Time object
    **/
    Time getCurrentTime() const { return current_time; }

    /**
    * Gets end date time
    * @returns a valid Time object
    **/
    Time getEndTime() const { return end_time; } 

    /**
    * Gets the evolution time threshold
    * @returns time [s]
    **/    
    double getEvolutionTimeQuantum() const { return evolution_time_quantum; }
    
    /**
    * Gets the surface great-circle distance between transmitter and receiver
    * @returns distance value [m]
    * @see Coord::getGreatCircleDistance()
    **/
    double getGreatCircleDistance() const { return total_great_circle_distance; }

    /**
    * Gets the distance between transmitter and receiver
    * @returns distance value [m]
    **/
    double getDistance() const { return total_distance; }

    /**
    * Gets the bearing between transmitter and receiver
    * @returns bearing [radians]
    **/
    double getBearing() const { return bearing; }

    /**
    * Checks if instance is using debug
    **/
    bool usingDebug() const { return debug; }

    /**
    * Checks if instance is already running the channel simulator
    **/
    virtual bool isRunning() const;
    
    
    protected:


    #ifdef WOSS_MULTITHREAD    
    /**
    * Spinlock for syncronization purposes
    **/
    static pthread_spinlock_t woss_mutex;
    
    friend void destroyWossSpinlock();

    #endif // WOSS_MULTITHREAD
    
    /**
    * Unique instances id-counter
    **/
    static int woss_counter;

    /**
    * id of specific instance
    **/
    int woss_id;


    /**
    * Directory path for temporary files (e.g. channel simulator files)
    **/
    ::std::string work_dir_path;


    /**
    * Constant pointer to the DbManager instance
    **/
    const WossDbManager* db_manager;


    /**
    * Valid start date time value, for SSP averaging purposes
    **/
    Time start_time;
    
    /**
     * Valid current simulated time (between start and end time)
     */
    Time current_time;

    /**
    * Valid end date time value, for SSP averaging purposes
    **/
    Time end_time;

    /**
    * Time threshold in seconds. For time evolution purposes. 
    * A value < 0 ==> evoulion is off
    * A value == 0 ==> no threshold
    **/
    double evolution_time_quantum;

    /**
    * Transmitter CoordZ
    **/
    CoordZ tx_coordz;

    /**
    * Receiver CoordZ
    **/
    CoordZ rx_coordz;

    
    /**
    * Frequencies in use
    **/
    FreqSet frequencies;
    
    
    /**
    * Initial bearing between tx_coordz and rx_coordz [radians]
    * @see Coord::getInitialBearing()
    **/ 
    double bearing;

    /**
    * Surface great-circle distance between tx_coordz and rx_coordz [m]
    * @see Coord::getGreatCircleDistance()
    **/ 
    double total_great_circle_distance;

    /**
    * Cartesian distance between tx_coordz and rx_coordz [m]
    * @see CoordZ::getDistance()
    **/ 
    double total_distance;

    /**
    * Total number of channel simulator's runs 
    **/
    int total_runs;
    

    /**
    * Debug flag
    **/
    bool debug;
 
    
    bool has_run_once;
    
    
    /**
    * Running flag
    **/
    volatile bool is_running;

    /**
    * flag for removing working dir
    */
    bool clean_workdir;    
    
    /**
    * Creates the temporary work directory
    * @param curr_frequency frequency in use [Hz]
    * @param curr_run current run value < total_runs
    * @returns <i>true</i> if method succeeded, <i>false</i> otherwise
    **/
    virtual bool mkWorkDir( double curr_frequency, int curr_run = 0 );

    /**
    * Removes the temporary work directory
    * @param curr_frequency frequency in use [Hz]
    * @param curr_run current run value < total_runs
    * @returns <i>true</i> if method succeeded, <i>false</i> otherwise
    **/
    virtual bool rmWorkDir( double curr_frequency, int curr_run = 0 );
    
    virtual bool rmWorkDir();
    
    
  };

  
  #ifdef WOSS_MULTITHREAD    

  /**
  * Function used to destroy the static pthread_spin_t woss_mutex
  **/
  void destroyWossSpinlock();
  
  #endif // WOSS_MULTITHREAD

  
  /**
  * Map that links a frequency [Hz] to a ResReader instance
  **/
  typedef ::std::map< double, ResReader* > ResReaderMap;
  typedef ResReaderMap::iterator RRMIter;
  typedef ResReaderMap::reverse_iterator RRMRIter;
  typedef ResReaderMap::const_iterator RRMCIter;
  typedef ResReaderMap::const_reverse_iterator RRMCRIter;
  typedef ::std::pair< RRMIter, bool > RRMPair;


  /**
  * \brief Woss class with ResReader objects for reading simulated results 
  *
  * WossResReader implements Woss with the add of a ResReaderMap object for reading and further refination purposes
  * 
  **/
  class WossResReader : public Woss { 

    
    public: 
    
      
    /**
    * Default constructor
    **/    
    WossResReader() : Woss() { }

    /**
    * Woss constructor
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_t const reference to a valid Time object for SSP's averaging purposes ( start date time )
    * @param end_t const reference to a valid Time object for SSP's averaging purposes ( end date time )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param freq_step frequency step [Hz]
    **/
    WossResReader( const CoordZ& tx, const CoordZ& rx, const Time& start_t, const Time& end_t, 
                  double start_freq, double end_freq, double freq_step ) 
                  : Woss( tx, rx, start_t, end_t, start_freq, end_freq, freq_step ) { }

    
    virtual ~WossResReader() { clearResReaderMap(); }


    /**
    * Initializes current ResReader object
    * @param curr_frequency frequency in use [Hz]
    * @returns <i>true</i> if method succeeded, <i>false</i> otherwise
    **/
    virtual bool initResReader( double curr_frequency ) = 0;
    
    
    protected:
       
    
    /**
    * ResReader map
    **/ 
    ResReaderMap res_reader_map;
    

    /**
    * Clears res_reader_map
    **/ 
    void clearResReaderMap();

    
  };

  
}


#endif /* WOSS_PROGRAM_DEFINITIONS_H */


