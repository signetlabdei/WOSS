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
 * @file   uw-woss-random-generator.cpp
 * @author Federico Guerra
 *
 * \brief  Provides the implementation of WossRandomGenerator class 
 *
 * Provides the implementation of WossRandomGenerator class
 */


#include <uw-woss-random-generator.h>
#include <iostream>


WossRandomGenerator::WossRandomGenerator( int seed ) 
: RandomGenerator(seed),
  rng(std::make_unique<RNG>()) 
{ 
}

WossRandomGenerator::WossRandomGenerator( const WossRandomGenerator& copy )
: RandomGenerator(copy),
  rng(std::make_unique<RNG>(*(copy.rng)))
{
  initialized = copy.initialized;
}

WossRandomGenerator& WossRandomGenerator::operator=( const WossRandomGenerator& copy ) {
  if (this == &copy) 
    return *this;
  seed = copy.seed;
  rng = std::make_unique<RNG>(*(copy.rng));
  initialized = copy.initialized;
  return *this;
}

void WossRandomGenerator::initialize() {
  if (!initialized) {
    for ( int i = 0; i < seed; i++ ) {
      rng->reset_next_substream();
    }

    initialized = true;
  }
}


double WossRandomGenerator::getRand() const {
  if (!initialized) {
    std::cerr << "WossRandomGenerator::getRand() WARNING RNG object is not initialized!!" << std::endl;
    exit(1);
  }
  return rng->uniform_double(); 
}


int WossRandomGenerator::getRandInt() const {
  if (!initialized) {
    std::cerr << "WossRandomGenerator::getRandInt() WARNING RNG object is not initialized!!" << std::endl;
    exit(1);
  }
  return rng->uniform_positive_int(); 
}


WossRandomGeneratorTcl::WossRandomGeneratorTcl() 
: WossRandomGenerator()
{
  bind( "rep_number_", &seed);
  setSeed(seed);
}


static class WossRandomGeneratorTclClass : public TclClass {
public:
  WossRandomGeneratorTclClass() : TclClass("WOSS/Definitions/RandomGenerator/NS2") {}
  TclObject* create(int, const char*const*) {
    return (new WossRandomGeneratorTcl());
  }
} class_WossRandomGeneratorTcl;


int WossRandomGeneratorTcl::command(int argc, const char*const* argv) {
  if (argc == 2) {
    if(strcasecmp(argv[1], "initialize") == 0) {
      initialize();
      return TCL_OK;
    }
  }
  return( TclObject::command(argc, argv) );
}
