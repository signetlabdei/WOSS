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
 * @file   uw-woss-random-generator.h
 * @author Federico Guerra
 *
 * \brief  Provides the interface for WossRandomGenerator and WossRandomGeneratorTcl classes
 *
 * Provides the interface for WossRandomGenerator and WossRandomGenerator classes
 */


#ifndef UNDERWATER_WOSS_RANDOM_GENERATOR_H
#define UNDERWATER_WOSS_RANDOM_GENERATOR_H


#include <random-generator-definitions.h>
#include <rng.h>
#include <tclcl.h>


class WossRandomGenerator : public woss::RandomGenerator {
  
  
    public:
      
       
    WossRandomGenerator( int seed = 0 ); 
    
    
    WossRandomGenerator( const WossRandomGenerator& copy );
    
    WossRandomGenerator& operator=( const WossRandomGenerator& copy );
    
    
    virtual ~WossRandomGenerator();
    
    
    virtual WossRandomGenerator* create( int s ) { return new WossRandomGenerator(s); }
    
    virtual WossRandomGenerator* clone() const { return new WossRandomGenerator(*this); }
      
    
    virtual double getRand() const;

    virtual int getRandInt() const;
    
    virtual void initialize();

    
    protected:
      
      
    RNG* rng;
    
    
};


class WossRandomGeneratorTcl : public WossRandomGenerator, public TclObject { 

  
  public:
    
    
  WossRandomGeneratorTcl();

  
  virtual int command(int argc, const char*const* argv);  

  
};


#endif // UNDERWATER_WOSS_RANDOM_GENERATOR_H

