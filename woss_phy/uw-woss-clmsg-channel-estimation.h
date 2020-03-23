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
 * @file   uw-woss-clmsg-channel-estimation.h
 * @author Federico Guerra
 *
 * \brief  Provides the interface for ClMsgChannelEstimation class 
 *
 * Provides the interface for ClMsgChannelEstimation class
 */


#ifndef UW_WOSS_CLMSG_CHANNEL_ESTIMATION_H
#define UW_WOSS_CLMSG_CHANNEL_ESTIMATION_H


#include <clmessage.h>
// #include <coordinates-definitions.h>

#define CLMSG_CH_ESTIMATION_VERBOSITY 2  // verbosity of this message


extern ClMessage_t CLMSG_CHANNEL_ESTIMATION;


namespace woss {
  class TimeArr;
}


/**
 * \brief Class for channel estimation synchronous cross-layer messaging 
 *
 * ClMsgChannelEstimation provides synchronous cross-layer communication for updating and requests 
 * of channel estimation
 */
class ClMsgChannelEstimation : public ClMessage {
  
  
  public:
    
    
  ClMsgChannelEstimation( int i, int j, woss::TimeArr* time_arr = NULL );
  
  virtual ~ClMsgChannelEstimation();
  
  virtual ClMessage* copy();  // copy the message
  

  const int getTx() { return tx; }
  
  const int getRx() { return rx; }
  
  
  void setTimeArr( woss::TimeArr* time_arr ) { ch_estimation = time_arr; }
  
  woss::TimeArr* getTimeArr() { return ch_estimation; }
  
  
  void setDeletable() { deletable = true; }
  
  void unsetDeletable() { deletable = false; }
  
  
  bool isQuery() {return query; }

  bool isDeletable() { return deletable; }


  protected:
  
    
  bool query;
  
  bool deletable;
  
  int tx;
  
  int rx;

  woss::TimeArr* ch_estimation;

  
};


#endif //UW_WOSS_CLMSG_CHANNEL_ESTIMATION_H

