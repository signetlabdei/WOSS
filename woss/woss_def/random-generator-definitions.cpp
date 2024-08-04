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
 * @file   random-generator-definitions.cpp
 * @author Federico Guerra
 *
 * \brief  Implementation of woss::RandomGenerator class 
 *
 * Implementation of woss::RandomGenerator class 
 */


#include <iostream>
#include "random-generator-definitions.h"


using namespace woss;


void RandomGenerator::initialize() {
  if (!initialized) {
    srand(seed);
    initialized = true;
  }
}


double RandomGenerator::getRand() const { 
  if (!initialized) {
    ::std::cerr << "RandomGenerator::getRand() ERROR this instance is not initialized!!" << ::std::endl;
    exit(1);
  }

  return( (double)rand() / (double) RAND_MAX ); 
}


int RandomGenerator::getRandInt() const { 
  if (!initialized) {
    ::std::cerr << "RandomGenerator::getRandInt() ERROR this instance is not initialized!!" << ::std::endl;
    exit(1);
  }
  
  return( rand() ); 
}

