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
 * @file   transducer-handler-tcl.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of the the woss::TransducerHandlerTcl class
 *
 * Provides the implementation of the the woss::TransducerHandlerTcl class
 */


#ifdef WOSS_NS_MIRACLE_SUPPORT


#include <iostream>
#include <cassert>
#include <fstream>
#include <transducer-definitions.h>
#include "transducer-handler-tcl.h"


using namespace woss;


static class TransducerHandlerClass : public TclClass {
  public:
  TransducerHandlerClass() : TclClass("WOSS/Definitions/TransducerHandler") {}
  TclObject* create(int, const char*const*) {
    return (new TransducerHandlerTcl());
  }
} class_TransducerHandlerClass;


TransducerHandlerTcl::TransducerHandlerTcl()
: TransducerHandler()
{
  bind( "debug", &debug_);
  
  debug = (bool) debug_;
}


int TransducerHandlerTcl::command( int argc, const char*const* argv ) {
  if ( argc == 4) {
    if(strcasecmp(argv[1], "importAscii") == 0) {
      ::std::string type_name = argv[2];
      ::std::string file_path = argv[3];
      
      if (debug) ::std::cout << "TransducerHandlerTcl::command() importAscii called, type = " 
                             << type_name <<"; path = " << argv[3] << ::std::endl;

      bool is_ok = importValueAscii( type_name, file_path );
      
      if ( is_ok ) return TCL_OK;
      else return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "importBinary") == 0) {
      ::std::string type_name = argv[2];
      ::std::string file_path = argv[3];
      
      if (debug) ::std::cout << "TransducerHandlerTcl::command() importBinary called, type = " 
                             << type_name <<"; path = " << argv[3] << ::std::endl;

      bool is_ok = importValueBinary( type_name, file_path );
      
      if ( is_ok ) return TCL_OK;
      else return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "writeBinary") == 0) {      
      ::std::string type_name = argv[2];
      ::std::string file_path = argv[3];
      
      if (debug) ::std::cout << "TransducerHandlerTcl::command() writeValueBinary called, type = " 
                             << type_name <<"; path = " << argv[3] << ::std::endl;

      bool is_ok = writeValueBinary( type_name, file_path );
      
      if ( is_ok ) return TCL_OK;
      else return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "writeAscii") == 0) {
      ::std::string type_name = argv[2];

      if (debug) ::std::cout << "TransducerHandlerTcl::command() writeAscii called, type = " 
                             << type_name <<"; path = " << argv[3] << ::std::endl;
      
      ::std::fstream file_out; 
      file_out.open( argv[3], ::std::ios::out );
      assert( file_out.is_open() );
      
      TMIter it = transducer_map.find( type_name );
      if ( it == transducer_map.end() ) {
        ::std::cerr << "TransducerHandlerTcl::command() writeAscii error, type " << type_name 
                    << " not found " << ::std::endl;
                    
        return TCL_ERROR;
      }
      
      bool is_ok = it->second->write( file_out );

      if ( is_ok ) return TCL_OK;
      else return TCL_ERROR;  
    }    
  }
  return TclObject::command(argc, argv);
}   


#endif // WOSS_NS_MIRACLE_SUPPORT
