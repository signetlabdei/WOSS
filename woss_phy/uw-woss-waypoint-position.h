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
 * @file   uw-woss-waypoint-position.h
 * @author Federico Guerra
 *
 * \brief  Provides the interface for WossWpPosition class 
 *
 * Provides the interface for WossWpPosition class
 */


#ifndef UNDERWATER_WOSS_WAYPOINT_POSITION_H
#define UNDERWATER_WOSS_WAYPOINT_POSITION_H


#include <map>
#include <utility>
#include <climits>
#include "uw-woss-position.h"


class WossWpPosition : public WossPosition {

  
  public:
    
    
  WossWpPosition();

  virtual ~WossWpPosition() { }
  
  
  virtual int command(int argc, const char*const* argv);  
  
  
  virtual bool isEquivalentTo( const woss::CoordZ& coordz );
  
  
  virtual woss::CoordZ getLocation();

  
  virtual double getVerticalOrientation();
  
  virtual double getBearing();
  
  
  protected:
  
  
  class WayPoint {
    
    
    public:
    
      
    WayPoint( const woss::CoordZ& destination, double speed = 0.0, double time = HUGE_VAL, int loop_id = INT_MAX, int total_loops = 0 );
    
    
    void setDestination( const woss::CoordZ& dest ) { destination = dest; }
   
    void setSpeed( double s ) { speed = s; }

    void setTimeToWait( double t ) { time_to_wait = t; }
    
    void setLoopId( int id ) { loop_to_waypoint_id = id; }
    
    void setTotalLoops( int loops ) { total_loops = loops; }
    
    
    const woss::CoordZ& getDestination() const { return destination; }  
       
    // speed used to reach this node
    double getSpeed() const { return speed; }
        
    double getTimeToWait() const { return time_to_wait; }
    
    int getLoopId() const { return loop_to_waypoint_id; }
    
    int getTotalLoops() const { return total_loops; }


    bool isValid() const { return( destination.isValid() && !( speed == 0.0 && time_to_wait == 0.0 ) ); }

    bool hasToLoop() const { return( loop_to_waypoint_id != INT_MAX && total_loops != 0.0 ); }

    bool hasToWait() const { return( speed == 0.0 && time_to_wait > 0 && time_to_wait != HUGE_VAL ); }
    
    bool hasToStop() const { return( speed == 0.0 && time_to_wait == HUGE_VAL ); } 


    virtual double getTimeOfArrival( const WayPoint& dest_waypoint ) const;
    
    virtual woss::CoordZ getCurrentPosition( const WayPoint& dest_waypoint, double time_elapsed ) const;


    friend std::ostream& operator<<( std::ostream& os, const WayPoint& instance ) {
      os << instance.destination << "; speed = " << instance.speed << "; time to wait = " << instance.time_to_wait
         << "; loop id = " << instance.loop_to_waypoint_id <<"; total loops = " << instance.total_loops;
      return os;
    }
    
    protected:
  
    
    woss::CoordZ destination;
        
    double speed;
    
    double time_to_wait;

    int loop_to_waypoint_id;

    int total_loops;
    
    
  };
   
  typedef ::std::vector< WayPoint > WayPointVect;
  
  typedef ::std::map< double, int > TimeIdMap;
  typedef TimeIdMap::iterator TIMIter;
  typedef TimeIdMap::reverse_iterator TIMRIter;
  
  
  double time_threshold;
  
  double last_time_update;
  
  
  WayPointVect waypoint_vect;
  
  TimeIdMap timeid_map;
  
  
  virtual void update( double now );
  
  virtual void updateVerticalOrientation( const woss::CoordZ& prev, const woss::CoordZ& curr );
  
  virtual void updateBearing( const woss::CoordZ& prev, const woss::CoordZ& curr );
  
  
  virtual double addWayPoint( const WayPoint& waypoint );
  
  virtual double addLoopPoint( const WayPoint& waypoint );
  
  
};


#endif // UNDERWATER_WOSS_WAYPOINT_POSITION_H

