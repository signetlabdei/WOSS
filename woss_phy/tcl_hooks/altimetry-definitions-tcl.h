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
 * @file   altimetry-definitions-tcl.h
 * @author Federico Guerra
 *
 * \brief  Definitions and library for woss::Altimetry Tcl class 
 *
 * Definitions and library for woss::Altimetry Tcl class
 */


#ifndef ALTIMETRY_DEFINITIONS_TCL_H
#define ALTIMETRY_DEFINITIONS_TCL_H


#ifdef WOSS_NS_MIRACLE_SUPPORT

#include <altimetry-definitions.h>
#include <tclcl.h>


namespace woss {

  /**
  * \brief Tcl hook class, needed by Tcl::command() of woss::DefHandler
  *
  * Tcl hook class, needed by Tcl::command() of woss::DefHandler
  **/  
  class AltimetryTcl : public Altimetry, public TclObject { 
    
   public:
     
    AltimetryTcl();
    
    /**
    * TCL command interpreter. It implements the following OTcl methods:
    * <ul>
    *  <li><b>setDebug &lt;<i>debug value</i>&gt;</b>: 
    *      <b>optional</b> command that configures the Altimetry debug flag
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
    
  };

	
  /**
   * \brief Tcl hook class, needed by Tcl::command() of woss::DefHandler
   *
   * Tcl hook class, needed by Tcl::command() of woss::DefHandler
   **/  
  class AltimBretschneiderTcl : public AltimBretschneider, public TclObject { 
    
  public:
    
    AltimBretschneiderTcl();

    /**
    * TCL command interpreter. It implements the following OTcl methods:
    * <ul>
    *  <li><b>setDebug &lt;<i>debug value</i>&gt;</b>: 
    *      <b>optional</b> command that configures the Altimetry debug flag
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
  };

}

#endif // WOSS_NS_MIRACLE_SUPPORT
  
#endif /* ALTIMETRY_DEFINITIONS_TCL_H */


 
