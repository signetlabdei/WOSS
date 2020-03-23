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
 * @file   singleton-definitions.h
 * @author Federico Guerra
 *
 * \brief  Definitions of woss::Singleton template 
 *
 * Definitions of woss::Singleton template
 */


#ifndef SINGLETON_DEFINITIONS_H
#define SINGLETON_DEFINITIONS_H


namespace woss {

  
  /**
  * \brief Singleton design pattern template
  *
  * A simple singleton template pattern, useful to decouple classes from singleton 
  * implementation
  **/ 
  template< typename T >
  class Singleton {


    public:
    
    /**
    * Returns the singleton instance
    * @returns a pointer to <i>the_instance</i>
    */
    static T* instance();
   

    private:
      
 
    /**
    * static heap instance
    */
    static T* the_instance;
    
    
    /**
    * Disallowed default constructor
    */
    Singleton() { }
    
    /**
    * Disallowed copy constructor
    */
    Singleton( const Singleton& copy ) { }
    
    /**
    * Disallowed assignment operator
    */
    Singleton& operator=( const Singleton& copy ) { return( *this ); }


  };
  

  template <typename T>
  T* Singleton<T>::the_instance = 0;

  
  template <typename T>
  inline T* Singleton<T>::instance() { 
    if (the_instance) 
      return the_instance; 
    else { 
      the_instance = new T(); 
      return the_instance; 
    } 
  }


}


#endif // SINGLETON_DEFINITIONS_H

