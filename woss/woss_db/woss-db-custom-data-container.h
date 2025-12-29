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
 * @file   woss-db-custom-data-container.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::CustomDataContainer class
 *
 * Provides the interface for the woss::CustomDataContainer template class
 */


#ifndef WOSS_DB_CUSTOM_DATA_CONTAINER_H 
#define WOSS_DB_CUSTOM_DATA_CONTAINER_H


#include <map>
#include <iostream>
#include <time-definitions.h>
#include <complex>
#include <optional>
#include <memory>

namespace woss {

  /**
  * \brief Class for managing custom db data
  *
  * CustomDataContainer is a template that manages environmental data provided 
  * by the user. It sorts data by the first template parameter then by two doubles, (e.g. bearing 
  * and range)
  */
  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp = std::less<T>, class MidComp = std::less<double>, class InComp = std::less<double> > 
  class CustomDataContainer {

    protected:
    
    /**
   * The inner level map, that links a double to actual data
   */
    using InnerData = typename std::map< double, Data, InComp >;
    using CDCInnerIt = typename InnerData::iterator;
    using CDCInnerRIt = typename InnerData::reverse_iterator;
    using CDCInnerCIt = typename InnerData::const_iterator;
    using CDCInnerCRIt = typename InnerData::const_reverse_iterator;
    
    /**
   * The medium level map, that links a double to a InnerData instance
   */
    using MediumData = typename std::map< double, InnerData, MidComp >;
    using CDCMediumIt = typename MediumData::iterator;
    using CDCMediumCIt = typename MediumData::const_iterator;
    using CDCMediumRIt = typename MediumData::reverse_iterator;
    using CDCMediumCRIt = typename MediumData::const_reverse_iterator;
      
    /**
   * The outer level map, that links a the first template parameter to a MediumData instance
   */
    using CustomContainer = typename std::map< T, MediumData, OutComp >;
    using CDCIt = typename CustomContainer::iterator;
    using CDCRIt = typename CustomContainer::reverse_iterator;
    using CDCCIt = typename CustomContainer::const_iterator;
    using CDCCRIt = typename CustomContainer::const_reverse_iterator;

    public:
      
  #if __cplusplus >= 201103L // C++11 or later
    static constexpr double DB_CDATA_ALL_MEDIUM_KEYS = -190.0;
    
    static constexpr double DB_CDATA_ALL_INNER_KEYS = -10.0;
  #else
    static const double DB_CDATA_ALL_MEDIUM_KEYS = -190.0;
    
    static const double DB_CDATA_ALL_INNER_KEYS = -10.0;
  #endif // __cplusplus == 201103L
    
    static const T DB_CDATA_ALL_OUTER_KEYS;
    
    
    /**
   * CustomDataContainer default constructor
   */
    CustomDataContainer() = default; 

    /**
   * CustomDataContainer destructor
   */
    ~CustomDataContainer() = default;

    
    /**
   * operator[]
   * @param key a const reference to a T type ( first class parameter of template )
   * @returns a reference to the linked MediumData
   */
    MediumData& operator[] ( const T& key ) { return data_map[key]; }
    
    
    /**
   * Checks if the outer map is empty
   * @returns <i>true</i> if it is empty, <i>false</i> otherwise
   */
    bool empty() const { return data_map.empty(); }
    
    
    /**
   * Returns the size of the outer map
   * @returns the size
   */   
    int size() const { return data_map.size(); }


    /**
   * Finds and returns a const pointer to a Data for given parameters
   * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
   *          Default value means valid for all possible instances of T
   * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
   *          Default value means valid for all possible doubles of MediumData
   * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
   *          Default value means valid for all possible doubles of InnerData
   * @returns std::optional to Data
   */   
    const std::optional< Data > get( const T& t = DB_CDATA_ALL_OUTER_KEYS, double b = DB_CDATA_ALL_MEDIUM_KEYS, double r = DB_CDATA_ALL_INNER_KEYS ) const;

    /**
   * Finds and returns a const pointer to a Data for the nearest match in all generated keys 
   * @param generator const reference to a T instance
   * @param tobefound const reference to a T instance
    * @returns std::optional to Data
    */   
    const std::optional< Data > get( const T& tx, const T& rx ) const;

    /**
   * Inserts a Data object for given parameters. If keys are already present, the object is discarded
   * @param data const reference to a Data object to be inserted
   * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
   *          Default value means valid for all possible instances of T
   * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
   *          Default value means valid for all possible doubles of MediumData
   * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
   *          Default value means valid for all possible doubles of InnerData
   * @returns <i>true</i> if function was successful, <i>false</i> otherwise
   */
    bool insert( const Data& data, const T& t = DB_CDATA_ALL_OUTER_KEYS, double b = DB_CDATA_ALL_MEDIUM_KEYS, double r = DB_CDATA_ALL_INNER_KEYS );

    /**
   * Replaces a Data object for given parameters. If keys are not present, the object is inserted
   * @param data const reference to a Data object to be inserted
   * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
   *          Default value means valid for all possible instances of T
   * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
   *          Default value means valid for all possible doubles of MediumData
   * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
   *          Default value means valid for all possible doubles of InnerData
   * @returns <i>true</i> if function was successful, <i>false</i> otherwise
   */
    void replace( const Data& data, const T& t = DB_CDATA_ALL_OUTER_KEYS, double b = DB_CDATA_ALL_MEDIUM_KEYS, double r = DB_CDATA_ALL_INNER_KEYS );
    
    
    /**
   * Erases a Data object for given parameters.
   * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
   *          Default value means valid for all possible instances of T
   * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
   *          Default value means valid for all possible doubles of MediumData
   * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
   *          Default value means valid for all possible doubles of InnerData
   * @returns <i>true</i> if function was successful, <i>false</i> otherwise
   */
    void erase( const T& t = DB_CDATA_ALL_OUTER_KEYS, double b = DB_CDATA_ALL_MEDIUM_KEYS, double r = DB_CDATA_ALL_INNER_KEYS );


    /**
   * Clears the map
   */
    void clear();


    /**
   * Sets debug flag
   * @param flag debug flag
   **/
    void setDebug( bool flag ) { debug = flag; }
    
    
    /**
   * Gets debug flag
   * @returns flag debug flag
   **/
    bool usingDebug() { return debug; }
    
    
    protected:
      

    /**
   * Debug flag
   **/
    bool debug = false;

    /**
   * data map
   **/ 
    CustomContainer data_map;

    /**
   * Finds the given keys.
   * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
   *          Default value means valid for all possible instances of T
   * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
   *          Default value means valid for all possible doubles of MediumData
   * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
   *          Default value means valid for all possible doubles of InnerData
   * @returns std::optional to a Data object
   */
    std::optional< Data > find( const T& t = DB_CDATA_ALL_OUTER_KEYS, double b = DB_CDATA_ALL_MEDIUM_KEYS, double r = DB_CDATA_ALL_INNER_KEYS ) const;

  };


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp > 
  const T CustomDataContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::DB_CDATA_ALL_OUTER_KEYS = T();  


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp > 
  const std::optional< Data > CustomDataContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::get( const T& t, double b, double r ) const {
    if ( data_map.empty() ) 
      return {};
    
    if ( debug ) 
      std::cout << "CustomDataContainer::get() t = " << t << "; b = " << b << "; r = " << r << std::endl;

    auto val = find();
    if ( val )
      return val;
    
    CDCCIt it = data_map.find( t );
    if ( it == data_map.end() )
      return {};

    CDCMediumCIt it2 = it->second.lower_bound( b );
    CDCMediumCRIt rit2 = it->second.rbegin();

    CDCInnerCIt it3;
    CDCInnerCRIt rit3;
    
    if ( it2 == it->second.end() ) {
      if( rit2 == it->second.rend() ) 
        return {};
      it3 = rit2->second.lower_bound( r ); 
      rit3 = rit2->second.rbegin();
      if ( it3 != rit2->second.end() ) 
        return it3->second;
      if ( rit3 != rit2->second.rend() ) 
        return rit3->second;
      return {};
    }
    else {
      it3 = it2->second.lower_bound( r );
      rit3 = it2->second.rbegin();
      if ( it3 != it2->second.end() ) 
        return it3->second;
      if ( rit3 != it2->second.rend() ) 
        return rit3->second;
      return {};
    }
    if ( it3 == it2->second.end() ) 
      return {};
    return (it3->second);
  }
  

  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp > 
  const std::optional< Data > CustomDataContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::get( const T& tx, const T& rx ) const {
    MidFunctor mid_funct;
    InFunctor in_funct;
    
    double curr_b;
    double curr_r;
    double delta_b;
    double curr_dist;
    double min_dist = INFINITY;
    std::optional< Data > ret_val = {};
    
    if ( data_map.empty() == true ) {
      if ( debug ) std::cout << "CustomDataContainer::get() data_map is empty " << std::endl;
      
      return {};
    }
    
    for ( auto it = data_map.cbegin(); it != data_map.cend(); it++ ) {
      if ( debug ) 
        std::cout << "CustomDataContainer::get() start T = " << it->first << "; end T = " << rx << std::endl;
      
      if ( it->first == DB_CDATA_ALL_OUTER_KEYS ) {
        if ( debug ) 
          std::cout << "CustomDataContainer::get() overriding start T = " << tx << "; end T = " << rx << std::endl;

        curr_b = mid_funct(tx,rx);
        curr_r = in_funct(tx,rx);
      }
      else {
        curr_b = mid_funct(it->first,rx);
        curr_r = in_funct(it->first,rx);
      }
      
      if ( debug ) 
        std::cout << "CustomDataContainer::get() curr bearing = " << curr_b * 180.0 / M_PI 
                  << "; curr range = " << curr_r << std::endl;
      
      auto itb = it->second.cbegin();
      if ( itb->first == DB_CDATA_ALL_MEDIUM_KEYS ) 
        delta_b = 0;
      else { 
        itb = it->second.lower_bound( curr_b );
        if ( itb == it->second.cend() ) 
          itb = (++(it->second.rbegin())).base();
        
        delta_b = curr_b - itb->first;
        if (delta_b < 0.0) 
          delta_b = -delta_b;
        if (delta_b > M_PI) 
          delta_b = 2.0*M_PI - delta_b ;
      }
      
      double ort_dist = curr_r * sin(delta_b);
      double ort_projection = std::sqrt( curr_r*curr_r - ort_dist*ort_dist ); 
      
      if ( debug ) 
        std::cout << "CustomDataContainer::get() nearest bearing = " << itb->first * 180.0 / M_PI 
                  << "; diff bearing = " << delta_b * 180.0 / M_PI << "; orthog distance = " << ort_dist 
                  << "; orthog range projection = " << ort_projection << std::endl;

      auto itr = itb->second.cbegin();
      if ( itr->first == DB_CDATA_ALL_INNER_KEYS ) 
        curr_dist = ort_dist;
      else {
        itr = itb->second.lower_bound( ort_projection );
        if ( itr == itb->second.cbegin() || itr == itb->second.cend() || itr->first == ort_projection ) {
          if ( itr == itb->second.cend() ) 
            itr = (++(itb->second.rbegin())).base();
          double adj_distance = std::abs( ort_projection - itr->first );
          curr_dist = std::sqrt( ort_projection*ort_projection + adj_distance*adj_distance );
        }
        else {
          double adj_distance = std::abs( ort_projection - itr->first );
          double first_dist = std::sqrt( ort_projection*ort_projection + adj_distance*adj_distance );

          if(debug) 
            std::cout << "CustomDataContainer::get() first try, range = " << itr->first 
                      << "; dist = " << first_dist << std::endl; 

          itr--;
          adj_distance = std::abs( ort_projection - itr->first );
          double before_dist = std::sqrt( ort_projection*ort_projection + adj_distance*adj_distance );
          
          if (debug) 
            std::cout << "CustomDataContainer::get() second try, range = " << itr->first 
                      << "; dist = " << before_dist << std::endl; 
                      
          curr_dist = std::min( first_dist, before_dist );
          if ( curr_dist == first_dist ) 
            itr++;
        }
      }
      if ( debug )
         std::cout << "CustomDataContainer::get() nearest range = " << itr->first << "; distance = " << curr_dist 
                   << "; min distance = " << min_dist << std::endl;

      if ( curr_dist < min_dist ) {
        min_dist = curr_dist;
        ret_val = itr->second;
        if ( curr_dist == 0 ) 
          break;
      }
      
    }

    if ( debug && ret_val ) 
      std::cout << "CustomDataContainer::get() ret value " << *ret_val << std::endl; 

    return ret_val;
  }
  
  
  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp > 
  std::optional< Data > CustomDataContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::find( const T& t, double b, double r ) const {

    if (this->debug) 
      std::cout << "CustomDataContainer::find() << t = " << t << "; b = " << b << "; r = " << r << std::endl; 
      
    CDCCIt it = data_map.find( t );
    if ( it == data_map.end() ) {
      
      if ( debug ) 
        std::cout << "CustomDataContainer::find() t not found" << std::endl;

      return {};
    }
    
    if ( debug ) 
      std::cout << "CustomDataContainer::find() t found" << std::endl;

    CDCMediumCIt it2 = it->second.find( b );
    if ( it2 == it->second.end() ) {
      if ( debug ) 
        std::cout << "CustomDataContainer::find() b not found" << std::endl;

      return {};
    }
    
    if ( debug ) 
      std::cout << "CustomDataContainer::find() b found" << std::endl;
      
    CDCInnerCIt it3 = it2->second.find( r );
    if ( it3 == it2->second.end() ) {
      
      if ( debug ) 
        std::cout << "CustomDataContainer::find() r not found" << std::endl;
        
      return {};
    }
    if ( debug ) 
      std::cout << "CustomDataContainer::find() r found, data = " << it3->second << std::endl;
    return it3->second;
  }


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp > 
  bool CustomDataContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::insert( const Data& d, const T& t, double b, double r ) {
    auto res = find( t, b, r );
    if ( res ) 
      return false;
    data_map[t][b][r] = d;
    return true;
  } 


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp > 
  void CustomDataContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::replace( const Data& d, const T& t, double b, double r ) {
    data_map[t][b].erase(r);
    data_map[t][b][r] = d;
  } 


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp > 
  void CustomDataContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::erase( const T& t, double b, double r ) {
    data_map[t][b].erase(r);
    if ( data_map[t][b].empty() ) 
      data_map[t].erase(b);
    if ( data_map[t].empty() ) 
      data_map.erase(t);
  } 


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp > 
  void CustomDataContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::clear() {
    data_map.clear();
  }


  /**
  * \brief Class for managing custom db data
  *
  * CustomDataTimeContainer is a template that manages environmental data provided 
  * by the user. It sorts data by the first template parameter then by two doubles, (e.g. bearing 
  * and range) and woss::Time
  */
  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp = std::less<T>, class MidComp = std::less<double>, class InComp = std::less<double> > 
  class CustomDataTimeContainer {

    protected:
    
    /**
    * The Time map, that links a time_t to actual data
    */    
    using TimeData = typename std::map< time_t, Data >;
    using CDTCTimeIt = typename TimeData::iterator;
    using CDTCTimeRIt = typename TimeData::reverse_iterator;
    using CDTCTimeCIt = typename TimeData::const_iterator;
    using CDTCTimeCRIt = typename TimeData::const_reverse_iterator;
      
    /**
    * The inner level map, that links a double to TimeData
    */
    using InnerData = typename std::map< double, TimeData, InComp >;
    using CDCInnerIt = typename InnerData::iterator;
    using CDCInnerRIt = typename InnerData::reverse_iterator;
    using CDCInnerCIt = typename InnerData::const_iterator;
    using CDCInnerCRIt = typename InnerData::const_reverse_iterator;
    
    /**
    * The medium level map, that links a double to a InnerData instance
    */
   using MediumData = typename std::map< double, InnerData, MidComp >;
   using CDCMediumIt = typename MediumData::iterator;
   using CDCMediumCIt = typename MediumData::const_iterator;
   using CDCMediumRIt = typename MediumData::reverse_iterator;
   using CDCMediumCRIt = typename MediumData::const_reverse_iterator;
    
    /**
    * The outer level map, that links a the first template parameter to a MediumData instance
    */
   using CustomContainer = typename std::map< T, MediumData, OutComp >;
   using CDCIt = typename CustomContainer::iterator;
   using CDCRIt = typename CustomContainer::reverse_iterator;
   using CDCCIt = typename CustomContainer::const_iterator;
   using CDCCRIt = typename CustomContainer::const_reverse_iterator;

    public:

    static constexpr inline double DB_CDATA_ALL_MEDIUM_KEYS = -190.0;
    
    static constexpr inline double DB_CDATA_ALL_INNER_KEYS = -10.0;

    static const T DB_CDATA_ALL_OUTER_KEYS;  
    
    static const Time DB_CDATA_ALL_TIME_KEYS;

    /**
     * CustomDataTimeContainer default constructor
    */
    CustomDataTimeContainer() : debug(false), data_map() { }  
    
    /**
     * CustomDataTimeContainer destructor
    */
    ~CustomDataTimeContainer() = default;
    
    /**
    * operator[]
    * @param key a const reference to a T type ( first class parameter of template )
    * @returns a reference to the linked MediumData
    */
    MediumData& operator[] ( const T& key ) { return data_map[key]; }
    
    
    /**
    * Checks if the outer map is empty
    * @returns <i>true</i> if it is empty, <i>false</i> otherwise
    */
    bool empty() const { return data_map.empty(); }
    
    
    /**
    * Returns the size of the outer map
    * @returns the size
    */   
    int size() const { return data_map.size(); }
    
    
    /**
    * Finds and returns a std.:optional to a Data for given parameters
    * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
    *          Default value means valid for all possible instances of T
    * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
    *          Default value means valid for all possible doubles of MediumData
    * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
    *          Default value means valid for all possible doubles of InnerData
    * @param time_key const reference to a Time instance, default value is DB_CDATA_ALL_TIME_KEYS.
    *          Default value means it is valid for all possible Time values of TimeData
    * @returns std::optional to a Data object
    */
    std::optional<Data> get( const T& t = DB_CDATA_ALL_OUTER_KEYS, double b = DB_CDATA_ALL_MEDIUM_KEYS, double r = DB_CDATA_ALL_INNER_KEYS, const Time& time_key = DB_CDATA_ALL_TIME_KEYS ) const;
    
    /**
    * Finds and returns a std::optional to a Data for the nearest match in all generated keys 
    * @param tx const reference to a T instance
    * @param rx const reference to a T instance
    * @param time_key const reference to a Time instance, default value is DB_CDATA_ALL_TIME_KEYS.
    *          Default value means it is valid for all possible Time values of TimeData
    * @returns std::optional to a Data object
    */
    std::optional<Data> get( const T& tx, const T& rx, const Time& time_key = DB_CDATA_ALL_TIME_KEYS ) const;

    /**
    * Inserts a Data object for given parameters. If keys are already present, the object is discarded
    * @param data const reference to a Data object to be inserted
    * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
    *          Default value means valid for all possible instances of T
    * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
    *          Default value means valid for all possible doubles of MediumData
    * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
    *          Default value means valid for all possible doubles of InnerData
    * @param time_key const reference to a Time instance, default value is DB_CDATA_ALL_TIME_KEYS.
    *          Default value means it is valid for all possible Time values of TimeData
    * @returns <i>true</i> if function was successful, <i>false</i> otherwise
    */
    bool insert( const Data& data, const T& t = DB_CDATA_ALL_OUTER_KEYS, double b = DB_CDATA_ALL_MEDIUM_KEYS, double r = DB_CDATA_ALL_INNER_KEYS, const Time& time_key = DB_CDATA_ALL_TIME_KEYS );
    
    /**
    * Replaces a Data object for given parameters. If keys are not present, the object is inserted
    * @param data const reference to a Data object to be inserted
    * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
    *          Default value means valid for all possible instances of T
    * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
    *          Default value means valid for all possible doubles of MediumData
    * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
    *          Default value means valid for all possible doubles of InnerData
    * @param time_key const reference to a Time instance, default value is DB_CDATA_ALL_TIME_KEYS.
    *          Default value means it is valid for all possible Time values of TimeData
    * @returns <i>true</i> if function was successful, <i>false</i> otherwise
    */
    void replace( const Data& data, const T& t = DB_CDATA_ALL_OUTER_KEYS, double b = DB_CDATA_ALL_MEDIUM_KEYS, double r = DB_CDATA_ALL_INNER_KEYS, const Time& time_key = DB_CDATA_ALL_TIME_KEYS );
       
    /**
    * Erases a Data object for given parameters.
    * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
    *          Default value means valid for all possible instances of T
    * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
    *          Default value means valid for all possible doubles of MediumData
    * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
    *          Default value means valid for all possible doubles of InnerData
    * @param time_key const reference to a Time instance, default value is DB_CDATA_ALL_TIME_KEYS.
    *          Default value means it is valid for all possible Time values of TimeData
    * @returns <i>true</i> if function was successful, <i>false</i> otherwise
    */
    void erase( const T& t = DB_CDATA_ALL_OUTER_KEYS, double b = DB_CDATA_ALL_MEDIUM_KEYS, double r = DB_CDATA_ALL_INNER_KEYS, const Time& time_key = DB_CDATA_ALL_TIME_KEYS);
   
    
    /**
    * Clears the map
    */
    void clear();
    
    
    /**
    * Sets debug flag
    * @param flag debug flag
    **/
    void setDebug( bool flag ) { debug = flag; }
    
    
    /**
    * Gets debug flag
    * @returns flag debug flag
    **/
    bool usingDebug() const { return debug; }
    
    
    protected:
    
    
    /**
    * Debug flag
    **/
    bool debug;

    /**
    * data map
    **/ 
    CustomContainer data_map;
    
    /**
    * Finds the given keys. Returns a std::optional to a Data
    * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
    *          Default value means valid for all possible instances of T
    * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
    *          Default value means valid for all possible doubles of MediumData
    * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
    *          Default value means valid for all possible doubles of InnerData
    * @param time_key const reference to a Time instance, default value is DB_CDATA_ALL_TIME_KEYS.
    *          Default value means it is valid for all possible Time values of TimeData  
    * @returns std::optional to a Data object
    */
    std::optional<Data> find( const T& t = DB_CDATA_ALL_OUTER_KEYS, double b = DB_CDATA_ALL_MEDIUM_KEYS, double r = DB_CDATA_ALL_INNER_KEYS, const Time& time_key = DB_CDATA_ALL_TIME_KEYS ) const;
    
    /**
    * Finds the given keys. Returns a std::optional
    * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
    *          Default value means valid for all possible instances of T
    * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
    *          Default value means valid for all possible doubles of MediumData
    * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
    *          Default value means valid for all possible doubles of InnerData
    * @param time_key const reference to a Time instance, default value is DB_CDATA_ALL_TIME_KEYS.
    *          Default value means it is valid for all possible Time values of TimeData  
    * @returns std::optional to a Data object
    */
    std::optional<Data> calculateData( const TimeData& time_data , const Time& time_key = DB_CDATA_ALL_TIME_KEYS ) const;

  };

  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  const T CustomDataTimeContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::DB_CDATA_ALL_OUTER_KEYS = T();

  
  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  const Time CustomDataTimeContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::DB_CDATA_ALL_TIME_KEYS = Time(1, 1, 1901, 0, 0, 0);  

  
  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  std::optional<Data> CustomDataTimeContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::get( const T& t, double b, double r, const Time& time_key ) const {
      if ( data_map.empty() ) 
        return { };

      if ( debug ) 
        std::cout << "CustomDataTimeContainer::get() t = " << t << "; b = " << b << "; r = " << r 
                  << "; time_key = " << time_key << std::endl;

      auto ret_val = find();
      if ( ret_val ) 
        return ret_val;

      CDCCIt it = data_map.find( t );
      if ( it == data_map.end() ) 
        return { };

      CDCMediumCIt it2 = it->second.lower_bound( b );
      CDCMediumCRIt rit2 = it->second.rbegin();

      CDCInnerCIt it3;
      CDCInnerCRIt rit3;
      
      if ( it2 == it->second.end() ) {
          if ( rit2 == it->second.rend() ) 
            return { };
          it3 = rit2->second.lower_bound( r );
          rit3 = rit2->second.rbegin();
          if ( it3 != rit2->second.end() ) 
            return calculateData( it3->second, time_key);
          if ( rit3 != rit2->second.rend() ) 
            return calculateData( rit3->second, time_key);
          return { };
      }
      else {
          it3 = it2->second.lower_bound( r );
          rit3 = it2->second.rbegin();
          if ( it3 != it2->second.end() ) 
            return calculateData(it3->second, time_key);
          if ( rit3 != it2->second.rend() ) 
            return calculateData(rit3->second, time_key);
          return { };
      }
      if ( it3 == it2->second.end() ) 
        return { };
      return calculateData(it3->second, time_key);
  }


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  std::optional< Data > CustomDataTimeContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::get( const T& tx, const T& rx, const Time& time_key ) const {
      MidFunctor mid_funct;
      InFunctor in_funct;

      double curr_b;
      double curr_r;
      double delta_b;
      double delta_r;

      double curr_dist;
      double min_dist = INFINITY;

      if ( data_map.empty() == true ) {
          if ( debug ) 
            std::cout << "CustomDataTimeContainer::get() data_map is empty " << std::endl;

          return {};
      }
      const TimeData* time_data_ptr = nullptr;
      
      for ( CDCCIt it = data_map.begin(); it != data_map.end(); it++ ) {
          if ( debug ) 
            std::cout << "CustomDataTimeContainer::get() start T = " << it->first << "; end T = " << rx << std::endl;

          if ( it->first == DB_CDATA_ALL_OUTER_KEYS ) {
              if ( debug ) 
                std::cout << "CustomDataTimeContainer::get() overriding start T = " << tx << "; end T = " << rx << std::endl;

              curr_b = mid_funct(tx,rx);
              curr_r = in_funct(tx,rx);
          }
          else {
              curr_b = mid_funct(it->first,rx);
              curr_r = in_funct(it->first,rx);
          }

          if ( debug ) 
            std::cout << "CustomDataTimeContainer::get() curr bearing = " << curr_b * 180.0 / M_PI
                      << "; curr range = " << curr_r << std::endl;

          CDCMediumCIt itb = it->second.begin();
          if ( itb->first == DB_CDATA_ALL_MEDIUM_KEYS ) 
            delta_b = 0;
          else {
              itb = it->second.lower_bound( curr_b );
              if ( itb == it->second.end() ) 
                itb = (++(it->second.rbegin())).base();

              delta_b = curr_b - itb->first;
              if (delta_b < 0.0) 
                delta_b = -delta_b;
              if (delta_b > M_PI) 
                delta_b = 2.0*M_PI - delta_b ;
          }

          double ort_dist = curr_r * sin(delta_b);
          double ort_projection = std::sqrt( curr_r*curr_r - ort_dist*ort_dist );

          if ( debug ) 
            std::cout << "CustomDataTimeContainer::get() nearest bearing = " << itb->first * 180.0 / M_PI
                      << "; diff bearing = " << delta_b * 180.0 / M_PI << "; orthog distance = " << ort_dist
                      << "; orthog range projection = " << ort_projection << std::endl;

          CDCInnerCIt itr = itb->second.begin();
          if ( itr->first == DB_CDATA_ALL_INNER_KEYS ) 
            curr_dist = ort_dist;
          else {
              itr = itb->second.lower_bound( ort_projection );
              if ( itr == itb->second.begin() || itr == itb->second.end() || itr->first == ort_projection ) {
                  if ( itr == itb->second.end() ) itr = (++(itb->second.rbegin())).base();
                  double adj_distance = std::abs( ort_projection - itr->first );
                  curr_dist = std::sqrt( ort_projection*ort_projection + adj_distance*adj_distance );
              }
              else {
                  double adj_distance = std::abs( ort_projection - itr->first );
                  double first_dist = std::sqrt( ort_projection*ort_projection + adj_distance*adj_distance );

                  if (debug) 
                    std::cout << "CustomDataTimeContainer::get() first try, range = " << itr->first
                              << "; dist = " << first_dist << std::endl;

                  itr--;
                  adj_distance = std::abs( ort_projection - itr->first );
                  double before_dist = std::sqrt( ort_projection*ort_projection + adj_distance*adj_distance );

                  if (debug) 
                    std::cout << "CustomDataTimeContainer::get() second try, range = " << itr->first
                              << "; dist = " << before_dist << std::endl;

                  curr_dist = std::min( first_dist, before_dist );
                  if ( curr_dist == first_dist ) 
                    itr++;
              }
          }
          if ( debug ) 
            std::cout << "CustomDataTimeContainer::get() nearest range = " << itr->first << "; distance = " << curr_dist
                      << "; min distance = " << min_dist << std::endl;

          if ( curr_dist < min_dist ) {
              min_dist = curr_dist;
              time_data_ptr = &(itr->second);
              if ( curr_dist == 0 ) 
                break;
          }

      }

      if ( time_data_ptr != nullptr ) 
        return calculateData( *time_data_ptr, time_key);
      return {};
  }


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  std::optional<Data> CustomDataTimeContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::find( const T& t, double b, double r, const Time& time_key ) const {
      if (debug) 
        std::cout << "CustomDataTimeContainer::find() << t = " << t << "; b = " << b << "; r = " 
                  << r << "; time_key = " << time_key << std::endl;

      CDCCIt it = data_map.find( t );
      if ( it == data_map.end() ) {

          if ( debug ) 
            std::cout << "CustomDataTimeContainer::find() t not found" << std::endl;

          return { };
      }

      if ( debug ) std::cout << "CustomDataTimeContainer::find() t found" << std::endl;

      CDCMediumCIt it2 = it->second.find( b );
      if ( it2 == it->second.end() ) {
          if ( debug ) 
            std::cout << "CustomDataTimeContainer::find() b not found" << std::endl;

          return {};
      }

      if ( debug ) std::cout << "CustomDataTimeContainer::find() b found" << std::endl;

      CDCInnerCIt it3 = it2->second.find( r );
      if ( it3 == it2->second.end() ) {
          if ( debug ) 
            std::cout << "CustomDataTimeContainer::find() r not found" << std::endl;

          return {};
      }
      if ( debug ) std::cout << "CustomDataTimeContainer::find() r found" << std::endl;
      
      CDTCTimeCIt it4 = it3->second.find( time_key );
      if ( it4 == it3->second.end() ) {
          if ( debug ) 
            std::cout << "CustomDataTimeContainer::find() time_key not found" << std::endl;
          
          return {};
      }
      
      if ( debug ) std::cout << "CustomDataTimeContainer::find() time_key found, Data = " << it4->second << std::endl;

      return it4->second;
  }


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  bool CustomDataTimeContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::insert( const Data& d, const T& t, double b, double r, const Time& time_key ) {
      auto data = find( t, b, r, time_key );
      if ( data ) 
        return false;
      data_map[t][b][r][time_key] = d;
      return true;
  }


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  void CustomDataTimeContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::replace( const Data& d, const T& t, double b, double r, const Time& time_key ) {
      data_map[t][b][r][time_key] = d;
  }


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  void CustomDataTimeContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::erase( const T& t, double b, double r, const Time& time_key ) {
      data_map[t][b][r].erase(time_key);
      if ( data_map[t][b][r].empty() ) 
        data_map[t][b].erase(r);
      if ( data_map[t][b].empty() ) 
        data_map[t].erase(b);
      if ( data_map[t].empty() ) 
        data_map.erase(t);
  }


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  void CustomDataTimeContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::clear() {
      data_map.clear();
  }
              
  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  std::optional< Data > CustomDataTimeContainer< T, MidFunctor, InFunctor, Data, OutComp, MidComp, InComp >::calculateData( const TimeData& time_data, const Time& time_key ) const {
    if (debug) 
      std::cout << "CustomDataTimeContainer::calculateData() time_key = " << time_key << std::endl;
    
    if ( time_data.empty() ) {
      if (debug) 
        std::cout << "CustomDataTimeContainer::calculateData() time_data is empty." << std::endl;
    
      return {};
    }
    
    if ( time_data.size() == 1 ) {
      if (debug) 
        std::cout << "CustomDataTimeContainer::calculateData() time_data has size 1. Data created " 
                  << time_data.begin()->second << std::endl;
      
      return time_data.begin()->second;
    }
    
    time_t normalized_time = time_key;
    
    if ( normalized_time < time_data.begin()->first ) {
      if (debug) 
        std::cout << "CustomDataTimeContainer::calculateData() time_key has time < first key. Data created " 
                  << time_data.begin()->second << std::endl;
      
      return time_data.begin()->second;
    }

    normalized_time %= ( time_data.rbegin()->first - time_data.begin()->first );
    if ( normalized_time == 0 )
      return time_data.begin()->second;
    
    normalized_time += time_data.begin()->first;
    auto upper_it = time_data.upper_bound(normalized_time);
    auto lower_it = upper_it;
    --lower_it;

    double alpha = ( std::abs((double)(upper_it->first - normalized_time)) / std::abs((double)( upper_it->first - lower_it->first )) );
    double beta = ( std::abs((double)(normalized_time - lower_it->first)) / std::abs((double)( upper_it->first - lower_it->first )) );

    if (debug)
      std::cout << "CustomDataTimeContainer::calculateData() normalized_time = " << normalized_time 
                << "; lower_it time = " << lower_it->first 
                << "; upper_it time = " << upper_it->first 
                << "; alpha = " << alpha << "; beta " << beta << std::endl;

    Data ret_val = lower_it->second * alpha + upper_it->second * beta;
  
    if (debug)
      std::cout << "CustomDataTimeContainer::calculateData() return value = " 
                << ret_val << std::endl;

    return ret_val;
  }

  /**
  * \brief Partial specialization for Data = std::shared_ptr< Data >
  *
  */
  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp > 
  class CustomDataTimeContainer< T, MidFunctor, InFunctor, std::shared_ptr< Data >, OutComp, MidComp, InComp > 
  {
    
    protected:

    /**
     * The Time map, that links a time_t to actual data
     */
    using TimeData = typename std::map< time_t, std::shared_ptr< Data > >;
    using CDTCTimeIt = typename TimeData::iterator;
    using CDTCTimeRIt = typename TimeData::reverse_iterator;
    using CDTCTimeCIt = typename TimeData::const_iterator;
    using CDTCTimeCRIt = typename TimeData::const_reverse_iterator;

    /**
     * The inner level map, that links a double to TimeData
     */
    using InnerData = typename std::map< double, TimeData, InComp >;
    using CDCInnerIt = typename InnerData::iterator;
    using CDCInnerRIt = typename InnerData::reverse_iterator;
    using CDCInnerCIt = typename InnerData::const_iterator;
    using CDCInnerCRIt = typename InnerData::const_reverse_iterator;
    
    /**
     * The medium level map, that links a double to a InnerData instance
     */
    using MediumData = typename std::map< double, InnerData, MidComp >;
    using CDCMediumIt = typename MediumData::iterator;
    using CDCMediumCIt = typename MediumData::const_iterator;
    using CDCMediumRIt = typename MediumData::reverse_iterator;
    using CDCMediumCRIt = typename MediumData::const_reverse_iterator;
    
    /**
     * The outer level map, that links a the first template parameter to a MediumData instance
     */
    using CustomContainer = typename std::map< T, MediumData, OutComp >;
    using CDCIt = typename CustomContainer::iterator;
    using CDCRIt = typename CustomContainer::reverse_iterator;
    using CDCCIt = typename CustomContainer::const_iterator;
    using CDCCRIt = typename CustomContainer::const_reverse_iterator;
    
    
    public:
    

    static constexpr inline double DB_CDATA_ALL_MEDIUM_KEYS = -190.0;
    
    static constexpr inline double DB_CDATA_ALL_INNER_KEYS = -10.0;

    static const T DB_CDATA_ALL_OUTER_KEYS;  
    
    static const Time DB_CDATA_ALL_TIME_KEYS;
    
    
    /**
      * CustomDataTimeContainer default constructor
    */
    CustomDataTimeContainer() : debug(false), data_map() { }  
    
    /**
      * CustomDataTimeContainer destructor
    */
    ~CustomDataTimeContainer() = default;
    
    /**
     * operator[]
     * @param key a const reference to a T type ( first class parameter of template )
     * @returns a reference to the linked MediumData
     */
    MediumData& operator[] ( const T& key ) { return data_map[key]; }
    
    
    /**
     * Checks if the outer map is empty
     * @returns <i>true</i> if it is empty, <i>false</i> otherwise
     */
    bool empty() const { return data_map.empty(); }
    
    
    /**
     * Returns the size of the outer map
     * @returns the size
     */   
    int size() const { return data_map.size(); }
    
    
    /**
     * Finds and returns a std::optional to a std::shared_ptr of Data
     * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
     *          Default value means valid for all possible instances of T
     * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
     *          Default value means valid for all possible doubles of MediumData
     * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
     *          Default value means valid for all possible doubles of InnerData
     * @param time_key const reference to a Time instance, default value is DB_CDATA_ALL_TIME_KEYS.
     *          Default value means it is valid for all possible Time values of TimeData
     * @returns a std::optional to a std::shared_ptr of Data
     */
    std::optional< std::shared_ptr<Data> > get( const T& t = DB_CDATA_ALL_OUTER_KEYS, double b = DB_CDATA_ALL_MEDIUM_KEYS, double r = DB_CDATA_ALL_INNER_KEYS, const Time& time_key = DB_CDATA_ALL_TIME_KEYS ) const;
    
    /**
     * Finds and returns a std::optional to a std::shared_ptr of Data for the nearest match in all generated keys 
     * @param tx const reference to a T instance
     * @param rx const reference to a T instance
     * @param time_key const reference to a Time instance, default value is DB_CDATA_ALL_TIME_KEYS.
     *          Default value means it is valid for all possible Time values of TimeData
     * @returns a std::optional to a std::shared_ptr of Data
     */
    std::optional< std::shared_ptr<Data> > get( const T& tx, const T& rx, const Time& time_key = DB_CDATA_ALL_TIME_KEYS ) const;

    /**
     * Inserts a Data object for given parameters. If keys are already present, the object is discarded
     * @param data const reference to a std::shared_ptr of Data object to be inserted
     * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
     *          Default value means valid for all possible instances of T
     * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
     *          Default value means valid for all possible doubles of MediumData
     * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
     *          Default value means valid for all possible doubles of InnerData
     * @param time_key const reference to a Time instance, default value is DB_CDATA_ALL_TIME_KEYS.
     *          Default value means it is valid for all possible Time values of TimeData
     * @returns <i>true</i> if function was successful, <i>false</i> otherwise
     */
    bool insert( const std::shared_ptr<Data>& data, const T& t = DB_CDATA_ALL_OUTER_KEYS, double b = DB_CDATA_ALL_MEDIUM_KEYS, double r = DB_CDATA_ALL_INNER_KEYS, const Time& time_key = DB_CDATA_ALL_TIME_KEYS );
    
    /**
     * Replaces a Data object for given parameters. If keys are not present, the object is inserted
     * @param data const reference to a std::shared_ptr of Data object to be inserted
     * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
     *          Default value means valid for all possible instances of T
     * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
     *          Default value means valid for all possible doubles of MediumData
     * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
     *          Default value means valid for all possible doubles of InnerData
     * @param time_key const reference to a Time instance, default value is DB_CDATA_ALL_TIME_KEYS.
     *          Default value means it is valid for all possible Time values of TimeData
     * @returns <i>true</i> if function was successful, <i>false</i> otherwise
     */
    void replace( const std::shared_ptr<Data>& data, const T& t = DB_CDATA_ALL_OUTER_KEYS, double b = DB_CDATA_ALL_MEDIUM_KEYS, double r = DB_CDATA_ALL_INNER_KEYS, const Time& time_key = DB_CDATA_ALL_TIME_KEYS );
        
    /**
     * Erases a Data object for given parameters.
     * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
     *          Default value means valid for all possible instances of T
     * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
     *          Default value means valid for all possible doubles of MediumData
     * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
     *          Default value means valid for all possible doubles of InnerData
     * @param time_key const reference to a Time instance, default value is DB_CDATA_ALL_TIME_KEYS.
     *          Default value means it is valid for all possible Time values of TimeData
     * @returns <i>true</i> if function was successful, <i>false</i> otherwise
     */
    void erase( const T& t = DB_CDATA_ALL_OUTER_KEYS, double b = DB_CDATA_ALL_MEDIUM_KEYS, double r = DB_CDATA_ALL_INNER_KEYS, const Time& time_key = DB_CDATA_ALL_TIME_KEYS);
    
    
    /**
     * Clears the map
     */
    void clear();
    
    
    /**
     * Sets debug flag
     * @param flag debug flag
     **/
    void setDebug( bool flag ) { debug = flag; }
    
    
    /**
     * Gets debug flag
     * @returns flag debug flag
     **/
    bool usingDebug() const { return debug; }
    
    
    protected:
    
    
    /**
     * Debug flag
     **/
    bool debug;
    
    
    /**
     * data map
     **/ 
    CustomContainer data_map;
    
    /**
     * Finds the given keys. Returns a std::optional to a std::shared_ptr of Data
     * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
     *          Default value means valid for all possible instances of T
     * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
     *          Default value means valid for all possible doubles of MediumData
     * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
     *          Default value means valid for all possible doubles of InnerData
     * @param time_key const reference to a Time instance, default value is DB_CDATA_ALL_TIME_KEYS.
     *          Default value means it is valid for all possible Time values of TimeData  
     * @returns std::optional to a std::shared_ptr of Data
     */
    std::optional< std::shared_ptr<Data> > find( const T& t = DB_CDATA_ALL_OUTER_KEYS, double b = DB_CDATA_ALL_MEDIUM_KEYS, double r = DB_CDATA_ALL_INNER_KEYS, const Time& time_key = DB_CDATA_ALL_TIME_KEYS ) const;
    
    /**
     * Finds the given keys. Returns std::optional to a std::shared_ptr of Data
     * @param t const reference to a T instance, default value is DB_CDATA_ALL_OUTER_KEYS.
     *          Default value means valid for all possible instances of T
     * @param b double, default value is DB_CDATA_ALL_MEDIUM_KEYS.
     *          Default value means valid for all possible doubles of MediumData
     * @param r double, default value is DB_CDATA_ALL_INNER_KEYS.
     *          Default value means valid for all possible doubles of InnerData
     * @param time_key const reference to a Time instance, default value is DB_CDATA_ALL_TIME_KEYS.
     *          Default value means it is valid for all possible Time values of TimeData  
     * @returns std::optional to a std::shared_ptr of Data
     */
    std::optional< std::shared_ptr<Data> > calculateData( const TimeData& time_data , const Time& time_key = DB_CDATA_ALL_TIME_KEYS ) const;

  };

  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  const T CustomDataTimeContainer< T, MidFunctor, InFunctor, std::shared_ptr< Data >, OutComp, MidComp, InComp >::DB_CDATA_ALL_OUTER_KEYS = T();


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  const Time CustomDataTimeContainer< T, MidFunctor, InFunctor, std::shared_ptr< Data >, OutComp, MidComp, InComp >::DB_CDATA_ALL_TIME_KEYS = Time(1, 1, 1901, 0, 0, 0);  

  
  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  std::optional< std::shared_ptr<Data> > CustomDataTimeContainer< T, MidFunctor, InFunctor, std::shared_ptr< Data >, OutComp, MidComp, InComp >::get( const T& t, double b, double r, const Time& time_key ) const {
      if ( data_map.empty() ) 
        return { };

      if ( debug ) 
        std::cout << "CustomDataTimeContainer::get() t = " << t << "; b = " << b << "; r = " << r 
                  << "; time_key = " << time_key << std::endl;

      auto ret_val = find();
      if ( ret_val ) 
        return ret_val;

      CDCCIt it = data_map.find( t );
      if ( it == data_map.end() ) 
        return { };

      CDCMediumCIt it2 = it->second.lower_bound( b );
      CDCMediumCRIt rit2 = it->second.rbegin();

      CDCInnerCIt it3;
      CDCInnerCRIt rit3;
      
      if ( it2 == it->second.end() ) {
          if ( rit2 == it->second.rend() ) 
            return { };
          it3 = rit2->second.lower_bound( r );
          rit3 = rit2->second.rbegin();
          if ( it3 != rit2->second.end() ) 
            return calculateData( it3->second, time_key);
          if ( rit3 != rit2->second.rend() ) 
            return calculateData( rit3->second, time_key);
          return { };
      }
      else {
          it3 = it2->second.lower_bound( r );
          rit3 = it2->second.rbegin();
          if ( it3 != it2->second.end() ) 
            return calculateData(it3->second, time_key);
          if ( rit3 != it2->second.rend() ) 
            return calculateData(rit3->second, time_key);
          return { };
      }
      if ( it3 == it2->second.end() ) 
        return { };
      return calculateData(it3->second, time_key);
  }


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  std::optional< std::shared_ptr< Data > > CustomDataTimeContainer< T, MidFunctor, InFunctor, std::shared_ptr< Data >, OutComp, MidComp, InComp > ::get( const T& tx, const T& rx, const Time& time_key ) const {
      MidFunctor mid_funct;
      InFunctor in_funct;

      double curr_b;
      double curr_r;
      double delta_b;
      double delta_r;

      double curr_dist;
      double min_dist = INFINITY;

      if ( data_map.empty() == true ) {
          if ( debug ) 
            std::cout << "CustomDataTimeContainer::get() data_map is empty " << std::endl;

          return {};
      }
      const TimeData* time_data_ptr = nullptr;
      
      for ( CDCCIt it = data_map.begin(); it != data_map.end(); it++ ) {
          if ( debug ) 
            std::cout << "CustomDataTimeContainer::get() start T = " << it->first << "; end T = " << rx << std::endl;

          if ( it->first == DB_CDATA_ALL_OUTER_KEYS ) {
              if ( debug ) 
                std::cout << "CustomDataTimeContainer::get() overriding start T = " << tx << "; end T = " << rx << std::endl;

              curr_b = mid_funct(tx,rx);
              curr_r = in_funct(tx,rx);
          }
          else {
              curr_b = mid_funct(it->first,rx);
              curr_r = in_funct(it->first,rx);
          }

          if ( debug ) 
            std::cout << "CustomDataTimeContainer::get() curr bearing = " << curr_b * 180.0 / M_PI
                      << "; curr range = " << curr_r << std::endl;

          CDCMediumCIt itb = it->second.begin();
          if ( itb->first == DB_CDATA_ALL_MEDIUM_KEYS ) 
            delta_b = 0;
          else {
              itb = it->second.lower_bound( curr_b );
              if ( itb == it->second.end() ) 
                itb = (++(it->second.rbegin())).base();

              delta_b = curr_b - itb->first;
              if (delta_b < 0.0) 
                delta_b = -delta_b;
              if (delta_b > M_PI) 
                delta_b = 2.0*M_PI - delta_b ;
          }

          double ort_dist = curr_r * sin(delta_b);
          double ort_projection = std::sqrt( curr_r*curr_r - ort_dist*ort_dist );

          if ( debug ) 
            std::cout << "CustomDataTimeContainer::get() nearest bearing = " << itb->first * 180.0 / M_PI
                      << "; diff bearing = " << delta_b * 180.0 / M_PI << "; orthog distance = " << ort_dist
                      << "; orthog range projection = " << ort_projection << std::endl;

          CDCInnerCIt itr = itb->second.begin();
          if ( itr->first == DB_CDATA_ALL_INNER_KEYS ) 
            curr_dist = ort_dist;
          else {
              itr = itb->second.lower_bound( ort_projection );
              if ( itr == itb->second.begin() || itr == itb->second.end() || itr->first == ort_projection ) {
                  if ( itr == itb->second.end() ) itr = (++(itb->second.rbegin())).base();
                  double adj_distance = std::abs( ort_projection - itr->first );
                  curr_dist = std::sqrt( ort_projection*ort_projection + adj_distance*adj_distance );
              }
              else {
                  double adj_distance = std::abs( ort_projection - itr->first );
                  double first_dist = std::sqrt( ort_projection*ort_projection + adj_distance*adj_distance );

                  if (debug) 
                    std::cout << "CustomDataTimeContainer::get() first try, range = " << itr->first
                              << "; dist = " << first_dist << std::endl;

                  itr--;
                  adj_distance = std::abs( ort_projection - itr->first );
                  double before_dist = std::sqrt( ort_projection*ort_projection + adj_distance*adj_distance );

                  if (debug) 
                    std::cout << "CustomDataTimeContainer::get() second try, range = " << itr->first
                              << "; dist = " << before_dist << std::endl;

                  curr_dist = std::min( first_dist, before_dist );
                  if ( curr_dist == first_dist ) 
                    itr++;
              }
          }
          if ( debug ) 
            std::cout << "CustomDataTimeContainer::get() nearest range = " << itr->first << "; distance = " << curr_dist
                      << "; min distance = " << min_dist << std::endl;

          if ( curr_dist < min_dist ) {
              min_dist = curr_dist;
              time_data_ptr = &(itr->second);
              if ( curr_dist == 0 ) 
                break;
          }

      }

      if ( time_data_ptr != nullptr ) 
        return calculateData( *time_data_ptr, time_key);
      return {};
  }


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  std::optional< std::shared_ptr< Data > > CustomDataTimeContainer< T, MidFunctor, InFunctor, std::shared_ptr< Data >, OutComp, MidComp, InComp > ::find( const T& t, double b, double r, const Time& time_key ) const {
      if (debug) 
        std::cout << "CustomDataTimeContainer::find() << t = " << t << "; b = " << b << "; r = " 
                  << r << "; time_key = " << time_key << std::endl;

      CDCCIt it = data_map.find( t );
      if ( it == data_map.end() ) {

          if ( debug ) 
            std::cout << "CustomDataTimeContainer::find() t not found" << std::endl;

          return { };
      }

      if ( debug ) std::cout << "CustomDataTimeContainer::find() t found" << std::endl;

      CDCMediumCIt it2 = it->second.find( b );
      if ( it2 == it->second.end() ) {
          if ( debug ) 
            std::cout << "CustomDataTimeContainer::find() b not found" << std::endl;

          return {};
      }

      if ( debug ) std::cout << "CustomDataTimeContainer::find() b found" << std::endl;

      CDCInnerCIt it3 = it2->second.find( r );
      if ( it3 == it2->second.end() ) {
          if ( debug ) 
            std::cout << "CustomDataTimeContainer::find() r not found" << std::endl;

          return {};
      }
      if ( debug ) std::cout << "CustomDataTimeContainer::find() r found" << std::endl;
      
      CDTCTimeCIt it4 = it3->second.find( time_key );
      if ( it4 == it3->second.end() ) {
          if ( debug ) 
            std::cout << "CustomDataTimeContainer::find() time_key not found" << std::endl;
          
          return {};
      }
      
      if ( debug ) 
        std::cout << "CustomDataTimeContainer::find() time_key found, Data = " << it4->second << std::endl;

      return it4->second;
  }


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  bool CustomDataTimeContainer< T, MidFunctor, InFunctor, std::shared_ptr< Data >, OutComp, MidComp, InComp >::insert( const std::shared_ptr< Data >& d, const T& t, double b, double r, const Time& time_key ) {
      auto data = find( t, b, r, time_key );
      if ( data ) 
        return false;
      data_map[t][b][r][time_key] = d;
      return true;
  }


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  void CustomDataTimeContainer< T, MidFunctor, InFunctor, std::shared_ptr< Data >, OutComp, MidComp, InComp >::replace( const std::shared_ptr< Data >& d, const T& t, double b, double r, const Time& time_key ) {
      data_map[t][b][r][time_key] = d;
  }


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  void CustomDataTimeContainer< T, MidFunctor, InFunctor, std::shared_ptr< Data >, OutComp, MidComp, InComp >::erase( const T& t, double b, double r, const Time& time_key ) {
      data_map[t][b][r].erase(time_key);
      if ( data_map[t][b][r].empty() ) 
        data_map[t][b].erase(r);
      if ( data_map[t][b].empty() ) 
        data_map[t].erase(b);
      if ( data_map[t].empty() ) 
        data_map.erase(t);
  }


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  void CustomDataTimeContainer< T, MidFunctor, InFunctor, std::shared_ptr< Data >, OutComp, MidComp, InComp >::clear() {
      data_map.clear();
  }


  template < class T, class MidFunctor, class InFunctor, class Data, class OutComp, class MidComp, class InComp >
  std::optional< std::shared_ptr< Data > > CustomDataTimeContainer< T, MidFunctor, InFunctor, std::shared_ptr< Data >, OutComp, MidComp, InComp >::calculateData( const TimeData& time_data, const Time& time_key ) const {
    if (debug) 
      std::cout << "CustomDataTimeContainer::calculateData() time_key = " << time_key << std::endl;
    
    if ( time_data.empty() ) {
      if (debug) 
        std::cout << "CustomDataTimeContainer::calculateData() time_data is empty." << std::endl;
    
      return {};
    }
    
    if ( time_data.size() == 1 ) {
      if (debug) 
        std::cout << "CustomDataTimeContainer::calculateData() time_data has size 1. Data created " 
                  << time_data.begin()->second << std::endl;
      
      return time_data.begin()->second;
    }
    
    time_t normalized_time = time_key;
    
    if ( normalized_time < time_data.begin()->first ) {
      if (debug) 
        std::cout << "CustomDataTimeContainer::calculateData() time_key has time < first key. Data created " 
                  << time_data.begin()->second << std::endl;
      
      return time_data.begin()->second;
    }

    normalized_time %= ( time_data.rbegin()->first - time_data.begin()->first );
    if ( normalized_time == 0 )
      return time_data.begin()->second;
    
    normalized_time += time_data.begin()->first;
    auto upper_it = time_data.upper_bound(normalized_time);
    auto lower_it = upper_it;
    --lower_it;

    double alpha = ( std::abs((double)(upper_it->first - normalized_time)) / std::abs((double)( upper_it->first - lower_it->first )) );
    double beta = ( std::abs((double)(normalized_time - lower_it->first)) / std::abs((double)( upper_it->first - lower_it->first )) );

    if (debug)
      std::cout << "CustomDataTimeContainer::calculateData() normalized_time = " << normalized_time 
                << "; lower_it time = " << lower_it->first 
                << "; upper_it time = " << upper_it->first 
                << "; alpha = " << alpha << "; beta " << beta << std::endl;

    std::shared_ptr< Data > ret_val = std::move(lower_it->second->clone());

    *ret_val *= alpha;
    *ret_val += *(upper_it->second) * beta;
  
    if (debug)
      std::cout << "CustomDataTimeContainer::calculateData() return value = " 
                << ret_val << std::endl;

    return ret_val;
  }

}

#endif /* WOSS_DB_CUSTOM_DATA_CONTAINER_H */ 

