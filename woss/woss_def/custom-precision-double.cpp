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
 * @file   custom-precision-double.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation for the woss::PDouble class and friend functions
 *
 * Provides the implementation for the PDouble class and friend functions
 */


#include "custom-precision-double.h"

using namespace woss;

bool PDouble::debug = false;


PDouble::PDouble( const long double input, const long double prec )
: value(input), 
  precision(prec)
{

}


PDouble::PDouble( const PDouble& copy )
{
  precision = copy.precision;
  value = copy.value;
}


PDouble::~PDouble() 
{

}


PDouble& PDouble::operator=( const PDouble& copy )
{
  if (this == &copy) return *this;
  value = copy.value;
  precision = copy.precision;
  return *this;
}
