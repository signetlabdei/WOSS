/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2020 Federico Guerra
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
 * @file   woss-definitions-test.cpp
 * @author Federico Guerra
 * 
 * \brief 
 *
 * 
 */


#include <iostream>
#include <vector>
#include "woss-test.h"

using namespace std;
using namespace woss;

class WossCoordDefTest : public WossTest {

  private:

  virtual void doConfig();

  virtual void doInit();

  virtual void doRun(); 

};


void WossCoordDefTest::doConfig() {
  debug = false;
}

void WossCoordDefTest::doInit() {
}

void WossCoordDefTest::doRun() {
  double start_lat = -89.0;
  double end_lat = 89.0;
  double step_lat = 30.0;
  double start_lon = -180.0;
  double end_lon = 180.0;
  double step_lon = 30.0;
  double start_bearing = 0.0;
  double end_bearing = 360.0;
  double step_bearing = 20.0;
  double start_distance = 1000.0;
  double end_distance = 101000.0;
  double step_distance = 10000.0;
  double precision = 1e-5;

  std::vector<double> vector_lat;
  std::vector<double> vector_lon;
  std::vector<double> vector_dist;
  std::vector<double> vector_bearing;

  for (double i = start_lon; i <= end_lon; i += step_lon) {
    vector_lon.push_back(i);
  }

  for (double i = start_lat; i <= end_lat; i += step_lat) {
    vector_lat.push_back(i);
  }

  for (double i = start_bearing; i < end_bearing; i += step_bearing) {
    vector_bearing.push_back(i * M_PI / 180.0);
  }

  for (double i = start_distance; i <= end_distance; i += step_distance) {
    vector_dist.push_back(i);
  }

  for (vector<double>::iterator it = vector_lon.begin(); it != vector_lon.end(); ++it) {
    double curr_lon = *it;

    if (debug) {
      cout << __LINE__ << ": " << "curr_lon: " << curr_lon << endl;
    }

    for (vector<double>::iterator it2 = vector_lat.begin(); it2 != vector_lat.end(); ++it2) {
      double curr_lat = *it2;

      if (debug) {
        cout << __LINE__ << ": " << "curr_lat: " << curr_lat << endl;
      }

      Coord curr_coord(curr_lat, curr_lon);

      if (debug) {
        cout << __LINE__ << ": " << "curr_coord: " << curr_coord << endl;
        cout << __LINE__ << ": " << "marsden coords: " << curr_coord.getMarsdenSquare() 
                         << "," << curr_coord.getMarsdenOneDegreeSquare() << endl;
      }

      if (false == curr_coord.isValid()) {
        throw WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_INVALID_PARAM); 
      }

      for (vector<double>::iterator it3 = vector_bearing.begin(); it3 != vector_bearing.end(); ++it3) {
        double curr_bear = *it3;

        if (debug) {
          cout << __LINE__ << ": " << "curr_bear: " << curr_bear << ", " << curr_bear * 180.0 / M_PI << endl;
        }

        for (vector<double>::iterator it4 = vector_dist.begin(); it4 != vector_dist.end(); ++it4) {
          double curr_distance = *it4;

          if (debug) {
            cout << __LINE__ << ": " << "curr_distance: " << curr_distance << endl;
          }

          Coord new_coord = Coord::getCoordFromBearing(curr_coord, curr_bear, curr_distance);

          if (debug) {
            cout << __LINE__ << ": " << "curr_coord: " << curr_coord << "; new_coord: " << new_coord << endl;
          }

          if (false == new_coord.isValid()) {
            throw WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_INVALID_PARAM); 
          }

          double test_bearing = curr_coord.getInitialBearing( new_coord );
          PDouble test_bearing_pd = PDouble(test_bearing, precision);
          PDouble curr_bearing_pd = PDouble(curr_bear, precision);

          if (debug) {
            cout << __LINE__ << ": " << "curr_bear: " << curr_bear << "," << curr_bearing_pd << "," << curr_bearing_pd * PDouble(180.0 / M_PI) 
                 << "; test_bearing: " << test_bearing << "," <<  test_bearing_pd << "," << test_bearing_pd * PDouble(180.0 / M_PI) << endl;
          }

          if (test_bearing_pd != curr_bearing_pd) {
            throw WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM); 
          }

          double test_distance = curr_coord.getGreatCircleDistance( new_coord );
          PDouble test_distance_pd = PDouble(test_distance, precision);
          PDouble curr_distance_pd = PDouble(curr_distance, precision);

          if (debug) {
            cout << __LINE__ << ": " << "curr_distance: " << curr_distance << "," << curr_distance_pd
                 << "; test_distance: " << test_distance << "," << test_distance_pd << endl;
          }

          if (test_distance_pd != curr_distance_pd) {
            throw WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM); 
          }
        }
      }
    }
  }
}


int main(int argc, char* argv [])
{
  WossCoordDefTest* woss_def_test = new WossCoordDefTest();
  woss_def_test->run();
  delete woss_def_test;

  return 0;
}
