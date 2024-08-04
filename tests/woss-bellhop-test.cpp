/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2024 Federico Guerra
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
 * @file   woss-bellhop-test.cpp
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

class WossBellhopTest : public WossTest {

  public:
  
  WossBellhopTest();
  
  virtual ~WossBellhopTest() {}

  void setTxCoordZVector(vector<CoordZ>& tx) { 
    tx_coordz_vector.clear(); tx_coordz_vector = tx; }

  void setRxCoordZVector(vector<CoordZ>& rx) { 
    rx_coordz_vector.clear(); rx_coordz_vector = rx; }

  private:

  virtual void doConfig();

  virtual void doInit();

  virtual void doRun();

  vector<CoordZ> tx_coordz_vector;

  vector<CoordZ> rx_coordz_vector;
  
  vector<double> frequencies_vector;
  
  string db_path;
  
  string res_path;

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
  setWossTestDebug(false);

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

  if (db_path != "") {
    setSedimDeck41DbType(DECK41_DB_V2_TYPE);
    setSedimDbCoordFilePath(db_path + "/seafloor_sediment/DECK41_V2_coordinates.nc");
    setSedimDbMarsdenFilePath(db_path + "seafloor_sediment/DECK41_V2_marsden_square.nc");
    setSedimDbMarsdenOneFilePath(db_path + "seafloor_sediment/DECK41_V2_marsden_one_degree.nc");

    setSspDbFilePath(db_path + "/ssp/WOA2018/WOA2018_SSP_June.nc");
    setSspWoaDbType(WOA_DB_TYPE_2013);

    setBathyDbFilePath(db_path + "bathymetry/GEBCO_2023.nc");
    setBathyDbGebcoFormat(GEBCO_2D_15_SECONDS_BATHY_TYPE);
  }

  //setWossBellhopPath();
  setWossCreatorDebug(false);
  setWossWorkDirPath(res_path);
  setWossClearWorkDir(true);
  setWossDebug(false);
  setWossSimTime(SimTime(Time(1, 8, 2018), Time(1, 8, 2018)));
  setWossEvolutionTimeQuantum(-1.0);
  setWossTotalRuns(1);
  setWossFrequencyStep(0.0);
  setWossTotalRangeSteps(50.0);
  setWossTxMinDepthOffset(0.0);
  setWossTxMaxDepthOffset(0.0);
  setWossTotalTransmitters(1);
  setWossTotalRxDepths(1);
  setWossRxMinDepthOffset(0.0);
  setWossRxMaxDepthOffset(0.0);
  setWossTotalRxRanges(1);
  setWossRxMinRangeOffset(0.0);
  setWossRxMaxRangeOffset(0.0);
  setWossTotalRays(2000);
  setWossMinAngle(-75.0);
  setWossMaxAngle(75.0);
  setWossUseThorpeAtt(true);
  setWossSspDepthPrecision(1.0E-8);
  setWossNormalizedSspDepthSteps(100000);
  setWossBellhopMode("a");
  setWossBellhopBeamOptions("B");
  setWossBellhopBathyType("L");
  setWossBellhopBathyMethod("S");
  setWossBellhopAltimType("L");
  setWossBellhopArraySyntax(BELLHOP_CREATOR_ARR_FILE_SYNTAX_2);
  setWossBellhopShdSyntax(BELLHOP_CREATOR_SHD_FILE_SYNTAX_1);
  setWossBoxDepth(-3000.0);
  setWossBoxRange(-3000.0);

  setWossManagerDebug(false);
  setWossManagerTimeEvoActive(false);
  setWossManagerThreads(0.0);
  setWossManagerSpaceSampling(0.0);
  setWossManagerUseMultiThread(true);

  tx_coordz_vector.push_back(CoordZ(42.59, 10.125, 80.0));
  rx_coordz_vector.push_back(CoordZ(42.59, 10.1615, 1.0));
  for (int i = 0; i < 25; ++i)
  {
    frequencies_vector.push_back(500.0 + i*1000.0);
  }
}

void WossBellhopTest::doInit() {
  if (db_path == "") {
    woss_db_manager->setCustomBathymetry("5|0.0|100.0|100.0|200.0|300.0|150.0|400.0|100.0|700.0|300.0", CoordZ(42.59, 10.125, 80.0));
    woss_db_manager->setCustomSediment("TestSediment|1560.0|200.0|1.5|0.9|0.8|300.0");
    woss_db_manager->setCustomSSP("12|0|1508.42|10|1508.02|20|1507.71|30|1507.53|50|1507.03|75|1507.56|100|1508.08|125|1508.49|150|1508.91|200|1509.75|250|1510.58|300|1511.42");
  }
}


void WossBellhopTest::doRun() {
  WossManager* curr_woss_manager = woss_controller->getWossManager();

  for (vector< CoordZ >::iterator it = tx_coordz_vector.begin(); it != tx_coordz_vector.end(); ++it) {
    for (vector< CoordZ >::iterator it2 = rx_coordz_vector.begin(); it2 != rx_coordz_vector.end(); ++it2) {
      for (vector< double >::iterator it3 = frequencies_vector.begin(); it3 != frequencies_vector.end(); ++it3 ) {
        if (debug) {
          cout << endl << endl;
          cout << "tx coordz = " << (*it) << endl;
          cout << "rx coordz = " << (*it2) << endl;
          cout << "freq = " << (*it3) << endl;
        }
        // we need to erase the current woss in order to run it again with different setting
        curr_woss_manager->eraseAllWoss();
        bellhop_creator->setBhMode("A");
        bellhop_creator->setWrkDirPath(res_path + "arr_asc/");

        TimeArr* curr_time_arr_asc = curr_woss_manager->getWossTimeArr((*it), (*it2), (*it3), (*it3));
        
        if (curr_time_arr_asc == NULL) {
          throw WOSS_EXCEPTION(WOSS_ERROR_OUT_OF_MEMORY);
        }

        Pressure* press_temp_asc = SDefHandler::instance()->getPressure()->create( *curr_time_arr_asc );

        if (press_temp_asc == NULL) {
          throw WOSS_EXCEPTION(WOSS_ERROR_OUT_OF_MEMORY);
        }

        // we need to erase the current woss in order to run it again with Pressure setting
        curr_woss_manager->eraseAllWoss();
        bellhop_creator->setBhMode("a");
        bellhop_creator->setWrkDirPath(res_path + "arr_bin/");

        TimeArr* curr_time_arr_bin = curr_woss_manager->getWossTimeArr((*it), (*it2), (*it3), (*it3));
        
        if (curr_time_arr_bin == NULL) {
          throw WOSS_EXCEPTION(WOSS_ERROR_OUT_OF_MEMORY);
        }

        Pressure* press_temp_bin = SDefHandler::instance()->getPressure()->create( *curr_time_arr_bin );

        if (press_temp_bin == NULL) {
          throw WOSS_EXCEPTION(WOSS_ERROR_OUT_OF_MEMORY);
        }

        if ( abs(press_temp_bin->getTxLossDb() - press_temp_asc->getTxLossDb()) > 1.0) {
          throw WOSS_EXCEPTION(WOSS_ERROR_OUT_OF_RANGE_PARAM); 
        }

        // we need to erase the current woss in order to run it again with Pressure setting
        curr_woss_manager->eraseAllWoss();
        bellhop_creator->setBhMode("C");
        bellhop_creator->setWrkDirPath(res_path + "press_shd/");

        Pressure* curr_pressure = curr_woss_manager->getWossPressure((*it), (*it2), (*it3), (*it3));

        if (curr_pressure == NULL) {
          throw WOSS_EXCEPTION(WOSS_ERROR_OUT_OF_MEMORY);
        }

        if (debug) {
          cout << endl;
          cout << "curr_time_arr_asc = " << (*curr_time_arr_asc) << endl;
          cout << "curr_time_arr_bin = " << (*curr_time_arr_bin) << endl;
          cout << "time_arr_asc TL = " << (*press_temp_asc) << endl;
          cout << "time_arr_bin TL = " << (*press_temp_bin) << endl;
          cout << "curr_pressure = " << (*curr_pressure) << endl;
        }
      }
    }
  }
}


int main(int argc, char* argv [])
{
  WossBellhopTest* woss_bh_test = new WossBellhopTest();
  woss_bh_test->run();
  delete woss_bh_test;

  return 0;
}
