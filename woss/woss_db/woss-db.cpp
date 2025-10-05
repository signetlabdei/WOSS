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
 * @file   woss-db.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::WossDb class
 *
 * Provides the implementation of woss::WossDb class
 */


#include <cassert>
#include "woss-db.h"


using namespace woss;

WossDb::WossDb( const std::string& name )
 : db_name(name),
   debug(false)
{ 

}


PathName WossDb::getPathName( const std::string& full_path ) const {
   assert(full_path.length() > 0);

   PathName ret_value;
   size_t found;
   found = full_path.rfind("/");
   
   // path found
   if (found != std::string::npos) {
      // we set the name and excluding "/"
      ret_value.second = full_path.substr(found + 1);
      // we set the path, including "/"
      assert(full_path.length() > ret_value.second.length());
      ret_value.first = full_path.substr(0 , full_path.length() - ret_value.second.length());
   }
   else {
      // we set the path
      ret_value.first = "./";
      // we set the path
      ret_value.second = full_path;
   }
   return ret_value;
}


///////////////////////////////

#ifdef WOSS_NETCDF_SUPPORT
WossNetcdfDb::WossNetcdfDb( const std::string& name )
: WossDb(name),
  netcdf_db(nullptr)
{

}


WossNetcdfDb::~WossNetcdfDb() { 
  bool is_ok = closeConnection();
  assert(is_ok);
}


bool WossNetcdfDb::openConnection() {
  assert( isValid() );

  if (netcdf_db == nullptr) {
#if defined (WOSS_NETCDF4_SUPPORT)
    netcdf_db = std::move( std::make_unique< netCDF::NcFile >( db_name, netCDF::NcFile::read ) );
#else
    netcdf_db = std::move( std::make_unique< NcFile >( db_name.c_str() ) );
#endif // defined (WOSS_NETCDF4_SUPPORT)

    return ( netcdf_db != nullptr );
  }

  return true;
}

bool WossNetcdfDb::closeConnection() {
  netcdf_db->close();
  return true;
}
#endif // WOSS_NETCDF_SUPPORT
////////////////////////////

WossTextualDb::WossTextualDb( const std::string& name )
: WossDb(name),
  textual_db()
{

}


WossTextualDb::~WossTextualDb() { 
  bool is_ok = closeConnection();
  assert(is_ok);
}


bool WossTextualDb::openConnection() {
  return (db_name.length() > 0);
}


bool WossTextualDb::closeConnection() {
  textual_db.close();
  return true;
}
