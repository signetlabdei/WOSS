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
 * @file   definitions-handler-tcl.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::DefHandlerTcl class
 *
 * Provides the interface for the woss::DefHandlerTcl class
 */


#ifndef WOSS_DEF_HANDLER_TCL_H
#define WOSS_DEF_HANDLER_TCL_H

#ifdef WOSS_NS_MIRACLE_SUPPORT

#include <tclcl.h>

namespace woss {

  /**
  * \brief Class for managing dynamic instantiation of foundation classes
  *
  */
  
  class DefHandlerTcl : public TclObject {
    

    public:
		
   
    DefHandlerTcl();
	
		
    /**
    * TCL command interpreter. It implements the following OTcl methods:
    * <ul>
    *  <li><b>setSSPCreator &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>mandatory</b> command that stores a the pointer to a woss::SSP
    *  <li><b>setSedimentCreator &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>mandatory</b> command that stores a the pointer to a woss::Sediment
    *  <li><b>setPressureCreator &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>mandatory</b> command that stores a the pointer to a woss::Pressure
    *  <li><b>setTimeArrCreator &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>mandatory</b> command that stores a the pointer to a woss::TimeArr
    *  <li><b>setAltimetryCreator &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>mandatory</b> command that stores a the pointer to a woss::Altimetry
    *  <li><b>setTimeReference &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>mandatory</b> command that stores a the pointer to a woss::TimeReference
    *  <li><b>setRandomGenerator &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>mandatory</b> command that stores a the pointer to a woss::RandomGenerator
    *  <li><b>setTransducerCreator &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>mandatory</b> command that stores a the pointer to a woss::Transducer
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
    virtual int command( int argc, const char*const* argv ) override;

    
    protected:

    double debug;

  };

}

#endif // WOSS_NS_MIRACLE_SUPPORT
	
#endif /* WOSS_DEF_HANDLER_TCL_H */ 
