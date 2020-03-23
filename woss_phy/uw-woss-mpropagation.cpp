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
 * @file   uw-woss-mpropagation.cpp
 * @author Federico Guerra
 *
 * \brief  Provides the implementation of WossMPropagation class 
 *
 * Provides the implementation of WossMPropagation class
 */


#include "uw-woss-position.h"
#include <mpropagation.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <rng.h>
#include "uw-woss-mpropagation.h"
#include <woss-manager.h>


#define WOSS_MPROP_WRITE_GAIN_MARGIN (4)


using namespace woss;


static class WossMPropagationClass : public TclClass {
public:
  WossMPropagationClass() : TclClass("WOSS/MPropagation") {}
  TclObject* create(int, const char*const*) {
    return (new WossMPropagation);
  }
} class_WossMPropagation;


WossMPropagation::WossMPropagation() 
 : woss_manager(NULL),
   write_gain_matrix(false)
{

}


int WossMPropagation::command(int argc, const char*const* argv) {
  Tcl& tcl = Tcl::instance();

  if (argc==3) {
    if(strcasecmp(argv[1], "setwriteStdGainMatrix") == 0) {
      int ret_val = 0;

      if (debug_) cout << NOW << "  WossMPropagation::command() setwriteStdGainMatrix called"  << endl;

      std_gain_matrix_name = argv[2] ;

      write_gain_matrix = true;

      string command = "rm -f " + std_gain_matrix_name;
      if (system(NULL)) {
        ret_val = system(command.c_str());
      }
      command = "touch " + std_gain_matrix_name;
      if (system(NULL)) {
        ret_val = system(command.c_str());
      }

      if (debug_) cout << NOW << "  WossMPropagation::command() setwriteStdGainMatrix ret_val = " << ret_val  << endl;

      return TCL_OK;
    }
    if(strcasecmp(argv[1], "setWossManager") == 0) {
      if (debug_) cout << NOW << "  WossMPropagation::command() setWossManager called"  << endl;
      woss_manager = dynamic_cast< woss::WossManager* >( tcl.lookup(argv[2]) );

      if (woss_manager) return TCL_OK;
      else return TCL_ERROR;
    }
  }
  else if (argc == 2) {
    if(strcasecmp(argv[1], "writeStdGainMatrix") == 0) {
      if (debug_) cout << NOW << "  WossMPropagation::command() writeStdGainMatrix called"  << endl;

      if (write_gain_matrix == true) writeStdGainMatrix();
      return TCL_OK;
    }
  }
  return( UnderwaterMPropagation::command(argc, argv) );
}


double WossMPropagation::getGain(Packet* p) {
   hdr_woss* hdr_woss = HDR_WOSS(p);

   // processed by woss_channel
   if (hdr_woss->already_processed == true) {

     double gain = pow( abs( hdr_woss->attenuation ), 2.0 );

     hdr_MPhy* ph = HDR_MPHY(p);
     WossPosition* sp = dynamic_cast< WossPosition* >( ph->srcPosition );
     WossPosition* rp = dynamic_cast< WossPosition* >( ph->dstPosition );
     
     assert(sp);
     assert(rp);
     
     if (write_gain_matrix == true) {
        double curr_f0 = ph->srcSpectralMask->getFreq();
        double curr_bw = ph->srcSpectralMask->getBandwidth();
        double curr_f1 = curr_f0 + curr_bw/2.0;
        double curr_f2 = curr_f0 - curr_bw/2.0;
      
        assert(curr_f1 > 0);
        assert(curr_f2 > 0);
      
        double curr_freq = sqrt( curr_f1 * curr_f2 );
        
        double curr_dist = CoordZ( sp->getLatitude(), sp->getLongitude(), ::std::abs(sp->getAltitude()) ).getCartDistance( CoordZ( rp->getLatitude(), rp->getLongitude(), ::std::abs(rp->getAltitude()) ) ) ;
        //double k = 1.75; // practical spreading
        double std_gain_2 = uw.getAttenuation( curr_dist, curr_freq/1000.0 );
       
        insertStdGainMatrix(sp, sp, 0);
        insertStdGainMatrix(sp, rp, std_gain_2 );
     }

     if(debug_) cout << NOW 
	                   << "  WossMPropagation::getGain() packet already processed by UwWossChannel, " 
	                   << "attenuation db = " << 10.0 * abs(log10(gain))
	                   << endl;
//      if (debug_) debugWaitForUser();

     return(gain);
   }

   return computeGain(p);
}


double WossMPropagation::computeGain(Packet* p)  {
  if (!woss_manager) {
    cout << "WossMPropagation::computeGain() ERROR, setWossManager not called!" << endl;
    exit(1);
  }

  hdr_MPhy* ph = HDR_MPHY(p);
  hdr_woss* hdr_woss = HDR_WOSS(p);

  WossPosition* sp = dynamic_cast< WossPosition* >( ph->srcPosition );
  WossPosition* rp = dynamic_cast< WossPosition* >( ph->dstPosition );
  
  assert(sp);
  assert(rp);

  MSpectralMask* sm = ph->srcSpectralMask;

  assert(sp);
  assert(rp);
  assert(sm);

  double curr_f0 = ph->srcSpectralMask->getFreq();
  double curr_bw = ph->srcSpectralMask->getBandwidth();
  double curr_f1 = curr_f0 + curr_bw/2.0;
  double curr_f2 = curr_f0 - curr_bw/2.0;

  assert(curr_f1 > 0);
  assert(curr_f2 > 0);

  double curr_freq = sqrt(curr_f1*curr_f2);

  CoordZ tx_coordz ( sp->getLatitude(), sp->getLongitude(), ::std::abs(sp->getAltitude()) );
  CoordZ rx_coordz ( rp->getLatitude(), rp->getLongitude(), ::std::abs(rp->getAltitude()) );
  double curr_dist = tx_coordz.getCartDistance( rx_coordz ) ;

  Pressure* pressure = woss_manager->getWossPressure( tx_coordz, rx_coordz, curr_freq, curr_freq, NOW );

  if ( write_gain_matrix == true ) {
//    double k = 1.75; // practical spreading
    double std_gain_2 = uw.getAttenuation(curr_dist*1000.0, curr_freq/1000.0);

    insertStdGainMatrix(sp, sp, 0);
    insertStdGainMatrix(sp, rp, std_gain_2 );
  }

  pressure->checkAttenuation( curr_dist, curr_freq );
  hdr_woss->already_processed = true;
  hdr_woss->attenuation = *pressure;
  hdr_woss->frequency = curr_freq;

  if(debug_) cout << NOW 
              << "  WossMPropagation::computeGain()" 
              << " dist = " << curr_dist
              << "; freq = " << curr_freq
              << "; atten_db = " << Pressure::getTxLossDb(hdr_woss->attenuation)
              << endl;

  delete pressure;
  return( pow( pressure->abs(), 2.0 ) );
}


void WossMPropagation::writeStdGainMatrix() {

  stringstream s_out;

  if (std_gain_matrix_name.length() == 0) {
     cout << NOW << "  WossMPropagation::writeStdGainMatrix() error, command setwriteStdGainMatrix not called" << endl;
     exit(1);
  }

  std_gain_out.open(std_gain_matrix_name.c_str());

  if ( !std_gain_out ) {
     cout << NOW << "  WossMPropagation::printStdGainMatrix() error(s) in command setPrintGainMatrix" << endl;
     exit(1);
  }

  s_out << ((std_gain_map.begin())->first).getLatitude() << ";" << ((std_gain_map.begin())->first).getLongitude()
        << ";" << ((std_gain_map.begin())->first).getDepth() << " : ";
  int size = (s_out.str().length());
  s_out.str("");

  std_gain_out << setw(size) << right << " : " ;
  for ( GMIter it = std_gain_map.begin(); it != std_gain_map.end(); it++) {
      s_out << it->first.getLatitude() << ";" << it->first.getLongitude() << ";" << it->first.getDepth();
      string coords = s_out.str();
      s_out.str("");

      std_gain_out << setw(size) << left << coords ;
  }
  std_gain_out << endl;

  for ( GMIter it = std_gain_map.begin(); it != std_gain_map.end(); it++) {
      std_gain_out << it->first.getLatitude() << ";" << it->first.getLongitude() << ";" << it->first.getDepth() << " : " ;

      for ( map< CoordZ, double >::iterator it2 = (it->second).begin(); it2 != (it->second).end(); it2++) {
           std_gain_out << setw(size) << left <<  it2->second ;
      }
      std_gain_out << endl;
  }
  std_gain_out.close();
}




