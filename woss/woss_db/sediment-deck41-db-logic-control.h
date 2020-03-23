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
 * @file   sediment-deck41-db-logic-control.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::Deck41TypeTests class
 *
 * Provides the interface for the woss::Deck41TypeTests class
 */


#ifndef FLOOR_CONDITIONS_H
#define FLOOR_CONDITIONS_H


#ifdef WOSS_NETCDF_SUPPORT

#include <sediment-definitions.h>


namespace woss {
  
    
  static const int DECK41_FLOORTYPE_GRAVEL = 0; /**< Value stored in NetCDF file */

  static const int DECK41_FLOORTYPE_SAND = 1;  /**< Value stored in NetCDF file */

  static const int DECK41_FLOORTYPE_SILT = 2; /**< Value stored in NetCDF file */

  static const int DECK41_FLOORTYPE_CLAY = 3; /**< Value stored in NetCDF file */

  static const int DECK41_FLOORTYPE_OOZE = 4; /**< Value stored in NetCDF file */

  static const int DECK41_FLOORTYPE_MUD = 5; /**< Value stored in NetCDF file */

  static const int DECK41_FLOORTYPE_ROCKS = 6; /**< Value stored in NetCDF file */

  static const int DECK41_FLOORTYPE_ORGANIC = 7; /**< Value stored in NetCDF file */

  static const int DECK41_FLOORTYPE_NODULES = 8; /**< Value stored in NetCDF file */

  static const int DECK41_FLOORTYPE_HARDBOTTOM = 9; /**< Value stored in NetCDF file */

  static const int DECK41_FLOORTYPE_NODATA = 11; /**< Value stored in NetCDF file */


  /**
  * \brief Abstraction layer for database and data manipulation
  *
  * Deck41TypeTests helps SedimDeck41Db deciding what is the best DECK41 Sediment value to return.
  * To this purpose it provides 8 conditional tests
  **/
  class Deck41TypeTests {

    
    public:
      

    /**
    * Deck41TypeTests default constructor
    **/
    Deck41TypeTests();
    
    virtual ~Deck41TypeTests();

    
    /**
    * Deck41TypeTests test A. If it succeed SedimDeck41Db will return DECK41 main type as Sediment
    * @param types const reference to a Deck41Types object 
    * @returns <i>true</i> if condition holds, <i>false</i> otherwise
    **/
    bool conditionFloorA( const Deck41Types& types ) const ; 
    
    /**
    * Deck41TypeTests test B. If it succeed SedimDeck41Db will return DECK41 secondary type as Sediment
    * @param types const reference to a Deck41Types object 
    * @returns <i>true</i> if condition holds, <i>false</i> otherwise
    **/
    bool conditionFloorB( const Deck41Types& types ) const ; 
    
    /**
    * Deck41TypeTests test C. If it succeed SedimDeck41Db will try other coordinates, if those fails
    * it will return DECK41 main type as Sediment
    * @param types const reference to a Deck41Types object 
    * @returns <i>true</i> if condition holds, <i>false</i> otherwise
    **/
    bool conditionFloorC( const Deck41Types& types ) const ; 
    
    /**
    * Deck41TypeTests test D. If it succeed SedimDeck41Db will try other coordinates, if those fails
    * it will return DECK41 secondary type as Sediment
    * @param types const reference to a Deck41Types object 
    * @returns <i>true</i> if condition holds, <i>false</i> otherwise
    **/
    bool conditionFloorD( const Deck41Types& types ) const ; 
    
    /**
    * Deck41TypeTests test E. If it succeed SedimDeck41Db will return DECK41 a weighted average of both main and secondary
    * type as Sediment
    * @param types const reference to a Deck41Types object 
    * @returns <i>true</i> if condition holds, <i>false</i> otherwise
    **/
    bool conditionFloorE( const Deck41Types& types ) const ; 
    
    /**
    * Deck41TypeTests test F. If it succeed SedimDeck41Db will return DECK41 a weighted average of both main and secondary
    * type as Sediment
    * @param types const reference to a Deck41Types object 
    * @returns <i>true</i> if condition holds, <i>false</i> otherwise
    **/
    bool conditionFloorF( const Deck41Types& types ) const ;
    
    /**
    * Deck41TypeTests test G. If it succeed SedimDeck41Db will try other coordinates, if those fails
    * a Sediment not valid will be returned
    * @param types const reference to a Deck41Types object 
    * @returns <i>true</i> if condition holds, <i>false</i> otherwise
    **/
    bool conditionFloorG( const Deck41Types& types ) const ; 
    

    /**
    * Gets condition_a bool value
    * @returns condition_a value
    **/
    bool getConditionA() const { return condition_a; }
    
    /**
    * Gets condition_b bool value
    * @returns condition_b value
    **/
    bool getConditionB() const { return condition_b; }
    
    /**
    * Gets condition_c bool value
    * @returns condition_c value
    **/
    bool getConditionC() const { return condition_c; }
    
    /**
    * Gets condition_d bool value
    * @returns condition_d value
    **/
    bool getConditionD() const { return condition_d; }
    
    /**
    * Gets condition_e bool value
    * @returns condition_e value
    **/
    bool getConditionE() const { return condition_e; }
    
    /**
    * Gets condition_f bool value
    * @returns condition_f value
    **/
    bool getConditionF() const { return condition_f; }
    
    /**
    * Gets condition_g bool value
    * @returns condition_g value
    **/
    bool getConditionG() const { return condition_g; }
    

    /**
    * Performs all tests at once
    * @param types const reference to a Deck41Types object 
    **/
    void updateAllConditions( const Deck41Types& types );  
      
      
    private:
      

    bool condition_a;
    
    bool condition_b;
    
    bool condition_c;
    
    bool condition_d;
    
    bool condition_e;
    
    bool condition_f;
    
    bool condition_g;
    
    
  };


  //inline functions
  ///////
  inline bool Deck41TypeTests::conditionFloorA(const Deck41Types& types) const { // first = return main type
    bool equal = (types.first == types.second);
    bool first = (types.first == DECK41_FLOORTYPE_GRAVEL) || (types.first == DECK41_FLOORTYPE_SAND) || (types.first == DECK41_FLOORTYPE_SILT)
                || (types.first == DECK41_FLOORTYPE_MUD) || (types.first == DECK41_FLOORTYPE_HARDBOTTOM);
    bool second = (types.second == DECK41_FLOORTYPE_ROCKS) || (types.second == DECK41_FLOORTYPE_ORGANIC) || 
                  (types.second == DECK41_FLOORTYPE_NODULES) || (types.second == DECK41_FLOORTYPE_NODATA) || 
                  (types.second == DECK41_FLOORTYPE_HARDBOTTOM);

    return ( (equal && first ) || (first && second));
  }


  inline bool Deck41TypeTests::conditionFloorB(const Deck41Types& types) const { // second = return secondary type
    bool first = (types.first == DECK41_FLOORTYPE_NODATA);
    bool second = (types.second == DECK41_FLOORTYPE_GRAVEL) || (types.second == DECK41_FLOORTYPE_SAND) || 
                  (types.second == DECK41_FLOORTYPE_SILT) || (types.second == DECK41_FLOORTYPE_MUD);

    return (first && second);
  }


  inline bool Deck41TypeTests::conditionFloorC(const Deck41Types& types) const { // first* = try other coords, if fails get main type
    bool first = (types.first == DECK41_FLOORTYPE_CLAY) || (types.first == DECK41_FLOORTYPE_OOZE) || (types.first == DECK41_FLOORTYPE_ORGANIC)
              || (types.first == DECK41_FLOORTYPE_ROCKS) || (types.first == DECK41_FLOORTYPE_NODULES);
    bool second = (types.second == DECK41_FLOORTYPE_ROCKS) || (types.second == DECK41_FLOORTYPE_ORGANIC) || 
                  (types.second == DECK41_FLOORTYPE_NODULES) || (types.second == DECK41_FLOORTYPE_NODATA) || 
                  (types.second == DECK41_FLOORTYPE_HARDBOTTOM);

    return (first && second);
  }


  inline bool Deck41TypeTests::conditionFloorD(const Deck41Types& types) const { // second* = try other coords, if fails get secondary type
    if ( types.first == DECK41_FLOORTYPE_ORGANIC ) return true;
    else {
        bool first = (types.first == DECK41_FLOORTYPE_ROCKS) || (types.first == DECK41_FLOORTYPE_NODULES) || 
                    (types.first == DECK41_FLOORTYPE_NODATA) ;
        bool second = (types.second == DECK41_FLOORTYPE_GRAVEL) || (types.second == DECK41_FLOORTYPE_SAND) || 
                      (types.second == DECK41_FLOORTYPE_SILT) || (types.second == DECK41_FLOORTYPE_MUD);

    return (first && second);
    }
  }

  inline bool Deck41TypeTests::conditionFloorE(const Deck41Types& types) const { // 65%/35% = weighted avg
    bool nequal = types.first != types.second;
    bool first = (types.first == DECK41_FLOORTYPE_GRAVEL) || (types.first == DECK41_FLOORTYPE_SAND) || (types.first == DECK41_FLOORTYPE_SILT)
                || (types.first == DECK41_FLOORTYPE_MUD);
    bool second = first || (types.second == DECK41_FLOORTYPE_OOZE) || (types.second == DECK41_FLOORTYPE_CLAY);
    bool special = (types.first == DECK41_FLOORTYPE_OOZE) && (types.second == DECK41_FLOORTYPE_CLAY);

    return ( (first && second && nequal) || special );
  }


  inline bool Deck41TypeTests::conditionFloorF(const Deck41Types& types) const { // 40%/60% = weighted avg
    bool nequal = types.first != types.second;
    bool first = (types.first == DECK41_FLOORTYPE_CLAY) || (types.first == DECK41_FLOORTYPE_OOZE);
    bool second = (types.second == DECK41_FLOORTYPE_GRAVEL) || (types.second == DECK41_FLOORTYPE_SAND) || (types.second == DECK41_FLOORTYPE_SILT)
                || (types.second == DECK41_FLOORTYPE_MUD) || (types.second == DECK41_FLOORTYPE_OOZE);

    return (first && second && nequal);
  }


  inline bool Deck41TypeTests::conditionFloorG(const Deck41Types& types) const { // exit* = try other coords, if fails exit program
    return ( (types.first == DECK41_FLOORTYPE_NODATA) && (types.second == DECK41_FLOORTYPE_NODATA));
  }


  inline void Deck41TypeTests::updateAllConditions(const Deck41Types& types) {
    condition_a = conditionFloorA(types); // first = return main type
    condition_b = conditionFloorB(types); // second = return secondary type
    condition_c = conditionFloorC(types); // first* = try other coords, if fails get main type
    condition_d = conditionFloorD(types); // second* = try other coords, if fails get secondary type
    condition_e = conditionFloorE(types); // 65%/35% = weighted avg
    condition_f = conditionFloorF(types); // 40%/60% = weighted avg
    condition_g = conditionFloorG(types); 
  }


}

#endif // WOSS_NETCDF_SUPPORT

#endif /* FLOOR_CONDITIONS_H */

