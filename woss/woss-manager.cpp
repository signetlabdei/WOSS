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
 * @file   woss-manager.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the implementation of woss::WossManager, woss::WossManagerResDb and woss::WossManagerResDbMT classes
 *
 * Provides the implementation of woss::WossManager, woss::WossManagerResDb and woss::WossManagerResDbMT classes
 */


#include <iomanip>
#include <time-arrival-definitions.h>
#include <definitions-handler.h>
#include "woss-manager.h"


#ifdef WOSS_MULTITHREAD
#include <pthread.h>
#include <unistd.h>
#endif //WOSS_MULTITHREAD


using namespace woss;


const Time WossManager::NO_EVOLUTION_TIME = Time(1,1,1901,1,1,1);

WossManager::WossManager()
: woss_creator(NULL),
  debug(false)
{

}


WossManager::~WossManager() {
//   delete woss_creator;
}


TimeArr* WossManager::getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) return( SDefHandler::instance()->getTimeArr()->create( TimeArr::createImpulse() ) ); // it is the same node!
    
  if ( debug ) ::std::cout << "WossManager::getWossTimeArr() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
                           << "; start freq = " << start_frequency << "; end freq = " << end_frequency << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) << ::std::endl; 
 
  Woss* const curr_woss = getWoss( tx_coordz, rx_coordz, start_frequency, end_frequency );
    
  bool is_ok = curr_woss->timeEvolve(time_value);
  assert(is_ok);
  is_ok = curr_woss->run();
  assert(is_ok);
  
  if ( start_frequency == end_frequency ) return( curr_woss->getTimeArr( start_frequency, tx_coordz.getDepth(), rx_coordz.getDepth(), tx_coordz.getGreatCircleDistance( rx_coordz ) ) );
  else {
    FreqSIt it = curr_woss->freq_lower_bound( start_frequency );
    TimeArr* sum = curr_woss->getTimeArr( *it, tx_coordz.getDepth(), rx_coordz.getDepth(), tx_coordz.getGreatCircleDistance( rx_coordz ) );
    it++;
    
    TimeArr* curr_time_arr = NULL;
    for( ; it == curr_woss->freq_lower_bound( end_frequency ); it++ ) {
      curr_time_arr = curr_woss->getTimeArr( *it, tx_coordz.getDepth(), rx_coordz.getDepth(), tx_coordz.getGreatCircleDistance( rx_coordz ) );
      *sum += *curr_time_arr;
      delete curr_time_arr;
      curr_time_arr = NULL;
    }
    return sum;   
  }
}


TimeArr* WossManager::getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) return( SDefHandler::instance()->getTimeArr()->create( TimeArr::createImpulse() ) ); // it is the same node!

  SimTime sim_time = woss_creator->getSimTime(tx_coordz,rx_coordz);
  if ( sim_time.start_time.isValid() ) {
    Time time = sim_time.start_time + (time_t)time_value;
    
    if (debug) ::std::cout << "WossManager::getWossTimeArr() time value in seconds = " << time_value
                           << "; start sime time = " << sim_time.start_time << "; computed time = " << time << ::std::endl;
    
    return getWossTimeArr(tx_coordz, rx_coordz, start_frequency, end_frequency, time );
  }
  else {
    ::std::cout << "WossManager::getWossTimeArr() WARNING, invalid start time for tx = " << tx_coordz << "; rx = " 
                << rx_coordz << ::std::endl;
                
    return NULL;
  }
}


Pressure* WossManager::getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) return( SDefHandler::instance()->getPressure()->create(1.0, 0) ); // it is the same node!
    
  if (debug) ::std::cout << "WossManager::getWossPressure() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
                       << "; start freq = " << start_frequency << "; end freq = " << end_frequency << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) << ::std::endl; 

  Woss* const curr_woss = getWoss( tx_coordz, rx_coordz, start_frequency, end_frequency );
  
  bool is_ok = curr_woss->timeEvolve(time_value);
  assert(is_ok);
  is_ok = curr_woss->run();
  assert(is_ok);
  
  if ( start_frequency == end_frequency ) return( curr_woss->getPressure( start_frequency, tx_coordz.getDepth(), rx_coordz.getDepth(), tx_coordz.getGreatCircleDistance( rx_coordz ) ) );
  else {
    FreqSIt it = curr_woss->freq_lower_bound( start_frequency );
    Pressure* sum_avg = curr_woss->getAvgPressure( *it, tx_coordz.getDepth(), rx_coordz.getDepth(), tx_coordz.getGreatCircleDistance( rx_coordz ) );
    it++;
    
    Pressure* curr_press = NULL;
    for( ; it == curr_woss->freq_lower_bound( end_frequency ); it++ ) {
      curr_press = curr_woss->getAvgPressure( *it, tx_coordz.getDepth() );
      *sum_avg += *curr_press;
      delete curr_press;
      curr_press = NULL;
    }
    return sum_avg;   
  }
}


Pressure* WossManager::getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) return( SDefHandler::instance()->getPressure()->create(1.0, 0) ); // it is the same node!

  SimTime sim_time = woss_creator->getSimTime(tx_coordz,rx_coordz);
  if ( sim_time.start_time.isValid() ) {
    Time time = sim_time.start_time + (time_t)time_value;

    if (debug) ::std::cout << "WossManager::getWossPressure() time value in seconds = " << time_value
                           << "; start sime time = " << sim_time.start_time << "; computed time = " << time << ::std::endl;
    
    return getWossPressure(tx_coordz, rx_coordz, start_frequency, end_frequency, time );
  }
  else {
    ::std::cout << "WossManager::getWossPressure() WARNING, invalid start time for tx = " << tx_coordz << "; rx = " 
                << rx_coordz << ::std::endl;
                
    return NULL;
  }
}


PressureVector WossManager::getWossPressure( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, const Time& time_value ) {
  PressureVector ret_value;
  ret_value.reserve(coordinates.size());

  for ( int i = 0; i < (int) coordinates.size(); i++ ) {
    ret_value.push_back( getWossPressure( coordinates[i].first, coordinates[i].second, start_frequency, end_frequency, time_value ) );
  }
  return ret_value;
}


PressureVector WossManager::getWossPressure( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, double time_value ) {
  PressureVector ret_value;
  ret_value.reserve(coordinates.size());

  for ( int i = 0; i < (int) coordinates.size(); i++ ) {
    SimTime sim_time = woss_creator->getSimTime(coordinates[i].first, coordinates[i].second);
    
    if ( sim_time.start_time.isValid() ) {
      Time time = sim_time.start_time + (time_t)time_value;
      
      if (debug) ::std::cout << "WossManager::getWossPressure() time value in seconds = " << time_value
                             << "; start sime time = " << sim_time.start_time << "; computed time = " << time << ::std::endl;
          
      ret_value.push_back( getWossPressure( coordinates[i].first, coordinates[i].second, start_frequency, end_frequency, time_value ) );
    }
    else {
      ::std::cout << "WossManager::getWossPressure() WARNING, invalid start time for tx = " << coordinates[i].first << "; rx = " 
                  << coordinates[i].second << ::std::endl;

      ret_value.push_back( SDefHandler::instance()->getPressure()->create( Pressure::createNotValid() ) );
    }
  }
  return ret_value; 
}


TimeArrVector WossManager::getWossTimeArr( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, const Time& time_value ) {
  TimeArrVector ret_value;
  ret_value.reserve(coordinates.size());
  
  for ( int i = 0; i < (int) coordinates.size(); i++ ) {
    ret_value.push_back( getWossTimeArr( coordinates[i].first, coordinates[i].second, start_frequency, end_frequency, time_value ) );
  }
  return ret_value;
}


TimeArrVector WossManager::getWossTimeArr( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, double time_value ) {
  TimeArrVector ret_value;
  ret_value.reserve(coordinates.size());

  for ( int i = 0; i < (int) coordinates.size(); i++ ) {
    SimTime sim_time = woss_creator->getSimTime(coordinates[i].first, coordinates[i].second);
    
    if ( sim_time.start_time.isValid() ) {
      Time time = sim_time.start_time + (time_t)time_value;
      
      if (debug) ::std::cout << "WossManager::getWossTimeArr() time value in seconds = " << time_value
                             << "; start sime time = " << sim_time.start_time << "; computed time = " << time << ::std::endl;
          
      ret_value.push_back( getWossTimeArr( coordinates[i].first, coordinates[i].second, start_frequency, end_frequency, time_value ) );
    }
    else {
      ::std::cout << "WossManager::getWossTimeArr() WARNING, invalid start time for tx = " << coordinates[i].first << "; rx = " 
                  << coordinates[i].second << ::std::endl;
                  
      ret_value.push_back( SDefHandler::instance()->getTimeArr()->create( TimeArr::createNotValid() ) );
    }
  }
  return ret_value; 
}


const Woss& WossManager::getActiveWoss( const CoordZ& tx, const CoordZ& rx, double start_frequency, double end_frequency ) const {
  Woss* ptr = const_cast< woss::WossManager* >( this )->getWoss( tx, rx, start_frequency, end_frequency );
//   if ( ptr->hasRun() ) return *ptr;
//   ptr->run();
  return *ptr;
}


/////////
WossManagerResDb::WossManagerResDb()
:  woss_db_manager(NULL)
{

}


TimeArr* WossManagerResDb::getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) return( SDefHandler::instance()->getTimeArr()->create( TimeArr::createImpulse() ) ); // it is the same node!
   
  if ( debug ) ::std::cout << "WossManagerResDb::getWossTimeArr() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
                           << "; start freq = " << start_frequency << "; end freq = " << end_frequency << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) << ::std::endl; 
  
  bool valid = true;
  
  double freq_step = woss_creator->getFrequencyStep(tx_coordz,rx_coordz);
  int i = 0;
  const Time* time = NULL;
  
  if ( is_time_evolution_active == false ) {
    time = &NO_EVOLUTION_TIME;
  }
  else 
    time = &time_value;
  
  TimeArr* sum = dbGetTimeArr( tx_coordz, rx_coordz, (start_frequency + ((double)i) * freq_step ), *time );
  
  if ( debug && sum != NULL ) ::std::cout << "WossManagerResDb::getWossTimeArr() first TimeArr in db " << *sum << ::std::endl; 
  
  i++;
  valid = valid && sum->isValid();
  
  TimeArr* curr_time_arr = NULL;
  for( ; i <= floor( ( end_frequency - start_frequency) / freq_step ); i++ ) {
      curr_time_arr = dbGetTimeArr( tx_coordz, rx_coordz, (start_frequency + ((double)i) * freq_step ), *time );
      valid = valid && curr_time_arr->isValid();

      if ( debug && curr_time_arr != NULL ) ::std::cout << "WossManagerResDb::getWossTimeArr() " << i << "-th TimeArr in db" << *curr_time_arr << ::std::endl; 
      
      if (!valid) { 
        delete curr_time_arr; 
        curr_time_arr = NULL;
        break;
      }
      *sum += *curr_time_arr;

      if ( debug && sum != NULL ) ::std::cout << "WossManagerResDb::getWossTimeArr() sum TimeArr " << *sum << ::std::endl; 
     
      delete curr_time_arr;
      curr_time_arr = NULL;
  }
  if (valid) return sum;
  
  sum->clear();

  bool is_ok = true;
  Woss* const curr_woss = getWoss( tx_coordz, rx_coordz, start_frequency, end_frequency );
  
  if ( curr_woss->timeEvolve(time_value) ) is_ok = curr_woss->run();
  assert(is_ok);
  
  for( FreqSIt it = curr_woss->freq_lower_bound( start_frequency ); it == curr_woss->freq_lower_bound( end_frequency ); it++ ) {
    curr_time_arr = curr_woss->getTimeArr( *it, tx_coordz.getDepth(), rx_coordz.getDepth(), tx_coordz.getGreatCircleDistance( rx_coordz ) ) ; 

    if ( debug && curr_time_arr != NULL ) ::std::cout << "WossManagerResDb::getWossTimeArr() " << i << "-th TimeArr " << *curr_time_arr << ::std::endl; 
    
    dbInsertTimeArr( tx_coordz, rx_coordz, *it, *time, *curr_time_arr );
    *sum += *curr_time_arr;
    delete curr_time_arr;
    curr_time_arr = NULL;
  }

  if ( debug && sum != NULL ) ::std::cout << "WossManagerResDb::getWossTimeArr() final TimeArr " << *sum << ::std::endl; 

  return sum; 
}


Pressure* WossManagerResDb::getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) return( SDefHandler::instance()->getPressure()->create(1.0, 0) ); // it is the same node!
  
  if ( debug ) ::std::cout << "WossManagerResDb::getWossPressure() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
                           << "; start freq = " << start_frequency << "; end freq = " << end_frequency << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) << ::std::endl; 
  
  bool valid = true;
  double freq_step = woss_creator->getFrequencyStep(tx_coordz,rx_coordz);
  int i = 0;
 
  const Time* time = NULL;

  if ( is_time_evolution_active == false ) {
    time = &NO_EVOLUTION_TIME;
  }
  else 
    time = &time_value;
  
  Pressure* press_temp = dbGetPressure( tx_coordz, rx_coordz, (start_frequency + ((double)i) * freq_step ), *time );
  
  TimeArr* sum_avg = SDefHandler::instance()->getTimeArr()->create( *press_temp );
  
  delete press_temp;
  press_temp = NULL;
  
  valid = valid && sum_avg->isValid();

  Pressure* curr_press = NULL;
  for( ; i <= floor( ( end_frequency - start_frequency) / freq_step ); i++ ) {
      curr_press = dbGetPressure( tx_coordz, rx_coordz, (start_frequency + ((double)i) * freq_step), *time );
      
      valid = valid && curr_press->isValid();
      if (!valid) {
        delete curr_press;
        curr_press = NULL;
        break;
      }
      *sum_avg += *curr_press;
      delete curr_press;
      curr_press = NULL;
  }
  if (valid) {
    Pressure* ret_val = SDefHandler::instance()->getPressure()->create( *sum_avg );
    delete sum_avg;
    sum_avg = NULL;

    return ret_val;
  }
  
  delete curr_press;
  curr_press = NULL;
  
  sum_avg->clear();

  bool is_ok = false;
  Woss* const curr_woss = getWoss( tx_coordz, rx_coordz, start_frequency, end_frequency );
  
  if ( curr_woss->timeEvolve(time_value) ) is_ok = curr_woss->run();
  assert(is_ok);
  
  for( FreqSIt it = curr_woss->freq_lower_bound( start_frequency ); it == curr_woss->freq_lower_bound( end_frequency ); it++ ) {
    curr_press = curr_woss->getAvgPressure( *it, tx_coordz.getDepth() ) ; 
    dbInsertPressure( tx_coordz, rx_coordz, *it, *time, *curr_press );
    *sum_avg += *curr_press;
    delete curr_press;
    curr_press = NULL;
  }
  
  Pressure* ret_value = SDefHandler::instance()->getPressure()->create( *sum_avg );
  
  delete sum_avg;
  sum_avg = NULL;
  
  return( ret_value ); 
}


#ifdef WOSS_MULTITHREAD


WossManagerResDbMT::WossManagerResDbMT() 
: max_thread_number(0),
  total_queries(0),
  concurrent_threads(0),
  total_thread_created(0),
  total_thread_ended(0),
  thread_query(),
  thread_time_arr_reply(),
  thread_pressure_reply()
{
  int ret = pthread_spin_init( &mutex, PTHREAD_PROCESS_PRIVATE );
  assert( ret == 0 );
  
  ret = pthread_spin_init( &request_mutex, PTHREAD_PROCESS_PRIVATE );
  assert( ret == 0 );

  max_thread_number = sysconf(_SC_NPROCESSORS_CONF);
  if ( max_thread_number != 1 ) {
	max_thread_number += ::std::floor( (double)max_thread_number / 2.0 ); 
	if ( max_thread_number > MAX_TOTAL_PTHREAD ) max_thread_number = MAX_TOTAL_PTHREAD;
  }
  checkConcurrentThreads();
}


WossManagerResDbMT::~WossManagerResDbMT() {
  pthread_spin_destroy( &mutex );
  pthread_spin_destroy( &request_mutex );
}


void WossManagerResDbMT::checkConcurrentThreads() {
  if ( concurrent_threads <= 0 ) 
    concurrent_threads = max_thread_number;
  else 
    concurrent_threads = ::std::min( max_thread_number, concurrent_threads );
  
  ::std::cout << "WossManagerResDbMT::checkConcurrentThreads() " << concurrent_threads << ::std::endl;
}


WossManagerResDbMT::ThreadParamIndex WossManagerResDbMT::popThreadParamIndex() {
  pthread_spin_lock( &mutex );

  if( debug ) ::std::cout << "WossManagerResDbMT::popThreadParamIndex() thread = " << ::std::hex << pthread_self() 
                          << ::std::dec << "; thread_query size = " << thread_query.coordz_pair_vect.size() 
                          << "; index = " << thread_query.curr_index << ::std::endl;

  ThreadParamIndex ret_value; 
  assert( !thread_query.coordz_pair_vect.empty() );

  ret_value.first = thread_query.curr_index;

  CoordZPairVect::iterator it = thread_query.coordz_pair_vect.begin();
  ret_value.second.coordz_pair = *it; 
  ret_value.second.sim_freq = thread_query.sim_freq;
  ret_value.second.time_data.is_time_object = thread_query.time_data.is_time_object;
  ret_value.second.time_data.time_cal = thread_query.time_data.time_cal;
  ret_value.second.time_data.time_double = thread_query.time_data.time_double;
  
  if( debug ) ::std::cout << "WossManagerResDbMT::popThreadParamIndex() tx = " << it->first << "; rx " << it->second
                          << "; start freq " << thread_query.sim_freq.first << "; end freq " << thread_query.sim_freq.second
                          << "; time cal " << thread_query.time_data.time_cal 
                          << "; time double " << thread_query.time_data.time_double << ::std::endl;

  thread_query.coordz_pair_vect.erase(it);
  thread_query.curr_index++;

  pthread_spin_unlock( &mutex );
  
  return ret_value;
}


PressureVector WossManagerResDbMT::getWossPressure( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( concurrent_threads < 0 ) return WossManager::getWossPressure( coordinates, start_frequency, end_frequency );

  thread_pressure_reply.clear();
  thread_pressure_reply.reserve(coordinates.size());
  thread_pressure_reply.resize(coordinates.size(), NULL);
  
  thread_query.coordz_pair_vect = coordinates;
  thread_query.sim_freq = ::std::make_pair( start_frequency, end_frequency );
  thread_query.time_data.is_time_object = true;
  thread_query.time_data.time_cal = time_value;
  thread_query.time_data.time_double = 0.0;
  thread_query.curr_index = 0;

  initThreadVars();
  
  int index_thd = 0;
  for ( ; index_thd < concurrent_threads; index_thd++ ) {
    if ( total_thread_created < total_queries ) {
      total_thread_created++;
      int ret = pthread_create( &thread_arr[index_thd], NULL, WMSMTcreateThreadPressure , (void*)this );
      assert( ret == 0 );
    }
  }
  
  int curr = 0;
  while ( total_thread_ended < total_thread_created ) {
    int ret = pthread_join( thread_arr[curr], NULL ); 
    assert( ret == 0 );
    total_thread_ended++;
    curr = (curr + 1) % MAX_TOTAL_PTHREAD;
    if ( total_thread_created < total_queries ) {
      index_thd %= MAX_TOTAL_PTHREAD;
      ret = pthread_create( &thread_arr[index_thd], NULL, WMSMTcreateThreadPressure , (void*)this );
      total_thread_created++;
      index_thd++;
    }
  }

  assert( total_thread_ended == total_queries );
   
  return thread_pressure_reply;
}


PressureVector WossManagerResDbMT::getWossPressure( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, double time_value ) {
  if ( concurrent_threads < 0 ) return WossManager::getWossPressure( coordinates, start_frequency, end_frequency, time_value );

  thread_pressure_reply.clear();
  thread_pressure_reply.reserve(coordinates.size());
  thread_pressure_reply.resize(coordinates.size(), NULL);
  
  thread_query.coordz_pair_vect = coordinates;
  thread_query.sim_freq = ::std::make_pair( start_frequency, end_frequency );
  thread_query.time_data.is_time_object = false;
  thread_query.time_data.time_cal = Time();
  thread_query.time_data.time_double = time_value;
  thread_query.curr_index = 0;

  initThreadVars();
  
  int index_thd = 0;
  for ( ; index_thd < concurrent_threads; index_thd++ ) {
    if ( total_thread_created < total_queries ) {
      total_thread_created++;
      int ret = pthread_create( &thread_arr[index_thd], NULL, WMSMTcreateThreadPressure , (void*)this );
      assert( ret == 0 );
    }
  }
  
  int curr = 0;
  while ( total_thread_ended < total_thread_created ) {
    int ret = pthread_join( thread_arr[curr], NULL ); 
    assert( ret == 0 );
    total_thread_ended++;
    curr = (curr + 1) % MAX_TOTAL_PTHREAD;
    if ( total_thread_created < total_queries ) {
      index_thd %= MAX_TOTAL_PTHREAD;
      ret = pthread_create( &thread_arr[index_thd], NULL, WMSMTcreateThreadPressure , (void*)this );
      total_thread_created++;
      index_thd++;
    }
  }

  assert( total_thread_ended == total_queries );
   
  return thread_pressure_reply;
}



TimeArrVector WossManagerResDbMT::getWossTimeArr( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( concurrent_threads < 0 ) return WossManager::getWossTimeArr( coordinates, start_frequency, end_frequency );

  thread_time_arr_reply.clear();
  thread_time_arr_reply.reserve(coordinates.size());
  thread_time_arr_reply.resize(coordinates.size(), NULL);
  
  thread_query.coordz_pair_vect = coordinates;
  thread_query.sim_freq = ::std::make_pair( start_frequency, end_frequency );
  thread_query.time_data.is_time_object = true;
  thread_query.time_data.time_cal = time_value;
  thread_query.time_data.time_double = 0.0;
  thread_query.curr_index = 0;
  
  initThreadVars();
  
  int index_thd = 0;
  for ( ; index_thd < concurrent_threads; index_thd++ ) {
    if ( total_thread_created < total_queries ) {
      total_thread_created++;
      int ret = pthread_create( &thread_arr[index_thd], NULL, WMSMTcreateThreadTimeArr , (void*)this );
      assert( ret == 0 );
    }
  }
  
  int curr = 0;
  while ( total_thread_ended < total_thread_created ) {
    int ret = pthread_join( thread_arr[curr], NULL ); 
    assert( ret == 0 );
    total_thread_ended++;
    curr = (curr + 1) % MAX_TOTAL_PTHREAD;
    if ( total_thread_created < total_queries ) {
      index_thd %= MAX_TOTAL_PTHREAD;
      ret = pthread_create( &thread_arr[index_thd], NULL, WMSMTcreateThreadTimeArr , (void*)this );
      total_thread_created++;
      index_thd++;
    }
  }
     
  assert( total_thread_ended == total_queries );
  
  return thread_time_arr_reply;
}


TimeArrVector WossManagerResDbMT::getWossTimeArr( const CoordZPairVect& coordinates, double start_frequency, double end_frequency, double time_value ) {
  if ( concurrent_threads < 0 ) return WossManager::getWossTimeArr( coordinates, start_frequency, end_frequency );

  thread_time_arr_reply.clear();
  thread_time_arr_reply.reserve(coordinates.size());
  thread_time_arr_reply.resize(coordinates.size(), NULL);
  
  thread_query.coordz_pair_vect = coordinates;
  thread_query.sim_freq = ::std::make_pair( start_frequency, end_frequency );
  thread_query.time_data.is_time_object = false;
  thread_query.time_data.time_cal = Time();
  thread_query.time_data.time_double = time_value;
  thread_query.curr_index = 0;

  initThreadVars();
  
  int index_thd = 0;
  for ( ; index_thd < concurrent_threads; index_thd++ ) {
    if ( total_thread_created < total_queries ) {
      total_thread_created++;
      int ret = pthread_create( &thread_arr[index_thd], NULL, WMSMTcreateThreadTimeArr , (void*)this );
      assert( ret == 0 );
    }
  }
  
  int curr = 0;
  while ( total_thread_ended < total_thread_created ) {
    int ret = pthread_join( thread_arr[curr], NULL ); 
    assert( ret == 0 );
    total_thread_ended++;
    curr = (curr + 1) % MAX_TOTAL_PTHREAD;
    if ( total_thread_created < total_queries ) {
      index_thd %= MAX_TOTAL_PTHREAD;
      ret = pthread_create( &thread_arr[index_thd], NULL, WMSMTcreateThreadTimeArr , (void*)this );
      total_thread_created++;
      index_thd++;
    }
  }
     
  assert( total_thread_ended == total_queries );
  
  return thread_time_arr_reply;
}


void WossManagerResDbMT::initThreadVars() {
  total_thread_created = 0;
  total_thread_ended = 0;
  
  total_queries = thread_query.coordz_pair_vect.size();
  
  if(debug) ::std::cout << "WossManagerResDbMT::initThreadVars() thread = " << ::std::hex << pthread_self()
                        << ::std::dec << "; total queries = " << total_queries 
                        << "; concurrent threads = " << concurrent_threads << "; total_thread_created = " 
                        << total_thread_created << "; total thread ended = "<< total_thread_ended << ::std::endl; 
}


void* woss::WMSMTcreateThreadTimeArr( void* ptr ) {
  WossManager* manager_ptr = reinterpret_cast< WossManager* >( ptr );
  WossManagerResDbMT* manager_mt_ptr = reinterpret_cast< WossManagerResDbMT* >( ptr );
  
  assert( manager_mt_ptr );
  
  WossManagerResDbMT::ThreadParamIndex curr_param = manager_mt_ptr->popThreadParamIndex();

  int index = curr_param.first;
  WossManagerResDbMT::ThreadParam& param = curr_param.second;

  if(manager_mt_ptr->debug) ::std::cout << "woss::WMSMTcreateThreadTimeArr() called, index = " << index << "; tx = " << param.coordz_pair.first
                                        << "; rx = " << param.coordz_pair.second << "; start freq = " << param.sim_freq.first
                                        << "; end_freq = " << param.sim_freq.second  << ::std::flush
                                        << "; time = " << (param.time_data.is_time_object?param.time_data.time_cal:param.time_data.time_double) << ::std::endl;
  woss::TimeArr* value = NULL;
  
  if ( param.time_data.is_time_object ) {
    value = manager_ptr->getWossTimeArr( param.coordz_pair.first, param.coordz_pair.second, 
                                                        param.sim_freq.first, param.sim_freq.second, param.time_data.time_cal);
  }                                                  
  else {
     SimTime sim_time = manager_ptr->getWossCreator()->getSimTime(param.coordz_pair.first, param.coordz_pair.second);
     if ( sim_time.start_time.isValid() ) {
        Time time = sim_time.start_time + (time_t)param.time_data.time_double;
        value = manager_ptr->getWossTimeArr( param.coordz_pair.first, param.coordz_pair.second, 
                                                            param.sim_freq.first, param.sim_freq.second, time);
     }
     else {
        ::std::cout << "woss::WMSMTcreateThreadTimeArr() WARNING, invalid start time for tx = " << param.coordz_pair.first << "; rx = " 
                    << param.coordz_pair.second << ::std::endl;
     }
  }

  assert( value != NULL );

  manager_mt_ptr->insertThreadReplyTimeArr( index, value );
  
  return NULL;
}


void* woss::WMSMTcreateThreadPressure( void* ptr ) {
  WossManager* manager_ptr = reinterpret_cast< WossManager* >( ptr );
  WossManagerResDbMT* manager_mt_ptr = dynamic_cast< WossManagerResDbMT* >( manager_ptr );
  
  assert( manager_mt_ptr );
  
  WossManagerResDbMT::ThreadParamIndex curr_param = manager_mt_ptr->popThreadParamIndex();
  
  int index = curr_param.first;
  WossManagerResDbMT::ThreadParam& param = curr_param.second;

  if(manager_mt_ptr->debug) ::std::cout << "woss::WMSMTcreateThreadPressure() called, index = " << index << "; tx = " << param.coordz_pair.first
                                        << "; rx = " << param.coordz_pair.second << "; start freq = " << param.sim_freq.first << ::std::flush
                                        << "; time = " << (param.time_data.is_time_object?param.time_data.time_cal:param.time_data.time_double) << ::std::endl;
  woss::Pressure* value = NULL;
  
  if ( param.time_data.is_time_object ) {
    value = manager_ptr->getWossPressure( param.coordz_pair.first, param.coordz_pair.second, 
                                                        param.sim_freq.first, param.sim_freq.second, param.time_data.time_cal);
  }                                                  
  else {
    SimTime sim_time = manager_ptr->getWossCreator()->getSimTime(param.coordz_pair.first, param.coordz_pair.second);
     if ( sim_time.start_time.isValid() ) {
        Time time = sim_time.start_time + (time_t)param.time_data.time_double;
        value = manager_ptr->getWossPressure( param.coordz_pair.first, param.coordz_pair.second, 
                                              param.sim_freq.first, param.sim_freq.second, time);
     }
     else {
        ::std::cout << "woss::WMSMTcreateThreadPressure() WARNING, invalid start time for tx = " << param.coordz_pair.first << "; rx = " 
                    << param.coordz_pair.second << ::std::endl;
     }
  }
  manager_mt_ptr->insertThreadReplyPressure( index, value );
  
  return NULL;
}
 

void WossManagerResDbMT::insertThreadReplyTimeArr( int index, woss::TimeArr* time_arr ) {
  pthread_spin_lock( &mutex );
  thread_time_arr_reply[index] = time_arr;
  pthread_spin_unlock( &mutex );
}


void WossManagerResDbMT::insertThreadReplyPressure( int index, woss::Pressure* pressure ) {
  pthread_spin_lock( &mutex );
  thread_pressure_reply[index] = pressure;
  pthread_spin_unlock( &mutex );
}


TimeArr* WossManagerResDbMT::getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) return( SDefHandler::instance()->getTimeArr()->create( TimeArr::createImpulse() ) ); // it is the same node!
   
  if ( concurrent_threads < 0 ) return WossManagerResDb::getWossTimeArr( tx_coordz, rx_coordz, start_frequency, end_frequency, time_value );
  
  if ( debug ) ::std::cout << "WossManagerResDbMT::getWossTimeArr() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
                           << "; start freq = " << start_frequency << "; end freq = " << end_frequency 
                           << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) << ::std::flush
                           << "; time_value = " << time_value << ::std::endl; 
  
  bool valid = true;
  
  double freq_step = woss_creator->getFrequencyStep( tx_coordz, rx_coordz );
  
  pthread_spin_lock( &request_mutex );

  const Time* time = NULL;
  
  if ( is_time_evolution_active == false ) {
    time = &NO_EVOLUTION_TIME;
  }
  else 
    time = &time_value;
  
  TimeArr* sum = dbGetTimeArr( tx_coordz, rx_coordz, start_frequency, *time );
  
  if ( debug && sum != NULL ) ::std::cout << "WossManagerResDbMT::getWossTimeArr() first TimeArr in db " << *sum << ::std::endl; 
    
  valid = valid && sum->isValid();
  
  int i = 1;
  TimeArr* curr_time_arr = NULL;
  for( ; i <= floor( ( end_frequency - start_frequency) / freq_step ); i++ ) {
      curr_time_arr = dbGetTimeArr( tx_coordz, rx_coordz, (start_frequency + ((double)i) * freq_step ), *time );
      valid = valid && curr_time_arr->isValid();

      if (!valid) { 
        delete curr_time_arr; 
        curr_time_arr = NULL;
        break;
      }
      *sum += *curr_time_arr;
      delete curr_time_arr;
      curr_time_arr = NULL;
  }
  if (valid) {
    
    if ( debug ) ::std::cout << "WossManagerResDbMT::getWossTimeArr() valid TimeArr in db found." << ::std::endl;
    
    pthread_spin_unlock( &request_mutex );
    return sum;
  }
  
  sum->clear();

  if ( debug ) ::std::cout << "WossManagerResDbMT::getWossTimeArr() NO valid TimeArr in db found" 
                           << ", getting a Woss object." << ::std::endl;
  
  Woss* const curr_woss = getWoss( tx_coordz, rx_coordz, start_frequency, end_frequency );
  
  if ( curr_woss->isRunning() ) {
    AWIter it = active_woss.find( curr_woss ); 
    
    if ( it != active_woss.end() ) {

      if ( debug ) ::std::cout << "WossManagerResDbMT::getWossTimeArr() curr Woss is running." 
                               << " Waiting for it to finish..."<< ::std::endl;
          
      assert( it->second != NULL );
      
      pthread_spin_unlock( &request_mutex );

      pthread_mutex_lock( &(it->second->mutex) );
      pthread_cond_wait( &(it->second->condition), &(it->second->mutex) );
      pthread_mutex_unlock( &(it->second->mutex) ); 
      
      pthread_spin_lock( &request_mutex );
    }
  }
  else {    
    bool has_to_run = curr_woss->timeEvolve(time_value);
    bool is_ok = true;

    if ( has_to_run )
       active_woss[curr_woss] = new ThreadCondSignal();
    
    pthread_spin_unlock( &request_mutex );

    if ( has_to_run ) is_ok = curr_woss->run();
    assert( is_ok );
    
    pthread_spin_lock( &request_mutex );

    if ( debug ) ::std::cout << "WossManagerResDbMT::getWossTimeArr() curr Woss was active and has now run." 
                              << ::std::endl;
  }  
  
  if ( debug ) ::std::cout << "WossManagerResDbMT::getWossTimeArr() curr Woss object has run." << ::std::endl;
  
  for( FreqSIt it = curr_woss->freq_lower_bound( start_frequency ); it == curr_woss->freq_lower_bound( end_frequency ); ++it ) {
    curr_time_arr = curr_woss->getTimeArr( *it, tx_coordz.getDepth(), rx_coordz.getDepth(), tx_coordz.getGreatCircleDistance( rx_coordz ) ) ; 
    dbInsertTimeArr( tx_coordz, rx_coordz, *it, *time, *curr_time_arr );
    *sum += *curr_time_arr;
    delete curr_time_arr;
    curr_time_arr = NULL;
  }
  
  if ( debug ) ::std::cout << "WossManagerResDbMT::getWossTimeArr() TimeArr computed = " << *sum << ::std::endl;
   
  AWIter it = active_woss.find( curr_woss ); 
  if ( it != active_woss.end() ) {
    assert( it->second != NULL );
    
    pthread_mutex_lock( &(it->second->mutex) );
    pthread_cond_broadcast( &(it->second->condition) );
    pthread_mutex_unlock( &(it->second->mutex) );    
    
    delete it->second;
    active_woss.erase(it);
  }

  pthread_spin_unlock( &request_mutex );
  
  return sum; 
}


TimeArr* WossManagerResDbMT::getWossTimeArr( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) return( SDefHandler::instance()->getTimeArr()->create( TimeArr::createImpulse() ) ); // it is the same node!
 
  pthread_spin_lock( &request_mutex );
 
  if ( debug ) ::std::cout << "WossManagerResDbMT::getWossTimeArr() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
                           << "; start freq = " << start_frequency << "; end freq = " << end_frequency << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) 
                           << "; time_value = " << time_value << ::std::endl; 

  SimTime sim_time = woss_creator->getSimTime(tx_coordz,rx_coordz);
  if ( sim_time.start_time.isValid() ) {
    Time time = sim_time.start_time + (time_t)time_value;
    
    if ( debug ) ::std::cout << "WossManagerResDbMT::getWossTimeArr() time converted = " << time << ::std::endl;
    
    pthread_spin_unlock( &request_mutex );
    
    return getWossTimeArr(tx_coordz, rx_coordz, start_frequency, end_frequency, time );
  }
  else {
    ::std::cout << "WossManagerResDbMT::getWossTimeArr() WARNING, invalid start time for tx = " << tx_coordz << "; rx = " 
                << rx_coordz << ::std::endl;
          
    pthread_spin_unlock( &request_mutex );
    
    return NULL;
  }  
}


Pressure* WossManagerResDbMT::getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, const Time& time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) return( SDefHandler::instance()->getPressure()->create(1.0, 0) ); // it is the same node!
  
  if ( concurrent_threads < 0 ) return WossManagerResDb::getWossPressure( tx_coordz, rx_coordz, start_frequency, end_frequency, time_value );

  if ( debug ) ::std::cout << "WossManagerResDbMT::getWossPressure() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
                           << "; start freq = " << start_frequency << "; end freq = " << end_frequency 
                           << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) 
                           << "; time_value = " << time_value << ::std::endl; 
  
  bool valid = true;
  double freq_step = woss_creator->getFrequencyStep(tx_coordz,rx_coordz);
  
  pthread_spin_lock( &request_mutex );
  
  const Time* time = NULL;
  
  if ( is_time_evolution_active == false ) {
    time = &NO_EVOLUTION_TIME;
  }
  else 
    time = &time_value;
  
  Pressure* temp = dbGetPressure( tx_coordz, rx_coordz, start_frequency, *time );
  TimeArr* sum_avg = SDefHandler::instance()->getTimeArr()->create( *temp );
  delete temp;
  temp = NULL;
  
  valid = valid && sum_avg->isValid();
  
  int i = 1;
  Pressure* curr_press = NULL;
  for( ; i <= floor( ( end_frequency - start_frequency) / freq_step ); i++ ) {
      curr_press = dbGetPressure( tx_coordz, rx_coordz, (start_frequency + ((double)i) * freq_step), *time );
      
      valid = valid && curr_press->isValid();
      if (!valid) {
        delete curr_press;
        curr_press = NULL;
        break;
      }
      *sum_avg += *curr_press;
      delete curr_press;
      curr_press = NULL;
  }
  if (valid) {
    Pressure* ret_val = SDefHandler::instance()->getPressure()->create( *sum_avg );
    delete sum_avg;
    sum_avg = NULL;

    if ( debug ) ::std::cout << "WossManagerResDbMT::getWossPressure() valid Pressure in db found." << ::std::endl;
    
    pthread_spin_unlock( &request_mutex );

    return ret_val;
  }
  
  delete curr_press;
  curr_press = NULL;
  
  sum_avg->clear();

  if ( debug ) ::std::cout << "WossManagerResDbMT::getWossPressure() NO valid Pressure in db found." 
                           << ", getting a Woss object." << ::std::endl;
  
  Woss* const curr_woss = getWoss( tx_coordz, rx_coordz, start_frequency, end_frequency );
  
  if ( curr_woss->isRunning() ) {
    AWIter it = active_woss.find( curr_woss ); 
    
    if ( it != active_woss.end() ) {

      if ( debug ) ::std::cout << "WossManagerResDbMT::getWossPressure() curr Woss is running." 
                               << " Waiting for it to finish..."<< ::std::endl;
          
      assert( it->second != NULL );
      
      pthread_spin_unlock( &request_mutex );

      pthread_mutex_lock( &(it->second->mutex) );
      pthread_cond_wait( &(it->second->condition), &(it->second->mutex) );
      pthread_mutex_unlock( &(it->second->mutex) ); 
      
      pthread_spin_lock( &request_mutex );
    }
  }
  else {    
    bool has_to_run = curr_woss->timeEvolve(time_value);
    bool is_ok = true;

    if ( has_to_run )
       active_woss[curr_woss] = new ThreadCondSignal();
    
    pthread_spin_unlock( &request_mutex );

    if ( has_to_run ) is_ok = curr_woss->run();
    assert( is_ok );
    
    pthread_spin_lock( &request_mutex );

    if ( debug ) ::std::cout << "WossManagerResDbMT::getWossPressure() curr Woss was active and has now run." 
                              << ::std::endl;
  } 
  
  if ( debug ) ::std::cout << "WossManagerResDbMT::getWossPressure() curr Woss object has run." << ::std::endl;
  
  for( FreqSIt it = curr_woss->freq_lower_bound( start_frequency ); it == curr_woss->freq_lower_bound( end_frequency ); it++ ) {
    curr_press = curr_woss->getAvgPressure( *it, tx_coordz.getDepth() ) ; 
    dbInsertPressure( tx_coordz, rx_coordz, *it, *time, *curr_press );
    *sum_avg += *curr_press;
    delete curr_press;
    curr_press = NULL;
  }
  
  Pressure* ret_value = SDefHandler::instance()->getPressure()->create( *sum_avg );
  
  delete sum_avg;
  sum_avg = NULL;
  
  if ( debug ) ::std::cout << "WossManagerResDbMT::getWossPressure() Pressure computed = " << *ret_value << ::std::endl;
  
  AWIter it = active_woss.find( curr_woss ); 
  if ( it != active_woss.end() ) {
    assert( it->second != NULL );
    
    pthread_mutex_lock( &(it->second->mutex) );
    pthread_cond_broadcast( &(it->second->condition) );
    pthread_mutex_unlock( &(it->second->mutex) );    
    
    delete it->second;
    active_woss.erase(it);
  }

  pthread_spin_unlock( &request_mutex );

  return( ret_value ); 
}


Pressure* WossManagerResDbMT::getWossPressure( const CoordZ& tx_coordz, const CoordZ& rx_coordz, double start_frequency, double end_frequency, double time_value ) {
  if ( tx_coordz.getCartDistance( rx_coordz ) == 0 ) return( SDefHandler::instance()->getPressure()->create(1.0, 0) ); // it is the same node!
 
  pthread_spin_lock( &request_mutex );
  
  if ( debug ) ::std::cout << "WossManagerResDbMT::getWossPressure() tx coords = " << tx_coordz << "; rx coords = " << rx_coordz
                           << "; start freq = " << start_frequency << "; end freq = " << end_frequency 
                           << "; distance = " << tx_coordz.getCartDistance( rx_coordz ) 
                           << "; time_value = " << time_value << ::std::endl;
  
  SimTime sim_time = woss_creator->getSimTime(tx_coordz,rx_coordz);
  if ( sim_time.start_time.isValid() ) {
    Time time = sim_time.start_time + (time_t)time_value;
    
    if ( debug ) ::std::cout << "WossManagerResDbMT::getWossPressure() time converted = " << time << ::std::endl;
    
    pthread_spin_unlock( &request_mutex );
    
    return getWossPressure(tx_coordz, rx_coordz, start_frequency, end_frequency, time );
  }
  else {
    ::std::cout << "WossManagerResDbMT::getWossPressure() WARNING, invalid start time for tx = " << tx_coordz << "; rx = " 
                << rx_coordz << ::std::endl;
          
    pthread_spin_unlock( &request_mutex );
    
    return NULL;
  }  
}

    
#endif // WOSS_MULTITHREAD

