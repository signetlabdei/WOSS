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


 
#ifdef WOSS_NS_MIRACLE_SUPPORT

#include <iostream>
#include "bathymetry-utm-csv-db-creator-tcl.h"

using namespace woss;

static class BathyUtmCsvDbCreatorClass : public TclClass {
public:
  BathyUtmCsvDbCreatorClass() : TclClass("WOSS/Creator/Database/Textual/Bathymetry/UMT_CSV") {}
  TclObject* create(int, const char*const*) {
    return( new BathyUtmCsvDbCreatorTcl() );
  }
} class_BathyUtmCsvDbCreatorClass;


BathyUtmCsvDbCreatorTcl::BathyUtmCsvDbCreatorTcl()
: BathyUtmCsvDbCreator()
{
  bind("debug", &debug_);
  bind("woss_db_debug", &woss_db_debug_);
  bind("db_spacing", &db_spacing);
  bind("total_northing_values", &total_northing_values);
  bind("total_easting_values", &total_easting_values);
  bind("range_easting_start", &range_easting_start);
  bind("range_easting_end", &range_easting_end);
  bind("range_northing_start", &range_northing_start);
  bind("range_northing_end", &range_northing_end);
  bind("approx_land_to_sea_surface", &approx_land_to_sea_surface_);

  debug = (bool) debug_;
  woss_db_debug = (bool) woss_db_debug_;
  approx_land_to_sea_surface = (bool) approx_land_to_sea_surface_;
}


int BathyUtmCsvDbCreatorTcl::command(int argc, const char*const* argv) {
  if ( argc == 3 ) {
    if(strcasecmp(argv[1], "setDbPathName") == 0) {

      if (debug) ::std::cout << "BathyUtmCsvDbCreatorTcl::command() setDbPathName " << argv[2] << " called"  << ::std::endl;

      pathname = argv[2];

      return TCL_OK;
    }
    else if(strcasecmp(argv[1], "setCSVSeparator") == 0) {

      if (debug) ::std::cout << "BathyUtmCsvDbCreatorTcl::command() setCSVSeparator " << argv[2] << " called"  << ::std::endl;

      separator = argv[2][0];

      return TCL_OK;
    }
  }
  return TclObject::command(argc,argv);
}

#endif // WOSS_NS_MIRACLE_SUPPORT
