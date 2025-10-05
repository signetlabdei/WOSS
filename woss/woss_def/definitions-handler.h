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
 * @file   definitions-handler.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::DefHandler class
 *
 * Provides the interface for the woss::DefHandler class
 */


#ifndef WOSS_DEF_HANDLER_H
#define WOSS_DEF_HANDLER_H


#include <iostream>
#include <cassert>
#include <cstdlib>
#include <memory>
#include "singleton-definitions.h"
#include "time-definitions.h"
#include "altimetry-definitions.h"
#include "random-generator-definitions.h"
#include "pressure-definitions.h"
#include "location-definitions.h"
#include "sediment-definitions.h"
#include "time-arrival-definitions.h"
#include "transducer-definitions.h"
#include "ssp-definitions.h"

namespace woss {
  
  /**
  * \brief Class for managing dynamic instantiation of foundation classes
  *
  * woss::DefHandler is a class that handle the creation of dynamic 
  * instances of woss::SSP, woss::Sediment, woss::TimeArr, woss::Pressure, woss::RandomGenerator, woss::TimeReference, 
  * woss::Transducer, woss::Location and woss::Altimetry. 
  * A custom derived definition class can be "plug in" at run time in the WOSS framework without changing the framework source code.
  * DefHandler has been be created with singleton pattern for safety reasons (e.g. woss::Sigleton< woss::DefHandler > )
  */
  class DefHandler {

    public:

    DefHandler() = default;

    DefHandler( const DefHandler& copy ) = default;

    DefHandler( DefHandler&& tmp ) = default;

    DefHandler& operator=( const DefHandler& copy ) = default;

    DefHandler& operator=( DefHandler&& tmp ) = default;

    ~DefHandler() = default;


    /**
    * Sets the wos::Pressure prototype, that will be used by the whole WOSS framework.
    * The ownership of the input smart pointer will be trasfered to the woss::DefHandler instance.
    * 
    * @param ptr a std::unique_ptr to a woss::Pressure object.
    */
    void setPressure( std::unique_ptr<Pressure> ptr ) { pressure_creator = std::move(ptr); }
    /**
    * Sets the wos::TimeArr prototype, that will be used by the whole WOSS framework.
    * The ownership of the input smart pointer will be trasfered to the woss::DefHandler instance.
    * 
    * @param ptr a std::unique_ptr to a woss::TimeArr object.
    */
    void setTimeArr( std::unique_ptr<TimeArr> ptr ) { time_arr_creator = std::move(ptr); }
    /**
    * Sets the wos::SSP prototype, that will be used by the whole WOSS framework.
    * The ownership of the input smart pointer will be trasfered to the woss::DefHandler instance.
    * 
    * @param ptr a std::unique_ptr to a woss::SSP object.
    */
    void setSSP( std::unique_ptr<SSP> ptr ) { ssp_creator = std::move(ptr); }
    /**
    * Sets the wos::Sediment prototype, that will be used by the whole WOSS framework.
    * The ownership of the input smart pointer will be trasfered to the woss::DefHandler instance.
    * 
    * @param ptr a std::unique_ptr to a woss::Sediment object.
    */
    void setSediment( std::unique_ptr<Sediment> ptr ) { sediment_creator = std::move(ptr); }
    /**
    * Sets the wos::TimeReference prototype, that will be used by the whole WOSS framework.
    * The ownership of the input smart pointer will be trasfered to the woss::DefHandler instance.
    * 
    * @param ptr a std::unique_ptr to a woss::TimeReference object.
    */
    void setTimeReference( std::unique_ptr<TimeReference> ptr ) { time_reference = std::move(ptr); }
    /**
    * Sets the wos::RandomGenerator prototype, that will be used by the whole WOSS framework.
    * The ownership of the input smart pointer will be trasfered to the woss::DefHandler instance.
    * 
    * @param ptr a std::unique_ptr to a woss::RandomGenerator object.
    */
    void setRandGenerator( std::unique_ptr<RandomGenerator> ptr ) { rand_generator = std::move(ptr); }
    /**
    * Sets the wos::Transducer prototype, that will be used by the whole WOSS framework.
    * The ownership of the input smart pointer will be trasfered to the woss::DefHandler instance.
    * 
    * @param ptr a std::unique_ptr to a woss::Transducer object.
    */
    void setTransducer( std::unique_ptr<Transducer> ptr ) { transducer_creator = std::move(ptr); }
    /**
    * Sets the wos::Altimetry prototype, that will be used by the whole WOSS framework.
    * The ownership of the input smart pointer will be trasfered to the woss::DefHandler instance.
    * 
    * @param ptr a std::unique_ptr to a woss::Altimetry object.
    */
    void setAltimetry( std::unique_ptr<Altimetry> ptr ) { altimetry_creator = std::move(ptr); }
    /**
    * Sets the wos::Location prototype, that will be used by the whole WOSS framework.
    * The ownership of the input smart pointer will be trasfered to the woss::DefHandler instance.
    * 
    * @param ptr a std::unique_ptr to a woss::Location object.
    */
    void setLocation( std::unique_ptr<Location> ptr) { location_creator = std::move(ptr); }
    /**
    * Sets the debug flag.
    * 
    * @param flag debug flag
    */
    void setDebug( bool flag ) { debug = flag; }

    /**
    * Returns a random value from the connected woss::RandomGenerator object
    * @returns a random value
    */
    double getRand() const { assert(rand_generator); return rand_generator->getRand(); }
    /**
     * Returns a random integer from the connected woss::RandomGenerator object
     * @returns a random integer value
     */
    int getRandInt() const { assert(rand_generator); return rand_generator->getRandInt(); }
    /**
    * Returns the current simulation time reference from the connected woss::TimeReference object
    * @returns a simulation time reference in seconds
    */
    double getTimeReference() const { assert(time_reference); return time_reference->getTimeReference(); }
    /**
    * Returns the current debug flag 
    * @returns boolean debug flag
    */
    bool getDebug() const { return debug; }

    /**
    * Returns a std::unique_ptr to heap allocated instance of the woss::Sediment class hierarchy. 
    * 
    * @param args variadic arguments that will be perfectly forwarded to the underlying woss::Sediment::create() function
    * @returns a std::unique_ptr to a woss::Sediment instance
    */
    template<class... Args>
    std::unique_ptr<Sediment> createSediment(Args&&... args) const { assert(sediment_creator); return sediment_creator->create(std::forward<Args>(args)...); }
    /**
    * Returns a std::unique_ptr to heap allocated instance of the woss::Transducer class hierarchy. 
    * 
    * @param args variadic arguments that will be perfectly forwarded to the underlying woss::Transducer::create() function
    * @returns a std::unique_ptr to a woss::Transducer instance
    */
    template<class... Args>
    std::unique_ptr<Transducer> createTransducer(Args&&... args) const { assert(transducer_creator); return transducer_creator->create(std::forward<Args>(args)...); }
    /**
    * Returns a std::unique_ptr to heap allocated instance of the woss::Altimetry class hierarchy. 
    * 
    * @param args variadic arguments that will be perfectly forwarded to the underlying woss::Altimetry::create() function
    * @returns a std::unique_ptr to a woss::Altimetry instance
    */
    template<class... Args>
    std::unique_ptr<Altimetry> createAltimetry(Args&&... args) const { assert(altimetry_creator); return altimetry_creator->create(std::forward<Args>(args)...); }
    /**
    * Returns a std::unique_ptr to heap allocated instance of the woss::Location class hierarchy. 
    * 
    * @param args variadic arguments that will be perfectly forwarded to the underlying woss::Location::create() function
    * @returns a std::unique_ptr to a woss::Location instance
    */
    template<class... Args>
    std::unique_ptr<Location> createLocation(Args&&... args) const { assert(location_creator); return location_creator->create(std::forward<Args>(args)...); }
    /**
    * Returns a std::unique_ptr to heap allocated instance of the woss::SSP class hierarchy. 
    * 
    * @param args variadic arguments that will be perfectly forwarded to the underlying woss::SSP::create() function
    * @returns a std::unique_ptr to a woss::SSP instance
    */
    template<class... Args>
    std::unique_ptr<SSP> createSSP(Args&&... args) const { assert(ssp_creator); return ssp_creator->create(std::forward<Args>(args)...); }
    /**
    * Returns a std::unique_ptr to heap allocated instance of the woss::Pressure class hierarchy. 
    * 
    * @param args variadic arguments that will be perfectly forwarded to the underlying woss::Pressure::create() function
    * @returns a std::unique_ptr to a woss::Pressure instance
    */
    template<class... Args>
    std::unique_ptr<Pressure> createPressure(Args&&... args) const { assert(pressure_creator); return pressure_creator->create(std::forward<Args>(args)...); }
    /**
    * Returns a std::unique_ptr to heap allocated instance of the woss::TimeArr class hierarchy. 
    * 
    * @param args variadic arguments that will be perfectly forwarded to the underlying woss::TimeArr::create() function
    * @returns a std::unique_ptr to a woss::TimeArr instance
    */
    template<class... Args>
    std::unique_ptr<TimeArr> createTimeArr(Args&&... args) const { assert(time_arr_creator); return time_arr_creator->create(std::forward<Args>(args)...); }
    /**
    * Returns a std::unique_ptr to heap allocated array of instances of the woss::Pressure class hierarchy. 
    * 
    * @param args variadic arguments that will be perfectly forwarded to the underlying woss::Pressure::createArray() function
    * @returns a std::unique_ptr to an array of woss::Pressure instances
    */
    template<class... Args>
    std::unique_ptr<Pressure[]> createPressureArray(Args&&... args) const { assert(pressure_creator); return pressure_creator->createArray(std::forward<Args>(args)...); }
    /**
    * Returns a std::unique_ptr to heap allocated array of instances of the woss::TimeArr class hierarchy. 
    * 
    * @param args variadic arguments that will be perfectly forwarded to the underlying woss::TimeArr::createArray() function
    * @returns a std::unique_ptr to an array of woss::TimeArr instances
    */
    template<class... Args>
    std::unique_ptr<TimeArr[]> createTimeArrArray(Args&&... args) const { assert(time_arr_creator); return time_arr_creator->createArray(std::forward<Args>(args)...); }

    private:

    /**
    * Debug flag
    */
    bool debug = false; 
    /**
    * std::unique_ptr of an instance of the woss::SSP class hierarchy
    */
    std::unique_ptr<SSP> ssp_creator = nullptr;
    /**
    * std::unique_ptr of an instance of the woss::Sediment class hierarchy
    */
    std::unique_ptr<Sediment> sediment_creator = nullptr;
    /**
    * std::unique_ptr of an instance of the woss::TimeReference class hierarchy
    */
    std::unique_ptr<TimeReference> time_reference = nullptr;
    /**
    * std::unique_ptr of an instance of the woss::RandomGenerator class hierarchy
    */
    std::unique_ptr<RandomGenerator> rand_generator = nullptr;
    /**
    * std::unique_ptr of an instance of the woss::Transducer class hierarchy
    */
    std::unique_ptr<Transducer> transducer_creator = nullptr;
    /**
    * std::unique_ptr of an instance of the woss::Altimetry class hierarchy
    */
    std::unique_ptr<Altimetry> altimetry_creator = nullptr;
    /**
    * std::unique_ptr of an instance of the woss::Location class hierarchy
    */
    std::unique_ptr<Location> location_creator = nullptr;
    /**
    * std::unique_ptr of an instance of the woss::Pressure class hierarchy
    */
    std::unique_ptr<Pressure> pressure_creator = nullptr;
    /**
    * std::unique_ptr of an instance of the woss::TimeArr class hierarchy
    */
    std::unique_ptr<TimeArr> time_arr_creator = nullptr;
  };

  /**
  * \brief Singleton implementation of DefHandler class
  *
  * Singleton implementation of DefHandler class
  */
  typedef Singleton< DefHandler > SDefHandler;

}

#endif /* WOSS_DEF_HANDLER_H */ 
