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
 * @file   uw-woss-channel.cpp
 * @author Federico Guerra
 *
 * \brief  Provides the implementation of WossChannelModule class 
 *
 * Provides the implementation of WossChannelModule class
 */


#include <iostream>
#include <iomanip>
#include <mphy.h>
#include <uw-woss-channel.h>
#include <uw-woss-pkt-hdr.h>
#include <definitions-handler.h>
#include <time-arrival-definitions.h>
#include <uw-woss-channel-estimator.h>
#include <uw-woss-clmsg-channel-estimation.h>
#include <uw-woss-position.h>


using namespace woss;
using namespace std;

static class WossChannelModuleClass : public TclClass {
  public:
  WossChannelModuleClass() : TclClass("WOSS/Module/Channel") {}
  TclObject* create(int, const char*const*) {
    return (new WossChannelModule());
  }
} class_WossChannelModule_module;


WossChannelModule::WossChannelModule() 
: woss_manager(nullptr),
  channel_estimator(nullptr)
{
  bind("channel_symbol_resolution_", &channel_symbol_resolution);
  bind("channel_eq_time_", &channel_eq_time);
  bind("channel_eq_snr_threshold_db_", &channel_eq_snr_threshold_db);
  bind("channel_max_distance_", &channel_max_distance);
  bind("windspeed_", &uw.windspeed);
  bind("shipping_", &uw.shipping);
  bind("practical_spreading_", &uw.practical_spreading);
  bind("prop_speed_", &uw.prop_speed);

//   if (channel_eq_snr_threshold_db < 0) 
//     channel_eq_snr_threshold_db = 0;
  if (channel_eq_time < 0.0 ) 
    channel_eq_time = HUGE_VAL;
  if (channel_max_distance < 0.0) 
    channel_max_distance = HUGE_VAL;

//   if (channel_symbol_resolution < 0 ) 
//    channel_symbol_resolution = HUGE_VAL;
}


int WossChannelModule::command(int argc, const char*const* argv) {
  Tcl& tcl = Tcl::instance();

 if (argc==3) {
    if(strcasecmp(argv[1], "setWossManager") == 0) {
      if (debug_) 
        cout << NOW << "  WossChannelModule::command() setWossManager called"  << endl;
      woss_manager = dynamic_cast< WossManager* >( tcl.lookup(argv[2]) );

      if (woss_manager) 
        return TCL_OK;
      else 
        return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setChannelEstimator") == 0) {
      if (debug_) 
        cout << NOW << "  WossChannelModule::command() setChannelEstimator called"  << endl;
      channel_estimator = dynamic_cast< ChannelEstimator* >( tcl.lookup(argv[2]) );

      if (channel_estimator) 
        return TCL_OK;
      else 
        return TCL_ERROR;
    }
  }

  return ChannelModule::command(argc, argv);
}


void WossChannelModule::recv(Packet* p, ChSAP* chsap) {
  if (!woss_manager) {
     cerr << NOW << "  WossChannelModule::recv() ERROR, setWossManager not called!" << endl;
     exit(1);
  }
  
  hdr_cmn* cmh = HDR_CMN(p);
  hdr_MPhy* ph = HDR_MPHY(p);
	
  cmh->direction() = hdr_cmn::UP;

  chsap_vector.clear();
  chsap_vector.reserve(getChSAPnum());

  double noiseSPDdBperHz = uw.getNoise(ph->srcSpectralMask->getFreq()/1000.0);
  double noisepow_db = 10.0 * std::log10(ph->srcSpectralMask->getBandwidth() * std::pow(10, noiseSPDdBperHz/10.0));
  channel_eq_attenuation_db = -(channel_eq_snr_threshold_db - 10.0*log10(ph->Pt) + noisepow_db);

  if ( channel_eq_attenuation_db < 0 ) 
    channel_eq_attenuation_db = 0;

  if (debug_) 
    cout << NOW << "  WossChannelModule::recv() freq " << ph->srcSpectralMask->getBandwidth()
         << "; bw " << ph->srcSpectralMask->getBandwidth() << "; Pt db " << 10.0*log10(ph->Pt)
         << "; noisespd_per hz " << noiseSPDdBperHz
         << "; noise_pow_db "<< noisepow_db
         << "; channel_att_eq_db " <<  channel_eq_attenuation_db << endl;

  CoordZPairVect coordinates = computeCoordZPairVect( chsap );
  
  SimFreq sim_freq = computeSimFreq( p );
  
  TimeArrVector channels = computeTimeArrVector( coordinates, sim_freq );
  
  assert( channels.size() == coordinates.size() && channels.size() == chsap_vector.size() );
   
  checkTimeArrVector( coordinates, channels );
   
  schedulePacketCopies( coordinates, channels, sim_freq, p );
  
  deleteChannels( channels );
  
  Packet::free(p);
}


woss::CoordZPairVect WossChannelModule::computeCoordZPairVect( ChSAP* chsap ) { 
  CoordZPairVect ret_value;
  ret_value.reserve(getChSAPnum());
  
  WossPosition* sourcePos = dynamic_cast< WossPosition* >( chsap->getPosition() );
  WossPosition* destPos;

  assert( sourcePos );
  
  // HAS TO BE UNDERWATER!!!
  assert( sourcePos->getDepth() >= 0.0 );
  
  CoordZ tx_coordz = sourcePos->getLocation();
      
  ChSAP* dest;
  
  if (debug_) 
    cout << NOW << " WossChannelModule::computeCoordZPairVect() " << endl;

  for (int i=0; i < getChSAPnum(); i++) {
    dest = (ChSAP*)getChSAP(i);
          
    if (chsap == dest) { // it's the source node -> skip it
      if ( debug_ ) 
        cout << "    tx ChSAP num = " << i << "; tx coordz = " << tx_coordz << endl;

      continue;
    }

    destPos = dynamic_cast< WossPosition* >( dest->getPosition() );

    // HAS TO BE UNDERWATER!!!
//     assert( destPos->getZ() < 0.0 );

    CoordZ rx_coordz = destPos->getLocation();
    
    if ( debug_ ) 
      cout << "    rx ChSAP num = " << i 
           << "; channel_max_distance = " << channel_max_distance
           << "; rx_coordz = " << rx_coordz << endl;

    // check distance first
    if (tx_coordz.getGreatCircleDistance(rx_coordz) <= channel_max_distance) {
      chsap_vector.push_back(dest);
      ret_value.emplace_back( std::make_pair( tx_coordz, rx_coordz ) );
    }
  }
  if ( debug_ ) 
    cout << NOW << " WossChannelModule::computeCoordZPairVect() size = " << ret_value.size() << endl;

  return ret_value;
}


woss::SimFreq WossChannelModule::computeSimFreq( Packet* p ) {
  hdr_MPhy* ph = HDR_MPHY(p);

  double curr_f0 = ph->srcSpectralMask->getFreq();
  double curr_bw = ph->srcSpectralMask->getBandwidth();
  double curr_f1 = curr_f0 + curr_bw/2.0;
  double curr_f2 = curr_f0 - curr_bw/2.0;

  assert(curr_f1 > 0);
  assert(curr_f2 > 0);

  double curr_freq = sqrt( curr_f1 * curr_f2 );
  
  return( std::make_pair( curr_freq, curr_freq ) );
}


woss::TimeArrVector WossChannelModule::computeTimeArrVector( const CoordZPairVect& coordinates, const SimFreq& sim_freq ) {
  return( woss_manager->getWossTimeArr( coordinates, sim_freq.first, sim_freq.second, NOW ) );
}


void WossChannelModule::checkTimeArrVector( const CoordZPairVect& coords, TimeArrVector& channels ) { 
  std::unique_ptr<TimeArr> coherent_sampled_channel;
  std::unique_ptr<TimeArr> incoherent_sampled_channel;
  std::unique_ptr<TimeArr> cropped_channel_equaliz;
  std::unique_ptr<TimeArr> cropped_channel_after_equaliz;
  std::unique_ptr<TimeArr> final_channel;

  for( int i = 0; i < (int)channels.size(); i++ ) {
    if ( channels[i]->isConvertedFromPressure() ) {
      auto value = channels[i]->begin()->second;
      channels[i]->clear();
      channels[i]->insertValue( getPropDelay( coords[i].first, coords[i].second ), value ); 
    }
    
    if ( channels[i]->size() == 1 ) 
      continue;
    
    if ( channel_symbol_resolution > 0.0 ) {
      coherent_sampled_channel = channels[i]->coherentSumSample( channel_symbol_resolution );  
    }
    else {
			coherent_sampled_channel = std::move(channels[i]);
			channels[i] = nullptr;
		}

    if (debug_) 
      cout << NOW << "  WossChannelModule::checkTimeArrVector() symbol sampled response :" 
           << *coherent_sampled_channel << endl;

    TimeArrCIt tap_iter;
    if ( channel_eq_attenuation_db > 0 ) 
      tap_iter = coherent_sampled_channel->lowerBoundTxLoss(channel_eq_attenuation_db);
    else 
      tap_iter = coherent_sampled_channel->begin();

    if ( channel_eq_time != HUGE_VAL && channel_eq_time != 0 ) {
      cropped_channel_equaliz = coherent_sampled_channel->crop(tap_iter->first, ( (double) tap_iter->first + channel_eq_time) );
      cropped_channel_after_equaliz = coherent_sampled_channel->crop( ( (double) tap_iter->first + channel_eq_time) , HUGE_VAL);	
    }
    else {
      cropped_channel_equaliz = std::move(coherent_sampled_channel);
      cropped_channel_after_equaliz = cropped_channel_equaliz->create(TimeArr::createNotValid());
      coherent_sampled_channel = nullptr;
    } 
 
    if (debug_ && cropped_channel_equaliz->isValid() ) 
      cout << NOW << "  WossChannelModule::checkTimeArrVector() cropped channel for eq :" 
           << *cropped_channel_equaliz << endl;

    if (debug_ && cropped_channel_after_equaliz->isValid() ) 
      cout << NOW << "  WossChannelModule::checkTimeArrVector() cropped channel after t_eq :" 
           << *cropped_channel_after_equaliz << endl;

    if ( channel_eq_time != 0 ) {
      if ( cropped_channel_equaliz->isValid() ) {
        incoherent_sampled_channel = cropped_channel_equaliz->incoherentSumSample( channel_eq_time ); 

        if (debug_) 
          cout << NOW << "  WossChannelModule::checkTimeArrVector() incoher channel after eq :" 
               << *incoherent_sampled_channel << endl;

        if ( cropped_channel_after_equaliz->isValid() ) {
          *incoherent_sampled_channel += *cropped_channel_after_equaliz;
          final_channel = incoherent_sampled_channel->clone();
        }
        else {
          final_channel = std::move(incoherent_sampled_channel);
          incoherent_sampled_channel = nullptr;
        }
      }
      else {
        final_channel = std::move(cropped_channel_after_equaliz);
        cropped_channel_after_equaliz = nullptr;
        incoherent_sampled_channel = nullptr;
      }
    }
    else {
      incoherent_sampled_channel = std::move(cropped_channel_equaliz);
      cropped_channel_equaliz = nullptr;
      final_channel = incoherent_sampled_channel->clone();
    }

    if (debug_) 
      cout << NOW << "  WossChannelModule::checkTimeArrVector() final channel: " 
           << *final_channel << endl;

    channels[i] = std::move(final_channel);

    if ( channel_estimator ) 
      channel_estimator->updateEstimation( coords[i].first, coords[i].second, *channels[i] );
  }
}


void WossChannelModule::schedulePacketCopies( const woss::CoordZPairVect& coordinates, const woss::TimeArrVector& channels, const woss::SimFreq& sim_freq, Packet* p ) {
  Scheduler &s = Scheduler::instance();

  for( int i = 0; i < (int)coordinates.size(); i++ ) {
    double curr_dist = coordinates[i].first.getCartDistance(coordinates[i].second);

    if (debug_) 
      cout << NOW << "  WossChannelModule::schedulePacketCopies() sampled response :" << endl;

    for ( auto it = channels[i]->begin(); it != channels[i]->end(); it++ ) {
      if (debug_) 
        cout << "   delay = " << it->first << "; complex att = " << it->second 
             << "; db = " << Pressure::getTxLossDb(it->second) << endl;

      Packet* p_copy = p->copy();

      hdr_woss* hdr_woss = HDR_WOSS(p_copy);

      auto press_temp = SDefHandler::instance().createPressure( (it->second) );
      press_temp->checkAttenuation( curr_dist, sim_freq.first );
      const_cast< std::complex<double>& >( it->second ) = *press_temp;

      hdr_woss->already_processed = true;
      hdr_woss->attenuation = *press_temp;
      hdr_woss->frequency = sim_freq.first;

      if (debug_) 
        cout << "  packet pressure db = " << Pressure::getTxLossDb(hdr_woss->attenuation) << endl;
      
      press_temp.reset();

      s.schedule(chsap_vector[i], p_copy, it->first);
    }
  }
}


void WossChannelModule::deleteChannels( woss::TimeArrVector& channels ) {
  channels.clear();
}

double WossChannelModule::getPropDelay( const CoordZ& tx, const CoordZ& rx ) {
  return(uw.getPropagationDelay( tx.getLatitude(), tx.getLongitude(), ( -1.0 * tx.getDepth()),
                                 rx.getLatitude(), rx.getLongitude(), ( -1.0 * rx.getDepth())));
}
