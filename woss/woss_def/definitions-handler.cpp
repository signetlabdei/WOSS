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
 * @file   definitions-handler.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::DefHandler class
 *
 * Provides the implementation of the woss::DefHandler class
 */


#include "sediment-definitions.h"
#include "time-definitions.h"
#include "time-arrival-definitions.h"
#include "ssp-definitions.h"
#include "definitions-handler.h"
#include "transducer-definitions.h"
#include "altimetry-definitions.h"

using namespace woss;


DefHandler::DefHandler()
: debug(false),
  ssp_creator(NULL),
  sediment_creator(NULL),
  pressure_creator(NULL),
  time_arr_creator(NULL),
  time_reference(NULL),
  rand_generator(NULL),
  transducer(NULL),
  altimetry_creator(NULL)
{

}


DefHandler::DefHandler( const DefHandler& copy ) 
: debug(copy.debug),
  ssp_creator(copy.ssp_creator->clone()),
  sediment_creator(copy.sediment_creator->clone()),
  pressure_creator(copy.pressure_creator->clone()),
  time_arr_creator(copy.time_arr_creator->clone()),
  time_reference(copy.time_reference->clone()),
  rand_generator(copy.rand_generator->clone()),
  transducer(copy.transducer->clone()),
  altimetry_creator(copy.altimetry_creator->clone())
{


}


DefHandler& DefHandler::operator=( const DefHandler& copy ) {
  if (this == &copy) return *this;
  debug = copy.debug;
  ssp_creator = copy.ssp_creator->clone();
  sediment_creator = copy.sediment_creator->clone();
  pressure_creator = copy.pressure_creator->clone();
  time_arr_creator = copy.time_arr_creator->clone();
  time_reference = copy.time_reference->clone();
  rand_generator = copy.rand_generator->clone();
  transducer = copy.transducer->clone();
  altimetry_creator = copy.altimetry_creator->clone();
  return( *this );
}
    
    
DefHandler::~DefHandler() {
  delete ssp_creator;
  delete sediment_creator;
  delete pressure_creator;
  delete time_arr_creator;
  delete time_reference;
  delete rand_generator;
  delete transducer;
  delete altimetry_creator;
}


 
