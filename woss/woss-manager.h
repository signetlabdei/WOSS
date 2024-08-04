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
 * @file   woss-manager.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossManager, woss::WossManagerResDb and woss::WossManagerResDbMT classes
 *
 * Provides the interface for woss::WossManager, woss::WossManagerResDb and woss::WossManagerResDbMT classes
 */


#ifndef WOSS_MANAGER_DEFINITIONS_H
#define WOSS_MANAGER_DEFINITIONS_H


#include <definitions-handler.h>
#include <time-arrival-definitions.h>
#include "woss-creator.h"
#include <woss-db-manager.h>


namespace woss {

  
  /**
  * A pair of CoordZ (tx, rx)
  */
  typedef ::std::pair< CoordZ, CoordZ > CoordZPair;
  
  /**
  * A vector of CoordZPair
  */
  typedef ::std::vector< CoordZPair > CoordZPairVect;
  
  
  /**
  * A pair of frequency (start, end)
  */
  typedef ::std::pair< double, double > SimFreq;
  
  /**
  * A vector of SimFreq
  */
  typedef ::std::vector< SimFreq > SimFreqVector;
 
  
  /**
  * A vector of heap-created Pressure objects
  */
  typedef ::std::vector< Pressure* > PressureVector;

  /**
  * A vector of heap-created TimeArr objects
  */
  typedef ::std::vector< TimeArr* > TimeArrVector;
  
    
  /**
  * \brief Abstract class that interfaces Pressure or TimeArr requests from user layer
  *
  * WossManager interfaces the user with the whole library. Every request for Pressure or TimeArr
  * should be done to this class. Logical inteligence for planning CPU load,  multi-frequency and multi-run channel simulations 
  * should be placed in this inheritance chain
  */
  class WossManager {

		
    public:
      
    
    /**
    * WossManager default constructor
    */
    WossManager();   
    
    virtual ~WossManager();
    
        
    /**
    * Returns a const reference to a valid and properly initialized woss::Woss object. 
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @returns const reference to a valid woss::Woss object
    **/
    virtual const Woss& getActiveWoss( const CoordZ& tx, const CoordZ& rx, double start_frequency, double end_frequency ) const;        
    
    /**
    * Deletes a woss::Woss object for given params
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @returns reference to *this
    **/
    virtual WossManager& eraseActiveWoss( const CoordZ& tx, const CoordZ& rx, double start_frequency, double end_frequency ) = 0;
    
    /**
    * Deletes all woss::Woss object
    * @returns reference to *this
    **/
    virtual WossManager& eraseAllWoss() = 0;
        
    /**
    * Returns a valid Pressure* for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns valid Pressure* pointer
    **/
    virtual Pressure* getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) = 0;
    
    /**
    * Returns a valid Pressure* for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns valid Pressure* pointer
    **/
    virtual Pressure* getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value = 0.0 );
    
    /**
    * Returns a valid vector of Pressure* for given parameters
    * @param coordinates const reference to a valid CoordZPairVect
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns valid PressureVector
    **/
    virtual PressureVector getWossPressure( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, const Time& time_value );
    
    /**
    * Returns a valid vector of Pressure* for given parameters
    * @param coordinates const reference to a valid CoordZPairVect
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value seconds after start time
    * @returns valid PressureVector
    **/
    virtual PressureVector getWossPressure( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, double time_value = 0.0 );
        
    /**
    * Returns a valid TimeArr* for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns valid TimeArr* pointer
    **/
    virtual TimeArr* getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) = 0;
    
    /**
    * Returns a valid TimeArr* for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns valid TimeArr* pointer
    **/
    virtual TimeArr* getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value = 0.0 );
        
    /**
    * Returns a valid vector of TimeArr* for given parameters
    * @param coordinates const reference to a valid CoordZPairVect
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a Time object
    * @returns valid TimeArrVector
    **/
    virtual TimeArrVector getWossTimeArr( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, const Time& time_value );
   
    /**
    * Returns a valid vector of TimeArr* for given parameters
    * @param coordinates const reference to a valid CoordZPairVect
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value number of seconds after start time
    * @returns valid TimeArrVector
    **/
    virtual TimeArrVector getWossTimeArr( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, double time_value = 0.0 );
   
    
    /**
    * Deletes all created Woss instances
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool reset() = 0;


    /**
    * Performs a time evoulion of all time-dependant parameters of all created Woss instances
    * @param time_value const reference to a valid Time object
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool timeEvolve( const Time& time_value ) = 0;


    /**
    * Sets a pointer to a WossCreator instance, for Woss creation purposes
    * @param ptr const pointer to a const WossCreator instance
    * @return reference to <b>*this</b>
    **/
    WossManager& setWossCreator( const WossCreator* const ptr ) { woss_creator = ptr; return *this; }
        
    void setTimeEvolutionActiveFlag( bool flag ) { is_time_evolution_active = flag; }
  
    void setDebugFlag( bool flag ) { debug = flag; }
    
    
    const WossCreator* const getWossCreator() { return woss_creator; }
    
    bool getTimeEvolutionActiveFlag() { return (bool)is_time_evolution_active; }
    
    bool getDebugFlag() { return (bool)debug; }
    
    
    protected:
      
    
    static const Time NO_EVOLUTION_TIME;
    
    
    /**
    * Const pointer to a WossCreator instance, for Woss creation purposes
    **/
    const WossCreator* woss_creator;
    
  
    /**
    * Debug flag
    **/
    bool debug;
  
		
    bool is_time_evolution_active;  
    
		
    /**
    * Returns a pointer to a properly initialized Woss, for storage purposes. 
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @returns pointer to a valid Woss object
    **/
    virtual Woss* const getWoss( const CoordZ& tx, const CoordZ& rx, double start_frequency, double end_frequency ) = 0;
    
    
  };


  /**
  * \brief Abstract class that implements WossManager. It adds computed results dbs control
  *
  * WossManagerResDb adds control over optional computed dbs control. If dbs are present and valid requested TimeArr
  * or Pressure is returned, no channel simulator is run
  */
  class WossManagerResDb : public WossManager {

    
    public:
      
      
    WossManagerResDb();
    
    virtual ~WossManagerResDb() {}
    

    /**
    * Returns a valid Pressure for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns valid Pressure value
    **/
    virtual Pressure* getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value );
    
    /**
    * Returns a valid Pressure for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value number of seconds after start time
    * @returns valid Pressure value
    **/
//     virtual Pressure* getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value );
        
    /**
    * Returns a valid TimeArr for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time& object
    * @returns valid TimeArr value
    **/
    virtual TimeArr* getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value );
    
    /**
    * Returns a valid TimeArr for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value number of seconds after start time
    * @returns valid TimeArr value
    **/
//     virtual TimeArr* getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value );
    
    
    /**
    * Sets a pointer to a WossDbManager instance, for db query purposes
    * @param ptr const pointer to a const WossDbManager instance
    * @return reference to <b>*this</b>
    **/
    WossManagerResDb& setWossDbManager( const WossDbManager* const ptr ) { woss_db_manager = ptr; return *this; }
    
    
    protected:
      
      
    /**
    * Const pointer to a WossDbManager
    **/
    const WossDbManager* woss_db_manager;
    
    
    /**
    * Returns a TimeArr* from a WossResTimeArrDb for given parameters.
    * <b>User is responsible of pointer's ownership</b>
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param frequency frequency [Hz]
    * @param time_value const reference to a valid Time oject
    * @returns TimeArr value
    **/
    TimeArr* dbGetTimeArr( const CoordZ& tx, const CoordZ& rx, double frequency, const Time& time_value ) const;
    
    /**
    * Inserts a TimeArr in a WossResTimeArrDb
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param frequency frequency [Hz]
    * @param time_value const reference to a valid Time oject
    * @param channel const reference to a valid TimeArr to be inserted
    **/
    void dbInsertTimeArr( const CoordZ& tx, const CoordZ& rx, double frequency, const Time& time_value, const TimeArr& channel ) const;
    
    
    /**
    * Returns a Pressure* from a WossPressureDb for given parameters.
    * <b>User is responsible of pointer's ownership</b>
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param frequency frequency [Hz]
    * @param time_value const reference to a valid Time oject
    * @returns Pressure value
    **/
    Pressure* dbGetPressure( const CoordZ& tx, const CoordZ& rx, double frequency, const Time& time_value ) const;

    /**
    * Inserts a Pressure in a WossResPressureDb
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param frequency frequency [Hz]
    * @param time_value const reference to a valid Time oject
    * @param press const reference to a valid Pressure to be inserted
    **/
    void dbInsertPressure( const CoordZ& tx, const CoordZ& rx, double frequency, const Time& time_value, const Pressure& press ) const;

    
  };


  //inline functions
  //////////
  inline void WossManagerResDb::dbInsertTimeArr( const CoordZ& tx, const CoordZ& rx, double frequency, const Time& time_value, const TimeArr& channel ) const {
    if ( woss_db_manager ) woss_db_manager->insertTimeArr( tx, rx, frequency, time_value, channel ) ;
  }


  inline TimeArr* WossManagerResDb::dbGetTimeArr( const CoordZ& tx, const CoordZ& rx, double frequency, const Time& time_value ) const {
    if ( woss_db_manager ) return( woss_db_manager->getTimeArr( tx, rx, frequency, time_value ) );
    return( SDefHandler::instance()->getTimeArr()->create( TimeArr::createNotValid() ) );
  }


  inline void WossManagerResDb::dbInsertPressure( const CoordZ& tx, const CoordZ& rx, double frequency, const Time& time_value, const Pressure& press ) const {
    if ( woss_db_manager ) woss_db_manager->insertPressure( tx, rx, frequency, time_value, press );
  }


  inline Pressure* WossManagerResDb::dbGetPressure( const CoordZ& tx, const CoordZ& rx, double frequency, const Time& time_value ) const {
    if ( woss_db_manager ) return( woss_db_manager->getPressure( tx, rx, frequency, time_value ) );
    return( SDefHandler::instance()->getPressure()->create( Pressure::createNotValid() ) );
  }


  #ifdef WOSS_MULTITHREAD
  
  
  #include <set>
  
  
  /**
  * Max number of active threads
  */
  #define MAX_TOTAL_PTHREAD 32 
  
  
  /**
  * \brief Multi-threaded extension of WossManagerResDb
  *
  * WossManagerResDbMT is a multi-threaded extension of WossManagerResDb. It uses the pthread library</b>.
  * This class is optimized for multi-processor cpu.<b>Don't use it if a multi-processor cpu is not installed</b>.
  * <i>Please notice that simulation will suffer an heavy time penalty if a result db is used and Woss objects 
  * are not run</i>. This is due to the thread creation and synchronization overhead. Therefore no multi-thread should
  * be used when reading already computed channel simulator data.
  */
  class WossManagerResDbMT : public WossManagerResDb {

 
    public:


    /**
    * WossManagerResDbMT default constructor
    */
    WossManagerResDbMT();
    
    virtual ~WossManagerResDbMT();
    
    
    /**
    * Returns a valid Pressure for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns valid Pressure value
    **/
    virtual Pressure* getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value );
    
    /**
    * Returns a valid Pressure for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value number of seconds after start time
    * @returns valid Pressure value
    **/
    virtual Pressure* getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value );
        

    /**
    * Returns a valid TimeArr for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns valid TimeArr value
    **/
    virtual TimeArr* getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value );

        /**
    * Returns a valid TimeArr for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value number of seconds after start time
    * @returns valid TimeArr value
    **/
    virtual TimeArr* getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value );
    
    
    /**
    * Returns a valid vector of Pressure* for given parameters
    * @param coordinates const reference to a valid CoordZPairVect
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns valid PressureVector
    **/
    virtual PressureVector getWossPressure( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, const Time& time_value );
        
    /**
    * Returns a valid vector of Pressure* for given parameters
    * @param coordinates const reference to a valid CoordZPairVect
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value number of seconds after start time
    * @returns valid PressureVector
    **/
    virtual PressureVector getWossPressure( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, double time_value );
            
    /**
    * Returns a valid vector of TimeArr* for given parameters
    * @param coordinates const reference to a valid CoordZPairVect
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns valid TimeArrVector
    **/
    virtual TimeArrVector getWossTimeArr( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, const Time& time_value );
 
    /**
    * Returns a valid vector of TimeArr* for given parameters
    * @param coordinates const reference to a valid CoordZPairVect
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value number of seconds after start time
    * @returns valid TimeArrVector
    **/
    virtual TimeArrVector getWossTimeArr( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, double time_value );
    
    /**
    * Sets the number of concurrent threads. If <i>number</i> < 0 multi-threading is disabled. 
    * If <i>number</i> = 0 the thread number is automatically handled.
    * @param number number of concurrent threads
    **/
    void setConcurrentThreads( int number ) { concurrent_threads = number; checkConcurrentThreads(); }

    /**
    * Gets the number of concurrent threads
    * @returns number of concurrent threads
    **/
    int getConcurrentThreads() { return concurrent_threads; }
    
    
    friend void* WMSMTcreateThreadTimeArr( void* ptr );
      
    friend void* WMSMTcreateThreadPressure( void* ptr );       

    
    protected:   
    
    
    /**
    * Type for storing current queries
    */
    //typedef ::std::pair< CoordZPairVect, SimFreq > ThreadQuery;
    struct ThreadQuery {
      
      
      CoordZPairVect coordz_pair_vect;
      
      SimFreq sim_freq;

      int curr_index;
      
      struct {
          bool is_time_object;
          
          Time time_cal;
          
          double time_double;
          
      } time_data;
      
    };
    
    
    /**
    * Type used by an active query thread
    */
//     typedef ::std::pair< CoordZPair, SimFreq > ThreadParam;
    struct ThreadParam {
      
      
      CoordZPair coordz_pair;
      
      SimFreq sim_freq;
      
      struct {
          bool is_time_object;
          
          Time time_cal;
          
          double time_double;
          
      } time_data;
      
      
    };
    
    
    /**
    * Type used by an active query thread
    */ 
    typedef ::std::pair< int, ThreadParam > ThreadParamIndex;
    
    
    struct ThreadCondSignal {
    
      
      ThreadCondSignal() { pthread_mutex_init( &mutex, NULL ); pthread_cond_init( &condition, NULL );  }
      
      ~ThreadCondSignal() { pthread_mutex_destroy( &mutex ); pthread_cond_destroy( &condition ); }
      
      
      pthread_mutex_t mutex;
      
      pthread_cond_t condition;
      
      
    };
    
    
    typedef ::std::map< Woss*, ThreadCondSignal* > ActiveWoss;
    typedef ActiveWoss::iterator AWIter;
    typedef ActiveWoss::reverse_iterator AWRIter;
    typedef ActiveWoss::const_iterator AWCIter;
    typedef ActiveWoss::const_reverse_iterator AWCRIter;
    
    
    /**
    * Max number of created threads
    **/
    int max_thread_number;    
    
    /**
    * Number of queries
    **/
    int total_queries;

    /**
    * Max number of concurrent threads
    **/
    int concurrent_threads;
    
       
    /**
    * Number of created threads
    **/
    volatile int total_thread_created;
       
    /**
    * Total number of completed threads
    **/
    volatile int total_thread_ended;
        

    /**
    * pthread id associated to the creation croller thread
    **/
    pthread_t thread_controller;
    
    /**
    * Array of pthread ids associated to the created query threads
    **/
    pthread_t thread_arr[MAX_TOTAL_PTHREAD];
    

    /**
    * Master spinlock
    **/
    pthread_spinlock_t mutex;
    
    /**
    * Secondary spinlock
    **/
    pthread_spinlock_t request_mutex;
   
    
    /**
    * Storage for current queries
    **/
    ThreadQuery thread_query;
      
    
    /**
    * The computation of current queries
    **/
    TimeArrVector thread_time_arr_reply;
    
    /**
    * The computation of current queries
    **/
    PressureVector thread_pressure_reply;
      
    /**
    * Set of current active Woss objects
    **/   
    ActiveWoss active_woss;
    
    /**
    * Sets concurrent_threads valid range
    **/    
    void checkConcurrentThreads();
    
    /**
    * Initializes variable for current query round
    **/
    void initThreadVars();


    /**
    * Returns a valid ThreadParamIndex for a requesting thread
    * @returns valid ThreadParamIndex
    **/
    ThreadParamIndex popThreadParamIndex();

    
    /**
    * Insert the given TimeArr pointer in the TimeArrVector reply at given index
    * @param index vector index
    * @param time_arr pointer to a heap-created TimeArr
    **/
    void insertThreadReplyTimeArr( int index, woss::TimeArr* time_arr );
    
    /**
    * Insert the given Pressure pointer in the PressureVector reply at given index
    * @param index vector index
    * @param pressure pointer to a heap-created Pressure
    **/
    void insertThreadReplyPressure( int index, woss::Pressure* pressure );
         
   
  };
  
  
  /**
  * Function used for TimeArr thread creation
  * @param ptr void pointer
  * @returns void pointer
  **/
  void* WMSMTcreateThreadTimeArr( void* ptr );
  
  /**
  * Function used for Pressure thread creation
  * @param ptr void pointer
  * @returns void pointer
  **/
  void* WMSMTcreateThreadPressure( void* ptr );
  
  
#endif // WOSS_MULTITHREAD
  
  
}


#endif /* WOSS_MANAGER_DEFINITIONS_H */ 

