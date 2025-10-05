/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2009, 2025 Federico Guerra
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
 * @file   altimetry-definitions.h
 * @author Federico Guerra
 *
 * \brief  Definitions and library for woss::Altimetry class 
 *
 * Definitions and library for woss::Altimetry class
 */


#ifndef ALTIMETRY_DEFINITIONS_H
#define ALTIMETRY_DEFINITIONS_H


#include <map>
#include <memory>
#include "custom-precision-double.h"
#include "time-definitions.h"


namespace woss {
 
  /**
  * Map that links a PDouble range [m] to a double altimetry value.
  * Negative values ==> under water.
  * Positive values ==> above water.
  **/ 
  using AltimetryMap = std::map < PDouble, double >; 
  using AltIt = AltimetryMap::iterator;
  using AltCIt = AltimetryMap::const_iterator;
  using AltRIt = AltimetryMap::reverse_iterator;
  using AltCRIt = AltimetryMap::const_reverse_iterator;


  /**
  * \brief Altimetry profile class
  *
  * Altimetry class offers the possibility to store and manipulate altimetry profiles, e.g. a collection of range [m] 
  * values associated to a altimetry value [m].
  **/ 
  class Altimetry {

    
    public:

      
    /**
    * Default Altimetry constructor
    **/ 
    Altimetry();

    /**
    * Altimetry constructor
    * @param map custom time arrival map
    **/
    Altimetry( AltimetryMap& map );
    
    /**
    * Altimetry constructor
    * @param range range [m]
    * @param altimetry altimetry [m]
    **/ 
    Altimetry( double range, double altimetry );
    
    /**
    * Altimetry copy constructor
    * @param copy Altimetry to be copied
    **/
    Altimetry( const Altimetry& copy ) = default;

    /**
    * Altimetry move constructor
    * @param tmp Altimetry to be moved from
    **/
    Altimetry( Altimetry&& tmp) = default;

    virtual ~Altimetry() = default;

    /**
    * Altimetry virtual factory method
    * @return a std::unique_ptr to a heap-created Altimetry object
    **/ 
    virtual std::unique_ptr<Altimetry> create() const { 
      auto ptr = std::make_unique<Altimetry>();
      ptr->updateMinMaxAltimetryValues();
      return ptr; }

    /**
    * Altimetry virtual factory method
    * @param map custom time arrival map
    * @return a std::unique_ptr to a heap-created Altimetry object
    **/ 
    virtual std::unique_ptr<Altimetry> create( AltimetryMap& map ) const {
      auto ptr = std::make_unique<Altimetry>(map);
      ptr->updateMinMaxAltimetryValues();
      return ptr; }
    /**
    * Altimetry virtual factory method
    * @return a std::unique_ptr to a heap-created copy of <b>this</b> instance
    **/
    virtual std::unique_ptr<Altimetry> clone() const { 
      return std::make_unique<Altimetry>(*this); }
    /**
    * Returns a reference an AltimetryMap object not valid
    * @return a AltimetryMap instance not valid ( e.g. range 0.0 = +inf )
    **/
    static AltimetryMap& createNotValid();
    
    /**
    * Creates a flat AltimetryMap
    * @return a reference AltimetryMap flat altimetry instance ( e.g. range 0.0 = altimetry )
    **/
    static AltimetryMap& createFlat(double altimetry = 0 );

		
    /**
    * Inserts and replace an altimetry value at closest range step
    * @param range range value [m]
    * @param altimetry altimetry value [m]; > 0 ==> under water; < 0 ==> above water
    * @return reference to <b>*this</b>
    **/
    Altimetry& insertValue( double range, double altimetry );

    /**
    * Inserts and sums an altimetry value at closest range step
    * @param range range value [m]
    * @param altimetry altimetry value [m]; > 0 ==> under water; < 0 ==> above water
    **/
    Altimetry& sumValue( double range, double altimetry );
    
    
    /**
    * Returns a const iterator to the altimetry with key == <i>range</i> parameter
    * @param range range value [m]
    * @returns const iterator to end() if <i>range</i> is not found
    **/
    AltCIt findValue( double range ) const { return( altimetry_map.find(range) ); }
    
    
    /**
    * Erase the altimetry value with key == <i>range</i> parameter
    * @param range range value [s]
    * @return reference to <b>*this</b>
    **/
    Altimetry& eraseValue( double range ) { altimetry_map.erase(range); return *this; }


    /**
    * Crops the Altimetry between given range values and returns a new heap-allocated object. 
    * The new object will have range values in [range_start, range_end)
    * @param time_start start range value [m]
    * @param time_end end range value [m]
    * @returns a new Altimetry object
    **/
    virtual std::unique_ptr<Altimetry> crop( double range_start, double range_end ) const;
 
     /**
    * Performs a random perturbation of altimetry values with given ratio
    * @param ratio_incr_value perturbation ratio
    * @return a <i>new</i> Altimetry object
    **/
    virtual std::unique_ptr<Altimetry> randomize( double ratio_incr_value ) const;
    
    /**
    * Returns a const iterator to the beginning of the altimetry map
    * @returns const iterator 
    **/
    AltCIt begin() const { return altimetry_map.begin(); }
    
    /**
    * Returns a const iterator to the end of the altimetry map
    * @returns const iterator 
    **/
    AltCIt end() const { return altimetry_map.end(); }
    
    /**
    * Returns a const iterator to the altimetry value at i-th position 
    * @param i integer should be between 0 and size() 
    * @returns const iterator to end() if position <i>i</i> is not found
    **/
    AltCIt at( const int i ) const;

    /**
    * Returns the number of altimetry values stored
    * @return number of altimetry values stored
    **/
    int size() const { return altimetry_map.size(); }

    /**
    * Checks if the instance has stored values
    * @return <i>true</i> if condition applies, <i>false</i> otherwise
    **/
    bool empty() const { return altimetry_map.empty(); } 


    /**
    * Erase all values of altimetry
    **/
    void clear() { altimetry_map.clear(); }
    

   /**
    * Sets debug flag for all instances
    * @param flag debug bool
    **/
    static void setDebug( bool flag ) { debug = flag; }

   /**
    * Gets debug flag for all instances
    * @return flag debug
    **/
    static bool getDebug( bool flag ) { return debug; }

    /**
    * Sets the evolution time quantum
    * @param quantum time quantum [s]
    **/
    Altimetry& setEvolutionTimeQuantum( double quantum ) { evolution_time_quantum = quantum;  return *this;}
    
    /**
    * Sets the total range step
    * @param r_s range steps
    **/    
    Altimetry& setTotalRangeSteps( int r_s ) { total_range_steps = r_s;  return *this;}
    
    /**
    * Sets the range 
    * @param r max range [m]
    **/    
    Altimetry& setRange( double r ) { range = r;  return *this; }
    
    /**
     * Sets the depth 
     * @param d scenario depth [m]
     **/
    Altimetry& setDepth( double d) { depth = d; return *this; }
    
    
    /**
    * Returns the maximum range value
    * @returns maximum range [m]
    **/
    double getMaxRangeValue() const { return( altimetry_map.rbegin()->first ); }

    /**
    * Returns the maximum range value
    * @returns maximum range [m]
    **/
    double getMinRangeValue() const { return( altimetry_map.begin()->first ); }
    
     /**
    * Returns the maximum altimetry value
    * @returns maximum altimetry [m]
    **/
    double getMaxAltimetryValue() const { return( max_altimetry_value ); }

    /**
    * Returns the minimum altimetry value
    * @returns minimum altimetry [m]
    **/
    double getMinAltimetryValue() const { return( min_altimetry_value ); }   
    
    /**
    * Returns the range precision
    * @returns range precision [m]
    **/
    long double getRangePrecision() const { return range_precision; }

    /**
    * Returns the evolution time quantum
    * @returns time quantum [s]
    **/
    double getEvolutionTimeQuantum() const { return evolution_time_quantum; }
 
    /**
    * Returns the max range [m]
    * @returns max range [m]
    **/ 
    double getRange() const { return range; }
    
    /**
    * Returns the total range step
    * @returns range steps
    **/    
    int getTotalRangeSteps() const { return total_range_steps; }
    
    /**
     * Returns the scenario depth
     * @returns depth [m]
     **/
    double getDepth() const { return depth; }
    
    
    /**
    * Checks the validity of Altimetry
    * @return <i>true</i> if it has at least one value, <i>false</i> otherwise
    **/
    virtual bool isValid() const;
 
    /**
    * Initializes the altimetry vector. used if there is a matematical function
    * that generates the whole vector.
    * @return <i>true</i> succeeded <i>false</i> otherwise
    **/    
    virtual bool initialize();
    
    /**
    * Performs a time evolution
    * @param time_value const reference to a valid Time object
    * @return a std::unique_ptr to a new heap allocated Altimetry object
    **/
    virtual std::unique_ptr<Altimetry> timeEvolve( const Time& time_value );
    
    
    /**
    * Assignment operator
    * @param copy const reference to a Altimetry object to be copied
    * @return Altimetry reference to <i>this</i>
    **/ 
    Altimetry& operator=( const Altimetry& copy ) = default;

    /**
    * Assignment move operator
    * @param tmp rvalue to be moved from
    * @return Altimetry reference to <i>this</i>
    **/ 
   Altimetry& operator=( Altimetry&& tmp ) = default;

    /**
    * Equality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left == right</i>, false otherwise
    **/  
    friend bool operator==( const Altimetry& left, const Altimetry& right );

    /**
    * Inequality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left != right</i>, false otherwise
    **/  
    friend bool operator!=( const Altimetry& left, const Altimetry& right );


    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const Altimetry operator+( const Altimetry& left, const Altimetry& right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const Altimetry operator-( const Altimetry& left, const Altimetry& right );

    
    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const Altimetry operator+( const Altimetry& left, const double right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const Altimetry operator-( const Altimetry& left, const double right );  

    /**
    * Division operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const Altimetry operator/( const Altimetry& left, const double right );

    /**
    * Multiplication operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const Altimetry operator*( const Altimetry& left, const double right );

    
    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const Altimetry operator+( const double left, const Altimetry& right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const Altimetry operator-( const double left, const Altimetry& right );  

    /**
    * Division operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const Altimetry operator/( const double left, const Altimetry& right );

    /**
    * Multiplication operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/  
    friend const Altimetry operator*( const double left, const Altimetry& right );  
    

    /**
    * Compound assignment sum operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Altimetry& operator+=( Altimetry& left, const Altimetry& right );

    /**
    * Compound assignment subtraction operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Altimetry& operator-=( Altimetry& left, const Altimetry& right );

    
    /**
    * Compound assignment sum operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Altimetry& operator+=( Altimetry& left, double right );

    /**
    * Compound assignment subtraction operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Altimetry& operator-=( Altimetry& left, double right );
    
    /**
    * Compound assignment division operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Altimetry& operator/=( Altimetry& left, double right );

    /**
    * Compound assignment multiplication operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Altimetry& operator*=( Altimetry& left, double right );
  

    /**
    * << operator
    * @param os left operand ostream reference
    * @param instance right operand const Pressure reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend std::ostream& operator<<( std::ostream& os, const Altimetry& instance );


    protected:


    virtual Altimetry& updateMinMaxAltimetryValues();
    
    /**
    * Debug flag
    **/
    static bool debug;
    
    /**
    * range [m]
    **/ 
    double range;
    
    /**
    *range total steps
    **/    
    int total_range_steps;
     
    /**
    * Stores the miminum altimetry value [m] i.e. the maximum positive value (if present)
    **/    
    double min_altimetry_value;

    /**
    * Stores the maximum altimetry value [m] i.e. the min negative value (if present)
    **/ 
    double max_altimetry_value;
    
    /**
    * Stores the precision of all PDouble ranges [m] 
    **/ 
    long double range_precision;

    /**
    * Stores the last evolution's simulation time
    **/
    Time last_evolution_time;
    
    /**
    * Stores the last evolution time quantum [s]  
    **/    
    double evolution_time_quantum;
    
    /**
     * Stores the depth of the scenario [m]
     **/
    double depth;
    
    /**
    * altimetry values map 
    **/ 
    AltimetryMap altimetry_map;

  };
 
  
 /**
  * \brief AltimBretschneider supports Bretschneider wave model
  *
  *  AltimBretschneider wave model
  **/    
  class AltimBretschneider : public Altimetry {
    
    public:
      
    /**
    * Default AltimBretschneider constructor
    **/ 
    AltimBretschneider();

    /**
    * AltimBretschneider constructor
    * @param map custom time arrival map
    **/ 
    AltimBretschneider( AltimetryMap& map );
    
    /**
    * AltimBretschneider constructor
    * @param ch_height characteristic wave height [m]
    * @param avg_per averate period [s]
    * @param total_range_steps total range steps
    * @param depth depth [m]
    **/ 
    AltimBretschneider( double ch_height, double avg_per, int total_range_steps, double depth );
    
    /**
    * AltimBretschneider copy constructor
    * @param copy AltimBretschneider to be copied
    **/
    AltimBretschneider( const AltimBretschneider& copy );

    /**
    * AltimBretschneider move constructor
    * @param tmp AltimBretschneider to be moved
    **/
    AltimBretschneider( AltimBretschneider&& tmp );

    virtual ~AltimBretschneider() override = default;

    /**
    * AltimBretschneider virtual factory method
    * @return a std::unique_ptr to a heap-created AltimBretschneider object
    **/ 
    virtual std::unique_ptr<Altimetry> create() const override { 
      auto ptr = std::make_unique<AltimBretschneider>();
      ptr->updateMinMaxAltimetryValues();
      return ptr; }
    /**
    * AltimBretschneider virtual factory method
    * @param map custom time arrival map
    * @return a std::unique_ptr to a heap-created AltimBretschneider object
    **/ 
    virtual std::unique_ptr<Altimetry> create( AltimetryMap& map ) const override {
      auto ptr = std::make_unique<AltimBretschneider>( map );
      ptr->updateMinMaxAltimetryValues();
      return ptr; }
    /**
     * AltimBretschneider virtual factory method
     * @param ch_height characteristic wave height [m]
     * @param avg_per averate period [s]
     * @param total_range_steps total range steps
     * @param depth depth [m]
     * @return a std::unique_ptr to a heap-created AltimBretschneider object
     **/     
    virtual std::unique_ptr<Altimetry> create( double ch_height, double avg_per, int total_range_steps, double depth ) const {
      return std::make_unique<AltimBretschneider>( ch_height, avg_per, total_range_steps, depth);
    }
    
    /**
    * AltimBretschneider virtual factory method
    * @return a std::unique_ptr to heap-created copy of <b>this</b> instance
    **/
    virtual std::unique_ptr<Altimetry> clone() const override { 
      return std::make_unique<AltimBretschneider>( *this ); }  
  
    /**
    * Assignment operator
    * @param copy const reference to a AltimBretschneider object to be copied
    * @return AltimBretschneider reference to <i>this</i>
    **/ 
    AltimBretschneider& operator=( const AltimBretschneider& copy ) = default;

    /**
    * Assignment operator
    * @param tmp rvalue reference to a AltimBretschneider object to be moved
    * @return AltimBretschneider reference to <i>this</i>
    **/ 
    AltimBretschneider& operator=( AltimBretschneider&& tmp ) = default;
    
    
    /**
    * Initializes the altimetry vector. used if there is a matematical function
    * that generates the whole vector.
    * @return <i>true</i> succeeded <i>false</i> otherwise
    **/     
    virtual bool initialize() override;
    
    
    /**
    * Checks the validity of AltimBretschneider
    * @return <i>true</i> if it has at least one value, <i>false</i> otherwise
    **/    
    virtual bool isValid() const override;
  
    
    /**
    * Performs a time evoulion
    * @param time_value const reference to a valid Time object
    * @return a std::unique_ptr to a new heap allocated AltimBretschneider object
    **/    
    virtual std::unique_ptr<Altimetry> timeEvolve( const Time& time_value ) override;
    
    
    /**
     * Performs a random perturbation of altimetry values with given ratio
     * @param ratio_incr_value perturbation ratio
     * @return a std::unique_ptr to a <i>new</i> Altimetry object
     **/
    virtual std::unique_ptr<Altimetry> randomize( double ratio_incr_value ) const override;

    /**
     * Configures H - the characteristic height of the wave [m]
     * @param h characteristic height [m]
     * @return AltimBretschneider reference to <i>this</i>
     **/
    AltimBretschneider& setCharacteristicHeight( double h ) { char_height = h; return *this; }

    /**
     * Configures T - the wave average period [s]
     * @param h average period [s]
     * @return AltimBretschneider reference to <i>this</i>
     **/
    AltimBretschneider& setAveragePeriod( double p ) { average_period = p; return *this; }

    /**
     * Gets H - the characteristic height of the wave [m]
     * @return characteristic height [m]
     **/
    double getCharacteristicHeight() const { return char_height; }

    /**
     * Gets T - the wave average period [s]
     * @return h average period [s]
     **/
    double getAveragePeriod() const { return average_period; }
      
    
    protected:
    
    
    virtual AltimBretschneider& createWaveSpectrum();
    
    /**
     * H - Model's characteristic height [m]
     * Refer to:
     * G. J. Komen et al., Dynamics and modeling of ocean waves. Cambridge University Press, 1994. 
     */
    double char_height;
    
    /**
     * T - Model's average wave period [s]
     * Refer to:
     * G. J. Komen et al., Dynamics and modeling of ocean waves. Cambridge University Press, 1994. 
     */
    double average_period;

  };
  
  //non-inline operator declarations
  ///////////////////
  const Altimetry operator+( const Altimetry& left, const Altimetry& right ); 

  const Altimetry operator-( const Altimetry& left, const Altimetry& right );  


  const Altimetry operator+( const Altimetry& left, const double right );  

  const Altimetry operator-( const Altimetry& left, const double right );  

  const Altimetry operator/( const Altimetry& left, const double right );  

  const Altimetry operator*( const Altimetry& left, const double right );  


  const Altimetry operator+( const double left, const Altimetry& right );  

  const Altimetry operator-( const double left, const Altimetry& right );  

  const Altimetry operator/( const double left, const Altimetry& right );  

  const Altimetry operator*( const double left, const Altimetry& right );  


  Altimetry& operator+=( Altimetry& left, const Altimetry& right );  

  Altimetry& operator-=( Altimetry& left, const Altimetry& right );  

  Altimetry& operator+=( Altimetry& left, double right );  


  Altimetry& operator-=( Altimetry& left, double right ); 

  Altimetry& operator/=( Altimetry& left, double right ); 

  Altimetry& operator*=( Altimetry& left, double right ); 
  
  //////////
  //inline functions
  inline AltimetryMap& Altimetry::createNotValid() {
    static AltimetryMap altimetry_map;
    altimetry_map.clear();
    return altimetry_map;
  }


  inline AltimetryMap& Altimetry::createFlat( double altimetry ) {
    static AltimetryMap altimetry_map;
    altimetry_map.clear();
    altimetry_map[0.0] = altimetry;
    return altimetry_map;
  }


  inline Altimetry& Altimetry::sumValue( double range, double altimetry ) {  
    if ( altimetry < min_altimetry_value ) 
      min_altimetry_value = altimetry;
    if ( altimetry > max_altimetry_value ) 
      max_altimetry_value = altimetry;
    
    auto it = altimetry_map.find( range );
    if ( it == altimetry_map.end() ) 
      altimetry_map.emplace( std::make_pair( range, altimetry ) );
    else 
      it->second += altimetry;

    return *this;
  }


  inline std::ostream& operator<<( std::ostream& os, const Altimetry& instance ) {
    if ( !instance.altimetry_map.empty() ) {
      os << "size = " << instance.altimetry_map.size() << "; min range = " << instance.altimetry_map.begin()->first 
                      << "; altimetry = " << instance.begin()->second 
                      << "; max range = " << instance.altimetry_map.rbegin()->first 
                      << "; altimetry = " << instance.altimetry_map.rbegin()->second;
    }
    else {
      os << "is empty.";
    }
    return os;
  }


  inline bool operator==( const Altimetry& left, const Altimetry& right ) { 
    if ( &left == &right ) 
      return true;
    return( left.altimetry_map == right.altimetry_map );
  }


  inline bool operator!=( const Altimetry& left, const Altimetry& right ) { 
    if ( &left == &right ) 
      return false; 
    return( left.altimetry_map != right.altimetry_map );
  }

}

#endif /* ALTIMETRY_DEFINITIONS_H */
