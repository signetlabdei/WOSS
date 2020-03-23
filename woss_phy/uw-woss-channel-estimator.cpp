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
 * @file   uw-woss-channel-estimator.cpp
 * @author Federico Guerra
 *
 * \brief  Provides the interface for ChannelEstimator class 
 *
 * Provides the interface for ChannelEstimator class
 */


#include <iostream>
#include "uw-woss-channel-estimator.h"
#include "uw-woss-clmsg-channel-estimation.h"
#include <position-clmsg.h>

#include <definitions-handler.h>
#include <time-arrival-definitions.h>
#include <definitions.h>


static class ChannelEstimatorClass : public TclClass {
public:
  ChannelEstimatorClass() : TclClass("WOSS/ChannelEstimator") {}
   TclObject* create(int, const char*const*) {
      return (new ChannelEstimator());
   }
} class_channelEstimatorClass;


// ChannelMap ChannelEstimator::channel_map;

double ChannelEstimator::space_sampling = 0.0;


ChannelEstimator::ChannelEstimator() {
  bind("debug_",&debug_);
  bind("avg_coeff_", &avg_coeff);
  bind("space_sampling_",&space_sampling);
}


ChannelEstimator::~ChannelEstimator() {
  resetEstimator();
}


int ChannelEstimator::command(int argc, const char*const* argv) {
  if(argc == 2) {
    if(strcasecmp(argv[1], "resetEstimator") == 0) {
      if ( resetEstimator() ) return TCL_OK;
      else return TCL_ERROR;
    }
  }
  return TclObject::command(argc, argv);
}


void ChannelEstimator::updateEstimation( const woss::CoordZ& tx, const woss::CoordZ& rx, woss::TimeArr* curr_channel ) {  
  if (debug_) std::cout << NOW << "  ChannelEstimator::updateEstimation() size = " << channel_map.size() 
                        << "; tx = " << tx << "; rx = " << rx 
                        << "; channel = " << *curr_channel << std::endl;
  
  ChMapIter it = channel_map.find( tx );
  
  if ( it == channel_map.end() ) {
    if (debug_) std::cout << NOW << "  ChannelEstimator::updateEstimation() tx-rx pair not found, inserting new channel = "
                          << *curr_channel << std::endl;

    channel_map[tx][rx] = curr_channel;
  }
  else {
    
    if (debug_) cout << NOW << "  ChannelEstimator::updateEstimation() tx found" << endl;
   
    RxMIter it2 = it->second.find( rx );

    if ( it2 != it->second.end() ) { 
      if (debug_) std::cout << NOW << "  ChannelEstimator::updateEstimation() rx found, old channel = "
                            << *it2->second << std::endl;
      
      if ( avg_coeff > 0) *(it2->second) = avg_coeff * (*(it2->second)) + (1.0 - avg_coeff) * (*curr_channel);
      else *(it2->second) = *curr_channel;
        
      if (debug_) std::cout << NOW << "  ChannelEstimator::updateEstimation() new channel = "
                            << *it2->second << std::endl;
      
      delete curr_channel;
    }
    else (it->second)[rx] = curr_channel;    
  }
}


woss::TimeArr* ChannelEstimator::getEstimation( const woss::CoordZ& tx, const woss::CoordZ& rx ) {
  if (debug_) std::cout << NOW << "  ChannelEstimator::getEstimation() size = " << channel_map.size() 
                        << "; tx = " << tx << "; rx = " << rx << std::endl;

  ChMapIter it = channel_map.find( tx );
  
  if ( it != channel_map.end() ) { 
    
    if (debug_) std::cout << NOW << "  ChannelEstimator::getEstimation() tx found" << ::std::endl;
    
    RxMIter it2 = it->second.find( rx );
    
    if ( it2 != it->second.end() ) {
    
      if (debug_) std::cout << NOW << "  ChannelEstimator::getEstimation() rx found, ch " << *it2->second << std::endl;

      return it2->second->clone();
    }
  }
  
  if (debug_) std::cout << NOW << "  ChannelEstimator::getEstimation() tx or rx not found" << std::endl;
  return NULL;
}


bool ChannelEstimator::resetEstimator() {
  for ( ChMapIter it = channel_map.begin(); it != channel_map.end(); ++it ) {
    for ( RxMIter it2 = it->second.begin(); it2 != it->second.end(); ++it2 ) {
      delete it2->second;
    }
  }
  
  return true;
}


WossPosition* ChannelEstimator::findMacAddress( int addr ) {
  MacMapIter it = mac_to_pos_map.find( addr );
  if ( it != mac_to_pos_map.end() ) return it->second;
  return NULL;
}

////////////////


static class ChEstimatorPlugInClass : public TclClass {
public:
  ChEstimatorPlugInClass() : TclClass("WOSS/PlugIn/ChannelEstimator") {}
  TclObject* create(int, const char*const*) {
      return (new ChEstimatorPlugIn());
        }
} class_ChEstimatorPlugInClass;


ChEstimatorPlugIn::ChEstimatorPlugIn()
: channel_estimator(NULL)
{ 
  bind( "debug_", &debug_ );
}


int ChEstimatorPlugIn::command(int argc, const char*const* argv) {
  Tcl& tcl = Tcl::instance();

 if (argc==3) {
    if(strcasecmp(argv[1], "setChannelEstimator") == 0) {
      if (debug_) cout << NOW << "  ChEstimatorPlugIn::command() setChannelEstimator called"  << endl;
      
      channel_estimator = dynamic_cast< ChannelEstimator* >( tcl.lookup(argv[2]) );

      if (channel_estimator) return TCL_OK;
      return TCL_ERROR;
    }
  }  
  else if (argc==4) {
    if(strcasecmp(argv[1], "insertNode") == 0) {
      int addr = atoi(argv[2]);

      WossPosition* wpos = dynamic_cast< WossPosition* >( tcl.lookup(argv[3]) );

      if (!wpos) {
        cerr << "ChEstimatorPlugIn::command() insertNode error, you have to pass a WossPosition instance" << endl;
        return TCL_ERROR;
      }
      if (!channel_estimator) {
        return TCL_ERROR;
      }
      channel_estimator->addMacAddress(addr, wpos);
      
      if (debug_) cout << NOW << "  ChEstimatorPlugIn::command() insertNode called, mac addr = " << addr << endl;

      return TCL_OK;
    }
  }  
  
  return PlugIn::command(argc, argv);
}


int ChEstimatorPlugIn::recvSyncClMsg( ClMessage* m ) {

  if ( m->type() == CLMSG_CHANNEL_ESTIMATION ) {
      
    ClMsgChannelEstimation* msg = dynamic_cast< ClMsgChannelEstimation* >( m );
    assert(msg);

    WossPosition* tx = channel_estimator->findMacAddress( msg->getTx() );
    WossPosition* rx = channel_estimator->findMacAddress( msg->getRx() );
    
    woss::CoordZ tx_coordz;
    woss::CoordZ rx_coordz;
    
    if ( tx ) tx_coordz = tx->getLocation();
    if ( rx ) rx_coordz = rx->getLocation();

    if (debug_) std::cout << "ChEstimatorPlugIn::recvSyncClMsg() CLMSG_CHANNEL_ESTIMATION received, tx mac = " << msg->getTx()
                          << "; tx coordZ = " << tx_coordz << "; rx mac = " << msg->getRx() << "; rx coordz = " << rx_coordz << std::endl;

    if ( msg->isQuery() ) {
      if (debug_) std::cout << "ChEstimatorPlugIn::recvSyncClMsg() is a query" << std::endl;

      if ( channel_estimator ) msg->setTimeArr( channel_estimator->getEstimation( tx_coordz, rx_coordz ) );
      else msg->setTimeArr( NULL );        
    }
    else {
      if (debug_) std::cout << "ChEstimatorPlugIn::recvSyncClMsg() is an estimation update. "  << ::std::endl;
      
      if ( channel_estimator ) {
        msg->unsetDeletable();
        if ( msg->getTimeArr() != NULL ) channel_estimator->updateEstimation( tx_coordz, rx_coordz, msg->getTimeArr() );
      }
      else msg->setDeletable();
    }
    return 0;
  }
  if (m->type() == CLMSG_POSITION_GET_DIST) {
  
    ClMsgPositionGetDist* mgd;
    mgd = dynamic_cast<ClMsgPositionGetDist*>(m);
    assert(mgd);
    //std::cerr << __PRETTY_FUNCTION__ << " received CLMSG_POSITION_GET_DIST" << std::endl; 

    WossPosition* tx = channel_estimator->findMacAddress( mgd->getId1() );
    WossPosition* rx = channel_estimator->findMacAddress( mgd->getId2() );
    
    woss::CoordZ tx_coordz;
    woss::CoordZ rx_coordz;
    
    if ( tx ) tx_coordz = tx->getLocation();
    if ( rx ) rx_coordz = rx->getLocation();    
    
    if (debug_) std::cout << "ChEstimatorPlugIn::recvSyncClMsg() CLMSG_POSITION_GET_DIST, tx mac " << mgd->getId1() 
                          << ";tx " << tx_coordz << "; rx mac " << mgd->getId2() << "; rx " << rx_coordz << "; distance = "
                          << tx_coordz.getCartDistance(rx_coordz) << ::std::endl;
    
    mgd->returnDist( tx_coordz.getCartDistance(rx_coordz) );
    
    return 0;
  }  
  return PlugIn::recvSyncClMsg(m);
}


