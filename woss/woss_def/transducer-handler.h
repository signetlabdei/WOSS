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
 * @file   transducer-handler.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for the woss::transducer::TransducerHandler class
 *
 * Provides the interface for the woss::trasducer::TransducerHandler class
 */


#ifndef WOSS_TRANSDUCER_HANDLER_DEFINITIONS_H
#define WOSS_TRANSDUCER_HANDLER_DEFINITIONS_H


// #include <cmath>
// #include <cassert>
// #include <iostream>
#include <string>
#include <map>


namespace woss {    
  
  
  class Transducer;
  
  
  /**
  * \brief Transducer creator and handler class
  *
  * woss::TransducerHandler class imports and saves woss::Transducer. It provides access to them via string name.
  *
  **/
  class TransducerHandler {

		
    protected:
      
      
    /**
    * Map that links a string to a pointer to a woss::Transducer
    **/      
    typedef ::std::map< ::std::string, Transducer* > TransducerMap;
    typedef TransducerMap::iterator TMIter;
    typedef TransducerMap::reverse_iterator TMRIter;
    typedef TransducerMap::const_iterator TMCIter;
    typedef TransducerMap::const_reverse_iterator TMCRIter;
    
    
    public:
    

    /**
    * TransducerHandler default constructor
    **/        
    TransducerHandler();

    /**
    * TransducerHandler copy constructor
    * @param copy const reference a TransducerHandler
    **/      
    TransducerHandler( const TransducerHandler& copy );
    
    /**
    * TransducerHandler constructor.
    * @param transduc_map TransducerMap to be inserted
    **/     
    TransducerHandler( TransducerMap& transduc_map );
      
    
    virtual ~TransducerHandler();
     
   
    /**
    * Inserts and doesn't replace if another pointer to a woss::Transducer is found for given key; in this case
    * the given pointer is deleted.
    * @param name type name
    * @param transducer constant pointer to a woss::Transducer
    * @return <i>true</i> if inserted, <i>false</i> otherwise
    **/
    bool insertValue( const ::std::string& name, Transducer* const transducer );
    

    /**
    * Replaces a pointer to a woss::Transducer for given key. The old value is deleted
    * @param name type name
    * @param transducer constant pointer to a woss::Transducer
    * @return <i>true</i> if inserted, <i>false</i> otherwise
    **/
    TransducerHandler& replaceValue( const ::std::string& name, Transducer* const transducer );
    
    
    /**
    * Returns a const iterator to the woss::Transducer for given string.
    * @param name const reference to string
    * @returns constant pointer to a constant woss::Transducer that is invalid if the given key is not found.
    **/    
    const Transducer* const getValue( const ::std::string& name ) const;

    
    /**
    * Erases and deletes the pointer to a woss::Transducer for given string
    * @param name const reference to string
    * @return reference to <b>*this</b>
    **/        
    TransducerHandler& eraseValue( const ::std::string& name );
    
    
    /**
    * Returns the number of pointers stored
    * @return number of pointers stored
    **/
    int size() const;

    /**
    * Checks if the instance has any stored values
    * @return <i>true</i> if condition applies, <i>false</i> otherwise
    **/
    bool empty() const;
    
    
    /**
    * Deletes all pointers and clears the map
    * @return reference to <b>*this</b>
    **/       
    TransducerHandler& clear();
    
    
    /**
    * Returns a const iterator to the beginning of the tranducer map
    * @returns const iterator 
    **/
    TMCIter begin() const;
    
    /**
    * Returns a const iterator to the end of the tranducer map
    * @returns const iterator 
    **/    
    TMCIter end() const;
    
    /**
    * Returns a const reverse iterator to the reverse beginning of the tranducer map
    * @returns const reverse iterator 
    **/    
    TMCRIter rbegin() const;
    
    /**
    * Returns a const reverse iterator to the reverse end of the tranducer map
    * @returns const reverse iterator 
    **/    
    TMCRIter rend() const;          
    
    /**
    * Imports a woss::Transducer from the given file and with given string key
    * @param type_name transducer type name
    * @param file_name file path
    * @return true if method was successful, false otherwise
    **/
    virtual bool importValueAscii( const ::std::string& type_name, const ::std::string& file_name );
    
    /**
    * Imports a woss::Transducer from the given binary file and with given string key
    * @param type_name transducer type name
    * @param file_name file path
    * @return true if method was successful, false otherwise
    **/
    virtual bool importValueBinary( const ::std::string& type_name, const ::std::string& file_name );
    
   
    /**
    * Writes a woss::Transducer to the given file
    * @param type_name transducer type name
    * @param file_name file path
    * @return true if method was successful, false otherwise
    **/
    virtual bool writeValueAscii( const ::std::string& type_name, const ::std::string& file_name );
    
    /**
    * Writes a woss::Transducer to the given binary file
    * @param type_name transducer type name
    * @param file_name file path
    * @return true if method was successful, false otherwise
    **/
    virtual bool writeValueBinary( const ::std::string& type_name, const ::std::string& file_name );    
    
    
    /**
    * Assignment operator
    * @param copy const reference to a TransducerHandler object to be copied
    * @return TransducerHandler reference to <i>this</i>
    **/
    TransducerHandler& operator=( const TransducerHandler& x ) ;

     
    /**
    * Sets debug flag for all instances
    * @param flag debug bool
    **/
    TransducerHandler& setDebug( bool flag );
            
		bool getDebug() { return debug; }
		
    
    protected:
      
 
    /**
    * string key that represents an invalid woss::Transducer
    **/      
    static const ::std::string TRANSDUCER_NOT_VALID;
    
    
    /**
    * debug flag
    **/    
    bool debug;

    
    /**
    * beam pattern map
    **/    
    TransducerMap transducer_map;
              
    
  };
    
   
  /////////////////

  inline int TransducerHandler::size() const {
    return transducer_map.size();
  }


  inline bool TransducerHandler::empty() const {
    return transducer_map.empty();
  }
   

  inline TransducerHandler::TMCIter TransducerHandler::begin() const {
    return transducer_map.begin();
  }
  
  
  inline TransducerHandler::TMCIter TransducerHandler::end() const {
    return transducer_map.end();
  }
  

  inline TransducerHandler::TMCRIter TransducerHandler::rbegin() const {
    return transducer_map.rbegin();  
  }

  inline TransducerHandler::TMCRIter TransducerHandler::rend() const {
    return transducer_map.rend();  
  }    


  inline TransducerHandler& TransducerHandler::setDebug( bool flag ) {
    debug = flag;
    return *this;
  }

  
}


#endif // WOSS_TRANSDUCER_HANDLER_DEFINITIONS_H

