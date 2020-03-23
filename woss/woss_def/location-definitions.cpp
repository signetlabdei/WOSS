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
 * @file   location-definitions.h
 * @author Federico Guerra
 *
 * \brief  Implementation of woss::Location class 
 *
 * Implementation of woss::Location class
 */


#include "location-definitions.h"


using namespace woss;

     
Location::Location( const CoordZ& coordz, double dist ) 
: comparison_distance(dist),
  curr_coordz(coordz),
  vertical_orientation(0.0),
  horizontal_orientation(0.0),
  bearing(0.0)
{ 
}
  

Location::Location( double latitude, double longitude, double depth, double dist ) 
: comparison_distance(dist),
  curr_coordz(latitude,longitude,depth)
{ 
}


bool Location::isEquivalentTo( const woss::CoordZ& coordz ) { 
  if ( !coordz.isValid() ) return false;
  // force curr_coordz update by calling getLocation() !!
  return getLocation().getCartDistance(coordz) <= comparison_distance; 
}


void Location::setLocation( const CoordZ& coordz ) { 
  curr_coordz = coordz; 
}

   
void Location::setLatitude( double lat ) { 
  curr_coordz.setLatitude(lat); 
}

 
void Location::setLongitude( double lon ) { 
  curr_coordz.setLongitude(lon); 
}


void Location::setDepth( double depth ) { 
  curr_coordz.setDepth(::std::abs(depth)); 
}


void Location::setVerticalOrientation( double angle ) {
  vertical_orientation = angle;
}


void Location::setHorizontalOrientation( double angle ) { 
  horizontal_orientation = angle;
}

    
CoordZ Location::getLocation() { 
  return curr_coordz; 
}

    
double Location::getLatitude() { 
// force curr_coordz update by calling getLocation() !!
  return getLocation().getLatitude(); 
}

       
double Location::getLongitude() { 
// force curr_coordz update by calling getLocation() !!
  return getLocation().getLongitude(); 
}

   
double Location::getDepth() { 
// force curr_coordz update by calling getLocation() !!
  return getLocation().getDepth(); 
}

  
double Location::getX() { 
// force curr_coordz update by calling getLocation() !!
  return getLocation().getCartX(); 
}

       
double Location::getY() { 
// force curr_coordz update by calling getLocation() !!
  return getLocation().getCartY(); 
}

   
double Location::getZ() { 
// force curr_coordz update by calling getLocation() !!
  return getLocation().getCartZ(); 
}


double Location::getVerticalOrientation() {
  return vertical_orientation;
}


double Location::getHorizontalOrientation() { 
  return horizontal_orientation;
}


double Location::getBearing() { 
  return bearing;
}


