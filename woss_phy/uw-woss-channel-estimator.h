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
 * @file   uw-woss-channel-estimator.h
 * @author Federico Guerra
 *
 * \brief  Provides the interfaces for ChannelEstimator and ChEstimatorPlugIn classes
 *
 * Provides the interfaces for ChannelEstimator and ChEstimatorPlugIn classes
 */


#ifndef CHANNEL_ESTIMATOR_H
#define CHANNEL_ESTIMATOR_H


#include <plugin.h>
#include <coordinates-definitions.h>
#include <map>
#include "uw-woss-position.h"


namespace woss {
  class TimeArr;
}


/**
 * \brief Class for channel estimation and averaging
 *
 * ChannelEstimator provides extensible estimation and averaging methods. 
 */
class ChannelEstimator : public TclObject {
  
  
  public:
  
    
  typedef ::std::map< woss::CoordZ, woss::TimeArr*, woss::CoordComparator< ChannelEstimator, woss::CoordZ > > RxMap;
  typedef RxMap::iterator RxMIter;
  typedef RxMap::const_iterator RxMCIter;
  typedef RxMap::reverse_iterator RxMRIter;
  
  typedef ::std::map< woss::CoordZ, RxMap, woss::CoordComparator< ChannelEstimator, woss::CoordZ > > ChannelMap;
  typedef ChannelMap::iterator ChMapIter;
  typedef ChannelMap::reverse_iterator ChMapRIter;
    
  typedef std::map< int, WossPosition* > MacToPosMap;
  typedef MacToPosMap::iterator MacMapIter;
  typedef MacToPosMap::reverse_iterator MacMapRIter;
  
//   typedef std::map< std::pair< woss::CoordZ, woss::CoordZ > , woss::TimeArr* > ChannelMap;
//   typedef ChannelMap::iterator ChMapIter;
//   typedef ChannelMap::reverse_iterator ChMapRIter;

  
  ChannelEstimator();
  
  virtual ~ChannelEstimator(); 
  
  
  virtual int command(int argc, const char*const* argv);
  
  
  static double getSpaceSampling() { return space_sampling; }
  
  
  /**
  * Updates channel estimation for the given tx-rx couple
  *
  * @param tx ns address type of transmitter node
  * @param rx ns address type of receiver node
  * @param curr_channel pointer to new channel value
  */
  virtual void updateEstimation( const woss::CoordZ& tx, const woss::CoordZ& rx, woss::TimeArr* curr_channel );
 
  /**
  * Returns channel estimation for the given tx-rx couple
  *
  * @param tx ns address type of transmitter node
  * @param rx ns address type of receiver node
  * @returns pointer to a heap-allocated woss::TimeArr channel
  */
  woss::TimeArr* getEstimation( const woss::CoordZ& tx, const woss::CoordZ& rx );
  
  
  void addMacAddress( int addr, WossPosition* pos ) { mac_to_pos_map[addr] = pos; }
  
  virtual WossPosition* findMacAddress( int addr );
  
  
  virtual bool resetEstimator();
  
  
  protected:
  
  
  static double space_sampling;
  
    
  ChannelMap channel_map;
   
  MacToPosMap mac_to_pos_map;

  
  double debug_;
  
  double avg_coeff;

};


/**
 * \brief Service class for attaching a ChannelEstimator to the node bus
 *
 * ChEstimatorPlugIn allows attaching of a ChannelEstimator instance to the node bus, permitting 
 * cross-layer communications 
 */
class ChEstimatorPlugIn : public PlugIn {

  
  public:
    
    
  ChEstimatorPlugIn();
  
  virtual ~ChEstimatorPlugIn() { }
  
  
  virtual int command(int argc, const char*const* argv);
  
  virtual int recvSyncClMsg(ClMessage* m);
  
  
  protected:
  
    
  ChannelEstimator* channel_estimator;
  
  double debug_;

};


#endif  // CHANNEL_ESTIMATOR_H

