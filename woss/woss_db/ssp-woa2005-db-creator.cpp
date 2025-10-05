/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2009 Federico Guerra
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


#if defined (WOSS_NETCDF4_SUPPORT)
SspWoa2005DbCreator::SspWoa2005DbCreator( WOADbType db_type )
: WossDbCreator(),
  woa_db_type(db_type)
{

}
#endif // defined (WOSS_NETCDF4_SUPPORT)


std::unique_ptr<WossDb> SspWoa2005DbCreator::createWossDb() const {
  assert( pathname.length() > 0 );

#if defined (WOSS_NETCDF4_SUPPORT)
  auto woss_db = std::make_unique<SspWoa2005Db>( pathname, woa_db_type );
#else
  auto woss_db = std::make_unique<SspWoa2005Db>( pathname );
#endif // defined (WOSS_NETCDF4_SUPPORT)

  assert( initializeDb( *woss_db ) );

  return( woss_db );
}


bool SspWoa2005DbCreator::initializeDb( WossDb& woss_db ) const {
  return( WossDbCreator::initializeDb( woss_db ) );
}

#endif // WOSS_NETCDF_SUPPORT
