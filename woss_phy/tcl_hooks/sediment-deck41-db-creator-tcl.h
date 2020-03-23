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
 * @file   sediment-deck41-db-creator-tcl.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::SedimDeck41DbCreatorTcl class
 *
 * Provides the interface for the woss::SedimDeck41DbCreatorTcl class
 */


#ifndef WOSS_SEDIMENT_DECK41_DB_CREATOR_TCL_H 
#define WOSS_SEDIMENT_DECK41_DB_CREATOR_TCL_H


#ifdef WOSS_NS_MIRACLE_SUPPORT

#include <tclcl.h>

#ifdef WOSS_NETCDF_SUPPORT

#include <sediment-deck41-db-creator.h>


namespace woss {
  
    
  /**
  * \brief Tcl hooks for SedimDeck41DbCreator class
  *
  * Tcl hooks for SedimDeck41DbCreator class
  **/
  class SedimDeck41DbCreatorTcl : public SedimDeck41DbCreator, public TclObject {

    
    public:
    
      
    /**
    * SedimDeck41DbCreatorTcl default constructor
    **/
    SedimDeck41DbCreatorTcl();
    
    virtual ~SedimDeck41DbCreatorTcl() { }
    
        
    /**
    * TCL command interpreter. It implements the following OTcl methods:
    * <ul>
    *  <li><b>setDbPathName &lt;<i>pathname or path identifier</i>&gt;</b>: 
    *     sets the pathname or path identifier. Instantiated WossDb objects will have this pathname
    *  <li><b>setUpDeck41CoordinatesDb &lt;<i>pathname or path identifier</i>&gt;</b>: 
    *     sets the pathname or path identifier for the NetCDF DECK41 coordinates database
    *  <li><b>setUpDeck41MarsdenDb &lt;<i>pathname or path identifier</i>&gt;</b>: 
    *     sets the pathname or path identifier for the NetCDF DECK41 marsden square database
    *  <li><b>setUpDeck41MarsdenOneDb &lt;<i>pathname or path identifier</i>&gt;</b>: 
    *     sets the pathname or path identifier for the NetCDF DECK41 marsden coordinates database
    * </ul>
    * 
    * Moreover it inherits all the OTcl methods of WossDbCreator
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
    
    
    double debug_;
    
    double woss_db_debug_;
    
    
  };

  
}


#endif // WOSS_NETCDF_SUPPORT

#endif // WOSS_NS_MIRACLE_SUPPORT

#endif /* WOSS_SEDIMENT_DECK41_DB_CREATOR_TCL_H */

