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
 * @file   woss-creator-container.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossCreatorContainer class
 *
 * Provides the interface for woss::WossCreatorContainer class
 */


#ifndef WOSS_CREATOR_CONTAINER_DEFINITIONS_H
#define WOSS_CREATOR_CONTAINER_DEFINITIONS_H


#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <definitions-handler.h>


namespace woss {

  /**
  * \brief Initial set up of a transducer
  *
  * Struct that stores the Transducer's type name, its vertical beam pattern initial rotation, multiplicative and additive
  * constants.
  **/  
  struct CustomTransducer {

    /**
    * Default constructor
    * @param name transducer's type name
    * @param rot initial rotation [decimal degrees]
    * @param mult multiplicative constant
    * @param add additive constant
    **/     
    CustomTransducer( const std::string& name = "", double bearing = 0.0, double vert_rot = 0.0, double horiz_rot = 0.0, double mult = 1.0, double add = 0.0 ) 
      : type(name), initial_bearing(bearing), initial_vert_rotation(vert_rot), initial_horiz_rotation(horiz_rot), multiply_costant(mult), add_costant(add) { }

    friend std::ostream& operator<<( std::ostream& os, const CustomTransducer& instance ) {
      os << "type name = " << instance.type <<"; initial bearing = " << instance.initial_bearing
         << "; initial vertical rotation = " << instance.initial_vert_rotation 
         << "; initial horizontal rotation = " << instance.initial_horiz_rotation
         << "; mult costant = " << instance.multiply_costant << "; add costant = "
         << instance.add_costant;
      return os;
    }

    /**
    * transducer type name
    */  
    std::string type;

    /**
    * beam pattern bearing orientation
    */
    double initial_bearing;
    
    /**
    * beam pattern initial vertical rotation [decimal degrees]
    */
    double initial_vert_rotation;
    
    /**
    * beam pattern initial horizontal rotation [decimal degrees]
    */
    double initial_horiz_rotation;
   
    /**
    * beam pattern multiplicative constant
    */
    double multiply_costant;

    /**
    * beam pattern additive constant
    */
    double add_costant;

  };

  /**
  * \brief Class that stores WossCreator parameters
  *
  * WossCreatorContainer provides interface for storing and retrieving custom WossCreator data, indexed by 
  * transmitter and receiver std::shared_ptr<woss::Location>
  */
  template< typename Data > 
  class WossCreatorContainer {
  
    public:

    /**
    * Pointer that represents the index for all possible Locations
    */
    static std::shared_ptr<Location> ALL_LOCATIONS;

    /**
    * Pointer that represents the index for all possible coordinates
    */
    static const CoordZ ALL_COORDZ;

    WossCreatorContainer() = default;
    
    ~WossCreatorContainer() = default;

    /**
    * Checks if the container is empty
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/   
    bool isEmpty() const;

    /**
    * Returns the size of the container
    * @return container's size
    **/
    int size() const;

    /**
    * Inserts an object into the container. If the keys are already present, the object is NOT inserted
    * @param data const reference to a Data object to be inserted
    * @param tx const reference to a std::shared_ptr to a valid transmitter Location
    * @param rx const reference to a std::shared_ptr to a valid receiver Location
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/
    bool insert( const Data& data, const std::shared_ptr<Location>& tx, const std::shared_ptr<Location>& rx );

    /**
    * Inserts an object into the container. If the keys are already present, the object is NOT inserted
    * @param data const reference to a Data object to be inserted
    * @param tx const reference to a valid transmitter CoordZ
    * @param rx const reference to a valid receiver CoordZ
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/
    bool insert( const Data& data, const CoordZ& tx, const CoordZ& rx );

    /**
    * Returns an object into the container. If the keys are NOT found, a default constructed object is returned
    * @param tx const reference to a std::shared_ptr to a valid transmitter Location
    * @param rx const reference to a std::shared_ptr to a valid receiver Location
    * @return a copy of the original object
    **/
    Data get( const std::shared_ptr<Location>& tx, const std::shared_ptr<Location>& rx  ) const;

    /**
    * Returns an object into the container. If the keys are NOT found, a default constructed object is returned
    * @param tx const reference to a valid transmitter CoordZ
    * @param rx const reference to a valid receiver CoordZ
    * @return a copy of the original object
    **/
    Data get( const CoordZ& tx, const CoordZ& rx ) const;

    /**
    * Returns a reference to the Data object associated to transmitter and receiver Location equal to ALL_LOCATIONS.
    * If no keys were present, the Data object is default constructed and a reference to it is returned
    * @return a reference to a Data object
    **/
    Data& accessAllLocations();

    /**
    * Erases an object into the container
    * @param tx const reference to a std::shared_ptr to a valid transmitter Location
    * @param rx const reference to a std::shared_ptr to a valid receiver Location
    **/
    void erase( const std::shared_ptr<Location>& tx, const std::shared_ptr<Location>& rx );
    
    /**
    * Erases an object into the container
    * @param tx const reference to a valid transmitter CoordZ
    * @param rx const reference to a valid receiver CoordZ
    **/
    void erase( const CoordZ& tx, const CoordZ& rx );

    /**
    * Replaces an object into the container. If the key are not present, the object is still inserted
    * @param data const reference to a Data object to be inserted
    * @param tx const reference to a std::shared_ptr to a valid transmitter Location
    * @param rx const reference to a std::shared_ptr to a valid receiver Location
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/
    void replace( const Data& data, const std::shared_ptr<Location>& tx, const std::shared_ptr<Location>& rx );

    /**
    * Replaces an object into the container. If the key are not present, the object is still inserted
    * @param data const reference to a Data object to be inserted
    * @param tx const reference to a valid transmitter CoordZ
    * @param rx const reference to a valid receiver CoordZ
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/
    void replace( const Data& data, const CoordZ& tx, const CoordZ& rx );

    /**
    * Clears the container
    */
    void clear();

    /**
    * Sets or unsets the debug flag
    * @param flag boolean flag
    */
    void setDebug( bool flag ) { debug = flag; }

    /**
    * Retuns the debug flag
    * @return <i>true</i> if is using the debug flag, <i>false</i> otherwise
    */
    bool isUsingDebug() const { return debug; }

    protected:

    /**
    * The innermost container. It associates a receiver Location pointer to the Data object
    */
    using InnerContainer = std::map< std::shared_ptr<Location>, Data >;
    using ICIter = typename InnerContainer::iterator;
    using ICRIter = typename InnerContainer::reverse_iterator;
    using ICCIter = typename InnerContainer::const_iterator;
    using ICCRIter = typename InnerContainer::const_reverse_iterator;

    /**
    * The outer container. It associates a transmitter Location pointer to a InnerContainer object
    */
    using DataContainer = std::map< std::shared_ptr<Location>, InnerContainer >;
    using DCIter = typename DataContainer::iterator;
    using DCCIter = typename DataContainer::const_iterator;
    using DCRIter = typename DataContainer::reverse_iterator;
    using DCRCIter = typename DataContainer::const_reverse_iterator;

    /**
    * Returns an DataContainer iterators that points to the given transmitter coordinates.
    * An iterator to the end of the DataContainer is returned if no coordinates are found
    * @param coordinates a const reference to a valid transmitter CoordZ
    * @return a DataContainer::iterator
    */
    DCIter find( const CoordZ& coordinates );

    /**
    * Returns an InnerContainer iterators for the DataContainter iterator that points 
    * to the given receiver coordinates. An iterator to the relative end of the InnerContainer 
    * is returned if no coordinates are found
    * @param coordinates a const reference to a valid receiver CoordZ
    * @return a DataContainer::iterator
    */
    ICIter find( const CoordZ& coordinates, const DCIter& iter );

    /**
    * Creates a Location for a given CoordZ
    * @param coordinates a const reference to a valid receiver CoordZ
    * @return a std::shared_ptr to a heap-allocated Location pointer
    */
    std::shared_ptr<Location> createLocation( const CoordZ& coordinates );

    /**
    * The Data container
    */
    DataContainer data_container;

    /**
    * Debug flag
    */
    bool debug = false;

  };


  template< typename Data > 
  std::shared_ptr<Location> WossCreatorContainer< Data >::ALL_LOCATIONS = nullptr;


  template< typename Data > 
  const CoordZ WossCreatorContainer< Data >::ALL_COORDZ = CoordZ();


  template< typename Data >
  inline bool WossCreatorContainer< Data >::isEmpty() const {
    return data_container.empty();
  }


  template< typename Data >
  inline int WossCreatorContainer< Data >::size() const {
    return data_container.size();
  }


  template< typename Data >
  inline std::shared_ptr<Location> WossCreatorContainer< Data >::createLocation( const CoordZ& coordinates ) {
    if ( coordinates == ALL_COORDZ ) 
      return nullptr;
    return std::move( SDefHandler::instance().createLocation(coordinates) );
  }


  template< typename Data >
  typename WossCreatorContainer< Data >::DCIter WossCreatorContainer< Data >::find( const CoordZ& coordinates ) {
    auto it_all_loc = data_container.end();

    for ( auto it = data_container.begin(); it != data_container.end(); it++ ) {
      if ( it->first == ALL_LOCATIONS ) {
        it_all_loc = it;
        if ( coordinates == ALL_COORDZ ) 
          break;
        else 
          continue;
      }
      if ( it->first->isEquivalentTo( coordinates ) ) {
        if (debug) 
          std::cout << "WossCreatorContainer::find() tx coordinates found = " 
                    << coordinates << std::endl;
        return it;
      }
    }
   
    if ( coordinates == ALL_COORDZ && it_all_loc != data_container.end() ) {
      if (debug) 
        std::cout << "WossCreatorContainer::find() tx ALL_COORDZ found" << std::endl;
      return it_all_loc;
    }
    
    if (debug && coordinates != ALL_COORDZ ) 
      std::cout << "WossCreatorContainer::find() tx coordinates not found = " 
                << coordinates << std::endl; 
    
    if (debug && coordinates == ALL_COORDZ ) 
      std::cout << "WossCreatorContainer::find() tx ALL_COORDZ not found" << std::endl;
    return data_container.end();
  } 


  template< typename Data >  
  typename WossCreatorContainer< Data >::ICIter WossCreatorContainer< Data >::find( const CoordZ& coordinates, const DCIter& iter ) {
    auto it_all_loc = iter->second.end();
    
    for ( auto it = iter->second.begin(); it != iter->second.end(); it++ ) {
      if ( it->first == ALL_LOCATIONS ) {
        it_all_loc = it;
        if ( coordinates == ALL_COORDZ ) 
          break;
        else 
          continue;
      }
      if ( it->first->isEquivalentTo( coordinates ) ) {
        if (debug) 
          std::cout << "WossCreatorContainer::find() rx coordinates found = " 
                    << coordinates << std::endl;
        return it;
      }
    }
   
    if ( coordinates == ALL_COORDZ && it_all_loc != iter->second.end() ) {
      if (debug) 
        std::cout << "WossCreatorContainer::find() rx ALL_COORDZ found" << std::endl;
      return it_all_loc;
    }
    
    if (debug && coordinates != ALL_COORDZ ) 
      std::cout << "WossCreatorContainer::find() rx coordinates not found = " 
                << coordinates << std::endl; 
    
    if (debug && coordinates == ALL_COORDZ ) 
      std::cout << "WossCreatorContainer::find() rx ALL_COORDZ not found" << std::endl;
    return iter->second.end(); 
  }
  
  
  template< typename Data >  
  inline bool WossCreatorContainer< Data >::insert( const Data& data, const std::shared_ptr<Location>& tx, const std::shared_ptr<Location>& rx ) { 
    auto it = data_container.find(tx);
    if ( it == data_container.end() ) {
      data_container[tx][rx] = data;
      return true;
    }
    auto it2 = it->second.find(rx);
    if ( it2 == it->second.end() ) {
      (it->second)[rx] = data;
      return true;
    }
    return false;
  }
 
 
  template< typename Data >
  inline bool WossCreatorContainer< Data >::insert( const Data& data, const CoordZ& tx, const CoordZ& rx ) { 
    auto it = find( tx );
    if ( it == data_container.end() ) {
      data_container[ createLocation(tx) ][ createLocation(rx) ] = data; 
      return true;
    }
    auto it2 = find( rx, it );
    if ( it2 == it->second.end() ) {
      (it->second)[ createLocation(rx) ] = data;
      return true;
    }
    return false;
  }
 
 
  template< typename Data >
  inline Data WossCreatorContainer< Data >::get( const std::shared_ptr<Location>& tx, const std::shared_ptr<Location>& rx ) const { 
    auto it = const_cast< typename WossCreatorContainer< Data >::DataContainer& >(data_container).find( tx );

    if ( tx != ALL_LOCATIONS ) {
      if ( it == data_container.end() ) {
        if ( debug ) 
          std::cout << "WossCreatorContainer::get() no tx location found = " << *tx 
                    << "; trying ALL_LOCATIONS"<< std::endl;

        it = const_cast< typename WossCreatorContainer< Data >::DataContainer& >(data_container).find( ALL_LOCATIONS );
      }

      if ( debug ) 
        std::cout << "WossCreatorContainer::get() tx location found = " << *tx << std::endl;
    }
    
    if ( it != data_container.end() ) {
      auto it2 = it->second.find( rx );

      if ( rx != ALL_LOCATIONS ) {
        if ( it2 == it->second.end() ) {
          if ( debug ) 
            std::cout << "WossCreatorContainer::get() no rx location found = " << *rx 
                      << "; trying ALL_LOCATIONS"<< std::endl;

          it2 = it->second.find( ALL_LOCATIONS );
        }

        if ( debug ) 
          std::cout << "WossCreatorContainer::get() rx location found = " << *rx << std::endl;
      }
      
      if ( it2 != it->second.end() ) 
        return it2->second;
    }
     
    std::cerr << "WARNING: WossCreatorContainer::get() no tx nor rx location found, returning default constructor!!" << std::endl;

    return Data();
  }
 
 
  template< typename Data >
  inline Data& WossCreatorContainer< Data >::accessAllLocations() {
    return data_container[ALL_LOCATIONS][ALL_LOCATIONS];
  }
 
  
  template< typename Data >
  inline Data WossCreatorContainer< Data >::get( const CoordZ& tx, const CoordZ& rx ) const { 
    auto it = const_cast< WossCreatorContainer< Data >& >(*this).find( tx );
    
    if ( tx != ALL_COORDZ ) {
      if ( it == data_container.end() ) 
        it = const_cast< WossCreatorContainer< Data >& >(*this).find( ALL_COORDZ );
    }
    
    if ( it != data_container.end() ) {
      auto it2 = const_cast< WossCreatorContainer< Data >& >(*this).find( rx, it );

      if ( rx != ALL_COORDZ ) {
        if ( it2 == it->second.end() ) 
          it2 = const_cast< WossCreatorContainer< Data >& >(*this).find( ALL_COORDZ, it );
      }

      if ( it2 != it->second.end() ) {
        if ( debug ) 
          std::cout << "WossCreatorContainer::get() value found = " << it2->second << std::endl;

        return it2->second;
      }
    }
    std::cerr << "WARNING: WossCreatorContainer::get() no tx nor rx coordinates found, returning default constructor!!" << std::endl;

    return Data();
  }
 
 
  template< typename Data >
  inline void WossCreatorContainer< Data >::erase( const std::shared_ptr<Location>& tx, const std::shared_ptr<Location>& rx ) { 
    auto it = data_container.find(tx);
    if ( it != data_container.end() ) 
      it->second.erase( rx );
    if ( it->second.empty() ) 
      data_container.erase(it);
  }
 
 
  template< typename Data >
  inline void WossCreatorContainer< Data >::erase( const CoordZ& tx, const CoordZ& rx ) { 
    for ( auto it = data_container.begin(); it != data_container.end();  ) {
      bool tx_found = false;
 
      if ( it->first != ALL_LOCATIONS ) {
         if ( it->first->isEquivalentTo( tx ) ) 
            tx_found = true;
      } else {
         if ( tx == ALL_COORDZ ) 
            tx_found = true;
      }

      if ( tx_found ) {
        for ( auto it2 = it->second.begin(); it2 != it->second.end(); ) {
           bool rx_found = false;

           if ( it2->first != ALL_LOCATIONS ) {
              if ( it2->first->isEquivalentTo( rx ) ) 
                rx_found = true;
           } else {
              if ( rx == ALL_COORDZ ) 
                rx_found = true;
           }

          if ( rx_found ) 
            it->second.erase(it2++);
          else 
            ++it2;
        }
        
      } 
        
      if ( it->second.empty() ) 
        data_container.erase(it++);
      else ++it;
    }  
  }
 
 
  template< typename Data >
  inline void WossCreatorContainer< Data >::replace( const Data& data, const std::shared_ptr<Location>& tx, const std::shared_ptr<Location>& rx ) { 
    data_container[tx][rx] = data;
  }
 
 
  template< typename Data >
  inline void WossCreatorContainer< Data >::replace( const Data& data, const CoordZ& tx, const CoordZ& rx ) { 
    data_container[ createLocation(tx) ][ createLocation(rx) ] = data;
  }
 
 
  template< typename Data >
  inline void WossCreatorContainer< Data >::clear() { 
    data_container.clear();
  }


  ////////

  /**
  * \brief Full specialization for woss::CustomTransducer
  *
  * Full specialization for woss::CustomTransducer
  */
  template<> 
  class WossCreatorContainer< CustomTransducer > {

    public:

    static std::shared_ptr<Location> ALL_LOCATIONS;
    
    static const CoordZ ALL_COORDZ;

    WossCreatorContainer() = default;
    
    ~WossCreatorContainer() = default;

    bool isEmpty() const;
    
    int size() const;

    /**
    * Inserts an object into the container. If the keys are already present, the object is NOT inserted and it
    * is deleted
    * @param data const reference to a CustomTransducer object to be inserted
    * @param tx const reference to a std::shared_ptr to a valid transmitter Location
    * @param rx const reference to a std::shared_ptr to a valid receiver Location
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/      
    bool insert( const CustomTransducer& data, const std::shared_ptr<Location>& tx, const std::shared_ptr<Location>& rx );
    
    /**
    * Inserts an object into the container. If the keys are already present, the object is NOT inserted and it
    * is deleted
    * @param data const pointer to a CustomTransducer object to be inserted
    * @param tx const reference to a valid transmitter CoordZ
    * @param rx const reference to a valid receiver CoordZ
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/      
    bool insert( const CustomTransducer& data, const CoordZ& tx, const CoordZ& rx );

    /**
    * Returns a CustomTransducer for given keys. If the keys are not found a nullptr pointer is returned
    * @param tx const reference to a std::shared_ptr to a valid transmitter Location
    * @param rx const reference to a std::shared_ptr to a valid receiver Location
    * @return a pointer to a rotated CustomTransducer object
    **/
    CustomTransducer get( const std::shared_ptr<Location>& tx, const std::shared_ptr<Location>& rx ) const;

    /**
    * Returns a CustomTransducer pointer for given keys. If the keys are not found a nullptr pointer is returned
    * @param tx const reference to a valid transmitter CoordZ
    * @param rx const reference to a valid receiver CoordZ
    * @return a pointer to a rotated CustomTransducer object
    **/
    CustomTransducer get( const CoordZ& tx, const CoordZ& rx ) const;

    /**
    * Returns a reference to the CustomTransducer associated to transmitter and receiver Location equal to ALL_LOCATIONS.
    * If no keys were present, the CustomTransducer is defaulted constructed and a reference to it is returned.
    * The user has to properly delete the pointer prior changing it
    * @return a reference to a Data pointer
    **/
    CustomTransducer& accessAllLocations();

    /**
    * Erases an CustomTransducer from the container. If the keys are already present, the previous object is deleted
    * @param tx const reference to a std::shared_ptr to a valid transmitter Location
    * @param rx const reference to a std::shared_ptr to a valid receiver Location
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/
    void erase( const std::shared_ptr<Location>& tx, const std::shared_ptr<Location>& rx );

   /**
    * Erases an CustomTransducer from the container. If the keys are already present, the previous object is deleted
    * @param tx const reference to a valid transmitter CoordZ
    * @param rx const reference to a valid receiver CoordZ
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/
    void erase( const CoordZ& tx, const CoordZ& rx );

    /**
    * Replaces a pointer into the container. If the keys are already present, the previous object is deleted
    * @param data const reference to a CustomTransducer object to be inserted
    * @param tx const reference to a std::shared_ptr to a valid transmitter Location
    * @param rx const reference to a std::shared_ptr to a valid receiver Location
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/
    void replace( const CustomTransducer& data, const std::shared_ptr<Location>& tx, const std::shared_ptr<Location>& rx );

    /**
    * Replaces a pointer into the container. If the keys are already present, the previous object is deleted
    * @param data const pointer to a CustomTransducer object to be inserted
    * @param tx const reference to a valid transmitter CoordZ
    * @param rx const reference to a valid receiver CoordZ
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/
    void replace( const CustomTransducer& data, const CoordZ& tx, const CoordZ& rx );

    void clear();

    void setDebug( bool flag ) { debug = flag; }
    
    bool isUsingDebug() const { return debug; }

    protected:

    using InnerContainer = std::map< std::shared_ptr<Location>, CustomTransducer >;
    using ICIter = InnerContainer::iterator;
    using ICRIter = InnerContainer::reverse_iterator;
    using ICCIter = InnerContainer::const_iterator ;
    using ICCRIter = InnerContainer::const_reverse_iterator;

    using DataContainer = std::map< std::shared_ptr<Location>, InnerContainer >;
    using DCIter = DataContainer::iterator;
    using DCCIter = DataContainer::const_iterator;
    using DCRIter = DataContainer::reverse_iterator;
    using DCRCIter = DataContainer::const_reverse_iterator;
  
    DCIter find( const CoordZ& coordinates );
    
    ICIter find( const CoordZ& coordinates, const DCIter& iter );

    std::shared_ptr<Location> createLocation( const CoordZ& coordinates );
    
    DataContainer data_container;

    bool debug = false;

  };


  inline bool WossCreatorContainer< CustomTransducer >::isEmpty() const {
    return data_container.empty();
  }


  inline int WossCreatorContainer< CustomTransducer >::size() const {
    return data_container.size();
  }


  inline std::shared_ptr<Location> WossCreatorContainer< CustomTransducer >::createLocation( const CoordZ& coordinates ) {
    if ( coordinates == ALL_COORDZ ) 
      return nullptr;
    return std::move(SDefHandler::instance().createLocation(coordinates));
  }


  inline bool WossCreatorContainer< CustomTransducer >::insert( const CustomTransducer& data, const std::shared_ptr<Location>& tx, const std::shared_ptr<Location>& rx ) { 
    auto it = data_container.find(tx);
    if ( it == data_container.end() ) {
      data_container[tx][rx] = data;
      return true;
    }
    auto it2 = it->second.find(rx);
    if ( it2 == it->second.end() ) {
      (it->second)[rx] = data;
      return true;
    }
    return false;
  }
 
 
  inline bool WossCreatorContainer< CustomTransducer >::insert( const CustomTransducer& data, const CoordZ& tx, const CoordZ& rx ) { 
    auto it = find( tx );
    if ( it == data_container.end() ) {
      data_container[ createLocation(tx) ][ createLocation(rx) ] = data; 
      return true;
    }
    auto it2 = find( rx, it );
    if ( it2 == it->second.end() ) {
      (it->second)[ createLocation(rx) ] = data;
      return true;
    }
    return false;
  }
 
  
  inline CustomTransducer& WossCreatorContainer< CustomTransducer >::accessAllLocations() {
    return data_container[ALL_LOCATIONS][ALL_LOCATIONS];
  }
 
 
  inline void WossCreatorContainer< CustomTransducer >::erase( const std::shared_ptr<Location>& tx, const std::shared_ptr<Location>& rx ) { 
    DCIter it = data_container.find(tx);
    if ( it != data_container.end() ) {
      
      ICIter it2 = it->second.find( rx );
      
      if ( it2 != it->second.end() ) {
        it->second.erase( it2 );
      }
      
    }
    if ( it->second.empty() ) { 
      data_container.erase(it);
    }
  }
 
 
  inline void WossCreatorContainer< CustomTransducer >::erase( const CoordZ& tx, const CoordZ& rx ) { 
    for ( DCIter it = data_container.begin(); it != data_container.end();  ) {
      bool tx_found = false;

      if ( it->first != ALL_LOCATIONS ) {
         if ( it->first->isEquivalentTo( tx ) ) 
          tx_found = true;
      } else {
         if ( tx == ALL_COORDZ ) 
          tx_found = true;
      }

      if ( tx_found ) {
        for ( ICIter it2 = it->second.begin(); it2 != it->second.end(); ) {
           bool rx_found = false;

           if ( it2->first != ALL_LOCATIONS ) {
              if ( it2->first->isEquivalentTo( rx ) ) 
                rx_found = true;
           } else {
              if ( rx == ALL_COORDZ ) 
                rx_found = true;
           }

          if ( rx_found ) {
            it->second.erase(it2++);
          }
          else ++it2;
        }
      } 
      if ( it->second.empty() ) {
        data_container.erase(it++);
      }
      else 
        ++it;
    }
  }
 
 
  inline void WossCreatorContainer< CustomTransducer >::replace( const CustomTransducer& data, const std::shared_ptr<Location>& tx, const std::shared_ptr<Location>& rx ) { 
    data_container[tx][rx] = data;
  }
 
 
  inline void WossCreatorContainer< CustomTransducer >::replace( const CustomTransducer& data, const CoordZ& tx, const CoordZ& rx ) { 
    std::shared_ptr<Location> tx_loc = createLocation(tx);
    std::shared_ptr<Location> rx_loc = createLocation(rx);
    data_container[tx_loc][rx_loc] = data;
  }
 
 
  inline void WossCreatorContainer< CustomTransducer >::clear() { 
    data_container.clear();
  }
  
}

#endif // WOSS_CREATOR_CONTAINER_DEFINITIONS_H
