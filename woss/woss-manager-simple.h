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
 * @file   woss-manager-simple.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossManagerSimple class
 *
 * Provides the interface for the woss::WossManagerSimple class
 */


#ifndef WOSS_MANAGER_SIMPLE_DEFINITIONS_H
#define WOSS_MANAGER_SIMPLE_DEFINITIONS_H


#include <time-arrival-definitions.h>
#include <definitions-handler.h>
#include "woss-manager.h"


namespace woss {
 
  
  /**
  * \brief simple template extension of WossManagerResDb or WossManagerResDbMT
  *
  * WossManagerSimple is a simple but functional template extension of WossManagerResDb or WossManagerResDbMT. 
  * It creates a Woss for every tx-rx pair. No memory management is done. In simulation with high mobility rate, 
  * a Woss for every receiver will be created everytime a transmitter will move, without removing old objects. 
  * <b>If a memory management is needed, the user should extend this class to suit his needs</b>.
  */
  template< typename WMResDb = WossManagerResDb >
  class WossManagerSimple : public WMResDb {
     
  
    public:


    /**
    * WossManagerSimple default constructor
    */
    WossManagerSimple();
    
    virtual ~WossManagerSimple() { reset(); }
       

    virtual WossManagerSimple& eraseActiveWoss( const CoordZ& tx, const CoordZ& rx, double start_frequency, double end_frequency );

    /**
    * Deletes all woss::Woss object
    * @returns reference to *this
    **/
    virtual WossManagerSimple& eraseAllWoss(); 

    /**
    * Deletes all created Woss instances
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool reset();

    
    /**
    * Performs a time evoulion of all time-dependant parameters of all created Woss instances
    * @param time_value const reference to a valid Time object
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool timeEvolve( const Time& time_value );


    /**
    * Sets the radius in meters of a cartesian sphere, in which all coordinates are considered to be equivalent
    * @param radius radius >= 0.0 in meters
    **/   
    static void setSpaceSampling( double radius ) { space_sampling = radius; }
    
    /**
    * Gets the radius in meters of a cartesian sphere, in which all coordinates are considered to be equivalent
    * @returns radius in meters
    **/   
    static double getSpaceSampling() { return space_sampling; }
    
    
    protected:
      
    
    /**
    * Map that links a receiver CoordZ to a pointer to a valid Woss object
    */
    typedef typename ::std::map< CoordZ, Woss*, CoordComparator< WossManagerSimple, CoordZ > > WossCoordZMap;
    typedef typename WossCoordZMap::iterator WCZIter;
    typedef typename WossCoordZMap::reverse_iterator WCZRIter;


    /**
    * Map that links a transmitter CoordZ to a WossCoordZMap
    */
    typedef typename ::std::map< CoordZ, WossCoordZMap, CoordComparator< WossManagerSimple, CoordZ > > WossContainer;
    typedef typename WossContainer::iterator WCIter;
    typedef typename WossContainer::reverse_iterator WCRIter;
    
    
    /**
    * The radius in meters (>= 0.0) of a cartesian sphere, in which all coordinates
    * are considered to be equivalent
    **/   
    static double space_sampling;
    
    /**
    * Map containing all created Woss objects
    **/ 
    WossContainer woss_map;


    /**
    * Returns a pointer to a properly initialized Woss, for storage purposes
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @returns pointer to a valid Woss object
    **/
    virtual Woss* const getWoss( const CoordZ& tx, const CoordZ& rx, double start_frequency, double end_frequency ); 
    
    
  };
  
    
  template< typename WMResDb >
  double WossManagerSimple< WMResDb >::space_sampling = 0.0;
    
    
  template< typename WMResDb >
  WossManagerSimple< WMResDb >::WossManagerSimple()
  : woss_map() 
  { 


  }


  template< typename WMResDb >
  bool WossManagerSimple< WMResDb >::reset() {
    for (WCIter it1 = woss_map.begin(); it1 != woss_map.end(); it1++) {
        for (WCZIter it2 = (it1->second).begin(); it2 != (it1->second).end(); it2++) {
          delete it2->second;
          it2->second = NULL;
        }
    }
    woss_map.clear();
    return true;
  }

  
  template< typename WMResDb >
  bool WossManagerSimple< WMResDb >::timeEvolve( const Time& time_value ) {
    for (WCIter it1 = woss_map.begin(); it1 != woss_map.end(); it1++) {
        for (WCZIter it2 = (it1->second).begin(); it2 != (it1->second).end(); it2++) {
          it2->second->timeEvolve(time_value);
        }
    }
    return true;    
  }
  

  template< typename WMResDb >
  Woss* const WossManagerSimple< WMResDb >::getWoss( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency ) {
    if (WMResDb::debug) ::std::cout << "WossManagerSimple::getWoss() tx coords " << tx_coordz << "; rx coords "
                                    << rx_coordz << "; start freq " << start_frequency << "; end freq " 
                                    << end_frequency << ::std::endl;

    WCIter it1 = woss_map.find( tx_coordz );

    if (it1 == woss_map.end() ) { // no tx CoordZ found
  
      if (WMResDb::debug) ::std::cout << "WossManagerSimple::getWoss() no tx CoordZ found" << ::std::endl;
      
      Woss* const curr_woss = this->woss_creator->createWoss( tx_coordz, rx_coordz, start_frequency, end_frequency );
      
      woss_map[tx_coordz][rx_coordz] = curr_woss;
      return( curr_woss );
    }
    else { // start CoordZ found
      WCZIter it2 = (it1->second).find( rx_coordz );

      if ( it2 == it1->second.end() ) { // no rx CoordZ foundZ

        if (WMResDb::debug) ::std::cout << "WossManagerSimple::getWoss() no rx CoordZ found" << ::std::endl;

        Woss* const curr_woss = WMResDb::woss_creator->createWoss( tx_coordz, rx_coordz, start_frequency, end_frequency );

        woss_map[tx_coordz][rx_coordz] = curr_woss;
        return( curr_woss );  
      }
      else return( it2->second );
    }
  }
  
  template< typename WMResDb >
  WossManagerSimple< WMResDb >& WossManagerSimple< WMResDb >::eraseAllWoss() {
    if (WMResDb::debug) ::std::cout << "WossManagerSimple::eraseAllWoss() map size = " << woss_map.size() << ::std::endl;
    
    for (WCIter it1 = woss_map.begin(); it1 != woss_map.end(); ++it1) {
      for (WCZIter it2 = (it1->second).begin(); it2 != (it1->second).end(); ++it2) {
        delete it2->second;
      }
    }
    
    woss_map.clear();

    if (WMResDb::debug) ::std::cout << "WossManagerSimple::eraseAllWoss() final map size = " << woss_map.size() << ::std::endl;

    return *this;
  }

  template< typename WMResDb >
  WossManagerSimple< WMResDb >& WossManagerSimple< WMResDb >::eraseActiveWoss( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency ) {
    if (WMResDb::debug) ::std::cout << "WossManagerSimple::eraseActiveWoss() tx coords " << tx_coordz << "; rx coords "
                                    << rx_coordz << "; start freq " << start_frequency << "; end freq " 
                                    << end_frequency << ::std::endl;

    WCIter it1 = woss_map.find( tx_coordz );

    if (it1 == woss_map.end() ) return *this;
    else { // start CoordZ found
      WCZIter it2 = (it1->second).find( rx_coordz );

      if ( it2 == it1->second.end() ) return *this;
      else {
        delete it2->second;
        it1->second.erase(it2);
        if ( it1->second.empty() ) woss_map.erase(it1);
      }
    }
    return *this;
  } 
  
  
}
  
  

#endif /* WOSS_MANAGER_SIMPLE_DEFINITIONS_H */


