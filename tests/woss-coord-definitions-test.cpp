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

  public:
  
  WossCoordDefTest();
  
  virtual ~WossCoordDefTest() {}

  
  private:

  virtual void doConfig();

  virtual void doInit();

  virtual void doRun(); 


  void doCoordCartTests(const CoordZ& curr_coord);

  void doBearingTests(const CoordZ& curr_coord, const CoordZ& new_coord, double curr_distance, double curr_bear);


  double start_lat;
  double end_lat;
  double step_lat;
  double start_lon;
  double end_lon;
  double step_lon;
  double start_bearing;
  double end_bearing;
  double step_bearing;
  double start_distance; // meters
  double end_distance; // meters
  double step_distance; // meters
  double precision;
  double cartesian_precision; // meters
  double start_depth; // meters
  double end_depth; // meters
  double step_depth; // meters

  std::vector<double> vector_lat;
  std::vector<double> vector_lon;
  std::vector<double> vector_dist;
  std::vector<double> vector_bearing;
  std::vector<double> vector_depth;
};

WossCoordDefTest::WossCoordDefTest()
: WossTest(),
  start_lat(-89.0),
  end_lat(89.0),
  step_lat(30.0),
  start_lon(-180.0),
  end_lon(180.0),
  step_lon(30.0),
  start_bearing(0.0),
  end_bearing(360.0),
  step_bearing(20.0),
  start_distance(1000.0),
  end_distance(101000.0),
  step_distance(10000.0),
  precision(1e-5),
  cartesian_precision(0.5),
  start_depth(1.0),
  end_depth(10000.0),
  step_depth(100.0),
  vector_lat(),
  vector_lon(),
  vector_dist(),
  vector_bearing(),
  vector_depth()
{
  //debug = true;
}

void WossCoordDefTest::doConfig() {
}

void WossCoordDefTest::doInit() {
}

void WossCoordDefTest::doCoordCartTests(const CoordZ& test_coord) {
  CoordZ::CoordZSpheroidType type;

  for (int i = 0; i < 3; ++i) {
    if (i == 0) {
      type = CoordZ::CoordZSpheroidType::COORDZ_SPHERE;
    }
    else if (i == 1) {
      type = CoordZ::CoordZSpheroidType::COORDZ_GRS80;
    }
    else {
      type = CoordZ::CoordZSpheroidType::COORDZ_WGS84;
    }

    CoordZ::CartCoords test_cart = test_coord.getCartCoords(type);

    if (debug) {
      cout << __LINE__ << ": " << "test_cart = " << test_cart << endl;
    }

    CoordZ test_cart_coord = CoordZ::getCoordZFromCartesianCoords(test_cart);

    if (debug) {
      cout << __LINE__ << ": " << "test_cart_coord: " << test_cart_coord << endl;
    }

    if (false == test_cart_coord.isValid()) {
      throw WOSS_EXCEPTION(WOSS_ERROR_INVALID_PARAM); 
    }

    double error_distance = test_cart_coord.getCartDistance(test_coord, type);

    if (debug) {
      cout << __LINE__ << ": " << "test error_distance: " << error_distance << endl;
    }

    if (error_distance > cartesian_precision) {
      throw WOSS_EXCEPTION(WOSS_ERROR_OUT_OF_RANGE_PARAM); 
    }
  }
}

void WossCoordDefTest::doBearingTests(const CoordZ& curr_coord, const CoordZ& new_coord, double curr_distance, double curr_bear) {
  double test_bearing = curr_coord.getInitialBearing( new_coord );
  PDouble test_bearing_pd = PDouble(test_bearing, precision);
  PDouble curr_bearing_pd = PDouble(curr_bear, precision);

  if (debug) {
    cout << __LINE__ << ": " << "curr_bear: " << curr_bear << "," << curr_bearing_pd << "," << curr_bearing_pd * PDouble(180.0 / M_PI) 
        << "; test_bearing: " << test_bearing << "," <<  test_bearing_pd << "," << test_bearing_pd * PDouble(180.0 / M_PI) << endl;
  }

  if (test_bearing_pd != curr_bearing_pd) {
    throw WOSS_EXCEPTION(WOSS_ERROR_OUT_OF_RANGE_PARAM); 
  }

  double test_distance = curr_coord.getGreatCircleDistance( new_coord );
  PDouble test_distance_pd = PDouble(test_distance, precision);
  PDouble curr_distance_pd = PDouble(curr_distance, precision);

  if (debug) {
    cout << __LINE__ << ": " << "curr_distance: " << curr_distance << "," << curr_distance_pd
        << "; test_distance: " << test_distance << "," << test_distance_pd << endl;
  }

  if (test_distance_pd != curr_distance_pd) {
    throw WOSS_EXCEPTION(WOSS_ERROR_OUT_OF_RANGE_PARAM); 
  }
  
  double cart_distance = curr_coord.getCartDistance( new_coord );
  PDouble cart_distance_pd = PDouble(curr_distance, precision);

  if (debug) {
    cout << __LINE__ << ": " << "curr_distance: " << curr_distance << "," << curr_distance_pd
        << "; cart_distance: " << cart_distance << "," << cart_distance_pd << endl;
  }

  if (curr_distance_pd != cart_distance_pd) {
    throw WOSS_EXCEPTION(WOSS_ERROR_OUT_OF_RANGE_PARAM); 
  }
}

void WossCoordDefTest::doRun() {

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

  for (double i = start_depth; i <= end_depth; i += step_depth) {
    vector_depth.push_back(i);
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

      for (vector<double>::iterator it = vector_depth.begin(); it != vector_depth.end(); ++it) {
        double curr_depth = *it;

        if (debug) {
          cout << __LINE__ << ": " << "curr_depth: " << curr_depth << endl;
        }
        CoordZ curr_coord(curr_lat, curr_lon, curr_depth);

        if (debug) {
          cout << __LINE__ << ": " << "curr_coord: " << curr_coord << endl;
        }

        if (false == curr_coord.isValid()) {
          throw WOSS_EXCEPTION(WOSS_ERROR_INVALID_PARAM); 
        }

        // Do geographical coordinates to Cartesian Coordinates tests
        doCoordCartTests(curr_coord);

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

            CoordZ new_coord = CoordZ( Coord::getCoordFromBearing(curr_coord, curr_bear, curr_distance), curr_depth);

            if (debug) {
              cout << __LINE__ << ": " << "curr_coord: " << curr_coord << "; new_coord: " << new_coord << endl;
            }

            if (false == new_coord.isValid()) {
              throw WOSS_EXCEPTION(WOSS_ERROR_INVALID_PARAM); 
            }

            // Do geographical coordinates to Cartesian Coordinates tests
            doCoordCartTests(new_coord);

            // Do bearing tests
            doBearingTests(curr_coord, new_coord, curr_distance, curr_bear);
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
