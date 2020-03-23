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
 * @file   ssp-woa2005-db-creator.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::SspWoa2005DbCreator class
 *
 * Provides the implementation of the woss::SspWoa2005DbCreator class
 */


#ifdef WOSS_NETCDF_SUPPORT


#include "ssp-woa2005-db-creator.h"
#include "ssp-woa2005-db.h"


using namespace woss;


SspWoa2005DbCreator::SspWoa2005DbCreator() {

}


SspWoa2005DbCreator::~SspWoa2005DbCreator() {

}


WossDb* const SspWoa2005DbCreator::createWossDb() {
  assert( pathname.length() > 0 );
  
  SspWoa2005Db* woss_db = new SspWoa2005Db( pathname );
  
  assert( initializeDb( woss_db ) );
  
  return( woss_db );
}


bool SspWoa2005DbCreator::initializeDb( WossDb* const woss_db ) {
  return( WossDbCreator::initializeDb( woss_db ) );
}

#endif // WOSS_NETCDF_SUPPORT
