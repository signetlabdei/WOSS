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
 * @file   uw-woss-position.cpp
 * @author Federico Guerra
 *
 * \brief  Provides the implementation of WossPosition class 
 *
 * Provides the implementation of WossPosition class 
 */


#include "uw-woss-position.h"


static class WossPositionClass : public TclClass {
public:
  WossPositionClass() : TclClass("WOSS/Position") {}
  TclObject* create(int, const char*const*) {
    return (new WossPosition);
  }
} class_WossPosition;


WossPosition::WossPosition( double latitude, double longitude, double depth, double dist ) 
: Location( latitude, longitude, depth, dist ),
  min_vertical_orientation(-45.0),
  max_vertical_orientation(45.0)
{ 
  latitude_ = latitude;
  longitude_ = longitude;
  altitude_ = -depth;
  x_ = curr_coordz.getCartX();
  y_ = curr_coordz.getCartY();
  z_ = curr_coordz.getCartZ();
  
  bind("compDistance_", &comparison_distance );
  bind("verticalOrientation_", &vertical_orientation);
  bind("minVerticalOrientation_", &min_vertical_orientation);
  bind("maxVerticalOrientation_", &max_vertical_orientation);
}


WossPosition::WossPosition( const woss::CoordZ& coordz, double dist ) 
: Location( coordz, dist ),
  min_vertical_orientation(-45.0),
  max_vertical_orientation(45.0)
{ 
  latitude_ = coordz.getLatitude();
  longitude_ = coordz.getLongitude();
  altitude_ = - coordz.getDepth();
  x_ = coordz.getCartX();
  y_ = coordz.getCartY();
  z_ = coordz.getCartZ();
  
  
  bind("compDistance_", &comparison_distance );
  bind("verticalOrientation_", &vertical_orientation);
  bind("minVerticalOrientation_", &min_vertical_orientation);
  bind("maxVerticalOrientation_", &max_vertical_orientation);
}


void WossPosition::setLatitude( double val ) { 
  woss::Location::setLatitude(val); 
  latitude_ = val; 
}


void WossPosition::setLongitude( double val ) { 
  woss::Location::setLongitude(val); 
  longitude_ = val; 
}


void WossPosition::setDepth( double val ) { 
  woss::Location::setDepth(val); 
  altitude_ = -1.0 * val; 
}


void WossPosition::setAltitude( double val ) { 
  woss::Location::setDepth( ::std::abs(val) ); 
  altitude_ = val; 
}


void WossPosition::setX( double val ) { 
  x_ = val; 
  ::std::cerr << "WossPosition::setX() WARNING this function is not supported." << ::std::endl;
}


void WossPosition::setY( double val ) { 
  y_ = val; 
  ::std::cerr << "WossPosition::setY() WARNING this function is not supported." << ::std::endl; 
}


void WossPosition::setZ( double val ) { 
  z_ = val; 
  ::std::cerr << "WossPosition::setZ() WARNING this function is not supported." << ::std::endl; 
}
 
 
void WossPosition::setMinVerticalOrientation( double val ) {
  min_vertical_orientation = val;
}
  

void WossPosition::setMaxVerticalOrientation( double val ) { 
  max_vertical_orientation = val;
}
  
  
double WossPosition::getX() { 
  return woss::Location::getX(); 
}


double WossPosition::getY() { 
  return woss::Location::getY(); 
}


double WossPosition::getZ() { 
  return woss::Location::getZ(); 
}


double WossPosition::getLatitude() { 
  return woss::Location::getLatitude(); 
}


double WossPosition::getLongitude() { 
  return woss::Location::getLongitude(); 
}


double WossPosition::getDepth()  { 
  return woss::Location::getDepth(); 
}


double WossPosition::getAltitude() { 
  return -1.0 * woss::Location::getDepth(); 
}  
  

double WossPosition::getMinVerticalOrientation() {
  return min_vertical_orientation;
}
  

double WossPosition::getMaxVerticalOrientation() { 
  return max_vertical_orientation;
}
  

int WossPosition::command(int argc, const char*const* argv) {
  Tcl& tcl = Tcl::instance();
  if (argc == 3) {
    if( strcasecmp(argv[1], "setDepth_") == 0 ) {
//       ::std::cout << NOW << "  WossWpPosition::command() addLoopPoint lat = " << argv[2] << "; long " << argv[3] 
//                   << "; depth = " << argv[4] << "; speed = " << argv[5] << "; time to wait = " << argv[6]
//                   << "; loop to waypoint = " << argv[7] << "; total loops = " << argv[8] << ::std::endl;

      setDepth( atof(argv[2]) ); 
      
      setAltitude( -1.0 * atof(argv[2]) );
      
      return TCL_OK;
    }
  }
  else if (argc == 2) {
    if( strcasecmp(argv[1], "getDepth_") == 0 ) {
      tcl.resultf("%f", getDepth());           
      return TCL_OK;
    }
  }
  return Position::command(argc, argv);
}
