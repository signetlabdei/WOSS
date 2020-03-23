/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2009 Regents of Patavina Technologies 
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
 * This software has been developed by Patavina Technologies, s.r.l., 
 * in collaboration with the NATO Undersea Research Centre 
 * (http://www.nurc.nato.int; E-mail: pao@nurc.nato.int), 
 * whose support is gratefully acknowledged.
 *//* WOSS - World Ocean Simulation System -
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
 * @file   res-pressure-bin-db-creator.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::ResPressureBinDbCreator class
 *
 * Provides the implementation of the woss::ResPressureBinDbCreator class
 */


#include <cassert>
#include "res-pressure-bin-db-creator.h"
#include "res-pressure-bin-db.h"


using namespace woss;


ResPressureBinDbCreator::ResPressureBinDbCreator()
: space_sampling(0.0)
{

}


ResPressureBinDbCreator::~ResPressureBinDbCreator() {

}
  

WossDb* const ResPressureBinDbCreator::createWossDb() {
  assert( pathname.length() > 0 );
  
  ResPressureTxtDb* woss_db = new ResPressureBinDb( pathname );
  
  woss_db->setSpaceSampling(space_sampling);
  bool ok = initializeDb( woss_db );  
  assert( ok );
  
  return( woss_db );
}


bool ResPressureBinDbCreator::initializeDb( WossDb* const woss_db ) {
  return( WossDbCreator::initializeDb( woss_db ) );
}



