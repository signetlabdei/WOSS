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
 * @file   woss-test.cpp
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossTest class
 *
 * Provides the interface for the woss::WossTest class
 */


#include "woss-test.h"

using namespace std;
using namespace woss;



#define WH_SPACE_SAMPLING_DEFAULT (0.0)
#define WH_DEBUG_DEFAULT false
#define WH_TIME_EVOLUTION_DEFAULT (-1.0)
#define WH_STRING_DEFAULT ""
#define WH_TOTAL_RUNS_DEFAULT (5)
#define WH_FREQUENCY_STEP_DEFAULT (0.0)
#define WH_TOTAL_RANGE_STEPS_DEFAULT (3000.0)
#define WH_TX_MIN_DEPTH_OFFSET_DEFAULT (0.0)
#define WH_TX_MAX_DEPTH_OFFSET_DEFAULT (0.0)
#define WH_TOTAL_TRANSMITTERS_DEFAULT (1)
#define WH_TOTAL_RX_DEPTHS_DEFAULT (2)
#define WH_RX_MIN_DEPTH_OFFSET_DEFAULT (-0.1)
#define WH_RX_MAX_DEPTH_OFFSET_DEFAULT (0.1)
#define WH_TOTAL_RX_RANGES_DEFAULT (2)
#define WH_RX_MIN_RANGE_OFFSET_DEFAULT (-0.1)
#define WH_RX_MAX_RANGE_OFFSET_DEFAULT (0.1)
#define WH_TOTAL_RAYS_DEFAULT (0)
#define WH_MIN_ANGLE_DEFAULT (-180.0)
#define WH_MAX_ANGLE_DEFAULT (180.0)
#define WH_SSP_DEPTH_PRECISION_DEFAULT (1.0E-8)
#define WH_NORMALIZED_SSP_DEPTH_STEPS_DEFAULT (100000)
#define WH_WORK_PATH_DEFAULT "./woss-output/work-dir"
#define WH_BELLHOP_MODE_DEFAULT "A"
#define WH_BEAM_OPTIONS_DEFAULT "B"
#define WH_BATHYMETRY_TYPE_DEFAULT "L"
#define WH_BATHYMETRY_METHOD_DEFAULT "S"
#define WH_ALTIMETRY_TYPE_DEFAULT "L"
#define WH_SIMULATION_TIMES_DEFAULT ()
#define WH_CONCURRENT_THREADS_DEFAULT (0.0)
#define WH_BELLHOP_ARR_SYNTAX_DEFAULT BELLHOP_CREATOR_ARR_FILE_SYNTAX_2
#define WH_BELLHOP_SHD_SYNTAX_DEFAULT BELLHOP_CREATOR_SHD_FILE_SYNTAX_1
#define WH_BOX_DEPTH (-3000.0)
#define WH_BOX_RANGE (-3000.0)
#define WH_GEBCO_FORMAT_DEFAULT GEBCO_2D_15_SECONDS_BATHY_TYPE
#if defined (WOSS_NETCDF4_SUPPORT)
#define WH_WOA_DB_TYPE_DEFAULT WOA_DB_TYPE_2013
#define WH_DECK41_DB_TYPE_DEFAULT DECK41_DB_V2_TYPE
#endif // defined (WOSS_NETCDF_SUPPORT)


WossException::WossException(WossErrorType error_type_, const char* file_, int line_, const char* func_, const char* info_) 
: exception(),
  error_type(error_type_),
  file(),
  line(line_),
  func(),
  info(),
  what_str()
{
  snprintf(file, sizeof(file), "%s", file_);
  snprintf(func, sizeof(func), "%s", func_);
  snprintf(info, sizeof(info), "%s", info_);
}


WossException::WossException(const WossException& copy)
: exception(),
  error_type(copy.error_type),
  file(),
  line(copy.line),
  func(),
  info(),
  what_str()
{
  snprintf(file, sizeof(file), "%s", copy.file);
  snprintf(func, sizeof(func), "%s", copy.func);
  snprintf(info, sizeof(info), "%s", copy.info);
  snprintf(what_str, sizeof(what_str), "%s", copy.what_str);
}

WossException& WossException::operator=(const WossException& copy)
{
  if (this == &copy) return *this;

  error_type = copy.error_type;
  snprintf(file, sizeof(file), "%s", copy.file);
  line = copy.line;
  snprintf(func, sizeof(func), "%s", copy.func);
  snprintf(info, sizeof(info), "%s", copy.info);
  snprintf(what_str, sizeof(what_str), "%s", copy.what_str);
  return *this;
}


const char* WossException::what() const throw() {
  snprintf(const_cast<char*>(what_str), sizeof(what_str), "WossException() err_type:%d, %s:%s:%d, info=%s", (int)error_type, file, func, line, info);
  return what_str;
}


void WossException::printAndExit(const ::exception* e) {
  WossErrorType err = WOSS_ERROR_UNEXPECTED_EXCEPTION;
  const WossException* ptr = dynamic_cast<const WossException* >(e);

  if (ptr != NULL)
  {
    err = ptr->getErrorType();
  }

  cerr << e->what() << endl;

  exit(126 + (int) err);
}


WossTest::WossTest ()
  : debug(false),
    ssp_proto (),
    sediment_proto (),
    pressure_proto (),
    time_arr_proto (),
    transducer_proto (),
    altim_bret_proto (),
    woss_random_gen_stream (0),
    random_gen_proto (),
    time_ref_proto (),
    def_handler (NULL),
    res_db_creator_debug (WH_DEBUG_DEFAULT),
    res_db_debug (WH_DEBUG_DEFAULT),
    res_db_use_binary (true),
    res_db_use_time_arr (false),
    res_db_use_pressure (false),
    res_db_space_sampling (WH_SPACE_SAMPLING_DEFAULT),
    res_db_file_path (WH_STRING_DEFAULT),
    res_db_file_name (WH_STRING_DEFAULT),
    res_db_creator_press_bin (NULL),
    res_db_creator_press_txt (NULL),
    res_db_creator_time_arr_bin (NULL),
    res_db_creator_time_arr_txt (NULL),
#if defined (WOSS_NETCDF_SUPPORT)
#if defined (WOSS_NETCDF4_SUPPORT)
    sedim_deck41_db_type(WH_DECK41_DB_TYPE_DEFAULT),
#endif // defined (WOSS_NETCDF4_SUPPORT)
    sedim_db_creator_debug (WH_DEBUG_DEFAULT),
    sedim_db_debug (WH_DEBUG_DEFAULT),
    sedim_db_coord_file_path (WH_STRING_DEFAULT),
    sedim_db_marsden_file_path (WH_STRING_DEFAULT),
    sedim_db_marsden_one_file_path (WH_STRING_DEFAULT),
    sedim_db_creator (NULL),
    ssp_db_creator_debug (WH_DEBUG_DEFAULT),
    ssp_db_debug (WH_DEBUG_DEFAULT),
    ssp_db_file_path (WH_STRING_DEFAULT),
#if defined (WOSS_NETCDF4_SUPPORT)
    ssp_woa_db_type (WH_WOA_DB_TYPE_DEFAULT),
#endif // defined (WOSS_NETCDF_SUPPORT)
    ssp_db_creator (NULL),
    bathy_db_creator_debug (WH_DEBUG_DEFAULT),
    bathy_db_debug (WH_DEBUG_DEFAULT),
    bathy_db_gebco_format(WH_GEBCO_FORMAT_DEFAULT),
    bathy_db_file_path (WH_STRING_DEFAULT),
    bathy_db_creator (NULL),
#endif // defined (WOSS_NETCDF_SUPPORT)
    woss_db_manager_debug (WH_DEBUG_DEFAULT),
    woss_db_manager (NULL),
    woss_creator_debug (WH_DEBUG_DEFAULT),
    woss_debug (WH_DEBUG_DEFAULT),
    woss_clear_work_dir (true),
    evolution_time_quantum (WH_TIME_EVOLUTION_DEFAULT),
    total_runs (WH_TOTAL_RUNS_DEFAULT),
    frequency_step (WH_FREQUENCY_STEP_DEFAULT),
    total_range_steps (WH_TOTAL_RANGE_STEPS_DEFAULT),
    tx_min_depth_offset (WH_TX_MIN_DEPTH_OFFSET_DEFAULT),
    tx_max_depth_offset (WH_TX_MAX_DEPTH_OFFSET_DEFAULT),
    total_transmitters (WH_TOTAL_TRANSMITTERS_DEFAULT),
    total_rx_depths (WH_TOTAL_RX_DEPTHS_DEFAULT),
    rx_min_depth_offset (WH_RX_MIN_DEPTH_OFFSET_DEFAULT),
    rx_max_depth_offset (WH_RX_MAX_DEPTH_OFFSET_DEFAULT),
    total_rx_ranges (WH_TOTAL_RX_RANGES_DEFAULT),
    rx_min_range_offset (WH_RX_MIN_RANGE_OFFSET_DEFAULT),
    rx_max_range_offset (WH_RX_MAX_RANGE_OFFSET_DEFAULT),
    total_rays (WH_TOTAL_RAYS_DEFAULT),
    min_angle (WH_MIN_ANGLE_DEFAULT),
    max_angle (WH_MAX_ANGLE_DEFAULT),
    use_thorpe_att(true),
    ssp_depth_precision (WH_SSP_DEPTH_PRECISION_DEFAULT),
    normalized_ssp_depth_steps (WH_NORMALIZED_SSP_DEPTH_STEPS_DEFAULT),
    work_dir_path (WH_WORK_PATH_DEFAULT),
    bellhop_path (WH_STRING_DEFAULT),
    bellhop_mode (WH_BELLHOP_MODE_DEFAULT),
    bellhop_beam_options (WH_BEAM_OPTIONS_DEFAULT),
    bellhop_bathy_type (WH_BATHYMETRY_TYPE_DEFAULT),
    bellhop_bathy_method(WH_BATHYMETRY_METHOD_DEFAULT),
    bellhop_altim_type (WH_ALTIMETRY_TYPE_DEFAULT),
    bellhop_arr_syntax (WH_BELLHOP_ARR_SYNTAX_DEFAULT),
    bellhop_shd_syntax (WH_BELLHOP_SHD_SYNTAX_DEFAULT),
    sim_time (),
    bellhop_creator (NULL),
    box_depth (WH_BOX_DEPTH),
    box_range (WH_BOX_RANGE),
    woss_manager_debug (WH_DEBUG_DEFAULT),
    time_evolution_active (false),
    concurrent_threads (WH_CONCURRENT_THREADS_DEFAULT),
    woss_manager_space_sampling (WH_SPACE_SAMPLING_DEFAULT),
    woss_manager_use_multithread (false),
    woss_manager_simple (NULL),
    woss_manager_multi (NULL),
    woss_transducer_handler_debug (WH_DEBUG_DEFAULT),
    woss_transducer_handler (NULL),
    woss_controller_debug (WH_DEBUG_DEFAULT),
    woss_controller (),
    initialized (false)
{
  def_handler = woss::SDefHandler::instance ();
}


WossTest::~WossTest() {
  if (woss_manager_simple != NULL) {
    delete woss_manager_simple;
  }

  if (woss_manager_multi != NULL) {
    delete woss_manager_multi;
  }

  if (woss_transducer_handler != NULL) {
    delete woss_transducer_handler;
  }

  if (bellhop_creator != NULL) {
    delete bellhop_creator;
  }

  if (woss_db_manager != NULL) {
    delete woss_db_manager;
  }

#if defined (WOSS_NETCDF_SUPPORT)
  if (bathy_db_creator != NULL) {
    delete bathy_db_creator;
  }

  if (ssp_db_creator != NULL) {
    delete ssp_db_creator;
  }

  if (sedim_db_creator != NULL) {
    delete sedim_db_creator;
  }
#endif // defined (WOSS_NETCDF_SUPPORT)

  if (res_db_creator_press_bin != NULL) {
    delete res_db_creator_press_bin;
  }

  if (res_db_creator_press_txt != NULL) {
    delete res_db_creator_press_txt;
  }

  if (res_db_creator_time_arr_bin != NULL) {
    delete res_db_creator_time_arr_bin;
  }

  if (res_db_creator_time_arr_txt != NULL) {
    delete res_db_creator_time_arr_txt;
  }

  if (woss_controller != NULL) {
    delete woss_controller;
  }

  initialized = false;
}


void WossTest::config() {
  return doConfig();
}


void WossTest::init() {
  if (initialized == false) {
    if (debug) {
      cout << "Setting DefHandler" << endl;
    }

    def_handler->setSSP (ssp_proto.clone ());
    def_handler->setSediment (sediment_proto.clone ());
    def_handler->setTransducer (transducer_proto.clone ());
    def_handler->setAltimetry (altim_bret_proto.clone ());
    def_handler->setPressure (pressure_proto.clone ());
    def_handler->setTimeArr (time_arr_proto.clone ());
    def_handler->setSSP (ssp_proto.clone ());
    random_gen_proto.setSeed (woss_random_gen_stream);
    random_gen_proto.initialize ();
    def_handler->setRandGenerator (random_gen_proto.clone ());

    woss_controller = new woss::WossController ();

  #if defined (WOSS_NETCDF_SUPPORT)
    if (debug) {
      cout << "Setting BathymetryDbCreator" << endl;
    }

    if ( bathy_db_file_path != WH_STRING_DEFAULT ) {
      bathy_db_creator = new woss::BathyGebcoDbCreator ();

      bathy_db_creator->setDbPathName (bathy_db_file_path);
      bathy_db_creator->setDebug (bathy_db_creator_debug);
      bathy_db_creator->setWossDebug (bathy_db_debug);
      bathy_db_creator->setGebcoBathyType (bathy_db_gebco_format);

      woss_controller->setBathymetryDbCreator (bathy_db_creator);
    }

    if (debug) {
      cout << "Setting SedimDbCreator" << endl;
    }

    if ( sedim_db_coord_file_path != WH_STRING_DEFAULT && sedim_db_marsden_file_path != WH_STRING_DEFAULT
        && sedim_db_marsden_one_file_path != WH_STRING_DEFAULT ) {
      sedim_db_creator = new woss::SedimDeck41DbCreator ();

#if defined (WOSS_NETCDF4_SUPPORT)
      sedim_db_creator->setDeck41DbType (sedim_deck41_db_type);
#endif // defined (WOSS_NETCDF4_SUPPORT)
      sedim_db_creator->setDeck41CoordPathName (sedim_db_coord_file_path);
      sedim_db_creator->setDeck41MarsdenPathName (sedim_db_marsden_file_path);
      sedim_db_creator->setDeck41MarsdenOnePathName (sedim_db_marsden_one_file_path);
      sedim_db_creator->setDebug (sedim_db_creator_debug);
      sedim_db_creator->setWossDebug (sedim_db_debug);

      woss_controller->setSedimentDbCreator (sedim_db_creator);
    }

    if (debug) {
      cout << "Setting SspDbCreator" << endl;
    }

    if ( ssp_db_file_path != WH_STRING_DEFAULT ) {
  #if defined (WOSS_NETCDF4_SUPPORT)
      ssp_db_creator = new woss::SspWoa2005DbCreator ((woss::WOADbType)ssp_woa_db_type);
  #else
      ssp_db_creator = new woss::SspWoa2005DbCreator ();
  #endif // defined (WOSS_NETCDF_SUPPORT)

      ssp_db_creator->setDbPathName (ssp_db_file_path);
      ssp_db_creator->setDebug (ssp_db_creator_debug);
      ssp_db_creator->setWossDebug (ssp_db_debug);

      woss_controller->setSSPDbCreator (ssp_db_creator);
    }
  #endif // defined (WOSS_NETCDF_SUPPORT)

    if (debug) {
      cout << "Setting ResDbCreator" << endl;
    }

    if ( (res_db_file_path != WH_STRING_DEFAULT) && (res_db_file_name != WH_STRING_DEFAULT) ) {
      if ( res_db_use_binary == true && res_db_use_time_arr == true ) {
        if (debug) {
          cout << "Setting TimeArr binary" << endl;
        }
        string name = "arr_bin_" + res_db_file_name;
        res_db_creator_time_arr_bin = new woss::ResTimeArrBinDbCreator ();
        
        res_db_creator_time_arr_bin->setDbPathName (res_db_file_path + "/" + name);
        res_db_creator_time_arr_bin->setDebug (res_db_creator_debug);
        res_db_creator_time_arr_bin->setWossDebug (res_db_debug);

        woss_controller->setTimeArrDbCreator (res_db_creator_time_arr_bin);
      }
      else if ( res_db_use_binary == false && res_db_use_time_arr == true ) {
        if (debug) {
          cout << "Setting TimeArr ASCII" << endl;
        }
        string name = "arr_asc_" + res_db_file_name;

        res_db_creator_time_arr_txt = new woss::ResTimeArrTxtDbCreator ();

        res_db_creator_time_arr_txt->setDbPathName (res_db_file_path + "/" +  name);
        res_db_creator_time_arr_txt->setDebug (res_db_creator_debug);
        res_db_creator_time_arr_txt->setWossDebug (res_db_debug);

        woss_controller->setTimeArrDbCreator (res_db_creator_time_arr_txt);
      }
      if ( res_db_use_binary == true && res_db_use_pressure == true ) {
        if (debug) {
          cout << "Setting Pressure binary" << endl;
        }
        string name = "pres_bin_" + res_db_file_name;

        res_db_creator_press_bin = new woss::ResPressureBinDbCreator ();

        res_db_creator_press_bin->setDbPathName (res_db_file_path + "/" + name);
        res_db_creator_press_bin->setDebug (res_db_creator_debug);
        res_db_creator_press_bin->setWossDebug (res_db_debug);

        woss_controller->setPressureDbCreator (res_db_creator_press_bin);
      }
      else if ( res_db_use_binary == false && res_db_use_pressure == true ) {
        if (debug) {
          cout << "Setting Pressure ASCII" << endl;
        }
        string name = "pres_asc" + res_db_file_name;

        res_db_creator_press_txt = new woss::ResPressureTxtDbCreator ();

        res_db_creator_press_txt->setDbPathName (res_db_file_path + "/" + name);
        res_db_creator_press_txt->setDebug (res_db_creator_debug);
        res_db_creator_press_txt->setWossDebug (res_db_debug);

        woss_controller->setPressureDbCreator (res_db_creator_press_txt);
      }
    }

    if (debug) {
      cout << "Setting Bellhop Creator" << endl;
    }

    bellhop_creator = new woss::BellhopCreator ();

    bellhop_creator->setDebug (woss_creator_debug);
    bellhop_creator->setWossDebug (woss_debug);
    bellhop_creator->setWrkDirPath (work_dir_path);
    bellhop_creator->setCleanWorkDir (woss_clear_work_dir);
    bellhop_creator->setEvolutionTimeQuantum (evolution_time_quantum);
    bellhop_creator->setThorpeAttFlag (use_thorpe_att);
    bellhop_creator->setTotalRuns (total_runs);
    bellhop_creator->setFrequencyStep (frequency_step);
    bellhop_creator->setTotalRangeSteps (total_range_steps);
    bellhop_creator->setTxMinDepthOffset (tx_min_depth_offset);
    bellhop_creator->setTxMaxDepthOffset (tx_max_depth_offset);
    bellhop_creator->setTotalTransmitters (total_transmitters);
    bellhop_creator->setRxTotalDepths (total_rx_depths);
    bellhop_creator->setRxMinDepthOffset (rx_min_depth_offset);
    bellhop_creator->setRxMaxDepthOffset (rx_max_depth_offset);
    bellhop_creator->setRxTotalRanges (total_rx_ranges);
    bellhop_creator->setRxMinRangeOffset (rx_min_range_offset);
    bellhop_creator->setRxMaxRangeOffset (rx_max_range_offset);
    bellhop_creator->setRaysNumber (total_rays);
    bellhop_creator->setAngles (woss::CustomAngles (min_angle, max_angle));
    bellhop_creator->setThorpeAttFlag (use_thorpe_att);
    bellhop_creator->setSspDepthPrecision (ssp_depth_precision);
    bellhop_creator->setSspDepthSteps (normalized_ssp_depth_steps);
    bellhop_creator->setBellhopPath (bellhop_path);
    bellhop_creator->setBhMode (bellhop_mode);
    bellhop_creator->setBeamOptions (bellhop_beam_options);
    bellhop_creator->setBathymetryType (bellhop_bathy_type);
    bellhop_creator->setBathymetryMethod (bellhop_bathy_method);
    bellhop_creator->setAltimetryType (bellhop_altim_type);
    if ((sim_time.start_time.isValid () == false) || (sim_time.end_time.isValid () == false) )
    {
      throw WOSS_EXCEPTION(WOSS_ERROR_UNKNOWN);
    }
    bellhop_creator->setSimTime (sim_time);
    bellhop_creator->setBellhopArrSyntax (bellhop_arr_syntax);
    bellhop_creator->setBellhopShdSyntax (bellhop_shd_syntax);
    bellhop_creator->setBoxDepth(box_depth);
    bellhop_creator->setBoxRange(box_range);
    woss_controller->setWossCreator (bellhop_creator);

    if (debug) {
      cout << "Setting WossDbManager" << endl;
    }

    woss_db_manager = new woss::WossDbManager ();

    woss_db_manager->setDebug (woss_db_manager_debug);

    woss_controller->setWossDbManager (woss_db_manager);

    if (woss_manager_use_multithread == false) {
      if (debug) {
        cout << "Setting WossManager Single Threaded" << endl;
      }
      woss_manager_simple = new woss::WossManagerSimple<woss::WossManagerResDb> ();

      woss_manager_simple->setDebugFlag (woss_manager_debug);
      woss_manager_simple->setTimeEvolutionActiveFlag (time_evolution_active);
      woss_manager_simple->setSpaceSampling (woss_manager_space_sampling);

      woss_controller->setWossManager (woss_manager_simple);
    }
    else {
      if (debug) {
        cout << "Setting WossManager Multi Threaded" << endl;
      }
      woss_manager_multi = new woss::WossManagerSimple<woss::WossManagerResDbMT> ();

      woss_manager_multi->setDebugFlag (woss_manager_debug);
      woss_manager_multi->setTimeEvolutionActiveFlag (time_evolution_active);
      woss_manager_multi->setSpaceSampling (woss_manager_space_sampling);
      woss_manager_multi->setConcurrentThreads (concurrent_threads);

      woss_controller->setWossManager (woss_manager_multi);
    }

    if (debug) {
      cout << "Setting TransducerHandler" << endl;
    }
    woss_transducer_handler = new woss::TransducerHandler ();

    woss_transducer_handler->setDebug (woss_transducer_handler_debug);
    woss_controller->setTransducerHandler (woss_transducer_handler);

    bool isOk = woss_controller->initialize ();

    if (debug) {
      cout << "WossController is initialized: " << (isOk ? "true" : "false") << endl;
    }

    if (isOk == false) {
      throw WOSS_EXCEPTION(WOSS_ERROR_UNKNOWN);
    }

    initialized = true;

    return doInit();
  }
}

void WossTest::run() {
  try {
    config();

    init();

    doRun();
  }
  catch(const std::exception& e) { 
    WossException::printAndExit(&e);
  }
}
