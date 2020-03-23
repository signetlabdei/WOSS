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
 * @file   random-generator-definitions.h
 * @author Federico Guerra
 *
 * \brief  Definitions and library for woss::RandomGenerator class 
 *
 * Definitions and library for woss::RandomGenerator class
 */


#ifndef RANDOM_GENERATOR_DEFINITIONS_H
#define RANDOM_GENERATOR_DEFINITIONS_H 


#include <cstdlib>
#include <cassert>


namespace woss {
  
  
  /**
  * \brief woss::RandomGenerator class
  *
  * Class for random value generation purposes
  **/
  class RandomGenerator {
  
  
    public:
      
    
    /**
    * Default RandomGenerator constructor
    * @param s seed
    **/ 
    RandomGenerator( int s = 0 ) : seed(s), initialized(false) { }
    
    /**
    * Default destructor
    **/    
    virtual ~RandomGenerator() { }
    
    
    /**
    * RandomGenerator virtual factory method
    * @param copy RandomGenerator to be copied
    * @return a heap-allocated RandomGenerator object
    **/
    virtual RandomGenerator* create( double seed ) { return new RandomGenerator(seed); }
         
    /**
    * RandomGenerator virtual factory method
    * @return a heap-allocated RandomGenerator copy of <b>this</b> instance
    **/         
    virtual RandomGenerator* clone() const { return new RandomGenerator(*this); }    
    
    
    /**
    * Checks the validity of RandomGenerator
    * @return <i>true</i> if initialized, <i>false</i> otherwise
    **/
    virtual bool isValid() const { return initialized; }
    
    
    /**
    * Sets the seed
    * @param s seed
    **/ 
    virtual void setSeed( int s ) { seed = s; }
    
    /**
    * Sets the seed
    * @return the seed
    **/ 
    virtual int getSeed() const { return seed; }
    
    
    /**
    * Mandatory function to initialize the instance
    **/ 
    virtual void initialize();
    
    
    /**
    * Gets a random value
    * @return a random value between 0 and 1
    **/    
    virtual double getRand() const;

    /**
     * Gets a random integer value
     * @return a random integer
     **/    
    virtual int getRandInt() const;

    protected:
      
    
    /**
    * seed value
    **/       
    int seed;
    
    
    /**
    * true if initialize() has been called, false otherwise
    **/     
    bool initialized;
    
    
  };
 
  
}


#endif /* RANDOM_GENERATOR_DEFINITIONS_H */

