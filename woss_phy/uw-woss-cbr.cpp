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
 * @file   uw-woss-cbr.cpp
 * @author Federico Guerra
 *
 * \brief  Provides the implementation of WossCbrModule class 
 *
 * Provides the implementation of WossCbrModule class 
 */


#include"uw-woss-cbr.h"
#include <iostream>
#include <cmath>
#include <mphy.h>
#include <ip.h>
#include <rng.h>


static class WossCbrModuleClass : public TclClass {
public:
  WossCbrModuleClass() : TclClass("WOSS/Module/CBR") {}
  TclObject* create(int, const char*const*) {
    return (new WossCbrModule);
  }
} class_WossCbrModule;


WossCbrModule::WossCbrModule()
: first_time_rx(HUGE_VAL),
  last_time_rx(0)
{ 

}

void WossCbrModule::updateFirstTimeRx(double time) { 
  first_time_rx = min(time, first_time_rx); 
}

void WossCbrModule::updateLastTimeRx(double time) { 
  last_time_rx = max(time, last_time_rx); 
}

int WossCbrModule::command(int argc, const char*const* argv)
{
//   printf("MPhy::command -- %s (%d)\n", argv[1], argc);
  Tcl& tcl = Tcl::instance();

  if(argc == 2) {

      if(strcasecmp(argv[1], "getFirstTimeRx")==0) {

        if (debug_) cout << NOW << "  WossCbrModule::command() getFirstTimeRx called" << endl;
 
	tcl.resultf("%f",getFirstTimeRx());
        return TCL_OK;
      }
      else if(strcasecmp(argv[1], "getLastTimeRx")==0) {

        if (debug_) cout << NOW << "  WossCbrModule::command() getLastTimeRx called" << endl;
 
	tcl.resultf("%f",getLastTimeRx());
        return TCL_OK;
      }
  }
  
  return(CbrModule::command(argc, argv));
}

void WossCbrModule::recv(Packet* p)
{
  hdr_cmn* ch = hdr_cmn::access(p);
  if (debug_>10)     
    printf("WossCbrModule(%d)::recv(Packet*p,Handler*) pktId %d\n",getId(), ch->uid());
    
  if (ch->ptype() != PT_MCBR)
    {
      drop(p,1,CBR_DROP_REASON_UNKNOWN_TYPE);
      incrPktInvalid();
      return;
    }

  hdr_ip* iph = hdr_ip::access(p);
  if (iph->saddr() != dstAddr_)
    {
      drop(p,1,CBR_DROP_REASON_WRONG_SADDR);
      incrPktInvalid();
      return;
    }
  if (iph->sport() != dstPort_)
    {
      drop(p,1,CBR_DROP_REASON_WRONG_SPORT);
      incrPktInvalid();
      return;
    }

  hdr_cbr* cbrh = HDR_CBR(p);

  int esn = hrsn + 1; // expected sn
    
  if (cbrh->sn < esn)
    { // packet is out of sequence and is to be discarded
      incrPktOoseq();
      if (debug_ > 1)
	printf("CbrModule::recv() Pkt out of sequence! cbrh->sn=%d\thrsn=%d\tesn=%d\n",cbrh->sn,hrsn,esn);
      drop(p,1,CBR_DROP_REASON_OUT_OF_SEQUENCE);
      return;
    }

  hdr_MPhy* ph = HDR_MPHY(p);

  updateFirstTimeRx(ph->rxtime);
  updateLastTimeRx(ph->rxtime);

  return(CbrModule::recv(p));
}
