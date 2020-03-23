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
 * @file   uw-woss-cbr.h
 * @author Federico Guerra
 *
 * \brief  Provides the interface for WossCbrModule class 
 *
 * Provides the interface for WossCbrModule class
 */


#ifndef UNDERWATER_WOSS_CBR_H 
#define UNDERWATER_WOSS_CBR_H


#include <cbr-module.h>


class WossCbrModule :  public CbrModule {
  
  
  public:
  
    
  WossCbrModule();
  
  virtual ~WossCbrModule() { }
  
  
  virtual void recv(Packet*);
  
  
  virtual int command(int argc, const char*const* argv);  

  
  protected:
  
    
  double first_time_rx;
  
  double last_time_rx;

  
  void updateFirstTimeRx(double time);
  
  void updateLastTimeRx(double time);
  
  
  double getFirstTimeRx(){ return first_time_rx; }
  
  double getLastTimeRx() { return last_time_rx; }


};


#endif /* UNDERWATER_WOSS_CBR_H */




