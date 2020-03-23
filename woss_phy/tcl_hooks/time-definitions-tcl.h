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
 * @file   time-definitions-tcl.h
 * @author Federico Guerra
 *
 * \brief  Definitions and library for woss::TimeTcl class
 *
 * Definitions and library for woss::TimeTcl
 */


#ifndef WOSS_TIME_DEFINITIONS_TCL_H
#define WOSS_TIME_DEFINITIONS_TCL_H


#ifdef WOSS_NS_MIRACLE_SUPPORT


#include <time-definitions.h>
#include <tclcl.h>


namespace woss {


	/**
  * \brief Tcl hook class for Time class
  *
  * Tcl hook class for Time class
  **/    
  class TimeTcl : public Time, public TclObject {
    
    
    public:
      
      
    /**
    * TCL command interpreter. It implements the following OTcl methods:
    * <ul>
    *  <li><b>setTime &lt;<i>day [1-31]</i>&gt; &lt;<i>month [1-12]</i>&gt; &lt;<i>year [>1900]</i>&gt;
    *                           &lt;<i>hours [0-23]</i>&gt; &lt;<i>minutes [0-59]</i>&gt; &lt;<i>seconds [0-59]</i>&gt;</b>: 
    *     sets the time value for current object;
    *  <li><b>setYear &lt;<i>year [>1900]</i>&gt</b>: 
    *     sets the year for current object;
    *  <li><b>setMonth &lt;<i>month [1-12]</i>&gt; </b>: 
    *     sets the month for current object;
    *  <li><b>setDay &lt;<i>day [1-31]</i>&gt; </b>: 
    *     sets the day for current object;
    *  <li><b>setHours &lt;<i>hours [0-23]</i>&gt; </b>: 
    *     sets the hours for current object;
    *  <li><b>setMinutes &lt;<i>minutes [0-59]</i>&gt; </b>: 
    *     sets the minutes for current object;
    *  <li><b>setSeconds &lt;<i>seconds [0-59]</i>&gt; </b>: 
    *     sets the seconds for current object;
    * </ul>
    *
    * @param argc number of arguments in <i>argv</i>
    * @param argv array of strings which are the comand parameters (Note that argv[0] is the name of the object)
    * 
    * @return TCL_OK or TCL_ERROR whether the command has been dispatched succesfully or not
    * 
    **/    
    virtual int command( int argc, const char*const* argv );
    
    
  };
	
	
}

#endif // WOSS_NS_MIRACLE_SUPPORT

#endif /* WOSS_TIME_DEFINITIONS_TCL_H */


