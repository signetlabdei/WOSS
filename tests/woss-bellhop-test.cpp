/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2024 2025 Federico Guerra
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
 * @file   woss-bellhop-test.cpp
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

class WossBellhopTest : public WossTest {

  public:
  
  WossBellhopTest();
  
  virtual ~WossBellhopTest() override = default;

  void setTxCoordZVector(vector<CoordZ>& tx) { 
    tx_coordz_vector.clear(); tx_coordz_vector = tx; }

  void setRxCoordZVector(vector<CoordZ>& rx) { 
    rx_coordz_vector.clear(); rx_coordz_vector = rx; }

  private:

  virtual void doConfig() override;

  virtual void doInit() override;

  virtual void doRun() override;

  vector<CoordZ> tx_coordz_vector;

  vector<CoordZ> rx_coordz_vector;

  CoordZPairVect coordz_pair_vector;

  vector<double> frequencies_vector;

  string db_path;

  string res_path;

  bool skip_single_tests = false;

};

WossBellhopTest::WossBellhopTest()
: WossTest(),
  tx_coordz_vector(),
  rx_coordz_vector(),
  frequencies_vector(),
  db_path(),
  res_path("./bh_test_out/")
{

}

void WossBellhopTest::doConfig() {
  setWossTestDebug(true);

  setWossRandomGenStream(1);

  setResDbCreatorDebug(false);
  setResDbDebug(false);
  setResDbUseBinary(false);
  setResDbUseTimeArr(false);
  setResDbUsePressure(false);
  setResDbFilePath(res_path);
  setResDbFileName("bh_test.bin");

  setWossDbManagerDebug(false);

  //db_path = "";

#if defined (WOSS_NETCDF_SUPPORT)
  if (db_path != "") {
    setSedimDeck41DbType(DECK41DbType::DECK41_DB_V2_TYPE);
    setSedimDbCoordFilePath(db_path + "/seafloor_sediment/DECK41_V2_coordinates.nc");
    setSedimDbMarsdenFilePath(db_path + "seafloor_sediment/DECK41_V2_marsden_square.nc");
    setSedimDbMarsdenOneFilePath(db_path + "seafloor_sediment/DECK41_V2_marsden_one_degree.nc");

    setSspDbFilePath(db_path + "/ssp/WOA2023/WOA2023_SSP_June.nc");
#if defined (WOSS_NETCDF4_SUPPORT)
    setSspWoaDbType(WOADbType::WOA_DB_TYPE_2013);
#endif // defined (WOSS_NETCDF4_SUPPORT)
    setBathyDbFilePath(db_path + "bathymetry/GEBCO_2025_sub_ice.nc");
    setBathyDbGebcoFormat(GEBCO_BATHY_TYPE::GEBCO_2D_15_SECONDS_BATHY_TYPE);
  }
#endif // defined (WOSS_NETCDF_SUPPORT)

  //setWossBellhopPath();
  setWossBellhopBinName(BellhopWoss::WOSS_BELLHOP_BIN_NAME);
  //setWossBellhopBinName(BellhopWoss::WOSS_BELLHOPCXX_BIN_NAME);
  //setWossBellhopBinName(BellhopWoss::WOSS_BELLHOPCUDA_BIN_PATH);
  setWossCreatorDebug(false);
  setWossWorkDirPath(res_path);
  setWossClearWorkDir(true);
  setWossDebug(false);
  setWossSimTime(SimTime(Time(1, 8, 2025), Time(1, 8, 2025)));
  setWossEvolutionTimeQuantum(-1.0);
  setWossTotalRuns(1);
  setWossFrequencyStep(0.0);
  setWossTotalRangeSteps(3000.0);
  setWossTxMinDepthOffset(0.0);
  setWossTxMaxDepthOffset(0.0);
  setWossTotalTransmitters(1);
  setWossTotalRxDepths(1);
  setWossRxMinDepthOffset(0.0);
  setWossRxMaxDepthOffset(0.0);
  setWossTotalRxRanges(1);
  setWossRxMinRangeOffset(0.0);
  setWossRxMaxRangeOffset(0.0);
  setWossTotalRays(0);
  setWossMinAngle(-75.0);
  setWossMaxAngle(75.0);
  setWossUseThorpeAtt(true);
  setWossSspDepthPrecision(1.0E-8);
  setWossNormalizedSspDepthSteps(100000);
  setWossBellhopMode("a");
  setWossBellhopBeamOptions("B");
  setWossBellhopBathyType("LL");
  setWossBellhopBathyMethod("D");
  setWossBellhopAltimType("L");
  setWossBellhopArraySyntax(BellhopArrSyntax::BELLHOP_CREATOR_ARR_FILE_SYNTAX_2);
  setWossBellhopShdSyntax(BellhopShdSyntax::BELLHOP_CREATOR_SHD_FILE_SYNTAX_1);
  setWossBoxDepth(-3000.0);
  setWossBoxRange(-3000.0);

  setWossManagerDebug(false);
  setWossManagerTimeEvoActive(false);
  setWossManagerThreads(0);
  setWossManagerSpaceSampling(0.0);
  setWossManagerUseThreadPool(false);
  setWossManagerUseMultiThread(true);

  tx_coordz_vector.emplace_back(CoordZ(42.59, 10.125, 80.0));
  rx_coordz_vector.emplace_back(CoordZ(42.59, 10.1615, 1.0));

  coordz_pair_vector = { make_pair<CoordZ, CoordZ>(CoordZ(42.59, 10.125, 1.0), CoordZ(42.59, 10.1615, 50.0)),
                         make_pair<CoordZ, CoordZ>(CoordZ(42.59, 10.125, 1.0), CoordZ(42.59, 10.1613, 40.0)), 
                         make_pair<CoordZ, CoordZ>(CoordZ(42.59, 10.125, 1.0), CoordZ(42.59, 10.1612, 22.0)),
                         make_pair<CoordZ, CoordZ>(CoordZ(42.59, 10.125, 1.0), CoordZ(42.59, 10.1621, 33.0)),
                         make_pair<CoordZ, CoordZ>(CoordZ(42.59, 10.125, 1.0), CoordZ(42.59, 10.1710, 44.0)),
                         make_pair<CoordZ, CoordZ>(CoordZ(42.59, 10.125, 1.0), CoordZ(42.59, 10.1501, 30.0)),
                         make_pair<CoordZ, CoordZ>(CoordZ(42.59, 10.125, 1.0), CoordZ(42.59, 10.1601, 40.0)),
                         make_pair<CoordZ, CoordZ>(CoordZ(42.59, 10.125, 1.0), CoordZ(42.59, 10.1644, 50.0)),
                      };

  for (int i = 0; i < 25; ++i)
  {
    frequencies_vector.emplace_back(500.0 + i*1000.0);
  }

  skip_single_tests = false;
}

void WossBellhopTest::doInit() {
  if (db_path == "") {
    woss_db_manager->setCustomBathymetry("5|0.0|100.0|100.0|200.0|300.0|150.0|400.0|100.0|700.0|300.0", CoordZ(42.59, 10.125, 80.0));
    woss_db_manager->setCustomSediment("TestSediment|1560.0|200.0|1.5|0.9|0.8|300.0");
    woss_db_manager->setCustomSSP("12|0|1508.42|10|1508.02|20|1507.71|30|1507.53|50|1507.03|75|1507.56|100|1508.08|125|1508.49|150|1508.91|200|1509.75|250|1510.58|300|1511.42");
  }
}


void WossBellhopTest::doRun() {
  auto curr_woss_manager = woss_controller->getWossManager();

  if (curr_woss_manager == nullptr) {
    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_INVALID_PARAM);
  }

  if ( skip_single_tests == false ) {
    for (auto& tx_coordz : tx_coordz_vector) {
      for (auto& rx_coordz : rx_coordz_vector) {
        for (auto& freq : frequencies_vector) {
          if (debug) {
            cout << endl << endl;
            cout << "tx coordz = " << tx_coordz << endl;
            cout << "rx coordz = " << rx_coordz << endl;
            cout << "freq = " << freq << endl;
          }
          // we need to erase the current woss in order to run it again with different setting
          curr_woss_manager->reset();
          bellhop_creator->setBhMode("A");
          bellhop_creator->setWrkDirPath(res_path + "arr_asc/");

          auto curr_time_arr_asc = curr_woss_manager->getWossTimeArr(tx_coordz, rx_coordz, freq, freq);
          
          if (curr_time_arr_asc == nullptr) {
            WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
          }

          if (curr_time_arr_asc->isConvertedFromPressure() == true) {
            WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_BAD_FORMAT);
          }

          auto press_temp_asc = SDefHandler::instance().createPressure( *curr_time_arr_asc );

          if (press_temp_asc == nullptr) {
            WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
          }

          // we need to erase the current woss in order to run it again with Pressure setting
          curr_woss_manager->reset();
          bellhop_creator->setBhMode("a");
          bellhop_creator->setWrkDirPath(res_path + "arr_bin/");

          auto curr_time_arr_bin = curr_woss_manager->getWossTimeArr(tx_coordz, rx_coordz, freq, freq);
          
          if (curr_time_arr_bin == nullptr) {
            WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
          }

          if (curr_time_arr_bin->isConvertedFromPressure() == true) {
            WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_BAD_FORMAT);
          }

          auto press_temp_bin = SDefHandler::instance().createPressure( *curr_time_arr_bin );

          if (press_temp_bin == nullptr) {
            WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
          }

          double diff = abs(press_temp_bin->getTxLossDb() - press_temp_asc->getTxLossDb());

          if (debug) {
            cout << "abs press diff = " << diff << endl;
          }

          if (diff > 1.0) {
            stringstream ss;
            ss << "abs press diff(" << diff << ")";
            WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM, ss.str().c_str());
          }

          // we need to erase the current woss in order to run it again with Pressure setting
          curr_woss_manager->reset();
          bellhop_creator->setBhMode("C");
          bellhop_creator->setWrkDirPath(res_path + "press_shd/");

          auto curr_time_arr_press = curr_woss_manager->getWossTimeArr(tx_coordz, rx_coordz, freq, freq);
          
          if (curr_time_arr_press == nullptr) {
            WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
          }

          if (curr_time_arr_press->isConvertedFromPressure() == false) {
            WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_BAD_FORMAT);
          }

          auto press_temp_shd = SDefHandler::instance().createPressure( *curr_time_arr_press );

          if (press_temp_shd == nullptr) {
            WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
          }

          auto curr_pressure = curr_woss_manager->getWossPressure(tx_coordz, rx_coordz, freq, freq);

          if (curr_pressure == nullptr) {
            WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
          }

          if (debug) {
            cout << endl;
            cout << "curr_time_arr_asc = " << (*curr_time_arr_asc) << endl;
            cout << "curr_time_arr_bin = " << (*curr_time_arr_bin) << endl;
            cout << "time_arr_asc TL = " << (*press_temp_asc) << endl;
            cout << "time_arr_bin TL = " << (*press_temp_bin) << endl;
            cout << "curr_time_arr_press = " << (*curr_time_arr_press) << endl;
            cout << "curr_pressure = " << (*curr_pressure) << endl;
          }

          diff = abs(press_temp_shd->getTxLossDb() - curr_pressure->getTxLossDb());

          if (debug) {
            cout << "abs press shd diff = " << diff << endl;
          }
          
          if ( diff > 1.0) {
            stringstream ss;
            ss << "abs press diff(" << diff << ")";
            WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM, ss.str().c_str());
          }
        }
      }
    }
  }

  if (debug) {
    cout << endl << endl;
    cout << "coordz_pair_vector size = " << coordz_pair_vector.size() << endl;
    cout << "freq = " << *frequencies_vector.begin() << endl;
  }
  // we need to erase the current woss in order to run it again with different setting
  curr_woss_manager->reset();
  bellhop_creator->setBhMode("A");
  bellhop_creator->setWrkDirPath(res_path + "arr_asc_MT/");

  auto curr_time_arr_asc_vect = curr_woss_manager->getWossTimeArr(coordz_pair_vector, *frequencies_vector.begin(), *frequencies_vector.begin());
  
  if (curr_time_arr_asc_vect.empty()) {
    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
  }

  // we need to erase the current woss in order to run it again with Pressure setting
  curr_woss_manager->reset();
  bellhop_creator->setBhMode("a");
  bellhop_creator->setWrkDirPath(res_path + "arr_bin_MT/");

  auto curr_time_arr_bin_vect = curr_woss_manager->getWossTimeArr(coordz_pair_vector, *frequencies_vector.begin(), *frequencies_vector.begin());
  
  if (curr_time_arr_bin_vect.empty()) {
    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
  }

  // we need to erase the current woss in order to run it again with Pressure setting
  curr_woss_manager->reset();
  
  // run MT pressure with woss::ThreadPool
  WossManagerResDbMT* curr_wm_MT = dynamic_cast<WossManagerResDbMT*>(curr_woss_manager.get());
  if (curr_wm_MT == nullptr) {
    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_BAD_FORMAT);
  }

  curr_wm_MT->setUseThreadPoolFlag(true);

  bellhop_creator->setBhMode("C");
  bellhop_creator->setWrkDirPath(res_path + "press_shd_MT/");

  auto curr_pressure_vect = curr_woss_manager->getWossPressure(coordz_pair_vector, *frequencies_vector.begin(), *frequencies_vector.begin());

  if (curr_pressure_vect.empty()) {
    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
  }

  auto it = curr_time_arr_asc_vect.cbegin();
  auto it2 = curr_time_arr_bin_vect.cbegin();

  for ( ; it != curr_time_arr_asc_vect.cend() && it2 != curr_time_arr_bin_vect.cend(); ++it, ++it2) {
    auto press_temp_asc = SDefHandler::instance().createPressure( **it );
    auto press_temp_bin = SDefHandler::instance().createPressure( **it2 );

    if (press_temp_asc == nullptr) {
      WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
    }

    if (press_temp_bin == nullptr) {
      WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
    }

    if ( abs(press_temp_bin->getTxLossDb() - press_temp_asc->getTxLossDb()) > 1.0) {
      stringstream ss;
      ss << "abs press diff(" << abs(press_temp_bin->getTxLossDb() - press_temp_asc->getTxLossDb()) << ")";
      WOSS_EXCEPTION_INFO(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM, ss.str().c_str());
    }
  }

  if (debug) {
    cout << endl;
    cout << "curr_time_arr_asc_vect = " << curr_time_arr_asc_vect.size() << endl;
    cout << "curr_time_arr_bin_vect = " << curr_time_arr_asc_vect.size() << endl;
    cout << "curr_pressure_vect = " << curr_pressure_vect.size() << endl;
  }
}


int main(int argc, char* argv [])
{
  auto woss_bh_test = make_unique<WossBellhopTest>();
  woss_bh_test->run();

  return 0;
}
