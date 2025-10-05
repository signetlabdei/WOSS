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
 * @file   woss-manager.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossManager, woss::WossManagerResDb and woss::WossManagerResDbMT classes
 *
 * Provides the interface for woss::WossManager, woss::WossManagerResDb and woss::WossManagerResDbMT classes
 */


#ifndef WOSS_MANAGER_DEFINITIONS_H
#define WOSS_MANAGER_DEFINITIONS_H

#if defined (WOSS_MULTITHREAD)
#include <set>
#include <map>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#endif // defined (WOSS_MULTITHREAD)
#include <utility>
#include <vector>
#include <memory>
#include <definitions-handler.h>
#include "woss-creator.h"
#include <woss-db-manager.h>


namespace woss {

  /**
  * A pair of CoordZ (tx, rx)
  */
  using CoordZPair = std::pair< CoordZ, CoordZ >;

  /**
  * A vector of CoordZPair
  */
  using CoordZPairVect = std::vector< CoordZPair >;

  /**
  * A pair of frequency (start, end)
  */
  using SimFreq = std::pair< double, double >;
  
  /**
  * A vector of SimFreq
  */
  using SimFreqVector = std::vector< SimFreq >;

  /**
  * A vector of heap-created Pressure objects
  */
  using PressureVector = std::vector< std::unique_ptr<Pressure> >;

  /**
  * A vector of heap-created TimeArr objects
  */
  using TimeArrVector = std::vector< std::unique_ptr<TimeArr> >;

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

    virtual ~WossManager() = default;

    /**
    * Returns a shared_ptr to a valid and properly initialized woss::Woss object. 
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @returns std::shared_ptr to a valid Woss instance
    **/
    virtual std::shared_ptr<Woss> getActiveWoss( const CoordZ& tx, const CoordZ& rx, double start_frequency, double end_frequency );

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
    * Returns a valid unique_ptr<Pressure> for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns std::unique_ptr to a valid Pressure object
    **/
    virtual std::unique_ptr<Pressure> getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) = 0;

    /**
    * Returns a valid Pressure* for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns std::unique_ptr to a valid Pressure object
    **/
    virtual std::unique_ptr<Pressure> getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value = 0.0 );

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
    * @returns std::unique_ptr to a valid TimeArr object
    **/
    virtual std::unique_ptr<TimeArr> getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) = 0;

    /**
    * Returns a valid TimeArr* for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns std::unique_ptr to a valid TimeArr object
    **/
    virtual std::unique_ptr<TimeArr> getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value = 0.0 );

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
    WossManager& setWossCreator( const std::shared_ptr<WossCreator>& ptr ) { woss_creator = ptr; return *this; }

    void setTimeEvolutionActiveFlag( bool flag ) { is_time_evolution_active = flag; }

    void setDebugFlag( bool flag ) { debug = flag; }

    std::shared_ptr<WossCreator> getWossCreator() const { return woss_creator; }
    
    bool getTimeEvolutionActiveFlag() const { return (bool)is_time_evolution_active; }
    
    bool getDebugFlag() const { return (bool)debug; }

    protected:

    static const Time NO_EVOLUTION_TIME;

    /**
    * Const pointer to a WossCreator instance, for Woss creation purposes
    **/
    std::shared_ptr<WossCreator> woss_creator;

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
    * @returns std::shared_ptr to a valid Woss object
    **/
    virtual std::shared_ptr<Woss> getWoss( const CoordZ& tx, const CoordZ& rx, double start_frequency, double end_frequency ) = 0;

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

    virtual ~WossManagerResDb() override = default;

    /**
    * Returns a valid unique_ptr<Pressure> for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns std::unique_ptr to a valid Pressure object
    **/
    virtual std::unique_ptr<Pressure> getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) override;
    
    /**
    * Returns a valid TimeArr for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time& object
    * @returns std::unique_ptr to a valid TimeArr object
    **/
    virtual std::unique_ptr<TimeArr> getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) override;
    
    /**
    * Sets a std::shared_ptr to a WossDbManager instance, for db query purposes
    * @param ptr const pointer to a const WossDbManager instance
    * @return reference to <b>*this</b>
    **/
    WossManagerResDb& setWossDbManager( const std::shared_ptr<WossDbManager>& ptr ) { woss_db_manager = ptr; return *this; }

    protected:

    /**
    * Const pointer to a WossDbManager
    **/
    std::shared_ptr<WossDbManager> woss_db_manager;

    /**
    * Returns a TimeArr* from a WossResTimeArrDb for given parameters.
    * <b>User is responsible of pointer's ownership</b>
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param frequency frequency [Hz]
    * @param time_value const reference to a valid Time oject
    * @returns std::unique_ptr to a valid TimeArr object
    **/
    std::unique_ptr<TimeArr> dbGetTimeArr( const CoordZ& tx, const CoordZ& rx, double frequency, const Time& time_value ) const;

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
    * Returns a unique_ptr<Pressure> from a WossPressureDb for given parameters.
    * <b>User is responsible of pointer's ownership</b>
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param frequency frequency [Hz]
    * @param time_value const reference to a valid Time oject
    * @returns std::unique_ptr to a valid Pressure object
    **/
    std::unique_ptr<Pressure> dbGetPressure( const CoordZ& tx, const CoordZ& rx, double frequency, const Time& time_value ) const;

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
    if ( woss_db_manager ) 
      woss_db_manager->insertTimeArr( tx, rx, frequency, time_value, channel ) ;
  }

  inline std::unique_ptr<TimeArr> WossManagerResDb::dbGetTimeArr( const CoordZ& tx, const CoordZ& rx, double frequency, const Time& time_value ) const {
    if ( woss_db_manager ) 
      return( woss_db_manager->getTimeArr( tx, rx, frequency, time_value ) );
    return( SDefHandler::instance().createTimeArr( TimeArr::createNotValid() ) );
  }

  inline void WossManagerResDb::dbInsertPressure( const CoordZ& tx, const CoordZ& rx, double frequency, const Time& time_value, const Pressure& press ) const {
    if ( woss_db_manager ) 
      woss_db_manager->insertPressure( tx, rx, frequency, time_value, press );
  }

  inline std::unique_ptr<Pressure> WossManagerResDb::dbGetPressure( const CoordZ& tx, const CoordZ& rx, double frequency, const Time& time_value ) const {
    if ( woss_db_manager ) 
      return( woss_db_manager->getPressure( tx, rx, frequency, time_value ) );
    return(SDefHandler::instance().createPressure( Pressure::createNotValid()));
  }

#ifdef WOSS_MULTITHREAD
/**
  * Max number of active threads
  */
  static constexpr inline int MAX_TOTAL_THREADS = 32;

  /**
  * \brief Multi-threaded extension of WossManagerResDb
  *
  * WossManagerResDbMT is a multi-threaded extension of WossManagerResDb. It uses the std c++ multithread library</b>.
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
    
    virtual ~WossManagerResDbMT() override = default;

    /**
    * Returns a valid Pressure for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns std::unique_ptr to a valid Pressure object
    **/
    virtual std::unique_ptr<Pressure> getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) override;
    
    /**
    * Returns a valid Pressure for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value number of seconds after start time
    * @returns std::unique_ptr to a valid Pressure object
    **/
    virtual std::unique_ptr<Pressure> getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value ) override;


    /**
    * Returns a valid TimeArr for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns std::unique_ptr to a valid TimeArr object
    **/
    virtual std::unique_ptr<TimeArr> getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) override;

    /**
    * Returns a valid TimeArr for given parameters
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value number of seconds after start time
    * @returns std::unique_ptr to a valid TimeArr object
    **/
    virtual std::unique_ptr<TimeArr> getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value ) override;
    
    
    /**
    * Returns a valid vector of Pressure* for given parameters
    * @param coordinates const reference to a valid CoordZPairVect
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns valid PressureVector
    **/
    virtual PressureVector getWossPressure( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, const Time& time_value ) override;
        
    /**
    * Returns a valid vector of Pressure* for given parameters
    * @param coordinates const reference to a valid CoordZPairVect
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value number of seconds after start time
    * @returns valid PressureVector
    **/
    virtual PressureVector getWossPressure( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, double time_value ) override;
            
    /**
    * Returns a valid vector of TimeArr* for given parameters
    * @param coordinates const reference to a valid CoordZPairVect
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value const reference to a valid Time object
    * @returns valid TimeArrVector
    **/
    virtual TimeArrVector getWossTimeArr( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, const Time& time_value ) override;
 
    /**
    * Returns a valid vector of TimeArr* for given parameters
    * @param coordinates const reference to a valid CoordZPairVect
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param time_value number of seconds after start time
    * @returns valid TimeArrVector
    **/
    virtual TimeArrVector getWossTimeArr( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, double time_value ) override;
    
    /**
    * Sets the number of concurrent threads. If <i>number</i> < 0 multi-threading is disabled. 
    * If <i>number</i> = 0 the thread number is automatically handled.
    * @param number number of concurrent threads
    **/
    void setConcurrentThreads( unsigned int number ) { concurrent_threads = number; checkConcurrentThreads(); }

    /**
    * Gets the number of concurrent threads
    * @returns number of concurrent threads
    **/
    unsigned int getConcurrentThreads() const { return concurrent_threads; }

    /**
    * Sets the usage of woss::ThreadPool or std::async for multithread operation
    * 
    * @param flag true if woss::ThreadPool has to be used, false otherwise
    **/
    void setUseThreadPoolFlag (bool flag ) { use_thread_pool = flag; }

    /**
    * Gets the usage of woss::ThreadPool flag, which configures the multithread operation
    * 
    * @returns true if woss::ThreadPool is in use, false if std::async is in use.
    **/
    bool getUseThreadPoolFlag() { return use_thread_pool; }

    protected:

    /**
    * Structs that associates a std::mutex and a std::condition to an active woss::Woss object,
    * in order to wait for simulation completion.
    **/
    struct ThreadCondSignal {
      std::mutex mutex;
      std::condition_variable condition;
    };

    using ActiveWoss = std::map< std::shared_ptr<Woss>, std::unique_ptr<ThreadCondSignal> >;
    using AWIter = ActiveWoss::iterator;
    using AWRIter = ActiveWoss::reverse_iterator;
    using AWCIter = ActiveWoss::const_iterator;
    using AWCRIter = ActiveWoss::const_reverse_iterator;

    /**
    * Max number of created threads
    **/
    unsigned int max_thread_number;

    /**
    * Max number of concurrent threads
    **/
    unsigned int concurrent_threads;

    /**
    * Request mutex
    **/
    std::mutex request_mutex;

    /**
    * Set of current active Woss objects
    **/
    ActiveWoss active_woss;

    /**
    * Flag that configures multithread ops, either with woss::ThreadPool or with the usage of std::async 
    **/
    bool use_thread_pool;
    
    /**
    * Sets concurrent_threads valid range
    **/
    void checkConcurrentThreads();
  };

#endif // WOSS_MULTITHREAD

}

#endif /* WOSS_MANAGER_DEFINITIONS_H */ 
