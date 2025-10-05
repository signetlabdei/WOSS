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
 * @file   uw-woss-clmsg-channel-estimation.h
 * @author Federico Guerra
 *
 * \brief  Provides the interface for ClMsgChannelEstimation class 
 *
 * Provides the interface for ClMsgChannelEstimation class
 */


#ifndef UW_WOSS_CLMSG_CHANNEL_ESTIMATION_H
#define UW_WOSS_CLMSG_CHANNEL_ESTIMATION_H

#include <memory>
#include <clmessage.h>
#include <time-arrival-definitions.h>


static constexpr inline int CLMSG_CH_ESTIMATION_VERBOSITY = 2;  // verbosity of this message


extern ClMessage_t CLMSG_CHANNEL_ESTIMATION;

/**
 * \brief Class for channel estimation synchronous cross-layer messaging 
 *
 * ClMsgChannelEstimation provides synchronous cross-layer communication for updating and requests 
 * of channel estimation
 */
class ClMsgChannelEstimation : public ClMessage {

  public:

  ClMsgChannelEstimation( int i, int j, std::unique_ptr<woss::TimeArr> time_arr = nullptr );
  
  virtual ~ClMsgChannelEstimation() override = default;
  
  virtual ClMessage* copy() override;  // copy the message
  

  int getTx() const { return tx; }
  
  int getRx() const { return rx; }
  
  
  void setTimeArr( std::unique_ptr<woss::TimeArr> time_arr ) { ch_estimation = std::move(time_arr); }
  
  std::unique_ptr<woss::TimeArr> getTimeArr() { return std::move(ch_estimation); }


  bool isQuery() const {return query; }


  protected:
  
    
  bool query;
   
  int tx;
  
  int rx;

  std::unique_ptr<woss::TimeArr> ch_estimation;

};

#endif //UW_WOSS_CLMSG_CHANNEL_ESTIMATION_H
