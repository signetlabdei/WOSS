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
 * @file   transducer-definitions.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of the the woss::Transducer class
 *
 * Provides the implementation of the the woss::Transducer class
 */


#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <definitions.h>
#include "coordinates-definitions.h"
#include "transducer-definitions.h"

#define TRANSDUCER_WRITE_PRECISION ( 17 )
#define TRANSDUCER_WRITE_MARGIN ( TRANSDUCER_WRITE_PRECISION + 9 )


static inline void convSpher2Cart( double rho, double theta, double phi, double& x, double& y, double& z);

static inline void convCart2Spher( double x, double y, double z, double& rho, double& theta, double& phi);

static inline void rotCartXY( double rot_angle, double x_init, double y_init, double& x, double& y);

static inline void rotCartXZ( double rot_angle, double x_init, double z_init, double& x, double& z);

static inline void rotCartYZ( double rot_angle, double y_init, double z_init, double& y, double& z);

static inline void crossProd( double left_x, double left_y, double left_z, double right_x, double right_y, double right_z, double& res_x, double& res_y, double& res_z);

static inline double scalarProd( double left_x, double left_y, double left_z, double right_x, double right_y, double right_z);

static inline double linInterp( double start_var, double end_var, double start_val, double end_val, double x_var );


static double normCart( double x, double y, double z ) {
  return ( sqrt(x*x + y*y + z*z) );
}


void convSpher2Cart( double rho, double theta, double phi, double& x, double& y, double& z) {
  x = rho*::std::sin(theta) * ::std::cos(phi);
  y = rho*::std::sin(theta) * ::std::sin(phi);
  z = rho*::std::cos(theta); 
}


void convCart2Spher( double x, double y, double z, double& rho, double& theta, double& phi) {
  rho = ::std::sqrt( x*x + y*y + z* z);
  theta = ::std::acos( z / rho );
  phi = ::std::atan2( y, x );  
}


void rotCartXY( double rot_angle, double x_init, double y_init, double& x, double& y ) {
  if (rot_angle == 0) {
    x = x_init;
    y = y_init;
    return;
  }
  x = ::std::cos(rot_angle)*x_init - ::std::sin(rot_angle)*y_init;
  y = ::std::sin(rot_angle)*x_init + ::std::cos(rot_angle)*y_init;
}


void rotCartXZ( double rot_angle, double x_init, double z_init, double& x, double& z) {
  if (rot_angle == 0) {
    x = x_init;
    z = z_init;
    return;
  }  
  x = ::std::cos(rot_angle)*x_init - ::std::sin(rot_angle)*z_init;
  z = ::std::sin(rot_angle)*x_init + ::std::cos(rot_angle)*z_init;  
}


void rotCartYZ( double rot_angle, double y_init, double z_init, double& y, double& z) {
  if (rot_angle == 0) {
    y = y_init;
    z = z_init;
    return;
  }  
  y = ::std::cos(rot_angle)*y_init - ::std::sin(rot_angle)*z_init;
  z = ::std::sin(rot_angle)*y_init + ::std::cos(rot_angle)*z_init;    
}


void crossProd( double left_x, double left_y, double left_z, double right_x, double right_y, double right_z, double& res_x, double& res_y, double& res_z ) {
  res_x = left_y*right_z - left_z*right_y;
  res_y = left_z*right_x - left_x*right_z;
  res_z = left_x*right_y - left_y*right_x;
}


static double scalarProd( double left_x, double left_y, double left_z, double right_x, double right_y, double right_z ) {
  return( left_x*right_x + left_y*right_y + left_z*right_z );
}


double linInterp( double start_var, double end_var, double start_val, double end_val, double x_var ) {
    return (end_val-start_val)/(end_var-start_var) * (x_var-start_var) + start_val;
}



using namespace woss;


bool Transducer::debug = false;


const ::std::string Transducer::conical_string = "CONICAL";  

const ::std::string Transducer::toroidal_string = "TOROIDAL";


Transducer::Transducer( long double beam_prec, long double cond_prec, long double tvr_prec, long double ocv_prec )
: has_conical_symmetry( false ),
  resonance_frequency( TRANSDUCER_NOT_SET ),
  bandwith_3db( TRANSDUCER_NOT_SET ),
  max_power( TRANSDUCER_NOT_SET ),
  duty_cycle( TRANSDUCER_NOT_SET ),
  beam_precision( beam_prec ),
  conductance_precision( cond_prec ),
  tvr_precision( tvr_prec ),
  ocv_precision( ocv_prec ),
  type_name(),
  beam_power_map(),
  conductance_map(),
  tvr_map(),
  ocv_map()
{

}


Transducer::Transducer( const Transducer& copy )
: has_conical_symmetry( copy.has_conical_symmetry ),
  resonance_frequency( copy.resonance_frequency ),
  bandwith_3db( copy.bandwith_3db ),
  max_power( copy.max_power ),
  duty_cycle( copy.duty_cycle ),
  beam_precision( copy.beam_precision ),
  conductance_precision( copy.conductance_precision ),
  tvr_precision( copy.tvr_precision ),
  ocv_precision( copy.ocv_precision ),
  type_name( copy.type_name ),
  beam_power_map( copy.beam_power_map ),
  conductance_map( copy.conductance_map ),
  tvr_map( copy.tvr_map ),
  ocv_map( copy.ocv_map )
{

}


Transducer::Transducer( BeamPowerMap& beam_map, ConductanceMap& cond_map, TVRMap& t_map, OCVMap& o_map )
: has_conical_symmetry(false),
  resonance_frequency( TRANSDUCER_NOT_SET ),
  bandwith_3db( TRANSDUCER_NOT_SET ),
  max_power( TRANSDUCER_NOT_SET ),
  duty_cycle( TRANSDUCER_NOT_SET ),
  beam_precision( BEAM_PATTERN_CUSTOM_BEAM_PRECISION ),
  conductance_precision( CONDUCTANCE_CUSTOM_FREQUENCY_PRECISION ),
  tvr_precision( TVR_CUSTOM_FREQUENCY_PRECISION ),
  ocv_precision( OCV_CUSTOM_FREQUENCY_PRECISION ),
  type_name()
{
  beam_power_map.swap(beam_map);
  conductance_map.swap(cond_map);
  tvr_map.swap(t_map);
  ocv_map.swap(o_map);

  if ( !beam_power_map.empty() ) beam_precision = beam_power_map.begin()->first.getPrecision();
  if ( !conductance_map.empty() ) conductance_precision = conductance_map.begin()->first.getPrecision();
  if ( !tvr_map.empty() ) tvr_precision = tvr_map.begin()->first.getPrecision();
  if ( !ocv_map.empty() ) ocv_precision = ocv_map.begin()->first.getPrecision();
}


Transducer* Transducer::create( long double beam_prec, long double cond_prec, long double tvr_prec, long double ocv_prec ) const {
  return new Transducer( beam_prec, cond_prec, tvr_prec, ocv_prec );
}


Transducer* Transducer::create( BeamPowerMap& beam_map, ConductanceMap& cond_map, TVRMap& t_map, OCVMap& o_map ) const {
  return new Transducer( beam_map, cond_map, t_map, o_map );
}


Transducer* Transducer::create( const Transducer& copy ) const {
  return new Transducer( copy );
}

Transducer* Transducer::clone() const {
  return new Transducer( *this );
}


bool Transducer::isValid() const {
  return(  resonance_frequency != TRANSDUCER_NOT_SET && bandwith_3db != TRANSDUCER_NOT_SET && duty_cycle != TRANSDUCER_NOT_SET
        && max_power != TRANSDUCER_NOT_SET && ( !beam_power_map.empty() ) && (!tvr_map.empty())
        && (!ocv_map.empty()) && (!conductance_map.empty()) );
}
 
    
Transducer& Transducer::operator=( const Transducer& copy )  {
  if (this == &copy) return *this;
  
  has_conical_symmetry = copy.has_conical_symmetry;
  resonance_frequency = copy.resonance_frequency;
  bandwith_3db = copy.bandwith_3db;
  max_power = copy.max_power;
  duty_cycle = copy.duty_cycle;
  
  beam_precision = copy.beam_precision;
  conductance_precision = copy.conductance_precision;
  tvr_precision = copy.tvr_precision;
  ocv_precision = copy.ocv_precision;

  beam_power_map = copy.beam_power_map;
  conductance_map = copy.conductance_map;
  tvr_map = copy.tvr_map;
  ocv_map = copy.ocv_map;

  return( *this );
}


Transducer& Transducer::setBeamPrecision( long double prec ) {
  if ( prec == beam_precision ) return *this;
  
  BeamPowerMap beam_power_temp;
  
  for ( BPMIter it = beam_power_map.begin(); it != beam_power_map.end(); it++ ) {
    beam_power_temp.insert( ::std::make_pair( PDouble( it->first.getValue(), prec), it->second ) );
  }
  beam_power_map.swap(beam_power_temp);
  return *this;
}


Transducer& Transducer::setOCVPrecision( long double prec ) {
  if ( prec == beam_precision ) return *this;
  
  OCVMap ocv_temp;
  
  for ( OCVMIter it = ocv_map.begin(); it != ocv_map.end(); it++ ) {
    ocv_temp.insert( ::std::make_pair( PDouble( it->first.getValue(), prec), it->second ) );
  }
  ocv_map.swap(ocv_temp);
  return *this;
}


Transducer& Transducer::setConductancePrecision( long double prec ) {
  if ( prec == conductance_precision ) return *this;
  
  ConductanceMap conductance_temp;
  
  for ( CMIter it = conductance_map.begin(); it != conductance_map.end(); it++ ) {
    conductance_temp.insert( ::std::make_pair( PDouble( it->first.getValue(), prec), it->second ) );
  }
  conductance_map.swap(conductance_temp);
  return *this;
}


Transducer& Transducer::setTVRPrecision( long double prec ) {
  if ( prec == tvr_precision ) return *this;
  
  TVRMap tvr_temp;
  
  for ( TVRMIter it = tvr_map.begin(); it != tvr_map.end(); it++ ) {
    tvr_temp.insert( ::std::make_pair( PDouble( it->first.getValue(), prec), it->second ) );
  }
  tvr_map.swap(tvr_temp);
  return *this;
}


const ::std::string& Transducer::getSymmetryString() const {
  if (has_conical_symmetry) return conical_string;
  else return toroidal_string;
}


void Transducer::beampattern_rotate( double angle, BeamPowerMap& map ) {
  double norm_angle = normalizeAngle(angle);

//   ::std::cout << "Transducer::rotate() angle = " << angle << "; norm angle = " << norm_angle << ::std::endl;
  if (norm_angle == 0) return;
  
  BeamPowerMap beam_power_temp;
  
  for ( BPMIter it = map.begin(); it != map.end(); it++ ) {
    double new_angle = normalizeAngle(it->first.getValue() + norm_angle);
    beam_power_temp[ PDouble(new_angle, beam_precision) ] = it->second;
    
//     ::std::cout << "Transducer::rotate() old angle = " << it->first 
//                 << "; rot angle = " << new_angle << ::std::endl;

  }
  
  double min_angle = beam_power_temp.begin()->first;
  double max_angle = beam_power_temp.rbegin()->first;
  double min_angle_value_lin = ::std::pow( 10.0, beam_power_temp.begin()->second / 10.0 );
  double max_angle_value_lin = ::std::pow( 10.0, beam_power_temp.rbegin()->second / 10.0 );               
  double min_angle_2 = min_angle;
  if ( min_angle < 0 ) min_angle_2 = 360.0 + min_angle;
  
  double delta_angle = ::std::abs(max_angle - min_angle_2);
  double delta_value_lin = min_angle_value_lin - max_angle_value_lin;

//   ::std::cout << "Transducer::rotate() min angle " << min_angle << "; min angle 2 " << min_angle_2 
//               << "; max angle " << max_angle << "; min angle value " << min_angle_value_lin 
//               << "; max angle value " << max_angle_value_lin << "; delta angle " << delta_angle 
//               << "; delta value " << delta_value_lin << ::std::endl;
              
  if ( min_angle != -180.0 || max_angle != 180.0 ) {
    beam_power_temp[PDouble(-180.0, beam_precision) ] = 10.0*log10( min_angle_value_lin - (180.0 - ::std::abs(min_angle)) * (delta_value_lin / delta_angle) );
    beam_power_temp[PDouble(180.0, beam_precision) ] = 10.0*log10( max_angle_value_lin + (180.0 - ::std::abs(max_angle)) * ( delta_value_lin / delta_angle) );
  }
  
  map.swap(beam_power_temp);
}


void Transducer::beampattern_sum( double value, BeamPowerMap& map ) {
  if ( value == 0.0 ) return;
  for ( BPMIter it = map.begin(); it != map.end(); ++it ) {
    it->second += value;
  }
}
 
 
void Transducer::beampattern_multiply( double value, BeamPowerMap& map ) {
  if ( value == 1.0 ) return;
  for ( BPMIter it = map.begin(); it != map.end(); ++it ) {
    it->second *= value;
  }
}


double Transducer::getValue( double key, const ::std::map< PDouble, double >& map, long double precision, bool use_linear, double lin_costant ) const {
  assert( !map.empty() );
  
  if (debug) ::std::cout << "Transducer::getValue() key " << key << "; prec " << precision 
                         << "; map size " << map.size() << ::std::endl;    
  
  PDouble pd_key ( key, precision );
  double ret_val;
  
  if ( tvr_map.size() == 1 ) {
    ret_val = map.begin()->second;
    if ( use_linear ) ret_val = ::std::pow( 10.0, ret_val/lin_costant );
    
    if (debug) ::std::cout << "Transducer::getValue() map has one value " << ret_val << ::std::endl;    
  }
  else {
    ::std::map< PDouble, double >::const_iterator it = map.lower_bound( pd_key );
    ::std::map< PDouble, double >::const_iterator before_it;
    
    if ( it == map.end() ) {
      it = (++map.rbegin()).base();
      ret_val = it->second;
      if ( use_linear ) ret_val = ::std::pow( 10.0, ret_val/lin_costant );
      
      if (debug) ::std::cout << "Transducer::getValue() all map's values have key < " << key << "; ret val " << ret_val << ::std::endl;          
    }
    else if ( it->first == pd_key || it == map.begin() ) {
      ret_val = it->second;
      if ( use_linear ) ret_val = ::std::pow( 10.0, ret_val/lin_costant );

      if (debug) ::std::cout << "Transducer::getValue() key value found or map begin()  " << key << "; ret val " << ret_val << ::std::endl;                
    }
    else {
      before_it = it;
      before_it--;
      
      if ( use_linear ) 
        ret_val = ::std::pow( 10.0, before_it->second/lin_costant ) + ( ::std::pow( 10.0, it->second/lin_costant ) - ::std::pow( 10.0, before_it->second / lin_costant ) ) / ( (double)it->first - (double)before_it->first ) * ( key - (double)before_it->first );
      else 
        ret_val = before_it->second + ( it->second - before_it->second ) / ( (double)it->first - (double)before_it->first ) * ( key - (double)before_it->first );
      
      if (debug) ::std::cout << "Transducer::getValue() frequency value made from: prev key " << (double)before_it->first 
                             << "; prev value " <<  before_it->second << "; up key " << (double)it->first
                             << "; up value " << it->second << "; ret val " << ret_val << ::std::endl;                      
    }
  }  
  return ret_val;
}


double Transducer::getSPL( double frequency, double power ) const {
  if ( power > max_power ) power = max_power;

  double tvr = 20.0*log10( getValue( frequency, tvr_map, tvr_precision, true, 20.0 ) ); 
  double g = getValue( frequency, conductance_map, conductance_precision ) * 1.0e-6; // [uS]
  double spl = 10.0*log10(power) - 10.0*log10(g) + tvr;
  
  if (debug) ::std::cout << "Transducer::getSPL() freq " << frequency << "; power " << power
                         << "; tvr " << tvr << "; g " << g << "; spl = " << spl << ::std::endl;
  
  return( spl );
}


double Transducer::getPowerFromSPL( double frequency, double spl ) const {  
  double tvr = 20.0*log10( getValue( frequency, tvr_map, tvr_precision, true, 20.0 ) ); 
  double g = getValue( frequency, conductance_map, conductance_precision ) * 1.0e-6;
  
  double min_power = ::std::min( ::std::pow( 10.0, ( spl - tvr + 10.0*log10(g) ) / 10.0 ), max_power );
  
  if (debug) ::std::cout << "Transducer::getPowerFromSPL() freq " << frequency << "; spl " << spl
                         << "; tvr " << tvr << "; g " << g 
                         << "; power = " << ::std::pow( 10.0, ( spl - tvr + 10.0*log10(g) ) / 10.0 ) 
                         << "; min_power = " << min_power << ::std::endl;
  
  return( min_power );
}

    
bool Transducer::import( std::istream& stream_in ) {
  ::std::streamsize old_precision = stream_in.precision();
  stream_in.precision(TRANSDUCER_WRITE_PRECISION);
  
  ::std::string symmetry;
  
  stream_in >> type_name >> symmetry >> resonance_frequency >> bandwith_3db >> max_power >> duty_cycle
            >> tvr_precision >> ocv_precision >> conductance_precision >> beam_precision;
             
  if ( symmetry == conical_string ) {
    has_conical_symmetry = true;
  }
  else if ( symmetry == toroidal_string ) {
    has_conical_symmetry = false;
  }
  else {
    ::std::cerr << "Transducer::import() unkown symmetry pattern!" << ::std::endl;
    exit(1);
  }
  
  
  if (debug) ::std::cout << "Transducer::import() type name " << type_name << "; symmetry " << getSymmetryString()
                         << "; res freq " << resonance_frequency
                         << "; bandwith " << bandwith_3db << "; max pow " << max_power << "; duty cycle " << duty_cycle
                         << "; tvr prec " << tvr_precision << "; ocv prec " << ocv_precision 
                         << "; cond prec " << conductance_precision << "; beam prec " << beam_precision << ::std::endl;
              
   
  bool is_ok = import( stream_in, tvr_map, tvr_precision );
  assert( is_ok );
  is_ok = import( stream_in, ocv_map, ocv_precision );
  assert( is_ok );  
  is_ok = import( stream_in, conductance_map, conductance_precision );
  assert( is_ok );
  
//   stream_in >> axis_of_symmetry_angle;
  
  is_ok = import( stream_in, beam_power_map, beam_precision, true );
  assert( is_ok );
  
  stream_in.precision(old_precision);  
  
  return true;
}


bool Transducer::importBinary( std::fstream& file_in ) {
//   if ( !file_in.is_open() ) return false;
  assert( file_in.is_open() );
  
  int type_name_size;
  int type_symmetry_size;
  
  file_in.read( reinterpret_cast< char* > (&type_name_size), sizeof(int) );
  char* type_name_char = new char[type_name_size];
  
  file_in.read( type_name_char, type_name_size*sizeof(char) );
  type_name = ::std::string( type_name_char );
 
  file_in.read( reinterpret_cast< char* > (&type_symmetry_size), sizeof(int) );
  char* symmetry_char = new char[type_symmetry_size];
  
  file_in.read( symmetry_char, type_symmetry_size*sizeof(char) );
  ::std::string symmetry ( type_name_char );
  
  
  if ( symmetry == conical_string ) {
    has_conical_symmetry = true;
  }
  else if ( symmetry == toroidal_string ) {
    has_conical_symmetry = false;
  }
  else {
    ::std::cerr << "Transducer::importBinary() unkown symmetry pattern!" << ::std::endl;
    exit(1);
  }  
  
  file_in.read( reinterpret_cast< char* > (&resonance_frequency), sizeof(double) );
  file_in.read( reinterpret_cast< char* > (&bandwith_3db), sizeof(double) );
  file_in.read( reinterpret_cast< char* > (&max_power), sizeof(double) );
  file_in.read( reinterpret_cast< char* > (&duty_cycle), sizeof(double) );

  file_in.read( reinterpret_cast< char* > (&tvr_precision), sizeof(long double) );
  file_in.read( reinterpret_cast< char* > (&ocv_precision), sizeof(long double) );
  file_in.read( reinterpret_cast< char* > (&conductance_precision), sizeof(long double) );
  file_in.read( reinterpret_cast< char* > (&beam_precision), sizeof(long double) );  
  
  if ( debug )::std::cout << "Transducer::importBinary() type name " << type_name << "; symmetry " << getSymmetryString()
                          << "; res freq " << resonance_frequency
                          << "; bandwith " << bandwith_3db << "; max pow " << max_power << "; duty cycle " << duty_cycle
                          << "; tvr prec " << tvr_precision << "; ocv prec " << ocv_precision 
                          << "; cond prec " << conductance_precision << "; beam prec " << beam_precision << ::std::endl;  
  
  bool is_ok = importBinary( file_in, tvr_map, tvr_precision );
  assert( is_ok );
  is_ok = importBinary( file_in, ocv_map, ocv_precision );
  assert( is_ok );  
  is_ok = importBinary( file_in, conductance_map, conductance_precision );
  assert( is_ok );
  
//   file_in.read( reinterpret_cast< char* > (&axis_of_symmetry_angle ), sizeof(double) );  
  
  is_ok = importBinary( file_in, beam_power_map, beam_precision, true );
  assert( is_ok );
  
  return true;
}


bool Transducer::write( ::std::ostream& stream_out ) const { 
  ::std::streamsize old_precision = stream_out.precision();
  stream_out.precision(TRANSDUCER_WRITE_PRECISION);
   
  stream_out << ::std::setw(TRANSDUCER_WRITE_MARGIN) << type_name 
             << ::std::setw(TRANSDUCER_WRITE_MARGIN) << getSymmetryString() << ::std::endl;
  
  stream_out << ::std::setw(TRANSDUCER_WRITE_MARGIN) << resonance_frequency 
             << ::std::setw(TRANSDUCER_WRITE_MARGIN) << bandwith_3db << ::std::endl;
            
  stream_out << ::std::setw(TRANSDUCER_WRITE_MARGIN) << max_power 
             << ::std::setw(TRANSDUCER_WRITE_MARGIN) << duty_cycle << ::std::endl;
             
  stream_out << ::std::setw(TRANSDUCER_WRITE_MARGIN) << tvr_precision 
             << ::std::setw(TRANSDUCER_WRITE_MARGIN) << ocv_precision
             << ::std::setw(TRANSDUCER_WRITE_MARGIN) << conductance_precision
             << ::std::setw(TRANSDUCER_WRITE_MARGIN) << beam_precision << ::std::endl;
  
  bool is_ok = write( stream_out, tvr_map );
  assert( is_ok );
  is_ok = write( stream_out, ocv_map );
  assert( is_ok );
  is_ok = write( stream_out, conductance_map );
  assert( is_ok );
  
//   stream_out << ::std::setw(TRANSDUCER_WRITE_MARGIN) << axis_of_symmetry_angle << ::std::endl; 
  
  is_ok = write( stream_out, beam_power_map );
  assert( is_ok );
  
  stream_out.precision(old_precision);  
  return true;
}


bool Transducer::writeBinary( ::std::fstream& file_out ) const {   
//   if ( !file_out.is_open() ) return false;
  assert( file_out.is_open() );
  
  int type_name_size = type_name.size() + 1;
  const char* type_name_char = type_name.c_str();
  
  int symmetry_size = getSymmetryString().size() + 1;
  const char* symmetry_char = getSymmetryString().c_str();
  
  
  file_out.write( reinterpret_cast< char* > (&type_name_size), sizeof(int) );
  file_out.write( type_name_char, type_name_size*sizeof(char) );

  file_out.write( reinterpret_cast< char* > (&symmetry_size), sizeof(int) );
  file_out.write( symmetry_char, symmetry_size*sizeof(char) );  
  
  file_out.write( (char*) (&resonance_frequency), sizeof(double) );
  file_out.write( (char*) (&bandwith_3db), sizeof(double) );
  file_out.write( (char*) (&max_power), sizeof(double) );
  file_out.write( (char*) (&duty_cycle), sizeof(double) );

  file_out.write( (char*) (&tvr_precision), sizeof(long double) );
  file_out.write( (char*) (&ocv_precision), sizeof(long double) );
  file_out.write( (char*) (&conductance_precision), sizeof(long double) );
  file_out.write( (char*) (&beam_precision), sizeof(long double) );
  
  bool is_ok = writeBinary( file_out, tvr_map );
  assert( is_ok );
  is_ok = writeBinary( file_out, ocv_map );
  assert( is_ok );
  is_ok = writeBinary( file_out, conductance_map );
  assert( is_ok );

//   file_out.write( (char*) (&axis_of_symmetry_angle), sizeof(double) );
  
  is_ok = writeBinary( file_out, beam_power_map );
  assert( is_ok );
  
  return true;
}


bool Transducer::import( ::std::istream& stream_in, ::std::map< PDouble, double >& map, long double prec, bool is_angle ) { 
  using namespace std;
  
//   if ( !stream_in.good() ) return false;
  assert( stream_in.good() );

  int total_values = TRANSDUCER_NOT_SET;
  double curr_key = TRANSDUCER_NOT_SET;
  double curr_value = TRANSDUCER_NOT_SET;
  int value_counter = 0;
  
  stream_in >> total_values;
  assert(total_values > 0);
  
  while ( value_counter != total_values ) {

    stream_in >> curr_key >> curr_value;
    
    assert( curr_key != TRANSDUCER_NOT_SET );
    assert( curr_value != TRANSDUCER_NOT_SET );
    if ( is_angle ) curr_key = normalizeAngle(curr_key);
     
    map[PDouble(curr_key, prec)] = curr_value;
    
    ++value_counter;
    
    if ( debug ) ::std::cout << "Transducer::import() key = " << curr_key << "; value = " << curr_value 
                             << "; cnt = " << value_counter << "; total values = " << total_values << ::std::endl; 
  }
  
  assert( value_counter == total_values );
  
  return true;
}


bool Transducer::importBinary( ::std::fstream& file_in, ::std::map< PDouble, double >& map, long double prec, bool is_angle ) { 
  using namespace std;
  
//   if ( !file_in.good() ) return false;
  assert( file_in.good() );
  
  int total_values = TRANSDUCER_NOT_SET;
  double curr_key = TRANSDUCER_NOT_SET;
  double curr_value = TRANSDUCER_NOT_SET;
  int value_counter = 0;

  file_in.read( reinterpret_cast< char* > (&total_values), sizeof(int) );
  assert(total_values > 0);
  
  while ( value_counter != total_values ) {

    file_in.read( reinterpret_cast< char* > (&curr_key), sizeof(double) );
    file_in.read( reinterpret_cast< char* > (&curr_value), sizeof(double) );
    
    assert( curr_key != TRANSDUCER_NOT_SET );
    assert( curr_value != TRANSDUCER_NOT_SET );
    if ( is_angle ) curr_key = normalizeAngle(curr_key);
     
    map[PDouble(curr_key, prec)] = curr_value;
    
    ++value_counter;
    
    if (debug) ::std::cout << "Transducer::importBinary() key = " << curr_key << "; value = " << curr_value 
                           << "; cnt = " << value_counter << "; total values = " << total_values << ::std::endl; 
  }
  
  assert( value_counter == total_values );
  
  return true;
}


bool Transducer::write( ::std::ostream& stream_out, const ::std::map< PDouble, double >& map ) const { 
  using namespace std;
  
  stream_out << setw(TRANSDUCER_WRITE_MARGIN) << map.size() << endl;
             
  for ( ::std::map< PDouble, double >::const_iterator it = map.begin(); it != map.end(); ++it ) {
    stream_out << setw(TRANSDUCER_WRITE_MARGIN) << it->first 
               << setw(TRANSDUCER_WRITE_MARGIN) << it->second << endl;
  }
  
  return true;
}


bool Transducer::writeBinary( ::std::fstream& file_out, const ::std::map< PDouble, double >& map ) const {
  int size = map.size();
  
  file_out.write( reinterpret_cast< char* > (&size), sizeof(int) );
  
  for ( ::std::map< PDouble, double >::const_iterator it = map.begin(); it != map.end(); ++it ) {
    double key = it->first.getValue();
    double value = it->second;
    file_out.write( reinterpret_cast< char* > (&key), sizeof(double) );
    file_out.write( reinterpret_cast< char* > (&value), sizeof(double) );
  }
  
  return true;
}


bool Transducer::writeSPL( ::std::ostream& stream_out, double freq_step, double power ) const {
  ::std::streamsize old_precision = stream_out.precision();
  stream_out.precision(TRANSDUCER_WRITE_PRECISION);

  if ( power > max_power ) power = max_power;
  double min_freq = ::std::min( conductance_map.begin()->first.getValue(), tvr_map.begin()->first.getValue() );
  double max_freq = ::std::max( conductance_map.rbegin()->first.getValue(), tvr_map.rbegin()->first.getValue() );
  
  for( int i = 0; i <= floor( (max_freq - min_freq) / freq_step ); i++ ) {
    double curr_freq = min_freq + ((double)i) * freq_step;
    double spl = getSPL( curr_freq, power );
    
    stream_out << ::std::setw(TRANSDUCER_WRITE_MARGIN) << curr_freq 
               << ::std::setw(TRANSDUCER_WRITE_MARGIN) << spl << ::std::endl;
  }  
  stream_out.precision(old_precision);
  return true;
}


bool Transducer::writeVertBeamPattern( ::std::ostream& stream_out, const CoordZ& tx, const CoordZ& rx, double init_bearing, double vert_rot, double horiz_rot, double mult_costant, double add_costant ) const {

  ::std::streamsize old_prec;
  
  if (debug) {
    old_prec = ::std::cout.precision(TRANSDUCER_WRITE_PRECISION);
  
    ::std::cout << "Transducer::writeVertBeamPattern() tx " << tx << "; rx " << rx << "; init bearing " << init_bearing 
                << "; vert_rot " << vert_rot << "; horiz_rot " << horiz_rot << "; mult_costant " << mult_costant 
                << "; add_costant " << add_costant << ::std::endl
                << "; tx x " << tx.getCartX() << "; y " << tx.getCartY() 
                << "; z "<< tx.getCartZ() << ::std::endl
                << "; rx x " << rx.getCartX() << "; y " << rx.getCartY() 
                << "; z "<< rx.getCartZ() << ::std::endl;
  }
  
  ::std::streamsize old_precision = stream_out.precision();
  stream_out.precision(TRANSDUCER_WRITE_PRECISION);
  ::std::cout.precision(TRANSDUCER_WRITE_PRECISION);

  BeamPowerMap beam_write_map;
 
  const_cast< Transducer* > (this)->beampattern_multiply( mult_costant, beam_write_map );  
  const_cast< Transducer* > (this)->beampattern_sum( add_costant, beam_write_map );
  
  double rx_x_primo = rx.getCartX() - tx.getCartX();
  double rx_y_primo = rx.getCartY() - tx.getCartY();
  double rx_z_primo = rx.getCartZ() - tx.getCartZ();
    
  double rx_rho_primo, rx_theta_primo, rx_phi_primo; // tx_bw_rho_primo, tx_bw_theta_primo, tx_bw_phi_primo;
  convCart2Spher( rx_x_primo, rx_y_primo, rx_z_primo, rx_rho_primo, rx_theta_primo, rx_phi_primo );

  if (debug) ::std::cout << "Transducer::writeVertBeamPattern() rx_x_primo " << rx_x_primo << "; rx_y_primo " << rx_y_primo 
                         << "; rx_z_primo " << rx_z_primo << "; rx_rho_primo " << rx_rho_primo << "; rx_theta_primo " << rx_theta_primo*180.0/M_PI 
                         << "; rx_phi_primo " << rx_phi_primo*180.0/M_PI << ::std::endl;
  
   // ruoto di theta_tx e ottengo rx_secondo
  rx_theta_primo -= (tx.getSphericalTheta()*M_PI/180.0);

  double rx_x_secondo, rx_y_secondo, rx_z_secondo;
  convSpher2Cart( rx_rho_primo, rx_theta_primo, rx_phi_primo, rx_x_secondo, rx_y_secondo, rx_z_secondo ); 

   if (debug) ::std::cout << "Transducer::writeVertBeamPattern() rx_x_secondo " << rx_x_secondo << "; rx_y_secondo " << rx_y_secondo 
                         << "; rx_z_secondo " << rx_z_secondo << ::std::endl; 
  
  double alpha = -tx.getSphericalPhi()*M_PI/180.0 - M_PI / 2.0;
  
  // calcolo rx_terzo
  
  double rot_angle = M_PI/2.0 - alpha - init_bearing + horiz_rot*M_PI/180.0 ;
  
  if (debug) ::std::cout << "Transducer::writeVertBeamPattern() alpha " << alpha*180.0/M_PI 
                         << "; rot_angle " << rot_angle*180.0/M_PI << ::std::endl;    
  
  double rx_x_terzo, rx_y_terzo, rx_z_terzo;
  rotCartXY( rot_angle, rx_x_secondo, rx_y_secondo, rx_x_terzo, rx_y_terzo );
  rx_z_terzo = rx_z_secondo;

  double tx_bw_x_terzo = 0.0;
  double tx_bw_y_terzo = 0.0;
  double tx_bw_z_terzo = -20.0;
  
  if (debug) ::std::cout << "Transducer::writeVertBeamPattern() rx_x_terzo " << rx_x_terzo << "; rx_y_terzo " << rx_y_terzo 
                         << "; rx_z_terzo " << rx_z_terzo 
                         << "; tx_bw_x_terzo "<< tx_bw_x_terzo << "; tx_bw_y_terzo " << tx_bw_y_terzo 
                         << "; tx_bw_z_terzo " << tx_bw_z_terzo << ::std::endl;              
              
  // calcolo rx_quarto 
  double rot_angle2 = -vert_rot*M_PI/180.0;
  
  double rx_x_quarto, rx_y_quarto, rx_z_quarto, tx_bw_x_quarto, tx_bw_y_quarto, tx_bw_z_quarto;
  rotCartXZ( rot_angle2, rx_x_terzo, rx_z_terzo, rx_x_quarto, rx_z_quarto );
  rotCartXZ( rot_angle2, tx_bw_x_terzo, tx_bw_z_terzo, tx_bw_x_quarto, tx_bw_z_quarto );
  rx_y_quarto = rx_y_terzo;
  tx_bw_y_quarto = tx_bw_y_terzo;

  if (debug) ::std::cout << "Transducer::writeVertBeamPattern() rot_angle2 " << rot_angle2*180.0/M_PI << "; rx_x_quarto " << rx_x_quarto 
                         << "; rx_y_quarto " << rx_y_quarto << "; rx_z_quarto " << rx_z_quarto  
                         << "; tx_bw_x_quarto "<< tx_bw_x_quarto << "; tx_bw_y_quarto " << tx_bw_y_quarto 
                         << "; tx_bw_z_quarto " << tx_bw_z_quarto << ::std::endl;    
  
  // calcolo rx_quinto        
  double rx_x_quinto, rx_y_quinto, rx_z_quinto, tx_bw_x_quinto, tx_bw_y_quinto, tx_bw_z_quinto;
  
  rx_x_quinto = -rx_z_quarto;
  rx_y_quinto = rx_y_quarto;
  rx_z_quinto = rx_x_quarto;
  tx_bw_x_quinto = -tx_bw_z_quarto;
  tx_bw_y_quinto = tx_bw_y_quarto;
  tx_bw_z_quinto = tx_bw_x_quarto;
  
  if (debug) ::std::cout << "Transducer::writeVertBeamPattern() rx_x_quinto " << rx_x_quinto 
                         << "; rx_y_quinto " << rx_y_quarto << "; rx_z_quinto " << rx_z_quinto 
                         << "; tx_bw_x_quinto "<< tx_bw_x_quinto << "; tx_bw_y_quinto " << tx_bw_y_quinto 
                         << "; tx_bw_z_quinto " << tx_bw_z_quinto << ::std::endl;    
              
  double nv_x, nv_y, nv_z;
  
  crossProd( rx_x_quinto, rx_y_quinto, rx_z_quinto, tx_bw_x_quinto, tx_bw_y_quinto, tx_bw_z_quinto, nv_x, nv_y, nv_z );
  
  double norm_nv = normCart( nv_x, nv_y, nv_z );
  nv_x = nv_x / norm_nv;
  nv_y = nv_y / norm_nv;
  nv_z = nv_z / norm_nv;
  
  if (debug) ::std::cout << "Transducer::writeVertBeamPattern() nv_x " 
                         << nv_x << "; nv_y " << nv_y << "; nv_z " << nv_z << ::std::endl; 
   
  
  double phi_step = 1.0/200.0 * M_PI;
  double theta_step =  1.0/200.0 * M_PI;
  double scalar_thresh = 1e-3;
//   if ( has_conical_symmetry ) scalar_thresh = 1e-3;
//   else scalar_thresh = 1e-2;
  
  typedef ::std::map< double, ::std::pair< double, double> > TriMap;
  typedef TriMap::reverse_iterator TMRIter;
  typedef TriMap::iterator TMIter;

  TMRIter min_iter;
  TMRIter max_iter;
  double curr_min_theta = 2*M_PI;
  double curr_max_theta = -2*M_PI;
  
  TriMap tri_map;
  
  double curr_gain = 0;
    
  for ( double curr_theta = 0; curr_theta <= M_PI; curr_theta += theta_step ) {

    if ( has_conical_symmetry ) curr_gain = getValue( curr_theta*180.0/M_PI, beam_power_map, beam_precision, true, 10.0 );
    else curr_gain = getValue( (M_PI/2.0 - curr_theta)*180.0/M_PI, beam_power_map, beam_precision, true, 10.0 );

    for ( double curr_phi = -M_PI; curr_phi <= M_PI; curr_phi += phi_step ) {
      
      double curr_x, curr_y, curr_z;
      convSpher2Cart( curr_gain, curr_theta, curr_phi, curr_x, curr_y, curr_z );
 
      double curr_scal = scalarProd( nv_x, nv_y, nv_z, curr_x, curr_y, curr_z ) / normCart( curr_x, curr_y, curr_z );

      if (debug) ::std::cout << "Transducer::writeVertBeamPattern() curr_theta " << curr_theta*180.0/M_PI 
                             << "; curr min theta " << curr_min_theta*180.0/M_PI << "; curr_max_theta " << curr_max_theta*180.0/M_PI 
                             << "; curr_phi " << curr_phi*180.0/M_PI << "; curr_gain " << 10.0*log10( curr_gain ) 
                             << "; curr_x " << curr_x << "; curr_y " << curr_y << "; curr_z " << curr_z 
                             << "; curr_scal " << curr_scal << ::std::endl;
                                                         
               
      if ( ::std::abs(curr_scal) <= scalar_thresh ) {
        TMIter curr_iter = tri_map.insert( ::std::make_pair( curr_phi, ::std::make_pair( curr_theta, curr_gain ) ) ).first;

        if (debug) ::std::cout << "Transducer::writeVertBeamPattern() inserting curr_theta " << curr_theta*180.0/M_PI 
                               << "; curr_phi " << curr_phi*180.0/M_PI << "; curr_gain " << 10.0*log10( curr_gain ) 
                               << "; curr_scal " << curr_scal << ::std::endl;  
                      
              
        if ( curr_theta > curr_max_theta ) {
          curr_max_theta = curr_theta;
          max_iter = TMRIter(curr_iter);
        }
        if ( curr_theta < curr_min_theta ) {
          curr_min_theta = curr_theta;
          min_iter = TMRIter(curr_iter);          
        }
        
      }
    }
    
  }
   
  if (debug) ::std::cout << "Transducer::writeVertBeamPattern() trimap size " << tri_map.size() << ::std::endl;

  for ( TMIter it = tri_map.begin(); it != tri_map.end(); it++ ) {
    
    if (debug) ::std::cout << "Transducer::writeVertBeamPattern() phi " << it->first*180.0/M_PI 
                           << "; theta " << it->second.first*180.0/M_PI << "; gain db " << 10.0*log10( it->second.second ) << ::std::endl;
    
  }
  
  bool has_to_change_theta = false;
  for ( TMRIter rit = min_iter; rit != tri_map.rend(); rit++ ) {
    
      if ( rit->second.first == curr_max_theta ) has_to_change_theta = true;
    
      double theta = (rit->second.first - curr_min_theta) / ( curr_max_theta - curr_min_theta ) * M_PI;
      
      if ( has_to_change_theta ) {
        theta *= -1.0;
      }
 
    if (debug) ::std::cout << "Transducer::writeVertBeamPattern() change theta " << has_to_change_theta 
                           << "; theta " << theta*180.0/M_PI << "; gain db " << 10.0*log10( rit->second.second ) << ::std::endl;
      
    beam_write_map[theta*180.0/M_PI] = 10.0*log10( rit->second.second );
  }
  
  if ( min_iter != tri_map.rbegin() ) {
    
    for ( TMRIter rit = tri_map.rbegin(); rit != min_iter; rit++ ) {
      
      if ( rit->second.first == curr_max_theta ) has_to_change_theta = true;
    
      double theta = (rit->second.first - curr_min_theta) / ( curr_max_theta - curr_min_theta ) * M_PI;
      
      if ( has_to_change_theta ) {
        theta *= -1.0;
      }    
    
      if (debug) ::std::cout << "Transducer::writeVertBeamPattern() change theta " << has_to_change_theta 
                             << "; theta " << theta*180.0/M_PI << "; gain db " << 10.0*log10( rit->second.second ) << ::std::endl;
                
      beam_write_map[theta*180.0/M_PI] = 10.0*log10( rit->second.second );   
    }    
    
  }

 
  if (has_conical_symmetry) const_cast< Transducer* > (this)->beampattern_rotate( vert_rot, beam_write_map );  
  else const_cast< Transducer* > (this)->beampattern_rotate( vert_rot - 90.0, beam_write_map );
  
  if (debug) {
    ::std::cout << "Final Vertical Beam Pattern " << ::std::endl;
    write( ::std::cout, beam_write_map );
    ::std::cout.precision(old_prec);
  }
  
  bool ok = write( stream_out, beam_write_map );

  stream_out.precision(old_precision);
  return ok;
}


double Transducer::normalizeAngle( double angle ) const {
  double ret_val = ::std::fmod( angle, 360.0 );
  if ( ret_val > 180.0 ) ret_val -= 360.0;
  if ( ret_val < -180.0 ) ret_val += 360.0;
  return ret_val;
}

