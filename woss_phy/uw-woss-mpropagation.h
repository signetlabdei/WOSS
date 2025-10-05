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
 * @file   uw-woss-mpropagation.h
 * @author Federico Guerra
 *
 * \brief  Provides the interface for WossMPropagation class 
 *
 * Provides the interface for WossMPropagation class
 */


#ifndef UNDERWATER_WOSS_PROPAGATION_H 
#define UNDERWATER_WOSS_PROPAGATION_H


#include <fstream>
#include <map>
#include <underwater-mpropagation.h>
#include <underwater.h>
#include <mphy.h>
#include <uw-woss-pkt-hdr.h>
#include <coordinates-definitions.h>


namespace woss {
  class WossManager;
}


using GainMatrix = std::map< woss::CoordZ , std::map < woss::CoordZ , double > >;
using GMIter = GainMatrix::iterator;
using GMRIter = GainMatrix::reverse_iterator;


/**
 * \brief  UnderwaterMPropagation class for channel calculations with WOSS
 *
 * WossMPropagation extends UnderwaterMPropagation for channel calculations with WOSS
 */
class WossMPropagation : public UnderwaterMPropagation {  

  public:

  WossMPropagation();
  
  virtual ~WossMPropagation() override = default;

  /**
  * Checks if a WossChannelModule has already processed current Packet; if not, it provides calculations
  *
  * @param p pointer to the current Packet being processed
  * @return gain in uPa
  */
  virtual double getGain(Packet* p) override;

  virtual int command(int argc, const char*const* argv) override;

  private:
  
  // keep raw pointer since we are dependant on legacy TCL memory management
  woss::WossManager* woss_manager;
  
  GainMatrix std_gain_map;
  
  bool write_gain_matrix;

  fstream std_gain_out;

  string std_gain_matrix_name;

  
  void insertStdGainMatrix( Position* sp, Position* rp, double gain );
 
  
  /**
  * Provides attenuation calculation with WOSS framework
  *
  * @param p pointer to the current Packet being processed
  * @return gain in uPa
  */
  double computeGain(Packet* p);

  void writeStdGainMatrix();

};


//inline functions
///////////

inline void WossMPropagation::insertStdGainMatrix( Position* sp, Position* rp, double gain ) {
  std_gain_map[ woss::CoordZ( sp->getLatitude(), sp->getLongitude(), abs( sp->getZ() ) ) ]
              [ woss::CoordZ( rp->getLatitude(), rp->getLongitude(), abs( rp->getZ() ) ) ] = gain;
}

#endif /* UNDERWATER_WOSS_PROPAGATION_H */
