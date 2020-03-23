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
 * @file   woss-controller.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossController class
 *
 * Provides the interface for the woss::WossController class
 */


#ifndef WOSS_CONTROLLER_TCL_H
#define WOSS_CONTROLLER_TCL_H


#ifdef WOSS_NS_MIRACLE_SUPPORT


#include <woss-controller.h>
#include <tclcl.h>


namespace woss {
  

  /**
  * \brief Tcl hooks class for SWossController
  *
  * Tcl hooks class for WossControllerTcl
  */
  class WossControllerTcl : public TclObject {
   
    public:
      
    /**
    * Default constructor
    */
    WossControllerTcl();
       
    
    /**
    * Destructor
    */
    virtual ~WossControllerTcl() { }
    

    /**
    * TCL command interpreter. It implements the following OTcl methods:
    * <ul>
    *  <li><b>setBathymetryDbCreator &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>optional</b> command that stores a pointer to the woss::WossDbCreator for the woss::WossBathymetryDb
    *  <li><b>setSedimentDbCreator &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>optional</b> command that stores a a pointer to the woss::WossDbCreator for the woss::WossSedimentDb
    *  <li><b>setSSPDbCreator &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>optional</b> command that stores a the pointer to the woss::WossDbCreator for the woss::WossSSPDb
    *  <li><b>setPressureResultsDbCreator &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>optional</b> command that stores a pointer to the woss::WossDbCreator for the woss::WossResPressureDb
    *  <li><b>setTimeArrResultsDbCreator &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>optional</b> command that stores a pointer to the woss::WossDbCreator for the woss::WossResTimeArrDb
    *  <li><b>setWossCreator &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>mandatory</b> command that stores a pointer to the woss::WossCreator
    *  <li><b>setWossDbManager &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>mandatory</b> command that stores a pointer to the woss::WossDbManager
    *  <li><b>setWossManager &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>mandatory</b> command that stores a pointer to the woss::WossManager
    *  <li><b>setTransducerHandler &lt;<i>Tcl object</i>&gt;</b>: 
    *     <b>mandatory</b> command that stores a the pointer to a woss::TransducerHandler    
    *  <li><b>initialize &lt;&gt;</b>: 
    *     <b>mandatory</b> command that initialize all internal connections
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
    
      
    double debug;
    
  };

  /**
  * \brief Singleton implementation of WossControllerTcl class
  *
  * Singleton implementation of WossControllerTcl class
  */
  typedef Singleton< WossControllerTcl > SWossControllerTcl;  

  
}


#endif // WOSS_NS_MIRACLE_SUPPORT


#endif /* WOSS_CONTROLLER_TCL_H */

