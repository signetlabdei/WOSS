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
 * @file   uw-woss-position.h
 * @author Federico Guerra
 *
 * \brief  Provides the interface for WossWpPosition class 
 *
 * Provides the interface for WossWpPosition class
 */


#ifndef UNDERWATER_WOSS_POSITION_H
#define UNDERWATER_WOSS_POSITION_H


#include <location-definitions.h>
#include <node-core.h>


class WossPosition : public Position, public woss::Location {

  
  public:
    
    
  WossPosition( double latitude, double longitude, double depth = 0, double dist = LOCATION_COMPARISON_DISTANCE );

  WossPosition( const woss::CoordZ& coordz = woss::CoordZ(), double dist = LOCATION_COMPARISON_DISTANCE );
  
  
  virtual ~WossPosition() {  }
  
  
  virtual WossPosition* create( double latitude, double longitude, double depth = 0, double dist = LOCATION_COMPARISON_DISTANCE ) const { 
    return new WossPosition(latitude, longitude, depth); }
  
  virtual WossPosition* create( const woss::CoordZ& coordz = woss::CoordZ(), double dist = LOCATION_COMPARISON_DISTANCE ) const { 
    return new WossPosition( coordz, dist ); }

  virtual WossPosition* clone() const { return new WossPosition(*this); } 

  
  virtual int command(int argc, const char*const* argv);  
   
  
  virtual void setLatitude( double val ); 
  
  virtual void setLongitude( double val );
  
  virtual void setDepth( double val );
  
  virtual void setAltitude( double val );
  
  
  virtual void setMinVerticalOrientation( double val );
  
  virtual void setMaxVerticalOrientation( double val );
  
  
  virtual void setX( double val ); 
  
  virtual void setY( double val ); 
  
  virtual void setZ( double val ); 
  
  
  virtual double getX(); 
  
  virtual double getY(); 

  virtual double getZ();
  
  
  virtual double getLatitude();
  
  virtual double getLongitude();
  
  virtual double getDepth();
  
  virtual double getAltitude();
  
  
  virtual double getMinVerticalOrientation();
  
  virtual double getMaxVerticalOrientation();
  
  
  protected:
    
 
  double min_vertical_orientation;
  
  double max_vertical_orientation;
  

};


#endif // UNDERWATER_WOSS_POSITION_H


