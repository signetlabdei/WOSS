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

/*
 * This source code has been developed under the
 * "NATO OTAN SPS G5884" project.
 */

 
/**
 * @file   woss-bellhop-script.cpp
 * @author Federico Guerra
 * 
 * \brief 
 *
 * WOSS commandline binary for channel tests and scripting,
 * with minimal and simple bellhop configuration.
 * 
 * some syntax examples
 * 
 * /<wossdir>/tests/.libs/woss-bellhop-script-bin 1 1 /home/<user>/ocean_databases/dbs/ /bathymetry/GEBCO_2025_sub_ice.nc /ssp/WOA2023/WOA2023_SSP_June.nc ./bh_script_out/ 42.59 10.125 60 42.59 10.131 2 1500
 * 
 * /<wossdir>/tests/.libs/woss-bellhop-script-bin 1 1 /home/<user>/ocean_databases/dbs/ /bathymetry/GEBCO_2025_sub_ice.nc /ssp/WOA2023/WOA2023_SSP_June.nc ./bh_script_out/ 42.59 10.125 60 42.59 10.131 2 1500 "5|0|100|300|200|400|500|600|550|800|760"
 * 
 * /<wossdir>/tests/.libs/woss-bellhop-script-bin 1 1 /home/<user>/ocean_databases/dbs/ /bathymetry/GEBCO_2025_sub_ice.nc /ssp/WOA2023/WOA2023_SSP_June.nc ./bh_script_out/ 42.59 10.125 60 42.59 10.131 2 1500 "#" "TestSediment|1560.0|200.0|1.5|0.9|0.8|300.0"
 * 
 * /<wossdir>/tests/.libs/woss-bellhop-script-bin 1 1 /home/<user>/ocean_databases/dbs/ /bathymetry/GEBCO_2025_sub_ice.nc /ssp/WOA2023/WOA2023_SSP_June.nc ./bh_script_out/ 42.59 10.125 60 42.59 10.131 2 1500 "5|0|100|300|200|400|500|600|550|800|760" "TestSediment|1560.0|200.0|1.5|0.9|0.8|300.0" "12|0|1508.42|10|1508.02|20|1507.71|30|1507.53|50|1507.03|75|1507.56|100|1508.08|125|1508.49|150|1508.91|200|1509.75|250|1510.58|300|1511.42"
 */

#include <iostream>
#include <string>
#include "woss-test.h"

using namespace std;
using namespace woss;

class WossBellhopScript : public WossTest {

  public:
  
  WossBellhopScript();
  
  virtual ~WossBellhopScript() = default;

  /** debug level
   * 0 = no debug prints
   * > 0 enable test script debug log
   * > 10 enable main woss debug logs
   * > 100 enable all woss debug logs
   */
  int debug_level;

  /** WOSS random simulator seed */
  int stream;

  /** Transmitter coordinates */
  CoordZ tx_coordz;

  /** Receiver coordinates */
  CoordZ rx_coordz;

  /** channel simulation frequency */
  double frequency;

  /** path to the WOSS db directories */
  string db_path;

  /** subpath to the bathymetry NetCDF db */
  string bathy_db_file_path;

  /** subpath to the SSP NetCDF db */
  string ssp_db_file_path;

  /** path where the channel results will be written to */
  string res_path;
  
  /** 
   * string representing a custom SSP.
   * Syntax:
   * "tot_depths|depth1[m]|speed1[m/s]|...|depthN[m]|speedN[m/s]"
   */
  string custom_ssp;
  
  /** 
   * string representing a custom bathymetry line.
   * Syntax:
   * "tot_ranges|range1[m]|depth1[m]|....|rangeN[m]|depthN[m]"
   */
  string custom_bathy;
  
  /** 
   * string representing a custom sediment.
   * Syntax:
   * "SedimName|vel_comp[m/s]|vel_shear[m/s]|dens[g/cm3]|atten_comp[db/wavelength]|atten_shear[db/wavelength]|bottom depth[m]"
   */
  string custom_sedim;


  private:


  virtual void doConfig() override;

  virtual void doInit() override;

  virtual void doRun() override;

};

WossBellhopScript::WossBellhopScript()
: WossTest(),
  stream(),
  tx_coordz(),
  rx_coordz(),
  frequency(0.0),
  db_path(),
  bathy_db_file_path("/bathymetry/GEBCO_2025_sub_ice.nc"),
  ssp_db_file_path("/ssp/WOA2023/WOA2023_SSP_June.nc"),
  res_path("./bh_script_out/"),
  custom_ssp(),
  custom_bathy(),
  custom_sedim()
{

}

void WossBellhopScript::doConfig() {
  setWossRandomGenStream(stream);

  setResDbCreatorDebug(debug_level > 100 ? true : false);
  setResDbDebug(debug_level > 100 ? true : false);
  setResDbUseBinary(false);
  setResDbUseTimeArr(false);
  setResDbUsePressure(false);
  setResDbFilePath(res_path);
  setResDbFileName("bh_test.bin");

  setWossDbManagerDebug(debug_level > 10 ? true : false);

  if (db_path != "") {
    setSedimDeck41DbType(DECK41DbType::DECK41_DB_V2_TYPE);
    setSedimDbCoordFilePath(db_path + "/seafloor_sediment/DECK41_V2_coordinates.nc");
    setSedimDbMarsdenFilePath(db_path + "/seafloor_sediment/DECK41_V2_marsden_square.nc");
    setSedimDbMarsdenOneFilePath(db_path + "/seafloor_sediment/DECK41_V2_marsden_one_degree.nc");

    setSspDbFilePath(db_path + ssp_db_file_path);

    /// valid for 2001 2013, 2018 and 2023 WOA SSP NetCDF db files
    setSspWoaDbType(WOADbType::WOA_DB_TYPE_2013);

    setBathyDbFilePath(db_path + bathy_db_file_path);

    /// valid for latest GEBCO db files (>= 2021)
    setBathyDbGebcoFormat(GEBCO_BATHY_TYPE::GEBCO_2D_15_SECONDS_BATHY_TYPE);
  }
  else if ((custom_ssp == "") || (custom_sedim == "") || (custom_bathy == "")) {
    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM); 
  }

  //setWossBellhopPath();
  setWossBellhopBinName(BellhopWoss::WOSS_BELLHOP_BIN_NAME);
  //setWossBellhopBinName(BellhopWoss::WOSS_BELLHOPCXX_BIN_NAME);
  //setWossBellhopBinName(BellhopWoss::WOSS_BELLHOPCUDA_BIN_PATH);
  setWossCreatorDebug(debug_level > 100 ? true : false);
  setWossWorkDirPath(res_path);
  setWossClearWorkDir(false);
  setWossDebug(debug_level > 10 ? true : false);
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
  setWossBellhopBathyType("LL");
  setWossBellhopBathyMethod("D");
  setWossBellhopAltimType("L");
  setWossBellhopArraySyntax(BellhopArrSyntax::BELLHOP_CREATOR_ARR_FILE_SYNTAX_2);
  setWossBellhopShdSyntax(BellhopShdSyntax::BELLHOP_CREATOR_SHD_FILE_SYNTAX_1);
  setWossBoxDepth(-3000.0);
  setWossBoxRange(-3000.0);

  setWossManagerDebug(debug_level > 10 ? true : false);
  setWossManagerTimeEvoActive(false);
  setWossManagerThreads(0.0);
  setWossManagerSpaceSampling(0.0);
  setWossManagerUseMultiThread(true);
}

void WossBellhopScript::doInit() {
  if (custom_bathy != "") {
    woss_db_manager->setCustomBathymetry(custom_bathy);
  }
  if (custom_sedim != "") {
    woss_db_manager->setCustomSediment(custom_sedim);
  }
  if (custom_ssp != "") {
    woss_db_manager->setCustomSSP(custom_ssp);
  }

  if ((db_path == "") && (custom_bathy == "") 
       && (custom_sedim == "") && (custom_ssp == "")) {
    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM);
  }

  if (   (tx_coordz.isValid() == false) || (rx_coordz.isValid() == false) 
      || (frequency == 0.0))
  {
    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM);
  }
}


void WossBellhopScript::doRun() {
  auto curr_woss_manager = woss_controller->getWossManager();

  if (debug) {
    cout << endl << endl;
    cout << "tx coordz = " << tx_coordz << endl;
    cout << "rx coordz = " << rx_coordz << endl;
    cout << "freq = " << frequency << endl;
  }
  // we need to erase the current woss in order to run it again with different setting
  curr_woss_manager->reset();
  bellhop_creator->setBhMode("A");
  bellhop_creator->setWrkDirPath(res_path + "arr_asc/");

  auto curr_time_arr_asc = curr_woss_manager->getWossTimeArr(tx_coordz, rx_coordz, frequency, frequency);
  
  if (curr_time_arr_asc == NULL) {
    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
  }

  auto press_temp_asc = SDefHandler::instance().createPressure( *curr_time_arr_asc );

  if (press_temp_asc == NULL) {
    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
  }

  // we need to erase the current woss in order to run it again with Pressure setting
  curr_woss_manager->reset();
  bellhop_creator->setBhMode("a");
  bellhop_creator->setWrkDirPath(res_path + "arr_bin/");

  auto curr_time_arr_bin = curr_woss_manager->getWossTimeArr(tx_coordz, rx_coordz, frequency, frequency);
  
  if (curr_time_arr_bin == NULL) {
    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
  }

  auto press_temp_bin = SDefHandler::instance().createPressure( *curr_time_arr_bin );

  if (press_temp_bin == NULL) {
    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
  }

  if ( abs(press_temp_bin->getTxLossDb() - press_temp_asc->getTxLossDb()) > 1.0) {
    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM);
  }

  // we need to erase the current woss in order to run it again with Pressure setting
  curr_woss_manager->reset();
  bellhop_creator->setBhMode("C");
  bellhop_creator->setWrkDirPath(res_path + "press_shd/");

  auto curr_pressure = curr_woss_manager->getWossPressure(tx_coordz, rx_coordz, frequency, frequency);

  if (curr_pressure == NULL) {
    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
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


int main(int argc, char* argv []) {
  WossBellhopScript* woss_bh_test = new WossBellhopScript();

  if (woss_bh_test == NULL) {
     WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_MEMORY);
  }

  if ((argc > 13) && (argc < 18)) {
    woss_bh_test->debug_level = atoi(argv[1]);
  
    bool debug = (bool)(woss_bh_test->debug_level);

    woss_bh_test->setWossTestDebug(debug);
    
    woss_bh_test->stream = atoi(argv[2]);
    
    woss_bh_test->db_path = argv[3];

    woss_bh_test->bathy_db_file_path = argv[4];

    woss_bh_test->ssp_db_file_path = argv[5];

    woss_bh_test->res_path = argv[6];

    double tx_lat = std::atof(argv[7]);
    double tx_lon = std::atof(argv[8]);
    double tx_dep = std::atof(argv[9]);

    woss_bh_test->tx_coordz = CoordZ(tx_lat, tx_lon, tx_dep);

    double rx_lat = std::atof(argv[10]);
    double rx_lon = std::atof(argv[11]);
    double rx_dep = std::atof(argv[12]);

    woss_bh_test->rx_coordz = CoordZ(rx_lat, rx_lon, rx_dep);

    woss_bh_test->frequency = std::atof(argv[13]);

    if (woss_bh_test->getWossTestDebug() == true) {
       cout << "debug_level = " << woss_bh_test->debug_level <<"; stream = " << woss_bh_test->stream << endl
            << "tx_coordz = " << woss_bh_test->tx_coordz << "; rx_coordz = " 
            << woss_bh_test->rx_coordz << "; freq = " << woss_bh_test->frequency << endl
            << "db_path = " << woss_bh_test->db_path << endl
            << "bathy_db_file_path = " << woss_bh_test->db_path + woss_bh_test->bathy_db_file_path << endl
            << "ssp_db_file_path = " << woss_bh_test->db_path + woss_bh_test->ssp_db_file_path << endl
            << "res_path = " << woss_bh_test->res_path << endl;
    }

    if (argc > 14) {
      // check input validity, skip with "#" input
      if ((argc > 14) && (std::string(argv[14]) != "#")) {
        woss_bh_test->custom_bathy = argv[14];
      }
      // check input validity, skip with "#" input
      if ((argc > 15) && (std::string(argv[15]) != "#")) {
        woss_bh_test->custom_sedim = argv[15];
      }
      // check input validity, skip with "#" input
      if ((argc > 16) && (std::string(argv[16]) != "#")) {
        woss_bh_test->custom_ssp = argv[16];
      }

      // if every custom environment data is set, then do not set db path
      if (    (woss_bh_test->custom_bathy != "") 
           && (woss_bh_test->custom_sedim != "") 
           && (woss_bh_test->custom_ssp != "") ) {
        woss_bh_test->db_path = "";
      }

      if (woss_bh_test->getWossTestDebug() == true) {
        cout << "custom_bathy = " << woss_bh_test->custom_bathy << endl
             << "custom_sedim = " << woss_bh_test->custom_sedim << endl
             << "custom_ssp = " << woss_bh_test->custom_ssp << endl;
      }
    }
  }
  else {
    cerr << argv[0] << " usage: <debug_level> <seed> <db_path> <bathy_db_file_path> <ssp_db_file_path> <res_path>"
         << " <tx_lat> <tx_lon> <tx_depth> <rx_lat> <rx_lon> <rx_depth> <frequency> [<custom_bathy, \"#\" to skip>]"
         << " [<custom_sedim>, \"#\" to skip] [<custom_ssp>, \"#\" to skip]" << endl;
    
    cerr << "<custom_bathy> syntax: \"tot_ranges|range1[m]|depth1[m]|....|rangeN[m]|depthN[m]\"" << endl
         << "<custom_sedim> syntax: \"SedimName|vel_comp[m/s]|vel_shear[m/s]|dens[g/cm3]|atten_comp[db/wavelength]|atten_shear[db/wavelength]|bottom depth[m]\"" << endl
         << "<custom_ssp>   syntax: \"tot_depths|depth1[m]|speed1[m/s]|...|depthN[m]|speedN[m/s]\"" << endl;

    // argc == 1 means internal test, which should pass
    if (argc == 1)
    {
      return 0;
    }

    WOSS_EXCEPTION(WossErrorType::WOSS_ERROR_OUT_OF_RANGE_PARAM);
  }

  woss_bh_test->run();
  delete woss_bh_test;

  return 0;
}
