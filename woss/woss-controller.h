/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2009 Federico Guerra
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
 * @file   woss-controller.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossController class
 *
 * Provides the interface for the woss::WossController class
 */


#ifndef WOSS_CONTROLLER_H
#define WOSS_CONTROLLER_H

#include <memory>
#include <cassert>
#include <singleton-definitions.h>
#include <transducer-handler.h>
#include <woss-db-creator.h>
#include <woss-db-manager.h>
#include "woss-creator.h"
#include "woss-manager.h"


namespace woss {

  /**
  * \brief Class for managing all WOSS classes involved
  *
  * woss::WossController is a class that sets all needed connections between primary WOSS classes.
  * It should be used with woss::Singleton for safety reasons. ( e.g. woss::Singleton\<woss::WossController\> )
  */
  class WossController {

    public:

    /**
    * Default constructor
    */
    WossController() = default;

    WossController( WossController& copy ) = default;

    WossController& operator=( WossController& copy ) = default;

    virtual ~WossController() = default;

    /**
    * Initializes all connections
    */
    bool initialize();

    /**
    * Configures the BathymetryDbCreator object
    * 
    * @param ptr a const reference to a std::shared_ptr to a BathymetryDbCreator instance
    * @returns a reference to <i>*this</i>
    */
    WossController& setBathymetryDbCreator( const std::shared_ptr<WossDbCreator>& ptr ) { bathymetry_db_creator = ptr; return *this; }

    /**
    * Configures the SedimentDbCreator object
    * 
    * @param ptr a const reference to a std::shared_ptr to a SedimentDbCreator instance
    * @returns a reference to <i>*this</i>
    */
    WossController& setSedimentDbCreator( const std::shared_ptr<WossDbCreator>& ptr ) { sediment_db_creator = ptr; return *this; }

    /**
    * Configures the SSPDbCreator object
    * 
    * @param ptr a const reference to a std::shared_ptr to a SSPDbCreator instance
    * @returns a reference to <i>*this</i>
    */
    WossController& setSSPDbCreator( const std::shared_ptr<WossDbCreator>& ptr ) { ssp_db_creator = ptr; return *this; }

    /**
    * Configures the PressureDbCreator object
    * 
    * @param ptr a const reference to a std::shared_ptr to a PressureDbCreator instance
    * @returns a reference to <i>*this</i>
    */
    WossController& setPressureDbCreator( const std::shared_ptr<WossDbCreator>& ptr ) { pressure_result_db_creator = ptr; return *this; }

    /**
    * Configures the TimeArrDbCreator object
    * 
    * @param ptr a const reference to a std::shared_ptr to a BathymetryDbCreator instance
    * @returns a reference to <i>*this</i>
    */
    WossController& setTimeArrDbCreator( const std::shared_ptr<WossDbCreator>& ptr ) { timearr_result_db_creator = ptr; return *this; }

    /**
    * Configures the WossCreator object
    * 
    * @param ptr a const reference to a std::shared_ptr to a WossCreator instance
    * @returns a reference to <i>*this</i>
    */
    WossController& setWossCreator( const std::shared_ptr<WossCreator>& ptr ) { woss_creator = ptr; return *this; }
 
    /**
    * Configures the WossDbManager object
    * 
    * @param ptr a const reference to a std::shared_ptr to a WossDbManager instance
    * @returns a reference to <i>*this</i>
    */
    WossController& setWossDbManager( const std::shared_ptr<WossDbManager>& ptr ) { woss_db_manager = ptr; return *this; }

    /**
    * Configures the WossManager object
    * 
    * @param ptr a const reference to a std::shared_ptr to a WossManager instance
    * @returns a reference to <i>*this</i>
    */
    WossController& setWossManager( const std::shared_ptr<WossManager>& ptr ) { woss_manager = ptr; return *this; }
 
    /**
    * Configures the TransducerHandler object
    * 
    * @param ptr a const reference to a std::shared_ptr to a TransducerHandler instance
    * @returns a reference to <i>*this</i>
    */
    WossController& setTransducerHandler( const std::shared_ptr<TransducerHandler>& ptr ) { transducer_handler = ptr; return *this; }

    /**
    * Configures the debug flag
    * 
    * @param ptr a const reference to a std::shared_ptr to a WossManager instance
    * @returns a reference to <i>*this</i>
    */
    WossController& setDebug( bool flag) { debug = flag; return *this; }

    /**
    * Returns the current BathymetryDbCreator object
    * 
    * @returns a std::shared_ptr to a BathymetryDbCreator instance
    */
    std::shared_ptr<WossDbCreator> getBathymetryDbCreator() const { assert(initialized); return bathymetry_db_creator; }

    /**
    * Returns the current SedimentDbCreator object
    * 
    * @returns a std::shared_ptr to a SedimentDbCreator instance
    */
    std::shared_ptr<WossDbCreator> getSedimentDbCreator() const { assert(initialized); return sediment_db_creator; }

    /**
    * Returns the current SSPDbCreator object
    * 
    * @returns a std::shared_ptr to a SSPDbCreator instance
    */
    std::shared_ptr<WossDbCreator> getSSPDbCreator() const { assert(initialized); return ssp_db_creator; }

    /**
    * Returns the current PressureDbCreator object
    * 
    * @returns a std::shared_ptr to a PressureDbCreator instance
    */
    std::shared_ptr<WossDbCreator> getPressureDbCreator() const { assert(initialized); return pressure_result_db_creator; }

    /**
    * Returns the current TimeArrDbCreator object
    * 
    * @returns a std::shared_ptr to a TimeArrDbCreator instance
    */
    std::shared_ptr<WossDbCreator> getTimeArrDbCreator() const { assert(initialized); return timearr_result_db_creator; }

    /**
    * Returns the current WossCreator object
    * 
    * @returns a std::shared_ptr to a WossCreator instance
    */
    std::shared_ptr<WossCreator> getWossCreator() const { assert(initialized); return woss_creator; }

    /**
    * Returns the current WossDbManager object
    * 
    * @returns a std::shared_ptr to a WossDbManager instance
    */
    std::shared_ptr<WossDbManager> getWossDbManager() const { assert(initialized); return woss_db_manager; }

    /**
    * Returns the current WossManager object
    * 
    * @returns a std::shared_ptr to a WossManager instance
    */
    std::shared_ptr<WossManager> getWossManager() const { assert(initialized); return woss_manager; }

    /**
    * Returns the current TransducerHandler object
    * 
    * @returns a std::shared_ptr to a TransducerHandler instance
    */
    std::shared_ptr<TransducerHandler> getTransducerHandler() const { assert(initialized); return transducer_handler; }

    /**
    * Returns the current debug flag 
    * 
    * @returns debug flag
    */
    bool getDebug() const { return debug; }

    protected:

    /**
    * Debug flag
    */
    double debug = false; 

    /**
    * Initialized flag
    */
    bool initialized = false;

    /**
    * std::shared_ptr of the current BathymetryDbCreator
    */
    std::shared_ptr<WossDbCreator> bathymetry_db_creator = nullptr;

    /**
    * std::shared_ptr of the current SedimentDbCreator
    */
    std::shared_ptr<WossDbCreator> sediment_db_creator = nullptr;

    /**
    * std::shared_ptr of the current SSPDbCreator
    */
    std::shared_ptr<WossDbCreator> ssp_db_creator = nullptr;

    /**
    * std::shared_ptr of the current PressureDbCreator
    */
    std::shared_ptr<WossDbCreator> pressure_result_db_creator = nullptr;

    /**
    * std::shared_ptr of the current TimeArrDbCreator
    */
    std::shared_ptr<WossDbCreator> timearr_result_db_creator = nullptr;

    /**
    * std::shared_ptr of the current WossCreator
    */
    std::shared_ptr<WossCreator> woss_creator = nullptr;

    /**
    * std::shared_ptr of the current WossDbManager
    */
    std::shared_ptr<WossDbManager> woss_db_manager = nullptr;

    /**
    * std::shared_ptr of the current WossManager
    */
    std::shared_ptr<WossManager> woss_manager = nullptr;

    /**
    * std::shared_ptr of the current TransducerHandler
    */
    std::shared_ptr<TransducerHandler> transducer_handler = nullptr;

  };

  /**
  * \brief Singleton implementation of WossController class
  *
  * Singleton implementation of WossController class
  */
  typedef Singleton< WossController > SWossController;

}

#endif /* WOSS_CONTROLLER_H */
