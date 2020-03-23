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
 * @file   uw-woss-waypoint-position.cpp
 * @author Federico Guerra
 *
 * \brief  Provides the implementation of WossWpPosition class 
 *
 * Provides the implementation of WossWpPosition class
 */


#include <cmath>
#include <cassert>
#include "uw-woss-waypoint-position.h"


WossWpPosition::WayPoint::WayPoint( const woss::CoordZ& dest, double velocity, double time, int loop_id, int total_loops )
: destination(dest),
  speed(velocity),
  time_to_wait(time),
  loop_to_waypoint_id(loop_id),
  total_loops(total_loops)
{

}


double WossWpPosition::WayPoint::getTimeOfArrival( const WayPoint& waypoint ) const { 
  if ( destination == waypoint.destination ) return time_to_wait;

  double distance = destination.getGreatCircleDistance( waypoint.getDestination() );
  double start_depth = destination.getDepth();
  double end_depth = waypoint.destination.getDepth();
  
  if ( distance == 0.0 && start_depth != end_depth ) distance = ::std::abs(end_depth - start_depth);
  
//   ::std::cout << "WossWpPosition::WayPoint::getTimeOfArrival() start wp = " << *this << ::std::endl 
//               << "WossWpPosition::WayPoint::getTimeOfArrival() end wp = " << waypoint << ::std::endl 
//               << "WossWpPosition::WayPoint::getTimeOfArrival() distance = " << distance;
              
  if ( waypoint.speed == 0 && speed == 0) {
    ::std::cerr << "WossWpPosition::WayPoint::getTimeOfArrival() ERROR, destination waypoint has zero speed. Time of " 
                << " arrival is INFINITY" << ::std::endl;
  
    return HUGE_VAL;
  }
  
//   ::std::cout << "; toa = " << ( waypoint.speed <= 0 ? (distance / waypoint.speed + time_to_wait) : (distance / speed + time_to_wait) ) 
//               << ::std::endl;

  if ( waypoint.speed <= 0 ) return distance / speed + time_to_wait;
  return( distance / waypoint.speed + time_to_wait );
}


woss::CoordZ WossWpPosition::WayPoint::getCurrentPosition( const WayPoint& dest_waypoint, double time_elapsed ) const {
  if ( time_elapsed <= time_to_wait ) return destination;
  if ( destination == dest_waypoint.destination ) return dest_waypoint.destination;
  
  double distance = (time_elapsed - time_to_wait) * speed;
  
//   ::std::cout << NOW <<"  WossWpPosition::WayPoint::getCurrentPosition() start wp = " << *this << ::std::endl 
//               << "WossWpPosition::WayPoint::getCurrentPosition() end wp = " << dest_waypoint << ::std::endl
//               << "WossWpPosition::WayPoint::getCurrentPosition() time elapsed = " << time_elapsed << "; distance = " << distance << ::std::endl;
              
  return( woss::CoordZ::getCoordZAlongGreatCircle(destination, dest_waypoint.destination, distance ) );
}


static class WossWpPositionClass : public TclClass {
public:
  WossWpPositionClass() : TclClass("WOSS/Position/WayPoint") {}
  TclObject* create(int, const char*const*) {
    return (new WossWpPosition);
  }
} class_WossWpPosition;



WossWpPosition::WossWpPosition()
: time_threshold(1e-5),
  last_time_update(0.0),
  waypoint_vect(),
  timeid_map()
{
  bind("time_threshold_",&time_threshold);
}


int WossWpPosition::command(int argc, const char*const* argv) {
  Tcl& tcl = Tcl::instance();
  if (argc == 9) {
    if( strcasecmp(argv[1], "addLoopPoint") == 0 ) {
//       ::std::cout << NOW << "  WossWpPosition::command() addLoopPoint lat = " << argv[2] << "; long " << argv[3] 
//                   << "; depth = " << argv[4] << "; speed = " << argv[5] << "; time to wait = " << argv[6]
//                   << "; loop to waypoint = " << argv[7] << "; total loops = " << argv[8] << ::std::endl;

      double toa = addLoopPoint( WayPoint( woss::CoordZ( atof(argv[2]), atof(argv[3]), ::std::abs(atof(argv[4])) ), atof(argv[5]), atof(argv[6]), atoi(argv[7]), atoi(argv[8]) ) );
      tcl.resultf("%f", toa);

      return TCL_OK;
    }
  }
  else if (argc == 7) {
    if( strcasecmp(argv[1], "addWayPoint") == 0 ) {
//       ::std::cout << NOW << "  WossWpPosition::command() addWaitPoint lat = " << argv[2] << "; long " << argv[3] 
//                   << "; depth = " << ::std::abs(atof(argv[4])) << "; speed = " << argv[5] <<"; time to wait = " << argv[6] << ::std::endl;
     
      double toa = addWayPoint( WayPoint( woss::CoordZ( atof(argv[2]), atof(argv[3]), ::std::abs(atof(argv[4])) ), atof(argv[5]), atof(argv[6]) ) );
      tcl.resultf("%f", toa);
      
      return TCL_OK;
    }
    if( strcasecmp(argv[1], "addStartWayPoint") == 0 ) {
//       ::std::cout << NOW << "  WossWpPosition::command() addStartWayPoint lat = " << argv[2] << "; long " << argv[3] 
//                   << "; depth = " << ::std::abs(atof(argv[4])) << "; speed = " << argv[5] <<"; time to wait = " << argv[6] << ::std::endl;
     
      latitude_ = atof(argv[2]);
      longitude_ = atof(argv[3]);
      altitude_ = atof(argv[4]);
      
      setLocation( woss::CoordZ(latitude_, longitude_, ::std::abs(altitude_) ) );
           
      addWayPoint( WayPoint( woss::CoordZ( latitude_, longitude_, ::std::abs(altitude_) ), atof(argv[5]), atof(argv[6]) ) );
      
      return TCL_OK;
    }
  }
  else if (argc == 3) {
    if( strcasecmp(argv[1], "getWaypointLatitude_") == 0 ) {
      int index = atoi(argv[2]);
      assert( index >= 0 && index < (int)waypoint_vect.size() );
           
      tcl.resultf("%f", waypoint_vect[index].getDestination().getLatitude());
      return TCL_OK;
    }
    else if( strcasecmp(argv[1], "getWaypointLongitude_") == 0 ) {
      int index = atoi(argv[2]);
      assert( index >= 0 && index < (int)waypoint_vect.size() );
           
      tcl.resultf("%f", waypoint_vect[index].getDestination().getLongitude());
      return TCL_OK;
    }
    else if( strcasecmp(argv[1], "getWaypointAltitude_") == 0 ) {
      int index = atoi(argv[2]);
      assert( index >= 0 && index < (int)waypoint_vect.size() );
           
      tcl.resultf("%f", (-1.0 * waypoint_vect[index].getDestination().getDepth() ) );
      return TCL_OK;
    }
    else if( strcasecmp(argv[1], "getWaypointDepth_") == 0 ) {
      int index = atoi(argv[2]);
      assert( index >= 0 && index < (int)waypoint_vect.size() );
           
      tcl.resultf("%f", waypoint_vect[index].getDestination().getDepth());
      return TCL_OK;
    }
  }
 else if (argc == 2) {
    if( strcasecmp(argv[1], "getWpLastLatitude_") == 0 ) {           
      tcl.resultf("%f", waypoint_vect.rbegin()->getDestination().getLatitude());
      return TCL_OK;
    }
    else if( strcasecmp(argv[1], "getWpLastLongitude_") == 0 ) {
      tcl.resultf("%f", waypoint_vect.rbegin()->getDestination().getLongitude());
      return TCL_OK;
    }
    else if( strcasecmp(argv[1], "getWpLastAltitude_") == 0 ) {
      tcl.resultf("%f", ( -1.0 * waypoint_vect.rbegin()->getDestination().getDepth() ) );
      return TCL_OK;
    }
    else if( strcasecmp(argv[1], "getWpLastDepth_") == 0 ) {
      tcl.resultf("%f", waypoint_vect.rbegin()->getDestination().getDepth() );
      return TCL_OK;
    }
  }
  return WossPosition::command(argc, argv);
}


bool WossWpPosition::isEquivalentTo( const woss::CoordZ& coordz ) {
  // disable curr_coordz updating by NOT calling getLocation()
  return curr_coordz.getCartDistance(coordz) <= comparison_distance; 
}


void WossWpPosition::update( double time ) {
     
//   ::std::cout << NOW "  WossWpPosition::update() now " << time << "; last_time_update " << last_time_update
//               << "; time_threshold " << time_threshold << " curr loc " << curr_coordz << ::std::endl;
                
  last_time_update = time;
  if( waypoint_vect.empty() || waypoint_vect.size() == 1 ) return;
  
  TIMIter it = timeid_map.lower_bound(time);
  TIMIter before_it;
  
  if ( it == timeid_map.end() ) return;
  if ( it->first == time ) setLocation( waypoint_vect[it->second].getDestination() );
  else {
    before_it = it;
    before_it--;
    double time_elapsed = time - before_it->first;
    woss::CoordZ pos = waypoint_vect[it->second - 1].getCurrentPosition( waypoint_vect[it->second], time_elapsed );
    setLocation( pos );  
    updateVerticalOrientation( waypoint_vect[it->second - 1].getDestination(), pos );
    updateBearing( waypoint_vect[it->second - 1].getDestination(), pos );
  }
  
//   ::std::cout << "WossWpPosition::update() update loc " << curr_coordz << ::std::endl;
}
  
   
double WossWpPosition::addWayPoint( const WayPoint& waypoint ) { 
  double time_of_arrival;
  
  if ( waypoint_vect.empty() ) {
    time_of_arrival = 0.0;
  }
  else time_of_arrival = timeid_map.rbegin()->first + waypoint_vect.rbegin()->getTimeOfArrival( waypoint );
    
//   ::std::cout << NOW << "  WossWpPosition::addWayPoint() waypoint = " << waypoint << "; cumulative toa = " << time_of_arrival << ::std::endl;

  waypoint_vect.push_back( waypoint );
  timeid_map[time_of_arrival] = waypoint_vect.size() - 1;
  return time_of_arrival;
}


double WossWpPosition::addLoopPoint( const WayPoint& waypoint ) {
  assert( !waypoint_vect.empty() );
 
  double time_of_arrival = addWayPoint(waypoint);

  int start_id = waypoint.getLoopId();
  int end_id = waypoint_vect.size();
 
  for (int i = 0; i < waypoint.getTotalLoops(); i++ ) {
//     ::std::cout << NOW << "  WossWpPosition::addLoopPoint() current loop = " << i << ::std::endl;

    time_of_arrival = addWayPoint( WayPoint( waypoint_vect[start_id].getDestination(), waypoint.getSpeed(), waypoint_vect[start_id].getTimeToWait() ) );
    
    for ( int id = start_id + 1; id < end_id; id++ ) {
        time_of_arrival = addWayPoint( waypoint_vect[id] );   
    }
  }
  
  return time_of_arrival;
}


void WossWpPosition::updateVerticalOrientation( const woss::CoordZ& prev, const woss::CoordZ& curr ) {
  double prev_depth = prev.getDepth();
  double curr_depth = curr.getDepth();
  double max_depth = ::std::max( prev_depth, curr_depth );
  double delta_depth = curr_depth - prev_depth;
  
  double depth_sign = 1.0;
  if ( delta_depth < 0.0 ) depth_sign = -1.0;
  
  double total_distance = prev.getGreatCircleDistance( curr, max_depth );
  
  if (total_distance == 0.0) {
    if (delta_depth != 0.0) {
      // vertical movement only
      total_distance = ::std::abs(delta_depth);
    }
    else {
      // no movement at all: no update
      return;
    }
  }
  
  vertical_orientation = depth_sign * 180.0 / M_PI * ::std::acos( 1.0 - ::std::sqrt( ( ::std::abs(delta_depth) ) / total_distance ) );
  
//   ::std::cout << "WossWpPosition::updateVerticalOrientation() prev " << prev << "; curr " << curr 
//               << "; delta depth " << delta_depth << "; total distance " << total_distance 
//               << "; vertical orient " << vertical_orientation << "; min vo " << min_vertical_orientation
//               << "; max vo " << max_vertical_orientation; 
  
  if ( vertical_orientation > max_vertical_orientation ) vertical_orientation = max_vertical_orientation;
  if ( vertical_orientation < min_vertical_orientation ) vertical_orientation = min_vertical_orientation;

//   ::std::cout << "; upd vertical orient " << vertical_orientation << ::std::endl;
}


void WossWpPosition::updateBearing( const woss::CoordZ& prev, const woss::CoordZ& curr ) {
  bearing = prev.getInitialBearing(curr);

//   ::std::cout << "WossWpPosition::updateBearing() prev " << prev << "; curr " << curr 
//               << "; bearing " << ( bearing * 180.0 / M_PI ) << ::std::endl;
}

  
woss::CoordZ WossWpPosition::getLocation() {
  double now = Scheduler::instance().clock();             
  if (now>last_time_update+time_threshold) update(now);
  return (WossPosition::getLocation());
}


double WossWpPosition::getVerticalOrientation() {
  double now = Scheduler::instance().clock();             
  if (now>last_time_update+time_threshold) update(now);

//   ::std::cout << "WossWpPosition::getVerticalOrientation() vertical orientation " << vertical_orientation << ::std::endl;
  return vertical_orientation;
}


double WossWpPosition::getBearing() {
  double now = Scheduler::instance().clock();             
  if (now>last_time_update+time_threshold) update(now);

//   ::std::cout << "WossWpPosition::getBearing() current bearing " << ( bearing * 180.0 / M_PI ) << ::std::endl;
  return bearing;
}


