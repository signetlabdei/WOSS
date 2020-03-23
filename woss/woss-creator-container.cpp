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
 * @file   woss-creator-container.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of WossCreator< CustomTransducer> class
 *
 * Provides the implementation of fully specialized WossCreator< CustomTransducer> class
 */


#include "woss-creator-container.h"


using namespace woss;


Location* const WossCreatorContainer< CustomTransducer >::ALL_LOCATIONS = NULL;  


const CoordZ WossCreatorContainer< CustomTransducer >::ALL_COORDZ = CoordZ();


WossCreatorContainer< CustomTransducer >::WossCreatorContainer()
: data_container(),
  debug(false)
{ 
}


WossCreatorContainer< CustomTransducer >::~WossCreatorContainer() { 
  clear(); 
}


WossCreatorContainer< CustomTransducer >::DCIter WossCreatorContainer< CustomTransducer >::find( const CoordZ& coordinates ) {
  DCIter it_all_loc = data_container.end();
  
  for ( DCIter it = data_container.begin(); it != data_container.end(); it++ ) {
    if ( it->first == ALL_LOCATIONS ) {
      it_all_loc = it;
      if ( coordinates == ALL_COORDZ ) break;
      else continue;
    }
    if ( it->first->isEquivalentTo( coordinates ) ) {
      if (debug) ::std::cout << "WossCreatorContainer<CustomTransducer>::find() tx coordinates found = " 
                              << coordinates << ::std::endl;      
      return it;
    }
  }
  
  if ( coordinates == ALL_COORDZ && it_all_loc != data_container.end() ) {
    if (debug) ::std::cout << "WossCreatorContainer<CustomTransducer>::find() tx ALL_COORDZ found" << ::std::endl;
    return it_all_loc;
  }
  
  if (debug && coordinates != ALL_COORDZ ) ::std::cout << "WossCreatorContainer<CustomTransducer>::find() tx coordinates not found = " 
                                                        << coordinates << ::std::endl; 
  
  if (debug && coordinates == ALL_COORDZ ) ::std::cout << "WossCreatorContainer<CustomTransducer>::find() tx ALL_COORDZ not found" << ::std::endl;
  return data_container.end();        
} 


WossCreatorContainer< CustomTransducer >::ICIter WossCreatorContainer< CustomTransducer >::find( const CoordZ& coordinates, const DCIter& iter ) {
  ICIter it_all_loc = iter->second.end();
  
  for ( ICIter it = iter->second.begin(); it != iter->second.end(); it++ ) {
    if ( it->first == ALL_LOCATIONS ) {
      it_all_loc = it;
      if ( coordinates == ALL_COORDZ ) break;
      else continue;
    }
    if ( it->first->isEquivalentTo( coordinates ) ) {
      if (debug) ::std::cout << "WossCreatorContainer<CustomTransducer>::find() rx coordinates found = " 
                              << coordinates << ::std::endl;      
      return it;
    }
  }
  
  if ( coordinates == ALL_COORDZ && it_all_loc != iter->second.end() ) {
    if (debug) ::std::cout << "WossCreatorContainer<CustomTransducer>::find() rx ALL_COORDZ found" << ::std::endl;
    return it_all_loc;
  }
  
  if (debug && coordinates != ALL_COORDZ ) ::std::cout << "WossCreatorContainer<CustomTransducer>::find() rx coordinates not found = " 
                                                        << coordinates << ::std::endl; 
  
  if (debug && coordinates == ALL_COORDZ ) ::std::cout << "WossCreatorContainer<CustomTransducer>::find() rx ALL_COORDZ not found" << ::std::endl;
  return iter->second.end(); 
}


CustomTransducer WossCreatorContainer< CustomTransducer >::get( const CoordZ& tx, const CoordZ& rx ) const { 
  DCIter it = const_cast< WossCreatorContainer< CustomTransducer >& >(*this).find( tx );
  bool tx_changed_to_all_coordz = false;

  if ( tx != ALL_COORDZ ) {
    if ( it == data_container.end() ) {
      it = const_cast< WossCreatorContainer< CustomTransducer >& >(*this).find( ALL_COORDZ );
      tx_changed_to_all_coordz = true;
    }
  }
  
  if ( it != data_container.end() ) {
    ICIter it2 = const_cast< WossCreatorContainer< CustomTransducer >& >(*this).find( rx, it );
    
    if ( rx != ALL_COORDZ ) {
      if ( it2 == it->second.end() ) it2 = const_cast< WossCreatorContainer< CustomTransducer >& >(*this).find( ALL_COORDZ, it );
    }
        
    if ( it2 != it->second.end() ) {
      if ( tx_changed_to_all_coordz ) return CustomTransducer(it2->second);
      else {
        if ( debug ) ::std::cout << "WossCreatorContainer<CustomTransducer>::get() rotating angle" << ::std::endl;
        return ( CustomTransducer( it2->second.type, it->first->getBearing(), 
                                  (it2->second.initial_vert_rotation + it->first->getVerticalOrientation()), 
                                  (it2->second.initial_horiz_rotation + it->first->getHorizontalOrientation()),
                                   it2->second.multiply_costant, it2->second.add_costant ) ); 
      }
    }
  }
  if ( debug ) ::std::cerr << "WARNING: WossCreatorContainer<CustomTransducer>::get() no tx nor rx coordinates found, returning default constructor!!" << ::std::endl;

  return CustomTransducer();
}


CustomTransducer WossCreatorContainer< CustomTransducer >::get( Location* const tx, Location* const rx ) const { 
  DCIter it = const_cast< WossCreatorContainer< CustomTransducer >::DataContainer& >(data_container).find( tx );
  bool tx_changed_to_all_locations = false;

  if ( tx != ALL_LOCATIONS ) {
    if ( it == data_container.end() ) {
      if ( debug ) ::std::cout << "WossCreatorContainer<CustomTransducer>::get() no tx location found = " << *tx 
                                << "; trying ALL_LOCATIONS"<< ::std::endl;

      it = const_cast< WossCreatorContainer< CustomTransducer >::DataContainer& >(data_container).find( ALL_LOCATIONS );
      tx_changed_to_all_locations = true;
    }

    if ( debug ) ::std::cout << "WossCreatorContainer<CustomTransducer>::get() tx location found = " << *tx << ::std::endl;
    
  }
  
  if ( it != data_container.end() ) {
    ICIter it2 = it->second.find( rx );
    
    if ( rx != ALL_LOCATIONS ) {
    
      if ( it2 == it->second.end() ) {
        if ( debug ) ::std::cout << "WossCreatorContainer<CustomTransducer>::get() no rx location found = " << *rx 
                                  << "; trying ALL_LOCATIONS"<< ::std::endl;

        it2 = it->second.find( ALL_LOCATIONS );
      }
      
      if ( debug ) ::std::cout << "WossCreatorContainer<CustomTransducer>::get() rx location found = " << *rx << ::std::endl;
    }

    if ( it2 != it->second.end() ) {
      if ( tx_changed_to_all_locations ) return CustomTransducer( it2->second );
      else {
        if ( debug ) ::std::cout << "WossCreatorContainer<CustomTransducer>::get() rotating angle" << ::std::endl;
        return ( CustomTransducer( it2->second.type, it->first->getBearing(), 
                                  (it2->second.initial_vert_rotation + it->first->getVerticalOrientation()), 
                                  (it2->second.initial_horiz_rotation + it->first->getHorizontalOrientation()),
                                   it2->second.multiply_costant, it2->second.add_costant ) ); 
      }
    }
  }
    
  if ( debug ) ::std::cerr << "WARNING: WossCreatorContainer<CustomTransducer>::get() no tx nor rx location found, returning default constructor!!" << ::std::endl;
  
  return CustomTransducer();
}
