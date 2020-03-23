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
 * @file   uw-woss-pkt-hdr.h
 * @author Federico Guerra
 *
 * \brief  Definition of hdr_woss, WOSS pkt header
 *
 * Definition of hdr_woss, WOSS pkt header
 */


#ifndef UW_WOSS_PKT_H 
#define UW_WOSS_PKT_H


#define HDR_WOSS(P)      (hdr_woss::access(P))


#include <packet.h>
#include <complex>


/**
 * \brief WOSS packet header
 *
 * struct hdr_woss extends Miracle MPhy header adding new information
 */
typedef struct hdr_woss {


  /**
  * complex attenuation of current Packet, calculated by woss::WossManager
  */
  std::complex<double> attenuation;
  
  /**
  * frequency of calculations
  */
  double frequency;
  
  /**
  * bool that informs WossMPropagation that WossChannelModule has already processed this Packet
  */
  bool already_processed;

  static int offset_;
  inline static int& offset() { return offset_; }
  inline static struct hdr_woss* access(const Packet* p) {
    return (struct hdr_woss*)p->access(offset_);
  }
  
} hdr_woss;


#endif /* UW_WOSS_PKT_H */
