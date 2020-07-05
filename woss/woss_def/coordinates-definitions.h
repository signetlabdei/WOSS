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
 * @file   coordinates-definitions.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for the woss::Coord and woss::CoordZ classes
 *
 * Provides the interface for the woss::Coord and woss::CoordZ classes
 */


#ifndef WOSS_COORDINATES_DEFINITIONS_H
#define WOSS_COORDINATES_DEFINITIONS_H


#include <vector>
#include <utility>
#include <iostream>


namespace woss {
    
    
  class Coord;
  class CoordZ;
  
  /**
  * The letter that identify a UTM zone
  **/
  typedef char UtmZoneChar;

  /**
  * A vector of CoordZ. Beware of object slicing if using this container for objects <i>derived</i>
  * from CoordZ
  **/
  typedef ::std::vector< CoordZ > CoordZVector;

  /**
  * The marsden square
  **/
  typedef int Marsden;

  /**
  * The Marsden coordinates 
  * @param int marsden square
  * @param int marsden one degree square
  **/
  typedef ::std::pair< int, int > MarsdenCoord; 
  
  /**
  * A vector of Coord
  **/
  typedef ::std::vector< Coord > CoordVector;

  /**
  * A vector of marsden squares
  **/
  typedef ::std::vector< Marsden > MarsdenVector;

  /**
  * A vector of marsden coordinates
  **/
  typedef ::std::vector< MarsdenCoord > MarsdenCoordVector;


  /**
  * Default value for not intialized variables
  **/
  #define COORD_NOT_SET_VALUE (-2000)


  /**
  * \brief Coordinates (lat, long) class definitions and functions library
  *
  * Coord class stores a double <b>decimal degree</b> latitude and double <b>decimal degree</b> longitude. 
  * It has a reach library for Marsden coordinates, distance, bearing and arithmetic calculations. This class has virtual
  * methods for the sole purpose of CoordZ inheritance. 
  **/
  class Coord {

    public:

    static const double COORD_MIN_LATITUDE; ///< Minimum valid Latitude

    static const double COORD_MAX_LATITUDE; ///< Maximum valid Latitude

    static const double COORD_MIN_LONGITUDE; ///< Minimum valid Longitude

    static const double COORD_MAX_LONGITUDE; ///< Maximum valid Longitude

    /**
    * Coord constructor.
    * @param lat <b>decimal degree</b> latitude value. Default value makes the instance <i>not valid</i>
    * @param lon <b>decimal degree</b> longitude value. Default value makes the instance <i>not valid</i>
    **/
    Coord( double lat = COORD_NOT_SET_VALUE, double lon = COORD_NOT_SET_VALUE );

    /**
    * Coord copy constructor.
    * @param copy Coord to be copied
    **/
    Coord( const Coord& copy );

    virtual ~Coord() { }

    /**
    * Sets the mean earth radius
    * @param value earth radius in meters
    **/
    static void setEarthRadius( double value ) { earth_radius = value; }
  
    /**
    * Gets the mean earth radius
    * @returns mean earth radius in meters
    **/
    static double getEarthRadius() { return earth_radius; }
    
    
    /**
    * Sets latitude and updates marsden coordinates
    * @param lat latitude value
    **/
    void setLatitude( double lat ) { latitude = lat; updateMarsdenCoord(); }

    /**
    * Sets longitude and updates marsden coordinates
    * @param lon longitude value
    **/
    void setLongitude( double lon ) { longitude = lon; updateMarsdenCoord(); }


    /**
    * Checks the validity of coordinates provided
    * @return <i>true</i> if coordinates are valid, <i>false</i> otherwise
    **/
    virtual bool isValid() const { return( latitude >= COORD_MIN_LATITUDE && latitude <= COORD_MAX_LATITUDE
                                        && longitude >= COORD_MIN_LONGITUDE && longitude <= COORD_MAX_LONGITUDE ); }

    /**
    * Gets the latitude value
    * @return latitude value of the instance
    **/
    double getLatitude() const { return latitude; }

    /**
    * Gets the longitude value
    * @return longitude value of the instance
    **/
    double getLongitude() const { return longitude; } 
  
    /**
    * Gets the marsden square value
    * @return marsden square value of the instance
    **/
    int getMarsdenSquare() const { return marsden_square; }

    /**
    * Gets the marsden one degree square value
    * @return marsden one degree square value of the instance
    **/
    int getMarsdenOneDegreeSquare() const { return marsden_one_degree; }

    /**
    * Gets the marsden coordinates
    * @return complete marsden coordinates of the instance
    **/
    MarsdenCoord getMarsdenCoord() const { return( ::std::make_pair( marsden_square, marsden_one_degree) ); }


    /**
    * Gets the initial bearing of a trajectory
    * @param destination destination Coord instance 
    * @return initial bearing measured in <i>radians</i>
    **/
    double getInitialBearing( const Coord& destination ) const ;

    /**
    * Gets the final bearing of a trajectory
    * @param destination destination Coord instance 
    * @return initial bearing measured in <i>radians</i>
    **/
    double getFinalBearing( const Coord& destination ) const ;

    /**
    * Gets calculates great-circle distances between the two points – that is, 
    * the shortest distance over the earth’s surface – using the ‘Haversine’ formula
    * @param destination <b>valid</b> destination Coord instance 
    * @return distance measured in <i>meters</i>
    **/
    double getGreatCircleDistance( const Coord& destination, double depth = 0 ) const ;

    
    /**
    * Gets destination Cood given bearing and distance from a start Coord,
    * travelling along a (shortest distance) great circle arc of given depth
    * @param start_coord <b>valid</b> start Coord instance 
    * @param bearing bearing measured in <i>radians</i>
    * @param distance distance measured in <i>meters</i>
    * @param depth depth measured in <i>meters</i>
    * @return a new Coord instance containing the destination coordinates
    **/
    static const Coord getCoordFromBearing( const Coord& start_coord, double bearing, double distance, double depth = 0.0 ); 
    
    /**
    * Gets destination Cood given bearing and distance from a start Coord,
    * travelling along a (shortest distance) great circle arc of given depth to end Coord
    * @param start_coord <b>valid</b> start Coord instance 
    * @param end_coord <b>valid</b> end Coord instance
    * @param distance distance measured in <i>meters</i>
    * @param depth depth measured in <i>meters</i>
    * @return a new Coord instance containing the destination coordinates
    **/
    static const Coord getCoordAlongGreatCircle( const Coord& start_coord, const Coord& end_coord, double distance, double depth = 0.0 ); 
    
    /**
    * Gets destination Cood given easting and northing in UTM - WGS84 coordinates
    * @param easting <b>valid</b> easting relative to central UTM zone meridian
    * @param end_coord <b>valid</b> northing
    * @param utm_zone_number <b>valid</b>zone number
    * @param utm_zone_char <b>valid</b>zone character
    * @return a new Coord instance containing the destination coordinates
    **/    
    static const Coord getCoordFromUtmWgs84( double easting, double northing, double utm_zone_number, UtmZoneChar utm_zone_char );
    
    
    /**
    * Assignment operator
    * @param copy const reference to a Coord object to be copied
    * @return Coord reference to <i>this</i>
    **/
    Coord& operator=( const Coord& copy );


    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Coord operator+( const Coord& left , const Coord& right ) ;

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Coord operator-( const Coord& left , const Coord& right ) ;


    /**
    * Compound assignment sum operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Coord& operator+=( Coord& left, const Coord& right );
    
    /**
    * Compound assignment subtraction operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Coord& operator-=( Coord& left, const Coord& right );
    

    /**
    * Equality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left == right</i>, false otherwise
    **/  
    friend bool operator==( const Coord& left , const Coord& right ) ;
    
    /**
    * Inequality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left != right</i>, false otherwise
    **/  
    friend bool operator!=( const Coord& left , const Coord& right ) ;
    
    
    /**
    * Greater than operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left > right</i>, false otherwise
    **/ 
    friend bool operator>( const Coord& left , const Coord& right ) ;

    /**
    * Less than operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left < right</i>, false otherwise
    **/ 
    friend bool operator<( const Coord& left , const Coord& right ) ;

    /**
    * Greater than or equal to operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left >= right</i>, false otherwise
    **/
    friend bool operator>=( const Coord& left , const Coord& right ) ;

    /**
    * Less than or equal to operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left <= right</i>, false otherwise
    **/
    friend bool operator<=( const Coord& left , const Coord& right ) ;

    
    /**
    * << operator
    * @param os left operand ostream reference
    * @param instance right operand const Coord reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend ::std::ostream& operator<<( ::std::ostream& os, const Coord& instance );
    

    protected:

   
    /**
    * Mean earth radius in meters
    **/
    static double earth_radius;
    
    
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
    void updateMarsdenCoord();

    /**
    * Checks if the passed utm zone character is valid
    * @param utm_zone_char utm zone character
    * @return <i>true</i> if valid, <i>false</i> otherwise
    **/ 
    static bool isValidUtmZoneChar( UtmZoneChar utm_zone_char );

  };


  /**
  * \brief 3D-Coordinates (lat, long, depth) class definitions and functions library
  *
  * CoordZ class inherit from Coord, adding a depth value
  **/
  class CoordZ : public Coord {

    
    public:

    static const double COORDZ_MIN_DEPTH; ///< Minimum valid depth

    /**
    * CoordZ constructor.
    * @param lat <b>decimal degree</b> latitude value. Default value makes the instance <i>not valid</i>
    * @param lon <b>decimal degree</b> longitude value. Default value makes the instance <i>not valid</i>
    * @param z depth value in meters. Default value makes the instance <i>not valid</i>
    **/
    CoordZ( double lat = COORD_NOT_SET_VALUE, double lon = COORD_NOT_SET_VALUE, double z = COORD_NOT_SET_VALUE );

    /**
    * Explicit CoordZ constructor. No implicit cast from Coord to CoordZ is allowed.
    * @param coords a Coord reference
    * @param depth depth value in meters. Default value makes the instance <i>not valid</i>
    **/
    explicit CoordZ( const Coord& coords, double depth = COORD_NOT_SET_VALUE );
    
    /**
    * CoordZ copy constructor.
    * @param copy const reference to a CoordZ to be copied
    **/
    CoordZ( const CoordZ& copy );

    virtual ~CoordZ() { }


    /**
    * Sets depth 
    * @param d depth in <i>meters</i>
    **/
    void setDepth( double d ) { depth = d; }


    /**
    * Gets depth 
    * @return depth in <i>meters</i>
    **/
    double getDepth() const { return depth; }


    /**
    * Gets cartesian x coordinate, assuming earth as a sphere of ray = 6371000.0 meters
    * @return x in <i>meters</i>
    **/
    double getCartX() const;
    
    /**
    * Gets cartesian y coordinate, assuming earth as a sphere of ray = 6371000.0 meters
    * @return y in <i>meters</i>
    **/
    double getCartY() const;
    
    /**
    * Gets cartesian z coordinate, assuming earth as a sphere of ray = 6371000.0 meters
    * @return z in <i>meters</i>
    **/
    double getCartZ() const;
    
    
    /**
    * Gets spherical rho coordinate, assuming earth as a sphere of ray = 6371000.0 meters
    * @return x in <i>meters</i>
    **/
    double getSphericalRho() const;
    
    /**
    * Gets spherical theta coordinate, assuming earth as a sphere of ray = 6371000.0 meters
    * @return y in <i>meters</i>
    **/
    double getSphericalTheta() const;
    
    /**
    * Gets spherical phi coordinate, assuming earth as a sphere of ray = 6371000.0 meters
    * @return z in <i>meters</i>
    **/
    double getSphericalPhi() const;    
    
    
    /**
    * Gets the distance from cartesian coordinates approximations
    * @param coords a const reference to a valid CoordZ object
    * @return distance in <i>meters</i>
    **/
    double getCartDistance( const CoordZ& coords ) const;
    
    /**
    * Gets relative zenith from cartesian coordinates approximations
    * @return zenith in <i>radians</i>
    **/
    double getCartRelZenith( const CoordZ& coords ) const;
    
    /**
    * Gets relative azimuth from cartesian coordinates approximations
    * @return azimuth in <i>radians</i>
    **/
    double getCartRelAzimuth( const CoordZ& coords ) const;
   
   
    /**
    * Gets The CoordZ at given distance along the line in cartesian coordinates 
    * that ties <i>start</i> and <i>end</i> CoordZ
    * @param start a const reference to a valid CoordZ object
    * @param end a const reference to a valid CoordZ object
    * @param distance travel distance in meters
    * @return a valid CoordZ object
    **/
    static const CoordZ getCoordZAlongCartLine( const CoordZ& start, const CoordZ& end, double distance );

    /**
    * Gets The CoordZ at given distance along the great circle at starting CoordZ depth. The output 
    * depth is calculated uniformly between start and end depth
    * that ties <i>start</i> and <i>end</i> CoordZ
    * @param start a const reference to a valid CoordZ object
    * @param end a const reference to a valid CoordZ object
    * @param distance travel distance in meters
    * @return a valid CoordZ object
    **/
    static const CoordZ getCoordZAlongGreatCircle( const CoordZ& start, const CoordZ& end, double distance );
    
    /**
    * Gets the CoordZ converted from given cartesian coordinates 
    * @param x x axis
    * @param y y axis
    * @param z z axis
    * @return a valid CoordZ object
    **/
    static const CoordZ getCoordZFromCartesianCoords( double x, double y, double z);
		
    /**
    * Gets the CoordZ converted from given spherical coordinates 
    * @param rho radius
    * @param theta theta angle
    * @param phi phi angle
    * @return a valid CoordZ object
    **/
    static const CoordZ getCoordZFromSphericalCoords( double rho, double theta, double phi);
    
    /**
    * Checks the validity of coordinates and depth provided
    * @return <i>true</i> if coordinates and depth are valid, <i>false</i> otherwise
    **/
    virtual bool isValid() const { return( Coord::isValid() && depth >= COORDZ_MIN_DEPTH ); }


    /**
    * Assignment operator
    * @param copy const reference to a CoordZ object to be copied
    * @return CoordZ reference to <i>this</i>
    **/
    CoordZ& operator=( const CoordZ& coordz );

    
    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const CoordZ operator+( const CoordZ& left , const CoordZ& right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const CoordZ operator-( const CoordZ& left , const CoordZ& right );


    /**
    * Compound assignment sum operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend CoordZ& operator+=( CoordZ& left, const CoordZ& right );
    
    /**
    * Compound assignment subtraction operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend CoordZ& operator-=( CoordZ& left, const CoordZ& right );
    

    /**
    * Equality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left == right</i>, false otherwise
    **/  
    friend bool operator==( const CoordZ& left , const CoordZ& right ) ;
    
    /**
    * Inequality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left != right</i>, false otherwise
    **/  
    friend bool operator!=( const CoordZ& left , const CoordZ& right ) ;
    
    
    /**
    * Greater than operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left > right</i>, false otherwise
    **/ 
    friend bool operator>( const CoordZ& left , const CoordZ& right ) ;

    /**
    * Less than operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left < right</i>, false otherwise
    **/ 
    friend bool operator<( const CoordZ& left , const CoordZ& right ) ;

    /**
    * Greater than or equal to operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left >= right</i>, false otherwise
    **/
    friend bool operator>=( const CoordZ& left , const CoordZ& right ) ;

    /**
    * Less than or equal to operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left <= right</i>, false otherwise
    **/
    friend bool operator<=( const CoordZ& left , const CoordZ& right ) ;

    
    /**
    * << operator
    * @param os left operand ostream reference
    * @param instance right operand const CoordZ reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend ::std::ostream& operator<<( ::std::ostream& os, const CoordZ& instance );
    

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
    UtmWgs84(int utmZone = COORD_NOT_SET_VALUE, double east = COORD_NOT_SET_VALUE, double north = COORD_NOT_SET_VALUE);

    ~UtmWgs84() { }


    int getZone() const;

    double getEasting() const;

    double getNorthing() const;

    bool isValid() const;

    /**
    * Gets destination easting and northing 
    * in UTM - WGS84 coordinates from a given Coord
    * 
    * @param coords reference to a valid Coord object
    * @return UTM - WGS84 coordinates 
    */
    static const UtmWgs84 getUtmWgs84FromCoord(const Coord& coords);

   /**
    * << operator
    * @param os left operand ostream reference
    * @param instance right operand const CoordZ reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend ::std::ostream& operator<<( ::std::ostream& os, const UtmWgs84& instance );

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
//       ::std::cout << "CoordComparator::operator() x " <<  x << "; y " << y << "; dist " << x.getCartDistance(y)
//                   << "; space samp " << CompUser::getSpaceSampling() << ::std::endl;     
                  
      if ( CompUser::getSpaceSampling() <= 0.0 ) return operator<(x,y);
      if ( x.getGreatCircleDistance(y) <= CompUser::getSpaceSampling() ) return false;
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
//       ::std::cout << "CoordComparator::operator() x " <<  x << "; y " << y << "; dist " << x.getCartDistance(y)
//                   << "; space samp " << CompUser::getSpaceSampling() << ::std::endl;     
                  
      if ( CompUser::getSpaceSampling() <= 0.0 ) return operator<(x,y);
      if ( x.getCartDistance(y) <= CompUser::getSpaceSampling() ) return false;
      return operator<(x,y);
    }
      
  
  };
  
  //non-inline operator declarations
  ///////////////////////
  Coord& operator+=( Coord& left, const Coord& right );

  Coord& operator-=( Coord& left, const Coord& right );

  CoordZ& operator+=( CoordZ& left, const CoordZ& right );
 
  CoordZ& operator-=( CoordZ& left, const CoordZ& right );
 
    
  ///////////////////////////////
  // inline functions

  inline double CoordZ::getSphericalRho() const {
    return earth_radius - depth;
  }
  

  inline double CoordZ::getSphericalTheta() const {
    return 90.0 - latitude;
  }
  

  inline double CoordZ::getSphericalPhi() const {
    return longitude;
  }


  inline const Coord operator+( const Coord& left , const Coord& right )  {
    if( !( left.isValid() && right.isValid() ) ) return Coord();
    return( Coord( (left.latitude + right.latitude), (left.longitude + right.longitude) ) );
  }


  inline const Coord operator-( const Coord& left , const Coord& right )  {
    if( !( left.isValid() && right.isValid() ) ) return Coord();
    return( Coord( (left.latitude - right.latitude), (left.longitude - right.longitude) ) );
  }


  inline bool operator==( const Coord& left , const Coord& right )  { 
    if ( &left == &right ) return true; 
    return( left.latitude == right.latitude && left.longitude == right.longitude );
  }


  inline bool operator!=( const Coord& left , const Coord& right )  { 
    if ( &left == &right ) return false; 
    return( left.latitude != right.latitude || left.longitude != right.longitude );
  }


  inline bool operator>( const Coord& left , const Coord& right )  { 
    if ( &left == &right ) return false; 
    return( ( left.latitude > right.latitude ) || ( ( left.latitude == right.latitude ) && ( left.longitude > right.longitude ) ) );
  }


  inline bool operator<( const Coord& left , const Coord& right )  { 
    if ( &left == &right ) return false; 
    return( ( left.latitude < right.latitude ) || ( ( left.latitude == right.latitude ) && ( left.longitude < right.longitude ) ) );
  }


  inline bool operator>=( const Coord& left , const Coord& right )  { 
    if ( left == right ) return true;
    return( left > right );
  }


  inline bool operator<=( const Coord& left , const Coord& right )  { 
    if ( left == right ) return true;
    return( left < right );
  }


  inline ::std::ostream& operator<<(::std::ostream& os, const Coord& x) {
    os << "Latitude = " << x.latitude << "; Longitude = " << x.longitude
       << "; Marsden square = " << x.marsden_square
       << "; Marsden One Degree square = " << x.marsden_one_degree;
    return os;
  }

    
  inline bool operator==( const CoordZ& left, const CoordZ& right ) { 
    if ( &left == &right ) return true; 
    return( left.latitude == right.latitude && left.longitude == right.longitude && left.depth == right.depth );
  }


  inline bool operator!=( const CoordZ& left, const CoordZ& right ) { 
    if ( &left == &right ) return false; 
    return( left.latitude != right.latitude || left.longitude != right.longitude || left.depth != right.depth );
  }


  inline bool operator>( const CoordZ& left, const CoordZ& right ) { 
    if ( &left == &right ) return false; 

    return( ( left.latitude > right.latitude ) || ( ( left.latitude == right.latitude ) && ( left.longitude > right.longitude ) ) 
      || ( ( left.latitude == right.latitude ) && ( left.longitude == right.longitude ) && ( left.depth > right.depth ) ) );
  }


  inline bool operator<( const CoordZ& left, const CoordZ& right ) { 
    if ( &left == &right ) return false; 
    return ( ( left.latitude < right.latitude ) || ( ( left.latitude == right.latitude ) && ( left.longitude < right.longitude ) ) 
      || ( ( left.latitude == right.latitude ) && ( left.longitude == right.longitude ) && ( left.depth < right.depth ) ) );
  }


  inline bool operator>=( const CoordZ& left, const CoordZ& right ) { 
    if ( left == right ) return true;
    return( left > right );
  }


  inline bool operator<=( const CoordZ& left, const CoordZ& right ) { 
    if ( left == right ) return true;
    return( left < right );
  }


  inline ::std::ostream& operator<<( ::std::ostream& os, const CoordZ& instance ) {
    os << "Latitude = " << instance.latitude << "; Longitude = " << instance.longitude << "; Depth = " << instance.depth
       << "; Marsden square = " << instance.marsden_square
       << "; Marsden One Degree square = " << instance.marsden_one_degree;
    return os;
  }


  inline const CoordZ operator+( const CoordZ& left , const CoordZ& right ) {
    if( !( left.isValid() && right.isValid() ) ) return CoordZ();
    return( CoordZ( left.latitude + right.latitude, left.longitude + right.longitude, left.depth + right.depth ) );
  }


  inline const CoordZ operator-( const CoordZ& left , const CoordZ& right ) {
    if( !( left.isValid() && right.isValid() ) ) return CoordZ();
    return( CoordZ( left.latitude - right.latitude, left.longitude - right.longitude, left.depth - right.depth ) );
  }

  inline int UtmWgs84::getZone() const {
    return zone; 
  }

  inline double UtmWgs84::getEasting() const { 
    return easting; 
  }

  inline double UtmWgs84::getNorthing() const { 
    return northing; 
  }
  
  inline bool UtmWgs84::isValid() const { 
    return ((zone != COORD_NOT_SET_VALUE) && (easting != COORD_NOT_SET_VALUE) && (northing != COORD_NOT_SET_VALUE)); 
  }
  
  inline ::std::ostream& operator<<( ::std::ostream& os, const UtmWgs84& instance ) {
    os << "Zone = " << instance.zone << "; Easting = " << instance.easting << "; Northing = " << instance.northing;
    return os;
  }
}

#endif /* WOSS_COORDINATES_DEFINITIONS_H */

