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
 * @file   sediment-definitions.cpp
 * @author Federico Guerra
 *
 * \brief  Implementation of woss::Sediment class 
 *
 * Implementation of Sediment class
 */


#include "sediment-definitions.h"
#include <sstream>
#include <iostream>
#include <cmath>


using namespace woss;


bool Sediment::debug = false;

::std::stringstream Sediment::sstream;
  
    
Sediment::Sediment() 
: type(),
  depth(SEDIMENT_NOT_SET_VALUE),
  vel_c(SEDIMENT_NOT_SET_VALUE),
  vel_s(SEDIMENT_NOT_SET_VALUE), 
  density(SEDIMENT_NOT_SET_VALUE),
  att_c(SEDIMENT_NOT_SET_VALUE),
  att_s(SEDIMENT_NOT_SET_VALUE) 
{

}

Sediment::Sediment( const ::std::string& name, double velc, double vels, double dens, double attc, double atts, double bottom ) 
: 
  type(name),
  depth(bottom),
  vel_c(velc),
  vel_s(vels),
  density(dens),
  att_c(attc),
  att_s(atts)
{

}


Sediment::Sediment( const Sediment& copy ) {
  type = copy.type;
  vel_c = copy.vel_c;
  vel_s = copy.vel_s;
  density = copy.density;
  att_c = copy.att_c;
  att_s = copy.att_s;
  depth = copy.depth;
}


Sediment& Sediment::operator=( const Sediment& copy ) {
  if (this == &copy) return *this;
  type = copy.type;
  vel_c = copy.vel_c;
  vel_s = copy.vel_s;
  density = copy.density;
  att_c = copy.att_c;
  att_s = copy.att_s;
  depth = copy.depth;
  return *this;
}


const ::std::string Sediment::getStringValues() const {
  Sediment::sstream.str("");
  Sediment::sstream << vel_c << " " << vel_s << " " << density << " " << att_c << " " << att_s;
  return Sediment::sstream.str(); 
}


const Sediment woss::operator/( const double left, const Sediment& right ) {
  Sediment::sstream.str("");
  Sediment::sstream << "( " << right.type << " / " << left << " )";
  return( Sediment( Sediment::sstream.str(), left / right.vel_c, left / right.vel_s, left / right.density, left / right.att_c, left / right.att_s ) );
}


const Sediment woss::operator*( const double left, const Sediment& right ) {
  Sediment::sstream.str("");
  Sediment::sstream << "( " << right.type << " * " << left << " )";
  return( Sediment( Sediment::sstream.str(), left * right.vel_c, left * right.vel_s, left * right.density, left * right.att_c, left * right.att_s ) );
}


const Sediment woss::operator/( const Sediment& left, const double right ) {
  Sediment::sstream.str("");
  Sediment::sstream << "( " << left.type << " / " << right << " )";
  return( Sediment( Sediment::sstream.str(), right / left.vel_c, right / left.vel_s, right / left.density, right / left.att_c, right / left.att_s ) );
}


const Sediment woss::operator*( const Sediment& left, const double right ) {
  Sediment::sstream.str("");  
  Sediment::sstream << "( " << left.type << " * " << right << " )";
  return( Sediment( Sediment::sstream.str(), right * left.vel_c, right * left.vel_s, right * left.density, right * left.att_c, right * left.att_s ) );
}


Sediment& woss::operator+=( Sediment& left, const Sediment& right ) {
//   if (&left == &right) { /* self assignment code here */ }
  left.type += " + " + right.type;
  left.vel_c += right.vel_c;
  left.vel_s += right.vel_s;
  left.density += right.density;
  left.att_c += right.att_c;
  left.att_s += right.att_s;
  return left;
}


Sediment& woss::operator-=( Sediment& left, const Sediment& right ) {
//   if (&left == &right) { /* self assignment code here */ }
  left.type += " - " + right.type;
  left.vel_c -= right.vel_c;
  left.vel_s -= right.vel_s;
  left.density -= right.density;
  left.att_c -= right.att_c;
  left.att_s -= right.att_s;  
  return left;
}


Sediment& woss::operator/=( Sediment& left, const Sediment& right ) {
//   if (&left == &right) { /* self assignment code here */ }
  left.type += " / " + right.type;
  left.vel_c /= right.vel_c;
  left.vel_s /= right.vel_s;
  left.density /= right.density;
  left.att_c /= right.att_c;
  left.att_s /= right.att_s;
  return left;
}


Sediment& woss::operator*=( Sediment& left, const Sediment& right ) {
//   if (&left == &right) { /* self assignment code here */ }
  left.type += " * " + right.type;
  left.vel_c *= right.vel_c;
  left.vel_s *= right.vel_s;
  left.density *= right.density;
  left.att_c *= right.att_c;
  left.att_s *= right.att_s;
  return left;
}



Sediment& woss::operator+=( Sediment& left, double right ) {
  Sediment::sstream.str("");  
  Sediment::sstream << left.type << " + " << right;
  
  left.type = Sediment::sstream.str();
  left.vel_c += right;
  left.vel_s += right;
  left.density += right;
  left.att_c += right;
  left.att_s += right;
  return left;
}


Sediment& woss::operator-=( Sediment& left, double right ) {
  Sediment::sstream.str("");  
  Sediment::sstream << left.type << " - " << right;
  
  left.type = Sediment::sstream.str();
  left.vel_c -= right;
  left.vel_s -= right;
  left.density -= right;
  left.att_c -= right;
  left.att_s -= right;  
  return left;
}


Sediment& woss::operator/=( Sediment& left, double right ) {
  Sediment::sstream.str("");  
  Sediment::sstream << left.type << " / " << right;
  
  left.type = Sediment::sstream.str();
  left.vel_c /= right;
  left.vel_s /= right;
  left.density /= right;
  left.att_c /= right;
  left.att_s /= right;
  return left;
}


Sediment& woss::operator*=( Sediment& left, double right ) {
  Sediment::sstream.str("");  
  Sediment::sstream << left.type << " * " << right;
  
  left.type = Sediment::sstream.str();
  left.vel_c *= right;
  left.vel_s *= right;
  left.density *= right;
  left.att_c *= right;
  left.att_s *= right;
  return left;
}


SedimentGravel::SedimentGravel(double bottom_depth) 
: Sediment( "GRAVEL", 1800.0, 180.0, 2.0, 0.6, 1.5, bottom_depth)
{
  vel_s = calculateVelocityS(180.0, bottom_depth);
}


double SedimentGravel::calculateVelocityS(double vels, double bottom_depth) {
  if ( bottom_depth <= 0.0 ) return vels;
  return ( vels * pow( bottom_depth, 0.3) ); 
}


SedimentSilt::SedimentSilt(double bottom_depth) 
: Sediment( "SILT", 1575.0, 80.0, 1.7, 1.0, 1.5, bottom_depth)
{
  vel_s = calculateVelocityS(80.0, bottom_depth);
}


double SedimentSilt::calculateVelocityS(double vels, double bottom_depth) {
  if ( bottom_depth <= 0.0 ) return vels;
  return ( vels * pow( bottom_depth, 0.3) ); 
}


SedimentMud::SedimentMud(double bottom_depth) 
: Sediment( "MUD", 1540.0, 70.0, 1.6, 0.8, 1.3, bottom_depth)
{
  vel_s = calculateVelocityS(70.0, bottom_depth);
}


double SedimentMud::calculateVelocityS(double vels, double bottom_depth) {
  if ( bottom_depth <= 0.0 ) return vels;
  return ( vels * pow( bottom_depth, 0.3) ); 
}


SedimentSand::SedimentSand() 
: Sediment( "SAND", 1650.0, 110.0, 1.9, 0.8, 2.5)
{

}


SedimentClay::SedimentClay() 
: Sediment( "CLAY", 1510.0, 95.0, 1.51, 0.17, 1.0)
{

}


SedimentOoze::SedimentOoze() 
: Sediment( "OOZE", 1560.0, 95.0, 1.6, 0.2, 0)
{

}


SedimentRocks::SedimentRocks() 
: Sediment( "CHALK", 2400.0, 1000.0, 2.2, 0.1, 0.2)
{

}

// missing data
SedimentOrganic::SedimentOrganic() 
: Sediment( "ORGANIC", 0, 0, 0, 0, 0)
{

}


SedimentNodules::SedimentNodules() 
: Sediment( "LIMESTONE", 3000.0, 1500.0, 2.4, 0.1, 0.2)
{

}


SedimentHardBottom::SedimentHardBottom() 
: Sediment( "CLAY", 5250.0, 2500.0, 3.5, 0.1, 0.2)
{

}

 
