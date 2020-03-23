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
 * \brief  Definitions and library for woss::Location class 
 *
 * Definitions and library for woss::Location class
 */


#ifndef LOCATION_DEFINITIONS_H
#define LOCATION_DEFINITIONS_H 


#include <cmath>
#include "coordinates-definitions.h"
#include <iostream>


namespace woss {
  
  
  #define LOCATION_COMPARISON_DISTANCE (0.0)
  
  
  /**
  * \brief Class that stores the coordinates of moving entity
  *
  * The woss::Location class has the task of storing the geographical coordinates of a moving entity
  **/
  class Location {
  
  
    public:
      

    /**
    * Location default constructor
    * @param coordz coordinates
    * @param dist distance comparison precision [m]
    **/      
    Location( const CoordZ& coordz = CoordZ(), double dist = LOCATION_COMPARISON_DISTANCE );
      
    /**
    * Location constructor
    * @param latitude latitude value [decimal degrees]
    * @param longitude longitude value [decimal degrees]
    * @param depth depth value [m]
    * @param dist distance comparison precision [m]
    **/       
    Location( double latitude, double longitude, double depth = 0, double dist = LOCATION_COMPARISON_DISTANCE );
    
    /**
    * Location destructor
    **/    
    virtual ~Location() { }
    
    /**
    * Location virtual factory method
    * @param latitude latitude value [decimal degrees]
    * @param longitude longitude value [decimal degrees]
    * @param depth depth value [m]
    * @param dist distance comparison precision [m]
    * @return a heap-allocated Location object
    **/ 
    virtual Location* create( double latitude, double longitude, double depth = 0, double dist = LOCATION_COMPARISON_DISTANCE ) const { 
      return new Location(latitude, longitude, depth, dist); }

    /**
    * Location virtual factory method
    * @param coordz coordinates
    * @param dist distance comparison precision [m]
    * @return a heap-allocated Location object
    **/ 
    virtual Location* create( const CoordZ& coordz = CoordZ(), double dist = LOCATION_COMPARISON_DISTANCE ) const { 
      return new Location(coordz,dist); }
    
    /**
    * Location virtual factory method
    * @return a heap-allocated copy of <b>this</b> instance
    **/
    virtual Location* clone() const { return new Location(*this); }


    /**
    * Checks the validity of Location
    * @return <i>true</i> if curr_coordz is valid, <i>false</i> otherwise
    **/
    virtual bool isValid() const { return curr_coordz.isValid(); }
    

    /**
    * Checks if the woss::CoordZ given is equivalent to this Location
    * @param coordz valid woss::CoordZ to check
    * @return <i>true</i> if assumption is valid, <i>false</i> otherwise
    **/
    virtual bool isEquivalentTo( const woss::CoordZ& coordz ); 
    
  
    /**
    * Sets initial coordinates
    * @param coordz valid woss::CoordZ 
    **/
    virtual void setLocation( const CoordZ& coordz );
    
    /**
    * Sets initial latitude
    * @param lat valid latitude [decimal degrees]
    **/    
    virtual void setLatitude( double lat ); 
  
    /**
    * Sets initial longitude
    * @param lat valid longitude [decimal degrees]
    **/  
    virtual void setLongitude( double lon );

    /**
    * Sets initial depth
    * @param lat valid depth [m]
    **/
    virtual void setDepth( double depth );
    
    
    /**
    * Sets current vertical orientation from reference line (0 degrees = parallel to sea surface / bottom ).
    * Negative values are towards the surface, while positive ones are towards sea bottom
    * @param angle delta angle [dec degrees]
    **/   
    virtual void setVerticalOrientation( double angle );
    
    /**
    * Gets current horizontal orientation from reference line (0 degrees = parallel to direction of current movement )
    * positive values are counter-clockwise rotations, while negative ones are clockwise rotations
    * @param angle delta angle [dec degrees]
    **/     
    virtual void setHorizontalOrientation( double angle );
    
    
    /**
    * Gets current coordinates
    * @returns valid woss::CoordZ 
    **/    
    virtual CoordZ getLocation(); 
 
    /**
    * Gets current latitude
    * @returns valid latitude [decimal degrees]
    **/     
    virtual double getLatitude(); 
    
    /**
    * Gets current longitude
    * @returns valid longitude [decimal degrees]
    **/        
    virtual double getLongitude(); 

    /**
    * Gets current depth
    * @returns valid depth [m]
    **/    
    virtual double getDepth(); 


    /**
    * Gets current cartesian x-axis value
    * @returns x value
    **/    
    virtual double getX(); 
    
    /**
    * Gets current cartesian y-axis value
    * @returns y value
    **/        
    virtual double getY(); 
    
    /**
    * Gets current cartesian z-axis value
    * @returns z value
    **/    
    virtual double getZ();
    
    /**
    * Gets current vertical orientation from reference line (0 degrees = parallel to sea surface / bottom ).
    * Negative values are towards the surface, while positive ones are towards sea bottom
    * @returns difference angle [dec degrees]
    **/   
    virtual double getVerticalOrientation();
    
    /**
    * Gets current horizontal orientation from reference line (0 degrees = parallel to direction of current movement )
    * positive values are counter-clockwise rotations, while negative ones are clockwise rotations
    * @returns difference angle [dec degrees]
    **/     
    virtual double getHorizontalOrientation();
  
    /**
    * Gets current bearing in [-pi,pi]
    * @returns bearing [dec degrees]
    **/         
    virtual double getBearing();
    
    
    /**
    * << operator
    * @param os left operand ostream reference
    * @param instance right operand const Location reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend ::std::ostream& operator<<( ::std::ostream& os, const Location& instance ) {
      os << "Location, latitude = " << instance.curr_coordz.getLatitude() << "; longitude = " << instance.curr_coordz.getLongitude()
         << "; depth = " << instance.curr_coordz.getDepth();
      return os;
    }

  
    protected:
      
    
    /**
    * Comparison distance [m]
    **/      
    double comparison_distance;
    
    
    /**
    * current coordinates
    **/    
    CoordZ curr_coordz;
    
    
    /**
    * current vertical orientation. It's the delta angle from reference direction 
    * (0 degrees = parallel to sea surface / bottom )
    **/      
    double vertical_orientation;
 
    /**
    * current horizontal orientation. It's the delta angle from reference direction 
    * (0 degrees = parallel to movement's direction )
    **/        
    double horizontal_orientation;

    /**
    * current bearing
    **/      
    double bearing;
    
    
  };
  
  
}


#endif /* LOCATION_DEFINITIONS_H */

