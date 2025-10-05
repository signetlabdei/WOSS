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
 * @file   ssp-definitions.cpp
 * @author Federico Guerra
 *
 * \brief  Implementation of woss::SSP (Sound Speed Profile) class 
 *
 * Implementation of woss::SSP (Sound Speed Profile) class 
 */


#define SSP_WRITE_PRECISION ( 17 )
#define SSP_WRITE_MARGIN ( SSP_WRITE_PRECISION + 4 )


#include <cassert>
#include <iomanip>
#include <set>
#include <sstream>
#include "definitions-handler.h"
#include "ssp-definitions.h"


using namespace woss;


bool SSP::debug = false;


SSP::SSP( long double prec ) 
: ssp_eq_type(SSPEqType::SSP_EQ_TEOS_10_EXACT),
  min_ssp_value(HUGE_VAL),
  max_ssp_value(0.0),
  depth_precision(prec),
  ssp_map(),
  pressure_map(),
  salinity_map(),
  temperature_map()
{

}


SSP::SSP( DepthMap& ssp_m, DepthMap& temp_map, DepthMap& sal_map, DepthMap& press_map, long double depth_p )
  : ssp_eq_type(SSPEqType::SSP_EQ_TEOS_10_EXACT),
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
  : ssp_eq_type(SSPEqType::SSP_EQ_TEOS_10_EXACT),
    min_ssp_value(HUGE_VAL),
    max_ssp_value(0.0),
    depth_precision(depth_p)
{
  ssp_map.swap(ssp_m);

}

  
DConstIter SSP::at( const int position ) const {
  if ( position >= (int) ssp_map.size() || position < 0 ) 
    return ssp_map.end();
  if ( position == 0 ) 
    return ssp_map.begin();

  DConstIter ret_val = ssp_map.begin();
  std::advance( ret_val, position );
  return ret_val;
}


SSP& SSP::insertValue( double depth, double ssp_value ) {
  assert( ssp_value > 0 && depth >= 0);

  if (ssp_value > max_ssp_value) 
    max_ssp_value = ssp_value;
  if (ssp_value < min_ssp_value) 
    min_ssp_value = ssp_value;

  ssp_map.emplace( std::make_pair( PDouble( depth, depth_precision ), ssp_value ) );
  
  return *this;
}


SSP& SSP::insertValue( double depth, double temperature, double salinity, const Coord& coordinates ) {
  assert( temperature > -20.0 && temperature < 50.0 );
  assert( salinity >= 0.0 && salinity <= 60.0 );
  assert( coordinates.isValid() );
  
  double pressure = getPressureFromDepth( coordinates, depth );
  double curr_ssp = calculateSSP( temperature, salinity, pressure );

  if (curr_ssp > max_ssp_value) 
    max_ssp_value = curr_ssp;
  if (curr_ssp < min_ssp_value) 
    min_ssp_value = curr_ssp;

  PDouble curr_depth = PDouble( depth , depth_precision ) ;

  ssp_map.emplace( std::make_pair( curr_depth, curr_ssp ) );
  pressure_map.emplace( std::make_pair( curr_depth, pressure ) );
  temperature_map.emplace( std::make_pair( curr_depth, temperature ) );
  salinity_map.emplace( std::make_pair( curr_depth, salinity ) );
  
  return *this;
}

  
SSP& SSP::insertValue( double depth, double temperature, double salinity, const std::complex<double>& pressure, double ssp_value ) {
  assert( temperature > -20.0 && temperature < 50.0 );
  assert( salinity >= 0.0 && salinity <= 60.0 );
  assert( pressure.real() >= 0 );
  assert( ssp_value > 0 && depth >= 0);

  PDouble curr_depth = PDouble( depth, depth_precision );

  if (ssp_value > max_ssp_value) 
    max_ssp_value = ssp_value;
  if (ssp_value < min_ssp_value) 
    min_ssp_value = ssp_value;

  ssp_map.emplace( std::make_pair( curr_depth, ssp_value ) );
  pressure_map.emplace( std::make_pair( curr_depth, pressure.real() ) );
  temperature_map.emplace( std::make_pair( curr_depth, temperature ) );
  salinity_map.emplace( std::make_pair( curr_depth, salinity ) );

  return *this;
}


SSP& SSP::insertValue( double temperature, double salinity, const std::complex<double>& pressure, const Coord& coordinates ) {
  assert( temperature > -20.0 && temperature < 50.0 );
  assert( salinity >= 0.0 && salinity <= 60.0 );
  assert( pressure.real() >= 0 );
  assert( coordinates.isValid() );

  double curr_ssp = calculateSSP( temperature, salinity, pressure.real() );
  if (curr_ssp > max_ssp_value) 
    max_ssp_value = curr_ssp;
  if (curr_ssp < min_ssp_value) 
    min_ssp_value = curr_ssp;

  PDouble curr_depth( getDepthfromPressure( coordinates, pressure.real() ) , depth_precision );
  
  ssp_map.emplace( std::make_pair( curr_depth, curr_ssp ) );
  pressure_map.emplace( std::make_pair( curr_depth, pressure.real() ) );
  temperature_map.emplace( std::make_pair( curr_depth, temperature ) );
  salinity_map.emplace( std::make_pair( curr_depth, salinity ) );
  
  return *this;
}


SSP& woss::operator+=( SSP& left, const SSP& right ) {
  auto tempit = right.temperature_map.cbegin();
  auto pressit = right.pressure_map.cbegin();
  auto salit = right.salinity_map.cbegin();

  for ( auto it = right.ssp_map.cbegin(); it != right.ssp_map.cend(); it++ ) {
    auto it2 = left.ssp_map.find(it->first);
    if ( it2 != left.ssp_map.end() ) 
      it2->second += it->second; 
    else 
      left.ssp_map[it->first] += it->second;

    if ( tempit != right.temperature_map.cend() ) {
      it2 = left.temperature_map.find(tempit->first);
      if ( it2 != left.temperature_map.end() ) 
        it2->second += tempit->second; 
      else 
        left.temperature_map[tempit->first] += tempit->second;
      tempit++;
    }
    if ( pressit != right.pressure_map.cend() ) {
      it2 = left.pressure_map.find(pressit->first);
      if ( it2 != left.pressure_map.end() ) 
        it2->second += pressit->second; 
      else 
        left.pressure_map[pressit->first] += pressit->second;
      pressit++;
    }
    if ( salit != right.salinity_map.cend() ) {
      it2 = left.salinity_map.find(salit->first);
      if ( it2 != left.salinity_map.end() ) 
        it2->second += salit->second; 
      else 
        left.salinity_map[salit->first] += salit->second;
      salit++;
    }
  }
  return left;
}


SSP& woss::operator-=( SSP& left, const SSP& right ) {
  auto tempit = right.temperature_map.cbegin();
  auto pressit = right.pressure_map.cbegin();
  auto salit = right.salinity_map.cbegin();

  for ( auto it = right.ssp_map.cbegin(); it != right.ssp_map.cend(); it++ ) {
    auto it2 = left.ssp_map.find(it->first);
    if ( it2 != left.ssp_map.end() ) 
      it2->second -= it->second; 
    else 
      left.ssp_map[it->first] -= it->second;

    if ( tempit != right.temperature_map.cend() ) {
      it2 = left.temperature_map.find(tempit->first);
      if ( it2 != left.temperature_map.end() ) 
        it2->second -= tempit->second; 
      else 
        left.temperature_map[tempit->first] -= tempit->second;
      tempit++;
    }
    if ( pressit != right.pressure_map.cend() ) {
      it2 = left.pressure_map.find(pressit->first);
      if ( it2 != left.pressure_map.end() ) 
        it2->second -= pressit->second; 
      else 
        left.pressure_map[pressit->first] -= pressit->second;
      pressit++;
    }
    if ( salit != right.salinity_map.cend() ) {
      it2 = left.salinity_map.find(salit->first);
      if ( it2 != left.salinity_map.end() ) 
        it2->second -= salit->second; 
      else 
        left.salinity_map[salit->first] -= salit->second;
      salit++;
    }
  }
  return left;
}


SSP& woss::operator*=( SSP& left, const SSP& right ) {
  auto tempit = right.temperature_map.cbegin();
  auto pressit = right.pressure_map.cbegin();
  auto salit = right.salinity_map.cbegin();

  for ( auto it = right.ssp_map.cbegin(); it != right.ssp_map.cend(); it++ ) {
    auto it2 = left.ssp_map.find(it->first);
    if ( it2 != left.ssp_map.end() ) 
      it2->second *= it->second; 
    else 
      left.ssp_map[it->first] *= it->second;

    if ( tempit != right.temperature_map.cend() ) {
      it2 = left.temperature_map.find(tempit->first);
      if ( it2 != left.temperature_map.end() ) 
        it2->second *= tempit->second; 
      else 
        left.temperature_map[tempit->first] *= tempit->second;
      tempit++;
    }
    if ( pressit != right.pressure_map.cend() ) {
      it2 = left.pressure_map.find(pressit->first);
      if ( it2 != left.pressure_map.end() ) 
        it2->second *= pressit->second; 
      else 
        left.pressure_map[pressit->first] *= pressit->second;
      pressit++;
    }
    if ( salit != right.salinity_map.cend() ) {
      it2 = left.salinity_map.find(salit->first);
      if ( it2 != left.salinity_map.end() ) 
        it2->second *= salit->second; 
      else 
        left.salinity_map[salit->first] *= salit->second;
      salit++;
    }
  }
  return left;
}


SSP& woss::operator/=( SSP& left, const SSP& right ) {
  auto tempit = right.temperature_map.cbegin();
  auto pressit = right.pressure_map.cbegin();
  auto salit = right.salinity_map.cbegin();

  for ( auto it = right.ssp_map.cbegin(); it != right.ssp_map.cend(); it++ ) {
    auto it2 = left.ssp_map.find(it->first);
    if ( it2 != left.ssp_map.end() ) 
      it2->second /= it->second; 
    else 
      left.ssp_map[it->first] /= it->second;

    if ( tempit != right.temperature_map.cend() ) {
      it2 = left.temperature_map.find(tempit->first);
      if ( it2 != left.temperature_map.end() ) 
        it2->second /= tempit->second; 
      else 
        left.temperature_map[tempit->first] /= tempit->second;
      tempit++;
    }
    if ( pressit != right.pressure_map.cend() ) {
      it2 = left.pressure_map.find(pressit->first);
      if ( it2 != left.pressure_map.end() ) 
        it2->second /= pressit->second; 
      else 
        left.pressure_map[pressit->first] /= pressit->second;
      pressit++;
    }
    if ( salit != right.salinity_map.cend() ) {
      it2 = left.salinity_map.find(salit->first);
      if ( it2 != left.salinity_map.end() ) 
        it2->second /= salit->second; 
      else 
        left.salinity_map[salit->first] /= salit->second;
      salit++;
    }
  }
  return left;
}


SSP& woss::operator+=( SSP& left, const double right ) {
  auto tempit = left.temperature_map.cbegin();
  auto pressit = left.pressure_map.cbegin();
  auto salit = left.salinity_map.cbegin();

  for ( auto it = left.ssp_map.cbegin(); it != left.ssp_map.cend(); it++ ) {
    left.ssp_map[it->first] += right;

    if ( tempit != left.temperature_map.cend() ) {
      left.temperature_map[tempit->first] += right;
      tempit++;
    }
    if ( pressit != left.pressure_map.cend() ) {
      left.temperature_map[pressit->first] += right;
      pressit++;
    }
    if ( salit != left.salinity_map.cend() ) {
      left.temperature_map[salit->first] += right;
      salit++;
    }
  }
  return left;
}


SSP& woss::operator-=( SSP& left, const double right ) {
  auto tempit = left.temperature_map.cbegin();
  auto pressit = left.pressure_map.cbegin();
  auto salit = left.salinity_map.cbegin();

  for ( auto it = left.ssp_map.cbegin(); it != left.ssp_map.cend(); it++ ) {
    left.ssp_map[it->first] -= right;

    if ( tempit != left.temperature_map.cend() ) {
      left.temperature_map[tempit->first] -= right;
      tempit++;
    }
    if ( pressit != left.pressure_map.cend() ) {
      left.temperature_map[pressit->first] -= right;
      pressit++;
    }
    if ( salit != left.salinity_map.cend() ) {
      left.temperature_map[salit->first] -= right;
      salit++;
    }
  }
  return left;
}


SSP& woss::operator/=( SSP& left, const double right ) {
  auto tempit = left.temperature_map.cbegin();
  auto pressit = left.pressure_map.cbegin();
  auto salit = left.salinity_map.cbegin();

  for ( auto it = left.ssp_map.cbegin(); it != left.ssp_map.cend(); it++ ) {
    left.ssp_map[it->first] /= right;

    if ( tempit != left.temperature_map.cend() ) {
      left.temperature_map[tempit->first] /= right;
      tempit++;
    }
    if ( pressit != left.pressure_map.cend() ) {
      left.temperature_map[pressit->first] /= right;
      pressit++;
    }
    if ( salit != left.salinity_map.cend() ) {
      left.temperature_map[salit->first] /= right;
      salit++;
    }
  }
  return left;
}


SSP& woss::operator*=( SSP& left, const double right ) {
  auto tempit = left.temperature_map.cbegin();
  auto pressit = left.pressure_map.cbegin();
  auto salit = left.salinity_map.cbegin();

  for ( auto it = left.ssp_map.cbegin(); it != left.ssp_map.cend(); it++ ) {
    left.ssp_map[it->first] *= right;

    if ( tempit != left.temperature_map.cend() ) {
      left.temperature_map[tempit->first] *= right;
      tempit++;
    }
    if ( pressit != left.pressure_map.cend() ) {
      left.temperature_map[pressit->first] *= right;
      pressit++;
    }
    if ( salit != left.salinity_map.cend() ) {
      left.temperature_map[salit->first] *= right;
      salit++;
    }
  }
  return left;
}


bool SSP::import( std::istream& stream_in ) {
  using namespace std;
  
  if ( !stream_in.good() ) 
    return false;
  
  streamsize old_precision = stream_in.precision();
  stream_in.precision(SSP_WRITE_PRECISION);

  string curr_token;
  set< string > string_set;
  
  getline( stream_in, curr_token );
  
  if (debug)
    cout << "first line " << curr_token << endl;
  
  stringstream ss( curr_token );

  while (ss >> curr_token) {
    if (debug)
      cout << "token = " << curr_token << endl;
    string_set.emplace(curr_token);
  }
  bool ssp_bool = false;
  bool temperature_bool = false;
  bool salinity_bool = false;
  bool pressure_bool = false;
  bool depth_bool = false;
  
  set< string >::iterator it;
  
  if ( ( it = string_set.find( "DEPTH_[m]" ) ) != string_set.end() ) 
    depth_bool = true;
  if ( ( it = string_set.find( "SSP_[m/s]" ) ) != string_set.end() ) 
    ssp_bool = true;
  if ( ( it = string_set.find( "TEMPERATURE_[C°]" ) ) != string_set.end() ) 
    temperature_bool = true;
  if ( ( it = string_set.find( "SALINITY_[ppu]" ) ) != string_set.end() ) 
    salinity_bool = true;
  if ( ( it = string_set.find( "PRESSURE_[bar]" ) ) != string_set.end() ) 
    pressure_bool = true;

  if ( depth_bool == false && pressure_bool == false ) 
    return false;
  if ( ssp_bool == false && ( temperature_bool == false || salinity_bool == false ) ) 
    return false;
  
  double curr_depth = -1;
  double curr_ssp = -1;
  double curr_temperature = -1;
  double curr_salinity = -1;
  double curr_pressure = -1;
 
  while ( stream_in.good() ) {
    if ( depth_bool ) 
      stream_in >> curr_depth;
    if ( ssp_bool ) 
      stream_in >> curr_ssp;
    if ( temperature_bool ) 
      stream_in >> curr_temperature;
    if ( salinity_bool ) 
      stream_in >> curr_salinity;
    if ( pressure_bool ) 
      stream_in >> curr_pressure;

    if ( depth_bool && ssp_bool && ( !temperature_bool || !pressure_bool || !salinity_bool ) ) 
      insertValue( curr_depth, curr_ssp); 
    else if ( depth_bool && ssp_bool && temperature_bool && salinity_bool && pressure_bool)
      insertValue( curr_depth, curr_temperature, curr_salinity, Pressure( curr_pressure ), curr_ssp );
    else if ( temperature_bool && salinity_bool && pressure_bool ) 
      insertValue( curr_temperature, curr_salinity, Pressure( curr_pressure) );
    else if ( depth_bool && temperature_bool && salinity_bool ) 
      insertValue( curr_depth, curr_temperature, curr_salinity );
  }
  
  stream_in.precision(old_precision);
  return true;
}


bool SSP::write( std::ostream& stream_out ) const {
  using namespace std;
  
  streamsize old_precision = stream_out.precision();
  stream_out.precision(SSP_WRITE_PRECISION);
  
  auto tempit = temperature_map.cbegin();
  auto pressit = pressure_map.cbegin();
  auto salit = salinity_map.cbegin();

  stream_out << setw(SSP_WRITE_MARGIN) << " DEPTH_[m] " << setw(SSP_WRITE_MARGIN) << " SSP_[m/s] ";
  if ( !temperature_map.empty() ) 
    stream_out << setw(SSP_WRITE_MARGIN) << " TEMPERATURE_[C°] ";
  if ( !salinity_map.empty() ) 
    stream_out << setw(SSP_WRITE_MARGIN) << " SALINITY_[ppu] ";
  if ( !pressure_map.empty() ) 
    stream_out << setw(SSP_WRITE_MARGIN) << " PRESSURE_[bar] "; 
  stream_out << std::endl;
             
  for ( auto it = ssp_map.cbegin(); it != ssp_map.cend(); it++ ) {
    stream_out << setw(SSP_WRITE_MARGIN) << it->first << setw(SSP_WRITE_MARGIN) << it->second;
    
    if ( tempit != temperature_map.cend() ) {
      stream_out << setw(SSP_WRITE_MARGIN) << tempit->second;
      tempit++;
    }
    if ( salit != salinity_map.cend() ) {
      stream_out << setw(SSP_WRITE_MARGIN) << salit->second;
      salit++;
    }
    if ( pressit != pressure_map.cend() ) {
      stream_out << setw(SSP_WRITE_MARGIN) << pressit->second;
      pressit++;
    }
    stream_out << endl;
  }
  
  stream_out.precision(old_precision);
  return true;
}


// gets the LAST value if same precision!
void SSP::setDepthPrecision( long double prec ) {
  if ( prec == depth_precision ) 
    return;

  DepthMap ssp_map_temp;
  DepthMap temp_map_temp;
  DepthMap salinity_map_temp;
  DepthMap press_map_temp;
  
  for ( const auto& it : ssp_map ) {
    ssp_map_temp.emplace( std::make_pair( PDouble(it.first.getValue(), prec), it.second ) );
  }
  ssp_map.swap(ssp_map_temp);
  
  for ( const auto& it : temperature_map ) {
    temp_map_temp.emplace( std::make_pair( PDouble(it.first.getValue(), prec), it.second ) );
  }
  temperature_map.swap(temp_map_temp);
  
  for ( const auto& it : salinity_map ) {
    salinity_map_temp.emplace( std::make_pair( PDouble( it.first.getValue(), prec), it.second ) );
  }
  salinity_map.swap(salinity_map_temp);
  
  for ( const auto& it : pressure_map ) {
    press_map_temp.emplace( std::make_pair( PDouble( it.first.getValue(), prec), it.second ) );
  }
  pressure_map.swap(press_map_temp);
} 


std::unique_ptr<SSP> SSP::transform( const Coord& coordinates, double new_min_depth, double new_max_depth, int total_depth_steps) const {
  if( !isTransformable() ) 
    return ( create() ) ;

  if ( new_min_depth == -HUGE_VAL ) 
    new_min_depth = ssp_map.begin()->first;
  if ( new_max_depth == HUGE_VAL ) 
    new_max_depth = ssp_map.rbegin()->first;
  if ( total_depth_steps <= 0 ) 
    total_depth_steps = ssp_map.size();
  
  if (debug) {
    std::cout << "SSP::transform() coord is valid " << coordinates.isValid() 
              << "; new min depth " << new_min_depth << "; new max depth " << new_max_depth 
              << "; total depth steps " << total_depth_steps << std::endl;
  }

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

      if ( temp_found != temperature_map.end() ) 
        new_temp_map[ curr_depth ] = temp_found->second;
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
        else 
          new_temp_map[ curr_depth ] = temperature_map.rbegin()->second;
      }

      if ( press_found != pressure_map.end() ) 
        new_press_map[ curr_depth ] = press_found->second;
      else {
        new_press_map[ curr_depth ] = getPressureFromDepth( coordinates, curr_depth );
      }

      if ( sal_found != salinity_map.end() ) 
        new_sal_map[ curr_depth ] = sal_found->second;
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
        else 
          new_sal_map[ curr_depth ] = salinity_map.rbegin()->second;
      }

      if ( ssp_found != ssp_map.end() ) 
        new_ssp_map[ curr_depth ] = ssp_found->second;
      else 
        new_ssp_map[ curr_depth ] = calculateSSP( new_temp_map[curr_depth], new_sal_map[curr_depth], new_press_map[curr_depth] );
    }
    return create( new_ssp_map, new_temp_map, new_sal_map, new_press_map, depth_precision );
  }
  else {
    DepthMap new_ssp_map;

    PDouble curr_depth;
    DConstIter ssp_found;
    total_depth_steps = ssp_map.size();
    
    for ( int i = 0; i < total_depth_steps; i++ ) {
      curr_depth = PDouble( new_min_depth + ( new_max_depth - new_min_depth ) / ( (double)total_depth_steps - 1.0 ) * i , depth_precision);
      
      if (debug)
        std::cout << "curr depth " << curr_depth << std::endl;
      
      ssp_found = ssp_map.find( curr_depth );
      
      if ( ssp_found != ssp_map.end() ) 
        new_ssp_map[ curr_depth ] = ssp_found->second;
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
        else 
          new_ssp_map[ curr_depth ] = ssp_map.rbegin()->second;
      }
    }
    return create( new_ssp_map, depth_precision);
  }
}

std::unique_ptr<SSP> SSP::truncate(const double max_depth) const {
  auto new_SSP = clone();

  auto it_lb = new_SSP->ssp_map.lower_bound( max_depth );

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
      return create();
    }
  }

  //Check if there are values to be truncated
  if (it_lb != new_SSP->end()) {
    if(debug) 
      std::cout << "Truncating" << std::endl;
    //Truncate - remove all values deeper than max_depth depth
    it_lb++;
    new_SSP->ssp_map.erase(it_lb, new_SSP->ssp_map.end());
  }

  return new_SSP;
}

std::unique_ptr<SSP> SSP::fullRandomize( double perc_incr_value ) const {
  assert( perc_incr_value > 0.0 && perc_incr_value <= 1.0 );
  
  if ( !isRandomizable() ) 
    return( create() ); // returns invalid SSP;
  
  DepthMap new_ssp_map;
  DepthMap new_temp_map;
  DepthMap new_sal_map;
  DepthMap new_press_map;
   
  double sign = 1.0;
  
  for( const auto& it : temperature_map ) {
    if ( SDefHandler::instance().getRand() >= 0.5 ) 
      sign = 1.0;
    else 
      sign = -1.0;
    new_temp_map[it.first] = it.second + sign * ( SDefHandler::instance().getRand() ) * ( it.second * perc_incr_value);
  }
  
  for( const auto& it : pressure_map ) {
    if ( SDefHandler::instance().getRand() >= 0.5 ) 
      sign = 1.0;
    else 
      sign = -1.0;
    new_press_map[it.first] = it.second + sign * ( SDefHandler::instance().getRand() ) * ( it.second * perc_incr_value);
  }
  
  for( const auto& it : salinity_map ) {
    if ( SDefHandler::instance().getRand() >= 0.5 ) 
      sign = 1.0;
    else 
      sign = -1.0;
    new_sal_map[it.first] = it.second + sign * ( SDefHandler::instance().getRand() ) * ( it.second * perc_incr_value);
  }
  
  for( const auto& it : ssp_map ) {
    new_ssp_map[it.first] = calculateSSP( new_temp_map[it.first], new_sal_map[it.first], new_press_map[it.first] );
  }

  return( create( new_ssp_map, new_temp_map, new_sal_map, new_press_map, depth_precision ) );
}


std::unique_ptr<SSP> SSP::randomize( double perc_incr_value ) const {
  if ( !isValid() ) 
    return( create() );

  assert( perc_incr_value > 0.0 && perc_incr_value <= 1.0 );

  DepthMap new_ssp;

  new_ssp.clear();

  double sign = 1.0;

  for( const auto& it : ssp_map ) {
    if ( SDefHandler::instance().getRand() >= 0.5 ) 
      sign = 1.0;
    else 
      sign = -1.0;
    new_ssp[it.first] = it.second + sign * ( SDefHandler::instance().getRand() ) * ( it.second * perc_incr_value);
  }
  
  return( create( new_ssp, depth_precision ) );
}

