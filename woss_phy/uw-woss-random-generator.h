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
 * @file   uw-woss-random-generator.h
 * @author Federico Guerra
 *
 * \brief  Provides the interface for WossRandomGenerator and WossRandomGeneratorTcl classes
 *
 * Provides the interface for WossRandomGenerator and WossRandomGenerator classes
 */


#ifndef UNDERWATER_WOSS_RANDOM_GENERATOR_H
#define UNDERWATER_WOSS_RANDOM_GENERATOR_H

#include <memory>
#include <random-generator-definitions.h>
#include <rng.h>
#include <tclcl.h>


class WossRandomGenerator : public woss::RandomGenerator {
  
  
    public:
      
       
    WossRandomGenerator( int seed = 0 ); 
    
    
    WossRandomGenerator( const WossRandomGenerator& copy );

    WossRandomGenerator( WossRandomGenerator&& tmp ) = default;
    
    WossRandomGenerator& operator=( const WossRandomGenerator& copy );

    WossRandomGenerator& operator=( WossRandomGenerator&& tmp ) = default;

    
    virtual ~WossRandomGenerator() override = default;
    
    
    virtual std::unique_ptr<woss::RandomGenerator> create( int s ) const override { return std::make_unique<WossRandomGenerator>(s); }
    
    virtual std::unique_ptr<woss::RandomGenerator> clone() const override { return std::make_unique<WossRandomGenerator>(*this); }

    virtual double getRand() const;

    virtual int getRandInt() const;
    
    virtual void initialize();

    
    protected:

    std::unique_ptr<RNG> rng = nullptr;

};


class WossRandomGeneratorTcl : public WossRandomGenerator, public TclObject { 

  
  public:
    
    
  WossRandomGeneratorTcl();

  virtual ~WossRandomGeneratorTcl() override = default;

  
  virtual int command(int argc, const char*const* argv) override;

  
};


#endif // UNDERWATER_WOSS_RANDOM_GENERATOR_H

