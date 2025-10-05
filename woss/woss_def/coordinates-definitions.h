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
 * @file   coordinates-definitions.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for the woss::Coord and woss::CoordZ classes
 *
 * Provides the interface for the woss::Coord and woss::CoordZ classes
 */


#ifndef WOSS_COORDINATES_DEFINITIONS_H
#define WOSS_COORDINATES_DEFINITIONS_H


#include <cmath>
#include <iostream>
#include <cassert>
#include <utility>
#include <vector>
#include <ostream>

namespace woss {

  class Coord;
  class CoordZ;
  
  /**
  * The letter that identify a UTM zone
  **/
  using UtmZoneChar = char;

  /**
  * A vector of CoordZ. Beware of object slicing if using this container for objects <i>derived</i>
  * from CoordZ
  **/
  using CoordZVector = std::vector< CoordZ >;

  /**
  * The marsden square
  **/
  using Marsden = int;

  /**
  * The Marsden coordinates 
  * @param int marsden square
  * @param int marsden one degree square
  **/
  using MarsdenCoord = std::pair< int, int >; 
  
  /**
  * A vector of Coord
  **/
  using CoordVector = std::vector< Coord >;

  /**
  * A vector of marsden squares
  **/
  using MarsdenVector = std::vector< Marsden >;

  /**
  * A vector of marsden coordinates
  **/
  using MarsdenCoordVector = std::vector< MarsdenCoord >;

  /**
  * Default value for not intialized variables
  **/
  static constexpr inline double COORD_NOT_SET_VALUE = -2000.0;

  /**
  * \brief Coordinates (lat, long) class definitions and functions library
  *
  * Coord class stores a double <b>decimal degree</b> latitude and double <b>decimal degree</b> longitude. 
  * It has a reach library for Marsden coordinates, distance, bearing and arithmetic calculations.
  **/
  class Coord {

    public:

    static constexpr double COORD_MIN_LATITUDE =  -90.0; ///< Minimum valid Latitude

    static constexpr double COORD_MAX_LATITUDE = 90.0; ///< Maximum valid Latitude

    static constexpr double COORD_MIN_LONGITUDE = -180.0; ///< Minimum valid Longitude

    static constexpr double COORD_MAX_LONGITUDE = 180.0; ///< Maximum valid Longitude

    static constexpr double EARTH_RADIUS = 6371000.0; ///< Mean earth radius in meters

    /// Earth's semi-major axis in meters as defined by both GRS80 and WGS84
    static constexpr double EARTH_SEMIMAJOR_AXIS = 6378137.0;

    /// Earth's semi-major axis in meters as defined by GRS80
    static constexpr double EARTH_GRS80_POLAR_RADIUS = 6356752.3141;

    /// Earth's polar radius in meters as defined by WGS84
    static constexpr double EARTH_WGS84_POLAR_RADIUS= 6356752.314245;

    /// Earth's first eccentricity as defined by GRS80
    static constexpr double EARTH_GRS80_ECCENTRICITY = 0.0818191910428158;

    /// Earth's first eccentricity as defined by WGS84
    static constexpr double EARTH_WGS84_ECCENTRICITY = 0.0818191908426215;

    /**
    * Coord constructor.
    * @param lat <b>decimal degree</b> latitude value. Default value makes the instance <i>not valid</i>
    * @param lon <b>decimal degree</b> longitude value. Default value makes the instance <i>not valid</i>
    **/
    constexpr Coord( double lat = COORD_NOT_SET_VALUE, double lon = COORD_NOT_SET_VALUE );

    constexpr Coord( const Coord& copy ) = default;

    constexpr Coord( Coord&& tmp ) = default;

    /**
    * Sets latitude and updates marsden coordinates
    * @param lat latitude value
    **/
    constexpr void setLatitude( double lat ) { latitude = lat; updateMarsdenCoord(); }

    /**
    * Sets longitude and updates marsden coordinates
    * @param lon longitude value
    **/
    constexpr void setLongitude( double lon ) { longitude = lon; updateMarsdenCoord(); }

    /**
    * Checks the validity of coordinates provided
    * @return <i>true</i> if coordinates are valid, <i>false</i> otherwise
    **/
    constexpr bool isValid() const { return( latitude >= COORD_MIN_LATITUDE && latitude <= COORD_MAX_LATITUDE
                                        && longitude >= COORD_MIN_LONGITUDE && longitude <= COORD_MAX_LONGITUDE ); }

    /**
    * Gets the latitude value
    * @return latitude value of the instance
    **/
    constexpr double getLatitude() const { return latitude; }

    /**
    * Gets the longitude value
    * @return longitude value of the instance
    **/
    constexpr double getLongitude() const { return longitude; } 

    /**
    * Gets the marsden square value
    * @return marsden square value of the instance
    **/
    constexpr int getMarsdenSquare() const { return marsden_square; }

    /**
    * Gets the marsden one degree square value
    * @return marsden one degree square value of the instance
    **/
    constexpr int getMarsdenOneDegreeSquare() const { return marsden_one_degree; }

    /**
    * Gets the marsden coordinates
    * @return complete marsden coordinates of the instance
    **/
    constexpr MarsdenCoord getMarsdenCoord() const { return( std::make_pair( marsden_square, marsden_one_degree) ); }

    /**
    * Gets the initial bearing of a trajectory
    * @param destination destination Coord instance 
    * @return initial bearing measured in <i>radians</i>
    **/
    constexpr double getInitialBearing( const Coord& destination ) const;

    /**
    * Gets the final bearing of a trajectory
    * @param destination destination Coord instance 
    * @return initial bearing measured in <i>radians</i>
    **/
    constexpr double getFinalBearing( const Coord& destination ) const;

    /**
    * Gets calculates great-circle distances between the two points – that is, 
    * the shortest distance over the earth’s surface – using the ‘Haversine’ formula
    * @param destination <b>valid</b> destination Coord instance 
    * @return distance measured in <i>meters</i>
    **/
    constexpr double getGreatCircleDistance( const Coord& destination, double depth = 0 ) const ;

    /**
    * Gets destination Cood given bearing and distance from a start Coord,
    * travelling along a (shortest distance) great circle arc of given depth
    * @param start_coord <b>valid</b> start Coord instance 
    * @param bearing bearing measured in <i>radians</i>
    * @param distance distance measured in <i>meters</i>
    * @param depth depth measured in <i>meters</i>
    * @return a new Coord instance containing the destination coordinates
    **/
    static constexpr Coord getCoordFromBearing( const Coord& start_coord, double bearing, double distance, double depth = 0.0 ); 

    /**
    * Gets destination Cood given bearing and distance from a start Coord,
    * travelling along a (shortest distance) great circle arc of given depth to end Coord
    * @param start_coord <b>valid</b> start Coord instance 
    * @param end_coord <b>valid</b> end Coord instance
    * @param distance distance measured in <i>meters</i>
    * @param depth depth measured in <i>meters</i>
    * @return a new Coord instance containing the destination coordinates
    **/
    static constexpr Coord getCoordAlongGreatCircle( const Coord& start_coord, const Coord& end_coord, double distance, double depth = 0.0 ); 

    /**
    * Gets destination Cood given easting and northing in UTM - WGS84 coordinates
    * @param easting <b>valid</b> easting relative to central UTM zone meridian
    * @param end_coord <b>valid</b> northing
    * @param utm_zone_number <b>valid</b>zone number
    * @param utm_zone_char <b>valid</b>zone character
    * @return a new Coord instance containing the destination coordinates
    **/
    static constexpr Coord getCoordFromUtmWgs84( double easting, double northing, double utm_zone_number, UtmZoneChar utm_zone_char );

    /**
    * Assignment operator
    * @param copy const reference to a Coord object to be copied
    * @return Coord reference to <i>this</i>
    **/
    constexpr Coord& operator=( const Coord& copy ) = default;

    /**
    * Move assignment operator
    * @param tmp rvalue reference to a Coord object to be move
    * @return Coord reference to <i>this</i>
    **/
    constexpr Coord& operator=( Coord&& tmp ) = default;

    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend constexpr Coord operator+( const Coord& left , const Coord& right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend constexpr Coord operator-( const Coord& left , const Coord& right );

    /**
    * Compound assignment sum operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend constexpr Coord& operator+=( Coord& left, const Coord& right );
    
    /**
    * Compound assignment subtraction operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend constexpr Coord& operator-=( Coord& left, const Coord& right );

    /**
    * Equality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left == right</i>, false otherwise
    **/
    friend constexpr bool operator==( const Coord& left, const Coord& right );

    /**
    * Inequality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left != right</i>, false otherwise
    **/  
    friend constexpr bool operator!=( const Coord& left, const Coord& right );

    /**
    * Greater than operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left > right</i>, false otherwise
    **/ 
    friend constexpr bool operator>( const Coord& left, const Coord& right );

    /**
    * Less than operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left < right</i>, false otherwise
    **/ 
    friend constexpr bool operator<( const Coord& left, const Coord& right );

    /**
    * Greater than or equal to operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left >= right</i>, false otherwise
    **/
    friend constexpr bool operator>=( const Coord& left, const Coord& right );

    /**
    * Less than or equal to operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left <= right</i>, false otherwise
    **/
    friend constexpr bool operator<=( const Coord& left, const Coord& right );

    /**
    * << operator
    * @param os left operand ostream reference
    * @param instance right operand const Coord reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend std::ostream& operator<<( std::ostream& os, const Coord& instance );

    protected:

    /**
    * Latitude value
    **/
    double latitude;

    /**
    * Longitude value
    **/
    double longitude;

    /**
    * Marsden square value
    **/
    int marsden_square;

    /**
    * Marsden one degree square value
    **/
    int marsden_one_degree;

    /**
    * Calculates marsden coordinates from latitude and longitude
    **/
    constexpr void updateMarsdenCoord();

    /**
    * Checks if the passed utm zone character is valid
    * @param utm_zone_char utm zone character
    * @return <i>true</i> if valid, <i>false</i> otherwise
    **/ 
    static constexpr bool isValidUtmZoneChar( UtmZoneChar utm_zone_char );
  };


  /**
  * \brief 3D-Coordinates (lat, long, depth) class definitions and functions library
  *
  * CoordZ class inherit from Coord, adding a depth value
  **/
  class CoordZ : public Coord {

    public:

    static constexpr double COORDZ_MIN_DEPTH = 0.0; ///< Minimum valid depth

    /**
    * \brief Spheroid model to use
    * 
    * Spheroid model to use: perfect sphere (CoordZSpheroidType::COORDZ_SPHERE), 
    * Geodetic Reference System 1980 (COORDZ_GRS80), 
    * or World Geodetic System 1984 (COORDZ_WGS84)
    */
    enum class CoordZSpheroidType
    {
      COORDZ_SPHERE = 0,
      COORDZ_GRS80,
      COORDZ_WGS84
    };

    /**
    * \brief Class that represents cartesian coordinates
    * 
    * Class that represents cartesian coordinates
    */
    class CartCoords {
      public:

      /**
      * CartCoords default constructor.
      **/
      constexpr CartCoords();

      /**
      * Coord constructor.
      * @param in_x number of <b>meters</b> along the X axis
      * @param in_y number of <b>meters</b> along the X axis
      * @param in_z number of <b>meters</b> along the X axis
      * @param in_type Spheroid model that has been used
      **/
      constexpr CartCoords(double in_x, double in_y, double in_z, CoordZSpheroidType in_type);

      /**
      * Returns the number of meters along the X axis
      * @return x axis value [m]
      **/ 
      constexpr double getX() const { return x; }

      /**
      * Returns the number of meters along the Y axis
      * @return y axis value [m]
      **/ 
      constexpr double getY() const { return y; }

      /**
      * Returns the number of meters along the z axis
      * @return Z axis value [m]
      **/ 
      constexpr double getZ() const { return z; }

      /**
      * Returns the CoordZSpheroidType used for computation
      * @return CoordZSpheroidType spheroid model
      **/ 
      constexpr CoordZSpheroidType getType() const { return type; }

      /**
      * << operator
      * @param os left operand ostream reference
      * @param instance right operand const CartCoords reference
      * @return <i>os</i> reference after the operation
      **/ 
      friend std::ostream& operator<<( std::ostream& os, const CartCoords& instance );

      protected:

      double x; /// X-asis value in meters
      
      double y; /// Y-asis value in meters
      
      double z; /// Z-asis value in meters
      
      CoordZSpheroidType type; /// Spheroid used during computation
    };

    /**
    * CoordZ constructor.
    * @param lat <b>decimal degree</b> latitude value. Default value makes the instance <i>not valid</i>
    * @param lon <b>decimal degree</b> longitude value. Default value makes the instance <i>not valid</i>
    * @param z depth value in meters. Default value makes the instance <i>not valid</i>
    **/
    constexpr CoordZ( double lat = COORD_NOT_SET_VALUE, double lon = COORD_NOT_SET_VALUE, double z = COORD_NOT_SET_VALUE );

    /**
    * Explicit CoordZ constructor. No implicit cast from Coord to CoordZ is allowed.
    * @param coords a Coord reference
    * @param depth depth value in meters. Default value makes the instance <i>not valid</i>
    **/
    explicit constexpr CoordZ( const Coord& coords, double depth = COORD_NOT_SET_VALUE );
    
    constexpr CoordZ( const CoordZ& copy ) = default;

    constexpr CoordZ( CoordZ&& tmp ) = default;
  
    /**
    * Sets depth 
    * @param d depth in <i>meters</i>
    **/
    constexpr void setDepth( double d ) { depth = d; }

    /**
    * Gets depth 
    * @return depth in <i>meters</i>
    **/
    constexpr double getDepth() const { return depth; }

    /**
    * Gets cartesian x coordinate
    * @param type Earh Model type
    * @return x in <i>meters</i>
    **/
    constexpr CartCoords getCartCoords(CoordZSpheroidType type = CoordZSpheroidType::COORDZ_SPHERE) const;

    /**
    * Gets cartesian x coordinate
    * @param type Earh Model type
    * @return x in <i>meters</i>
    **/
    constexpr double getCartX(CoordZSpheroidType type = CoordZSpheroidType::COORDZ_SPHERE) const;

    /**
    * Gets cartesian y coordinate
    * @param type Earh Model type
    * @return y in <i>meters</i>
    **/
    constexpr double getCartY(CoordZSpheroidType type = CoordZSpheroidType::COORDZ_SPHERE) const;

    /**
    * Gets cartesian z coordinate
    * @param type Earh Model type
    * @return z in <i>meters</i>
    **/
    constexpr double getCartZ(CoordZSpheroidType type = CoordZSpheroidType::COORDZ_SPHERE) const;

    /**
    * Gets spherical rho coordinate, assuming earth as a sphere of ray = 6371000.0 meters
    * @return x in <i>meters</i>
    **/
    constexpr double getSphericalRho() const;
    
    /**
    * Gets spherical theta coordinate, assuming earth as a sphere of ray = 6371000.0 meters
    * @return y in <i>meters</i>
    **/
    constexpr double getSphericalTheta() const;
    
    /**
    * Gets spherical phi coordinate, assuming earth as a sphere of ray = 6371000.0 meters
    * @return z in <i>meters</i>
    **/
    constexpr double getSphericalPhi() const;
    
    /**
    * Gets the distance from cartesian coordinates approximations
    * @param coords a const reference to a valid CoordZ object
    * @return distance in <i>meters</i>
    **/
    constexpr double getCartDistance( const CoordZ& coords, CoordZSpheroidType type = CoordZSpheroidType::COORDZ_SPHERE ) const;

    /**
    * Gets relative zenith from cartesian coordinates approximations, assuming earth as a sphere of ray = 6371000.0 meters
    * @return zenith in <i>radians</i>
    **/
    constexpr double getCartRelZenith( const CoordZ& coords ) const;
    
    /**
    * Gets relative azimuth from cartesian coordinates approximations, assuming earth as a sphere of ray = 6371000.0 meters
    * @return azimuth in <i>radians</i>
    **/
    constexpr double getCartRelAzimuth( const CoordZ& coords ) const;

    /**
    * Gets The CoordZ at given distance along the line in cartesian coordinates, assuming earth as a sphere of ray = 6371000.0 meters
    * that ties <i>start</i> and <i>end</i> CoordZ
    * @param start a const reference to a valid CoordZ object
    * @param end a const reference to a valid CoordZ object
    * @param distance travel distance in meters
    * @return a valid CoordZ object
    **/
    static constexpr CoordZ getCoordZAlongCartLine( const CoordZ& start, const CoordZ& end, double distance );

    /**
    * Gets The CoordZ at given distance along the great circle at starting CoordZ depth. The output 
    * depth is calculated uniformly between start and end depth
    * that ties <i>start</i> and <i>end</i> CoordZ
    * @param start a const reference to a valid CoordZ object
    * @param end a const reference to a valid CoordZ object
    * @param distance travel distance in meters
    * @return a valid CoordZ object
    **/
    static constexpr CoordZ getCoordZAlongGreatCircle( const CoordZ& start, const CoordZ& end, double distance );
    
    /**
    * Gets the CoordZ converted from given cartesian coordinates 
    * @param x x axis
    * @param y y axis
    * @param z z axis
    * @param type Earth model type
    * @return a valid CoordZ object
    **/
    static constexpr CoordZ getCoordZFromCartesianCoords( double x, double y, double z, CoordZSpheroidType type = CoordZSpheroidType::COORDZ_SPHERE);

    /**
    * Gets the CoordZ converted from given cartesian coordinates 
    * @param cart_coords const reference to a valid CoordZ::CartCoords object
    * @return a valid CoordZ object
    **/
    static constexpr CoordZ getCoordZFromCartesianCoords( const CartCoords& cart_coords );
		
    /**
    * Gets the CoordZ converted from given spherical coordinates 
    * @param rho radius
    * @param theta theta angle
    * @param phi phi angle
    * @return a valid CoordZ object
    **/
    static constexpr CoordZ getCoordZFromSphericalCoords( double rho, double theta, double phi);
    
    /**
    * Checks the validity of coordinates and depth provided
    * @return <i>true</i> if coordinates and depth are valid, <i>false</i> otherwise
    **/
    constexpr bool isValid() const { return( Coord::isValid() && depth >= COORDZ_MIN_DEPTH ); }

    /**
    * Assignment operator
    * @param copy const reference to a CoordZ object to be copied
    * @return CoordZ reference to <i>this</i>
    **/
    constexpr CoordZ& operator=( const CoordZ& coordz ) = default;

    /**
    * Move assignment operator
    * @param copy rvalue reference to a CoordZ object to be moved
    * @return CoordZ reference to <i>this</i>
    **/
    constexpr CoordZ& operator=( CoordZ&& coordz ) = default;
    
    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend constexpr CoordZ operator+( const CoordZ& left , const CoordZ& right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend constexpr CoordZ operator-( const CoordZ& left , const CoordZ& right );

    /**
    * Compound assignment sum operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend constexpr CoordZ& operator+=( CoordZ& left, const CoordZ& right );
    
    /**
    * Compound assignment subtraction operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend constexpr CoordZ& operator-=( CoordZ& left, const CoordZ& right );

    /**
    * Equality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left == right</i>, false otherwise
    **/  
    friend constexpr bool operator==( const CoordZ& left , const CoordZ& right ) ;
    
    /**
    * Inequality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left != right</i>, false otherwise
    **/  
    friend constexpr bool operator!=( const CoordZ& left , const CoordZ& right ) ;

    /**
    * Greater than operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left > right</i>, false otherwise
    **/ 
    friend constexpr bool operator>( const CoordZ& left , const CoordZ& right ) ;

    /**
    * Less than operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left < right</i>, false otherwise
    **/ 
    friend constexpr bool operator<( const CoordZ& left , const CoordZ& right ) ;

    /**
    * Greater than or equal to operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left >= right</i>, false otherwise
    **/
    friend constexpr bool operator>=( const CoordZ& left , const CoordZ& right ) ;

    /**
    * Less than or equal to operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left <= right</i>, false otherwise
    **/
    friend constexpr bool operator<=( const CoordZ& left , const CoordZ& right ) ;

    /**
    * << operator
    * @param os left operand ostream reference
    * @param instance right operand const CoordZ reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend std::ostream& operator<<( std::ostream& os, const CoordZ& instance );

    protected:

    /**
    * Depth value
    **/
    double depth;
  };

   class UtmWgs84 {
    
    public:
      
    /**
    * UtmWgs84 constructor.
    * @param utmZone zone number
    * @param east Easting value
    * @param north Northing value
    **/
    constexpr UtmWgs84(int utmZone = COORD_NOT_SET_VALUE, double east = COORD_NOT_SET_VALUE, double north = COORD_NOT_SET_VALUE);

    constexpr int getZone() const;

    constexpr double getEasting() const;

    constexpr double getNorthing() const;

    constexpr bool isValid() const;

    /**
    * Gets destination easting and northing 
    * in UTM - WGS84 coordinates from a given Coord
    * 
    * @param coords reference to a valid Coord object
    * @return UTM - WGS84 coordinates 
    */
    static constexpr UtmWgs84 getUtmWgs84FromCoord(const Coord& coords);

   /**
    * << operator
    * @param os left operand ostream reference
    * @param instance right operand const CoordZ reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend std::ostream& operator<<( std::ostream& os, const UtmWgs84& instance );

    protected:

    int zone; /** Zone number */

    double easting; /** Easting value */

    double northing; /** Northing value */
  };


  /**
  * \brief Function object for partial ordering of coordinates
  * 
  * Function object class for partial ordering of two coordinates instances based on great circle distance.
  * The user class has to provide a <i>static</i> method called <b>getSpaceSampling()</b>
  * that returns the threshold distance
  */
  template < class CompUser, class T = Coord >
  class CoordComparator {
  
    public:
    
    /**
    * Function that compares to T instances. If CompUser::getSpaceSampling() is valid (>=0)
    * two valid T are considered equivalent if their great circle distance is less or equal 
    * to the space sampling value 
    * @param tx const reference to a valid T object
    * @param rx const reference to a valid T object
    * @returns <i>true</i> if x less than y, <i>false</i> otherwise
    **/
    bool operator()( const T& x, const T& y ) const {
      if ( CompUser::getSpaceSampling() <= 0.0 ) 
        return operator<(x,y);
      if ( x.getGreatCircleDistance(y) <= CompUser::getSpaceSampling() ) 
        return false;
      return operator<(x,y);
    }

  };
  
  
    /**
  * \brief Partial specialization for partial ordering of CoordZ coordinates
  * 
  * Function object class for partial ordering of two CoordZ instances based on cartesian distance.
  * The user class has to provide a <i>static</i> method called <b>getSpaceSampling()</b>
  * that returns the threshold distance
  */
  template < class CompUser >
  class CoordComparator< CompUser, CoordZ > {

    public:

    /**
    * Function that compares to woss::CoordZ instances. If CompUser::getSpaceSampling() is valid (>=0)
    * two valid CoordZ are considered equivalent if their cartesian distance is less or equal 
    * to the space sampling value 
    * @param tx const reference to a valid CoordZ object
    * @param rx const reference to a valid CoordZ object
    * @returns <i>true</i> if x less than y, <i>false</i> otherwise
    **/
    bool operator()( const CoordZ& x, const CoordZ& y ) const {
      if ( CompUser::getSpaceSampling() <= 0.0 ) 
        return operator<(x,y);
      if ( x.getCartDistance(y) <= CompUser::getSpaceSampling() ) 
        return false;
      return operator<(x,y);
    }

  };


  // inline functions

  inline constexpr Coord::Coord( double lat, double lon ) 
  : latitude(lat),
    longitude(lon),
    marsden_square(COORD_NOT_SET_VALUE),
    marsden_one_degree(COORD_NOT_SET_VALUE)
  {
    updateMarsdenCoord();
  }


  inline constexpr void Coord::updateMarsdenCoord() {
    if( isValid() == false ) {
      marsden_one_degree = COORD_NOT_SET_VALUE;
      marsden_square = COORD_NOT_SET_VALUE;
      return;
    }
  
    double lat = latitude;
    double lon = longitude;
  
    marsden_one_degree =(int)( (std::floor(std::abs(lat)) - std::floor(std::abs(lat)/10.0)*10.0)*10.0 + (std::floor(std::abs(lon)) - std::floor(std::abs(lon)/10.0)*10.0));
  
    if (( lat >= 0.0) && (lat < 80.0) ) {
      if (lon > 0.0) 
        lon -= 360.0;
      lon = std::abs(lon);
      int quoz_lat = (int)std::floor(lat / 10.0);
      int quoz_long = (int)std::ceil(lon / 10.0);
      // marsden square's longitude should be in range (N, N+10]
      double rem_lon = std::fmod(lon, 10.0);
      if (rem_lon == 0)
        quoz_long += 1;
  
      marsden_square = quoz_lat * 36 + quoz_long;
    }
    else if (lat >= 80.0) {
      if (lon > 0.0) 
        lon -= 360.0;
      lon = std::abs(lon); 
      int quoz_long = (int)std::ceil(lon / 10.0);
      // marsden square's longitude should be in range (N, N+10]
      double rem_lon = std::fmod(lon, 10.0);
      if (rem_lon == 0)
        quoz_long += 1;
      marsden_square = 900 + quoz_long;
    }
    else {
      if (lon > 0.0) 
        lon -= 360.0;
      lon = std::abs(lon); 
      lat = std::abs(lat);
      int quoz_lat = (int)std::floor(lat / 10.0);
      int quoz_long = (int)std::floor(lon / 10.0);
  
      // marsden square of S latitude should be in range (N, N+10]
      double rem_lat = std::fmod(lat, 10.0);
      if (rem_lat == 0)
        quoz_lat -= 1;

      marsden_square = 300 + quoz_lat * 36 + quoz_long;
    }
  }


  inline constexpr Coord Coord::getCoordFromBearing( const Coord& destination, double bearing, double distance, double depth ) {
    assert( destination.isValid() );
  
    double lat1 = destination.latitude * M_PI / 180.0; 
    double lon1 = destination.longitude * M_PI / 180.0;
    double lat2 = std::asin( std::sin(lat1) * std::cos(distance/(EARTH_RADIUS-depth)) 
                + std::cos(lat1) * std::sin(distance/(EARTH_RADIUS-depth)) * std::cos(bearing) );
    double lon2 = lon1 + std::atan2(std::sin(bearing) * std::sin(distance/(EARTH_RADIUS-depth)) * std::cos(lat1), 
                                    std::cos(distance/(EARTH_RADIUS-depth)) - std::sin(lat1) * std::sin(lat2));
  
    double ret_lat = lat2 * 180.0 / M_PI;
    double ret_long = lon2 * 180.0 / M_PI;
  
    ret_long = std::fmod((ret_long + 540.0), 360.0) - 180.0;
  
    if (ret_lat > 90.0) 
      ret_lat -= 180.0;
  
    return( Coord(ret_lat, ret_long) );
  }


  inline constexpr double Coord::getInitialBearing( const Coord& destination ) const {
    assert( destination.isValid() );
  
    double lat1 =  latitude * M_PI / 180.0; 
    double lat2 =  destination.latitude * M_PI / 180.0;
    double dLon =  (destination.longitude - longitude) * M_PI / 180.0;
  
    double y = std::sin(dLon) * std::cos(lat2);
    double x = std::cos(lat1)* std::sin(lat2) - std::sin(lat1)* std::cos(lat2) * std::cos(dLon);
  
    double bearing = std::atan2(y, x);
    // normalize to 0 -> 360
    bearing += 2.0 * M_PI;
    bearing = std::fmod(bearing, 2.0 * M_PI);
  
    return bearing;
  }


  inline constexpr double Coord::getFinalBearing( const Coord& destination ) const {
    double init_bear = destination.getInitialBearing(*this);
  
    double fin_bear = init_bear + M_PI;
    fin_bear = std::fmod(fin_bear, 2.0 * M_PI);
  
    return fin_bear;
  }


  inline constexpr double Coord::getGreatCircleDistance( const Coord& destination, double depth ) const {
    assert( destination.isValid() );
  
    double dLat =  (destination.latitude - latitude) * M_PI / 180.0;
    double dLon =  (destination.longitude - longitude) * M_PI / 180.0;
    double lat1 =  latitude * M_PI / 180.0;
    double lat2 =  destination.latitude * M_PI / 180.0;
  
    double a = std::sin(dLat/2.0) * std::sin(dLat/2.0) +
            std::cos(lat1) * std::cos(lat2) * 
            std::sin(dLon/2.0) * std::sin(dLon/2.0);
  
    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0-a));
  
    return((EARTH_RADIUS - depth) * c);
  }


  inline constexpr bool Coord::isValidUtmZoneChar( UtmZoneChar utm_zone_char ) {
    if ( ( (utm_zone_char >= 'C') && ( utm_zone_char <= 'X' ) && (utm_zone_char != 'I') 
          && (utm_zone_char != 'O') ) 
       || ( (utm_zone_char >= 'c') && ( utm_zone_char <= 'x' ) && (utm_zone_char != 'i') 
          && (utm_zone_char != 'o') )  ) 
      return true;
     
    return false;
  }


  inline constexpr Coord Coord::getCoordFromUtmWgs84( double easting, double northing, double utm_zone_number, UtmZoneChar utm_zone_char) {
    if ( !isValidUtmZoneChar(utm_zone_char) ) {
       std::cerr << "Coord::getCoordFromUtmWgs84() easting = " << easting << "; northing = " << northing
                 << "; utm zone = " << utm_zone_number << "; utm zone char = " << utm_zone_char 
                 << "; WARNING, invalid input parameters!!" << std::endl;
    }
#if 0
    std::cout << "Coord::getCoordFromUtmWgs84() easting = " << easting << "; northing = " << northing
              << "; utm zone = " << utm_zone_number << "; utm zone char = " << utm_zone_char 
              << "; zone int = " << (unsigned int) utm_zone_char << "; c = " << (unsigned int) 'c' 
              << "; m = " << (unsigned int) 'm' << "; C = " << (unsigned int) 'C' << "; M = " 
              << (unsigned int) 'M' << std::endl;
#endif 
    bool is_hemis_north = true;
    
    if (  ((utm_zone_char < 'M') && (utm_zone_char >= 'C')) 
       || ((utm_zone_char < 'm') && (utm_zone_char >= 'c')) ) {
      is_hemis_north = false;
    }
    
    // Equatorial radius
    double sa = EARTH_SEMIMAJOR_AXIS;
    // Polar radius
    double sb = EARTH_WGS84_POLAR_RADIUS;
    
    double e2 = std::pow( ( std::pow(sa, 2.0) - std::pow(sb, 2.0) ), 0.5 ) / sb;
    double e2square = std::pow( e2, 2.0);
    double c = std::pow( sa, 2.0 ) / sb;
    
    // Conventional easting (not referred to standard meridian)
    double X = easting - 500000.0;
    
    // Standard northing shift if the UTM zone is in the southern emispèhere
    double Y = 0.0; 
    
    if ( is_hemis_north == false )
        Y = northing - 10000000.0;
    else
        Y = northing;
        
    // UTM zone
    double S   = ( ( utm_zone_number * 6.0 ) - 183.0 );
    double lat =  Y / ( 6366197.724 * 0.9996 ) ;
    
    double v   = ( c / std::pow( ( 1.0 + ( e2square * std::pow( std::cos(lat), 2.0 ) ) ), 0.5 ) ) * 0.9996;
    
    double a   = X / v ;
    
    double a1   = std::sin( 2.0 * lat ) ;
    double a2   = a1 * std::pow( std::cos(lat), 2.0);
    
    double j2   = lat + a1/2.0 ;
    double j4   = ( 3.0 * j2 + a2 ) / 4.0 ;
    double j6   = ( 5.0 * j4 + a2 * std::pow( std::cos(lat), 2.0 ) ) / 3.0;
    
    double alpha = 3.0/4.0 * e2square ;
    double beta  =  5.0/3.0 * std::pow( alpha, 2.0 );
    double gamma = 35.0/27.0 * std::pow( alpha, 3.0 );
    
    double Bm   = 0.9996 * c * ( lat - alpha * j2 + beta * j4 - gamma * j6 );
    double b    = ( Y - Bm ) / v ;
    double Epsi = e2square * std::pow( a, 2.0 ) / 2.0 * std::pow( std::cos(lat), 2.0 );
    double Epss = a * ( 1.0 - Epsi / 3.0 );
    double nab  = b * ( 1.0 - Epsi ) + lat;

    double Delt = std::atan( std::sinh(Epss) / std::cos(nab) );
    double TaO  = std::atan( std::cos(Delt) * std::tan(nab) );

    double longitude = Delt * 180.0 / M_PI + S;
    double latitude  = ( lat + ( 1.0 + e2square * std::pow( std::cos(lat), 2.0 ) - 3.0/2.0 * e2square * std::sin(lat) * std::cos(lat) * ( TaO - lat ) ) * ( TaO - lat ) ) * 180.0 / M_PI;

#if 0
    std::cout << "Coord::getCoordFromUtmWgs84() latitude = " << latitude 
              << "; longitude = " << longitude << std::endl;
#endif // 
    return Coord(latitude, longitude);
  }


  inline constexpr Coord Coord::getCoordAlongGreatCircle( const Coord& start_coord, const Coord& end_coord, double distance, double depth ) {
    return Coord::getCoordFromBearing( start_coord, start_coord.getInitialBearing(end_coord), distance, depth ); 
  } 


  inline constexpr CoordZ::CoordZ(double lat, double lon, double d) 
  : Coord(lat,lon),
    depth(d)
  {

  }


  inline constexpr CoordZ::CoordZ(const Coord& coords, double d)
  :  Coord(coords),
      depth(d)
  {

  }


  inline constexpr CoordZ::CartCoords::CartCoords()
  : x(0.0),
    y(0.0),
    z(0.0),
    type(CoordZSpheroidType::COORDZ_SPHERE)
  {
  }


  inline constexpr CoordZ::CartCoords::CartCoords(double in_x, double in_y, double in_z, CoordZSpheroidType in_type)
  : x(in_x),
    y(in_y),
    z(in_z),
    type(in_type)
  {
  }

  inline constexpr CoordZ::CartCoords CoordZ::getCartCoords(CoordZSpheroidType type) const {
    double latitudeRadians = M_PI / 180.0 * latitude;
    double longitudeRadians = M_PI / 180.0 * longitude;
    double a = 0.0; // semi-major axis of earth
    double e = 0.0; // first eccentricity of earth
    double altitude = -1.0 * depth;
    
    if (type == CoordZSpheroidType::COORDZ_SPHERE) {
      a = EARTH_RADIUS;
      e = 0.0;
    }
    else if (type == CoordZSpheroidType::COORDZ_GRS80) {
      a = EARTH_SEMIMAJOR_AXIS;
      e = EARTH_GRS80_ECCENTRICITY;
    }
    else { // if type == WGS84
      a = EARTH_SEMIMAJOR_AXIS;
      e = EARTH_WGS84_ECCENTRICITY;
    }
  
    // radius of curvature
    double Rn = a / (std::sqrt(1.0 - std::pow(e, 2.0) * std::pow(std::sin(latitudeRadians), 2.0)));
    double x = (Rn + altitude) * std::cos(latitudeRadians) * std::cos(longitudeRadians);
    double y = (Rn + altitude) * std::cos(latitudeRadians) * std::sin(longitudeRadians);
    double z = ((1 - std::pow (e, 2.0)) * Rn + altitude) * std::sin(latitudeRadians);
  
    return CoordZ::CartCoords(x, y, z, type);
  }

  inline constexpr double CoordZ::getCartX( CoordZSpheroidType type ) const {
    return getCartCoords(type).getX();
  }
  
  
  inline constexpr double CoordZ::getCartY( CoordZSpheroidType type ) const {
    return getCartCoords(type).getY();
  }
  
  
  inline constexpr double CoordZ::getCartZ( CoordZSpheroidType type ) const {
    return getCartCoords(type).getZ();
  }
  
  
  inline constexpr double CoordZ::getSphericalRho() const {
    return EARTH_RADIUS - depth;
  }
  
  
  inline constexpr double CoordZ::getSphericalTheta() const {
    return 90.0 - latitude;
  }
  
  
  inline constexpr double CoordZ::getSphericalPhi() const {
    return longitude;
  }
  
  
  inline constexpr double CoordZ::getCartDistance( const CoordZ& coords, CoordZSpheroidType type ) const {
    assert( coords.isValid() );
  
    CartCoords my_cart_coords = getCartCoords(type);
    CartCoords input_cart_coords = coords.getCartCoords(type);
  
    return std::sqrt(   std::pow((my_cart_coords.getX() - input_cart_coords.getX()), 2.0)
                      + std::pow((my_cart_coords.getY() - input_cart_coords.getY()), 2.0)
                      + std::pow((my_cart_coords.getZ() - input_cart_coords.getZ()), 2.0));
  }


  inline constexpr double CoordZ::getCartRelAzimuth( const CoordZ& coords ) const {
    assert( coords.isValid() );
  
    CartCoords my_cart_coords = getCartCoords(CoordZSpheroidType::COORDZ_SPHERE);
    CartCoords input_cart_coords = coords.getCartCoords(CoordZSpheroidType::COORDZ_SPHERE);
  
    return(std::atan2((input_cart_coords.getY() - my_cart_coords.getY()), (input_cart_coords.getX() - my_cart_coords.getX())));
  }
  
  
  inline constexpr double CoordZ::getCartRelZenith( const CoordZ& coords ) const {
    assert( coords.isValid() );
  
    CartCoords my_cart_coords = getCartCoords(CoordZSpheroidType::COORDZ_SPHERE);
    CartCoords input_cart_coords = coords.getCartCoords(CoordZSpheroidType::COORDZ_SPHERE);
  
    return acos((input_cart_coords.getZ() - my_cart_coords.getZ()) / getCartDistance(coords));
  }


  inline constexpr Coord& operator+=( Coord& left, const Coord& right ) {
    if( !( left.isValid() && right.isValid() ) ) {
      left.latitude = COORD_NOT_SET_VALUE;
      left.longitude = COORD_NOT_SET_VALUE;
    }
    left.latitude += right.latitude;
    left.longitude += right.longitude;
    return left;
  }


  inline constexpr Coord& operator-=( Coord& left, const Coord& right ) {
    if( !( left.isValid() && right.isValid() ) ) {
      left.latitude = COORD_NOT_SET_VALUE;
      left.longitude = COORD_NOT_SET_VALUE;
    }
    left.latitude -= right.latitude;
    left.longitude -= right.longitude;
    return left;
  }


  inline constexpr Coord operator+( const Coord& left , const Coord& right )  {
    if( !( left.isValid() && right.isValid() ) ) 
      return Coord();
    return( Coord( (left.latitude + right.latitude), (left.longitude + right.longitude) ) );
  }


  inline constexpr Coord operator-( const Coord& left , const Coord& right )  {
    if( !( left.isValid() && right.isValid() ) ) 
      return Coord();
    return( Coord( (left.latitude - right.latitude), (left.longitude - right.longitude) ) );
  }


  inline constexpr bool operator==( const Coord& left , const Coord& right )  { 
    if ( &left == &right ) 
      return true; 
    return( left.latitude == right.latitude && left.longitude == right.longitude );
  }


  inline constexpr bool operator!=( const Coord& left , const Coord& right )  { 
    if ( &left == &right ) 
      return false; 
    return( left.latitude != right.latitude || left.longitude != right.longitude );
  }


  inline constexpr bool operator>( const Coord& left , const Coord& right )  { 
    if ( &left == &right ) 
      return false; 
    return( ( left.latitude > right.latitude ) || ( ( left.latitude == right.latitude ) && ( left.longitude > right.longitude ) ) );
  }


  inline constexpr bool operator<( const Coord& left , const Coord& right )  { 
    if ( &left == &right ) 
      return false; 
    return( ( left.latitude < right.latitude ) || ( ( left.latitude == right.latitude ) && ( left.longitude < right.longitude ) ) );
  }


  inline constexpr bool operator>=( const Coord& left , const Coord& right )  { 
    if ( left == right ) 
      return true;
    return( left > right );
  }


  inline constexpr bool operator<=( const Coord& left , const Coord& right )  { 
    if ( left == right ) 
      return true;
    return( left < right );
  }


  inline std::ostream& operator<<(std::ostream& os, const Coord& x) {
    os << "Latitude = " << x.latitude << "; Longitude = " << x.longitude
       << "; Marsden square = " << x.marsden_square
       << "; Marsden One Degree square = " << x.marsden_one_degree;
    return os;
  }


  inline std::ostream& operator<<( std::ostream& os, const CoordZ::CartCoords& instance ) {
    os << "X = " << instance.x << "; Y = " << instance.y << "; Z = " << instance.z << "; type = " << static_cast<int>(instance.type);
    return os;
  }

  
  inline constexpr bool operator==( const CoordZ& left, const CoordZ& right ) { 
    if ( &left == &right ) 
      return true; 
    return( left.latitude == right.latitude && left.longitude == right.longitude && left.depth == right.depth );
  }


  inline constexpr bool operator!=( const CoordZ& left, const CoordZ& right ) { 
    if ( &left == &right ) 
      return false; 
    return( left.latitude != right.latitude || left.longitude != right.longitude || left.depth != right.depth );
  }


  inline constexpr bool operator>( const CoordZ& left, const CoordZ& right ) { 
    if ( &left == &right ) 
      return false; 

    return( ( left.latitude > right.latitude ) || ( ( left.latitude == right.latitude ) && ( left.longitude > right.longitude ) ) 
      || ( ( left.latitude == right.latitude ) && ( left.longitude == right.longitude ) && ( left.depth > right.depth ) ) );
  }


  inline constexpr bool operator<( const CoordZ& left, const CoordZ& right ) { 
    if ( &left == &right ) 
      return false; 
    return ( ( left.latitude < right.latitude ) || ( ( left.latitude == right.latitude ) && ( left.longitude < right.longitude ) ) 
      || ( ( left.latitude == right.latitude ) && ( left.longitude == right.longitude ) && ( left.depth < right.depth ) ) );
  }


  inline constexpr bool operator>=( const CoordZ& left, const CoordZ& right ) { 
    if ( left == right ) 
      return true;
    return( left > right );
  }


  inline constexpr bool operator<=( const CoordZ& left, const CoordZ& right ) { 
    if ( left == right ) 
      return true;
    return( left < right );
  }


  inline constexpr CoordZ& operator+=( CoordZ& left, const CoordZ& right ) {
    if( !( left.isValid() && right.isValid() ) ) {
      left.latitude = COORD_NOT_SET_VALUE;
      left.longitude = COORD_NOT_SET_VALUE;
      left.depth = COORD_NOT_SET_VALUE;
    }
    left.latitude += right.latitude;
    left.longitude += right.longitude;
    left.depth += right.depth;
    return left;
  }

  inline constexpr CoordZ& operator-=( CoordZ& left, const CoordZ& right ) {
    if( !( left.isValid() && right.isValid() ) ) {
      left.latitude = COORD_NOT_SET_VALUE;
      left.longitude = COORD_NOT_SET_VALUE;
      left.depth = COORD_NOT_SET_VALUE;
    }
    left.latitude -= right.latitude;
    left.longitude -= right.longitude;
    left.depth -= right.depth;
    return left;
  }

  inline std::ostream& operator<<( std::ostream& os, const CoordZ& instance ) {
    os << "Latitude = " << instance.latitude << "; Longitude = " << instance.longitude << "; Depth = " << instance.depth
       << "; Marsden square = " << instance.marsden_square
       << "; Marsden One Degree square = " << instance.marsden_one_degree;
    return os;
  }

  constexpr CoordZ CoordZ::getCoordZAlongGreatCircle( const CoordZ& start, const CoordZ& end, double distance ) {
    double start_depth = start.getDepth();
    double end_depth = end.getDepth();
    double total_distance = start.getGreatCircleDistance( end, start_depth );
  
    if ( total_distance == 0.0 && start_depth != end_depth ) {
      total_distance = std::abs(end_depth - start_depth);
      assert( distance < total_distance );

      return( CoordZ( static_cast<Coord>(start), distance ) );
    }
  
    double delta_depth = end_depth - start_depth;
    double curr_depth = start_depth + distance / total_distance * delta_depth;
  
    return( CoordZ( Coord::getCoordAlongGreatCircle( start, end, distance , start_depth ) , curr_depth ) );
  }

  constexpr CoordZ CoordZ::getCoordZAlongCartLine( const CoordZ& start, const CoordZ& end, double distance ) { 
    CoordZ::CartCoords sorg_cart_coords = start.getCartCoords(CoordZSpheroidType::COORDZ_SPHERE);
    double Xsorg_ = sorg_cart_coords.getX();
    double Ysorg_ = sorg_cart_coords.getY();
    double Zsorg_ = sorg_cart_coords.getZ();
  
    double azimut = start.getCartRelAzimuth( end );
    double polar = start.getCartRelZenith( end );
  
    double x_fin = Xsorg_ + distance * std::cos(azimut) * std::sin(polar);
    double y_fin = Ysorg_ + distance * std::sin(azimut) * std::sin(polar);
    double z_fin = Zsorg_ + distance * std::cos(polar);

#if 0
    CartCoords end_cart_coords = end.getCartCoords(CoordZSpheroidType::COORDZ_SPHERE);
    double Xdest_ = end_cart_coords.getX();
    double Ydest_ = end_cart_coords.getY();
    double Zdest_ = end_cart_coords.getZ();

    std::cout << std::endl;
    std::cout << "x_fin = " << x_fin << "; Xdest_ = " << Xdest_ << "; diff = " << (x_fin - Xdest_) << std::endl;
    std::cout << "y_fin = " << y_fin << "; Ydest_ = " << Ydest_ << "; diff = " << (y_fin - Ydest_) << std::endl;
    std::cout << "z_fin = " << z_fin << "; Zdest_ = " << Zdest_ << "; diff = " << (z_fin - Zdest_) << std::endl;
  
    std::cout << std::endl;
    std::cout << "x_fin = " << x_fin << "; Xsorg_ = " << Xsorg_ << "; diff = " << (x_fin - Xsorg_) << std::endl;
    std::cout << "y_fin = " << y_fin << "; Ysorg_ = " << Ysorg_ << "; diff = " << (y_fin - Ysorg_) << std::endl;
    std::cout << "z_fin = " << z_fin << "; Zsorg_ = " << Zsorg_ << "; diff = " << (z_fin - Zsorg_) << std::endl;
  
    std::cout << std::endl;
    std::cout << "Xdest_ = " << Xdest_ << "; Xsorg_ = " << Xsorg_ << "; diff = " << (Xdest_ - Xsorg_) << std::endl;
    std::cout << "Ydest_ = " << Ydest_ << "; Ysorg_ = " << Ysorg_ << "; diff = " << (Ydest_ - Ysorg_) << std::endl;
    std::cout << "Zdest_ = " << Zdest_ << "; Zsorg_ = " << Zsorg_ << "; diff = " << (Zdest_ - Zsorg_) << std::endl;
#endif // 

    double lat = 90.0 - 180.0 / M_PI * acos( z_fin / std::sqrt( std::pow(x_fin,2.0) + std::pow(y_fin,2.0) + std::pow(z_fin,2.0) ) );
    double lon = 180.0 / M_PI * std::atan2( y_fin, x_fin );
    double depth = std::sqrt( std::pow(x_fin,2.0) + std::pow(y_fin,2.0) + std::pow(z_fin,2.0) ) - EARTH_RADIUS;

#if 0
     std::cout << "lat = " << lat << std::endl;
     std::cout << "long = " << lon << std::endl;
     std::cout << "depth = " << depth << std::endl;
#endif // 

    return CoordZ( lat, lon, std::abs(depth) );
  }

  inline constexpr CoordZ CoordZ::getCoordZFromCartesianCoords( const CartCoords& cart_coords ) {
    return CoordZ::getCoordZFromCartesianCoords( cart_coords.getX(), cart_coords.getY(), cart_coords.getZ(), cart_coords.getType() );
  }

  inline constexpr CoordZ CoordZ::getCoordZFromCartesianCoords( double x, double y, double z, CoordZSpheroidType type ) {
    double polarRadius = 0.0;
    double equatorRadius = 0.0;
    double e2Param = 0.0;
  
    if (type == CoordZSpheroidType::COORDZ_SPHERE) {
      equatorRadius = EARTH_RADIUS;
      e2Param = 0.0;
    }
    else if (type == CoordZSpheroidType::COORDZ_GRS80) {
      polarRadius = EARTH_GRS80_POLAR_RADIUS;// GRS80 ellipsoide
      equatorRadius = EARTH_SEMIMAJOR_AXIS; 
      e2Param = ( std::pow(equatorRadius, 2.0) - std::pow(polarRadius, 2.0) ) / std::pow(equatorRadius, 2.0);
    }
    else { // if type == COORDZ_WGS84
      polarRadius = EARTH_WGS84_POLAR_RADIUS;// WGS84 ellipsoide
      equatorRadius = EARTH_SEMIMAJOR_AXIS; 
      e2Param = ( std::pow(equatorRadius, 2.0) - std::pow(polarRadius, 2.0) ) / std::pow(equatorRadius, 2.0);
    }
  
    double latitude = COORD_NOT_SET_VALUE;
    double longitude = COORD_NOT_SET_VALUE;
    double altitude = COORD_NOT_SET_VALUE;
  
    // distance from the position point (P) to earth center point (origin O)
    double op = std::sqrt( x*x + y*y + z*z );
  
    if ( op > 0.0 ) {
      // longitude calculation
      double lon2 = std::atan(y / x);
  
      // scale longitude between -PI and PI (-180.0 and 180.0 in degrees)
      if ( x != 0.0 || y != 0.0 ) {
        longitude = std::atan(y/x) * 180.0/M_PI;
  
        if ( x < 0.0 ) {
          if ( y > 0.0) {
            longitude = 180.0 + longitude;
            lon2 = lon2 - M_PI;
          }
          else {
            longitude = -180.0 + longitude;
            lon2 = M_PI + lon2;
          }
        }
      }
  
      // Geocentric latitude
      double latG = std::atan(z / (std::sqrt( x*x + y*y )));
  
      // Geocentric latitude (of point Q, Q is intersection point of segment OP and reference ellipsoid)
      double latQ = std::atan(z / ( (1.0 - e2Param ) * (std::sqrt( x*x + y*y ))) );
  
      // calculate radius of the curvature
      double rCurvature = ( equatorRadius / std::sqrt(1.0 - e2Param * std::sin(latQ) * std::sin(latQ)) );
  
      // x, y, z of point Q
      double xQ = rCurvature * std::cos(latQ) * std::cos(lon2);
      double yQ = rCurvature * std::cos(latQ) * std::sin(lon2);
      double zQ = rCurvature * (1.0 - e2Param) * std::sin(latQ);
  
      // distance OQ
      double oq = std::sqrt( xQ*xQ + yQ*yQ + zQ*zQ );
  
      // distance PQ is OP - OQ
      double pq = op - oq;
  
      // length of the normal segment from point P of line (PO) to point T.
      // T is intersection point of linen the PO normal and ellipsoid normal from point Q.
      double tp = pq * std::sin(latG - latQ);
  
      double lat_radians = latQ + tp / op * std::cos(latQ - latG);
      latitude = lat_radians*180.0/M_PI;
  
      altitude = pq * std::cos(latQ - latG);
#if 0
      std::cout << "lat new = " << latitude << std::endl;
      std::cout << "lon new = " << longitude << std::endl;
      std::cout << "alt new = " << altitude << std::endl;
#endif // 
    }
  
    return CoordZ( latitude, longitude, std::abs(altitude) );
  }

  constexpr CoordZ CoordZ::getCoordZFromSphericalCoords( double rho, double theta, double phi ) {
    // we want to be underwater!
    assert(rho <= EARTH_RADIUS);
  
    return CoordZ( (90.0 - theta), phi, std::abs(EARTH_RADIUS - rho) );
  }

  inline constexpr CoordZ operator+( const CoordZ& left , const CoordZ& right ) {
    if( !( left.isValid() && right.isValid() ) ) 
      return CoordZ();
    return( CoordZ( left.latitude + right.latitude, left.longitude + right.longitude, left.depth + right.depth ) );
  }

  inline constexpr CoordZ operator-( const CoordZ& left , const CoordZ& right ) {
    if( !( left.isValid() && right.isValid() ) ) 
      return CoordZ();
    return( CoordZ( left.latitude - right.latitude, left.longitude - right.longitude, left.depth - right.depth ) );
  }

  inline constexpr int UtmWgs84::getZone() const {
    return zone;
  }

  inline constexpr double UtmWgs84::getEasting() const { 
    return easting;
  }

  inline constexpr double UtmWgs84::getNorthing() const { 
    return northing;
  }
  
  inline constexpr bool UtmWgs84::isValid() const { 
    return ((zone != COORD_NOT_SET_VALUE) && (easting != COORD_NOT_SET_VALUE) && (northing != COORD_NOT_SET_VALUE)); 
  }
  
  inline std::ostream& operator<<( std::ostream& os, const UtmWgs84& instance ) {
    os << "Zone = " << instance.zone << "; Easting = " << instance.easting << "; Northing = " << instance.northing;
    return os;
  }


  inline constexpr UtmWgs84::UtmWgs84(int utmZone, double east, double north)
  : zone(utmZone),
    easting(east),
    northing(north)
  {
  }
  
  inline constexpr UtmWgs84 UtmWgs84::getUtmWgs84FromCoord(const Coord& coords) {
    double latitude = coords.getLatitude();
    double longitude = coords.getLongitude();
    double falseEasting = 500000.0;
    double falseNorthing = 10000000.0;
  
    int zone = std::floor((longitude + 180.0)/6.0) + 1;
    double lambda0 = ((zone-1)*6.0 - 180.0 + 3.0)*M_PI / 180.0;//central meridian
    double phi = latitude*M_PI/180.0;
    double lambda = longitude*M_PI/180.0 - lambda0;
   
    double a = Coord::EARTH_SEMIMAJOR_AXIS;
    double f = 1/298.2572215381486;
  
    double k0 = 0.9996;
  
    double e = std::sqrt(f*(2.0-f));
    double n = f/(2.0-f);
    double n2 = std::pow(n,2.0);
    double n3 = std::pow(n,3.0);
    double n4 = std::pow(n,4.0);
    double n5 = std::pow(n,5.0);
    double n6 = std::pow(n,6.0);
  
    double cosLambda = std::cos(lambda);
    double sinLambda = std::sin(lambda);
  
    double tau = std::tan(phi);
    double sigma = std::sinh(e*atanh(e*tau / std::sqrt(1.0+std::pow(tau,2.0)) ));
  
    double tau_prime = tau*std::sqrt(1.0 + std::pow(sigma,2.0)) - sigma*std::sqrt(1.0 + std::pow(tau,2.0));
  
    double epsilon_prime = std::atan2(tau_prime, cosLambda);
    double eta_prime = std::asinh(sinLambda/ std::sqrt( std::pow(tau_prime,2.0) + std::pow(cosLambda,2.0)));
  
    double A = a/(1.0+n) * (1.0 + 1.0/4.0*n2 + 1.0/64.0*n4 + 1.0/256.0*n6);
    double alpha[] = {0.0,
          1.0/2.0*n - 2.0/3.0*n2 + 5.0/16.0*n3 + 41.0/180.0*n4 -127.0/288.0*n5 + 7891.0/37800.0*n6,
          13.0/48.0*n2 - 3.0/5.0*n3 + 557.0/1440.0*n4 + 281.0/630.0*n5 - 1983433.0/1935360.0*n6,
          61.0/240.0*n3 - 103.0/140.0*n4 + 15061.0/26880.0*n5 + 167603.0/181440.0*n6,
          49561.0/161280.0*n4 - 179.0/168*n5 + 6601661.0/7257600.0*n6,
          34729.0/80640.0*n5 - 3418889.0/1995840.0*n6,
          212378941.0/319334400.0*n6};
    double epsilon = epsilon_prime;
  
    for(int j = 1; j<=6; j++) {
      epsilon += alpha[j]*std::sin(2.0*j*epsilon_prime)*std::cosh(2.0*j*eta_prime);
    }
  
    double eta = eta_prime;
    for(int j = 1; j<=6; j++) {
      eta += alpha[j]*std::cos(2.0*j*epsilon_prime)*std::sinh(2.0*j*eta_prime);
    }
  
    double x = k0*A*eta;
    double y = k0*A*epsilon;
  
    x = x + falseEasting;
  
    if(y<0.0)
        y = y + falseNorthing; 

    return UtmWgs84(zone,x,y);
  }
}

#endif /* WOSS_COORDINATES_DEFINITIONS_H */

