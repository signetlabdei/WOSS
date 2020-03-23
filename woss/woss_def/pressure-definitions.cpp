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
 * @file   pressure-definitions.cpp
 * @author Federico Guerra
 *
 * \brief  Implementation of woss::Pressure class
 *
 * Implementation of woss::Pressure class
 */


#include <iostream>
#include "pressure-definitions.h"
#include "time-arrival-definitions.h"


using namespace woss;


bool Pressure::debug = false;


Pressure::Pressure( double real, double imag ) 
: complex_pressure( real, imag )
{
  
}
  
  
Pressure::Pressure( const std::complex<double>& press ) 
: complex_pressure( press )
{

}


Pressure::Pressure( const Pressure& copy ) {
  complex_pressure = copy.complex_pressure;
}


Pressure::Pressure( const TimeArr& time_arr ) {
  if ( !time_arr.isValid() ) complex_pressure = Pressure::createNotValid();
  
  for ( TimeArrCIt it = time_arr.begin(); it != time_arr.end(); it++ ) {
    complex_pressure += it->second;
  }
}


Pressure& Pressure::operator=( const Pressure& press ) { 
  if (this == &press) return *this;
  complex_pressure = press.complex_pressure;
  return *this;
}

  
bool Pressure::checkAttenuation( double distance, double frequency ) { 
  if ( std::abs( complex_pressure ) > 1.0 ) {
    double amplitude = std::abs( complex_pressure ); 
    double phase = arg( complex_pressure );
    
    amplitude = pow( 10.0, ( getAttenuation( distance, frequency ) / -20.0 ) );
    complex_pressure = std::complex<double>( (amplitude * cos(phase)), (amplitude * sin(phase)) );
    
    if (debug) std::cerr << "Pressure::checkAttenuation() distance = " << distance << "; frequency = " << frequency 
                         << "; amplitude > 1, new pressure = " << complex_pressure << "; tx loss db = " << -20.0*log10( std::abs( complex_pressure ) ) << std::endl;
    
    return true;
  }
  return false;
}


double Pressure::getAttenuation( double distance, double frequency ) {
  double k = 1.5; // practical spreading
  
  if (distance > 0) {
    double att = (k * 10.0 * log10( distance ) + distance * getThorpAtt( frequency ) );
    if (att > 1.0) return att;
    else return 1.0;
  }
  else return 1.0;
}


double Pressure::getThorpAtt( double frequency ) { 
  frequency /= 1000.0; // kHz
  double f2 = pow( frequency, 2.0 );
  double atten;
  
  if( frequency > 0.4 ) atten = 0.11 * f2 / (1.0 + f2) + 44.0 * (f2 / (4100.0 + f2)) + 2.75e-4 * f2 + 0.003;
  else atten = 0.002 + 0.11 * (f2 / (1.0 + f2)) + 0.011 * f2;

  return atten/1000.0; // db re u Pa / m
}

   
