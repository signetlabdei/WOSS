/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2009 2025 Federico Guerra
 * and regents of the SIGNET lab, University of Padova
 *
 * Author: Federico Guerra - WOSS@guerra-tlc.com
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

#include <map>
#include <memory>
#include <plugin.h>
#include <coordinates-definitions.h>
#include <time-arrival-definitions.h>
#include <uw-woss-position.h>


/**
 * \brief Class for channel estimation and averaging
 *
 * ChannelEstimator provides extensible estimation and averaging methods. 
 */
class ChannelEstimator : public TclObject {

  public:

  using RxMap = std::map< woss::CoordZ, std::unique_ptr<woss::TimeArr>, woss::CoordComparator< ChannelEstimator, woss::CoordZ > >;
  using RxMIter = RxMap::iterator;
  using RxMCIter = RxMap::const_iterator;
  using RxMRIter = RxMap::reverse_iterator;
  
  using ChannelMap = std::map< woss::CoordZ, RxMap, woss::CoordComparator< ChannelEstimator, woss::CoordZ > >;
  using ChMapIter = ChannelMap::iterator;
  using ChMapRIter = ChannelMap::reverse_iterator;
    
  using MacToPosMap = std::map< int, WossPosition* >;
  using MacMapIter = MacToPosMap::iterator;
  using MacMapRIter = MacToPosMap::reverse_iterator;
  
  ChannelEstimator();
  
  virtual ~ChannelEstimator() override = default;
  
  virtual int command(int argc, const char*const* argv) override;
  
  static double getSpaceSampling() { return space_sampling; }
  
  /**
  * Updates channel estimation for the given tx-rx couple
  *
  * @param tx ns address type of transmitter node
  * @param rx ns address type of receiver node
  * @param curr_channel pointer to new channel value
  */
  virtual void updateEstimation( const woss::CoordZ& tx, const woss::CoordZ& rx, const woss::TimeArr& curr_channel );
 
  /**
  * Returns channel estimation for the given tx-rx couple
  *
  * @param tx ns address type of transmitter node
  * @param rx ns address type of receiver node
  * @returns pointer to a heap-allocated woss::TimeArr channel
  */
  std::unique_ptr<woss::TimeArr> getEstimation( const woss::CoordZ& tx, const woss::CoordZ& rx );

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
  
  virtual ~ChEstimatorPlugIn() override = default;
  
  virtual int command(int argc, const char*const* argv) override;
  
  virtual int recvSyncClMsg(ClMessage* m) override;
  
  protected:
    
  ChannelEstimator* channel_estimator;
  
  double debug_;

};

#endif  // CHANNEL_ESTIMATOR_H
