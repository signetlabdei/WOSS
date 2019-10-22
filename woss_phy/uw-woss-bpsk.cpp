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
 * @file   uw-woss-bpsk.cpp
 * @author Federico Guerra
 *
 * \brief  Provides the implementation of WossMPhyBpsk class 
 *
 * Provides the implementation of WossMPhyBpsk class 
 */


#include <iostream>
#include <mac.h>
#include <uwlib.h>
#include <woss-manager.h>
#include <definitions-handler.h>
#include <transducer-definitions.h>
#include <transducer-handler.h>
#include <woss-controller.h>
#include "uw-woss-clmsg-channel-estimation.h"
#include "uw-woss-channel-estimator.h"
#include "uw-woss-bpsk.h"
#include <phymac-clmsg.h> 


static class UwMPhyBpskTransducerClass : public TclClass {
public:
  UwMPhyBpskTransducerClass() : TclClass("Module/MPhy/BPSK/Underwater/Transducer") {}
  TclObject* create(int, const char*const*) {
    return (new UwMPhyBpskTransducer);
  }
} class_UwMPhyBpskTransducer;


static class WossMPhyBpskClass : public TclClass {
public:
  WossMPhyBpskClass() : TclClass("WOSS/Module/MPhy/BPSK") {}
  TclObject* create(int, const char*const*) {
    return (new WossMPhyBpsk);
  }
} class_WossMPhyBpsk;


UwMPhyBpskTransducer::UwMPhyBpskTransducer()
: freq_transd_map() 
{ 

}



int UwMPhyBpskTransducer::command(int argc, const char*const* argv) {
  if( argc == 5 ) {
    if(strcasecmp(argv[1], "setTransducerType") == 0) {
       
      if (debug_) ::std::cout << NOW << "  UwMPhyBpskTransducer::command() setTransducerType called, low freq " << argv[2]
                              << "; high freq " << argv[3] << "; type " << argv[4] << ::std::endl;
      
      freq_transd_map[::std::make_pair( atof(argv[2]), atof(argv[3]) )] = 
        woss::SWossController::instance()->getTransducerHandler()->getValue( argv[4] );
      
      return TCL_OK;
    }
  }
  return UnderwaterMPhyBpsk::command(argc, argv);
}



const woss::Transducer* const UwMPhyBpskTransducer::getTransducer( double frequency ) const
{
  if ( freq_transd_map.empty() ) return NULL;
  
  for ( FTMCIter it = freq_transd_map.begin(); it != freq_transd_map.end(); ++it ) {  
    if ( frequency >= it->first.first && frequency <= it->first.second ) {
      
      if (debug_) cout << NOW << "  UwMPhyBpskTransducer::getTransducer() frequency " << frequency << " is in " 
                       << it->first.first << ", " << it->first.second << "; transducer = " 
                       << it->second->getTypeName() << ::std::endl;
      
      return it->second;
    }
  }
  return NULL;
}

double UwMPhyBpskTransducer::consumedEnergyTx( double Ptx, double duration )
{  
  double frequency = spectralmask_->getFreq();
//   double bwidth = spectralmask_->getBandwidth();
  
  if ( CentralFreqOptimization_ ) frequency = txsmask.getFreq();
//   if ( BandwidthOptimization_ ) bwidth = txsmask.getBandwith();
  
  const woss::Transducer* const transducer = getTransducer( frequency );
  
  if ( transducer != NULL && transducer->isValid() ) {
    double fixed_rx = 2.0;
      
    double Power = transducer->getPowerFromSPL( frequency, 10.0 * std::log10( Ptx ) );
    
    if (debug_) cout << NOW << "  UwMPhyBpskTransducer::consumedEnergyTx() transducer = " << transducer->getTypeName()
                     << "; SPL = " << 10.0 * log10( Ptx ) << "; power = " << Power << endl;
    
    return ( (Power + fixed_rx) * duration );
  }
  else return UnderwaterMPhyBpsk::consumedEnergyTx(Ptx, duration);
}


WossMPhyBpsk::WossMPhyBpsk() {

}


double WossMPhyBpsk::getTxPower(Packet* p) {
  static int mac_addr = -1;
  static bool warn_once = false;
  
  if ( SPLOptimization_ ) {
    if (debug_) {
      ClMsgPhy2MacAddr msg;
      sendSyncClMsg(&msg);
      mac_addr = msg.getAddr();
    }
    
    double frequency = spectralmask_->getFreq();
//     double bwidth = spectralmask_->getBandwidth();
    
    if ( CentralFreqOptimization_ ) frequency = txsmask.getFreq();
//     if ( BandwidthOptimization_ ) bwidth = txsmask.getBandwith();
    
    const woss::Transducer* const& transducer = getTransducer( frequency );    
    
    double max_SPL = MaxTxSPL_dB_;
    if ( transducer != NULL && transducer->isValid() ) {
      max_SPL = ::std::min( MaxTxSPL_dB_, transducer->getMaxSPL( frequency ) );
      
      if (debug_) cout << NOW << "  WossMPhyBpsk(" << mac_addr << ")::getTxPower() MaxTxSPL_dB_ = " << MaxTxSPL_dB_
                       << "; max transducer SPL = " << transducer->getMaxSPL( frequency ) << endl;
    
    }
    
//     hdr_cmn* ch = HDR_CMN(p);
    hdr_mac* mach = HDR_MAC(p);
  
    ClMsgChannelEstimation m ( mach->macSA(), mach->macDA() );
    sendSyncClMsg(&m);
    woss::TimeArr* channel = m.getTimeArr();

    if (channel == NULL) {  
      if (debug_) cout << NOW << "  WossMPhyBpsk(" << mac_addr << ")::getTxPower() no estimation info, tx at max power = " 
                       << max_SPL << endl;

      return pow( 10.0, max_SPL / 10.0 );
    }
    else {
      hdr_cmn* ch = HDR_CMN(p);
      
      woss::Pressure* press_temp = woss::SDefHandler::instance()->getPressure()->create( *channel );

      double attenuation = std::pow( press_temp->abs(), 2.0 );
      
      if ( attenuation <= 0.0 ) {
        if (debug_) cout << NOW << "  WossMPhyBpsk(" << mac_addr << ")::getTxPower() no reception possible, tx at max power = " 
                         << max_SPL << endl;

        delete press_temp;

        return pow( 10.0, max_SPL / 10.0 );
      }
      
      double Ptx = woss_uwlib_Ptx_for_PER( spectralmask_->getFreq() / 1000.0, spectralmask_->getBandwidth() / 1000.0, attenuation, PER_target_, 8.0*ch->size(), RxSnrPenalty_dB_ );
      Ptx = Ptx * pow(10.0, TxSPLMargin_dB_ / 10.0);

     if (debug_) cout << NOW << "  WossMPhyBpsk(" << mac_addr << ")::getTxPower() SPL from formula and after SPL margin = " 
                      << 10.0*log10(Ptx) << endl;
  //     //sanity check
  //     double per = uwlib_PER(dist/1000.0, 10*log10(Ptx), 8*ch->size(), RxSnrPenalty_dB_);
  //     std::cerr << " per= " << per
  //     << " pertarget= " << PER_target_
  //     << " Ptx= " << Ptx
  //     << std::endl;
      
      if ( (10.0*log10(Ptx) > max_SPL) && !warn_once ) {
      
        if (!debug_) {
          ClMsgPhy2MacAddr msg;
          sendSyncClMsg(&msg);
          mac_addr = msg.getAddr();
        }
        
        cerr << NOW << "  WossMPhyBpsk(" << mac_addr << ")::getTxPower() WARNING estimated SPL = " 
             << Ptx << 10.0 * std::log10( Ptx ) << " for PER = " << PER_target_ 
             << " is greater than Max Tx SPL = " << max_SPL << endl;
          
        warn_once = true;
      }

      Ptx = std::min(Ptx, pow( 10.0, max_SPL / 10.0 ) );    
      Ptx = std::max(Ptx, pow( 10.0, MinTxSPL_dB_ / 10.0 ) );
      
      delete press_temp;
      
      if (debug_) cout << NOW << "  WossMPhyBpsk(" << mac_addr << ")::getTxPower() final spl = " 
                       << 10.0 * std::log10( Ptx ) << endl;

      return Ptx;
    }
  }
  return pow( 10.0, MaxTxSPL_dB_ / 10.0 );
}

