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
 * @file   woss-utilities-tcl.h
 * @author Federico Guerra
 *
 * \brief  Definitions and library for woss::WossUtilitiesTcl class 
 *
 * Definitions and library for woss::WossUtilitiesTcl class
 */


#ifndef WOSS_UTILITIES_TCL_H
#define WOSS_UTILITIES_TCL_H


#ifdef WOSS_NS_MIRACLE_SUPPORT
#include <tclcl.h>


namespace woss {
      
  /**
  * \brief WossUtilitiesTcl class has the task of exporting in TCL some WOSS functionality
  *
  * Custom WOSS methods should be exported in TCL by inheriting from WossUtilities  
  **/ 
  class WossUtilitiesTcl : public TclObject {

    
    public:
    
      
    /**
    * Default WossUtilities constructor
    **/ 
    WossUtilitiesTcl();
    
    
    virtual ~WossUtilitiesTcl() { }
    
    /**
    * TCL command interpreter. It implements the following OTcl methods:
    * <ul>
    *  <li><b>getLatfromDistBearing &lt;<i>latitude [decimal degree]</i>&gt; &lt;<i>longitude [decimal degree]</i>&gt; 
    *         &lt;<i>bearing [decimal degree]</i>&gt; &lt;<i>distance [m]</i>&gt; </b>: 
    *     gets the latitude [decimal degree] of destination point at given distance and bearing from given source coordinates 
    *  <li><b>getLongfromDistBearing &lt;<i>latitude [decimal degree]</i>&gt; &lt;<i>longitude [decimal degree]</i>&gt; 
    *         &lt;<i>bearing [decimal degree]</i>&gt; &lt;<i>distance [m]</i>&gt; </b>: 
    *     gets the longitude [decimal degree] of destination point at given distance and bearing from given source coordinates
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
    virtual int command(int argc, const char*const* argv);

    
    protected:

      
    /**
    * TCL binded debug flag 
    **/ 
    double debug;
    
    
    ::std::string convertMonth( int month );
    
  };

}
#endif // WOSS_NS_MIRACLE_SUPPORT

#endif /* WOSS_UTILITIES_TCL_H */

