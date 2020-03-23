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
 * @file   transducer-handler-tcl.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for the woss::transducer::TransducerHandlerTcl class
 *
 * Provides the interface for the woss::trasducer::TransducerHandlerTcl class
 */


#ifndef WOSS_TRANSDUCER_HANDLER_DEFINITIONS_TCL_H
#define WOSS_TRANSDUCER_HANDLER_DEFINITIONS_TCL_H


#ifdef WOSS_NS_MIRACLE_SUPPORT


#include <transducer-handler.h>
#include <tclcl.h>


namespace woss {    
  
    
  /**
  * \brief Tcl hooks for TransducerHandler class
	* 
  * Tcl hooks for TransducerHandler class
  *
  **/
  class TransducerHandlerTcl : public TransducerHandler, public TclObject {

    
    public:
    

    /**
    * TransducerHandlerTcl default constructor
    **/        
    TransducerHandlerTcl();

		
    virtual ~TransducerHandlerTcl() { }
     

    /**
    * TCL command interpreter. It implements the following OTcl methods:
    * <ul>
    *  <li><b>importAscii &lt;<i>transducer type name</i>&gt; &lt;<i>filepath</i>&gt; </b>: 
    *      imports a transducer from given ascii file and stores it inside its map with key == given type name
    *  <li><b>importBinary &lt;<i>transducer type name</i>&gt; &lt;<i>filepath</i>&gt; </b>: 
    *      imports a transducer from given binary file and stores it inside its map with key == given type name
    *  <li><b>writeAscii &lt;<i>transducer type name</i>&gt; &lt;<i>filepath</i>&gt; </b>: 
    *      writes a transducer associated to given type name to given ascii file   
    *  <li><b>writeBinary &lt;<i>transducer type name</i>&gt; &lt;<i>filepath</i>&gt; </b>:   
    *      writes a transducer associated to given type name to given binary file       
    * </ul>
    * 
    * Moreover it inherits all the OTcl method of TclObject
    * 
    * 
    * @param argc number of arguments in <i>argv</i>
    * @param argv array of strings which are the comand parameters (Note that argv[0] is the name of the object)
    * 
    * @return TCL_OK or TCL_ERROR whether the command has been dispatched succesfully or no
    * 
    **/
    virtual int command( int argc, const char*const* argv );    
     

    protected:
    
    
    double debug_;
    

  };
  
	
}


#endif // WOSS_NS_MIRACLE_SUPPORT


#endif // WOSS_TRANSDUCER_HANDLER_DEFINITIONS_TCL_H

