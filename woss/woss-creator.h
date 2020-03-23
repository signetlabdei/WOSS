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
 * @file   woss-creator.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossCreator class
 *
 * Provides the interface for woss::WossCreator class
 */


#ifndef WOSS_CREATOR_DEFINITIONS_H
#define WOSS_CREATOR_DEFINITIONS_H


#define WOSS_CREATOR_MAX_FREQ_STEP (1.0e20)
#define WOSS_CREATOR_ALL_COORDZ CoordZ()


#include "woss.h"
#include "woss-creator-container.h"


namespace woss {
  
   
  
  class WossDbManager;
  class TransducerHandler;
  

  /**
  * \brief Abstract class that provides correctly initialized Woss objects
  *
  * WossCreator provides interface for creation and initializiation of Woss objects, relieving the user from this task
  */
  class WossCreator {


    protected:
      
             
    /**
    * SimTime WossCreatorContainer
    */
    typedef WossCreatorContainer< SimTime > CCSimTime;

    /**
    * double WossCreatorContainer
    */    
    typedef WossCreatorContainer< double > CCDouble;
 
    /**
    * int WossCreatorContainer
    */    
    typedef WossCreatorContainer< int > CCInt;
       
        
    public:


    /**
    * WossCreator default constructor
    */
    WossCreator();
    
    virtual ~WossCreator() { }


    /**
    * Returns a pointer to valid Woss for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @returns pointer to properly initialized Woss object
    **/
    virtual Woss* const createWoss( const CoordZ& tx, const CoordZ& rx, double start_freq, double end_freq ) const = 0;
   
    
    /**
    * Sets debug flag of every Woss object created
    * @param flag debug flag
    * @return reference to <b>*this</b>
    **/
    WossCreator& setWossDebug( bool flag ) { woss_debug = flag; return *this; }
    
    /**
    * Sets debug flag
    * @param flag debug flag
    * @return reference to <b>*this</b>
    **/
    WossCreator& setDebug( bool flag ) { debug = flag; updateDebugFlag(); return *this; }

    /**
    * Returns debug flag
    * @return <i>true</i> if using debug, <i>false</i> otherwise
    **/
    bool usingDebug() const { return debug; }

    /**
    * Returns Woss debug flag
    * @return <i>true</i> if Woss objects will be using debug, <i>false</i> otherwise
    **/
    bool usingWossDebug() const { return woss_debug; }


    /**
    * Sets the work pathname
    * @param path valid pathname
    * @return reference to <b>*this</b>
    **/
    WossCreator& setWrkDirPath( const ::std::string& path ) { work_dir_path = path; return *this; }
    
     /**
     * Sets clean work dir flag
     * @param flag debug flag
     * @return reference to <b>*this</b>
     **/
     WossCreator& setCleanWorkDir( bool flag ) { woss_clean_workdir = flag; return *this; }

    /**
    * Gets the work pathname
    * @returns valid pathname
    **/
    ::std::string getWrkDirPath() const { return work_dir_path; }
    
    
    /**
    * Sets the frequency step for given transmitter, receiver woss::CoordZ
    * @param f_step valid frequency step [Hz]. If step <= 0 start frequency is used and no steps are done
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    **/
    WossCreator& setFrequencyStep( double f_step, const CoordZ& tx, const CoordZ& rx ) { 
      if ( f_step <= 0.0 ) f_step = WOSS_CREATOR_MAX_FREQ_STEP; 
      ccfrequency_step.replace(f_step, tx, rx);   
      return *this; }
 
    /**
    * Sets the frequency step for given transmitter, receiver woss::Location
    * @param f_step valid frequency step [Hz]. If step <= 0 start frequency is used and no steps are done
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    **/
    WossCreator& setFrequencyStep( double f_step, Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      if ( f_step <= 0.0 ) f_step = WOSS_CREATOR_MAX_FREQ_STEP; 
      ccfrequency_step.replace(f_step, tx, rx);   
      return *this; }
        
    /**
    * Returns the frequency step for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return frequency step in Hz
    **/
    double getFrequencyStep( const CoordZ& tx, const CoordZ& rx ) const { 
      return ccfrequency_step.get(tx, rx); }

    /**
    * Returns the frequency step for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return frequency step in Hz
    **/
    double getFrequencyStep( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) const { 
      return ccfrequency_step.get(tx, rx); }    

    /**
    * Erases the frequency step for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    **/
    WossCreator& eraseFrequencyStep( const CoordZ& tx, const CoordZ& rx ) { 
      ccfrequency_step.erase(tx, rx); return *this; }

    /**
    * Erases the frequency step for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    **/
    WossCreator& eraseFrequencyStep( Location* const tx = CCDouble::ALL_LOCATIONS, Location* const rx = CCDouble::ALL_LOCATIONS ) { 
      ccfrequency_step.erase(tx, rx); return *this; }

    
    /**
    * Sets the time evolution threshold in seconds
    * @param value time evolution threshold in seconds
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    **/   
    WossCreator& setEvolutionTimeQuantum( double value, const CoordZ& tx, const CoordZ& rx ) { 
      ccevolution_time_quantum.replace( value, tx, rx ); return *this; } 
    
    /**
    * Sets time threshold for time evolution purposes
    * @param value time threshold in seconds
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    **/   
    WossCreator& setEvolutionTimeQuantum( double value, Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      ccevolution_time_quantum.replace( value, tx, rx ); return *this; } 
    
    /**
    * Returns the time evolution threshold
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return time evolution threshold in seconds
    **/ 
    double getEvolutionTimeQuantum( const CoordZ& tx, const CoordZ& rx ) const { 
      return ccevolution_time_quantum.get(tx, rx); } 
    
    /**
    * Returns the time evolution threshold
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return time evolution threshold in seconds
    **/   
    double getEvolutionTimeQuantum( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) const { 
      return ccevolution_time_quantum.get(tx, rx); } 
      
    /**
    * Erases the time evolution threshold
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    **/       
    WossCreator& eraseEvolutionTimeQuantum( const CoordZ& tx, const CoordZ& rx ) { 
      ccevolution_time_quantum.erase(tx, rx); return *this; } 
      
    /**
    * Erases the time evolution threshold
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    **/  
    WossCreator& eraseEvolutionTimeQuantum( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) { 
      ccevolution_time_quantum.erase(tx, rx); return *this; } 
             
    
    /**
    * Sets the total number of channel simulator's runs 
    * @param runs number of runs
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    **/   
    WossCreator& setTotalRuns( int runs, const CoordZ& tx, const CoordZ& rx ) {  
      cctotal_runs.replace( runs, tx, rx ); return *this; }
 
    /**
    * Sets the total number of channel simulator's runs 
    * @param runs number of runs
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    **/   
    WossCreator& setTotalRuns( int runs, Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) {  
      cctotal_runs.replace( runs, tx, rx ); return *this; }
      
    /**
    * Returns the total number of channel simulator's runs 
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return number of runs
    **/ 
    int getTotalRuns( const CoordZ& tx, const CoordZ& rx ) const { 
      return cctotal_runs.get(tx, rx); }
    
    /**
    * Returns the total number of channel simulator's runs 
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return number of runs
    **/     
    int getTotalRuns( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) const { 
      return cctotal_runs.get(tx, rx); }
      
    /**
    * Erases the total number of channel simulator's runs 
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    **/       
    WossCreator& eraseTotalRuns( const CoordZ& tx, const CoordZ& rx ) {  
      cctotal_runs.erase( tx, rx ); return *this; }
    
    /**
    * Erases the total number of channel simulator's runs 
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    **/ 
    WossCreator& eraseTotalRuns( Location* const tx = CCInt::ALL_LOCATIONS, Location* const rx = CCInt::ALL_LOCATIONS ) {  
      cctotal_runs.erase( tx, rx ); return *this; }
    

    /**
    * Sets the SimTime for given transmitter, receiver woss::CoordZ
    * @param simtime const reference to a valid SimTime
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    **/
    WossCreator& setSimTime( const SimTime& simtime, const CoordZ& tx, const CoordZ& rx ) { 
      ccsimtime_map.replace(simtime, tx, rx); return *this; }
      
    /**
    * Sets the SimTime for given transmitter, receiver woss::Location
    * @param simtime const reference to a valid SimTime
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    **/
    WossCreator& setSimTime( const SimTime& simtime, Location* const tx = CCSimTime::ALL_LOCATIONS, Location* const rx = CCSimTime::ALL_LOCATIONS ) { 
      ccsimtime_map.replace(simtime, tx, rx); return *this; }

    /**
    * Returns the SimTime for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return SimTime instance
    **/
    SimTime getSimTime( const CoordZ& tx, const CoordZ& rx ) const { 
      return ccsimtime_map.get(tx, rx); }
  
    /**
    * Returns the SimTime for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return SimTime instance
    **/
    SimTime getSimTime( Location* const tx = CCSimTime::ALL_LOCATIONS, Location* const rx = CCSimTime::ALL_LOCATIONS ) const { 
      return ccsimtime_map.get(tx, rx); }

    /**
    * Erases the SimTime for given transmitter, receiver woss::CoordZ
    * @param tx const reference to a valid woss::CoordZ instance
    * @param rx const reference to a valid woss::CoordZ instance 
    * @return reference to <b>*this</b>
    **/
    WossCreator& eraseSimTime( const CoordZ& tx, const CoordZ& rx ) { ccsimtime_map.erase(tx, rx); return *this; }
   
    /**
    * Erases the SimTime for given transmitter, receiver woss::Location
    * @param tx const pointer to a valid woss::Location instance
    * @param rx const pointer to a valid woss::Location instance 
    * @return reference to <b>*this</b>
    **/
    WossCreator& eraseSimTime( Location* const tx = CCSimTime::ALL_LOCATIONS, Location* const rx = CCSimTime::ALL_LOCATIONS ) { 
      ccsimtime_map.erase(tx, rx); return *this; }


    /**
    * Sets the WossDbManager pointer
    * @param path const pointer to a const WossDbManager object
    * @return reference to <b>*this</b>
    **/
    WossCreator& setWossDbManager( const WossDbManager* const ptr ) { woss_db_manager = ptr; return *this; }


    /**
    * Sets the WossDbManager pointer
    * @param path const pointer to a const WossDbManager object
    * @return reference to <b>*this</b>
    **/
    WossCreator& setTransducerHandler( const TransducerHandler* const ptr ) { transducer_handler = ptr; return *this; }
    
   
    const Woss& getWossNotValid() const;
    

    protected:
      
    
    static const Woss* woss_not_valid;
      
      
    const WossDbManager* woss_db_manager;
    
    
    const TransducerHandler* transducer_handler;
    
    
    /**
    * Directory path for temporary files (e.g. channel simulator files)
    **/
    ::std::string work_dir_path;
    
    
    /**
    * Time evolution threshold in seconds
    **/   
    CCDouble ccevolution_time_quantum;

    /**
    * Total number of channel simulator's runs 
    **/
    CCInt cctotal_runs;
    
    /**
    * Frequency step [Hz]
    **/
    CCDouble ccfrequency_step;
    
    
    /**
    * SimTime containter for user-given transmitter CoordZ
    **/
    CCSimTime ccsimtime_map;
    

    /**
    * Debug flag
    **/
    bool debug;

    /**
    * Debug flag for all Woss instances
    **/
    bool woss_debug;
    
    /**
    * Debug flag for all Woss instances
    **/    
    bool woss_clean_workdir;
    
    
    /**
    * Initializes given Woss object
    * @param woss_ptr const pointer to an unitialized Woss
    * @returns <i>true</i> if method succeeded, <i>false</i> otherwise
    **/
    virtual bool initializeWoss( Woss* const woss_ptr ) const = 0;
  
    virtual const Woss* createNotValidWoss() const = 0;
    
    /**
    * Propagates the debug flag
    **/
    virtual void updateDebugFlag();
    
  };


}


#endif /* WOSS_CREATOR_DEFINITIONS_H */ 

