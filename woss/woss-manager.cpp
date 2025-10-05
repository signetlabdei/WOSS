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
 * @file   woss-manager.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::WossManager, woss::WossManagerResDb and woss::WossManagerResDbMT classes
 *
 * Provides the implementation of woss::WossManager, woss::WossManagerResDb and woss::WossManagerResDbMT classes
 */


#include <future>
#include <iomanip>
#include <definitions-handler.h>
#include "woss-manager.h"

#ifdef WOSS_MULTITHREAD
#include <thread-pool-definitions.h>
#endif // WOSS_MULTITHREAD

using namespace woss;

const Time WossManager::NO_EVOLUTION_TIME = {1, 1, 1901 , 1, 1, 1};

WossManager::WossManager()
: woss_creator(nullptr),
  debug(false)
{

}


std::unique_ptr<TimeArr> WossManager::getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) 
    return( SDefHandler::instance().createTimeArr( TimeArr::createImpulse() ) ); // it is the same node!
    
  if ( debug ) 
    std::cout << "WossManager::getWossTimeArr() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
              << "; start freq = " << start_frequency << "; end freq = " << end_frequency << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) << std::endl; 

  auto curr_woss = getWoss( tx_coordz, rx_coordz, start_frequency, end_frequency );

  bool is_ok = curr_woss->timeEvolve(time_value);
  assert(is_ok);
  is_ok = curr_woss->run();
  assert(is_ok);
  
  if ( start_frequency == end_frequency ) 
    return( curr_woss->getTimeArr( start_frequency, tx_coordz.getDepth(), rx_coordz.getDepth(), tx_coordz.getGreatCircleDistance( rx_coordz ) ) );
  else {
    auto it = curr_woss->freq_lower_bound( start_frequency );
    auto sum = curr_woss->getTimeArr( *it, tx_coordz.getDepth(), rx_coordz.getDepth(), tx_coordz.getGreatCircleDistance( rx_coordz ) );
    it++;
    
    for( ; it == curr_woss->freq_lower_bound( end_frequency ); it++ ) {
      auto curr_time_arr = curr_woss->getTimeArr( *it, tx_coordz.getDepth(), rx_coordz.getDepth(), tx_coordz.getGreatCircleDistance( rx_coordz ) );
      *sum += *curr_time_arr;
      curr_time_arr.reset();
    }
    return sum;
  }
}


std::unique_ptr<TimeArr> WossManager::getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) 
    return( SDefHandler::instance().createTimeArr( TimeArr::createImpulse() ) ); // it is the same node!

  SimTime sim_time = woss_creator->getSimTime(tx_coordz,rx_coordz);
  if ( sim_time.start_time.isValid() ) {
    Time time = sim_time.start_time + (time_t)time_value;
    
    if (debug) 
      std::cout << "WossManager::getWossTimeArr() time value in seconds = " << time_value
                << "; start sime time = " << sim_time.start_time << "; computed time = " << time << std::endl;
    
    return getWossTimeArr(tx_coordz, rx_coordz, start_frequency, end_frequency, time );
  }
  else {
    std::cout << "WossManager::getWossTimeArr() WARNING, invalid start time for tx = " << tx_coordz << "; rx = " 
              << rx_coordz << std::endl;
                
    return nullptr;
  }
}


std::unique_ptr<Pressure> WossManager::getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) 
    return( SDefHandler::instance().createPressure(1.0, 0) ); // it is the same node!
    
  if (debug) 
    std::cout << "WossManager::getWossPressure() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
              << "; start freq = " << start_frequency << "; end freq = " << end_frequency << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) << std::endl; 

  auto curr_woss = getWoss( tx_coordz, rx_coordz, start_frequency, end_frequency );
  
  bool is_ok = curr_woss->timeEvolve(time_value);
  assert(is_ok);
  is_ok = curr_woss->run();
  assert(is_ok);
  
  if ( start_frequency == end_frequency ) 
    return( curr_woss->getPressure( start_frequency, tx_coordz.getDepth(), rx_coordz.getDepth(), tx_coordz.getGreatCircleDistance( rx_coordz ) ) );
  else {
    auto it = curr_woss->freq_lower_bound( start_frequency );
    auto sum_avg = curr_woss->getAvgPressure( *it, tx_coordz.getDepth(), rx_coordz.getDepth(), tx_coordz.getGreatCircleDistance( rx_coordz ) );
    it++;

    for( ; it == curr_woss->freq_lower_bound( end_frequency ); it++ ) {
      auto curr_press = curr_woss->getAvgPressure( *it, tx_coordz.getDepth() );
      *sum_avg += *curr_press;
      curr_press.reset();
    }
    return sum_avg;
  }
}


std::unique_ptr<Pressure> WossManager::getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) 
    return( SDefHandler::instance().createPressure(1.0, 0) ); // it is the same node!

  SimTime sim_time = woss_creator->getSimTime(tx_coordz,rx_coordz);
  if ( sim_time.start_time.isValid() ) {
    Time time = sim_time.start_time + (time_t)time_value;

    if (debug) 
      std::cout << "WossManager::getWossPressure() time value in seconds = " << time_value
                << "; start sime time = " << sim_time.start_time << "; computed time = " << time << std::endl;

    return getWossPressure(tx_coordz, rx_coordz, start_frequency, end_frequency, time );
  }
  else {
    std::cout << "WossManager::getWossPressure() WARNING, invalid start time for tx = " << tx_coordz << "; rx = " 
              << rx_coordz << std::endl;

    return nullptr;
  }
}


PressureVector WossManager::getWossPressure( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, const Time& time_value ) {
  PressureVector ret_value;

  for ( const auto& it : coordinates ) {
    ret_value.emplace_back( getWossPressure( it.first, it.second, start_frequency, end_frequency, time_value ) );
  }
  return ret_value;
}


PressureVector WossManager::getWossPressure( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, double time_value ) {
  PressureVector ret_value;

  for ( const auto& it : coordinates ) {
    SimTime sim_time = woss_creator->getSimTime(it.first, it.second);

    if ( sim_time.start_time.isValid() ) {
      Time time = sim_time.start_time + (time_t)time_value;
 
      if (debug) 
        std::cout << "WossManager::getWossPressure() time value in seconds = " << time_value
                  << "; start sime time = " << sim_time.start_time << "; computed time = " << time << std::endl;

      ret_value.emplace_back( getWossPressure( it.first, it.second, start_frequency, end_frequency, time_value ) );
    }
    else {
      std::cout << "WossManager::getWossPressure() WARNING, invalid start time for tx = " << it.first << "; rx = " 
                << it.second << std::endl;

      ret_value.emplace_back( SDefHandler::instance().createPressure( Pressure::createNotValid() ) );
    }
  }
  return ret_value; 
}


TimeArrVector WossManager::getWossTimeArr( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, const Time& time_value ) {
  TimeArrVector ret_value;
  
  for ( const auto& it : coordinates ) {
    ret_value.emplace_back( getWossTimeArr( it.first, it.second, start_frequency, end_frequency, time_value ) );
  }
  return ret_value;
}


TimeArrVector WossManager::getWossTimeArr( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, double time_value ) {
  TimeArrVector ret_value;

  for ( const auto& it : coordinates ) {
    SimTime sim_time = woss_creator->getSimTime(it.first, it.second);
    
    if ( sim_time.start_time.isValid() ) {
      Time time = sim_time.start_time + (time_t)time_value;
      
      if (debug) 
        std::cout << "WossManager::getWossTimeArr() time value in seconds = " << time_value
                  << "; start sime time = " << sim_time.start_time << "; computed time = " << time << std::endl;

      ret_value.emplace_back( getWossTimeArr( it.first, it.second, start_frequency, end_frequency, time_value ) );
    }
    else {
      std::cout << "WossManager::getWossTimeArr() WARNING, invalid start time for tx = " << it.first << "; rx = " 
                << it.second << std::endl;

      ret_value.emplace_back( SDefHandler::instance().createTimeArr( TimeArr::createNotValid() ) );
    }
  }
  return ret_value; 
}


std::shared_ptr<Woss> WossManager::getActiveWoss( const CoordZ& tx, const CoordZ& rx, double start_frequency, double end_frequency ) {
  return const_cast< woss::WossManager* >( this )->getWoss( tx, rx, start_frequency, end_frequency );
}


/////////
WossManagerResDb::WossManagerResDb()
:  woss_db_manager(nullptr)
{

}


std::unique_ptr<TimeArr> WossManagerResDb::getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) 
    return( SDefHandler::instance().createTimeArr( TimeArr::createImpulse() ) ); // it is the same node!
   
  if ( debug ) 
    std::cout << "WossManagerResDb::getWossTimeArr() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
              << "; start freq = " << start_frequency << "; end freq = " << end_frequency << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) << std::endl; 

  bool valid = true;

  double freq_step = woss_creator->getFrequencyStep(tx_coordz,rx_coordz);
  int i = 0;
  const Time* time = nullptr;
  
  if ( is_time_evolution_active == false ) {
    time = &NO_EVOLUTION_TIME;
  }
  else 
    time = &time_value;
  
  auto sum = dbGetTimeArr( tx_coordz, rx_coordz, (start_frequency + ((double)i) * freq_step ), *time );
  
  if ( debug && sum != nullptr ) 
    std::cout << "WossManagerResDb::getWossTimeArr() first TimeArr in db " << *sum << std::endl; 
  
  i++;
  valid = valid && sum->isValid();
  
  for( ; i <= floor( ( end_frequency - start_frequency) / freq_step ); i++ ) {
      auto curr_time_arr = dbGetTimeArr( tx_coordz, rx_coordz, (start_frequency + ((double)i) * freq_step ), *time );
      valid = valid && curr_time_arr->isValid();

      if ( debug && curr_time_arr != nullptr ) 
        std::cout << "WossManagerResDb::getWossTimeArr() " << i << "-th TimeArr in db" << *curr_time_arr << std::endl; 

      if (!valid) { 
        break;
      }
      *sum += *curr_time_arr;

      if ( debug && sum != nullptr ) 
        std::cout << "WossManagerResDb::getWossTimeArr() sum TimeArr " << *sum << std::endl; 

      curr_time_arr.reset();
  }
  if (valid) 
    return sum;
  
  sum->clear();

  bool is_ok = true;
  auto curr_woss = getWoss( tx_coordz, rx_coordz, start_frequency, end_frequency );

  if ( curr_woss->timeEvolve(time_value) ) 
    is_ok = curr_woss->run();
  assert(is_ok);
  
  for( auto it = curr_woss->freq_lower_bound( start_frequency ); it == curr_woss->freq_lower_bound( end_frequency ); it++ ) {
    auto curr_time_arr = curr_woss->getTimeArr( *it, tx_coordz.getDepth(), rx_coordz.getDepth(), tx_coordz.getGreatCircleDistance( rx_coordz ) ) ; 

    assert(curr_time_arr != nullptr);

    if ( debug ) 
      std::cout << "WossManagerResDb::getWossTimeArr() " << i << "-th TimeArr " << *curr_time_arr << std::endl; 

    dbInsertTimeArr( tx_coordz, rx_coordz, *it, *time, *curr_time_arr );
    *sum += *curr_time_arr;
    curr_time_arr.reset();
  }

  if ( debug && sum != nullptr ) 
    std::cout << "WossManagerResDb::getWossTimeArr() final TimeArr " << *sum << std::endl; 

  return sum; 
}


std::unique_ptr<Pressure> WossManagerResDb::getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) 
    return( SDefHandler::instance().createPressure(1.0, 0) ); // it is the same node!
  
  if ( debug ) 
    std::cout << "WossManagerResDb::getWossPressure() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
              << "; start freq = " << start_frequency << "; end freq = " << end_frequency << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) << std::endl; 
  
  bool valid = true;
  double freq_step = woss_creator->getFrequencyStep(tx_coordz,rx_coordz);
  int i = 0;
 
  const Time* time = nullptr;

  if ( is_time_evolution_active == false ) {
    time = &NO_EVOLUTION_TIME;
  }
  else 
    time = &time_value;

  auto press_temp = dbGetPressure( tx_coordz, rx_coordz, (start_frequency + ((double)i) * freq_step ), *time );
  
  // create a time arr
  auto sum_avg = SDefHandler::instance().createTimeArr( *press_temp );

  valid = valid && sum_avg->isValid();

  for( ; i <= floor( ( end_frequency - start_frequency) / freq_step ); i++ ) {
    auto curr_press = dbGetPressure( tx_coordz, rx_coordz, (start_frequency + ((double)i) * freq_step), *time );
    
    valid = valid && curr_press->isValid();
    if (!valid) {
      break;
    }
    *sum_avg += TimeArr(*curr_press);
    curr_press.reset();
  }
  if (valid) {
    auto ret_val = SDefHandler::instance().createPressure( *sum_avg );
    return ret_val;
  }

  sum_avg->clear();

  bool is_ok = false;
  auto curr_woss = getWoss( tx_coordz, rx_coordz, start_frequency, end_frequency );
  
  if ( curr_woss->timeEvolve(time_value) ) 
    is_ok = curr_woss->run();
  assert(is_ok);
  
  for( auto it = curr_woss->freq_lower_bound( start_frequency ); it == curr_woss->freq_lower_bound( end_frequency ); it++ ) {
    auto curr_press = curr_woss->getAvgPressure( *it, tx_coordz.getDepth() ) ; 
    dbInsertPressure( tx_coordz, rx_coordz, *it, *time, *curr_press );
    *sum_avg += TimeArr(*curr_press);
    curr_press.reset();
  }

  auto ret_val = SDefHandler::instance().createPressure( *sum_avg );
  return ret_val;
}

#ifdef WOSS_MULTITHREAD
WossManagerResDbMT::WossManagerResDbMT() 
: max_thread_number(0),
  concurrent_threads(0),
  request_mutex(),
  active_woss(),
  use_thread_pool(false)
{
  // can be zero!
  max_thread_number = std::thread::hardware_concurrency();
  if ( max_thread_number > MAX_TOTAL_THREADS )
    max_thread_number = MAX_TOTAL_THREADS;

  checkConcurrentThreads();
}


void WossManagerResDbMT::checkConcurrentThreads() {
  if ( concurrent_threads == 0 ) 
    concurrent_threads = max_thread_number;
  else 
    concurrent_threads = std::min( max_thread_number, concurrent_threads );

  if (debug)
    std::cout << "WossManagerResDbMT::checkConcurrentThreads() " << concurrent_threads << std::endl;
}


PressureVector WossManagerResDbMT::getWossPressure( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( concurrent_threads == 0 ) 
    return WossManager::getWossPressure( coordinates, start_frequency, end_frequency, time_value );

  PressureVector replies;
  std::vector< std::future< std::unique_ptr<Pressure> > > futures;
  unsigned int total_queries = coordinates.size();

  if (use_thread_pool == false)
  {
    unsigned int cnt = 0;
    auto coord_iter = coordinates.cbegin();
    unsigned int total_thread_created = 0;
    unsigned int total_thread_ended = 0;

    while (coord_iter != coordinates.cend()) {
      for ( unsigned int cnt = 0; cnt < concurrent_threads && coord_iter != coordinates.cend(); cnt++, coord_iter++) {
        futures.emplace_back( std::async( std::launch::async, [this]( const CoordZ& tx, const CoordZ& rx, double sf, double ef, const Time& tv ) 
                                                              { 
                                                                return this->getWossPressure(tx, rx, sf, ef, tv);
                                                              }, coord_iter->first, coord_iter->second, start_frequency, end_frequency, time_value) );
        total_thread_created++;
      }

      for (auto& val : futures) {
        replies.emplace_back( val.get() );
        total_thread_ended++;
      }
      futures.clear();
    }

    if (debug)
      std::cout << "WossManagerResDbMT::getWossPressure() total queries = " << total_queries 
                << "; concurrent threads = " << concurrent_threads << "; total_thread_created = " 
                << total_thread_created << "; total thread ended = "<< total_thread_ended << std::endl; 

    assert( total_thread_ended == total_thread_created );
    assert( total_thread_ended == total_queries );
  }
  else {
    ThreadPool pool(concurrent_threads);

    for (const auto& coord_iter : coordinates) {
      futures.emplace_back(
        pool.submit([this] ( const CoordZ& tx, const CoordZ& rx, double sf, double ef, const Time& tv ) 
                      { 
                        return this->getWossPressure(tx, rx, sf, ef, tv);
                      }, 
                      coord_iter.first, coord_iter.second, start_frequency, end_frequency, time_value
                    )
        );
    }

    for (auto& val : futures) {
      replies.emplace_back( val.get() );
    }
    futures.clear();

    if (debug)
      std::cout << "WossManagerResDbMT::getWossPressure() total queries = " << total_queries 
                << "; total futures = " << replies.size() 
                << "; concurrent threads = " << concurrent_threads << std::endl;
  }
  return replies;
}


PressureVector WossManagerResDbMT::getWossPressure( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, double time_value ) {
  if ( concurrent_threads == 0 ) 
    return WossManager::getWossPressure( coordinates, start_frequency, end_frequency, time_value );

  PressureVector replies;
  std::vector< std::future< std::unique_ptr<Pressure> > > futures;
  unsigned int total_queries = coordinates.size();

  if (use_thread_pool == false) {
    unsigned int cnt = 0;
    auto coord_iter = coordinates.cbegin();
    unsigned int total_thread_created = 0;
    unsigned int total_thread_ended = 0;

    while (coord_iter != coordinates.cend()) {
      for ( unsigned int cnt = 0; cnt < concurrent_threads && coord_iter != coordinates.cend(); cnt++, coord_iter++) {
        SimTime sim_time = getWossCreator()->getSimTime(coord_iter->first, coord_iter->second);
        if ( sim_time.start_time.isValid() ) {
          Time time = sim_time.start_time + (time_t)time_value;
          futures.emplace_back( std::async( std::launch::async, [this]( const CoordZ& tx, const CoordZ& rx, double sf, double ef, const Time& tv ) 
                                                                { 
                                                                  return this->getWossPressure(tx, rx, sf, ef, tv);
                                                                }, coord_iter->first, coord_iter->second, start_frequency, end_frequency, time) );
          total_thread_created++;
        }
        else {
          std::cout << "WossManagerResDbMT::getWossPressure() WARNING, invalid start time for tx = " << coord_iter->first << "; rx = " 
                    << coord_iter->second << std::endl;
        }
      }

      for (auto& val : futures) {
        replies.emplace_back( val.get() );
        total_thread_ended++;
      }
      futures.clear();
    }

    if (debug)
      std::cout << "WossManagerResDbMT::getWossPressure() total queries = " << total_queries 
                << "; concurrent threads = " << concurrent_threads << "; total_thread_created = " 
                << total_thread_created << "; total thread ended = "<< total_thread_ended << std::endl; 

    assert( total_thread_ended == total_thread_created );
    assert( total_thread_ended == total_queries );
  }
  else {
    ThreadPool pool(concurrent_threads);

    for (const auto& coord_iter : coordinates) {
      SimTime sim_time = getWossCreator()->getSimTime(coord_iter.first, coord_iter.second);
      if ( sim_time.start_time.isValid() ) {
        Time time = sim_time.start_time + (time_t)time_value;
        futures.emplace_back(
          pool.submit([this] ( const CoordZ& tx, const CoordZ& rx, double sf, double ef, const Time& tv ) 
                        { 
                          return this->getWossPressure(tx, rx, sf, ef, tv);
                        }, 
                        coord_iter.first, coord_iter.second, start_frequency, end_frequency, time
                     ) 
          );
      }
      else {
        std::cout << "WossManagerResDbMT::getWossPressure() WARNING, invalid start time for tx = " << coord_iter.first << "; rx = " 
                  << coord_iter.second << std::endl;
      }
    }

    for (auto& val : futures) {
      replies.emplace_back( val.get() );
    }
    futures.clear();

    if (debug)
      std::cout << "WossManagerResDbMT::getWossPressure() total queries = " << total_queries 
                << "; total futures = " << replies.size() 
                << "; concurrent threads = " << concurrent_threads << std::endl;
  }

  return replies;
}


TimeArrVector WossManagerResDbMT::getWossTimeArr( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( concurrent_threads == 0 )
    return WossManager::getWossTimeArr( coordinates, start_frequency, end_frequency, time_value );

  TimeArrVector replies;
  std::vector< std::future< std::unique_ptr<TimeArr> > > futures;
  unsigned int total_queries = coordinates.size();

  if (use_thread_pool == false) {
    unsigned int cnt = 0;
    auto coord_iter = coordinates.cbegin();
    unsigned int total_thread_created = 0;
    unsigned int total_thread_ended = 0;

    while (coord_iter != coordinates.cend()) {
      for ( unsigned int cnt = 0; cnt < concurrent_threads && coord_iter != coordinates.cend(); cnt++, coord_iter++) {
        futures.emplace_back( std::async( std::launch::async, [this]( const CoordZ& tx, const CoordZ& rx, double sf, double ef, const Time& tv ) 
                                                              { 
                                                                return this->getWossTimeArr(tx, rx, sf, ef, tv);
                                                              }, coord_iter->first, coord_iter->second, start_frequency, end_frequency, time_value) );
        total_thread_created++;
      }

      for (auto& val : futures) {
        replies.emplace_back( val.get() );
        total_thread_ended++;
      }
      futures.clear();
    }

    if (debug)
      std::cout << "WossManagerResDbMT::getWossTimeArr() total queries = " << total_queries 
                << "; concurrent threads = " << concurrent_threads << "; total_thread_created = " 
                << total_thread_created << "; total thread ended = "<< total_thread_ended << std::endl; 

    assert( total_thread_ended == total_thread_created );
    assert( total_thread_ended == total_queries );
  }
  else {
    ThreadPool pool(concurrent_threads);

    for (const auto& coord_iter : coordinates) {
      futures.emplace_back(
        pool.submit([this] ( const CoordZ& tx, const CoordZ& rx, double sf, double ef, const Time& tv ) 
                      { 
                        return this->getWossTimeArr(tx, rx, sf, ef, tv);
                      }, 
                      coord_iter.first, coord_iter.second, start_frequency, end_frequency, time_value
                    )
        );
    }

    for (auto& val : futures) {
      replies.emplace_back( val.get() );
    }
    futures.clear();

    if (debug)
      std::cout << "WossManagerResDbMT::getWossTimeArr() total queries = " << total_queries 
                << "; total futures = " << replies.size() 
                << "; concurrent threads = " << concurrent_threads << std::endl;
  }

  return replies;
}


TimeArrVector WossManagerResDbMT::getWossTimeArr( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, double time_value ) {
  if ( concurrent_threads == 0 ) 
    return WossManager::getWossTimeArr( coordinates, start_frequency, end_frequency, time_value );

  TimeArrVector replies;
  std::vector< std::future< std::unique_ptr<TimeArr> > > futures;
  unsigned int total_queries = coordinates.size();

  if (use_thread_pool == false) {
    unsigned int cnt = 0;
    auto coord_iter = coordinates.cbegin();
    unsigned int total_thread_created = 0;
    unsigned int total_thread_ended = 0;

    while (coord_iter != coordinates.cend()) {
      for ( unsigned int cnt = 0; cnt < concurrent_threads && coord_iter != coordinates.cend(); cnt++, coord_iter++) {
        SimTime sim_time = getWossCreator()->getSimTime(coord_iter->first, coord_iter->second);
        if ( sim_time.start_time.isValid() ) {
          Time time = sim_time.start_time + (time_t)time_value;
          futures.emplace_back( std::async( std::launch::async, [this]( const CoordZ& tx, const CoordZ& rx, double sf, double ef, const Time& tv ) 
                                                                { 
                                                                  return this->getWossTimeArr(tx, rx, sf, ef, tv);
                                                                }, coord_iter->first, coord_iter->second, start_frequency, end_frequency, time) );
          total_thread_created++;
        }
        else {
          std::cout << "WossManagerResDbMT::getWossTimeArr() WARNING, invalid start time for tx = " << coord_iter->first << "; rx = " 
                    << coord_iter->second << std::endl;
        }
      }

      for (auto& val : futures) {
        replies.emplace_back( val.get() );
        total_thread_ended++;
      }
      futures.clear();
    }

    if (debug)
      std::cout << "WossManagerResDbMT::getWossTimeArr() total queries = " << total_queries 
                << "; concurrent threads = " << concurrent_threads << "; total_thread_created = " 
                << total_thread_created << "; total thread ended = "<< total_thread_ended << std::endl; 

    assert( total_thread_ended == total_thread_created );
    assert( total_thread_ended == total_queries );
  }
  else {
    ThreadPool pool(concurrent_threads);

    for (const auto& coord_iter : coordinates) {
      SimTime sim_time = getWossCreator()->getSimTime(coord_iter.first, coord_iter.second);
      if ( sim_time.start_time.isValid() ) {
        Time time = sim_time.start_time + (time_t)time_value;
        futures.emplace_back(
          pool.submit([this] ( const CoordZ& tx, const CoordZ& rx, double sf, double ef, const Time& tv ) 
                        { 
                          return this->getWossTimeArr(tx, rx, sf, ef, tv);
                        }, 
                        coord_iter.first, coord_iter.second, start_frequency, end_frequency, time
                     ) 
          );
      }
      else {
        std::cout << "WossManagerResDbMT::getWossTimeArr() WARNING, invalid start time for tx = " << coord_iter.first << "; rx = " 
                  << coord_iter.second << std::endl;
      }
    }

    for (auto& val : futures) {
      replies.emplace_back( val.get() );
    }
    futures.clear();

    if (debug)
      std::cout << "WossManagerResDbMT::getWossTimeArr() total queries = " << total_queries 
                << "; total futures = " << replies.size() 
                << "; concurrent threads = " << concurrent_threads << std::endl;
  }

  return replies;
}


std::unique_ptr<TimeArr> WossManagerResDbMT::getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) 
    return( SDefHandler::instance().createTimeArr( TimeArr::createImpulse() ) ); // it is the same node!
   
  if ( concurrent_threads == 0 ) 
    return WossManagerResDb::getWossTimeArr( tx_coordz, rx_coordz, start_frequency, end_frequency, time_value );
  
  if ( debug ) 
    std::cout << "WossManagerResDbMT::getWossTimeArr() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
              << "; start freq = " << start_frequency << "; end freq = " << end_frequency 
              << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) << std::flush
              << "; time_value = " << time_value << std::endl; 

  bool valid = true;

  std::unique_lock lock_request( request_mutex );

  double freq_step = woss_creator->getFrequencyStep( tx_coordz, rx_coordz );

  const Time* time = nullptr;

  if ( is_time_evolution_active == false ) {
    time = &NO_EVOLUTION_TIME;
  }
  else 
    time = &time_value;

  auto sum = dbGetTimeArr( tx_coordz, rx_coordz, start_frequency, *time );

  if ( debug && sum != nullptr ) 
    std::cout << "WossManagerResDbMT::getWossTimeArr() first TimeArr in db " << *sum << std::endl; 

  valid = valid && sum->isValid();

  for( int i = 1; i <= floor( ( end_frequency - start_frequency) / freq_step ); i++ ) {
    auto curr_time_arr = dbGetTimeArr( tx_coordz, rx_coordz, (start_frequency + ((double)i) * freq_step ), *time );
    valid = valid && curr_time_arr->isValid();

    if (valid == false) {
      break;
    }
    *sum += *curr_time_arr;
    curr_time_arr.reset();
  }
  lock_request.unlock();

  if (valid) {
    if ( debug ) 
      std::cout << "WossManagerResDbMT::getWossTimeArr() valid TimeArr in db found." << std::endl;

    return sum;
  }
  
  sum->clear();

  if ( debug ) 
    std::cout << "WossManagerResDbMT::getWossTimeArr() NO valid TimeArr in db found" 
              << ", getting a Woss object." << std::endl;

  lock_request.lock();
  auto curr_woss = getWoss( tx_coordz, rx_coordz, start_frequency, end_frequency );

  if ( curr_woss->isRunning() ) {
    auto it = active_woss.find( curr_woss );

    if ( it != active_woss.end() ) {
      if ( debug ) 
        std::cout << "WossManagerResDbMT::getWossTimeArr() curr Woss is running." 
                  << " Waiting for it to finish..."<< std::endl;

      assert( it->second != nullptr );

      lock_request.unlock();

      // take unique lock on woss-related mutex
      std::unique_lock curr_lock(it->second->mutex);

      // capture std::shared_ptr<Woss> by reference
      it->second->condition.wait(curr_lock, [&curr_woss] { return curr_woss->isRunning() == false; });
    }
  }
  else {
    bool has_to_run = curr_woss->timeEvolve(time_value);
    bool is_ok = true;

    if ( has_to_run ) {
      active_woss[curr_woss] = std::move( std::make_unique<ThreadCondSignal>() );
    }
    lock_request.unlock();

    if ( has_to_run ) 
      is_ok = curr_woss->run();
    assert( is_ok );

    if ( debug ) 
      std::cout << "WossManagerResDbMT::getWossTimeArr() curr Woss was active and has now run." 
                << std::endl;
  }
  
  if ( debug ) 
    std::cout << "WossManagerResDbMT::getWossTimeArr() curr Woss object has run." << std::endl;

  lock_request.lock();

  for( auto it = curr_woss->freq_lower_bound( start_frequency ); it == curr_woss->freq_lower_bound( end_frequency ); ++it ) {
    auto curr_time_arr = curr_woss->getTimeArr( *it, tx_coordz.getDepth(), rx_coordz.getDepth(), tx_coordz.getGreatCircleDistance( rx_coordz ) ) ;
    dbInsertTimeArr( tx_coordz, rx_coordz, *it, *time, *curr_time_arr );
    *sum += *curr_time_arr;
    curr_time_arr.reset();
  }

  if ( debug ) 
    std::cout << "WossManagerResDbMT::getWossTimeArr() TimeArr computed = " << *sum << std::endl;

  auto it = active_woss.find( curr_woss ); 
  if ( it != active_woss.end() ) {
    assert( it->second != nullptr );
    it->second->condition.notify_all();
    active_woss.erase(it);
  }

  return sum;
}


std::unique_ptr<TimeArr> WossManagerResDbMT::getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) 
    return( SDefHandler::instance().createTimeArr( TimeArr::createImpulse() ) ); // it is the same node!

  if ( debug )
    std::cout << "WossManagerResDbMT::getWossTimeArr() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
              << "; start freq = " << start_frequency << "; end freq = " << end_frequency << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) 
              << "; time_value = " << time_value << std::endl; 

  SimTime sim_time = woss_creator->getSimTime(tx_coordz,rx_coordz);
  if ( sim_time.start_time.isValid() ) {
    Time time = sim_time.start_time + (time_t)time_value;

    if ( debug )
      std::cout << "WossManagerResDbMT::getWossTimeArr() time converted = " << time << std::endl;

    return getWossTimeArr( tx_coordz, rx_coordz, start_frequency, end_frequency, time );
  }
  else {
    std::cout << "WossManagerResDbMT::getWossTimeArr() WARNING, invalid start time for tx = " << tx_coordz << "; rx = " 
              << rx_coordz << std::endl;

    return nullptr;
  }
}


std::unique_ptr<Pressure> WossManagerResDbMT::getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) 
    return( SDefHandler::instance().createPressure(1.0, 0) ); // it is the same node!

  if ( concurrent_threads == 0 ) 
    return WossManagerResDb::getWossPressure( tx_coordz, rx_coordz, start_frequency, end_frequency, time_value );

  if ( debug ) 
    std::cout << "WossManagerResDbMT::getWossPressure() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
              << "; start freq = " << start_frequency << "; end freq = " << end_frequency 
              << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) 
              << "; time_value = " << time_value << std::endl; 

  bool valid = true;

  std::unique_lock lock_request( request_mutex );

  double freq_step = woss_creator->getFrequencyStep(tx_coordz,rx_coordz);
  const Time* time = nullptr;

  if ( is_time_evolution_active == false ) {
    time = &NO_EVOLUTION_TIME;
  }
  else 
    time = &time_value;

  auto temp = dbGetPressure( tx_coordz, rx_coordz, start_frequency, *time );
  auto sum_avg = SDefHandler::instance().createTimeArr( *temp );

  valid = valid && sum_avg->isValid();

  int i = 1;

  for( ; i <= floor( ( end_frequency - start_frequency) / freq_step ); i++ ) {
    auto curr_press = dbGetPressure( tx_coordz, rx_coordz, (start_frequency + ((double)i) * freq_step), *time );

    valid = valid && curr_press->isValid();
    if (!valid) {
      break;
    }
    *sum_avg += TimeArr(*curr_press);
    curr_press.reset();
  }
  lock_request.unlock();

  if (valid) {
    auto ret_val = SDefHandler::instance().createPressure( *sum_avg );

    if ( debug ) 
      std::cout << "WossManagerResDbMT::getWossPressure() valid Pressure in db found." << std::endl;

    return ret_val;
  }
  
  sum_avg->clear();

  if ( debug ) 
    std::cout << "WossManagerResDbMT::getWossPressure() NO valid Pressure in db found." 
              << ", getting a Woss object." << std::endl;
  
  lock_request.lock();
  auto curr_woss = getWoss( tx_coordz, rx_coordz, start_frequency, end_frequency );

  if ( curr_woss->isRunning() ) {
    auto it = active_woss.find( curr_woss );

    if ( it != active_woss.end() ) {
      if ( debug ) 
        std::cout << "WossManagerResDbMT::getWossPressure() curr Woss is running." 
                  << " Waiting for it to finish..."<< std::endl;

      assert( it->second != nullptr );

      lock_request.unlock();

      std::unique_lock curr_lock(it->second->mutex);

      // capture std::shared_ptr<Woss> by reference
      it->second->condition.wait(curr_lock, [&curr_woss] { return curr_woss->isRunning() == false; });
    }
  }
  else {
    bool has_to_run = curr_woss->timeEvolve(time_value);
    bool is_ok = true;

    if ( has_to_run ) {
      active_woss[curr_woss] = std::move( std::make_unique<ThreadCondSignal>() );
    }
    lock_request.unlock();

    if ( has_to_run ) 
      is_ok = curr_woss->run();
    assert( is_ok );

    if ( debug )
      std::cout << "WossManagerResDbMT::getWossPressure() curr Woss was active and has now run." 
                << std::endl;
  } 
  
  if ( debug )
    std::cout << "WossManagerResDbMT::getWossPressure() curr Woss object has run." << std::endl;

  lock_request.lock();

  for( auto it = curr_woss->freq_lower_bound( start_frequency ); it == curr_woss->freq_lower_bound( end_frequency ); it++ ) {
    auto curr_press = curr_woss->getAvgPressure( *it, tx_coordz.getDepth() ) ; 
    dbInsertPressure( tx_coordz, rx_coordz, *it, *time, *curr_press );
    *sum_avg += TimeArr(*curr_press);
    curr_press.reset();
  }

  auto it = active_woss.find( curr_woss );
  if ( it != active_woss.end() ) {
    assert( it->second != nullptr );
    it->second->condition.notify_all();
    active_woss.erase(it);
  }

  auto ret_value = SDefHandler::instance().createPressure( *sum_avg );

  if ( debug ) 
    std::cout << "WossManagerResDbMT::getWossPressure() Pressure computed = " << *ret_value << std::endl;

  return( ret_value );
}


std::unique_ptr<Pressure> WossManagerResDbMT::getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) 
    return( SDefHandler::instance().createPressure(1.0, 0) ); // it is the same node!

  if ( debug ) 
    std::cout << "WossManagerResDbMT::getWossPressure() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
              << "; start freq = " << start_frequency << "; end freq = " << end_frequency 
              << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) 
              << "; time_value = " << time_value << std::endl;

  SimTime sim_time = woss_creator->getSimTime(tx_coordz,rx_coordz);
  if ( sim_time.start_time.isValid() ) {
    Time time = sim_time.start_time + (time_t)time_value;

    if ( debug )
      std::cout << "WossManagerResDbMT::getWossPressure() time converted = " << time << std::endl;

    return getWossPressure(tx_coordz, rx_coordz, start_frequency, end_frequency, time );
  }
  else {
    std::cout << "WossManagerResDbMT::getWossPressure() WARNING, invalid start time for tx = " << tx_coordz << "; rx = " 
              << rx_coordz << std::endl;

    return nullptr;
  }
}

#endif // WOSS_MULTITHREAD
