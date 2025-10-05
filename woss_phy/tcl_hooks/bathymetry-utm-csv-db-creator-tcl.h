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
 * @file   bathymetry-umt-csv-db-creator-tcl.h
 * @author Alberto Signori, Federico Guerra
 * 
 * \brief Provides the interface for woss::BathyUtmCsvDbCreatorTcl Tcl class
 *
 * Provides the interface for the woss::BathyHamburgPortDbCreator Tcl class
 */


#ifndef WOSS_BATHYMETRY_UMT_CSV_DB_CREATOR_TCL_H 
#define WOSS_BATHYMETRY_UMT_CSV_DB_CREATOR_TCL_H


#ifdef WOSS_NS_MIRACLE_SUPPORT

#include <tclcl.h>

#include <bathymetry-utm-csv-db-creator.h>

namespace woss {

  /**
  * \brief Tcl hooks for BathyUtmCsvDbCreatorTcl class
  *
  * Tcl hooks for BathyUtmCsvDbCreatorTcl class
  *
  **/
  class BathyUtmCsvDbCreatorTcl : public BathyUtmCsvDbCreator, public TclObject {

    
    public:

      
    BathyUtmCsvDbCreatorTcl();


    virtual ~BathyUtmCsvDbCreatorTcl() override = default;
    
    /**
    * TCL command interpreter. It implements the following OTcl methods:
    * <ul>
    *  <li><b>setDbPathName &lt;<i>pathname or path identifier</i>&gt;</b>: 
    *     sets the pathname or path identifier. Instantiated WossDb objects will have this pathname
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
    virtual int command(int argc, const char*const* argv) override;
    
    protected:
   
    double debug_;
    
    double woss_db_debug_;
    
    double approx_land_to_sea_surface_;
    
  };

}

#endif // WOSS_NS_MIRACLE_SUPPORT

#endif /* WOSS_BATHYMETRY_GEBCO_DB_CREATOR_TCL_H */
