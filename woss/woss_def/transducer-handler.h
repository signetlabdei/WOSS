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
 * @file   transducer-handler.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for the woss::transducer::TransducerHandler class
 *
 * Provides the interface for the woss::trasducer::TransducerHandler class
 */


#ifndef WOSS_TRANSDUCER_HANDLER_DEFINITIONS_H
#define WOSS_TRANSDUCER_HANDLER_DEFINITIONS_H


#include <string>
#include <unordered_map>
#include <memory>


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
    using TransducerMap = std::unordered_map< std::string, std::unique_ptr<Transducer> >;
    using TMIter = TransducerMap::iterator;
    using TMCIter = TransducerMap::const_iterator;

    public:

    /**
    * TransducerHandler default constructor
    **/        
    TransducerHandler();

    TransducerHandler( const TransducerHandler& copy ) = default;

    TransducerHandler( TransducerHandler&& tmp ) = default;
    
    /**
    * TransducerHandler constructor.
    * @param transduc_map TransducerMap to be inserted
    **/     
    TransducerHandler( TransducerMap& transduc_map );

    virtual ~TransducerHandler() = default;

    /**
    * Inserts and doesn't replace if another pointer to a woss::Transducer is found for given key; in this case
    * the given pointer is deleted.
    * @param name type name
    * @param transducer constant pointer to a woss::Transducer
    * @return <i>true</i> if inserted, <i>false</i> otherwise
    **/
    bool insertValue( const std::string& name, const Transducer& transducer );

    /**
    * Replaces a pointer to a woss::Transducer for given key. The old value is deleted
    * @param name type name
    * @param transducer constant pointer to a woss::Transducer
    * @return <i>true</i> if inserted, <i>false</i> otherwise
    **/
    TransducerHandler& replaceValue( const std::string& name, const Transducer& transducer );

    /**
    * Returns a std::unique_ptr to a  woss::Transducer for the input string.
    * @param name const reference to string
    * @returns a std::unique_ptr to a heap allocated woss::Transducer that is invalid if the given key is not found.
    **/
    std::unique_ptr<Transducer> getValue( const std::string& name ) const;

    /**
    * Erases and deletes the pointer to a woss::Transducer for given string
    * @param name const reference to string
    * @return reference to <b>*this</b>
    **/
    TransducerHandler& eraseValue( const std::string& name );

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
    * Imports a woss::Transducer from the given file and with given string key
    * @param type_name transducer type name
    * @param file_name file path
    * @return true if method was successful, false otherwise
    **/
    virtual bool importValueAscii( const std::string& type_name, const std::string& file_name );

    /**
    * Imports a woss::Transducer from the given binary file and with given string key
    * @param type_name transducer type name
    * @param file_name file path
    * @return true if method was successful, false otherwise
    **/
    virtual bool importValueBinary( const std::string& type_name, const std::string& file_name );

    /**
    * Writes a woss::Transducer to the given file
    * @param type_name transducer type name
    * @param file_name file path
    * @return true if method was successful, false otherwise
    **/
    virtual bool writeValueAscii( const std::string& type_name, const std::string& file_name );

    /**
    * Writes a woss::Transducer to the given binary file
    * @param type_name transducer type name
    * @param file_name file path
    * @return true if method was successful, false otherwise
    **/
    virtual bool writeValueBinary( const std::string& type_name, const std::string& file_name );    

    TransducerHandler& operator=( const TransducerHandler& copy ) = default;

    TransducerHandler& operator=( TransducerHandler&& copy ) = default;

    /**
    * Sets debug flag for all instances
    * @param flag debug bool
    **/
    TransducerHandler& setDebug( bool flag );

    bool getDebug() const { return debug; }

    protected:

    void initializeNotValid();

    /**
    * string key that represents an invalid woss::Transducer
    **/
    static const std::string TRANSDUCER_NOT_VALID;

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

  inline TransducerHandler& TransducerHandler::setDebug( bool flag ) {
    debug = flag;
    return *this;
  }

}

#endif // WOSS_TRANSDUCER_HANDLER_DEFINITIONS_H

