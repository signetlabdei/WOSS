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
#include "uw-woss-channel.h"
#include "uw-woss-pkt-hdr.h"
#include <definitions-handler.h>
#include <time-arrival-definitions.h>
#include "uw-woss-channel-estimator.h"
#include "uw-woss-clmsg-channel-estimation.h"
#include "uw-woss-position.h"


using namespace woss;


static class WossChannelModuleClass : public TclClass {
public:
	WossChannelModuleClass() : TclClass("WOSS/Module/Channel") {}
	TclObject* create(int, const char*const*) {
	    return (new WossChannelModule());
        }
} class_WossChannelModule_module;


WossChannelModule::WossChannelModule() 
: woss_manager(NULL),
  channel_estimator(NULL)
{
  bind("channel_symbol_resolution_", &channel_symbol_resolution);
  bind("channel_eq_time_", &channel_eq_time);
  bind("channel_eq_snr_threshold_db_", &channel_eq_snr_threshold_db);
  bind("channel_max_distance_", &channel_max_distance);
  bind("windspeed_", &uw.windspeed);
  bind("shipping_", &uw.shipping);
  bind("practical_spreading_", &uw.practical_spreading);
  bind("prop_speed_", &uw.prop_speed);

//   if (channel_eq_snr_threshold_db < 0) channel_eq_snr_threshold_db = 0;
  if (channel_eq_time < 0.0 ) channel_eq_time = HUGE_VAL;
  if (channel_max_distance < 0.0) channel_max_distance = HUGE_VAL;

//   if (channel_symbol_resolution < 0 ) channel_symbol_resolution = HUGE_VAL;
}


WossChannelModule::~WossChannelModule() {
  
}


int WossChannelModule::command(int argc, const char*const* argv) {
  Tcl& tcl = Tcl::instance();

 if (argc==3) {
    if(strcasecmp(argv[1], "setWossManager") == 0) {
      if (debug_) cout << NOW << "  WossChannelModule::command() setWossManager called"  << endl;
      woss_manager = dynamic_cast< WossManager* >( tcl.lookup(argv[2]) );

      if (woss_manager) return TCL_OK;
      else return TCL_ERROR;
    }
    else if(strcasecmp(argv[1], "setChannelEstimator") == 0) {
      if (debug_) cout << NOW << "  WossChannelModule::command() setChannelEstimator called"  << endl;
      channel_estimator = dynamic_cast< ChannelEstimator* >( tcl.lookup(argv[2]) );

      if (channel_estimator) return TCL_OK;
      else return TCL_ERROR;
    }
  }  
  
  return ChannelModule::command(argc, argv);
}


void WossChannelModule::recv(Packet* p, ChSAP* chsap) {
  if (!woss_manager) {
     cout << NOW << "  WossChannelModule::recv() ERROR, setWossManager not called!" << endl;
     exit(1);
  }
  
  hdr_cmn* cmh = HDR_CMN(p);
  hdr_MPhy* ph = HDR_MPHY(p);
	
  cmh->direction() = hdr_cmn::UP;
     
  chsap_vector.clear();
  chsap_vector.reserve(getChSAPnum());
  
	double noiseSPDdBperHz = uw.getNoise(ph->srcSpectralMask->getFreq()/1000.0);
  double noisepow_db = 10.0 * ::std::log10(ph->srcSpectralMask->getBandwidth() * ::std::pow(10, noiseSPDdBperHz/10.0));
	channel_eq_attenuation_db = -(channel_eq_snr_threshold_db - 10.0*log10(ph->Pt) + noisepow_db);
	if ( channel_eq_attenuation_db < 0 ) channel_eq_attenuation_db = 0;
	
	if (debug_) cout << NOW << "  WossChannelModule::recv() freq " << ph->srcSpectralMask->getBandwidth()
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
  
  if (debug_) cout << NOW << " WossChannelModule::computeCoordZPairVect() " << endl;

  for (int i=0; i < getChSAPnum(); i++) {
    dest = (ChSAP*)getChSAP(i);
          
    if (chsap == dest) { // it's the source node -> skip it
      if ( debug_ ) cout << "    tx ChSAP num = " << i << "; coordz = " << tx_coordz << endl;

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
      ret_value.push_back( ::std::make_pair( tx_coordz, rx_coordz ) );
    }
  } 
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
  
  return( ::std::make_pair( curr_freq, curr_freq ) );
}


woss::TimeArrVector WossChannelModule::computeTimeArrVector( const CoordZPairVect& coordinates, const SimFreq& sim_freq ) {
  return( woss_manager->getWossTimeArr( coordinates, sim_freq.first, sim_freq.second, NOW ) );
}


void WossChannelModule::checkTimeArrVector( const CoordZPairVect& coords, TimeArrVector& channels ) { 
  TimeArr* coherent_sampled_channel = NULL;
  TimeArr* incoherent_sampled_channel = NULL;
	TimeArr* cropped_channel_equaliz = NULL;
	TimeArr* cropped_channel_after_equaliz = NULL;
	TimeArr* final_channel = NULL;
	
  for( int i = 0; i < (int)channels.size(); i++ ) {
    if ( channels[i]->isConvertedFromPressure() ) {
      std::complex<double> value = channels[i]->begin()->second;
      channels[i]->clear();
      channels[i]->insertValue( getPropDelay( coords[i].first, coords[i].second ), value ); 
    }
    
    if ( channels[i]->size() == 1 ) continue;
    
    if ( channel_symbol_resolution > 0.0 ) {
      coherent_sampled_channel = channels[i]->coherentSumSample( channel_symbol_resolution );  
    }
    else {
			coherent_sampled_channel = channels[i];
			channels[i] = NULL;
		}

    if (debug_) cout << NOW << "  WossChannelModule::checkTimeArrVector() symbol sampled response :" 
                     << *coherent_sampled_channel << endl;			

    TimeArrCIt tap_iter;
    if ( channel_eq_attenuation_db > 0 ) tap_iter = coherent_sampled_channel->lowerBoundTxLoss(channel_eq_attenuation_db);
    else tap_iter = coherent_sampled_channel->begin();

    if ( channel_eq_time != HUGE_VAL && channel_eq_time != 0 ) {
      cropped_channel_equaliz = coherent_sampled_channel->crop(tap_iter->first, ( (double) tap_iter->first + channel_eq_time) );
      cropped_channel_after_equaliz = coherent_sampled_channel->crop( ( (double) tap_iter->first + channel_eq_time) , HUGE_VAL);	
    }
    else {
      cropped_channel_equaliz = coherent_sampled_channel;
      cropped_channel_after_equaliz = coherent_sampled_channel->create(TimeArr::createNotValid());
      coherent_sampled_channel = NULL;
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

        if (debug_) cout << NOW << "  WossChannelModule::checkTimeArrVector() incoher channel after eq :" 
                         << *incoherent_sampled_channel << endl;			

        if ( cropped_channel_after_equaliz->isValid() ) 
          final_channel = (*incoherent_sampled_channel + *cropped_channel_after_equaliz).clone();
        else {
          final_channel = incoherent_sampled_channel;
          incoherent_sampled_channel = NULL;
        }
      }
      else {
        final_channel = cropped_channel_after_equaliz;
        cropped_channel_after_equaliz = NULL;
        incoherent_sampled_channel = NULL;				
      }
    }
    else {
      incoherent_sampled_channel = cropped_channel_equaliz;
      cropped_channel_equaliz = NULL;
      final_channel = incoherent_sampled_channel->clone();
    }

    if (debug_) cout << NOW << "  WossChannelModule::checkTimeArrVector() final channel: " 
                     << *final_channel << endl;					

    if ( channels[i] != NULL ) delete channels[i];		
    if ( coherent_sampled_channel != NULL ) delete coherent_sampled_channel;
    if ( cropped_channel_equaliz != NULL ) delete cropped_channel_equaliz;
    if ( cropped_channel_after_equaliz != NULL ) delete cropped_channel_after_equaliz;
    if ( incoherent_sampled_channel != NULL ) delete incoherent_sampled_channel;
		
    channels[i] = final_channel;      

    if ( channel_estimator ) channel_estimator->updateEstimation( coords[i].first, coords[i].second, final_channel->clone() );
  }
}


void WossChannelModule::schedulePacketCopies( const woss::CoordZPairVect& coordinates, const woss::TimeArrVector& channels, const woss::SimFreq& sim_freq, Packet* p ) {
  Scheduler &s = Scheduler::instance();

  for( int i = 0; i < (int)coordinates.size(); i++ ) {
    
    double curr_dist = coordinates[i].first.getCartDistance(coordinates[i].second);
    
    if (debug_) cout << NOW << "  WossChannelModule::schedulePacketCopies() sampled response :" << endl;

    for ( TimeArrCIt it = channels[i]->begin(); it != channels[i]->end(); it++ ) {

      if (debug_) cout << "   delay = " << it->first << "; complex att = " << it->second 
                        << "; db = " << Pressure::getTxLossDb(it->second) << endl;

      Packet* p_copy = p->copy();

      hdr_woss* hdr_woss = HDR_WOSS(p_copy);

      Pressure* press_temp = SDefHandler::instance()->getPressure()->create( (it->second) );
      press_temp->checkAttenuation( curr_dist, sim_freq.first );
      const_cast< ::std::complex<double>& >( it->second ) = *press_temp;
          
      hdr_woss->already_processed = true;
      hdr_woss->attenuation = *press_temp;
      hdr_woss->frequency = sim_freq.first;

      if (debug_) cout << "  packet pressure db = " << Pressure::getTxLossDb(hdr_woss->attenuation) << endl;
      
      delete press_temp;
      press_temp = NULL;
      
      s.schedule(chsap_vector[i], p_copy, it->first);
    }
  }
}


void WossChannelModule::deleteChannels( woss::TimeArrVector& channels ) {
  for( int i = 0; i < (int)channels.size(); i++ ) {
    delete channels[i];
    channels[i] = NULL;
  }
  channels.clear();
}
  
  
double WossChannelModule::getPropDelay( const CoordZ& tx, const CoordZ& rx ) {
  return(uw.getPropagationDelay( tx.getLatitude(), tx.getLongitude(), ( -1.0 * tx.getDepth()),
                                 rx.getLatitude(), rx.getLongitude(), ( -1.0 * rx.getDepth())));
}


