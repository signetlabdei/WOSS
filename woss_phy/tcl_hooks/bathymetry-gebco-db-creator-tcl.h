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
 * @file   bathymetry-gebco-db-creator-tcl.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::BathyGebcoDbCreator Tcl class
 *
 * Provides the interface for the woss::BathyGebcoDbCreator Tcl class
 */


#ifndef WOSS_BATHYMETRY_GEBCO_DB_CREATOR_TCL_H 
#define WOSS_BATHYMETRY_GEBCO_DB_CREATOR_TCL_H


#ifdef WOSS_NS_MIRACLE_SUPPORT

#include <tclcl.h>

#ifdef WOSS_NETCDF_SUPPORT

#include <bathymetry-gebco-db-creator.h>

namespace woss {
  

  /**
  * \brief Tcl hooks for BathyGebcoDbCreator class
  *
  * Tcl hooks for BathyGebcoDbCreator class
  *
  **/
  class BathyGebcoDbCreatorTcl : public BathyGebcoDbCreator, public TclObject {

    
    public:

      
    BathyGebcoDbCreatorTcl();

    
    virtual ~BathyGebcoDbCreatorTcl() { }
    
    /**
    * TCL command interpreter. It implements the following OTcl methods:
    * <ul>
    *  <li><b>useOneMinutePrecision &lt; &gt;</b>: 
    *     notifies the database that the GEBCO version with one minute of arc precision is in use 
    *  <li><b>useThirtySecondsPrecision &lt; &gt;</b>: 
    *     notifies the database that the GEBCO version with thirty seconds of arc precision is in use 
    * </ul>
    * 
    * Moreover it inherits all the OTcl method of WossDbCreator
    * 
    * 
    * @param argc number of arguments in <i>argv</i>
    * @param argv array of strings which are the comand parameters (Note that argv[0] is the name of the object)
    * 
    * @return TCL_OK or TCL_ERROR whether the command has been dispatched succesfully or not
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

#endif /* WOSS_BATHYMETRY_GEBCO_DB_CREATOR_TCL_H */

