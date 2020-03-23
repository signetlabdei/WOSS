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
 * @file   sediment-deck41-db-logic-control.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::Deck41TypeTests class
 *
 * Provides the implementation of the woss::Deck41TypeTests class
 */

#ifdef WOSS_NETCDF_SUPPORT

#include <iostream>
#include "sediment-deck41-db-logic-control.h"


using namespace woss;


Deck41TypeTests::Deck41TypeTests() 
 : condition_a(false),
   condition_b(false),
   condition_c(false),
   condition_d(false),
   condition_e(false),
   condition_f(false),
   condition_g(false)
{

}


Deck41TypeTests::~Deck41TypeTests() {

}

#endif // WOSS_NETCDF_SUPPORT
