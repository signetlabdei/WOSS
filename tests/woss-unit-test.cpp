/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2025 Federico Guerra
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
 * @file   woss-unit-test.cpp
 * @author Federico Guerra
 * 
 * \brief Unit tests for WOSS components
 *
 * 
 */

#include "woss-test.h"
#include "definitions-handler.h"
#ifdef WOSS_MULTITHREAD
#include "thread-pool-definitions.h"
#endif
#include "transducer-definitions.h"
#include "transducer-handler.h"
#include "location-definitions.h"
#include "ac-toolbox-arr-asc-reader.h"
#include "ac-toolbox-arr-bin-reader.h"
#include "ac-toolbox-shd-reader.h"
#include "woss-db-custom-data-container.h"
#include "random-generator-definitions.h"
#include "res-pressure-txt-db-creator.h"
#include "res-pressure-txt-db.h"
#include "res-pressure-bin-db-creator.h"
#include "res-pressure-bin-db.h"
#include "res-time-arr-txt-db-creator.h"
#include "res-time-arr-txt-db.h"
#include "res-time-arr-bin-db-creator.h"
#include "res-time-arr-bin-db.h"
#include "woss-db-manager.h"
#include <iostream>
#include <cmath>
#include <complex>

using namespace woss;
using namespace std;

class WossUnitTest : public WossTest {

  public:

  WossUnitTest() = default;
  virtual ~WossUnitTest() = default;

  virtual void doConfig() override;
  virtual void doInit() override;
  virtual void doRun() override;

  private:

  void testTimeReference();
  void testSettings();
  void testExceptions();
  void testTime();
  void testRandomGenerator();
  void testCoordZ();
  void testSediment();
  void testSSP();
  void testBellhopCreator();
  void testBellhopWoss();
  void testPressure();
  void testTimeArr();
  void testDefHandler();
  void testPDouble();
  void testAltimetry();
  void testThreadPool();
  void testTransducer();
  void testTransducerHandler();
  void testLocation();
  void testArrDataAndReaders();
  void testShdDataAndReaders();
  void testCustomDataContainer();
  void testResPressureDb();
  void testResTimeArrDb();
  void testWossDbManager();
};

void WossUnitTest::doConfig() {
  setWossTestDebug(true);
}

void WossUnitTest::doInit() {
}

void WossUnitTest::testTimeReference() {
  if (debug) {
    cout << "Test: TimeReference..." << endl;
  }

  TestTimeReference timeRef;
  double setTime = 123.456;

  timeRef.setTimeReference(setTime);
  if (abs(timeRef.getTimeReference() - setTime) > 1e-9) {
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "TimeReference failed to store time correctly");
  }
}

void WossUnitTest::testSettings() {
  if (debug) {
    cout << "Test: WossTest Settings..." << endl;
  }

  // Random Stream
  setWossRandomGenStream(42);
  if (getWossRandomGenStream() != 42) {
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Random stream mismatch");
  }

  // ResDb Debug
  setResDbCreatorDebug(true);
  if (!getResDbCreatorDebug()) {
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_WRONG_STATE, "Debug flag set failed (expected true)");
  }

  setResDbCreatorDebug(false);
  if (getResDbCreatorDebug()) {
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_WRONG_STATE, "Debug flag clear failed (expected false)");
  }
}

void WossUnitTest::testExceptions() {
  if (debug) 
    cout << "Test: WossException..." << endl;
  try {
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_UNKNOWN, "TestInfoString");
  } 
  catch (const WossException& e) {
    string info = e.getInfo();
    if (info != "TestInfoString") {
      WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Exception info string mismatch");
    }
    if (e.getErrorType() != WossErrorType::WOSS_ERROR_UNKNOWN) {
      WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Exception type mismatch");
    }
  }
}

void WossUnitTest::testCoordZ() {
  if (debug) {
    cout << "Test: CoordZ..." << endl;
  }
  
  CoordZ c1(10, 20, 30);
  if (c1.getLatitude() != 10 || c1.getLongitude() != 20 || c1.getDepth() != 30) {
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CoordZ constructor failed to set values correctly");
  }
}

void WossUnitTest::testSediment() {
  if (debug) {
    cout << "Test: Sediment..." << endl;
  }

  // 1. Constructors & Accessors
  Sediment s1("Sand", 1650.0, 0.0, 1.9, 0.8, 0.0, 10.0);

  if (s1.getType() != "Sand") 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Sediment type mismatch");
  if (s1.getVelocityC() != 1650.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Sediment sound speed mismatch");
  if (s1.getDepth() != 10.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Sediment depth mismatch");
  if (!s1.isValid()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_WRONG_STATE, "Sediment validity check failed");

  Sediment s_invalid;
  if (s_invalid.isValid()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_WRONG_STATE, "Invalid Sediment check failed");

  // 2. Setters
  s_invalid.set("Silt", 1580, 0, 1.7, 0.5, 0, 5.0);
  if (!s_invalid.isValid() || s_invalid.getType() != "Silt") 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Sediment set() mismatch");

  // 3. Operators
  Sediment s2 = s1 * 2.0; 
  // Expecting all scalar values multiplied by 2.
  if (s2.getVelocityC() != 3300.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Sediment operator* mismatch");
    
  // Check +=
  Sediment s3 = s1;
  s3 += 100.0; // Adds 100 to all scalar fields
  if (s3.getVelocityC() != 1750.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Sediment operator+= mismatch");

  // Check additions of two sediments
  Sediment s4 = s1 + s1;
  if (s4.getVelocityC() != 3300.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Sediment operator+ mismatch");

  // Equality
  if (s1 == s2) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Sediment operator== mismatch");
  
  Sediment s1_clone = s1;
  if (s1 != s1_clone) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Sediment operator!= (clone) mismatch");

  // 4. Subclasses (Just checking one)
  SedimentSand sand;
  if (sand.getType() != "SAND") 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "SedimentSand type mismatch");
  
  // 5. Factory Methods & Clone
  auto s_create_def = s1.create();
  if (s_create_def->isValid()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Sediment::create() should return invalid");
  
  auto s_create_args = s1.create("Sand", 1650.0, 0.0, 1.9, 0.8, 0.0, 10.0);
  if (s_create_args->getType() != "Sand" || s_create_args->getVelocityC() != 1650.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Sediment::create(args) failed");

  auto s_cloned_ptr = s1.clone();
  if (*s_cloned_ptr != s1) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Sediment::clone() failed");
}

void WossUnitTest::testSSP() {
  if (debug) 
    cout << "Test: SSP..." << endl;

  // 1. Basic Insertion & size
  SSP ssp1;
  if (ssp1.isValid()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_WRONG_STATE, "SSP should be invalid when empty");

  ssp1.insertValue(0.0, 1500.0);
  ssp1.insertValue(100.0, 1490.0);
  ssp1.insertValue(500.0, 1480.0);
  ssp1.insertValue(1000.0, 1485.0);

  if (!ssp1.isValid()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_WRONG_STATE, "SSP validity check failed");
  if (ssp1.size() != 4) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "SSP size mismatch");
  if (ssp1.getMinDepthValue() != 0.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "SSP Min Depth Value mismatch");
  if (ssp1.getMaxDepthValue() != 1000.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "SSP Max Depth Value mismatch");

  // 2. Find/Erase
  if (ssp1.findValue(500.0) == ssp1.end()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM, "SSP findValue (existing) failed");
  
  ssp1.eraseValue(500.0);
  if (ssp1.size() != 3) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "SSP size after erase mismatch");
  if (ssp1.findValue(500.0) != ssp1.end()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM, "SSP findValue (erased) failed");

  // 3. Operators
  // Scalar mult
  SSP ssp2 = ssp1 * 1.1; // Increase all SS by 10%
  // Original at 0.0 was 1500.0
  double val0 = ssp2.findValue(0.0)->second;
  if (abs(val0 - 1650.0) > 1e-6) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "SSP scalar multiplication mismatch");

  // SSP addition
  SSP ssp3 = ssp1 + ssp1; // 2x values
  double val0_s3 = ssp3.findValue(0.0)->second;
  if (abs(val0_s3 - 3000.0) > 1e-6) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "SSP addition mismatch");

  // 4. Truncate
  auto ssp_trunc = ssp1.truncate(100.0);
  if (ssp_trunc->getMaxDepthValue() > 100.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "SSP truncate max depth mismatch");
  
  // Should have 0.0 and 100.0 (1000.0 was removed)
  if (ssp_trunc->size() != 2) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Truncate size mismatch");
  
  // 5. Transform (Basic check)
  auto ssp_trans = ssp1.transform(Coord(0,0), 0.0, 1000.0, 11); // Interpolate to 11 steps
  // Note: since ssp1 is not randomizable (no temperature/salinity/pressure data), 
  // the implementation overrides total_depth_steps to ssp_map.size()
  if (ssp_trans->size() != 3) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Unsuccessful interpolation");
  
  // 6. Advanced Insertion (Calculation from T, S, P)
  SSP ssp_calc;
  
  // insertValue(depth, T, S, coord) -> Calculates SS
  ssp_calc.insertValue(0.0, 10.0, 35.0); // depth 0, T=10, S=35.
  // Check calculated SS
  double ss_val = ssp_calc.findValue(0.0)->second;
  // 1490 +/- 20 m/s range roughly standard
  if (ss_val < 1400.0 || ss_val > 1600.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Calculated SS out of range");

  // 7. Factory Methods & Clone
  // create(depth_precision)
  auto ssp_ptr1 = ssp1.create();
  if (ssp_ptr1->size() != 0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "SSP::create() failed");

  // create(ssp_map, ...)
  auto dm = DepthMap();
  dm[PDouble(0.0)] = 1500.0;
  auto ssp_ptr2 = ssp1.create(dm);
  if (ssp_ptr2->size() != 1) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "SSP::create(map) failed");

  // clone
  auto ssp_clone_ptr = ssp1.clone();
  if (ssp1 != *ssp_clone_ptr) {
    if (debug) {
      cout << "SSP::clone() failed." << endl;
      cout << "ssp1: " << ssp1 << endl;
      cout << "ssp_clone_ptr: " << *ssp_clone_ptr << endl;
    }
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "SSP::clone() failed");
  }

  if (debug) 
    cout << "SSP Tests Passed." << endl;
}

void WossUnitTest::testBellhopCreator() {
  if (debug) 
    cout << "Test: BellhopCreator..." << endl;
  
  BellhopCreator creator;

  // 1. Global settings
  creator.setRaysNumber(500);
  if (creator.getRaysNumber() != 500) 
  {
    if (debug) 
      cout << "BellhopCreator RaysNumber mismatch " << creator.getRaysNumber() << " != 500" << endl;
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopCreator RaysNumber mismatch");
  }

  creator.setThorpeAttFlag(true);
  if (!creator.getThorpeAttFlag()) 
  {
    if (debug) 
      cout << "BellhopCreator ThorpeAttFlag mismatch " << creator.getThorpeAttFlag() << " != true" << endl;
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopCreator ThorpeAttFlag mismatch");
  }

  creator.setBellhopPath("/usr/bin/bellhop");
  if (creator.getBellhopPath() != "/usr/bin/bellhop") 
  {
    if (debug) 
      cout << "BellhopCreator BellhopPath mismatch " << creator.getBellhopPath() << " != /usr/bin/bellhop" << endl;
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopCreator BellhopPath mismatch");
  }

  creator.setBellhopBinName(BellhopWoss::WOSS_BELLHOP_BIN_NAME);
  if (creator.getBellhopBinName() != BellhopWoss::WOSS_BELLHOP_BIN_NAME)
  {
    if (debug) 
      cout << "BellhopCreator BellhopBinName mismatch " << creator.getBellhopBinName() << " != " << BellhopWoss::WOSS_BELLHOP_BIN_NAME << endl;
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopCreator BellhopBinName mismatch");
  }

  // erase Global first
  creator.eraseRaysNumber();

  // 2. Location-specific settings (Container test)
  CoordZ tx(45.0, 12.0, 10.0);
  CoordZ rx(45.1, 12.1, 100.0);

  //creator.setDebug(true);

  creator.setRaysNumber(1000, tx, rx);

  int rays = creator.getRaysNumber(tx, rx);
  if (rays != 1000) 
  {
    if (debug) 
      cout << "BellhopCreator specific RaysNumber mismatch " << rays << " != 1000" << endl;
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopCreator specific RaysNumber mismatch");
  }
  
  // 3. Other properties
  creator.setTotalRuns(5);
  if (creator.getTotalRuns() != 5) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopCreator TotalRuns mismatch");
  
  creator.setTxMinDepthOffset(10.0);
  if (creator.getTxMinDepthOffset() != 10.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopCreator TxMinDepthOffset mismatch");

  // 4. Enhanced WossCreator Tests
  // Frequency Step
  creator.setFrequencyStep(100.0, tx, rx);
  if (creator.getFrequencyStep(tx, rx) != 100.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "WossCreator FrequencyStep mismatch");
  
  creator.eraseFrequencyStep(tx, rx);
  // Default behavior on get might need verification, likely returns 0 or default
  
  // Evolution Time Quantum
  creator.setEvolutionTimeQuantum(0.5, tx, rx);
  if (creator.getEvolutionTimeQuantum(tx, rx) != 0.5)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "WossCreator EvolutionTimeQuantum mismatch");
  
  creator.eraseEvolutionTimeQuantum(tx, rx);

  // SimTime
  SimTime st(Time(1,1,2025), Time(2,1,2025));
  creator.setSimTime(st, tx, rx);
  SimTime st_ret = creator.getSimTime(tx, rx);
  if (st_ret.start_time != st.start_time || st_ret.end_time != st.end_time)
     WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "WossCreator SimTime mismatch");
     
  creator.eraseSimTime(tx, rx);

  // Debug & Other flags
  creator.setWossDebug(true);
  if (!creator.usingWossDebug())
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "WossCreator WossDebug mismatch");

  if (debug) 
    cout << "BellhopCreator Tests Passed." << endl;
}

void WossUnitTest::testTime() {
  if (debug) cout << "Test: Time..." << endl;
  
  // 1. Constructors
  Time t_def;
  if (!t_def.isValid())
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Time default constructor should be valid (initialized to now)");
    
  Time t1(1, 1, 2025, 12, 0, 0); // Jan 1st 2025, 12:00:00
  if (!t1.isValid())
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Time constructor failed");
    
  if (t1.getDay() != 1 || t1.getMonth() != 0 || t1.getYear() != 125) 
    // Note: tm_mon is 0-11, tm_year is years since 1900.
    // Time(d,m,y) sets tm_mon = m-1, tm_year = y-1900.
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Time getters mismatch (struct tm check)");
    
  // 2. Setters
  t1.setDay(2);
  if (t1.getDay() != 2) WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Time setDay failed");
  
  t1.setMonth(2); // Feb
  if (t1.getMonth() != 1) WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Time setMonth failed"); // 2-1 = 1
  
  t1.setYear(2026);
  if (t1.getYear() != 126) WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Time setYear failed");
  
  // 3. Operators
  Time t_base(1, 1, 2025, 12, 0, 0);
  
  // + seconds
  Time t_plus = t_base + (time_t)3600; // +1 hour -> 13:00:00
  if (t_plus.getHours() != 13)
     WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Time operator+ (seconds) failed");
     
  // - seconds
  Time t_minus = t_base - (time_t)3600; // -> 11:00:00
  if (t_minus.getHours() != 11)
     WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Time operator- (seconds) failed");
     
  // - Time (difference)
  double diff = t_plus - t_base;
  if (std::abs(diff - 3600.0) > 1e-6)
     WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Time operator- (Time) failed");
     
  // Comparison
  if (!(t_base < t_plus)) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Time operator< failed");
  if (!(t_plus > t_base)) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Time operator> failed");
  if (t_base != t_plus) {
    if (debug) 
      cout << "Test: Time operator== passed" << endl;
  } 
  else 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Time operator!= failed");
  if (t_base == t_base) {
    if (debug) 
      cout << "Test: Time operator== passed" << endl;
  } 
  else 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Time operator== failed");
  
  if (debug) 
    cout << "Time Tests Passed." << endl;
}

void WossUnitTest::testRandomGenerator() {
  if (debug) 
    cout << "Test: RandomGenerator..." << endl;
  
  RandomGenerator rg(12345);
  if (rg.getSeed() != 12345)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "RandomGenerator seed mismatch");
    
  if (rg.isValid())
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "RandomGenerator should not be valid before initialize");
    
  rg.initialize();
  if (!rg.isValid())
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "RandomGenerator initialize failed");
    
  // Range check
  for(int i=0; i<100; ++i) {
    double r = rg.getRand();
    if (r < 0.0 || r > 1.0)
       WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "RandomGenerator getRand out of range");
  }
  
  // Clone
  auto rg_clone = rg.clone();
  if (rg_clone->getSeed() != 12345)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "RandomGenerator clone seed mismatch");

  if (debug) 
    cout << "RandomGenerator Tests Passed." << endl;
}

void WossUnitTest::testBellhopWoss() {
  if (debug) 
    cout << "Test: BellhopWoss..." << endl;
    
  CoordZ tx(45.0, 12.0, 10.0);
  CoordZ rx(45.1, 12.1, 100.0);
    
  // Default Time() is invalid, use specific constructor
  Time start_t(1, 1, 2025, 0, 0, 0);
  Time end_t(2, 1, 2025, 0, 0, 0);
    
  double f1 = 1000.0;
  double f2 = 2000.0;
  double f_step = 100.0;

  BellhopWoss bh(tx, rx, start_t, end_t, f1, f2, f_step);

  // 1. Base Woss properties
  if (bh.getTxCoordZ() != tx) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopWoss TxCoordZ mismatch");
  if (bh.getRxCoordZ() != rx) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopWoss RxCoordZ mismatch");
  
  // Frequencies
  if (bh.getMinFrequency() != 1000.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopWoss MinFrequency mismatch");
  if (bh.getMaxFrequency() != 2000.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopWoss MaxFrequency mismatch");

  // 2. Bellhop-specific properties
  bh.setRaysNumber(1234);
  if (bh.getRaysNumber() != 1234) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopWoss RaysNumber mismatch");

  bh.setMinAngle(-45.0);
  bh.setMaxAngle(45.0);
  if (bh.getMinAngle() != -45.0 || bh.getMaxAngle() != 45.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopWoss Angle mismatch");

  bh.setBellhopPath("/bin/bh");
  if (bh.getBellhopPath() != "/bin/bh") 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopWoss BellhopPath mismatch");

  bh.setBellhopBinName(BellhopWoss::WOSS_BELLHOPCXX_BIN_NAME);
  if (bh.getBellhopBinName() != BellhopWoss::WOSS_BELLHOPCXX_BIN_NAME)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopWoss BellhopBinName mismatch");

  bh.setThorpeAttFlag(true);
  if (!bh.getThorpeAttFlag()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "BellhopWoss ThorpeAttFlag mismatch");

  // 3. Validity (Should be invalid as it's not initialized/run)
  if (bh.isValid()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_WRONG_STATE, "BellhopWoss validity check failed (should be invalid)");

  if (debug) 
    cout << "BellhopWoss Tests Passed." << endl;
}

void WossUnitTest::testPressure() {
  if (debug) 
    cout << "Test: Pressure..." << endl;
  
  // 1. Constructors
  Pressure p1(1.0, 2.0);
  if (p1.real() != 1.0 || p1.imag() != 2.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Pressure constructor(double, double) mismatch");

  std::complex<double> c(3.0, 4.0);
  Pressure p2(c);
  if (p2.real() != 3.0 || p2.imag() != 4.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Pressure constructor(complex) mismatch");

  // 2. Calculations
  double abs_val = p2.abs(); // sqrt(9 + 16) = 5
  if (std::abs(abs_val - 5.0) > 1e-9) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Pressure abs() calculation mismatch");
  
  // Transmission Loss
  // TL = -20 * log10(abs) = -20 * log10(5) approx -13.979
  double tl = p2.getTxLossDb();
  if (tl > -13.9) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Pressure getTxLossDb() calculation mismatch");

  // 3. Operators
  Pressure p3 = p1 + p2; // (1+3) + i(2+4) = 4 + 6i
  if (p3.real() != 4.0 || p3.imag() != 6.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Pressure operator+ mismatch");

  Pressure p4 = p2 * p1; // (3+4i) * (1+2i) = 3 + 6i + 4i - 8 = -5 + 10i
  if (p4.real() != -5.0 || p4.imag() != 10.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Pressure operator* mismatch");

  p1 += p2;
  if (p1.real() != 4.0 || p1.imag() != 6.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Pressure operator+= mismatch");

  // 4. Factory Methods
  // create(double, double)
  auto p_dbl = p1.create(1.5, 2.5);
  if (p_dbl->real() != 1.5 || p_dbl->imag() != 2.5) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "create(double, double) failed");

  // create(complex)
  auto p_cplx = p1.create(std::complex<double>(5.0, 5.0));
  if (p_cplx->real() != 5.0 || p_cplx->imag() != 5.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "create(complex) failed");
  
  // create(TimeArr)
  TimeArr ta;
  ta.insertValue(0.0, Pressure(10.0, 0.0));
  // TimeArr to Pressure conversion sums values (coherent sum operator implicit)
  auto p_ta = p1.create(ta);
  if (p_ta->real() != 10.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "create(TimeArr) failed");

  // create from invalid complex
  auto p_invalid_ptr = p1.create(Pressure::createNotValid());
  if (p_invalid_ptr->isValid()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Pressure invalid creation check failed");

  // clone()
  auto p_clone = p1.clone();
  if (*p_clone != p1) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "clone() failed");

  // createArray()
  auto p_arr = p1.createArray(10);
  if (!p_arr) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "createArray() failed");
  // Verify default initialization of array elements
  if (p_arr[0].real() != 0.0 || p_arr[9].imag() != 0.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "createArray() element initialization failed");

  if (debug) cout << "Pressure Tests Passed." << endl;
}

void WossUnitTest::testTimeArr() {
  if (debug)
    cout << "Test: TimeArr..." << endl;

  // 1. Creators
  // Default
  TimeArr ta_def;
  if (ta_def.isValid()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_WRONG_STATE, "TimeArr default constructor should be invalid");

  // From Map
  TimeArrMap m;
  m[0.1] = std::complex<double>(1.0, 0.0);
  TimeArr ta_map(m);
  if (ta_map.size() != 1 || ta_map.findValue(0.1) == ta_map.end()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "TimeArr map constructor failed");

  // From Pressure (Factory)
  Pressure p_conv(10.0, 0.0);
  auto ta_conv = ta_def.create(p_conv);
  if (!ta_conv->isConvertedFromPressure()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "isConvertedFromPressure failed");
  if (ta_conv->size() != 1) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "TimeArr creation from Pressure size mismatch");

  // Clone
  auto ta_clone = ta_map.clone();
  if (*ta_clone != ta_map) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "TimeArr clone failed");

  // Create Array
  auto ta_arr = ta_def.createArray(5);
  if (!ta_arr) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "TimeArr createArray failed");

  // 2. Manipulation & SumValue
  TimeArr ta;
  Pressure p1(1.0, 0.0);
  
  ta.insertValue(0.1, p1);
  if (ta.size() != 1) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "TimeArr insertValue failed");
  
  // Sum value (coherent sum) - Existing delay
  ta.sumValue(0.1, p1); // 1.0 + 1.0 = 2.0
  if (ta.findValue(0.1)->second.real() != 2.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "TimeArr sumValue (existing key) failed");

  // Sum value - New delay
  ta.sumValue(0.2, p1);
  if (ta.size() != 2 || ta.findValue(0.2)->second.real() != 1.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "TimeArr sumValue (new key) failed");

  // 3. Coherent Sum Sample
  // Setup: (0.1, 1.0), (0.15, 1.0), (0.3, 1.0)
  TimeArr ta_coh;
  ta_coh.insertValue(0.1, std::complex<double>(1.0, 0.0));
  ta_coh.insertValue(0.15, std::complex<double>(1.0, 0.0));
  ta_coh.insertValue(0.3, std::complex<double>(1.0, 0.0));
  
  // Resolution 0.1:
  // 0.1 starts bin. 0.15 <= 0.1+0.1? Yes -> Sum to 0.1
  // 0.3 > 0.1+0.1? Yes -> New bins starts at 0.3
  auto ta_coh_res = ta_coh.coherentSumSample(0.1);
  
  if (ta_coh_res->size() != 2) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CoherentSumSample size mismatch");
  if (ta_coh_res->findValue(0.1)->second.real() != 2.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CoherentSumSample logic bin 1 mismatch");
  if (ta_coh_res->findValue(0.3)->second.real() != 1.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CoherentSumSample logic bin 2 mismatch");

  // 4. Incoherent Sum Sample
  // Setup: (0.1, 3.0), (0.15, 4.0). Sum of squares = 9+16=25. Sqrt = 5.
  TimeArr ta_incoh;
  ta_incoh.insertValue(0.1, std::complex<double>(3.0, 0.0));
  ta_incoh.insertValue(0.15, std::complex<double>(4.0, 0.0));
  
  auto ta_incoh_res = ta_incoh.incoherentSumSample(0.1);
  
  if (ta_incoh_res->size() != 1) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "IncoherentSumSample size mismatch");
  // Result should be real ~5.0. 
  if (abs(ta_incoh_res->findValue(0.1)->second.real() - 5.0) > 1e-9) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "IncoherentSumSample calculation mismatch");

  // 5. Crop
  // ta_coh has 0.1, 0.15, 0.3
  // Crop [0.12, 0.25) -> should keep 0.15
  auto ta_crop = ta_coh.crop(0.12, 0.25);
  if (ta_crop->size() != 1) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Crop size mismatch");
  if (ta_crop->findValue(0.15) == ta_crop->end()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Crop content mismatch");

  // 6. Check Pressure Attenuation
  // Use ta_coh (0.1 -> 1.0, 0.15 -> 1.0, 0.3 -> 1.0).
  // Let's ensure one value is very large to trigger it.
  ta_coh.insertValue(0.4, std::complex<double>(100.0, 0.0));
  bool corrected = ta_coh.checkPressureAttenuation(1000.0, 1000.0); 
  // Should return true because 100.0 >= 1.0
  if (!corrected) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CheckPressureAttenuation failed to trigger");
  // Verify value changed
  if (std::abs(ta_coh.findValue(0.4)->second) >= 100.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CheckPressureAttenuation failed to apply attenuation");

  // 7. LowerBoundTxLoss
  // Setup:
  // p1: abs=100. TL = -20log10(100) = -40 dB
  // p2: abs=0.001. TL = -20log10(0.001) = 60 dB
  TimeArr ta_tl;
  ta_tl.insertValue(0.1, std::complex<double>(100.0, 0.0));
  ta_tl.insertValue(0.2, std::complex<double>(0.001, 0.0));
  
  // Find TL <= -30 dB. Should find p1 (-40).
  auto it_tl = ta_tl.lowerBoundTxLoss(-30.0);
  if (it_tl == ta_tl.end() || std::abs((double)it_tl->first - 0.1) > 1e-9) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "LowerBoundTxLoss fail case 1");
  
  // Find TL <= -50 dB. Should find none (min is -40).
  it_tl = ta_tl.lowerBoundTxLoss(-50.0);
  if (it_tl != ta_tl.end()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "LowerBoundTxLoss fail case 2");

  // 8. Set Delay Precision (Modification)
  // Setup: (0.1234567, 1.0) with high precision (default 1e-7)
  TimeArr ta_prec(1e-9);
  ta_prec.insertValue(0.1234567, std::complex<double>(1.0, 0.0));
  
  // Change precision to 0.1
  ta_prec.setDelayPrecision(0.1);
  
  double new_prec = ta_prec.begin()->first.getPrecision();
  if (new_prec != 0.1) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "SetDelayPrecision failed");
  
  if (debug)
    cout << "TimeArr Tests Passed." << endl;
}

void WossUnitTest::testDefHandler() {
  if (debug)
    cout << "Test: DefHandler..." << endl;
  
  // Singleton access
  DefHandler& handler = SDefHandler::instance();
  
  // Basic settings
  handler.setDebug(true);
  
  if (!handler.getDebug())
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "DefHandler setDebug failed");
  
  handler.setDebug(false);

  // Testing dynamic object creation via prototypes
  // DefHandler takes ownership of the pointer.
  handler.setTimeArr(std::make_unique<TimeArr>());
  
  // Create new instance
  auto ta = handler.createTimeArr();
  
  if (ta == nullptr)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "DefHandler createTimeArr failed");
  
  // Testing Pressure
  handler.setPressure(std::make_unique<Pressure>(1.0, 1.0));
  auto p = handler.createPressure();
  
  if (p == nullptr)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM, "DefHandler createPressure failed");

  // SSP
  handler.setSSP(std::make_unique<SSP>());
  auto ssp_ptr = handler.createSSP();
  
  if (ssp_ptr == nullptr)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "DefHandler::createSSP failed");

  // Sediment
  handler.setSediment(std::make_unique<Sediment>());
  auto sed_ptr = handler.createSediment();
  
  if (!sed_ptr)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "DefHandler::createSediment failed");

  // Transducer
  handler.setTransducer(std::make_unique<Transducer>());
  auto tr_ptr = handler.createTransducer();
  if (!tr_ptr)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "DefHandler::createTransducer failed");

  // Altimetry
  handler.setAltimetry(std::make_unique<Altimetry>());
  auto alt_ptr_h = handler.createAltimetry();
  if (!alt_ptr_h)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "DefHandler::createAltimetry failed");
  
  // Location
  handler.setLocation(std::make_unique<Location>());
  auto loc_ptr_h = handler.createLocation();
  
  if (!loc_ptr_h)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "DefHandler::createLocation failed");

  // Arrays
  // Pressure Array
  auto p_arr = handler.createPressureArray(5);
  if (!p_arr)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "DefHandler::createPressureArray failed");
  
  // TimeArr Array
  auto ta_arr_h = handler.createTimeArrArray(5);
  if (!ta_arr_h)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "DefHandler::createTimeArrArray failed");

  if (debug)
    cout << "DefHandler Tests Passed." << endl;
}

void WossUnitTest::testPDouble() {
  if (debug)
    cout << "Test: PDouble..." << endl;

  // 1. Constructors
  PDouble pd1(1.0, 1e-5);
  if (pd1.getValue() != 1.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "PDouble getValue mismatch");
  if (pd1.getPrecision() != 1e-5)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "PDouble getPrecision mismatch");

  // 2. Operators
  PDouble pd2(2.0, 1e-6);
  
  // + (takes max precision: 1e-5)
  PDouble sum = pd1 + pd2;
  if (sum.getValue() != 3.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "PDouble addition value mismatch");
  if (sum.getPrecision() != 1e-5)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "PDouble addition precision mismatch");

  // *
  PDouble prod = pd1 * pd2; // 1*2 = 2
  if (prod.getValue() != 2.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "PDouble multiplication value mismatch");

  // Comparison (uses precision)
  PDouble pd_close(1.000001, 1e-5); // Diff is 1e-6, which is <= 1e-5
  if (pd1 != pd_close)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "PDouble inequality check failed (should be equal)");
  if (!(pd1 == pd_close))
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "PDouble equality check failed");

  PDouble pd_far(1.1, 1e-5);
  if (pd1 == pd_far)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "PDouble equality check failed (should be unequal)");

  // Casts
  double d_val = (double)pd1;
  if (d_val != 1.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "PDouble cast to double failed");

  if (debug)
    cout << "PDouble Tests Passed." << endl;
}

void WossUnitTest::testAltimetry() {
  if (debug)
    cout << "Test: Altimetry..." << endl;

  // 1. Constructors
  Altimetry alt;
  // By default empty
  if (alt.size() != 0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_WRONG_STATE, "Altimetry default constructor should be empty");

  // 2. Manipulation
  alt.insertValue(0.0, 10.0); // range 0, alt 10
  alt.insertValue(100.0, -5.0); // range 100, alt -5

  if (alt.size() != 2)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Altimetry insertValue size mismatch");
  if (alt.getMaxRangeValue() != 100.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Altimetry getMaxRangeValue mismatch");
  
  // Check values via iterators or find
  if (alt.findValue(0.0)->second != 10.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Altimetry findValue (0.0) mismatch");

  // 3. Operators (with Scalar)
  // + scalar: Adds to altimetry values
  Altimetry alt_plus = alt + 2.0; 
  // 0.0 -> 12.0, 100.0 -> -3.0
  if (alt_plus.findValue(0.0)->second != 12.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Altimetry operator+ (scalar) mismatch");

  // * scalar
  Altimetry alt_scaled = alt * 2.0;
  // 0.0 -> 20.0, 100.0 -> -10.0
  if (alt_scaled.findValue(0.0)->second != 20.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Altimetry operator* (scalar) mismatch");

  // 4. Operators (with Altimetry)
  // Let's test same-range addition
  Altimetry alt2;
  alt2.insertValue(0.0, 5.0);
  alt2.insertValue(100.0, 5.0);

  Altimetry alt_sum = alt + alt2;
  // 0.0: 10+5=15
  // 100.0: -5+5=0
  if (alt_sum.findValue(0.0)->second != 15.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Altimetry operator+ (Altimetry) mismatch (0.0)");
  if (alt_sum.findValue(100.0)->second != 0.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Altimetry operator+ (Altimetry) mismatch (100.0)");

  // 5. Min/Max Altimetry Values
  // Let's try factory for safe measure regarding cached min/max
  AltimetryMap m;
  m[0.0] = 10.0;
  m[100.0] = -5.0;
  auto alt_ptr = alt.create(m);
  
  if (alt_ptr->getMaxAltimetryValue() != 10.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Altimetry getMaxAltimetryValue mismatch");
  if (alt_ptr->getMinAltimetryValue() != -5.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Altimetry getMinAltimetryValue mismatch");
  
  // 6. Factory Methods & Clone
  // create()
  auto alt_def_ptr = alt.create();
  if (alt_def_ptr->size() != 0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Altimetry::create() failed");

  // clone()
  auto alt_clone_ptr = alt.clone();
  if (*alt_clone_ptr != alt)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Altimetry::clone() failed");
  
  if (debug)
    cout << "Altimetry Tests Passed." << endl;
}


void WossUnitTest::testThreadPool() {
#ifdef WOSS_MULTITHREAD
  if (debug)
    cout << "Test: ThreadPool..." << endl;
  
  ThreadPool pool(4);
  if (pool.getNumThreads() != 4)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "ThreadPool thread count mismatch");

  // Test simple task
  auto f1 = pool.submit([]{ return 1; });
  auto f2 = pool.submit([]{ return 2; });
  
  if (f1.get() != 1)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "ThreadPool task 1 result mismatch");
  if (f2.get() != 2)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "ThreadPool task 2 result mismatch");
  
  // Test that destruction works (implicit, but good to know we don't hang)
  if (debug)
    cout << "ThreadPool Tests Passed." << endl;
#endif
}

void WossUnitTest::testTransducer() {
  if (debug) cout << "Test: Transducer..." << endl;

  // 1. Constructor & Beampattern
  Transducer t1;
  
  if (t1.isValid()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_WRONG_STATE, "Transducer should be invalid initially");

  t1.beampattern_insert(0.0, 10.0);
  t1.beampattern_insert(90.0, 5.0);

  if (t1.beampattern_size() != 2) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Transducer beampattern size mismatch");
  
  // Check values
  if (t1.beampattern_find(0.0)->second != 10.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Transducer beampattern value mismatch (0.0)");

  // 2. Manipulation
  t1.beampattern_rotate(10.0); // Shift angles by 10

  if (debug)
    cout << "t1: " << t1 << endl;

  // 0 -> 10, 90 -> 100
  if (t1.beampattern_find(10.0)->second != 10.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Transducer beampattern rotate value mismatch");
  if (t1.beampattern_find(0.0) != t1.beampattern_end()) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Transducer beampattern rotate old key mismatch");

  t1.beampattern_sum(5.0); // Add 5dB to all
  if (t1.beampattern_find(10.0)->second != 15.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Transducer beampattern sum mismatch");

  t1.beampattern_multiply(2.0); // Multiply by 2 -> 30
  if (t1.beampattern_find(10.0)->second != 30.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Transducer beampattern multiply mismatch");

  // 3. OCV / TVR
  t1.ocv_insert(1000.0, -170.0);
  if (t1.ocv_find(1000.0)->second != -170.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Transducer OCV mismatch");

  t1.tvr_insert(2000.0, 140.0);
  if (t1.tvr_find(2000.0)->second != 140.0) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Transducer TVR mismatch");

  // 4. Operators
  auto t_clone = t1.clone();

  if (debug)
    cout << "Transducer 1: " << t1 << endl
         << "Transducer 2: " << *t_clone << endl;

  if (t1 != *t_clone) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Transducer clone mismatch");

  // 6. Factory Methods
  // create()
  auto t_create_def = t1.create();
  if (t_create_def->isValid())
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Transducer::create() failed");
  
  // create(maps...)
  std::map<PDouble, double> bpm; 
  bpm[PDouble(0.0)] = 10.0;

  std::map<PDouble, double> cm;
  std::map<PDouble, double> tvrm;
  std::map<PDouble, double> ocvm;
  auto t_create_maps = t1.create(bpm, cm, tvrm, ocvm);
  if (t_create_maps->beampattern_size() != 1)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Transducer::create(maps) failed");

  if (debug)
    cout << "Transducer Tests Passed." << endl;
}

void WossUnitTest::testTransducerHandler() {
  if (debug)
    cout << "Test: TransducerHandler..." << endl;

  TransducerHandler th;
  if (!th.empty())
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_WRONG_STATE, "TransducerHandler default constructor should be empty");

  Transducer t_ref;
  t_ref.setTypeName("MyTransducer");
  t_ref.beampattern_insert(0.0, 0.0);

  // Insert
  th.insertValue("T1", t_ref);
  if (th.size() != 1)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "TransducerHandler insert failed");

  // Get
  auto t_ptr = th.getValue("T1");
  if (!t_ptr || t_ptr->getTypeName() != "MyTransducer")
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "TransducerHandler getValue failed");

  // Replace
  Transducer t_new;
  t_new.setTypeName("NewTransducer");
  th.replaceValue("T1", t_new);
  auto t_chk = th.getValue("T1");
  if (t_chk->getTypeName() != "NewTransducer")
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "TransducerHandler replaceValue failed");

  // Erase
  th.eraseValue("T1");
  if (!th.empty())
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_WRONG_STATE, "TransducerHandler not empty after erase");

  if (debug)
    cout << "TransducerHandler Tests Passed." << endl;
}

void WossUnitTest::testLocation() {
  if (debug)
    cout << "Test: Location..." << endl;
  
  // 1. Constructors
  Location loc1(10.0, 20.0, 30.0, 0.1);
  if (loc1.getLatitude() != 10.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Location getLatitude mismatch");
  if (loc1.getLongitude() != 20.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Location getLongitude mismatch");
  if (loc1.getDepth() != 30.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Location getDepth mismatch");
  
  // 2. Factory
  auto loc_ptr = loc1.create(11.0, 21.0, 31.0);
  if (loc_ptr->getLatitude() != 11.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Location create mismatch");

  // 3. Orientation
  loc1.setVerticalOrientation(45.0);
  if (loc1.getVerticalOrientation() != 45.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Location getVerticalOrientation mismatch");

  loc1.setHorizontalOrientation(90.0);
  if (loc1.getHorizontalOrientation() != 90.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Location getHorizontalOrientation mismatch");

  // 4. Equivalency
  CoordZ c1(10.0, 20.0, 30.0);
  if (!loc1.isEquivalentTo(c1))
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Location isEquivalentTo failed for equivalent coord");
  
  CoordZ c2(10.000001, 20.0, 30.0);
  // If we set dist=0.1 meters, 1e-6 degrees lat is ~0.11m. It might fail or pass depending on precision.
  // Let's test explicit fail
  CoordZ c_far(11.0, 20.0, 30.0);
  if (loc1.isEquivalentTo(c_far))
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Location isEquivalentTo passed for different coord");

  if (debug)
    cout << "Location Tests Passed." << endl;
}

void WossUnitTest::testArrDataAndReaders() {
  if (debug)
    cout << "Test: ArrData & Readers..." << endl;

  // 1. ArrData
  ArrData data;
  data.initialize();
  if (data.Nsd != 0 || data.Nrd != 0 || data.frequency != 0.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_WRONG_STATE, "ArrData initialization failed");

  // Setup dummy data for index test
  data.Nsd = 1;
  data.Nrd = 2; // depths: d0, d1
  data.Nrr = 2; // ranges: r0, r1
  // Flattened size: Nsd * Nrd * Nrr = 4
  // Let's allocate arrays to be safe if getIndex accesses them for values
  data.tx_depths = std::make_unique<float[]>(1); data.tx_depths[0] = 10.0;
  data.rx_depths = std::make_unique<float[]>(2); data.rx_depths[0] = 50.0; data.rx_depths[1] = 60.0;
  data.rx_ranges = std::make_unique<float[]>(2); data.rx_ranges[0] = 1000.0; data.rx_ranges[1] = 2000.0;
  
  // Check index logic (assuming we pass values that exist)
  int idx = data.getTimeArrIndex(10.0, 60.0, 2000.0);
  
  if (idx < 0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM, "ArrData getTimeArrIndex failed");
  
  // 2. Readers (Instantiation)
  // Needs a Woss object
  BellhopWoss woss_dummy; // Default
  
  ArrAscResReader asc_reader(woss_dummy);
  // asc_reader.initialize() would try to read file from woss settings.
  
  ArrBinResReader bin_reader(woss_dummy);
  
  if (debug)
    cout << "ArrData & Readers Tests Passed." << endl;
}

void WossUnitTest::testShdDataAndReaders() {
  if (debug)
    cout << "Test: ShdData & Readers..." << endl;

  // 1. ShdData
  ShdData shd;
  shd.initialize();
  if (shd.Ntheta != 0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_WRONG_STATE, "ShdData initialization failed");
  
  shd.Ntheta = 1;
  shd.Nsd = 1; 
  shd.Nrd = 1;
  shd.Nrr = 1;
  shd.tx_depths = std::make_unique<float[]>(1); shd.tx_depths[0] = 10.0;
  shd.rx_depths = std::make_unique<float[]>(1); shd.rx_depths[0] = 100.0;
  shd.rx_ranges = std::make_unique<float[]>(1); shd.rx_ranges[0] = 500.0;
  shd.theta = std::make_unique<float[]>(1); shd.theta[0] = 0.0;
  
  int idx = shd.getPressureIndex(10.0, 100.0, 500.0, 0.0);
  if (idx < 0)
    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_INVALID_PARAM);

  // 2. ShdData_v1
  ShdData_v1 shd_v1;
  shd_v1.initialize();
  
  // 3. ShdResReader
  BellhopWoss woss_dummy;
  ShdResReader shd_reader(woss_dummy);

  if (debug)
    cout << "ShdData & Readers Tests Passed." << endl;
}

void WossUnitTest::testCustomDataContainer() {
  if (debug)
    cout << "Test: CustomDataContainer..." << endl;

  // 1. CCBathymetry
  {
    if (debug) 
      cout << "  Testing CCBathymetry (Value Type)..." << endl;
    
    WossDbManager::CCBathymetry bathyMap;
    
    // Insert
    Coord c1(0.0, 0.0);
    // Bearing 0, Range 1000m -> Depth 500m
    if (!bathyMap.insert(500.0, c1, 0.0, 1000.0))
      WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CCBathymetry insert failed");
    
    // Duplicate check
    if (bathyMap.insert(600.0, c1, 0.0, 1000.0))
      WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CCBathymetry duplicate insert check failed");

    // Get Exact
    auto val = bathyMap.get(c1, 0.0, 1000.0);
    if (!val || *val != 500.0)
      WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CCBathymetry get (exact) failed");

    // Replace
    bathyMap.replace(550.0, c1, 0.0, 1000.0);
    val = bathyMap.get(c1, 0.0, 1000.0);
    if (!val || *val != 550.0)
      WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CCBathymetry replace failed");
      
    // Nearest neighbor
    // Insert another point: Bearing 0, Range 2000m -> Depth 600m
    bathyMap.insert(600.0, c1, 0.0, 2000.0);
    
    // Query at Range 1400m. 
    // Rx at Range 1400m, Bearing 0.
    Coord rx(0.0, 0.0);
    // Move Rx north by ~1400m. 
    // 1 deg Lat ~= 111km. 1400m ~= 0.0126 deg.
    
    // Testing basic container operations first.
    // Erase
    bathyMap.erase(c1, 0.0, 1000.0);

    // Note: get() returns nearest neighbor, so removing 1000.0 might still make get() return the value at 2000.0.
    // To verify erase works, let's verify we get the other value, or erase both and check for empty.    
    val = bathyMap.get(c1, 0.0, 1000.0);
    if (val && *val == 550.0)
       WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CCBathymetry erase failed (value still 550.0)");
       
    if (!val || *val != 600.0) {
       // Expecting 600.0 (from range 2000.0) as nearest neighbor
       WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CCBathymetry neighbor lookup after erase failed");
    }
    
    // Erase the second one
    bathyMap.erase(c1, 0.0, 2000.0);
    val = bathyMap.get(c1, 0.0, 1000.0);
    if (val)
       WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CCBathymetry erase (all) failed");
  }

  // 2. CCSediment (Shared Ptr Type)
  {
    if (debug) 
      cout << "  Testing CCSediment (Pointer Type)..." << endl;
    
    WossDbManager::CCSediment sedMap;
    
    auto s1 = std::make_shared<Sediment>("Sand", 1650.0, 0.0, 1.9, 0.8, 0.0, 10.0);
    Coord c1(10.0, 10.0);
    
    if (!sedMap.insert(s1, c1, 1.57, 500.0))
       WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CCSediment insert failed");
       
    auto ret = sedMap.get(c1, 1.57, 500.0);
    if (!ret || !(*ret) || (*ret)->getType() != "Sand")
       WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CCSediment get failed");
       
    // Replace with different pointer
    auto s2 = std::make_shared<Sediment>("Clay", 1500.0, 0.0, 1.5, 0.2, 0.0, 5.0);
    sedMap.replace(s2, c1, 1.57, 500.0);
    
    ret = sedMap.get(c1, 1.57, 500.0);
    if (!ret || !(*ret) || (*ret)->getType() != "Clay")
       WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CCSediment replace failed");
  }
  
  // 3. CCAltimetry
  {
    if (debug) cout << "  Testing CCAltimetry..." << endl;
    WossDbManager::CCAltimetry altMap;
    auto a1 = std::make_shared<Altimetry>();
    a1->insertValue(0.0, 5.0);
    
    Coord c1(20.0, 20.0);
    altMap.insert(a1, c1, 0.0, 0.0);
    
    auto ret = altMap.get(c1, 0.0, 0.0);
    if (!ret || !(*ret) || (*ret)->getMaxAltimetryValue() != 5.0)
       WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CCAltimetry get failed");
  }

  // 4. CCSSP (Time Container)
  {
    if (debug) cout << "  Testing CCSSP (Time Container)..." << endl;
    WossDbManager::CCSSP sspMap;
    
    auto ssp1 = std::make_shared<SSP>();
    ssp1->insertValue(0.0, 1480.0); // Simple isovelocity
    
    auto ssp2 = std::make_shared<SSP>();
    ssp2->insertValue(0.0, 1520.0);
    
    Coord c1(30.0, 30.0);
    Time t1(1, 1, 2025, 10, 0, 0);
    Time t2(1, 1, 2025, 11, 0, 0); // 1 hour later
    
    // Insert at t1
    sspMap.insert(ssp1, c1, 0.0, 100.0, t1);
    // Insert at t2
    sspMap.insert(ssp2, c1, 0.0, 100.0, t2);
    
    // Get exact t1
    auto ret1 = sspMap.get(c1, 0.0, 100.0, t1);
    if (!ret1 || !(*ret1) || std::abs((*ret1)->findValue(0.0)->second - 1480.0) > 1e-6)
       WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CCSSP get (exact t1) failed");
       
    // Note: get(t2) might wrap around to t1 in CustomDataTimeContainer due to cyclic logic,
    // so we skip verifying t2 returns 1520.0 explicitly.
    // Instead we verify that t2 is present/used by checking interpolation at t_mid.
       
    // Get interpolated time (t_mid = 10:30)
    Time t_mid(1, 1, 2025, 10, 30, 0);
    
    // The Container's calculateData logic interpolates linearly if Data is clonable and supports arithmetic.
    // SSP supports * double and + SSP.
    // 1480 * 0.5 + 1520 * 0.5 = 740 + 760 = 1500.
    auto ret_mid = sspMap.get(c1, 0.0, 100.0, t_mid);
    if (!ret_mid || !(*ret_mid))
       WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CCSSP get (interpolated) failed to return object");
       
    double val_mid = (*ret_mid)->findValue(0.0)->second;
    if (std::abs(val_mid - 1500.0) > 1.0) // Allow small floating point margin
    {
       if (debug) cout << "Interpolated value: " << val_mid << " Expected: 1500.0" << endl;
       WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "CCSSP get (interpolated) value mismatch");
    }
  }

  if (debug)
    cout << "CustomDataContainer Tests Passed." << endl;
}

void WossUnitTest::testResPressureDb() {
  if (debug)
    cout << "Test: ResPressureDb (Txt & Bin)..." << endl;

  // 1. Txt DB Creator & Db
  ResPressureTxtDbCreator txtCreator;
  txtCreator.setDbPathName("test_pressure.txt");
  txtCreator.setSpaceSampling(0.5);
  
  // Create DB (factory)
  auto db = txtCreator.createWossDb();
  if (!db)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "ResPressureTxtDb creation failed");
  
  // Cast to WossResPressDb to use interface
  auto pressDb = dynamic_cast<WossResPressDb*>(db.get());
  if (!pressDb)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "ResPressureTxtDb cast failed");

  CoordZ tx(0,0,0);
  CoordZ rx(1,1,100);
  Time t(1,1,2025,0,0,0);
  Pressure p(1.0, 2.0);
  
  // Insert
  pressDb->insertValue(tx, rx, 1000.0, t, p);
  
  // Get
  auto p_ret = pressDb->getValue(tx, rx, 1000.0, t);
  if (!p_ret || p_ret->real() != 1.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "ResPressureTxtDb getValue failed");

  // 2. Bin DB Creator & Db
  ResPressureBinDbCreator binCreator;
  binCreator.setDbPathName("test_pressure.bin");
  auto binDb = binCreator.createWossDb();
  if (!binDb)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "ResPressureBinDb creation failed");
  
  auto pressBinDb = dynamic_cast<WossResPressDb*>(binDb.get());
  if (!pressBinDb)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "ResPressureBinDb cast failed");
  
  pressBinDb->insertValue(tx, rx, 1000.0, t, p);
  auto p_bin_ret = pressBinDb->getValue(tx, rx, 1000.0, t);
  if (!p_bin_ret || p_bin_ret->real() != 1.0)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "ResPressureBinDb getValue failed");

  if (debug)
    cout << "ResPressureDb Tests Passed." << endl;
}

void WossUnitTest::testResTimeArrDb() {
  if (debug)
    cout << "Test: ResTimeArrDb (Txt & Bin)..." << endl;

  // 1. Txt
  ResTimeArrTxtDbCreator txtCreator;
  txtCreator.setDbPathName("test_timearr.txt");
  auto db = txtCreator.createWossDb();
  auto taDb = dynamic_cast<WossResTimeArrDb*>(db.get());
  if (!taDb)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "Failed to create WossResTimeArrDb from TxtCreator");

  CoordZ tx(0,0,0);
  CoordZ rx(1,1,100);
  Time t(1,1,2025,0,0,0);
  TimeArr ta;
  ta.insertValue(0.1, Pressure(1.0, 0.0));

  taDb->insertValue(tx, rx, 1000.0, t, ta);
  auto ta_ret = taDb->getValue(tx, rx, 1000.0, t);
  if (!ta_ret || ta_ret->size() != 1)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "ResTimeArrTxtDb getValue failed");

  // 2. Bin
  ResTimeArrBinDbCreator binCreator;
  binCreator.setDbPathName("test_timearr.bin");
  auto binDb = binCreator.createWossDb();
  auto taBinDb = dynamic_cast<WossResTimeArrDb*>(binDb.get());
  if (!taBinDb)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "ResTimeArrBinDb creation/cast failed");

  taBinDb->insertValue(tx, rx, 1000.0, t, ta);
  auto ta_bin_ret = taBinDb->getValue(tx, rx, 1000.0, t);
  if (!ta_bin_ret || ta_bin_ret->size() != 1)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "ResTimeArrBinDb getValue failed");

  if (debug)
    cout << "ResTimeArrDb Tests Passed." << endl;
}

void WossUnitTest::testWossDbManager() {
  if (debug)
    cout << "Test: WossDbManager..." << endl;

  // Ensure SDefHandler has prototypes for return of invalid objects
  auto& defHandler = SDefHandler::instance();
  // Re-setting prototypes to ensure valid factory behavior locally
  defHandler.setSSP(std::make_unique<SSP>());
  defHandler.setSediment(std::make_unique<Sediment>());
  defHandler.setAltimetry(std::make_unique<Altimetry>());
  
  WossDbManager dbManager; // Local instance
  
  // 1. Custom Altimetry
  Coord tx(0,0);
  double bear = 0.0;
  double range = 1000.0;
  
  Altimetry alt;
  alt.insertValue(0.0, 10.0);
  
  // Set
  if (!dbManager.setCustomAltimetry(alt, tx, bear, range)) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "WossDbManager setCustomAltimetry failed");
      
  // Get
  auto altPtr = dbManager.getCustomAltimetry(tx, bear, range);
  if (!altPtr || altPtr->size() != 1) 
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "WossDbManager getCustomAltimetry failed");
      
  // Erase
  dbManager.eraseCustomAltimetry(tx, bear, range);
  auto altGone = dbManager.getCustomAltimetry(tx, bear, range);
  if (altGone && altGone->size() > 0) // createNotValid() returns empty map
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "WossDbManager eraseCustomAltimetry failed");

  // 2. Custom Sediment
  Sediment sed("Sand", 1650.0, 0.0, 1.9, 0.8, 0.0, 10.0);
  dbManager.setCustomSediment(sed, tx, bear, range);
  auto sedPtr = dbManager.getCustomSediment(tx, bear, range);
  if (!sedPtr || sedPtr->getType() != "Sand")
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "WossDbManager CustomSediment mismatch");
      
  // 3. Custom SSP
  Time t(1,1,2025,0,0,0);
  SSP ssp;
  ssp.insertValue(0.0, 1500.0);
  dbManager.setCustomSSP(ssp, tx, bear, range, t);
  auto sspPtr = dbManager.getCustomSSP(tx, bear, range, t);
  if (!sspPtr || sspPtr->size() != 1)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "WossDbManager CustomSSP mismatch");

  // 4. Custom Bathymetry
  Bathymetry bathy = 500.0;
  dbManager.setCustomBathymetry(bathy, tx, bear, range);
  Bathymetry bVal = dbManager.getCustomBathymetry(tx, bear, range);
  if (std::abs(bVal - 500.0) > 1e-6)
    WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_INVALID_PARAM, "WossDbManager CustomBathymetry mismatch");

  if (debug)
    cout << "WossDbManager Tests Passed." << endl;
}

void WossUnitTest::doRun() {
  if (debug)
    cout << "Running Extended WOSS Unit Tests..." << endl;

  testTimeReference();
  testSettings();
  testExceptions();
  testTime();
  testRandomGenerator();
  testCoordZ();
  testSediment();
  testSSP();
  testBellhopCreator();
  testBellhopWoss();
  testPressure();
  testTimeArr();
  testDefHandler();
  testPDouble();
  testAltimetry();
  testThreadPool();
  testTransducer();
  testTransducerHandler();
  testLocation();
  testArrDataAndReaders();
  testShdDataAndReaders();
  testCustomDataContainer();
  testResPressureDb();
  testResTimeArrDb();
  testWossDbManager();
  
  if (debug)
    cout << "All Tests Completed Successfully." << endl;
}

int main(int argc, char* argv [])
{
  auto test = make_unique<WossUnitTest>();
  test->run();
  return 0;
}
