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
 * @file   woss-controller.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossController class
 *
 * Provides the interface for the woss::WossController class
 */


#ifndef WOSS_CONTROLLER_H
#define WOSS_CONTROLLER_H


#include <cassert>
#include <singleton-definitions.h> 


namespace woss {
  
    
  class WossDbCreator;
  class WossCreator;
  class WossDbManager;
  class WossManager;
  class TransducerHandler;
  

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
    WossController();
    
    /**
    * Copy constructor ( no const here, we have to modify the copy )
    */
    WossController( WossController& copy ); 
    
    /**
    * Assignment operator  ( no const here, we have to modify the copy )
    */
    WossController& operator=( WossController& copy );
    
    
    /**
    * Destructor
    */
    virtual ~WossController();
      
    
    /**
    * Initializes all connections
    */
    bool initialize();
    
    
    WossController& setBathymetryDbCreator( WossDbCreator* const ptr ) { bathymetry_db_creator = ptr; return *this; }
    
    WossController& setSedimentDbCreator( WossDbCreator* const ptr ) { sediment_db_creator = ptr; return *this; }
    
    WossController& setSSPDbCreator( WossDbCreator* const ptr ) { ssp_db_creator = ptr; return *this; }
    
    WossController& setPressureDbCreator( WossDbCreator* const ptr ) { pressure_result_db_creator = ptr; return *this; }

    WossController& setTimeArrDbCreator( WossDbCreator* const ptr ) { timearr_result_db_creator = ptr; return *this; }
    
    WossController& setWossCreator( WossCreator* const ptr ) { woss_creator = ptr; return *this; }
    
    WossController& setWossDbManager( WossDbManager* const ptr ) { woss_db_manager = ptr; return *this; }
    
    WossController& setWossManager( WossManager* const ptr ) { woss_manager = ptr; return *this; }
    
    WossController& setTransducerHandler( TransducerHandler* const ptr ) { transducer_handler = ptr; return *this; }
    
    void setDebug( bool flag) { debug = flag;}
    
    const WossDbCreator* const getBathymetryDbCreator() const { assert(initialized); return bathymetry_db_creator; }
    
    const WossDbCreator* const getSedimentDbCreator() const { assert(initialized); return sediment_db_creator; }
    
    const WossDbCreator* const getSSPDbCreator() const { assert(initialized); return ssp_db_creator; }

    const WossDbCreator* const getPressureDbCreator() const { assert(initialized); return pressure_result_db_creator; }

    const WossDbCreator* const getTimeArrDbCreator() const { assert(initialized); return timearr_result_db_creator; }
    
    const WossCreator* const getWossCreator() const { assert(initialized); return woss_creator; }
    
    const WossDbManager* const getWossDbManager() const { assert(initialized); return woss_db_manager; }
    
    WossManager* const getWossManager() const { assert(initialized); return woss_manager; }
  
    TransducerHandler* const getTransducerHandler() const { assert(initialized); return transducer_handler; }
    
    bool getDebug() const { return debug; }
    
    protected:
    
   
    /**
    * Debug flag
    */
    double debug; 
    
    /**
    * Initialized flag
    */
    bool initialized;
  
    
    WossDbCreator* bathymetry_db_creator;
    
    WossDbCreator* sediment_db_creator;
    
    WossDbCreator* ssp_db_creator;
    
    WossDbCreator* pressure_result_db_creator;
    
    WossDbCreator* timearr_result_db_creator;
    
    WossCreator* woss_creator;
    
    WossDbManager* woss_db_manager;
    
    WossManager* woss_manager;
    
    TransducerHandler* transducer_handler;
     
       
  };

  
  /**
  * \brief Singleton implementation of WossController class
  *
  * Singleton implementation of WossController class
  */
  typedef Singleton< WossController > SWossController;


}


#endif /* WOSS_CONTROLLER_H */

