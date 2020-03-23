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
 * @file   woss-creator-container.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossCreatorContainer class
 *
 * Provides the interface for woss::WossCreatorContainer class
 */


#ifndef WOSS_CREATOR_CONTAINER_DEFINITIONS_H
#define WOSS_CREATOR_CONTAINER_DEFINITIONS_H


#include <map>
#include <location-definitions.h>
#include <iostream>


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
    CustomTransducer( const ::std::string& name = "", double bearing = 0.0, double vert_rot = 0.0, double horiz_rot = 0.0, double mult = 1.0, double add = 0.0 ) 
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
    ::std::string type;

    
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
  * transmitter and receiver woss::Location*
  */
  template< typename Data > 
  class WossCreatorContainer {
  
    
    public:
      

    /**
    * Pointer that represents the index for all possible Locations
    */   
    static Location* const ALL_LOCATIONS;

    /**
    * Pointer that represents the index for all possible coordinates
    */  
    static const CoordZ ALL_COORDZ;
    

    /**
    * Default constructor
    */     
    WossCreatorContainer();
    
    /**
    * Destructor
    */     
    ~WossCreatorContainer();
    

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
    * @param tx const pointer to a valid transmitter Location
    * @param rx const pointer to a valid receiver Location
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/     
    bool insert( const Data& data, Location* const tx, Location* const rx );
    
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
    * @param tx const pointer to a valid transmitter Location
    * @param rx const pointer to a valid receiver Location
    * @return a copy of the original object
    **/     
    Data get( Location* const tx, Location* const rx  ) const;

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
    * @param tx const pointer to a valid transmitter Location
    * @param rx const pointer to a valid receiver Location
    **/     
    void erase( Location* const tx, Location* const rx );
    
    /**
    * Erases an object into the container
    * @param tx const reference to a valid transmitter CoordZ
    * @param rx const reference to a valid receiver CoordZ
    **/      
    void erase( const CoordZ& tx, const CoordZ& rx );
    
   
    /**
    * Replaces an object into the container. If the key are not present, the object is still inserted
    * @param data const reference to a Data object to be inserted
    * @param tx const pointer to a valid transmitter Location
    * @param rx const pointer to a valid receiver Location
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/     
    void replace( const Data& data, Location* const tx, Location* const rx );
    
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
    typedef ::std::map< Location*, Data > InnerContainer;
    typedef typename InnerContainer::iterator ICIter;
    typedef typename InnerContainer::reverse_iterator ICRIter;
    typedef typename InnerContainer::const_iterator ICCIter;
    typedef typename InnerContainer::const_reverse_iterator ICCRIter;
    
    
    /**
    * The outer container. It associates a transmitter Location pointer to a InnerContainer object
    */     
    typedef ::std::map< Location*, InnerContainer > DataContainer;
    typedef typename DataContainer::iterator DCIter;
    typedef typename DataContainer::const_iterator DCCIter;
    typedef typename DataContainer::reverse_iterator DCRIter;
    typedef typename DataContainer::const_reverse_iterator DCRCIter;
  
    
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
    * @return a heap-allocated Location pointer
    */    
    Location* createLocation( const CoordZ& coordinates );
    
    
    /**
    * The Data container
    */    
    DataContainer data_container;
    
    
    /**
    * Debug flag
    */    
    bool debug;
    
    
  };
  
  
  template< typename Data > 
  Location* const WossCreatorContainer< Data >::ALL_LOCATIONS = NULL;  
  
  template< typename Data > 
  const CoordZ WossCreatorContainer< Data >::ALL_COORDZ = CoordZ();
 
  
  template< typename Data > 
  WossCreatorContainer< Data >::WossCreatorContainer()
  : data_container(),
    debug(false)
  { 
  }
  
  
  template< typename Data >
  WossCreatorContainer< Data >::~WossCreatorContainer() { 
    clear(); 
  }
   
   
  template< typename Data >
  inline bool WossCreatorContainer< Data >::isEmpty() const {
    return data_container.empty();
  }
  
  
  template< typename Data >
  inline int WossCreatorContainer< Data >::size() const {
    return data_container.size();
  }
  
  
  template< typename Data >
  inline Location* WossCreatorContainer< Data >::createLocation( const CoordZ& coordinates ) {
    if ( coordinates == ALL_COORDZ ) return NULL;
    return new Location( coordinates );
  }

  
  template< typename Data >
  typename WossCreatorContainer< Data >::DCIter WossCreatorContainer< Data >::find( const CoordZ& coordinates ) {
    DCIter it_all_loc = data_container.end();
    
    for ( DCIter it = data_container.begin(); it != data_container.end(); it++ ) {
      if ( it->first == ALL_LOCATIONS ) {
        it_all_loc = it;
        if ( coordinates == ALL_COORDZ ) break;
        else continue;
      }
      if ( it->first->isEquivalentTo( coordinates ) ) {
        if (debug) ::std::cout << "WossCreatorContainer::find() tx coordinates found = " 
                               << coordinates << ::std::endl;      
        return it;
      }
    }
   
    if ( coordinates == ALL_COORDZ && it_all_loc != data_container.end() ) {
      if (debug) ::std::cout << "WossCreatorContainer::find() tx ALL_COORDZ found" << ::std::endl;
      return it_all_loc;
    }
    
    if (debug && coordinates != ALL_COORDZ ) ::std::cout << "WossCreatorContainer::find() tx coordinates not found = " 
                                                         << coordinates << ::std::endl; 
    
    if (debug && coordinates == ALL_COORDZ ) ::std::cout << "WossCreatorContainer::find() tx ALL_COORDZ not found" << ::std::endl;
    return data_container.end();        
  } 
  

  template< typename Data >  
  typename WossCreatorContainer< Data >::ICIter WossCreatorContainer< Data >::find( const CoordZ& coordinates, const DCIter& iter ) {
    ICIter it_all_loc = iter->second.end();
    
    for ( ICIter it = iter->second.begin(); it != iter->second.end(); it++ ) {
      if ( it->first == ALL_LOCATIONS ) {
        it_all_loc = it;
        if ( coordinates == ALL_COORDZ ) break;
        else continue;
      }
      if ( it->first->isEquivalentTo( coordinates ) ) {
        if (debug) ::std::cout << "WossCreatorContainer::find() rx coordinates found = " 
                               << coordinates << ::std::endl;      
        return it;
      }
    }
   
    if ( coordinates == ALL_COORDZ && it_all_loc != iter->second.end() ) {
      if (debug) ::std::cout << "WossCreatorContainer::find() rx ALL_COORDZ found" << ::std::endl;
      return it_all_loc;
    }
    
    if (debug && coordinates != ALL_COORDZ ) ::std::cout << "WossCreatorContainer::find() rx coordinates not found = " 
                                                         << coordinates << ::std::endl; 
    
    if (debug && coordinates == ALL_COORDZ ) ::std::cout << "WossCreatorContainer::find() rx ALL_COORDZ not found" << ::std::endl;
    return iter->second.end(); 
  }
  
  
  template< typename Data >  
  inline bool WossCreatorContainer< Data >::insert( const Data& data, Location* const tx, Location* const rx ) { 
    DCIter it = data_container.find(tx);
    if ( it == data_container.end() ) {
      data_container[tx][rx] = data;
      return true;
    }
    ICIter it2 = it->second.find(rx);
    if ( it2 == it->second.end() ) {
      (it->second)[rx] = data;
      return true;
    }
    return false;
  }
 
 
  template< typename Data >
  inline bool WossCreatorContainer< Data >::insert( const Data& data, const CoordZ& tx, const CoordZ& rx ) { 
    DCIter it = find( tx );
    if ( it == data_container.end() ) {
      data_container[ createLocation(tx) ][ createLocation(rx) ] = data; 
      return true;
    }
    ICIter it2 = find( rx, it );
    if ( it2 == it->second.end() ) {
      (it->second)[ createLocation(rx) ] = data;
      return true;
    }
    return false;
  }
 
 
  template< typename Data >
  inline Data WossCreatorContainer< Data >::get( Location* const tx, Location* const rx ) const { 
    DCIter it = const_cast< typename WossCreatorContainer< Data >::DataContainer& >(data_container).find( tx );
       
    if ( tx != ALL_LOCATIONS ) {
      if ( it == data_container.end() ) {
        if ( debug ) ::std::cout << "WossCreatorContainer::get() no tx location found = " << *tx 
                                 << "; trying ALL_LOCATIONS"<< ::std::endl;

        it = const_cast< typename WossCreatorContainer< Data >::DataContainer& >(data_container).find( ALL_LOCATIONS );
      }

      if ( debug ) ::std::cout << "WossCreatorContainer::get() tx location found = " << *tx << ::std::endl;
      
    }
    
    if ( it != data_container.end() ) {
      ICIter it2 = it->second.find( rx );
      
      if ( rx != ALL_LOCATIONS ) {
      
        if ( it2 == it->second.end() ) {
          if ( debug ) ::std::cout << "WossCreatorContainer::get() no rx location found = " << *rx 
                                   << "; trying ALL_LOCATIONS"<< ::std::endl;

          it2 = it->second.find( ALL_LOCATIONS );
        }
        
        if ( debug ) ::std::cout << "WossCreatorContainer::get() rx location found = " << *rx << ::std::endl;

      }
      
      if ( it2 != it->second.end() ) return it2->second;
    }
     
    ::std::cerr << "WARNING: WossCreatorContainer::get() no tx nor rx location found, returning default constructor!!" << ::std::endl;
    
    return Data();
  }
 
 
  template< typename Data >
  inline Data& WossCreatorContainer< Data >::accessAllLocations() {
    return data_container[ALL_LOCATIONS][ALL_LOCATIONS];
  }
 
  
  template< typename Data >
  inline Data WossCreatorContainer< Data >::get( const CoordZ& tx, const CoordZ& rx ) const { 
    DCIter it = const_cast< WossCreatorContainer< Data >& >(*this).find( tx );
    
    if ( tx != ALL_COORDZ ) {
      if ( it == data_container.end() ) it = const_cast< WossCreatorContainer< Data >& >(*this).find( ALL_COORDZ );
    }
    
    if ( it != data_container.end() ) {
      ICIter it2 = const_cast< WossCreatorContainer< Data >& >(*this).find( rx, it );
      
      if ( rx != ALL_COORDZ ) {
        if ( it2 == it->second.end() ) it2 = const_cast< WossCreatorContainer< Data >& >(*this).find( ALL_COORDZ, it );
      }
          
      if ( it2 != it->second.end() ) {
        if ( debug ) ::std::cout << "WossCreatorContainer::get() value found = " << it2->second << ::std::endl;       
        
        return it2->second;
      }
    }
    ::std::cerr << "WARNING: WossCreatorContainer::get() no tx nor rx coordinates found, returning default constructor!!" << ::std::endl;

    return Data();
  }
 
 
  template< typename Data >
  inline void WossCreatorContainer< Data >::erase( Location* const tx, Location* const rx ) { 
    DCIter it = data_container.find(tx);
    if ( it != data_container.end() ) it->second.erase( rx );
    if ( it->second.empty() ) data_container.erase(it);
  }
 
 
  template< typename Data >
  inline void WossCreatorContainer< Data >::erase( const CoordZ& tx, const CoordZ& rx ) { 
    for ( DCIter it = data_container.begin(); it != data_container.end();  ) {
      
      if ( it->first->isEquivalentTo( tx ) ) {
        
        for ( ICIter it2 = it->second.begin(); it2 != it->second.end(); ) {
          if ( it2->first->isEquivalentTo( rx ) ) it->second.erase(it2++);
          else ++it2;
        }
        
      } 
        
      if ( it->second.empty() ) data_container.erase(it++);
      else ++it;
    }  
  }
 
 
  template< typename Data >
  inline void WossCreatorContainer< Data >::replace( const Data& data, Location* const tx, Location* const rx ) { 
    data_container[tx][rx] = data;
  }
 
 
  template< typename Data >
  inline void WossCreatorContainer< Data >::replace( const Data& data, const CoordZ& tx, const CoordZ& rx ) { 
    data_container[ createLocation(tx) ][ createLocation(rx) ] = data;
  }
 
 
  template< typename Data >
  inline void WossCreatorContainer< Data >::clear() { 
//     for( DCIter it = data_container.begin(); it != data_container.end(); it++ ) {
      
//       for( ICIter it2 = it->second.begin(); it2 != it->second.end(); it2++ ) {
//         
//       }
      
//       it->second.clear();
//     }
    data_container.clear();
  }
  
  
  /////

/**
  * \brief Partial specialization for pointers
  *
  * Partial specialiation for heap-allocated pointers. PLEASE NOTE that object class type must implement clone() 
  * method
  */
   template< typename Data > 
  class WossCreatorContainer< Data* > {
  
    
    public:
      
   
    static Location* const ALL_LOCATIONS;
    
    static const CoordZ ALL_COORDZ;
    
      
    WossCreatorContainer();
    
    ~WossCreatorContainer();
    
    
    bool isEmpty() const;
    
    int size() const;
    

    /**
    * Inserts an object into the container. If the keys are already present, the object is NOT inserted and it
    * is deleted
    * @param data const pointer to a Data object to be inserted
    * @param tx const pointer to a valid transmitter Location
    * @param rx const pointer to a valid receiver Location
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/      
    bool insert( Data* data, Location* const tx, Location* const rx );
    
    /**
    * Inserts an object into the container. If the keys are already present, the object is NOT inserted and it
    * is deleted
    * @param data const pointer to a Data object to be inserted
    * @param tx const reference to a valid transmitter CoordZ
    * @param rx const reference to a valid receiver CoordZ
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/      
    bool insert( Data* data, const CoordZ& tx, const CoordZ& rx );
    

    /**
    * Returns a Data pointer for given keys. If the keys are not found a NULL pointer is returned
    * @param tx const pointer to a valid transmitter Location
    * @param rx const pointer to a valid receiver Location
    * @return a pointer to a cloned Data object
    **/     
    Data* get( Location* const tx, Location* const rx ) const;

    /**
    * Returns a Data pointer for given keys. If the keys are not found a NULL pointer is returned
    * @param tx const reference to a valid transmitter CoordZ
    * @param rx const reference to a valid receiver CoordZ
    * @return a pointer to a cloned Data object
    **/      
    Data* get( const CoordZ& tx, const CoordZ& rx ) const;
    
    
    /**
    * Returns a reference to the Data pointer associated to transmitter and receiver Location equal to ALL_LOCATIONS.
    * If no keys were present, the Data pointer is defaulted to NULL and a reference to it is returned.
    * The user has to properly delete the pointer prior changing it
    * @return a reference to a Data pointer
    **/         
    Data*& accessAllLocations();
       
    
    void erase( Location* const tx, Location* const rx );
    
    void erase( const CoordZ& tx, const CoordZ& rx );
    
    
    /**
    * Replaces a pointer into the container. If the keys are already present, the previous object is deleted
    * @param data const pointer to a Data object to be inserted
    * @param tx const pointer to a valid transmitter Location
    * @param rx const pointer to a valid receiver Location
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/    
    void replace( Data* const data, Location* const tx, Location* const rx );
    
    /**
    * Replaces a pointer into the container. If the keys are already present, the previous object is deleted
    * @param data const pointer to a Data object to be inserted
    * @param tx const reference to a valid transmitter CoordZ
    * @param rx const reference to a valid receiver CoordZ
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/    
    void replace( Data* const data, const CoordZ& tx, const CoordZ& rx );
    
    
    void clear();
    
    
    void setDebug( bool flag ) { debug = flag; }
    
    bool isUsingDebug() const { return debug; }
   
    
    protected:
       
    
    typedef ::std::map< Location*, Data* > InnerContainer;
    typedef typename InnerContainer::iterator ICIter;
    typedef typename InnerContainer::reverse_iterator ICRIter;
    typedef typename InnerContainer::const_iterator ICCIter;
    typedef typename InnerContainer::const_reverse_iterator ICCRIter;
    
    
    typedef ::std::map< Location*, InnerContainer > DataContainer;
    
    typedef typename DataContainer::iterator DCIter;
    typedef typename DataContainer::const_iterator DCCIter;
    typedef typename DataContainer::reverse_iterator DCRIter;
    typedef typename DataContainer::const_reverse_iterator DCRCIter;
  
       
    DCIter find( const CoordZ& coordinates );
    
    ICIter find( const CoordZ& coordinates, const DCIter& iter );
    
    
    Location* createLocation( const CoordZ& coordinates );
    
    
    DataContainer data_container;
    
    
    bool debug;
    
    
  };
  
  
  template< typename Data > 
  Location* const WossCreatorContainer< Data* >::ALL_LOCATIONS = NULL;  
  
  template< typename Data > 
  const CoordZ WossCreatorContainer< Data* >::ALL_COORDZ = CoordZ();
 
  
  template< typename Data > 
  WossCreatorContainer< Data* >::WossCreatorContainer()
  : data_container(),
    debug(false)
  { 
  }
  
  
  template< typename Data >
  WossCreatorContainer< Data* >::~WossCreatorContainer() { 
    clear(); 
  }
   
   
  template< typename Data >
  inline bool WossCreatorContainer< Data* >::isEmpty() const {
    return data_container.empty();
  }
  
  
  template< typename Data >
  inline int WossCreatorContainer< Data* >::size() const {
    return data_container.size();
  }
  
  
  template< typename Data >
  inline Location* WossCreatorContainer< Data* >::createLocation( const CoordZ& coordinates ) {
    if ( coordinates == ALL_COORDZ ) return NULL;
    return new Location( coordinates );
  }

  
  template< typename Data >
  typename WossCreatorContainer< Data* >::DCIter WossCreatorContainer< Data* >::find( const CoordZ& coordinates ) {
    DCIter it_all_loc = data_container.end();
    
    for ( DCIter it = data_container.begin(); it != data_container.end(); it++ ) {
      if ( it->first == ALL_LOCATIONS ) {
        it_all_loc = it;
        if ( coordinates == ALL_COORDZ ) break;
        else continue;
      }
      if ( it->first->isEquivalentTo( coordinates ) ) {
        if (debug) ::std::cout << "WossCreatorContainer*::find() tx coordinates found = " 
                               << coordinates << ::std::endl;      
        return it;
      }
    }
   
    if ( coordinates == ALL_COORDZ && it_all_loc != data_container.end() ) {
      if (debug) ::std::cout << "WossCreatorContainer*::find() tx ALL_COORDZ found" << ::std::endl;
      return it_all_loc;
    }
    
    if (debug && coordinates != ALL_COORDZ ) ::std::cout << "WossCreatorContainer*::find() tx coordinates not found = " 
                                                         << coordinates << ::std::endl; 
    
    if (debug && coordinates == ALL_COORDZ ) ::std::cout << "WossCreatorContainer*::find() tx ALL_COORDZ not found" << ::std::endl;
    return data_container.end();        
  } 
  

  template< typename Data >  
  typename WossCreatorContainer< Data* >::ICIter WossCreatorContainer< Data* >::find( const CoordZ& coordinates, const DCIter& iter ) {
    ICIter it_all_loc = iter->second.end();
    
    for ( ICIter it = iter->second.begin(); it != iter->second.end(); it++ ) {
      if ( it->first == ALL_LOCATIONS ) {
        it_all_loc = it;
        if ( coordinates == ALL_COORDZ ) break;
        else continue;
      }
      if ( it->first->isEquivalentTo( coordinates ) ) {
        if (debug) ::std::cout << "WossCreatorContainer*::find() rx coordinates found = " 
                               << coordinates << ::std::endl;      
        return it;
      }
    }
   
    if ( coordinates == ALL_COORDZ && it_all_loc != iter->second.end() ) {
      if (debug) ::std::cout << "WossCreatorContainer*::find() rx ALL_COORDZ found" << ::std::endl;
      return it_all_loc;
    }
    
    if (debug && coordinates != ALL_COORDZ ) ::std::cout << "WossCreatorContainer*::find() rx coordinates not found = " 
                                                         << coordinates << ::std::endl; 
    
    if (debug && coordinates == ALL_COORDZ ) ::std::cout << "WossCreatorContainer*::find() rx ALL_COORDZ not found" << ::std::endl;
    return iter->second.end(); 
  }
  
  
  template< typename Data >  
  inline bool WossCreatorContainer< Data* >::insert( Data* data, Location* const tx, Location* const rx ) { 
    DCIter it = data_container.find(tx);
    if ( it == data_container.end() ) {
      data_container[tx][rx] = data;
      return true;
    }
    ICIter it2 = it->second.find(rx);
    if ( it2 == it->second.end() ) {
      (it->second)[rx] = data;
      return true;
    }
    
    delete data;
    data = NULL;
    return false;
  }
 
 
  template< typename Data >
  inline bool WossCreatorContainer< Data* >::insert( Data* data, const CoordZ& tx, const CoordZ& rx ) { 
    DCIter it = find( tx );
    if ( it == data_container.end() ) {
      data_container[ createLocation(tx) ][ createLocation(rx) ] = data; 
      return true;
    }
    ICIter it2 = find( rx, it );
    if ( it2 == it->second.end() ) {
      (it->second)[ createLocation(rx) ] = data;
      return true;
    }
    
    delete data;
    data = NULL;
    return false;
  }
 
 
  template< typename Data >
  inline Data* WossCreatorContainer< Data* >::get( Location* const tx, Location* const rx ) const { 
    DCIter it = const_cast< typename WossCreatorContainer< Data* >::DataContainer& >(data_container).find( tx );
       
    if ( tx != ALL_LOCATIONS ) {
      if ( it == data_container.end() ) {
        if ( debug ) ::std::cout << "WossCreatorContainer*::get() no tx location found = " << *tx 
                                 << "; trying ALL_LOCATIONS"<< ::std::endl;

        it = const_cast< typename WossCreatorContainer< Data* >::DataContainer& >(data_container).find( ALL_LOCATIONS );
      }

      if ( debug ) ::std::cout << "WossCreatorContainer*::get() tx location found = " << *tx << ::std::endl;
      
    }
    
    if ( it != data_container.end() ) {
      ICIter it2 = it->second.find( rx );
      
      if ( rx != ALL_LOCATIONS ) {
      
        if ( it2 == it->second.end() ) {
          if ( debug ) ::std::cout << "WossCreatorContainer*::get() no rx location found = " << *rx 
                                   << "; trying ALL_LOCATIONS"<< ::std::endl;

          it2 = it->second.find( ALL_LOCATIONS );
        }
        
        if ( debug ) ::std::cout << "WossCreatorContainer*::get() rx location found = " << *rx << ::std::endl;
      }

      if ( it2 != it->second.end() ) return it2->second->clone();
    }
      
    if ( debug ) ::std::cerr << "WARNING: WossCreatorContainer*::get() no tx nor rx location found, returning default constructor!!" << ::std::endl;
    
    return new Data();
  }
 
 
  template< typename Data >
  inline Data*& WossCreatorContainer< Data* >::accessAllLocations() {
    return data_container[ALL_LOCATIONS][ALL_LOCATIONS];
  }
 
  
  template< typename Data >
  inline Data* WossCreatorContainer< Data* >::get( const CoordZ& tx, const CoordZ& rx ) const { 
    DCIter it = const_cast< WossCreatorContainer< Data* >& >(*this).find( tx );
    
    if ( tx != ALL_COORDZ ) {
      if ( it == data_container.end() ) it = const_cast< WossCreatorContainer< Data* >& >(*this).find( ALL_COORDZ );
    }
    
    if ( it != data_container.end() ) {
      ICIter it2 = const_cast< WossCreatorContainer< Data* >& >(*this).find( rx, it );
      
      if ( rx != ALL_COORDZ ) {
        if ( it2 == it->second.end() ) it2 = const_cast< WossCreatorContainer< Data* >& >(*this).find( ALL_COORDZ, it );
      }
          
      if ( it2 != it->second.end() ) return it2->second->clone();
    }
    if ( debug ) ::std::cerr << "WARNING: WossCreatorContainer*::get() no tx nor rx coordinates found, returning default constructor!!" << ::std::endl;

    return new Data();
  }
 
 
  template< typename Data >
  inline void WossCreatorContainer< Data* >::erase( Location* const tx, Location* const rx ) { 
    DCIter it = data_container.find(tx);
    if ( it != data_container.end() ) {
      
      ICIter it2 = it->second.find( rx );
      
      if ( it2 != it->second.end() ) {
        delete it2->second;
//         delete it2->first;
        it->second.erase( it2 );
      }
      
    }
    if ( it->second.empty() ) { 
//       delete it->first;
      data_container.erase(it);
    }
  }
 
 
  template< typename Data >
  inline void WossCreatorContainer< Data* >::erase( const CoordZ& tx, const CoordZ& rx ) { 
    for ( DCIter it = data_container.begin(); it != data_container.end();  ) {
      
      if ( it->first->isEquivalentTo( tx ) ) {
        
        for ( ICIter it2 = it->second.begin(); it2 != it->second.end(); ) {
          if ( it2->first->isEquivalentTo( rx ) ) {
            delete it2->second;
//             delete it2->first;
            it->second.erase(it2++);
          }
          else ++it2;
        }
        
      } 
        
      if ( it->second.empty() ) {
//         delete it->first;
        data_container.erase(it++);
      }
      else ++it;
    }  
  }
 
 
  template< typename Data >
  inline void WossCreatorContainer< Data* >::replace( Data* const data, Location* const tx, Location* const rx ) { 
    if ( data_container[tx][rx] != NULL ) delete data_container[tx][rx];
    data_container[tx][rx] = data;
  }
 
 
  template< typename Data >
  inline void WossCreatorContainer< Data* >::replace( Data* const data, const CoordZ& tx, const CoordZ& rx ) { 
    Location* tx_loc = createLocation(tx);
    Location* rx_loc = createLocation(rx);
    
    if ( data_container[tx_loc][rx_loc] != NULL ) delete data_container[tx_loc][rx_loc];
    data_container[tx_loc][rx_loc] = data;
  }
 
 
  template< typename Data >
  inline void WossCreatorContainer< Data* >::clear() { 
    for( DCIter it = data_container.begin(); it != data_container.end(); it++ ) {
      
      for( ICIter it2 = it->second.begin(); it2 != it->second.end(); it2++ ) {
        delete it2->second;
      }

      it->second.clear();
//       delete it->first;
    }
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
      
   
    static Location* const ALL_LOCATIONS;
    
    static const CoordZ ALL_COORDZ;
    
      
    WossCreatorContainer();
    
    ~WossCreatorContainer();
    
    
    bool isEmpty() const;
    
    int size() const;
    

    /**
    * Inserts an object into the container. If the keys are already present, the object is NOT inserted and it
    * is deleted
    * @param data const reference to a CustomTransducer object to be inserted
    * @param tx const pointer to a valid transmitter Location
    * @param rx const pointer to a valid receiver Location
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/      
    bool insert( const CustomTransducer& data, Location* const tx, Location* const rx );
    
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
    * Returns a CustomTransducer for given keys. If the keys are not found a NULL pointer is returned
    * @param tx const pointer to a valid transmitter Location
    * @param rx const pointer to a valid receiver Location
    * @return a pointer to a rotated CustomTransducer object
    **/     
    CustomTransducer get( Location* const tx, Location* const rx ) const;

    /**
    * Returns a CustomTransducer pointer for given keys. If the keys are not found a NULL pointer is returned
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
       
    
    void erase( Location* const tx, Location* const rx );
    
    void erase( const CoordZ& tx, const CoordZ& rx );
    
    
    /**
    * Replaces a pointer into the container. If the keys are already present, the previous object is deleted
    * @param data const reference to a CustomTransducer object to be inserted
    * @param tx const pointer to a valid transmitter Location
    * @param rx const pointer to a valid receiver Location
    * @return <i>true</i> if it's empty, <i>false</i> otherwise
    **/    
    void replace( const CustomTransducer& data, Location* const tx, Location* const rx );
    
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
       
    
    typedef ::std::map< Location*, CustomTransducer > InnerContainer;
    typedef InnerContainer::iterator ICIter;
    typedef InnerContainer::reverse_iterator ICRIter;
    typedef InnerContainer::const_iterator ICCIter;
    typedef InnerContainer::const_reverse_iterator ICCRIter;
    
    
    typedef ::std::map< Location*, InnerContainer > DataContainer;
    
    typedef DataContainer::iterator DCIter;
    typedef DataContainer::const_iterator DCCIter;
    typedef DataContainer::reverse_iterator DCRIter;
    typedef DataContainer::const_reverse_iterator DCRCIter;
  
       
    DCIter find( const CoordZ& coordinates );
    
    ICIter find( const CoordZ& coordinates, const DCIter& iter );
    
    
    Location* createLocation( const CoordZ& coordinates );
    
    
    DataContainer data_container;
    
    
    bool debug;
    
    
  };
     
   
  inline bool WossCreatorContainer< CustomTransducer >::isEmpty() const {
    return data_container.empty();
  }
  
  
  inline int WossCreatorContainer< CustomTransducer >::size() const {
    return data_container.size();
  }
  
  
  inline Location* WossCreatorContainer< CustomTransducer >::createLocation( const CoordZ& coordinates ) {
    if ( coordinates == ALL_COORDZ ) return NULL;
    return new Location( coordinates );
  }
 
  
  inline bool WossCreatorContainer< CustomTransducer >::insert( const CustomTransducer& data, Location* const tx, Location* const rx ) { 
    DCIter it = data_container.find(tx);
    if ( it == data_container.end() ) {
      data_container[tx][rx] = data;
      return true;
    }
    ICIter it2 = it->second.find(rx);
    if ( it2 == it->second.end() ) {
      (it->second)[rx] = data;
      return true;
    }
    return false;
  }
 
 
  inline bool WossCreatorContainer< CustomTransducer >::insert( const CustomTransducer& data, const CoordZ& tx, const CoordZ& rx ) { 
    DCIter it = find( tx );
    if ( it == data_container.end() ) {
      data_container[ createLocation(tx) ][ createLocation(rx) ] = data; 
      return true;
    }
    ICIter it2 = find( rx, it );
    if ( it2 == it->second.end() ) {
      (it->second)[ createLocation(rx) ] = data;
      return true;
    }
    return false;
  }
 
  
  inline CustomTransducer& WossCreatorContainer< CustomTransducer >::accessAllLocations() {
    return data_container[ALL_LOCATIONS][ALL_LOCATIONS];
  }
 
 
  inline void WossCreatorContainer< CustomTransducer >::erase( Location* const tx, Location* const rx ) { 
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
      
      if ( it->first->isEquivalentTo( tx ) ) {
        
        for ( ICIter it2 = it->second.begin(); it2 != it->second.end(); ) {
          if ( it2->first->isEquivalentTo( rx ) ) {
            it->second.erase(it2++);
          }
          else ++it2;
        }
        
      } 
        
      if ( it->second.empty() ) {
        data_container.erase(it++);
      }
      else ++it;
    }  
  }
 
 
  inline void WossCreatorContainer< CustomTransducer >::replace( const CustomTransducer& data, Location* const tx, Location* const rx ) { 
    data_container[tx][rx] = data;
  }
 
 
  inline void WossCreatorContainer< CustomTransducer >::replace( const CustomTransducer& data, const CoordZ& tx, const CoordZ& rx ) { 
    Location* tx_loc = createLocation(tx);
    Location* rx_loc = createLocation(rx);
    
    data_container[tx_loc][rx_loc] = data;
  }
 
 
  inline void WossCreatorContainer< CustomTransducer >::clear() { 
//     for( DCIter it = data_container.begin(); it != data_container.end(); it++ ) {
//       for( ICIter it2 = it->second.begin(); it2 != it->second.end(); it2++ ) {
//         
//       }      
//     }
    data_container.clear();
  }
  
}
  
  
#endif // WOSS_CREATOR_CONTAINER_DEFINITIONS_H
