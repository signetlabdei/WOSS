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
 * @file   singleton-definitions.h
 * @author Federico Guerra
 *
 * \brief  Definitions of woss::Singleton template 
 *
 * Definitions of woss::Singleton template
 */


#ifndef SINGLETON_DEFINITIONS_H
#define SINGLETON_DEFINITIONS_H

#include <memory>

namespace woss {

  /**
  * \brief Singleton design pattern template
  *
  * A thread-safe singleton template pattern, useful to decouple classes from singleton 
  * implementation
  **/ 
  template< typename T >
  class Singleton {

    public:

    /**
    * Returns the singleton instance
    * @returns a reference to <i>the_instance</i>
    */
    static T& instance();

#ifdef WOSS_NS_MIRACLE_SUPPORT
    static T* instance_ptr();
#endif // WOSS_NS_MIRACLE_SUPPORT

    ~Singleton() = default;

    /**
    * Disallowed default constructor
    */
    Singleton() = delete;

   /**
    * Disallowed copy constructor
    */
    Singleton( const Singleton& copy ) = delete;

   /**
    * Disallowed move constructor
    */
    Singleton( Singleton&& tmp ) = delete;
   
   /**
    * Disallowed assignment operator
    */
    Singleton& operator=( const Singleton& copy ) = delete;

   /**
    * Disallowed move assignment operator
    */
    Singleton& operator=( Singleton&& tmp ) = delete;
  };

  template <typename T>
  inline T& Singleton<T>::instance() {
    // from C++11 this is guaranteed to be thread safe
    static auto the_instance = std::make_unique<T>();
    return *the_instance;
  }

#ifdef WOSS_NS_MIRACLE_SUPPORT
  template <typename T>
  inline T* Singleton<T>::instance_ptr() {
    // from C++11 this is guaranteed to be thread safe
    static auto the_instance = new T();
    return the_instance;
  }
#endif // WOSS_NS_MIRACLE_SUPPORT
}

#endif // SINGLETON_DEFINITIONS_H

