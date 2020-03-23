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


#ifndef UNDERWATER_WOSS_BPSK_H 
#define UNDERWATER_WOSS_BPSK_H


/**
 * @file   uw-woss-bpsk.h
 * @author Federico Guerra
 *
 * \brief  Provides the interface for WossMPhyBpsk class 
 *
 * Provides the interface for WossMPhyBpsk class
 */


#include <underwater-bpsk.h>


#define UWWOSSMPHYBPSK_MODNAME "UWWOSSBPSK"


namespace woss {

  class Transducer;

}


/**
 * \brief  BPSK modulation class with woss::Transducer tx power control
 *
 * UwMPhyBpskTransducer  extends UnderwaterMPhyBpsk adding transducer power computations capabilities.
 */
class UwMPhyBpskTransducer :  public UnderwaterMPhyBpsk {

  
  public:
    
    
  UwMPhyBpskTransducer();


  virtual ~UwMPhyBpskTransducer() { }
  
  
  virtual int command( int argc, const char*const* argv );

  
  protected:

 
  virtual double consumedEnergyTx( double Ptx, double duration );
  
  
  virtual const woss::Transducer* const getTransducer( double frequency ) const; 
  
  
  typedef ::std::map< ::std::pair< double, double >, const woss::Transducer* > FreqTransducerMap;
  typedef FreqTransducerMap::iterator FTMIter;
  typedef FreqTransducerMap::const_iterator FTMCIter;
  typedef FreqTransducerMap::reverse_iterator FTMRIter;
  typedef FreqTransducerMap::const_reverse_iterator FTMRCIter;
   
 
  FreqTransducerMap freq_transd_map;
  
  
};


/**
 * \brief  BPSK modulation class with woss::Transducer power control
 *
 * WossMPhyBpsk  extends UwMPhyBpskTransducer replacing old tx power control based on distance.
 */
class WossMPhyBpsk :  public UwMPhyBpskTransducer {

  
  public:
    
    
  WossMPhyBpsk();


  virtual ~WossMPhyBpsk() { }
  
   
  protected:

    
  /**
  * It sends a synchronous cross-layer message asking for the current channel estimation. Upon valid answer
  * it provides optimal power calculations.
  *
  * @param p pointer to the current Packet being processed
  * @return Tx power in uPa
  */
  virtual double getTxPower( Packet* p );
 
 
};


#endif /* UNDERWATER_WOSS_BPSK_H */



