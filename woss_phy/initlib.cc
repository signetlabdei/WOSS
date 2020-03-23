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

#include <tclcl.h>
#include "uw-woss-pkt-hdr.h"
#include "uw-woss-clmsg-channel-estimation.h"


extern EmbeddedTcl WossPhyInitTclCode;
extern ClMessage_t CLMSG_CHANNEL_ESTIMATION;

int hdr_woss::offset_ = 0;


static class WOSSPktClass : public PacketHeaderClass {
public:
  WOSSPktClass() : PacketHeaderClass("PacketHeader/WOSS", sizeof(hdr_woss)) {
    bind_offset(&hdr_woss::offset_);
  }
} class_woss_pkt; 


extern "C" int Wossphy_Init() {
  WOSSPktClass* woss_header_class = new WOSSPktClass;  // <---
  woss_header_class->bind();	
  
  CLMSG_CHANNEL_ESTIMATION = ClMessage::addClMessage();
  
  WossPhyInitTclCode.load();
  return 0;
}


extern "C" int Cygwossphy_Init() {
  return Wossphy_Init();
}
