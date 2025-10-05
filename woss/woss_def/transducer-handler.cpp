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
 * @file   transducer-handler.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of the the woss::TransducerHandler class
 *
 * Provides the implementation of the the woss::TransducerHandler class
 */


#include <fstream>
#include "definitions-handler.h"
#include "transducer-definitions.h"
#include "transducer-handler.h"


using namespace woss;


const std::string TransducerHandler::TRANSDUCER_NOT_VALID = "TRANSDUCER_NOT_VALID";


TransducerHandler::TransducerHandler()
: debug(false),
  transducer_map()
{

}


TransducerHandler::TransducerHandler( TransducerMap& transduc_map )
: debug(false)
{
  transducer_map.swap(transduc_map);
}


void TransducerHandler::initializeNotValid() {
  if ( transducer_map.find(TRANSDUCER_NOT_VALID) == transducer_map.end() )
    transducer_map[TRANSDUCER_NOT_VALID] = std::move(SDefHandler::instance().createTransducer());
}


bool TransducerHandler::insertValue( const std::string& name, const Transducer& transducer ) {
  auto inserted = transducer_map.try_emplace( name, transducer.clone() );
  return inserted.second;
}


TransducerHandler& TransducerHandler::replaceValue( const std::string& name, const Transducer& transducer ) {
  transducer_map[name] = transducer.clone();
  return *this;
}


std::unique_ptr<Transducer> TransducerHandler::getValue( const std::string& name ) const { 
  TMCIter it = transducer_map.find( name );
  if ( it != transducer_map.end() ) {
    return it->second->clone();
  }
  else {
    const_cast<TransducerHandler&>(*this).initializeNotValid();
    return transducer_map.find(TRANSDUCER_NOT_VALID)->second->clone();
  }
}

  
TransducerHandler& TransducerHandler::eraseValue( const std::string& name ){
  transducer_map.erase(name);
  return *this;
}


TransducerHandler& TransducerHandler::clear() {
  transducer_map.clear();
  return *this;
}


bool TransducerHandler::importValueAscii( const std::string& type_name, const std::string& file_name ) {
  std::fstream file_in; 
  file_in.open( file_name.c_str(), std::ios::in );
  assert( file_in.is_open() );

  auto transducer = SDefHandler::instance().createTransducer();
  bool is_ok = transducer->import( file_in );
  if ( is_ok ) 
    transducer_map[type_name] = std::move(transducer);
  return is_ok;
}


bool TransducerHandler::importValueBinary( const std::string& type_name, const std::string& file_name ) {
  std::fstream file_in; 
  file_in.open( file_name.c_str(), std::ios::in | std::ios::binary );
  assert( file_in.is_open() );

  auto transducer = SDefHandler::instance().createTransducer();
  bool is_ok = transducer->importBinary( file_in );
  if ( is_ok ) 
    transducer_map[type_name] = std::move(transducer);
    
  return is_ok;
}


bool TransducerHandler::writeValueAscii( const std::string& type_name, const std::string& file_name ) {
  std::fstream file_out; 
  file_out.open( file_name.c_str(), std::ios::out );
  assert( file_out.is_open() );
  
  TMIter it = transducer_map.find( type_name );
  if ( it == transducer_map.end() ) {
    std::cerr << "TransducerHandler::writeValueAscii() error, type " << type_name 
              << " not found " << std::endl;
    return false;
  }
  return it->second->write( file_out );
}


bool TransducerHandler::writeValueBinary( const std::string& type_name, const std::string& file_name ) {
  std::fstream file_out; 
  file_out.open( file_name.c_str(), std::ios::out | std::ios::binary );
  assert( file_out.is_open() );
  
  TMIter it = transducer_map.find( type_name );
  if ( it == transducer_map.end() ) {
    std::cerr << "TransducerHandler::writeValueBinary() error, type " << type_name 
              << " not found " << std::endl;
    return false;
  }
  return it->second->writeBinary( file_out );
} 

