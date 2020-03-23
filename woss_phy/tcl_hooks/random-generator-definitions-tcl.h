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
 * @file   random-generator-definitions-tcl.h
 * @author Federico Guerra
 *
 * \brief  Definitions and library for woss::RandomGeneratorTcl class 
 *
 * Definitions and library for woss::RandomGeneratorTcl class
 */


#ifndef RANDOM_GENERATOR_DEFINITIONS_TCL_H
#define RANDOM_GENERATOR_DEFINITIONS_TCL_H 


#ifdef WOSS_NS_MIRACLE_SUPPORT
#include <tclcl.h>
#include <random-generator-definitions.h>


namespace woss {
  
  
  /**
  * \brief Tcl hook class, needed by Tcl::command of DefHandler
  *
  * Tcl hook class, needed by Tcl::command of DefHandler
  **/ 
  class RandomGeneratorTcl : public RandomGenerator, public TclObject { 
    
    
    public:
      
    
    /**
    * Default constructor
    **/      
    RandomGeneratorTcl();
    
      
    /**
    * TCL command interpreter. It implements the following OTcl methods:
    * <ul>
    *  <li><b>initialize &lt;gt; </b>:
    *     initializes current RandomGenerator object;
    * </ul>
    * 
    * Moreover it inherits all the OTcl method of TclObject
    * 
    * 
    * @param argc number of arguments in <i>argv</i>
    * @param argv array of strings which are the command parameters (Note that argv[0] is the name of the object)
    * 
    * @return TCL_OK or TCL_ERROR whether the command has been dispatched succesfully or not
    * 
    **/    
    virtual int command(int argc, const char*const* argv);  

    
  };
  
  
}


#endif // WOSS_NS_MIRACLE_SUPPORT


#endif /* RANDOM_GENERATOR_DEFINITIONS_TCL_H */

