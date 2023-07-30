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
 * @file   woss-manager-simple.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossManagerSimpleTcl class
 *
 * Provides the interface for the woss::WossManagerSimpleTcl class
 */


#ifndef WOSS_MANAGER_SIMPLE_DEFINITIONS_TCL_H
#define WOSS_MANAGER_SIMPLE_DEFINITIONS_TCL_H


#ifdef WOSS_NS_MIRACLE_SUPPORT

#include <tclcl.h>
#include <time-arrival-definitions.h>
#include <definitions-handler.h>
#include "woss-manager-simple.h"


namespace woss {
 
  
  /**
  * \brief TCL hook class for WossManagerSimple
  *
  * WossManagerSimpleTcl is a TCL hook class for woss::WossManagerSimple
	* 
  */
  template< typename WMResDb = WossManagerResDb >
  class WossManagerSimpleTcl : public WossManagerSimple< WMResDb >, public TclObject {
     
  
    public:


    /**
    * WossManagerSimpleTcl default constructor
    */
    WossManagerSimpleTcl();
    
    virtual ~WossManagerSimpleTcl() { WossManagerSimple<WMResDb>::reset(); }
       
    /**
    * TCL command interpreter. It implements the following OTcl methods:
    * <ul>
    *  <li><b>reset &lt; &gt;</b>: 
    *     resets the object 
    * </ul>
    * 
    * Moreover it inherits all the OTcl method of TclObject
    * 
    * @param argc number of arguments in <i>argv</i>
    * @param argv array of strings which are the comand parameters (Note that argv[0] is the name of the object)
    * 
    * @return TCL_OK or TCL_ERROR whether the command has been dispatched succesfully or not
    * 
    **/
    virtual int command( int argc, const char*const* argv );
    
    
    protected:
    
    
    double debug_;
    
    double is_time_evolution_active_;
  };

  template< typename WMResDb >
  WossManagerSimpleTcl< WMResDb >::WossManagerSimpleTcl()
  : WossManagerSimple<WMResDb>()  
  { 
    TclObject::bind("debug", &this->debug_);
    TclObject::bind("is_time_evolution_active", &this->is_time_evolution_active_);
    TclObject::bind("space_sampling",&this->space_sampling );

    this->debug = (bool) this->debug_;
    this->is_time_evolution_active = (bool) this->is_time_evolution_active_;
  }

  template< typename WMResDb > 
  int WossManagerSimpleTcl< WMResDb >::command( int argc, const char*const* argv ) {
    if(argc==2) {
      if(strcasecmp(argv[1], "reset") == 0) {
        
        if (this->debug) ::std::cout << "WossManagerSimpleTcl::command() reset called"  << ::std::endl;
      
        if ( this->reset() ) return TCL_OK;
        else return TCL_ERROR;
      }
    }
    return TclObject::command(argc,argv);
  }

#ifdef WOSS_MULTITHREAD
  template<>
  int WossManagerSimpleTcl< WossManagerResDbMT >::command( int argc, const char*const* argv ) {
    if(argc==2) {
      if(strcasecmp(argv[1], "reset") == 0) {
        
        if (this->debug) ::std::cout << "WossManagerSimpleTcl::command() reset called"  << ::std::endl;
      
        if ( this->reset() ) return TCL_OK;
        else return TCL_ERROR;
      }
    }
    else if (argc==3) {
      if(strcasecmp(argv[1], "setConcurrentThreads") == 0) {
        int threads = atoi(argv[2]);

        if (this->debug) ::std::cout << "WossManagerSimpleTcl::command() setConcurrentThreads " 
                                     << threads << " called"  << ::std::endl;

        this->setConcurrentThreads(threads);

        return TCL_OK;
      }
    }
    return TclObject::command(argc,argv);
  }
#endif // WOSS_MULTITHREAD
}

#endif // WOSS_NS_MIRACLE_SUPPORT

#endif /* WOSS_MANAGER_SIMPLE_DEFINITIONS_TCL_H */


