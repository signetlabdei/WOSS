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
 * @file   uw-woss-channel.h
 * @author Federico Guerra
 *
 * \brief  Provides the interface for WossChannelModule class 
 *
 * Provides the interface for WossChannelModule class
 */


#ifndef UW_WOSS_CHANNEL_H
#define UW_WOSS_CHANNEL_H


#include <channel-module.h>
#include <underwater.h>
#include <woss-manager.h>


namespace woss {
  class TimeArr;
  class WossManager;
}


class ChannelEstimator;


/**
 * \brief  WossChannelModule class for channel calculations with WOSS
 *
 * WossChannelModule extends ChannelModule for channel calculations with WOSS
 */
class  WossChannelModule : public ChannelModule
{

  
  public:
  
    
  WossChannelModule();
  
  virtual ~ WossChannelModule();
  
  
  virtual int command(int argc, const char*const* argv);  
  
  
  virtual void recv(Packet* p, ChSAP* chsap);
  
  
  protected:

    
  typedef ::std::vector< ChSAP* > ChSAPVector;
  
  
  double getPropDelay( const woss::CoordZ& s, const woss::CoordZ& d);
 
   
  woss::CoordZPairVect computeCoordZPairVect( ChSAP* dest );
  
  woss::SimFreq computeSimFreq( Packet* p );
  
  woss::TimeArrVector computeTimeArrVector( const woss::CoordZPairVect& coords, const woss::SimFreq& sim_freq );
  
  void checkTimeArrVector( const woss::CoordZPairVect& coords, woss::TimeArrVector& channels );

  void schedulePacketCopies( const woss::CoordZPairVect& coords, const woss::TimeArrVector& channels, const woss::SimFreq& sim_freq, Packet* p );

  void deleteChannels( woss::TimeArrVector& channels );

  
  double channel_eq_snr_threshold_db;

  double channel_symbol_resolution;

  double channel_eq_time;

  double channel_eq_attenuation_db;

  double channel_max_distance; // [m]
  
  
  woss::WossManager* woss_manager;
  
  ChannelEstimator* channel_estimator;
    
  
  Underwater uw;
 
  ChSAPVector chsap_vector;

};


#endif /* UW_WOSS_CHANNEL_H */

