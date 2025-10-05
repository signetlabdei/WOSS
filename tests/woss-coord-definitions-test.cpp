/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2020 2025 Federico Guerra
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
 * @file   woss-definitions-test.cpp
 * @author Federico Guerra
 * 
 * \brief 
 *
 * 
 */


#include <iostream>
#include <vector>
#include <sstream>
#include "woss-test.h"

using namespace std;
using namespace woss;

class WossCoordDefTest : public WossTest {

  public:
  
  WossCoordDefTest();
  
  virtual ~WossCoordDefTest() override = default;

  private:

  virtual void doConfig() override;

  virtual void doInit() override;

  virtual void doRun() override;


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

  vector<double> vector_lat;
  vector<double> vector_lon;
  vector<double> vector_dist;
  vector<double> vector_bearing;
  vector<double> vector_depth;
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
}

void WossCoordDefTest::doConfig() {
  //debug = true;
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
      WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_INVALID_PARAM); 
    }

    double error_distance = test_cart_coord.getCartDistance(test_coord, type);

    if (debug) {
      cout << __LINE__ << ": " << "test error_distance: " << error_distance << endl;
    }

    if (error_distance > cartesian_precision) {
      stringstream ss;
      ss << "er dist(" << error_distance << ") > cart dist(" << cartesian_precision << ")";

      WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM, ss.str().c_str()); 
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
    stringstream ss;
    ss << "test br(" << test_bearing_pd << ") != curr br(" << curr_bearing_pd << ")";
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM, ss.str().c_str());
  }

  double test_distance = curr_coord.getGreatCircleDistance( new_coord );
  PDouble test_distance_pd = PDouble(test_distance, precision);
  PDouble curr_distance_pd = PDouble(curr_distance, precision);

  if (debug) {
    cout << __LINE__ << ": " << "curr_distance: " << curr_distance << "," << curr_distance_pd
         << "; test_distance: " << test_distance << "," << test_distance_pd << endl;
  }

  if (test_distance_pd != curr_distance_pd) {
    stringstream ss;
    ss << "test dst(" << test_distance_pd << ") != curr dst(" << curr_distance_pd << ")";
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM, ss.str().c_str());
  }
  
  double cart_distance = curr_coord.getCartDistance( new_coord );
  PDouble cart_distance_pd = PDouble(curr_distance, precision);

  if (debug) {
    cout << __LINE__ << ": " << "curr_distance: " << curr_distance << "," << curr_distance_pd
         << "; cart_distance: " << cart_distance << "," << cart_distance_pd << endl;
  }

  if (curr_distance_pd != cart_distance_pd) {
    stringstream ss;
    ss << "cur dst(" << curr_distance_pd << ") != cart dst(" << cart_distance_pd << ")";
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM, ss.str().c_str());
  }
}

void WossCoordDefTest::doRun() {

  for (double i = start_lon; i <= end_lon; i += step_lon) {
    vector_lon.emplace_back(i);
  }

  for (double i = start_lat; i <= end_lat; i += step_lat) {
    vector_lat.emplace_back(i);
  }

  for (double i = start_bearing; i < end_bearing; i += step_bearing) {
    vector_bearing.emplace_back(i * M_PI / 180.0);
  }

  for (double i = start_distance; i <= end_distance; i += step_distance) {
    vector_dist.emplace_back(i);
  }

  for (double i = start_depth; i <= end_depth; i += step_depth) {
    vector_depth.emplace_back(i);
  }

  for (auto curr_lon : vector_lon) {
    if (debug) {
      cout << __LINE__ << ": " << "curr_lon: " << curr_lon << endl;
    }

    for (auto curr_lat : vector_lat) {
      if (debug) {
        cout << __LINE__ << ": " << "curr_lat: " << curr_lat << endl;
      }

      for (auto curr_depth : vector_depth) {
        if (debug) {
          cout << __LINE__ << ": " << "curr_depth: " << curr_depth << endl;
        }
        CoordZ curr_coord(curr_lat, curr_lon, curr_depth);

        if (debug) {
          cout << __LINE__ << ": " << "curr_coord: " << curr_coord << endl;
        }

        if (false == curr_coord.isValid()) {
          WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_INVALID_PARAM); 
        }

        // Do geographical coordinates to Cartesian Coordinates tests
        doCoordCartTests(curr_coord);

        for (auto curr_bear : vector_bearing) {
          if (debug) {
            cout << __LINE__ << ": " << "curr_bear: " << curr_bear << ", " << curr_bear * 180.0 / M_PI << endl;
          }

          for (auto curr_distance : vector_dist) {
            if (debug) {
              cout << __LINE__ << ": " << "curr_distance: " << curr_distance << endl;
            }

            CoordZ new_coord = CoordZ( Coord::getCoordFromBearing(curr_coord, curr_bear, curr_distance), curr_depth);

            if (debug) {
              cout << __LINE__ << ": " << "curr_coord: " << curr_coord << "; new_coord: " << new_coord << endl;
            }

            if (false == new_coord.isValid()) {
              WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_INVALID_PARAM); 
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
  auto woss_def_test = make_unique<WossCoordDefTest>();

  woss_def_test->run();

  return 0;
}
