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
 * @file   coordinates-definitions.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of the woss::Coord and woss::CoordZ classes
 *
 * Provides the implementation of the woss::Coord and woss::CoordZ classes
 */


#include <cmath>
#include <assert.h>


#include "coordinates-definitions.h"


using namespace woss;


double Coord::earth_radius = 6371000.0;

const double Coord::COORD_MIN_LATITUDE = -90.0;

const double Coord::COORD_MAX_LATITUDE = 90.0;

const double Coord::COORD_MIN_LONGITUDE = -180.0;

const double Coord::COORD_MAX_LONGITUDE = 180.0;

Coord::Coord( double lat, double lon ) 
: latitude(lat),
  longitude(lon),
  marsden_square(COORD_NOT_SET_VALUE),
  marsden_one_degree(COORD_NOT_SET_VALUE)
{
  updateMarsdenCoord();
}


Coord::Coord( const Coord& coords ) 
{
  latitude = coords.latitude;
  longitude = coords.longitude;
  updateMarsdenCoord();
}


void Coord::updateMarsdenCoord() {
  if( !isValid() ) {
    marsden_one_degree = COORD_NOT_SET_VALUE;
    marsden_square = COORD_NOT_SET_VALUE;
    return;
  }

  double lat = latitude;
  double lon = longitude;

  marsden_one_degree =(int)( (floor(std::abs(lat)) - floor(std::abs(lat)/10.0)*10.0)*10.0 + (floor(std::abs(lon)) - floor(std::abs(lon)/10.0)*10.0));

  if (( lat >= 0.0) && (lat < 80.0) ) {
    if (lon > 0.0) 
      lon -= 360.0;
    lon = std::abs(lon);
    int quoz_lat = (int)floor(lat / 10.0);
    int quoz_long = (int)ceil(lon / 10.0);
    // marsden square's longitude should be in range (N, N+10]
    double rem_lon = fmod(lon, 10.0);
    if (rem_lon == 0)
      quoz_long += 1;

    marsden_square = quoz_lat * 36 + quoz_long;
  }
  else if (lat >= 80.0) {
    if (lon > 0.0) 
      lon -= 360.0;
    lon = std::abs(lon); 
    int quoz_long = (int)ceil(lon / 10.0);
    // marsden square's longitude should be in range (N, N+10]
    double rem_lon = fmod(lon, 10.0);
    if (rem_lon == 0)
      quoz_long += 1;
    marsden_square = 900 + quoz_long;
  }
  else {
    if (lon > 0.0) 
      lon -= 360.0;
    lon = std::abs(lon); 
    lat = std::abs(lat);
    int quoz_lat = (int)floor(lat / 10.0);
    int quoz_long = (int)floor(lon / 10.0);

    // marsden square of S latitude should be in range (N, N+10]
    double rem_lat = fmod(lat, 10.0);
    if (rem_lat == 0)
      quoz_lat -= 1;

    marsden_square = 300 + quoz_lat * 36 + quoz_long;
  }
}


double Coord::getInitialBearing( const Coord& destination ) const {
  assert( static_cast<Coord>(destination).isValid() );

  double lat1 =  latitude * M_PI / 180.0; 
  double lat2 =  destination.latitude * M_PI / 180.0;
  double dLon =  (destination.longitude - longitude) * M_PI / 180.0;

  double y = sin(dLon) * cos(lat2);
  double x = cos(lat1)* sin(lat2) - sin(lat1)* cos(lat2) * cos(dLon);

  double bearing = atan2(y, x);
  // normalize to 0 -> 360
  bearing += 2.0 * M_PI;
  bearing = fmod(bearing, 2.0 * M_PI);

  return bearing;
}

double Coord::getFinalBearing( const Coord& destination ) const {
  double init_bear = destination.getInitialBearing(*this);

  double fin_bear = init_bear + M_PI;
  fin_bear = fmod(fin_bear, 2.0 * M_PI);

  return fin_bear;
}


double Coord::getGreatCircleDistance( const Coord& destination, double depth ) const {
  assert( static_cast<Coord>(destination).isValid() );

  double dLat =  (destination.latitude - latitude) * M_PI / 180.0;
  double dLon =  (destination.longitude - longitude) * M_PI / 180.0;
  double lat1 =  latitude * M_PI / 180.0;
  double lat2 =  destination.latitude * M_PI / 180.0;

  double a = sin(dLat/2.0) * sin(dLat/2.0) +
          cos(lat1) * cos(lat2) * 
          sin(dLon/2.0) * sin(dLon/2.0);

  double c = 2.0 * atan2(sqrt(a), sqrt(1.0-a));

  return((earth_radius - depth) * c);
}


const Coord Coord::getCoordFromBearing( const Coord& destination, double bearing, double distance, double depth ) {
  assert( static_cast<Coord>(destination).isValid() );
 
  double lat1 = destination.latitude * M_PI / 180.0; 
  double lon1 = destination.longitude * M_PI / 180.0;
  double lat2 = asin( sin(lat1) * cos(distance/(earth_radius-depth)) 
              + cos(lat1) * sin(distance/(earth_radius-depth)) * cos(bearing) );
  double lon2 = lon1 + atan2(sin(bearing) * sin(distance/(earth_radius-depth)) * cos(lat1), 
                             cos(distance/(earth_radius-depth)) - sin(lat1) * sin(lat2));

  double ret_lat = lat2 * 180.0 / M_PI;
  double ret_long = lon2 * 180.0 / M_PI;

  ret_long = fmod((ret_long + 540.0), 360.0) - 180.0;

  if (ret_lat > 90.0) ret_lat -= 180.0;

  return( Coord(ret_lat, ret_long) );
}


bool Coord::isValidUtmZoneChar( UtmZoneChar utm_zone_char )
{
  if ( ( (utm_zone_char >= 'C') && ( utm_zone_char <= 'X' ) && (utm_zone_char != 'I') 
        && (utm_zone_char != 'O') ) 
     || ( (utm_zone_char >= 'c') && ( utm_zone_char <= 'x' ) && (utm_zone_char != 'i') 
        && (utm_zone_char != 'o') )  ) 
    return true;
   
  return false;
}


const Coord Coord::getCoordFromUtmWgs84( double easting, double northing, double utm_zone_number, UtmZoneChar utm_zone_char)
{
  if ( !isValidUtmZoneChar(utm_zone_char) ) {
     ::std::cerr << "Coord::getCoordFromUtmWgs84() easting = " << easting << "; northing = " << northing
                 << "; utm zone = " << utm_zone_number << "; utm zone char = " << utm_zone_char 
                 << "; WARNING, invalid input parameters!!" << ::std::endl;
  }
  
  /*::std::cout << "Coord::getCoordFromUtmWgs84() easting = " << easting << "; northing = " << northing
              << "; utm zone = " << utm_zone_number << "; utm zone char = " << utm_zone_char 
              << "; zone int = " << (unsigned int) utm_zone_char << "; c = " << (unsigned int) 'c' 
              << "; m = " << (unsigned int) 'm' << "; C = " << (unsigned int) 'C' << "; M = " 
              << (unsigned int) 'M' << ::std::endl; */
  
  bool is_hemis_north = true;
  
  if (  ((utm_zone_char < 'M') && (utm_zone_char >= 'C')) 
     || ((utm_zone_char < 'm') && (utm_zone_char >= 'c')) ) {
    is_hemis_north = false;
  }
  
  // Equatorial radius
  double sa = 6378137.000000 ; 
  // Polar radius
  double sb = 6356752.314245 ;
  
  double e2 = ::std::pow( ( ::std::pow(sa, 2.0) - ::std::pow(sb, 2.0) ), 0.5 ) / sb;
  double e2square = ::std::pow( e2, 2.0);
  double c = ::std::pow( sa, 2.0 ) / sb;
  
  // Conventional easting (not referred to standard meridian)
  double X = easting - 500000.0;
  
  // Standard northing shift if the UTM zone is in the southern emispèhere
  double Y; 
  
  if ( is_hemis_north == false )
      Y = northing - 10000000.0;
  else
      Y = northing;
      
  // UTM zone
  double S   = ( ( utm_zone_number * 6.0 ) - 183.0 );
  double lat =  Y / ( 6366197.724 * 0.9996 ) ;
  
  double v   = ( c / ::std::pow( ( 1.0 + ( e2square * ::std::pow( ::std::cos(lat), 2.0 ) ) ), 0.5 ) ) * 0.9996;
  
  double a   = X / v ;
  
  double a1   = ::std::sin( 2.0 * lat ) ;
  double a2   = a1 * ::std::pow( ::std::cos(lat), 2.0);
  
  double j2   = lat + a1/2.0 ;
  double j4   = ( 3.0 * j2 + a2 ) / 4.0 ;
  double j6   = ( 5.0 * j4 + a2 * ::std::pow( ::std::cos(lat), 2.0 ) ) / 3.0;
  
  double alpha = 3.0/4.0 * e2square ;
  double beta  =  5.0/3.0 * ::std::pow( alpha, 2.0 );
  double gamma = 35.0/27.0 * ::std::pow( alpha, 3.0 );
  
  double Bm   = 0.9996 * c * ( lat - alpha * j2 + beta * j4 - gamma * j6 );
  double b    = ( Y - Bm ) / v ;
  double Epsi = e2square * ::std::pow( a, 2.0 ) / 2.0 * ::std::pow( ::std::cos(lat), 2.0 );
  double Epss = a * ( 1.0 - Epsi / 3.0 );
  double nab  = b * ( 1.0 - Epsi ) + lat;
  
  double Delt = ::std::atan( ::std::sinh(Epss) / ::std::cos(nab) );
  double TaO  = ::std::atan( ::std::cos(Delt) * ::std::tan(nab) );
  
  double longitude = Delt * 180.0 / M_PI + S;
  double latitude  = ( lat + ( 1.0 + e2square * ::std::pow( ::std::cos(lat), 2.0 ) - 3.0/2.0 * e2square * ::std::sin(lat) * ::std::cos(lat) * ( TaO - lat ) ) * ( TaO - lat ) ) * 180.0 / M_PI;
  
//     ::std::cout << "Coord::getCoordFromUtmWgs84() latitude = " << latitude 
//                 << "; longitude = " << longitude << ::std::endl;

  return Coord(latitude, longitude);
}


const Coord Coord::getCoordAlongGreatCircle( const Coord& start_coord, const Coord& end_coord, double distance, double depth ) {
  return Coord::getCoordFromBearing( start_coord, start_coord.getInitialBearing(end_coord), distance, depth ); 
} 


Coord& Coord::operator=( const Coord& coords ) {
  if (this == &coords) return *this;
  latitude = coords.latitude;
  longitude = coords.longitude;
  marsden_square = coords.marsden_square;
  marsden_one_degree = coords.marsden_one_degree;
  return *this;
}


Coord& woss::operator+=( Coord& left, const Coord& right ) {
  if( !( left.isValid() && right.isValid() ) ) {
    left.latitude = COORD_NOT_SET_VALUE;
    left.longitude = COORD_NOT_SET_VALUE;
  }
  left.latitude += right.latitude;
  left.longitude += right.longitude;
  return left;
}


Coord& woss::operator-=( Coord& left, const Coord& right ) {
  if( !( left.isValid() && right.isValid() ) ) {
    left.latitude = COORD_NOT_SET_VALUE;
    left.longitude = COORD_NOT_SET_VALUE;
  }
  left.latitude -= right.latitude;
  left.longitude -= right.longitude;
  return left;
}

const double CoordZ::COORDZ_MIN_DEPTH = 0.0;

CoordZ::CoordZ(double lat, double lon, double d) 
  : Coord(lat,lon),
    depth(d)
{

}


CoordZ::CoordZ(const Coord& coords, double d)
  :  Coord(coords),
     depth(d)
{

}


CoordZ::CoordZ(const CoordZ& coordz) 
: Coord(coordz),
  depth(coordz.depth)
{

}


double CoordZ::getCartX() const {
  return (earth_radius - depth) * sin( (90.0 - latitude) * M_PI / 180.0 ) * cos( longitude * M_PI / 180.0 );
}


double CoordZ::getCartY() const {
  return (earth_radius - depth) * sin( (90.0 - latitude) * M_PI / 180.0 ) * sin( longitude * M_PI / 180.0 );
}


double CoordZ::getCartZ() const {
  return (earth_radius - depth) * cos( (90.0 - latitude) * M_PI / 180.0 );
}
    
    
double CoordZ::getCartDistance( const CoordZ& coords ) const {   
  return sqrt( pow( (coords.getCartX() - getCartX()) ,2.0) + pow( (coords.getCartY() - getCartY()) ,2.0) 
             + pow( (coords.getCartZ() - getCartZ()) ,2.0) );
}


double CoordZ::getCartRelAzimuth( const CoordZ& coords ) const {
  assert( coords.isValid() );   
  return( atan2( (coords.getCartY() - getCartY()) , (coords.getCartX() - getCartX()) )  );
}


double CoordZ::getCartRelZenith( const CoordZ& coords ) const {
  assert( coords.isValid() );
  return acos( (coords.getCartZ() - getCartZ()) / getCartDistance(coords) );
}


const CoordZ CoordZ::getCoordZAlongGreatCircle( const CoordZ& start, const CoordZ& end, double distance ) {
  double start_depth = start.getDepth();
  double end_depth = end.getDepth();
  double total_distance = start.getGreatCircleDistance( end, start_depth );

  if ( total_distance == 0.0 && start_depth != end_depth ) {
    total_distance = ::std::abs(end_depth - start_depth);  
    assert( distance < total_distance );

    return( CoordZ( static_cast<Coord>(start), distance ) );
  }

  double delta_depth = end_depth - start_depth;
  double curr_depth = start_depth + distance / total_distance * delta_depth;

  return( CoordZ( Coord::getCoordAlongGreatCircle( start, end, distance , start_depth ) , curr_depth ) );
}


const CoordZ CoordZ::getCoordZAlongCartLine( const CoordZ& start, const CoordZ& end, double distance ) { 
  double Xsorg_ = start.getCartX();
  double Ysorg_ = start.getCartY();
  double Zsorg_ = start.getCartZ();

//  double Xdest_ = end.getCartX();
//  double Ydest_ = end.getCartY();
//  double Zdest_ = end.getCartZ();

  double azimut = start.getCartRelAzimuth( end );
  double polar = start.getCartRelZenith( end );

  double x_fin = Xsorg_ + distance * cos(azimut) * sin(polar);
  double y_fin = Ysorg_ + distance * sin(azimut) * sin(polar);
  double z_fin = Zsorg_ + distance * cos(polar);

//  ::std::cout << ::std::endl;
//  ::std::cout << "x_fin = " << x_fin << "; Xdest_ = " << Xdest_ << "; diff = " << (x_fin - Xdest_) << ::std::endl;
//  ::std::cout << "y_fin = " << y_fin << "; Ydest_ = " << Ydest_ << "; diff = " << (y_fin - Ydest_) << ::std::endl;
//  ::std::cout << "z_fin = " << z_fin << "; Zdest_ = " << Zdest_ << "; diff = " << (z_fin - Zdest_) << ::std::endl;

//  ::std::cout << ::std::endl;
//  ::std::cout << "x_fin = " << x_fin << "; Xsorg_ = " << Xsorg_ << "; diff = " << (x_fin - Xsorg_) << ::std::endl;
//  ::std::cout << "y_fin = " << y_fin << "; Ysorg_ = " << Ysorg_ << "; diff = " << (y_fin - Ysorg_) << ::std::endl;
//  ::std::cout << "z_fin = " << z_fin << "; Zsorg_ = " << Zsorg_ << "; diff = " << (z_fin - Zsorg_) << ::std::endl;

//  ::std::cout << ::std::endl;
//  ::std::cout << "Xdest_ = " << Xdest_ << "; Xsorg_ = " << Xsorg_ << "; diff = " << (Xdest_ - Xsorg_) << ::std::endl;
//  ::std::cout << "Ydest_ = " << Ydest_ << "; Ysorg_ = " << Ysorg_ << "; diff = " << (Ydest_ - Ysorg_) << ::std::endl;
//  ::std::cout << "Zdest_ = " << Zdest_ << "; Zsorg_ = " << Zsorg_ << "; diff = " << (Zdest_ - Zsorg_) << ::std::endl;

  double lat = 90.0 - 180.0 / M_PI * acos( z_fin / sqrt( pow(x_fin,2.0) + pow(y_fin,2.0) + pow(z_fin,2.0) ) );
  double lon = 180.0 / M_PI * atan2( y_fin, x_fin );
  double depth = sqrt( pow(x_fin,2.0) + pow(y_fin,2.0) + pow(z_fin,2.0) ) - earth_radius;

//   ::std::cout << "lat = " << lat << ::std::endl;
//   ::std::cout << "long = " << lon << ::std::endl;
//   ::std::cout << "depth = " << depth << ::std::endl;

  return CoordZ( lat, lon, ::std::abs(depth) );
}
   
    
CoordZ& CoordZ::operator=( const CoordZ& coordz ) {
  if (this == &coordz) return *this;
  latitude = coordz.latitude;
  longitude = coordz.longitude;
  marsden_square = coordz.marsden_square;
  marsden_one_degree = coordz.marsden_one_degree;
  depth = coordz.depth;
  return *this;
}


CoordZ& woss::operator+=( CoordZ& left, const CoordZ& right ) {
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
  

CoordZ& woss::operator-=( CoordZ& left, const CoordZ& right ) {
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


const CoordZ CoordZ::getCoordZFromCartesianCoords( double x, double y, double z ) {
  double wgs84_polarRadius = 6356752.314245;     // WGS84 ellipsoide
  double equatorRadius = 6378137.0;
  double e2Param = ( pow(equatorRadius,2.0) - pow(wgs84_polarRadius,2.0) ) / pow(equatorRadius,2.0);

  double latitude = COORD_NOT_SET_VALUE;
  double longitude = COORD_NOT_SET_VALUE;
  double altitude = COORD_NOT_SET_VALUE;

  // distance from the position point (P) to earth center point (origin O)
  double op = std::sqrt ( x*x + y*y + z*z );

  if ( op > 0 ) {
    // longitude calculation
    double lon2 = std::atan (y / x);

    // scale longitude between -PI and PI (-180 and 180 in degrees)
    if ( x != 0 || y != 0 ) {
      longitude = std::atan(y/x) * 180/M_PI;

      if ( x < 0 ) {
        if ( y > 0) {
          longitude = 180 + longitude;
          lon2 = lon2 - M_PI;
        }
        else {
          longitude = -180 + longitude;
          lon2 = M_PI + lon2;
        }
      }
    }

    // Geocentric latitude
    double latG = std::atan (z / (std::sqrt ( x*x + y*y )));

    // Geocentric latitude (of point Q, Q is intersection point of segment OP and reference ellipsoid)
    double latQ = std::atan (z / ( (1 - e2Param ) * (std::sqrt ( x*x + y*y ))) );

    // calculate radius of the curvature
    double rCurvature = ( equatorRadius / std::sqrt (1 - e2Param * std::sin (latQ) * std::sin (latQ)) );

    // x, y, z of point Q
    double xQ = rCurvature * std::cos (latQ) * std::cos (lon2);
    double yQ = rCurvature * std::cos (latQ) * std::sin (lon2);
    double zQ = rCurvature * (1 - e2Param) * std::sin (latQ);

    // distance OQ
    double oq = std::sqrt ( xQ*xQ + yQ*yQ + zQ*zQ );

    // distance PQ is OP - OQ
    double pq = op - oq;

    // length of the normal segment from point P of line (PO) to point T.
    // T is intersection point of linen the PO normal and ellipsoid normal from point Q.
    double tp = pq * std::sin (latG - latQ);

    double lat_radians = latQ + tp / op * std::cos (latQ - latG);
    latitude = lat_radians*180/M_PI;

    altitude = pq * std::cos (latQ - latG);

    //std::cout << "lat new = " << latitude << std::endl;
    //std::cout << "lon new = " << longitude << std::endl;
    //std::cout << "alt new = " << altitude << std::endl;
  }

  return CoordZ( latitude, longitude, ::std::abs(altitude) );
}


const CoordZ CoordZ::getCoordZFromSphericalCoords( double rho, double theta, double phi ) {
  // we want to be underwater!
  assert(rho <= earth_radius);
	
  return CoordZ( (90.0 - theta), phi, ::std::abs(earth_radius - rho) );
}


UtmWgs84::UtmWgs84(int utmZone, double east, double north)
: zone(utmZone),
  easting(east),
  northing(north)
{
}


const UtmWgs84 UtmWgs84::getUtmWgs84FromCoord(const Coord& coords) {
  double latitude = coords.getLatitude();
  double longitude = coords.getLongitude();
  //std::cout << "lat " << latitude << " long " << longitude << std::endl;

  double falseEasting = 500000;
  double falseNorthing = 10000000;

  int zone = floor((longitude + 180)/6) + 1;
  //std::cout << "UTM zone " << zone << std::endl;
  double lambda0 = ((zone-1)*6 - 180 + 3)*M_PI / 180;//central meridian

  double phi = latitude*M_PI/180;
  double lambda = longitude*M_PI/180 - lambda0;
 
  double a = 6378137;
  double f = 1/298.2572215381486;

  double k0 = 0.9996;

  double e = sqrt(f*(2-f));
  double n = f/(2-f);
  double n2 = pow(n,2);
  double n3 = pow(n,3);
  double n4 = pow(n,4);
  double n5 = pow(n,5);
  double n6 = pow(n,6);

  double cosLambda = cos(lambda);
  double sinLambda = sin(lambda);

  double tau = tan(phi);
  double sigma = sinh(e*atanh(e*tau / sqrt(1+pow(tau,2)) ));

  double tau_prime = tau*sqrt(1+ pow(sigma,2)) - sigma*sqrt(1 +pow(tau,2));

  double epsilon_prime = atan2(tau_prime, cosLambda);
  double eta_prime = asinh(sinLambda/ sqrt( pow(tau_prime,2) + pow(cosLambda,2)));

  double A = a/(1+n) * (1 + 1.0/4*n2 + 1.0/64*n4 + 1.0/256*n6);
  double alpha[] = {0,
        1.0/2*n - 2.0/3*n2 + 5.0/16*n3 + 41.0/180*n4 -127.0/288*n5 + 7891.0/37800*n6,
        13.0/48*n2 - 3.0/5*n3 + 557.0/1440*n4 + 281.0/630*n5 - 1983433.0/1935360*n6,
        61.0/240*n3 - 103.0/140*n4 + 15061.0/26880*n5 + 167603.0/181440*n6,
        49561.0/161280*n4 - 179.0/168*n5 + 6601661.0/7257600*n6,
        34729.0/80640*n5 - 3418889.0/1995840*n6,
        212378941.0/319334400*n6};
  double epsilon = epsilon_prime;

  for(int j = 1; j<=6; j++) {
    epsilon += alpha[j]*sin(2*j*epsilon_prime)*cosh(2*j*eta_prime);
  }

  double eta = eta_prime;
  for(int j = 1; j<=6; j++) {
    eta += alpha[j]*cos(2*j*epsilon_prime)*sinh(2*j*eta_prime);
  }

  double x = k0*A*eta;
  double y = k0*A*epsilon;

  x = x + falseEasting;

  if(y<0)
      y = y + falseNorthing; 

  return UtmWgs84(zone,x,y);
}
