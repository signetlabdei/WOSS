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
 * @file   woss-db-manager.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossDbManager class
 *
 * Provides the interface for the woss::WossDbManager class
 */


#ifndef WOSS_DB_MANAGER_IMPLEMENT_TCL_H 
#define WOSS_DB_MANAGER_IMPLEMENT_TCL_H


#ifdef WOSS_NS_MIRACLE_SUPPORT


#include <woss-db-manager.h>
#include <tclcl.h>


namespace woss {
 
  
  /**
  * \brief Tcl hooks for WossDbManager class
  *
  * Tcl hooks for WossDbManager class
	* 
  **/
  class WossDbManagerTcl : public WossDbManager, public TclObject {

    
    public: 
      
      
    /**
    * Default constructor
    **/
    WossDbManagerTcl();
    
     
    /**
    * Destructor. It deletes all pointers involved. Derived classes 
    * <b>don't have to do it</b>
    **/ 
    virtual ~WossDbManagerTcl() { }


    /**
    * TCL command interpreter. It implements the following OTcl methods:
    * <ul>
    *  <li><b>getBathymetry &lt;<i>latitude [dec degree]</i>&gt; &lt;<i>longitude [dec degree]</i>&gt;</b>: 
    *     return the bathymetry of given coordinates. HUGE_VAL is returned if they are not found
    *  <li><b>setCustomSediment &lt;<i>tx latitude [decimal degrees]</i>&gt; &lt;<i>tx longitude [decimal degrees]</i>&gt; &lt;<i>bearing [decimal degrees]</i>&gt;
    *                           &lt;<i>range [m]</i>&gt; &lt;<i>type name</i>&gt; &lt;<i>compressional velocity [m/s]</i>&gt; &lt;<i>shear velocity [m/s]</i>&gt; 
    *                           &lt;<i>density [g/cm^3 or user defined]</i>&gt; &lt;<i>compressional attenuation [db/wavelength or user defined]</i>&gt; 
    *                           &lt;<i>shear attenuation [db/wavelength or user defined]</i>&gt; &lt;<i>sediment depth [m]</i>&gt; </b>: 
    *     sets the custom Sediment for given transmitter coordinates, bearing and range;
    *  <li><b>setCustomSediment &lt;<i>type name</i>&gt; &lt;<i>compressional velocity [m/s]</i>&gt; &lt;<i>shear velocity [m/s]</i>&gt; 
    *                           &lt;<i>density [g/cm^3 or user defined]</i>&gt; &lt;<i>compressional attenuation [db/wavelength or user defined]</i>&gt; 
    *                           &lt;<i>shear attenuation [db/wavelength or user defined]</i>&gt; &lt;<i>sediment depth [m]</i>&gt; </b>: 
    *     sets the custom Sediment for all Woss instances;
    *  <li><b>setCustomAltimetry &lt;<i>tx latitude [decimal degrees]</i>&gt; &lt;<i>tx longitude [decimal degrees]</i>&gt; &lt;<i>bearing [decimal degrees]</i>&gt;
    *                           &lt;<i>range [m]</i>&gt; &lt;<i>AltimetryTcl object</i>&gt; </b>: 
    *     sets the custom Altimetry for given transmitter coordinates, bearing and range;
    *  <li><b>setCustomAltimetry &lt;<i>AltimetryTcl object</i>&gt; </b>: 
    *     sets the custom Altimetry for all Woss instances;
    *  <li><b>setCustomSSP &lt;<i>tx latitude [decimal degrees]</i>&gt; &lt;<i>tx longitude [decimal degrees]</i>&gt; &lt;<i>bearing [decimal degrees]</i>&gt;
    *                      &lt;<i>TimeTcl object</i>&gt; &lt;<i>SSP file pathname</i>&gt; </b>: 
    *     imports from file and sets the custom SSP for given transmitter coordinates, bearing and Time; @see importCustomSSP
    *  <li><b>setCustomSSP &lt;<i>TimeTcl object</i>&gt; &lt;<i>SSP file pathname</i>&gt; </b>: 
    *     imports from file and sets the custom SSP for all Woss instances and give Time value; @see importCustomSSP
    *  <li><b>setCustomBathymetry &lt;<i>tx latitude [decimal degrees]</i>&gt; &lt;<i>tx longitude [decimal degrees]</i>&gt; &lt;<i>tx bearing [decimal degrees]</i>&gt;
    *                             &lt;<i>total range-depth values</i>&gt; &lt;<i>range [m]</i>&gt; &lt;<i>depth [m]</i>&gt; <i>...</i> </b>: 
    *     imports on the fly and sets CustomBathymetry for given transmitter coordinates and bearing; If <i>total range-depth values</i>
    *     is equal to 0, then next token is interpreted as path to a CustomBathymetry file to be imported; @see importCustomBathymetry
    *  <li><b>closeAllConnections &lt; &gt;</b>: 
    *     closes all database connections
    * </ul>
    * @note an invalid bearing (e.g. bearing < -360.0 or > 360.0 ) will be interpreted as a special value to represent 
    * <b>all bearings</b>. An invalid range (e.g. range < 0.0 ) will be interpreted as a special case value to represent
    * <b>all ranges</b>. 
    * 
    * @param argc number of arguments in <i>argv</i>
    * @param argv array of strings which are the comand parameters (Note that argv[0] is the name of the object)
    * 
    * @return TCL_OK or TCL_ERROR whether the command has been dispatched succesfully or not
    * 
    **/
    virtual int command( int argc, const char*const* argv );
    
    
    protected:
    
    
    double debug_;
        
		
	};
  

}

#endif // WOSS_NS_MIRACLE_SUPPORT

#endif /* WOSS_DB_MANAGER_IMPLEMENT_TCL_H */

 
 
