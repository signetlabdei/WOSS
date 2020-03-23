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
 * @file   woss-utilities-tcl.cpp
 * @author Federico Guerra
 *
 * \brief  Implementation of woss::WossUtilitiesTcl class 
 *
 * Implementation of woss::WossUtilitiesTcl class 
 */


#ifdef WOSS_NS_MIRACLE_SUPPORT


#include <cstdlib>
#include <cmath>
#include <coordinates-definitions.h>
#include "woss-utilities-tcl.h"


using namespace woss;


static class WossUtilitiesClass : public TclClass {
public:
  WossUtilitiesClass() : TclClass("WOSS/Utilities") {}
  TclObject* create(int, const char*const*) {
    return (new WossUtilitiesTcl());
  }
} class_WossUtilities;


WossUtilitiesTcl::WossUtilitiesTcl() 
: debug(0)
{
  bind("debug", &debug); 
}


int WossUtilitiesTcl::command(int argc, const char*const* argv) {
  Tcl& tcl = Tcl::instance();

  
  if (argc == 8) {
    if(strcasecmp(argv[1], "getCartDistance") == 0) { // lat long lat long depth       
      double dist = CoordZ(atof(argv[2]), atof(argv[3]), ::std::abs(atof(argv[4]))).getCartDistance( CoordZ(atof(argv[5]), atof(argv[6]), ::std::abs(atof(argv[7]))) );

      if (debug) std::cout << "WossUtilitiesTcl::command() getCartDistance called, tx latitude = " 
                            << atof(argv[2]) << "; tx longitude = " << atof(argv[3]) << "; tx depth = " 
                            << atof(argv[4]) << "; rx latitude = " << atof(argv[5]) << "; rx longitude = " << atof(argv[6])
                            << "; depth = " << atof(argv[7]) << std::endl;

      tcl.resultf("%.17g", dist );
      return TCL_OK;
    }
  
  }
  else if (argc == 7) {
    if(strcasecmp(argv[1], "getGreatCircleDistance") == 0) { // lat long lat long depth       
      double dist = Coord(atof(argv[2]), atof(argv[3])).getGreatCircleDistance( Coord(atof(argv[4]), atof(argv[5])), atof(argv[6]) );

      if (debug) std::cout << "WossUtilitiesTcl::command() getGreatCircleDistance called, tx latitude = " 
                            << atof(argv[2]) << "; tx longitude = " << atof(argv[3]) << "; rx latitude = " 
                            << atof(argv[4]) << "; rx longitude = " << atof(argv[5]) << "; depth = " << atof(argv[6])
                            << "; distance " << dist << std::endl;

      tcl.resultf("%.17g", dist );
      return TCL_OK;
    }
  
  }
  else if (argc == 6) {

      if(strcasecmp(argv[1], "getLatfromDistBearing") == 0) {  // lat long bearing (decimal degrees) distance (m)

        double bearing = atof(argv[4]) * M_PI / 180.0;
        double distance = atof(argv[5]);

        Coord coord = Coord::getCoordFromBearing( Coord(atof(argv[2]), atof(argv[3])) , bearing , distance );

        if (debug) std::cout << "WossUtilitiesTcl::command() getLatfromDistBearing called, tx latitude = " 
                             << atof(argv[2]) << "; tx longitude = " << atof(argv[3]) << "; bearing = " << (bearing * 180.0 / M_PI) << "; distance = " 
                             << distance << "; latitude = " << coord.getLatitude() << std::endl;

        tcl.resultf("%.17g",(double) coord.getLatitude());
        return TCL_OK;
      }
      else if(strcasecmp(argv[1], "getLongfromDistBearing") == 0) { // lat long bearing (decimal degrees) distance (m)

        double bearing = atof(argv[4]) * M_PI / 180.0;
        double distance = atof(argv[5]);
        
        Coord coord = Coord::getCoordFromBearing( Coord(atof(argv[2]), atof(argv[3])) , bearing , distance );

        if (debug) std::cout << "WossUtilitiesTcl::command() getLongfromDistBearing called, tx latitude = " 
                             << atof(argv[2]) << "; tx longitude = " << atof(argv[3]) << "; bearing = " << (bearing * 180.0 / M_PI) << "; distance = " 
                             << distance << "; latitude = " << coord.getLongitude() << std::endl;

        tcl.resultf("%.17g", (double) coord.getLongitude());
        return TCL_OK;
      }
      if(strcasecmp(argv[1], "getLatfromUTMCoords") == 0) {  // lat long bearing (decimal degrees) distance (m)

        double easting = atof(argv[2]);
        double northing = atof(argv[3]);
        double utm_zone = atof(argv[4]);
        char utm_zone_char = argv[5][0];
        
        Coord coord = Coord::getCoordFromUtmWgs84( easting , northing, utm_zone, utm_zone_char );

        if (debug) std::cout << "WossUtilitiesTcl::command() getLatfromUTMCoords called, tx latitude = " 
                             << atof(argv[2]) << "; easting = " << easting << "; northing = " << northing 
                             << "; utm zone = " << utm_zone << "; utm zone char = " << utm_zone_char 
                             << "; latitude = " << coord.getLatitude() << std::endl;

        tcl.resultf("%.17g",(double) coord.getLatitude());
        return TCL_OK;
      }
      else if(strcasecmp(argv[1], "getLongfromUTMCoords") == 0) { // lat long bearing (decimal degrees) distance (m)

        double easting = atof(argv[2]);
        double northing = atof(argv[3]);
        double utm_zone = atof(argv[4]);
        char utm_zone_char = argv[5][0];
        
        Coord coord = Coord::getCoordFromUtmWgs84( easting , northing, utm_zone, utm_zone_char );

        if (debug) std::cout << "WossUtilitiesTcl::command() getLongfromUTMCoords called, tx latitude = " 
                             << atof(argv[2]) << "; easting = " << easting << "; northing = " << northing 
                             << "; utm zone = " << utm_zone << "; utm zone char = " << utm_zone_char 
                             << "; longitude = " << coord.getLongitude() << std::endl;

        tcl.resultf("%.17g",(double) coord.getLongitude());
        return TCL_OK;
      }      
      else if(strcasecmp(argv[1], "getGreatCircleDistance") == 0) { // lat long lat long     
        double dist = Coord(atof(argv[2]), atof(argv[3])).getGreatCircleDistance( Coord(atof(argv[4]), atof(argv[5])) );

        if (debug) std::cout << "WossUtilitiesTcl::command() getGreatCircleDistance called, tx latitude = " 
                             << atof(argv[2]) << "; tx longitude = " << atof(argv[3]) << "; rx latitude = " 
                             << atof(argv[4]) << "; rx longitude = " << atof(argv[5])  
                             << "; distance " << dist << std::endl;

        tcl.resultf("%.17g", dist );
        return TCL_OK;
      }
      else if(strcasecmp(argv[1], "getDegBearing") == 0) { // lat long lat long     
        double bearing = Coord(atof(argv[2]), atof(argv[3])).getInitialBearing( Coord(atof(argv[4]), atof(argv[5])) ) * M_PI / 180.0;

        if (debug) std::cout << "WossUtilitiesTcl::command() getDecBearing called, tx latitude = " 
                             << atof(argv[2]) << "; tx longitude = " << atof(argv[3]) << "; rx latitude = " 
                             << atof(argv[4]) << "; rx longitude = " << atof(argv[5])  
                             << "; degrees bearing  " << bearing << std::endl;

        tcl.resultf("%.17g", bearing );
        return TCL_OK;
      }      
  }
  else if ( argc == 3 ) {
      if(strcasecmp(argv[1], "convertMonth") == 0) {  // month number
        ::std::string month = convertMonth( atoi(argv[2]) );
        tcl.resultf( "%s", month.c_str() );
        return TCL_OK;
      }
  
  }
  
  return TCL_ERROR;
}


::std::string WossUtilitiesTcl::convertMonth( int month ) {
    switch ( month ) {
      case(1): { return "January"; }
      break;
      
      case(2): { return "February"; }
      break;

      case(3): { return "March"; }
      break;

      case(4): { return "April"; }
      break;

      case(5): { return "May"; }
      break;

      case(6): { return "June"; }
      break;

      case(7): { return "July"; }
      break;

      case(8): { return "August"; }
      break;

      case(9): { return "September"; }
      break;

      case(10): { return "October"; }
      break;

      case(11): { return "November"; }
      break;

      case(12): { return "December"; }
      break;

      case(13): { return "Jan-Mar"; }
      break;

      case(14): { return "Apr-Jun"; }
      break;

      case(15): { return "Jul-Sep"; }
      break;

      case(16): { return "Oct-Dec"; }
      break;

      default: { return "Annual"; }
      break;
  }
}


#endif // WOSS_NS_MIRACLE_SUPPORT

