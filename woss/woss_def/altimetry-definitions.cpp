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
 * @file   altimetry-definitions.cpp
 * @author Federico Guerra
 *
 * \brief  Implementations and library for woss::Altimetry class 
 *
 * Implementations and library for woss::Altimetry class
 */


#include <cassert>
#include <climits>
#include "altimetry-definitions.h"
#include "definitions-handler.h"

static constexpr inline double ALTIMETRY_DEFAULT_RANDOMIZATION = (1e-3);
static constexpr inline double ALTIMETRY_CHAR_HEIGHT_INVALID = (HUGE_VAL);
static constexpr inline double ALTIMETRY_AVG_PERIOD_INVALID = (HUGE_VAL);
static constexpr inline double ALTIMETRY_RANGE_INVALID = (HUGE_VAL);
static constexpr inline double ALTIMETRY_RANGE_PRECISION = (1e-20);
static constexpr inline double ALTIMETRY_RANGE_STEPS = (1);
static constexpr inline double ALTIMETRY_DEFAULT_DEPTH = (80.0);
static constexpr inline double G = (9.80665);

using namespace woss;

bool Altimetry::debug = false;

Altimetry::Altimetry()
: range(ALTIMETRY_RANGE_INVALID),
  total_range_steps(ALTIMETRY_RANGE_STEPS),
  min_altimetry_value(HUGE_VAL),
  max_altimetry_value(-HUGE_VAL),
  range_precision(ALTIMETRY_RANGE_PRECISION),
  last_evolution_time(),
  evolution_time_quantum(-1.0),
  depth(ALTIMETRY_DEFAULT_DEPTH),
  altimetry_map()
{

}


Altimetry::Altimetry( AltimetryMap& map )
: range(ALTIMETRY_RANGE_INVALID),  
  total_range_steps(map.size()),
  min_altimetry_value(HUGE_VAL),
  max_altimetry_value(-HUGE_VAL),
  range_precision(ALTIMETRY_RANGE_PRECISION),
  last_evolution_time(),
  evolution_time_quantum(-1.0),  
  depth(ALTIMETRY_DEFAULT_DEPTH),
  altimetry_map()
{
  altimetry_map.swap(map);

  if ( altimetry_map.size() == 1 ) 
    range_precision = altimetry_map.begin()->first.getPrecision();
}


Altimetry::Altimetry( double altimetry, double range ) 
: range(range),  
  total_range_steps(1),
  min_altimetry_value(HUGE_VAL),
  max_altimetry_value(-HUGE_VAL),
  range_precision(range),
  last_evolution_time(),
  evolution_time_quantum(-1.0),
  depth(ALTIMETRY_DEFAULT_DEPTH),
  altimetry_map()
{
  if (altimetry > max_altimetry_value) 
    max_altimetry_value = altimetry;

  if (altimetry < min_altimetry_value) 
    min_altimetry_value = altimetry;

  altimetry_map.emplace( std::make_pair( range, altimetry ) );
}


bool Altimetry::isValid() const { 
  if ( altimetry_map.size() < 1 )
    return false;

  return true;
}


Altimetry& Altimetry::insertValue( double range, double altimetry ) {  
  assert( (altimetry != HUGE_VAL) && (altimetry != -HUGE_VAL) );
  
  if (altimetry > max_altimetry_value)
    max_altimetry_value = altimetry;

  if (altimetry < min_altimetry_value)
    min_altimetry_value = altimetry;
  
  altimetry_map.emplace( std::make_pair( PDouble(range, ALTIMETRY_RANGE_PRECISION), altimetry ) ); 
  return *this;
}


AltCIt Altimetry::at( const int position ) const {
  if ( position >= (int)altimetry_map.size() || position < 0 ) 
    return altimetry_map.end();

  if ( position == 0 ) 
    return altimetry_map.begin();

  AltCIt ret_val = altimetry_map.begin();
  std::advance( ret_val, position );

  return ret_val;
}


std::unique_ptr<Altimetry> Altimetry::crop( double range_start, double range_end ) const {
  if ( !isValid() ) 
    return( create(Altimetry::createNotValid()) );

  AltimetryMap temp_map;

  int i = 0;
  for ( const auto& it : altimetry_map) {
    if ( it.first >= PDouble(range_start, range_precision) 
      && it.first < PDouble(range_end, range_precision) ) {
      if (debug) {
        std::cout << "Altimetry::crop() index " << i << "; range_start " << range_start << "; range_end " << range_end 
                  << "; time value " << it.first << "; att " << it.second << std::endl;
      }
      temp_map[it.first] = it.second;
    }
    ++i;
  }
  return( create(temp_map) );
}


std::unique_ptr<Altimetry> Altimetry::randomize( double perc_incr_value ) const {
  if ( !isValid() ) 
    return( create(Altimetry::createNotValid()) );

  assert( perc_incr_value > 0.0 && perc_incr_value <= 1.0 );

  auto new_altimetry = AltimetryMap();
  double sign = 1.0;

  for( const auto& it : altimetry_map ) {
    if ( SDefHandler::instance().getRand() >= 0.5 ) 
      sign = 1.0;
    else 
      sign = -1.0;
    new_altimetry[it.first] = it.second + sign * ( SDefHandler::instance().getRand() ) * (it.second * perc_incr_value);
  }

  auto ret_val = create( new_altimetry );
  ret_val->updateMinMaxAltimetryValues();

  return( ret_val );
}


std::unique_ptr<Altimetry> Altimetry::timeEvolve( const Time& time_value ) { 
  if ( debug ) 
    std::cout << "Altimetry::timeEvolve() time_value = " 
                << time_value << "; evolution_time_quantum = " << evolution_time_quantum << std::endl;

  if ( evolution_time_quantum < 0.0 || !time_value.isValid() ) 
    return clone();

  Time t_value = time_value;

  double time_difference;
  if ( last_evolution_time.isValid() ) 
    time_difference = std::abs(last_evolution_time - t_value);
  else 
    time_difference = -1.0;
  
  if ( debug ) std::cout << "Altimetry::timeEvolve() t_value = " 
                         << t_value << "; time_difference = " << time_difference << std::endl;
  
  if ( ( evolution_time_quantum == 0.0 ) || ( time_difference >= evolution_time_quantum ) 
    || ( time_difference == -1.0 ) ) {
    last_evolution_time = t_value; 
    return randomize( ALTIMETRY_DEFAULT_RANDOMIZATION );
  }
  return clone();
}


bool Altimetry::initialize()
{
  if ( isValid() == false ) return false;
  range_precision = std::ceil(range/total_range_steps);

  return true;
}


Altimetry& Altimetry::updateMinMaxAltimetryValues() {
  for (const auto& it : altimetry_map) {
    if ( it.second > max_altimetry_value ) 
      max_altimetry_value = it.second;
    if ( it.second < min_altimetry_value ) 
      min_altimetry_value = it.second;
  }

  return *this;
}

  
const Altimetry woss::operator+( const Altimetry& left, const Altimetry& right ) { 
  Altimetry ret_val ( left );
  ret_val += right;
  return ret_val;
}


const Altimetry woss::operator-( const Altimetry& left, const Altimetry& right ) { 
  Altimetry ret_val ( left );
  ret_val -= right;
  return ret_val;
}


const Altimetry woss::operator+( const Altimetry& left, const double right ) { 
  Altimetry ret_val ( left );
  ret_val += right;
  return ret_val;
}


const Altimetry woss::operator-( const Altimetry& left, const double right ) { 
  Altimetry ret_val ( left );
  ret_val -= right;
  return ret_val;
}


const Altimetry woss::operator/( const Altimetry& left, const double right ) { 
  Altimetry ret_val ( left );
  ret_val /= right;
  return ret_val;
}


const Altimetry woss::operator*( const Altimetry& left, const double right ) { 
  Altimetry ret_val ( left );
  ret_val *= right;
  return ret_val;
}


const Altimetry woss::operator+( const double left, const Altimetry& right ) { 
  Altimetry ret_val ( right );
  ret_val += left;
  return ret_val;
}


const Altimetry woss::operator-( const double left, const Altimetry& right ) { 
  Altimetry ret_val ( right );
  ret_val -= left;
  return ret_val;
}


const Altimetry woss::operator/( const double left, const Altimetry& right ) { 
  Altimetry ret_val ( right );
  ret_val /= left;
  return ret_val;
}


const Altimetry woss::operator*( const double left, const Altimetry& right ) { 
  Altimetry ret_val ( right );
  ret_val *= left;
  return ret_val;
}


Altimetry& woss::operator+=( Altimetry& left, const Altimetry& right ) { 
  for ( const auto& it : right.altimetry_map ) {
    left.altimetry_map[it.first] += it.second;
  }
  return left;
}


Altimetry& woss::operator-=( Altimetry& left, const Altimetry& right ) { 
  for ( const auto& it : right.altimetry_map ) {
    left.altimetry_map[it.first] -= it.second;
  }
  return left;
}


Altimetry& woss::operator+=( Altimetry& left, double right ) { 
  for ( const auto& it : left.altimetry_map ) {
    left.altimetry_map[it.first] += right;
  }
  return left;
}


Altimetry& woss::operator-=( Altimetry& left, double right ) { 
  for ( const auto& it : left.altimetry_map ) {
    left.altimetry_map[it.first] -= right;
  }
  return left;
}


Altimetry& woss::operator/=( Altimetry& left, double right ) { 
  for ( const auto& it : left.altimetry_map ) {
    left.altimetry_map[it.first] /= right;
  }
  return left;
}


Altimetry& woss::operator*=( Altimetry& left, double right ) { 
  for ( const auto& it : left.altimetry_map ) {
    left.altimetry_map[it.first] *= right;
  }
  return left;
}


AltimBretschneider::AltimBretschneider() 
: Altimetry(),
  char_height(ALTIMETRY_CHAR_HEIGHT_INVALID),
  average_period(ALTIMETRY_AVG_PERIOD_INVALID)
{
  
}


AltimBretschneider::AltimBretschneider( AltimetryMap& map ) 
: Altimetry(map),
  char_height(ALTIMETRY_CHAR_HEIGHT_INVALID),
  average_period(ALTIMETRY_AVG_PERIOD_INVALID)
{

}


AltimBretschneider::AltimBretschneider( double ch_height, double avg_per, int total_r_steps, double d )
: Altimetry(),
  char_height(ch_height),
  average_period(avg_per)
{
  depth = d;
  total_range_steps = total_r_steps;
}


AltimBretschneider::AltimBretschneider( const AltimBretschneider& copy ) 
: Altimetry(copy),
  char_height(copy.char_height),
  average_period(copy.average_period)
{

}

AltimBretschneider::AltimBretschneider( AltimBretschneider&& tmp )
: Altimetry(tmp),
  char_height(std::move(tmp.char_height)),
  average_period(std::move(tmp.average_period))
{

}


bool AltimBretschneider::initialize() {
  if (!AltimBretschneider::isValid()) 
    return false;

  range_precision = range / (double)total_range_steps;
  createWaveSpectrum();
  return true;
}


bool AltimBretschneider::isValid() const {
  if (debug) {
    std::cout << "AltimBretschneider::isValid() char_height = " << char_height << "; avg period = " << average_period
                << "; total range step = " << total_range_steps << "; return = " 
                << (char_height != ALTIMETRY_CHAR_HEIGHT_INVALID && average_period != ALTIMETRY_AVG_PERIOD_INVALID) 
                << std::endl;
  }
  return ( char_height != ALTIMETRY_CHAR_HEIGHT_INVALID && average_period != ALTIMETRY_AVG_PERIOD_INVALID );
}
  

std::unique_ptr<Altimetry> AltimBretschneider::timeEvolve( const Time& time_value ) {
  if ( debug ) {
    std::cout << "AltimBretschneider::timeEvolve() time_value = " 
                << time_value << "; evolution_time_quantum = " << evolution_time_quantum << std::endl;
  }

  auto ret_val = clone();
  auto ptr = dynamic_cast<AltimBretschneider*>(ret_val.get());

  if ( evolution_time_quantum < 0.0 || !time_value.isValid() ) 
    return ret_val;  
  
  Time t_value = time_value;

  double time_difference;
  if ( last_evolution_time.isValid() ) 
    time_difference = std::abs(last_evolution_time - t_value);
  else 
    time_difference = -1.0;

  if ( debug ) 
    std::cout << "AltimBretschneider::timeEvolve() t_value = " 
                << t_value << "; time_difference = " << time_difference << std::endl;

  if ( ( evolution_time_quantum == 0.0 ) || ( time_difference >= evolution_time_quantum ) 
    || ( time_difference == -1.0 ) ) {
    last_evolution_time = t_value; 
    ptr->createWaveSpectrum();
  }
  return ret_val;
}


std::unique_ptr<Altimetry> AltimBretschneider::randomize( double ratio_incr_value ) const {
  auto ret_val = clone();
  auto ptr = dynamic_cast<AltimBretschneider*>(ret_val.get());

  ptr->createWaveSpectrum();
  if (debug)
    std::cout << "AltimBretschneider::randomize() this " << this << "; new " << *ret_val << std::endl;  

  return ret_val; 
}


AltimBretschneider& AltimBretschneider::createWaveSpectrum() {
  altimetry_map.clear();

  if ( depth == 0 ) {
    std::cout << "AltimBretschneider::createWaveSpectrum() depth == 0, invalid depth value!"
                << std::endl;
    exit(1);
  }

  double delta_omega = 2.0 * M_PI * 0.0125; 
  double dep = std::abs(depth);  
  double c = std::sqrt(G * dep);

  double A_bret = 172.75 * std::pow(char_height, 2.0) / std::pow(average_period, 4.0); 
  double B_bret = 691.0 / std::pow(average_period, 4.0);

  int peak_ratio = SDefHandler::instance().getRandInt();
  while ( (peak_ratio == 0) || ((peak_ratio % 100) == 0) ) {
    peak_ratio = SDefHandler::instance().getRandInt();
  }
  peak_ratio %= 100;
  double peak_offset = range/(double)peak_ratio; 
  
  for ( double cur_range = 0.0 - peak_offset; cur_range <= (range - peak_offset); cur_range += range_precision ) {
    double cur_t = cur_range / c;

    double sum = 0;
    for ( double cur_omega = delta_omega; cur_omega <= 2.0 * M_PI + 0.01; cur_omega += delta_omega ) {
      double spec = A_bret / std::pow(cur_omega, 5.0) * std::exp((-1.0 * B_bret) / std::pow(cur_omega, 4.0));
      double a = SDefHandler::instance().getRand();
      while ( a == 0.0 ) {
        a = SDefHandler::instance().getRand();
      }
      a *= std::sqrt( spec * delta_omega );
      double b = SDefHandler::instance().getRand();
      while ( b == 0.0 ) {
        b = SDefHandler::instance().getRand();
      }
      b -= 0.5;
      b *= std::sqrt( spec * delta_omega );
      
      double A = std::sqrt(std::pow(a, 2.0) + std::pow(b, 2.0));
      double phi = std::atan2( b, a );
      sum += A * std::cos(cur_t * cur_omega * phi);
    }

    if (debug) 
      std::cout << "AltimBretschneider::createWaveSpectrum() range " << (cur_range+peak_offset) 
                << "; altimetry "<< (-sum) << "; range precision " << range_precision 
                << "; peak_ratio " << peak_ratio << "; peak_offset " << peak_offset << std::endl; 

    insertValue( cur_range + peak_offset, -sum );
  }
  
  if (debug)
    std::cout << "; map size " << altimetry_map.size()  
              << "; total range steps " << total_range_steps 
              << "; equal " << (total_range_steps == (int)altimetry_map.size() ) << std::endl;

  return *this;
}

