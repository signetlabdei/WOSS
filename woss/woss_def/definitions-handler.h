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
 * @file   definitions-handler.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::DefHandler class
 *
 * Provides the interface for the woss::DefHandler class
 */


#ifndef WOSS_DEF_HANDLER_H
#define WOSS_DEF_HANDLER_H


#include <cassert>
#include <singleton-definitions.h>
#include <time-definitions.h>
#include <random-generator-definitions.h>


namespace woss {
  
    
  class WossDbCreator;
  class WossCreator;
  class WossDbManager;
  class WossManager;
  class SSP;
  class Sediment;
  class Pressure;
  class TimeArr;
  class Transducer;
  class Altimetry;
  

  /**
  * \brief Class for managing dynamic instantiation of foundation classes
  *
  * woss::DefHandler is a class that handle dynamic 
  * instances of woss::SSP, woss::Sediment, woss::TimeArr, woss::Pressure. A client should 
  * aks for a pointer to any of them, and then call the virtual <b>create</b> method for 
  * an instance of that particular class. In this way a class that inherits from any of those mentioned 
  * above, could be "plugged-in" at run time in any WOSS class without changing the code. It should be created
  * with singleton pattern for safety reasons (e.g. woss::Sigleton< woss::DefHandler > )
  */
  class DefHandler {

		
    public:
      
    
    /**
    * Default constructor
    */
    DefHandler();
    
    
    /**
    * Copy constructor
    */
    DefHandler( const DefHandler& copy ); 
    
    /**
    * Assignment operator 
    */
    DefHandler& operator=( const DefHandler& copy ); 
    
         
    /**
    * Destructor
    */
    virtual ~DefHandler();
    

    /**
    * Returns a random value from the connected woss::RandomGenerator object
    * @returns a random value
    */
    double getRand() const; 
    
    /**
     * Returns a random integer from the connected woss::RandomGenerator object
     * @returns a random integer value
     */
    int getRandInt() const; 
    

    /**
    * Returns the current simulation time reference from the connected woss::TimeReference object
    * @returns a simulation time reference in seconds
    */
    double getTimeReference() const; 


    void setPressure( Pressure* const ptr ) { pressure_creator = ptr; }
    
    void setTimeArr( TimeArr* const ptr ) { time_arr_creator = ptr; }
    
    void setSSP( SSP* const ptr ) { ssp_creator = ptr; }
    
    void setSediment( Sediment* const ptr ) { sediment_creator = ptr; }

    void setTimeReference( TimeReference* const ptr ) { time_reference = ptr; }

    void setRandGenerator( RandomGenerator* const ptr ) { rand_generator = ptr; }
       
    void setTransducer( Transducer* const ptr ) { transducer = ptr; }
    
    void setAltimetry( Altimetry* const ptr ) { altimetry_creator = ptr; }
    
    
    const Pressure* const getPressure() const { assert(pressure_creator); return pressure_creator; }
    
    const TimeArr* const getTimeArr() const { assert(time_arr_creator); return time_arr_creator; }
    
    const SSP* const getSSP() const { assert(ssp_creator); return ssp_creator; }
    
    const Sediment* const getSediment() const { assert(sediment_creator); return sediment_creator; }
                                    
    RandomGenerator* const getRandomGenerator() const { assert(rand_generator); return rand_generator; }
        
    const Transducer* const getTransducer() const { assert(transducer); return transducer; }
    
    const Altimetry* const getAltimetry() const { assert(altimetry_creator); return altimetry_creator; }
    
    
    void setDebug( bool flag ) { debug = flag; }
    
    bool getDebug() { return debug; }
    
    
    protected:
      
    
    /**
    * Debug flag
    */
    bool debug; 
        
    
    SSP* ssp_creator;
    
    Sediment* sediment_creator;
    
    Pressure* pressure_creator;
    
    TimeArr* time_arr_creator;
 
    TimeReference* time_reference;
    
    RandomGenerator* rand_generator;
        
    Transducer* transducer;
    
    Altimetry* altimetry_creator;
    
  };

  
  /**
  * \brief Singleton implementation of DefHandler class
  *
  * Singleton implementation of DefHandler class
  */
  typedef Singleton< DefHandler > SDefHandler;
  
  
  //inline functions
  
  inline double DefHandler::getRand() const { 
    if (rand_generator) return rand_generator->getRand(); 
    else {
      ::std::cerr << "DefHandler::getRand() ERROR, random generator wasn't set" << ::std::endl;
      exit(1);
    }
  }
  
  
  inline int DefHandler::getRandInt() const { 
    if (rand_generator) return rand_generator->getRandInt(); 
    else {
      ::std::cerr << "DefHandler::getRandInt() ERROR, random generator wasn't set" << ::std::endl;
      exit(1);
    }
  }  
  
  
  inline double DefHandler::getTimeReference() const { 
    if (time_reference) return time_reference->getTimeReference(); 
    else {
      ::std::cerr << "DefHandler::getTimeReference() ERROR, time reference wasn't set" << ::std::endl;
      exit(1);
    }
  }
  
  
}


#endif /* WOSS_DEF_HANDLER_H */ 
