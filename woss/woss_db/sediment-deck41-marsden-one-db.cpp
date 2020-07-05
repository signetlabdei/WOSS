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
 * @file   sediment-deck41-marsden-one-db.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::SedimDeck41MarsdenOneDb class
 *
 * Provides the implementation of the woss::SedimDeck41MarsdenOneDb class
 */

#ifdef WOSS_NETCDF_SUPPORT

#include <cstdlib>
#include <cassert>
#include "sediment-deck41-marsden-one-db.h" 

#define SEDIMENT_NOT_FOUND (-1000000)

using namespace woss;

SedimDeck41MarsdenOneDb::SedimDeck41MarsdenOneDb( const ::std::string& name )
: WossNetcdfDb( name ),
#if defined (WOSS_NETCDF4_SUPPORT)
  main_sedim_var_marsden_one(),
  sec_sedim_var_marsden_one(),
  marsden_square_var(),
  marsden_one_square_var(),
#else
  main_sedim_var_marsden_one(NULL),
  sec_sedim_var_marsden_one(NULL),
  marsden_square_var(NULL),
  marsden_one_square_var(NULL),
#endif // defined (WOSS_NETCDF4_SUPPORT)
  deck41_db_type(DECK41_DB_V1_TYPE)
{
}

#if defined (WOSS_NETCDF4_SUPPORT)
SedimDeck41MarsdenOneDb::SedimDeck41MarsdenOneDb( const ::std::string& name, DECK41DbType db_type )
: WossNetcdfDb( name ),
  main_sedim_var_marsden_one(),
  sec_sedim_var_marsden_one(),
  marsden_square_var(),
  marsden_one_square_var(),
  deck41_db_type(db_type)
{
}
#endif // defined (WOSS_NETCDF4_SUPPORT)

bool SedimDeck41MarsdenOneDb::finalizeConnection() {
#if defined (WOSS_NETCDF4_SUPPORT)
  main_sedim_var_marsden_one = netcdf_db->getVar("seafloor_main_type");
  if (main_sedim_var_marsden_one.isNull()) {
    ::std::cout << "SedimDeck41MarsdenOneDb::finalizeConnection() main_sedim_var_marsden_one is not valid" << ::std::endl;
    return false;
  }
  sec_sedim_var_marsden_one = netcdf_db->getVar("seafloor_secondary_type");
  if (sec_sedim_var_marsden_one.isNull()) {
    ::std::cout << "SedimDeck41MarsdenOneDb::finalizeConnection() sec_sedim_var_marsden_one is not valid" << ::std::endl;
    return false;
  }
  if (deck41_db_type == DECK41_DB_V2_TYPE) {
    marsden_square_var = netcdf_db->getVar("marsden_square");
    if (marsden_square_var.isNull()) {
      ::std::cout << "SedimDeck41MarsdenDb::finalizeConnection() marsden_square_var is not valid" << ::std::endl;
      return false;
    }
    marsden_one_square_var = netcdf_db->getVar("marsden_one_degree_square");
    if (marsden_one_square_var.isNull()) {
      ::std::cout << "SedimDeck41MarsdenDb::finalizeConnection() marsden_one_square_var is not valid" << ::std::endl;
      return false;
    }
  }
  return true;
#else
  main_sedim_var_marsden_one = netcdf_db->get_var("seafloor_main_type");
  sec_sedim_var_marsden_one = netcdf_db->get_var("seafloor_secondary_type");
  return ( (main_sedim_var_marsden_one != NULL) && (sec_sedim_var_marsden_one != NULL) );
#endif // defined (WOSS_NETCDF4_SUPPORT)
}


Deck41Types SedimDeck41MarsdenOneDb::getSeaFloorType(const MarsdenCoord& marsden_coord) const {
  int main_type = SEDIMENT_NOT_FOUND;
  int secondary_type = SEDIMENT_NOT_FOUND;

  if (debug) ::std::cout << "SedimDeck41MarsdenOneDb::getSeaFloorType() marsden square = " << marsden_coord.first 
                         << "; marsden 10 degrees = " << marsden_coord.second << ::std::endl;

#if defined (WOSS_NETCDF4_SUPPORT)
  ::std::vector<size_t> index_vector, count_vector;

  if (deck41_db_type == DECK41_DB_V2_TYPE) {
    Marsden mars_square = SEDIMENT_NOT_FOUND;
    Marsden mars_one_square = SEDIMENT_NOT_FOUND;

    index_vector.push_back(marsden_coord.first - 1);

    marsden_square_var.getVar(index_vector, &mars_square);
    if (mars_square == SEDIMENT_NOT_FOUND) {
      ::std::cout << "SedimDeck41MarsdenOneDb::getSeaFloorType() Couldn't extract current mars_square" << ::std::endl;
      exit(1);
    }

    if (debug)
      ::std::cout << "SedimDeck41MarsdenOneDb::getSeaFloorType() got mars_square = " << mars_square << ::std::endl;

    assert(mars_square == marsden_coord.first);

    index_vector.clear();
    index_vector.push_back(marsden_coord.second);

    marsden_one_square_var.getVar(index_vector, &mars_one_square);
    if (mars_one_square == SEDIMENT_NOT_FOUND) {
      ::std::cout << "SedimDeck41MarsdenOneDb::getSeaFloorType() Couldn't extract current mars_one_square" << ::std::endl;
      exit(1);
    }

    if (debug)
      ::std::cout << "SedimDeck41MarsdenOneDb::getSeaFloorType() got mars_one_square = " << mars_one_square << ::std::endl;

    assert(mars_one_square == marsden_coord.second);

    index_vector.clear();
    index_vector.push_back(marsden_coord.first - 1);
  }
  else if (deck41_db_type == DECK41_DB_V1_TYPE) {
    index_vector.push_back(marsden_coord.first);
  }
  else {
    ::std::cout << "SedimDeck41MarsdenOneDb::getSeaFloorType() invalid deck41_db_type = " << (int)deck41_db_type << ::std::endl;
    exit(1);
  }

  index_vector.push_back(marsden_coord.second);

  count_vector.push_back(1);
  count_vector.push_back(1);

  main_sedim_var_marsden_one.getVar(index_vector, count_vector, &main_type);
  if (main_type == SEDIMENT_NOT_FOUND) {
    ::std::cout << "SedimDeck41MarsdenOneDb::getSeaFloorType() Couldn't extract current main_type" << ::std::endl;
    exit(1);
  }

  sec_sedim_var_marsden_one.getVar(index_vector, count_vector, &secondary_type);
  if (secondary_type == SEDIMENT_NOT_FOUND) {
    ::std::cout << "SedimDeck41MarsdenOneDb::getSeaFloorType() Couldn't extract current secondary_type" << ::std::endl;
    exit(1);
  }
#else
  if (deck41_db_type == DECK41_DB_V1_TYPE) {
    NcBool ret_val = main_sedim_var_marsden_one->set_cur(marsden_coord.first, marsden_coord.second);
    if (!ret_val) {
      ::std::cout << "SedimDeck41MarsdenOneDb::getSeaFloorType() Couldn't set_cur() of main_sedim_var_marsden_one;"
                  << " current indexes = " << marsden_coord.first << " , " << marsden_coord.second << ::std::endl;
      exit(1);
    }

    ret_val = sec_sedim_var_marsden_one->set_cur(marsden_coord.first, marsden_coord.second); 
    if (!ret_val) {
      ::std::cout << "SedimDeck41MarsdenOneDb::getSeaFloorType() Couldn't set_cur() of sec_sedim_var_marsden_one;"
                  << " current indexes = " << marsden_coord.first << " , " << marsden_coord.second << ::std::endl;
      exit(1);
    }
  }
  else {
    ::std::cout << "SedimDeck41MarsdenOneDb::getSeaFloorType() invalid deck41_db_type = " << (int)deck41_db_type << ::std::endl;
    exit(1);
  }

  ret_val = main_sedim_var_marsden_one->get(&main_type, 1, 1);
  if (!ret_val) {
    ::std::cout << "SedimDeck41MarsdenOneDb::getSeaFloorType() Couldn't extract main type; " 
                << " current indexes = " << marsden_coord.first << " , " << marsden_coord.second << ::std::endl;
    exit(1);
  }

  ret_val = sec_sedim_var_marsden_one->get(&secondary_type, 1, 1);
  if (!ret_val) {
    ::std::cout << "SedimDeck41MarsdenOneDb::getSeaFloorType() Couldn't extract secondary type; "
                << " current indexes = " << marsden_coord.first << " , " << marsden_coord.second << ::std::endl;
    exit(1);
  }
#endif // defined (WOSS_NETCDF4_SUPPORT)

  if (debug) {
    ::std::cout << "SedimDeck41MarsdenOneDb::getSeaFloorType() mardsen_square = " << marsden_coord.first 
                << "; mardsen_one_degree = " << marsden_coord.second << "; main type = " 
                << main_type << "; secondary type = " << secondary_type << ::std::endl;
  }

  return(::std::make_pair(main_type, secondary_type));
}

#endif // WOSS_NETCDF_SUPPORT
