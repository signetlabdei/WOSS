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
 * @file   ssp-definitions.cpp
 * @author Federico Guerra
 *
 * \brief  Implementation of woss::SSP (Sound Speed Profile) class 
 *
 * Implementation of woss::SSP (Sound Speed Profile) class 
 */


#define SSP_WRITE_PRECISION ( 17 )
#define SSP_WRITE_MARGIN ( SSP_WRITE_PRECISION + 4 )


#include <iterator>
#include <cassert>
#include <iomanip>
#include <set>
#include <sstream>
#include "ssp-definitions.h" 


using namespace woss;


bool SSP::debug = false;

    
SSP::SSP( long double prec ) 
: ssp_eq_type(SSP_EQ_TEOS_10_EXACT),
  min_ssp_value(HUGE_VAL),
  max_ssp_value(0.0),
  depth_precision(prec),
  ssp_map(),
  pressure_map(),
  salinity_map(),
  temperature_map()
{

}


SSP::SSP( const SSP& copy ) {
  ssp_eq_type = copy.ssp_eq_type;
  min_ssp_value = copy.min_ssp_value;
  max_ssp_value = copy.max_ssp_value;
  depth_precision = copy.depth_precision;
  
  ssp_map = copy.ssp_map;
  pressure_map = copy.pressure_map;
  salinity_map = copy.salinity_map;
  temperature_map = copy.temperature_map;
}


SSP::SSP( DepthMap& ssp_m, DepthMap& temp_map, DepthMap& sal_map, DepthMap& press_map, long double depth_p )
  : ssp_eq_type(SSP_EQ_TEOS_10_EXACT),
    min_ssp_value(HUGE_VAL),
    max_ssp_value(0.0),
    depth_precision(depth_p)
{
  ssp_map.swap(ssp_m);
  temperature_map.swap(temp_map);
  salinity_map.swap(sal_map);
  pressure_map.swap(press_map);
}


SSP::SSP( DepthMap& ssp_m, long double depth_p ) 
  : ssp_eq_type(SSP_EQ_TEOS_10_EXACT),
    min_ssp_value(HUGE_VAL),
    max_ssp_value(0.0),
    depth_precision(depth_p)
{
  ssp_map.swap(ssp_m);

}

  
DConstIter SSP::at( const int position ) const {
  if ( position >= (int) ssp_map.size() || position < 0 ) return ssp_map.end();
  if ( position == 0 ) return ssp_map.begin();

  DConstIter ret_val = ssp_map.begin();
  ::std::advance( ret_val, position );
  return ret_val;
}


SSP& SSP::insertValue( double depth, double ssp_value ) {
  assert( ssp_value > 0 && depth >= 0);

  if (ssp_value > max_ssp_value) max_ssp_value = ssp_value;
  if (ssp_value < min_ssp_value) min_ssp_value = ssp_value;

  ssp_map.insert( std::make_pair( PDouble( depth, depth_precision ), ssp_value ) );
  
  return *this;
}


SSP& SSP::insertValue( double depth, double temperature, double salinity, const Coord& coordinates ) {
  assert( temperature > -20.0 && temperature < 50.0 );
  assert( salinity >= 0.0 && salinity <= 60.0 );
  assert( coordinates.isValid() );
  
  double pressure = getPressureFromDepth( coordinates, depth );
  double curr_ssp = calculateSSP( temperature, salinity, pressure );

  if (curr_ssp > max_ssp_value) max_ssp_value = curr_ssp;
  if (curr_ssp < min_ssp_value) min_ssp_value = curr_ssp;

  PDouble curr_depth = PDouble( depth , depth_precision ) ;

  ssp_map.insert( std::make_pair( curr_depth, curr_ssp ) );
  pressure_map.insert( std::make_pair( curr_depth, pressure ) );
  temperature_map.insert( std::make_pair( curr_depth, temperature ) );
  salinity_map.insert( std::make_pair( curr_depth, salinity ) );
  
  return *this;
}

  
SSP& SSP::insertValue( double depth, double temperature, double salinity, const std::complex<double>& pressure, double ssp_value ) {
  assert( temperature > -20.0 && temperature < 50.0 );
  assert( salinity >= 0.0 && salinity <= 60.0 );
  assert( pressure.real() >= 0 );
  assert( ssp_value > 0 && depth >= 0);

  PDouble curr_depth = PDouble( depth, depth_precision );

  if (ssp_value > max_ssp_value) max_ssp_value = ssp_value;
  if (ssp_value < min_ssp_value) min_ssp_value = ssp_value;

  ssp_map.insert( std::make_pair( curr_depth, ssp_value ) );
  pressure_map.insert( std::make_pair( curr_depth, pressure.real() ) );
  temperature_map.insert( std::make_pair( curr_depth, temperature ) );
  salinity_map.insert( std::make_pair( curr_depth, salinity ) );

  return *this;
}


SSP& SSP::insertValue( double temperature, double salinity, const std::complex<double>& pressure, const Coord& coordinates ) {
  assert( temperature > -20.0 && temperature < 50.0 );
  assert( salinity >= 0.0 && salinity <= 60.0 );
  assert( pressure.real() >= 0 );
  assert( coordinates.isValid() );

  double curr_ssp = calculateSSP( temperature, salinity, pressure.real() );
  if (curr_ssp > max_ssp_value) max_ssp_value = curr_ssp;
  if (curr_ssp < min_ssp_value) min_ssp_value = curr_ssp;

  PDouble curr_depth( getDepthfromPressure( coordinates, pressure.real() ) , depth_precision );
  
  ssp_map.insert( std::make_pair( curr_depth, curr_ssp ) );
  pressure_map.insert( std::make_pair( curr_depth, pressure.real() ) );
  temperature_map.insert( std::make_pair( curr_depth, temperature ) );
  salinity_map.insert( std::make_pair( curr_depth, salinity ) );
  
  return *this;
}


SSP& SSP::operator=( const SSP& copy )  {
  if (this == &copy) return *this;
  max_ssp_value = copy.max_ssp_value;
  min_ssp_value = copy.min_ssp_value;

  depth_precision = copy.depth_precision;

  ssp_map = copy.ssp_map;
  pressure_map = copy.pressure_map;
  salinity_map = copy.salinity_map;
  temperature_map = copy.temperature_map;
  return( *this );
}


SSP& woss::operator+=( SSP& left, const SSP& right ) {
  DConstIter tempit = right.temperature_map.begin();
  DConstIter pressit = right.pressure_map.begin();
  DConstIter salit = right.salinity_map.begin();

  for ( DConstIter it = right.ssp_map.begin(); it != right.ssp_map.end(); it++ ) {
    DIter it2 = left.ssp_map.find(it->first);
    if ( it2 != left.ssp_map.end() ) it2->second += it->second; 
    else left.ssp_map[it->first] += it->second;

    if ( tempit != right.temperature_map.end() ) {
      it2 = left.temperature_map.find(tempit->first);
      if ( it2 != left.temperature_map.end() ) it2->second += tempit->second; 
      else left.temperature_map[tempit->first] += tempit->second;
      tempit++;
    }
    if ( pressit != right.pressure_map.end() ) {
      it2 = left.pressure_map.find(pressit->first);
      if ( it2 != left.pressure_map.end() ) it2->second += pressit->second; 
      else left.pressure_map[pressit->first] += pressit->second;
      pressit++;
    }
    if ( salit != right.salinity_map.end() ) {
      it2 = left.salinity_map.find(salit->first);
      if ( it2 != left.salinity_map.end() ) it2->second += salit->second; 
      else left.salinity_map[salit->first] += salit->second;
      salit++;
    }
  }
  return left;
}


SSP& woss::operator-=( SSP& left, const SSP& right ) {
  DConstIter tempit = right.temperature_map.begin();
  DConstIter pressit = right.pressure_map.begin();
  DConstIter salit = right.salinity_map.begin();

  for ( DConstIter it = right.ssp_map.begin(); it != right.ssp_map.end(); it++ ) {
    DIter it2 = left.ssp_map.find(it->first);
    if ( it2 != left.ssp_map.end() ) it2->second -= it->second; 
    else left.ssp_map[it->first] -= it->second;

    if ( tempit != right.temperature_map.end() ) {
      it2 = left.temperature_map.find(tempit->first);
      if ( it2 != left.temperature_map.end() ) it2->second -= tempit->second; 
      else left.temperature_map[tempit->first] -= tempit->second;
      tempit++;
    }
    if ( pressit != right.pressure_map.end() ) {
      it2 = left.pressure_map.find(pressit->first);
      if ( it2 != left.pressure_map.end() ) it2->second -= pressit->second; 
      else left.pressure_map[pressit->first] -= pressit->second;
      pressit++;
    }
    if ( salit != right.salinity_map.end() ) {
      it2 = left.salinity_map.find(salit->first);
      if ( it2 != left.salinity_map.end() ) it2->second -= salit->second; 
      else left.salinity_map[salit->first] -= salit->second;
      salit++;
    }
  }
  return left;
}


SSP& woss::operator*=( SSP& left, const SSP& right ) {
  DConstIter tempit = right.temperature_map.begin();
  DConstIter pressit = right.pressure_map.begin();
  DConstIter salit = right.salinity_map.begin();

  for ( DConstIter it = right.ssp_map.begin(); it != right.ssp_map.end(); it++ ) {
    DIter it2 = left.ssp_map.find(it->first);
    if ( it2 != left.ssp_map.end() ) it2->second *= it->second; 
    else left.ssp_map[it->first] *= it->second;

    if ( tempit != right.temperature_map.end() ) {
      it2 = left.temperature_map.find(tempit->first);
      if ( it2 != left.temperature_map.end() ) it2->second *= tempit->second; 
      else left.temperature_map[tempit->first] *= tempit->second;
      tempit++;
    }
    if ( pressit != right.pressure_map.end() ) {
      it2 = left.pressure_map.find(pressit->first);
      if ( it2 != left.pressure_map.end() ) it2->second *= pressit->second; 
      else left.pressure_map[pressit->first] *= pressit->second;
      pressit++;
    }
    if ( salit != right.salinity_map.end() ) {
      it2 = left.salinity_map.find(salit->first);
      if ( it2 != left.salinity_map.end() ) it2->second *= salit->second; 
      else left.salinity_map[salit->first] *= salit->second;
      salit++;
    }
  }
  return left;
}


SSP& woss::operator/=( SSP& left, const SSP& right ) {
  DConstIter tempit = right.temperature_map.begin();
  DConstIter pressit = right.pressure_map.begin();
  DConstIter salit = right.salinity_map.begin();

  for ( DConstIter it = right.ssp_map.begin(); it != right.ssp_map.end(); it++ ) {
    DIter it2 = left.ssp_map.find(it->first);
    if ( it2 != left.ssp_map.end() ) it2->second /= it->second; 
    else left.ssp_map[it->first] /= it->second;

    if ( tempit != right.temperature_map.end() ) {
      it2 = left.temperature_map.find(tempit->first);
      if ( it2 != left.temperature_map.end() ) it2->second /= tempit->second; 
      else left.temperature_map[tempit->first] /= tempit->second;
      tempit++;
    }
    if ( pressit != right.pressure_map.end() ) {
      it2 = left.pressure_map.find(pressit->first);
      if ( it2 != left.pressure_map.end() ) it2->second /= pressit->second; 
      else left.pressure_map[pressit->first] /= pressit->second;
      pressit++;
    }
    if ( salit != right.salinity_map.end() ) {
      it2 = left.salinity_map.find(salit->first);
      if ( it2 != left.salinity_map.end() ) it2->second /= salit->second; 
      else left.salinity_map[salit->first] /= salit->second;
      salit++;
    }
  }
  return left;
}


SSP& woss::operator+=( SSP& left, const double right ) {
  DConstIter tempit = left.temperature_map.begin();
  DConstIter pressit = left.pressure_map.begin();
  DConstIter salit = left.salinity_map.begin();

  for ( DConstIter it = left.ssp_map.begin(); it != left.ssp_map.end(); it++ ) {
    left.ssp_map[it->first] += right;

    if ( tempit != left.temperature_map.end() ) {
      left.temperature_map[tempit->first] += right;
      tempit++;
    }
    if ( pressit != left.pressure_map.end() ) {
      left.temperature_map[pressit->first] += right;
      pressit++;
    }
    if ( salit != left.salinity_map.end() ) {
      left.temperature_map[salit->first] += right;
      salit++;
    }
  }
  return left;
}


SSP& woss::operator-=( SSP& left, const double right ) {
  DConstIter tempit = left.temperature_map.begin();
  DConstIter pressit = left.pressure_map.begin();
  DConstIter salit = left.salinity_map.begin();

  for ( DConstIter it = left.ssp_map.begin(); it != left.ssp_map.end(); it++ ) {
    left.ssp_map[it->first] -= right;

    if ( tempit != left.temperature_map.end() ) {
      left.temperature_map[tempit->first] -= right;
      tempit++;
    }
    if ( pressit != left.pressure_map.end() ) {
      left.temperature_map[pressit->first] -= right;
      pressit++;
    }
    if ( salit != left.salinity_map.end() ) {
      left.temperature_map[salit->first] -= right;
      salit++;
    }
  }
  return left;
}


SSP& woss::operator/=( SSP& left, const double right ) {
  DConstIter tempit = left.temperature_map.begin();
  DConstIter pressit = left.pressure_map.begin();
  DConstIter salit = left.salinity_map.begin();

  for ( DConstIter it = left.ssp_map.begin(); it != left.ssp_map.end(); it++ ) {
    left.ssp_map[it->first] /= right;

    if ( tempit != left.temperature_map.end() ) {
      left.temperature_map[tempit->first] /= right;
      tempit++;
    }
    if ( pressit != left.pressure_map.end() ) {
      left.temperature_map[pressit->first] /= right;
      pressit++;
    }
    if ( salit != left.salinity_map.end() ) {
      left.temperature_map[salit->first] /= right;
      salit++;
    }
  }
  return left;
}


SSP& woss::operator*=( SSP& left, const double right ) {
  DConstIter tempit = left.temperature_map.begin();
  DConstIter pressit = left.pressure_map.begin();
  DConstIter salit = left.salinity_map.begin();

  for ( DConstIter it = left.ssp_map.begin(); it != left.ssp_map.end(); it++ ) {
    left.ssp_map[it->first] *= right;

    if ( tempit != left.temperature_map.end() ) {
      left.temperature_map[tempit->first] *= right;
      tempit++;
    }
    if ( pressit != left.pressure_map.end() ) {
      left.temperature_map[pressit->first] *= right;
      pressit++;
    }
    if ( salit != left.salinity_map.end() ) {
      left.temperature_map[salit->first] *= right;
      salit++;
    }
  }
  return left;
}


bool SSP::import( std::istream& stream_in ) {
  using namespace std;
  
  if ( !stream_in.good() ) return false;
  
  streamsize old_precision = stream_in.precision();
  stream_in.precision(SSP_WRITE_PRECISION);

  string curr_token;
  set< string > string_set;
  
  getline( stream_in, curr_token );
  
  cout << "first line " << curr_token << endl;
  
  stringstream ss( curr_token );

  while (ss >> curr_token) {
    cout << "token = " << curr_token << endl;
    string_set.insert(curr_token);
  }
  bool ssp_bool = false;
  bool temperature_bool = false;
  bool salinity_bool = false;
  bool pressure_bool = false;
  bool depth_bool = false;
  
  set< string >::iterator it;
  
  if ( ( it = string_set.find( "DEPTH_[m]" ) ) != string_set.end() ) depth_bool = true;
  if ( ( it = string_set.find( "SSP_[m/s]" ) ) != string_set.end() ) ssp_bool = true;
  if ( ( it = string_set.find( "TEMPERATURE_[C°]" ) ) != string_set.end() ) temperature_bool = true;
  if ( ( it = string_set.find( "SALINITY_[ppu]" ) ) != string_set.end() ) salinity_bool = true;
  if ( ( it = string_set.find( "PRESSURE_[bar]" ) ) != string_set.end() ) pressure_bool = true;

  if ( depth_bool == false && pressure_bool == false ) return false;
  if ( ssp_bool == false && ( temperature_bool == false || salinity_bool == false ) ) return false;
  
  double curr_depth = -1;
  double curr_ssp = -1;
  double curr_temperature = -1;
  double curr_salinity = -1;
  double curr_pressure = -1;
 
  while ( stream_in.good() ) {
    if ( depth_bool ) stream_in >> curr_depth;   
    if ( ssp_bool ) stream_in >> curr_ssp;
    if ( temperature_bool ) stream_in >> curr_temperature;   
    if ( salinity_bool ) stream_in >> curr_salinity;   
    if ( pressure_bool ) stream_in >> curr_pressure;

    if ( depth_bool && ssp_bool && ( !temperature_bool || !pressure_bool || !salinity_bool ) ) insertValue( curr_depth, curr_ssp); 
    else if ( depth_bool && ssp_bool && temperature_bool && salinity_bool && pressure_bool)
      insertValue( curr_depth, curr_temperature, curr_salinity, Pressure( curr_pressure ), curr_ssp );
    else if ( temperature_bool && salinity_bool && pressure_bool ) insertValue( curr_temperature, curr_salinity, Pressure( curr_pressure) );
    else if ( depth_bool && temperature_bool && salinity_bool ) insertValue( curr_depth, curr_temperature, curr_salinity );
  }
  
  stream_in.precision(old_precision);
  return true;
}


bool SSP::write( std::ostream& stream_out ) const {
  using namespace std;
  
  streamsize old_precision = stream_out.precision();
  stream_out.precision(SSP_WRITE_PRECISION);
  
  DConstIter tempit = temperature_map.begin();
  DConstIter pressit = pressure_map.begin();
  DConstIter salit = salinity_map.begin();

  stream_out << setw(SSP_WRITE_MARGIN) << " DEPTH_[m] " << setw(SSP_WRITE_MARGIN) << " SSP_[m/s] ";
  if ( !temperature_map.empty() ) stream_out << setw(SSP_WRITE_MARGIN) << " TEMPERATURE_[C°] ";
  if ( !salinity_map.empty() ) stream_out << setw(SSP_WRITE_MARGIN) << " SALINITY_[ppu] ";
  if ( !pressure_map.empty() ) stream_out << setw(SSP_WRITE_MARGIN) << " PRESSURE_[bar] "; 
  stream_out << std::endl;
             
  for ( DConstIter it = ssp_map.begin(); it != ssp_map.end(); it++ ) {
    
    stream_out << setw(SSP_WRITE_MARGIN) << it->first << setw(SSP_WRITE_MARGIN) << it->second;
    
    if ( tempit != temperature_map.end() ) {
      stream_out << setw(SSP_WRITE_MARGIN) << tempit->second;       
      tempit++;
    }
    if ( salit != salinity_map.end() ) {
      stream_out << setw(SSP_WRITE_MARGIN) << salit->second;       
      salit++;
    }
    if ( pressit != pressure_map.end() ) {
      stream_out << setw(SSP_WRITE_MARGIN) << pressit->second;       
      pressit++;
    }    
    stream_out << endl;
  }
  
  stream_out.precision(old_precision);
  return true;
}



// transformation methods

// gets the LAST value if same precision!
void SSP::setDepthPrecision( long double prec ) {
  if ( prec == depth_precision ) return;
  
  DepthMap ssp_map_temp;
  DepthMap temp_map_temp;
  DepthMap salinity_map_temp;
  DepthMap press_map_temp;
  
  for ( DIter it = ssp_map.begin(); it != ssp_map.end(); it++ ) {
    ssp_map_temp.insert( ::std::make_pair( PDouble( it->first.getValue(), prec), it->second ) );
  }
  ssp_map.swap(ssp_map_temp);
  
  for ( DIter it = temperature_map.begin(); it != temperature_map.end(); it++ ) {
    temp_map_temp.insert( ::std::make_pair( PDouble( it->first.getValue(), prec), it->second ) );
  }
  temperature_map.swap(temp_map_temp);
  
  for ( DIter it = salinity_map.begin(); it != salinity_map.end(); it++ ) {
    salinity_map_temp.insert( ::std::make_pair( PDouble( it->first.getValue(), prec), it->second ) );
  }
  salinity_map.swap(salinity_map_temp);
  
  for ( DIter it = pressure_map.begin(); it != pressure_map.end(); it++ ) {
    press_map_temp.insert( ::std::make_pair( PDouble( it->first.getValue(), prec), it->second ) );
  }
  pressure_map.swap(press_map_temp);
} 

  
SSP* SSP::transform( const Coord& coordinates, double new_min_depth, double new_max_depth, int total_depth_steps) const {
  if( !isTransformable() ) return ( new SSP() ) ;
  
  if ( new_min_depth == -HUGE_VAL ) new_min_depth = ssp_map.begin()->first;
  if ( new_max_depth == HUGE_VAL ) new_max_depth = ssp_map.rbegin()->first;
  if ( total_depth_steps <= 0 ) total_depth_steps = ssp_map.size();
  
//   ::std::cout.precision(18);
  
//   ::std::cout << "SSP::transform() coord is valid " << coordinates.isValid() 
//               << "; new min depth " << new_min_depth << "; new max depth " << new_max_depth 
//               << "; total depth steps " << total_depth_steps << ::std::endl;
              
  assert( coordinates.isValid() && total_depth_steps > 0 && new_max_depth > new_min_depth );
  
  if ( isRandomizable() ) {
    DepthMap new_ssp_map;
    DepthMap new_temp_map;
    DepthMap new_sal_map;
    DepthMap new_press_map;

    PDouble curr_depth;
    DConstIter ssp_found;
    DConstIter temp_found;
    DConstIter press_found;
    DConstIter sal_found;
    
    for ( int i = 0; i < total_depth_steps; i++ ) {
      curr_depth = PDouble( new_min_depth + ( new_max_depth - new_min_depth ) / ( (double)total_depth_steps - 1.0 ) * i , depth_precision);
  
      ssp_found = ssp_map.find( curr_depth );
      press_found = pressure_map.find( curr_depth );
      temp_found = temperature_map.find( curr_depth );
      sal_found = salinity_map.find( curr_depth );

      if ( temp_found != temperature_map.end() ) new_temp_map[ curr_depth ] = temp_found->second;
      else {
        temp_found = temperature_map.lower_bound( curr_depth );
        if ( temp_found != temperature_map.end() ) {
          double up_temp = temp_found->second;
          double up_depth = temp_found->first;
          double incr;
          double prev_temp;
                  
          if ( temp_found != temperature_map.begin() ) {
            temp_found--;
            prev_temp = temp_found->second;
            double prev_depth = temp_found->first;
            incr = ( up_temp - prev_temp ) / ( up_depth - prev_depth ) * ( (double) curr_depth - prev_depth );
          }
          else {
            prev_temp = up_temp;
            incr = 0.0;
          }
          new_temp_map[ curr_depth ] = prev_temp + incr; 
        }
        else new_temp_map[ curr_depth ] = temperature_map.rbegin()->second;
      }

      if ( press_found != pressure_map.end() ) new_press_map[ curr_depth ] = press_found->second;
      else {
        new_press_map[ curr_depth ] = getPressureFromDepth( coordinates, curr_depth );
      }

      if ( sal_found != salinity_map.end() ) new_sal_map[ curr_depth ] = sal_found->second;
      else {
        sal_found = salinity_map.lower_bound( curr_depth );
        if ( sal_found != salinity_map.end() ) {
          double up_sal = sal_found->second;
          double up_depth = sal_found->first;  
          double incr;
          double prev_sal;
          
          if (sal_found != salinity_map.begin() ) {
            sal_found--;
            prev_sal = sal_found->second;
            double prev_depth = sal_found->first;
            incr = ( up_sal - prev_sal ) / ( up_depth - prev_depth ) * ( (double) curr_depth - prev_depth );
          }
          else {
            prev_sal = up_sal;
            incr = 0.0;
          }
          new_sal_map[ curr_depth ] = prev_sal + incr; 
        }
        else new_sal_map[ curr_depth ] = salinity_map.rbegin()->second;
      }

      if ( ssp_found != ssp_map.end() ) new_ssp_map[ curr_depth ] = ssp_found->second;
      else new_ssp_map[ curr_depth ] = calculateSSP( new_temp_map[curr_depth], new_sal_map[curr_depth], new_press_map[curr_depth] );
    }
    return( new SSP( new_ssp_map, new_temp_map, new_sal_map, new_press_map, depth_precision ) );
  }
  else {
    DepthMap new_ssp_map;
//     new_min_depth -= depth_precision;
    
    PDouble curr_depth;
    DConstIter ssp_found;
    total_depth_steps = ssp_map.size();
    
    for ( int i = 0; i < total_depth_steps; i++ ) {
      curr_depth = PDouble( new_min_depth + ( new_max_depth - new_min_depth ) / ( (double)total_depth_steps - 1.0 ) * i , depth_precision);
//       ::std::cout << "curr depth " << curr_depth << ::std::endl;
      
      ssp_found = ssp_map.find( curr_depth );
      
      if ( ssp_found != ssp_map.end() ) new_ssp_map[ curr_depth ] = ssp_found->second;
      else {
        ssp_found = ssp_map.lower_bound( curr_depth );
        if ( ssp_found != ssp_map.end() ) {
          double up_ssp = ssp_found->second;
          double up_depth = ssp_found->first;
          double incr;
          double prev_ssp;
          
          if ( ssp_found != ssp_map.begin() ) {
            ssp_found--;
            prev_ssp = ssp_found->second;
            double prev_depth = ssp_found->first;
            incr = ( up_ssp - prev_ssp ) / ( up_depth - prev_depth ) * ( (double) curr_depth - prev_depth );
          }
          else {
            prev_ssp = up_ssp;
            incr = 0.0;
          }
          new_ssp_map[ curr_depth ] = prev_ssp + incr; 
        }
        else new_ssp_map[ curr_depth ] = ssp_map.rbegin()->second;
      }
    }
    return new SSP( new_ssp_map, depth_precision);
  }
}

SSP* SSP::truncate(const double max_depth) const {
  SSP* new_SSP = clone();

  DIter it_lb = new_SSP->ssp_map.lower_bound( max_depth );

  //If lower bound is below max depth, create new point at max depth
  if (it_lb->first > PDouble(max_depth)) {
    // we check if we can go back
    if (it_lb != new_SSP->begin()) {
      // go back to previous depth point
      it_lb--;

      double extrap_speed = 0;
      // we check if we can go back again
      if (it_lb != new_SSP->begin()) {
        DIter prev_lb = it_lb;
        // go back again to previous depth point
        prev_lb--;

        if (debug) {
          std::cout << "SSP::truncate() cur point: " << it_lb->first << " " << it_lb->second << std::endl;
          std::cout << "SSP::truncate() prev point: " << prev_lb->first << " " << prev_lb->second << std::endl;
        }

        extrap_speed = PDouble(it_lb->second) - ( (it_lb->first - PDouble(max_depth)) * PDouble(it_lb->second - prev_lb->second) / (it_lb->first - prev_lb->first) );

        if (debug) {
          std::cout << "SSP::truncate() extrapolated speed: " << extrap_speed << std::endl;
        }
      }
      else {
        //There is only one value above the max dpeth, so the best we can do is to duplicate this at max depth
        extrap_speed = PDouble(it_lb->second);

        if (debug) {
          std::cout << "SSP::truncate() extrapolated (duplicated) speed: " << extrap_speed << std::endl;
        }
      }
      // insert point at max_depth
      new_SSP->insertValue(max_depth, extrap_speed);

      // advance the iterator to max_depth value
      it_lb++;
    }
    else {
      // SSP should be not valid
      delete new_SSP;
      new_SSP = new SSP();
      return new_SSP;
    }
  }

  //Check if there are values to be truncated
  if (it_lb != new_SSP->end()) {
    if(debug) std::cout << "Truncating" << std::endl;
    //Truncate - remove all values deeper than max_depth depth
    it_lb++;
    new_SSP->ssp_map.erase(it_lb, new_SSP->ssp_map.end());
  }

  return ( new_SSP );
}

SSP* SSP::fullRandomize( double perc_incr_value ) const {
  assert( perc_incr_value > 0.0 && perc_incr_value <= 1.0 );
  
  if ( !isRandomizable() ) return( new SSP() ); // returns invalid SSP;
  
  DepthMap new_ssp_map;
  DepthMap new_temp_map;
  DepthMap new_sal_map;
  DepthMap new_press_map;
   
  double sign = 1.0;
  
  for( DConstIter it = temperature_map.begin(); it != temperature_map.end(); it++ ) {
    if ( SDefHandler::instance()->getRand() >= 0.5 ) sign = 1.0;
    else sign = -1.0;
    new_temp_map[it->first] = it->second + sign * ( SDefHandler::instance()->getRand() ) * ( it->second * perc_incr_value);
  }
  
  for( DConstIter it = pressure_map.begin(); it != pressure_map.end(); it++ ) {
    if ( SDefHandler::instance()->getRand() >= 0.5 ) sign = 1.0;
    else sign = -1.0;
    new_press_map[it->first] = it->second + sign * ( SDefHandler::instance()->getRand() ) * ( it->second * perc_incr_value);
  }
  
  for( DConstIter it = salinity_map.begin(); it != salinity_map.end(); it++ ) {
    if ( SDefHandler::instance()->getRand() >= 0.5 ) sign = 1.0;
    else sign = -1.0;
    new_sal_map[it->first] = it->second + sign * ( SDefHandler::instance()->getRand() ) * ( it->second * perc_incr_value);
  }
  
  for( DConstIter it = ssp_map.begin(); it != ssp_map.end(); it++ ) {
    new_ssp_map[it->first] = calculateSSP( new_temp_map[it->first], new_sal_map[it->first], new_press_map[it->first] );
  }

  return( new SSP( new_ssp_map, new_temp_map, new_sal_map, new_press_map, depth_precision ) );
}


SSP* SSP::randomize( double perc_incr_value ) const {
  if ( !isValid() ) return( new SSP() );

  assert( perc_incr_value > 0.0 && perc_incr_value <= 1.0 );

  static DepthMap new_ssp;
  
  new_ssp.clear();
   
  double sign = 1.0;
  
  for( DConstIter it = ssp_map.begin(); it != ssp_map.end(); it++ ) {
    if ( SDefHandler::instance()->getRand() >= 0.5 ) sign = 1.0;
    else sign = -1.0;
    new_ssp[it->first] = it->second + sign * ( SDefHandler::instance()->getRand() ) * ( it->second * perc_incr_value);
  }
  
  return( new SSP( new_ssp, depth_precision ) );
}
  
///// 

double SSP::getPressureCorreptions( const Coord& coordinates, double depth ) const { 

  if ( isCanonOcean(coordinates) ) {
    return ( 1.0e-2 * depth / (depth + 100.0) + 6.2e-6*depth );
  }
  else if ( isMediterraneanSea(coordinates) ) {
    return ( 8.0e-3 * depth / (depth + 1000.0) + 1.6e-6*depth );
  }
  else if ( isNEAtlanticOcean(coordinates) ) {
    return ( -8.5e-6 * depth + 1.4e-9*pow(depth, 2.0) );
  }
  else if ( isRedSea(coordinates) ) {
    return 0.0;
  }
  else if ( isJapanSea(coordinates) ) {
    return ( 7.8e-6*depth );
  }
  else if ( isHalmaheraSea(coordinates) ) {
    return ( 8.0e-3 * depth / (depth + 50.0) + 1.3e-5*depth );
  }
  else if ( isSuluSea(coordinates) ) {
    return ( 1.0e-2 * depth / (depth + 100.0) + 1.6e-5*depth + 1e-9*pow(depth,2.0) );
  }
  else if ( isCelebesSea(coordinates) && !isSuluSea(coordinates) ) {
    return ( 1.2e-2 * depth / (depth + 100.0) + 7.0e-6*depth + 2.5e-10*pow(depth,2.0) );
  }
  else if ( isBlackSea(coordinates) ) {
    return ( 1.13e-4 * depth );
  }
  else if ( isBalticSea(coordinates) ) {
    return ( 1.8e-4 * depth );
  }
  else if ( isAntarcticOcean(coordinates) ) {
    return ( 8.0e-3 * depth / (depth + 1000.0) + 1.6e-6*depth );
  }
//   else if ( isArcticOcean(coordinates) ) {
//     return 0.0;
//   }
  else {
    return 0.0;
  }
}

//////////////////

double SSP::getDepthCorreptions( const Coord& coordinates, double pressure ) const { 

  if ( isCanonOcean(coordinates) ) {
    return( pressure / (pressure + 1.0) + 5.7e-2*pressure );
  }
  else if ( isMediterraneanSea(coordinates) ) {
    return(  -7.0e-2*pressure + 2.0e-3*pow(pressure, 2.0) );
  }
  else if ( isNEAtlanticOcean(coordinates) ) {
    return( pressure / (pressure + 2.0) + 3.0e-2*pressure );
  }
  else if ( isRedSea(coordinates) ) {
    return 0.0;
  }
  else if ( isJapanSea(coordinates) ) {
    return( 6.0e-2*pressure );
  }
  else if ( isHalmaheraSea(coordinates) ) {
    return( 0.8*pressure / (pressure + 0.5) + 0.125*pressure );
  }
  else if ( isSuluSea(coordinates) ) {
    return( 0.9*pressure / (pressure + 1.0) + 0.17*pressure + 7.0e-4*pow(pressure,2.0) );
  }
  else if ( isCelebesSea(coordinates) && !isSuluSea(coordinates) ) {
    return( 1.2*pressure / (pressure + 1.0) + 6.7e-2*pressure + 2.2e-4*pow(pressure,2.0) );
  }
  else if ( isBlackSea(coordinates) ) {
    return( 1.1*pressure );
  }
  else if ( isBalticSea(coordinates) ) {
    return( 1.8*pressure );
  }
  else if ( isAntarcticOcean(coordinates) ) {
    return(  4.0e-2*pressure - 2.0e-4*pow(pressure, 2.0) );
  }
//   else if ( isArcticOcean(coordinates) ) {
//     return 0.0;
//   }
  else {
    return 0.0;
  }
}

