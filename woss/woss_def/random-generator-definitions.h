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
 * @file   random-generator-definitions.h
 * @author Federico Guerra
 *
 * \brief  Definitions and library for woss::RandomGenerator class 
 *
 * Definitions and library for woss::RandomGenerator class
 */


#ifndef RANDOM_GENERATOR_DEFINITIONS_H
#define RANDOM_GENERATOR_DEFINITIONS_H 


#include <memory>

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

    virtual ~RandomGenerator() = default;

    RandomGenerator(const RandomGenerator& copy) = default;

    RandomGenerator(RandomGenerator&& tmp) = default;
    
    RandomGenerator& operator=(const RandomGenerator& copy) = default;

    RandomGenerator& operator=(RandomGenerator&& tmp) = default;
    
    /**
    * RandomGenerator virtual factory method
    * @param copy RandomGenerator to be copied
    * @return a std::unique_ptr to a heap-allocated RandomGenerator object
    **/
    virtual std::unique_ptr<RandomGenerator> create( int seed ) const { return std::make_unique<RandomGenerator>(seed); }

    /**
    * RandomGenerator virtual factory method
    * @return std::unique_ptr to a heap-allocated RandomGenerator copy of <b>this</b> instance
    **/
    virtual std::unique_ptr<RandomGenerator> clone() const { return std::make_unique<RandomGenerator>(*this); }

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
    int seed = 0;
    /**
    * true if initialize() has been called, false otherwise
    **/
    bool initialized = false;

  };

}

#endif /* RANDOM_GENERATOR_DEFINITIONS_H */
