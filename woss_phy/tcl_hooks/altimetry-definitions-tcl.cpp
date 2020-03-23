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
 * @file   altimetry-definitions-tcl.cpp
 * @author Federico Guerra
 *
 * \brief  Implementations and library for woss::Altimetry Tcl class 
 *
 * Implementations and library for woss::Altimetry Tcl class
 */



#include "altimetry-definitions-tcl.h"


using namespace woss;


#ifdef WOSS_NS_MIRACLE_SUPPORT
static class AltimetryClass : public TclClass {
public:
  AltimetryClass() : TclClass("WOSS/Definitions/Altimetry/Flat") {}
  TclObject* create(int, const char*const*) {
    return( new AltimetryTcl() );
  }
} class_Altimetry;


static class AltimetryBretschneiderClass : public TclClass {
public:
  AltimetryBretschneiderClass() : TclClass("WOSS/Definitions/Altimetry/Bretschneider") {}
  TclObject* create(int, const char*const*) {
    return( new AltimBretschneiderTcl() );
  }
} class_Altimetry_Bretschneider;


AltimetryTcl::AltimetryTcl()
: Altimetry()
{
  bind("evolution_time_quantum", &evolution_time_quantum); 
  bind("range", &range);
  bind("total_range_steps", &total_range_steps);
  bind("depth", &depth);
  
//   if ( evolution_time_quantum < 0 ) evolution_time_quantum = HUGE_VAL;
}


AltimBretschneiderTcl::AltimBretschneiderTcl()
: AltimBretschneider()
{
  bind("evolution_time_quantum", &evolution_time_quantum); 
  bind("range", &range);
  bind("total_range_steps", &total_range_steps);  
  bind("depth", &depth);
  bind("characteristic_height",&char_height);
  bind("average_period",&average_period);
  
//   if ( evolution_time_quantum < 0 ) evolution_time_quantum = HUGE_VAL;  
}
#endif // WOSS_NS_MIRACLE_SUPPORT
