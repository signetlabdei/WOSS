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
 * @file   woss-test.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for WossTest class
 *
 * Provides the interface for the WossTest class
 */


#ifndef WOSS_TEST_H 
#define WOSS_TEST_H


#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <ssp-definitions.h>
#include <sediment-definitions.h>
#include <altimetry-definitions.h>
#include <pressure-definitions.h>
#include <time-arrival-definitions.h>
#include <transducer-definitions.h>
#include <transducer-handler.h>
#include <res-pressure-bin-db-creator.h>
#include <res-pressure-txt-db-creator.h>
#include <res-time-arr-bin-db-creator.h>
#include <res-time-arr-txt-db-creator.h>
#if defined (WOSS_NETCDF_SUPPORT)
#include <bathymetry-gebco-db-creator.h>
#include <sediment-deck41-db-creator.h>
#include <ssp-woa2005-db-creator.h>
#endif // defined (WOSS_NETCDF_SUPPORT)
#include <bellhop-creator.h>
#include <woss-manager-simple.h>
#include <woss-controller.h>


namespace woss {

  #define WOSS_EXCEPTION_INFO(err_type, info) WossException(err_type, __FILE__, __LINE__, __FUNCTION__, info)

  #define WOSS_EXCEPTION(err_type) WossException(err_type, __FILE__, __LINE__, __FUNCTION__, "")


  /**
   * error type enum
   **/
  enum WossErrorType {
    WOSS_ERROR_NO_ERROR = 0,
    WOSS_ERROR_OUT_OF_MEMORY, ///< out of memory
    WOSS_ERROR_OUT_OF_RANGE_PARAM, ///< a parameter passed to the function is outside the valid range
    WOSS_ERROR_INVALID_PARAM, ///< a parameter passed to the function is invalid
    WOSS_ERROR_TIMEOUT, ///< timeout has occurred
    WOSS_ERROR_WRONG_STATE, ///< the requested operation is not possible/allowed in the current state
    WOSS_ERROR_BAD_FORMAT, ///< data is not in the expected format
    WOSS_ERROR_INSUFFICIENT_PERMISSIONS, ///< the requester of the operation does not have sufficient permissions (access rights etc)
    WOSS_ERROR_IO_ERROR, ///< an I/O error occurred
    WOSS_ERROR_OUT_OF_HANDLES, ///< there is no free handle available
    WOSS_ERROR_UNKNOWN,
    WOSS_ERROR_UNEXPECTED_EXCEPTION,
  };

  /**
    * \brief Exception thrown by the Woss test framework
    *
    * WossException class stores the file, the line and the type of the thrown exception
    **/ 
  class WossException : public std::exception {

    public:

   /**
     * Max length of all char arrays 
     **/
    static const int CHAR_ARR_LENGTH = 250;

   /**
     * WossException constructor
     * @param error_type_ error type
     * @param file_ file where the exception has been thrown
     * @param line_ line where the exception has been thrown
     * @param func_ function name where the exception has been thrown
     * @param info_ additional info c string
     **/
    WossException(WossErrorType error_type_, const char* file_, int line_, const char* func_, const char* info_);

    /**
      * copy constructor 
      **/
    WossException(const WossException& copy);

    /**
      * destructor
      **/
    virtual ~WossException() throw () { }

    /**
      * assignement operator
      * @param copy the source copy
      **/
    WossException& operator=(const WossException& copy);


    /**
      * inherited from base class
      * @return a c-string description
      **/
    virtual const char* what() const throw();


    /**
      * Get current error type
      * @return the current WossErrorType
      **/
    WossErrorType getErrorType() const { return error_type; }

    /**
      * Get current file c string
      * @return const char* 
      **/
    const char* getFile() const { return file; }

    /**
      * Get current line
      * @return the current line
      **/
    int getLine() const { return line; }

    /**
      * Get current function c string
      * @return const char*
      **/
    const char* getFunc() const { return func; }

    /**
      * Get current info c string
      * @return const char*
      **/
    const char* getInfo() const { return info; }

    /**
      * Function that print the current exception and exit the test
      * @param e input exception
      **/
    static void printAndExit(const ::std::exception* e);


    protected:

    /**
      * current error type
      **/
    WossErrorType error_type;

    /**
      * current file c string
      **/
    char file[CHAR_ARR_LENGTH];

    /**
      * current line
      **/
    int line;
    /**
      * current function c string
      **/
    char func[CHAR_ARR_LENGTH];
    /**
      * current info c string
      **/
    char info[CHAR_ARR_LENGTH];
    /**
      * current what c string
      **/
    char what_str[4*CHAR_ARR_LENGTH];

  };

  /**
    * TestTimeReference provides the minimum functionality of the simulator time reference
    **/
  class TestTimeReference: public TimeReference {

    public:
    /**
      * Default constructor
      **/
    TestTimeReference() 
    : TimeReference(),
      time_reference(0.0) { }

    /**
      * Copy constructor
      * @param copy const reference to a TestTimeReference object
      **/
    TestTimeReference(const TestTimeReference& copy)
    : time_reference(copy.time_reference)
    { }

    /**
      * Default destructor
      **/
    virtual ~TestTimeReference() { }

    /**
     * virtual factory method
     * @return a heap-allocated copy of <b>this</b> instance
     **/
    virtual TestTimeReference* clone() { return new TestTimeReference(*this); }

    /**
      * Sets the current simulation time
      * @param time simulation time [s]
      **/
    void setTimeReference(double time) { time_reference = time; }
    /**
     * Retuns simulation time
     * @return value in seconds
     **/
    virtual double getTimeReference() const { return time_reference; }

    private:

    /**
      * Simulated time reference in seconds
      **/
    double time_reference;

  };


  /**
    * \brief WossTest provides the framework for the WOSS library test binaries
    * 
    * WossTest provides the framework for the WOSS library test binaries
    **/
  class WossTest {

    public: 

    /**
      * default constructor
      **/
    WossTest();

    /**
      * default destructor
      **/
    virtual ~WossTest();

    /**
      * Runs the current test.
      * Must be called after the configuration phase
      * and the initialization phase.
      **/
    void run();

    /**
      * Sets the WossTest object debug flag
      * @param flag debug flag
      * @return reference to this instance
      **/
    WossTest& setWossTestDebug(bool flag) { debug = flag; return *this; }

    /**
      * gets the WossTest object debug flag
      * @return debug flag
      **/
    bool getWossTestDebug() const { return debug; }


    /**
      * Sets the woss::RandomGenerator stream/seed value
      * @param stream seed/stream value
      * @return reference to this instance
      **/
    WossTest& setWossRandomGenStream(int stream) { woss_random_gen_stream = stream; return *this;}

    /**
      * Gets the woss::RandomGenerator stream/seed value
      * @return seed/stream value
      **/
    int getWossRandomGenStream() const { return woss_random_gen_stream; }


    /**
      * Sets the Result Db Creator debug flag
      * @param flag debug flag
      * @return reference to this instance
      **/
    WossTest& setResDbCreatorDebug(bool flag) { res_db_creator_debug = flag; return *this; }

    /**
      * Gets the Result Db Creator debug flag
      * @return debug flag
      **/
    bool getResDbCreatorDebug() const { return res_db_creator_debug; }

    /**
      * Sets the Result Db debug flag
      * @param flag debug flag
      * @return reference to this instance
      **/
    WossTest& setResDbDebug(bool flag) { res_db_debug = flag; return *this; }

    /**
      * Gets the Result Db debug flag
      * @return debug flag
      **/
    bool getResDbDebug() const { return res_db_debug; }

    /**
      * If set to true, it will use the ResDb binary format classes
      * @param flag use binary flag
      * @return reference to this instance
      **/
    WossTest& setResDbUseBinary(bool flag) { res_db_use_binary = flag; return *this; }

    /**
      * Gets the use binary flag
      * @return use binary flag
      **/
    bool getResDbUseBinary() const { return res_db_use_binary; }

    /**
      * If set to true, it will use the ResDb TimeArrival format classes
      * If set to false, it will use the ResDb Pressure format classes
      * @param flag use TimeArrival flag
      * @return reference to this instance
      **/
    WossTest& setResDbUseTimeArr(bool flag) { res_db_use_time_arr = flag; return *this; }

    /**
      * Gets the time arrival flag
      * @return use TimeArrival flag
      **/
    bool getResDbUseTimeArr() const { return res_db_use_time_arr; }

    /**
      * If set to true, it will use the ResDb Pressure format classes
      * If set to false, it will use the ResDb Pressure format classes
      * @param flag use Pressure flag
      * @return reference to this instance
      **/
    WossTest& setResDbUsePressure(bool flag) { res_db_use_pressure = flag; return *this; }

    /**
      * Gets the res db pressure flag
      * @return use Pressure flag
      **/
    bool getResDbUsePressure() const { return res_db_use_pressure; }

    /**
      * Sets the ResDb's space sampling
      * @param sampling space sampling in [m]
      * @return reference to this instance
      **/
    WossTest& setResDbSpaceSampling(double sampling) { res_db_space_sampling = sampling; return *this; }

    /**
      * Gets the ResDb's space sampling
      * @return space sampling in [m]
      **/
    double getResDbSpaceSampling() const { return res_db_space_sampling; }


    /**
      * Sets the ResDb'file path
      * @param path file path
      * @return reference to this instance
      **/
    WossTest& setResDbFilePath(const ::std::string& path) { res_db_file_path = path; return *this; }

    /**
      * Gets the ResDb'file path
      * @return file path
      **/
    ::std::string getResDbFilePath() const { return res_db_file_path; }

    /**
      * Sets the ResDb file name
      * @param path file name
      * @return reference to this instance
      **/
    WossTest& setResDbFileName(const ::std::string& file_name) { res_db_file_name = file_name; return *this; }

    /**
      * Gets the ResDb'file name
      * @return file name
      **/
    ::std::string getResDbFileName() const { return res_db_file_name; }


#if defined (WOSS_NETCDF_SUPPORT)
#if defined (WOSS_NETCDF4_SUPPORT)
    /**
      * Sets the DECK41DbType db type
      * @param type DECK41DbType type
      * @return reference to this instance
      **/
    WossTest& setSedimDeck41DbType( DECK41DbType type) { sedim_deck41_db_type = type; return *this; }

    /**
      * Gets the DECK41DbType db type
      * @return DECK41DbType db type
      **/
    DECK41DbType getSedimDeck41DbType() const { return sedim_deck41_db_type; }
#endif // defined (WOSS_NETCDF4_SUPPORT)

    /**
      * Sets the SedimDeck41DbCreator debug flag
      * @param flag debug boolean value
      * @return reference to this instance
      **/
    WossTest& setSedimDbCreatorDebug( bool flag ) { sedim_db_creator_debug = flag; return *this; }

    /**
      * Gets the SedimDeck41DbCreator debug flag
      * @return debug boolean value
      **/
    bool getSedimDbCreatorDebug() const { return sedim_db_creator_debug; }

    /**
      * Sets the Sediment db debug flag
      * @param flag debug boolean value
      * @return reference to this instance
      **/
    WossTest& setSedimDbDebug(bool flag) { sedim_db_debug = flag; return *this; }

    /**
      * Gets the Sediment db debug flag
      * @return debug boolean value
      **/
    bool getSedimDbDebug() const { return sedim_db_debug; }

    /**
      * Sets the Sediment DECK41 Db indexed by coordinates
      * @param path path to db file
      * @return reference to this instance
      **/
    WossTest& setSedimDbCoordFilePath(const ::std::string& path) { sedim_db_coord_file_path = path; return *this; }

    /**
      * Gets the Sediment DECK41 Db indexed by coordinates path
      * @return string containing the file path
      **/
    ::std::string getSedimDbCoordFilePath() const { return sedim_db_coord_file_path; }

    /**
      * Sets the Sediment DECK41 Db indexed by Marsden square file path
      * @param path path to db file
      * @return reference to this instance
      **/
    WossTest& setSedimDbMarsdenFilePath(const ::std::string& path) { sedim_db_marsden_file_path = path; return *this; }

    /**
      * Gets the Sediment DECK41 Db indexed by Marsden squares file path
      * @return string containing the file path
      **/
    ::std::string getSedimDbMarsdenFilePath() const { return sedim_db_marsden_file_path; }

    /**
      * Sets the Sediment DECK41 Db indexed by Marsden one squares file path
      * @param path path to db file
      * @return reference to this instance
      **/
    WossTest& setSedimDbMarsdenOneFilePath(const ::std::string& path) { sedim_db_marsden_one_file_path = path; return *this; }

    /**
      * Gets the Sediment DECK41 Db indexed by Marsden one squares file path
      * @return string containing the file path
      **/
    ::std::string getSedimDbMarsdenOneFilePath() const { return sedim_db_marsden_one_file_path; }


    /**
      * Sets the SspWoa2005DbCreator debug flag
      * @param flag debug boolean value
      * @return reference to this instance
      **/
    WossTest& setSspDbCreatorDebug( bool flag ) { ssp_db_creator_debug = flag; return *this; }

    /**
      * Gets the SspWoa2005DbCreator debug flag
      * @return boolean debug flag value
      **/
    bool getSspDbCreatorDebug() const { return ssp_db_creator_debug; }

    /**
      * Sets the SSP Db debug flag
      * @param flag debug boolean value
      * @return reference to this instance
      **/
    WossTest& setSspDbDebug( bool flag ) { ssp_db_debug = flag; return *this; }

    /**
      * Gets the Ssp Db debug flag
      * @return boolean debug flag value
      **/
    bool getSspDbDebug() const { return ssp_db_debug; }

    /**
      * Sets the SSP Db file path
      * @param path path to db file
      * @return reference to this instance
      **/
    WossTest& setSspDbFilePath(const ::std::string& path) { ssp_db_file_path = path; return *this; }

    /**
      * Gets the SSP Db file path
      * @return string containing the path to db file
      **/
    ::std::string getSspDbFilePath() const { return ssp_db_file_path; }

#if defined (WOSS_NETCDF4_SUPPORT)
    /**
      * Sets the SSP Db WOADbType type
      * @param type WOADbType of the db path
      * @return reference to this instance
      **/
    WossTest& setSspWoaDbType( WOADbType type) { ssp_woa_db_type = type; return *this; }

    /**
      * Gets the SSP Db WOADbType type
      * @return WOADbType of the db path
      **/
    WOADbType getSspWoaDbType() const { return ssp_woa_db_type; }
#endif // defined (WOSS_NETCDF_SUPPORT)


    /**
      * Sets the BathyGebcoDbCreator debug flag
      * @param flag debug boolean value
      * @return reference to this instance
      **/
    WossTest& setBathyDbCreatorDebug( bool flag ) { bathy_db_creator_debug = flag; return *this; }

    /**
      * Gets the BathyGebcoDbCreator debug flag
      * @return debug boolean value
      **/
    bool getBathyDbCreatorDebug() const { return bathy_db_creator_debug; }

    /**
      * Sets the Bathy Db debug flag
      * @param flag debug boolean value
      * @return reference to this instance
      **/
    WossTest& setBathyDbDebug( bool flag ) { bathy_db_debug = flag; return *this; }

    /**
      * Gets the Bathy Db debug flag
      * @return debug boolean value
      **/
    bool getBathyDbDebug() const { return bathy_db_debug; }

    /**
      * Sets the Bathy Db GEBCO_BATHY_TYPE format type
      * @param type GEBCO_BATHY_TYPE of the file db path
      * @return reference to this instance
      **/
    WossTest& setBathyDbGebcoFormat(GEBCO_BATHY_TYPE type) { bathy_db_gebco_format = type; return *this; }

    /**
      * Gets the Bathy Db GEBCO_BATHY_TYPE format type
      * @return GEBCO_BATHY_TYPE of the file db path
      **/
    GEBCO_BATHY_TYPE getBathyDbGebcoFormat() const { return bathy_db_gebco_format; }

    /**
      * Sets the Bathy Db file path
      * @param path path to db file
      * @return reference to this instance
      **/
    WossTest& setBathyDbFilePath(const ::std::string& path) { bathy_db_file_path = path; return *this; }

    /**
      * Gets the Bathy Db file path
      * @param path path to db file
      * @return string containing the path to Bathy Db file
      **/
    ::std::string getBathyDbFilePath() const { return bathy_db_file_path; }
#endif // defined (WOSS_NETCDF_SUPPORT)


    /**
      * Sets the WossDbManager debug flag
      * @param flag debug flag
      * @return reference to this instance
      **/
    WossTest& setWossDbManagerDebug(bool flag) { woss_db_manager_debug = flag; return *this; }

    /**
      * Gets the WossDbManager debug flag
      * @return boolean debug flag
      **/
    bool getWossDbManagerDebug() const { return woss_db_manager_debug; }


    /**
      * Sets the WossCreator debug flag
      * @param flag debug flag
      * @return reference to this instance
      **/
    WossTest& setWossCreatorDebug(bool flag) { woss_creator_debug = flag; return *this; }

    /**
      * Gets the WossCreator debug flag
      * @return boolean debug flag
      **/
    bool getWossCreatorDebug() const { return woss_creator_debug; }

    /**
      * Sets the Woss channel simulator debug flag
      * @param flag debug flag
      * @return reference to this instance
      **/
    WossTest& setWossDebug(bool flag) { woss_debug = flag; return *this; }

    /**
      * Gets the Woss channel simulator debug flag
      * @return boolean debug flag
      **/
    bool getWossDebug() const { return woss_debug; }

    /**
      * Configure if the Woss channel simulator working dir should be freed or kept
      * @param flag clean work dir boolean flag
      * @return reference to this instance
      **/
    WossTest& setWossClearWorkDir(bool flag) { woss_clear_work_dir = flag; return *this; }

    /**
      * Gets the Woss clean dir flag., if the Woss channel simulator working dir should be freed or kept
      * @return clean work dir boolean flag
      **/
    bool getWossClearWorkDir() const { return woss_clear_work_dir; }

    /**
      * Configures the Woss channel simulator time quantum to be used by time dependent simulations
      * @param time evolution time quantum of the channel model, in [s]
      * @return reference to this instance
      **/
    WossTest& setWossEvolutionTimeQuantum(double time) { evolution_time_quantum = time; return *this; }

    /**
      * Gets the Woss channel simulator time quantum which is used by time dependent simulations
      * @return evolution time quantum of the channel model, in [s]
      **/
    double getWossEvolutionTimeQuantum() const { return evolution_time_quantum; }

    /**
      * Configures the Woss channel simulator total realizations.
      * Each run is based on random perturbation of the input data.
      * @param runs number of total runs to be executed
      * @return reference to this instance
      **/
    WossTest& setWossTotalRuns(int runs) { total_runs = runs; return *this; }

    /**
      * Gets the Woss channel simulator total realizations number.
      * Each run is based on random perturbation of the input data.
      * @return number of total runs to be executed
      **/
    int getWossTotalRuns() const { return total_runs; }

    /**
      * Configures the Woss channel simulator frequencies that should be computed.
      * The steps are applied between the start and the end frequencies.
      * @param frequency_step frequency step in [hZ]
      * @return reference to this instance
      **/
    WossTest& setWossFrequencyStep(double step) { frequency_step = step; return *this; }

    /**
      * Gets the Woss channel simulator frequencies frequency steps
      * @return frequency step in [hZ]
      **/
    double getWossFrequencyStep() const { return frequency_step; }

    /**
      * Configures the Woss channel simulator range steps that should be used
      * The steps are applied between the tx and rx distance.
      * @param total_range_steps number of range steps
      * @return reference to this instance
      **/
    WossTest& setWossTotalRangeSteps(double step) { total_range_steps = step; return *this; }

    /**
      * Gets the Woss channel simulator range steps that should be used
      * The steps are applied between the tx and rx distance.
      * @return number of range steps
      **/
    double getWossTotalRangeSteps() const { return total_range_steps; }

    /**
      * Configures the Woss channel simulator transmitter minimum depth offset 
      * @param offset minimum depth offset in [m]
      * @return reference to this instance
      **/
    WossTest& setWossTxMinDepthOffset(double offset) { tx_min_depth_offset = offset; return *this; }

    /**
      * Gets the Woss channel simulator transmitter minimum depth offset 
      * @return minimum depth offset in [m]
      **/
    double getWossTxMinDepthOffset() const { return tx_min_depth_offset; }

    /**
      * Configures the Woss channel simulator transmitter maximum depth offset 
      * @param offset minimum depth offset in [m]
      * @return reference to this instance
      **/
    WossTest& setWossTxMaxDepthOffset(double offset) { tx_max_depth_offset = offset; return *this; }

    /**
      * Gets the Woss channel simulator transmitter maximum depth offset 
      * @return maximum depth offset in [m]
      **/
    double getWossTxMaxDepthOffset() const { return tx_max_depth_offset; }

    /**
      * Configures the Woss channel simulator number of transmitters
      * @param transmitters number of transmitters
      * @return reference to this instance
      **/
    WossTest& setWossTotalTransmitters(int transmitters) { total_transmitters = transmitters; return *this; }

    /**
      * Gets the Woss channel simulator number of transmitters
      * @return number of transmitters
      **/
    int getWossTotalTransmitters() const { return total_transmitters; }

    /**
      * Configures the Woss channel simulator number of receiver depth points
      * @param rx_depths number of receiver depth points
      * @return reference to this instance
      **/
    WossTest& setWossTotalRxDepths(int rx_depths) { total_rx_depths = rx_depths; return *this; }

    /**
      * Gets the Woss channel simulator number of receiver depth points
      * @return number of receiver depth points
      **/
    int getWossTotalRxDepths() const { return total_rx_depths; }

    /**
      * Configures the Woss channel simulator receiver mimimum depth offset
      * @param offset receiver mimimum depth offset in [m]
      * @return reference to this instance
      **/
    WossTest& setWossRxMinDepthOffset(double offset) { rx_min_depth_offset = offset; return *this; }

    /**
      * Gets the Woss channel simulator receiver mimimum depth offset
      * @return receiver mimimum depth offset in [m]
      **/
    double getWossRxMinDepthOffset() const { return rx_min_depth_offset; }

    /**
      * Configures the Woss channel simulator receiver maximum depth offset
      * @param offset receiver maximum depth offset in [m]
      * @return reference to this instance
      **/
    WossTest& setWossRxMaxDepthOffset(double offset) { rx_max_depth_offset = offset; return *this; }

    /**
      * Gets the Woss channel simulator receiver maximum depth offset
      * @return receiver maximum depth offset in [m]
      **/
    double getWossRxMaxDepthOffset() const { return rx_max_depth_offset; }

    /**
      * Configures the Woss channel simulator receiver number of range points
      * @param rx_ranges receiver range point number
      * @return reference to this instance
      **/
    WossTest& setWossTotalRxRanges(int rx_ranges) { total_rx_ranges = rx_ranges; return *this; }

    /**
      * Gets the Woss channel simulator receiver number of range points
      * @return receiver range point number
      **/
    int getWossTotalRxRanges() const { return total_rx_ranges; }

    /**
      * Configures the Woss channel simulator receiver mimimum range offset
      * @param offset receiver mimimum range offset [m]
      * @return reference to this instance
      **/
    WossTest& setWossRxMinRangeOffset(double offset) { rx_min_range_offset = offset; return *this; }

    /**
      * Gets the Woss channel simulator receiver mimimum range offset
      * @return receiver mimimum range offset [m]
      **/
    double getWossRxMinRangeOffset() const { return rx_min_range_offset; }

    /**
      * Configures the Woss channel simulator receiver maximum range offset
      * @param offset receiver maximum range offset [m]
      * @return reference to this instance
      **/
    WossTest& setWossRxMaxRangeOffset(double offset) { rx_max_range_offset = offset; return *this; }

    /**
      * Gets the Woss channel simulator receiver maximum range offset
      * @return receiver maximum range offset [m]
      **/
    double getWossRxMaxRangeOffset() const { return rx_max_range_offset; }

    /**
      * Configures the Woss channel simulator number of ray traces
      * @param rays total number of ray traces
      * @return reference to this instance
      **/
    WossTest& setWossTotalRays(double rays) { total_rays = rays; return *this; }

    /**
      * Gets the Woss channel simulator number of ray traces
      * @return total number of ray traces
      **/
    double getWossTotalRays() const { return total_rays; }

    /**
      * Configures the Woss channel simulator ray tracing mimimum angle
      * @param angle ray tracing mimimum angle [decimal degree]
      * @return reference to this instance
      **/
    WossTest& setWossMinAngle(double angle) { min_angle = angle; return *this; }

    /**
      * Gets the Woss channel simulator ray tracing mimimum angle
      * @return ray tracing mimimum angle [decimal degree]
      **/
    double getWossMinAngle() const { return min_angle; }

    /**
      * Configures the Woss channel simulator ray tracing maximum angle
      * @param angle ray tracing maximum angle [decimal degree]
      * @return reference to this instance
      **/
    WossTest& setWossMaxAngle(double angle) { max_angle = angle; return *this; }

    /**
      * Gets the Woss channel simulator ray tracing maximum angle
      * @return ray tracing maximum angle [decimal degree]
      **/
    double getWossMaxAngle() const { return max_angle; }

    /**
      * Configures if the Woss channel simulator should add the thorpe attenuation to its computed model
      * @param flag add the Thorpe attenuation model
      * @return reference to this instance
      **/
    WossTest& setWossUseThorpeAtt(bool flag) {use_thorpe_att = flag; return *this; }

    /**
      * Gets the Thorpe attenuation model flag
      * @return Thorpe attenuation model flag
      **/
    bool getWossUseThorpeAtt() const { return use_thorpe_att; }

    /**
      * Configures the Woss channel simulator SSP depth precision
      * @param precision SSP depth precision in [m]
      * @return reference to this instance
      **/
    WossTest& setWossSspDepthPrecision(double precision) { ssp_depth_precision = precision; return *this; }

    /**
      * Gets the Woss channel simulator SSP depth precision
      * @param precision SSP depth precision in [m]
      * @return reference to this instance
      **/
    double getWossSspDepthPrecision() const { return ssp_depth_precision; }

    /**
      * Configures the Woss channel simulator number normalized SSP depth
      * @param steps normalized SSP depth steps
      * @return reference to this instance
      **/
    WossTest& setWossNormalizedSspDepthSteps( double steps ) { normalized_ssp_depth_steps = steps; return *this; }

    /**
      * Gets the Woss channel simulator normalized depth steps
      * @return normalized SSP depth steps
      **/
    double getWossNormalizedSspDepthSteps() const { return normalized_ssp_depth_steps; }

    /**
      * Configures the Woss channel simulator work directory path
      * @param path path of the work directory
      * @return reference to this instance
      **/
    WossTest& setWossWorkDirPath(const ::std::string& path) { work_dir_path = path; return *this; }

    /**
      * Gets the Woss channel simulator work directory path
      * @return a string containing the path of the work directory
      **/
    ::std::string getWossWorkDirPath() const { return work_dir_path; }

    /**
      * Configures the Woss channel bellhop executable file path
      * @param path bellhop executable file path
      * @return reference to this instance
      **/
    WossTest& setWossBellhopPath(const ::std::string& path) { bellhop_path = path; return *this; }

    /**
      * Gets the Woss channel simulator bellhop executable file path
      * @return a string containing the path of the bellhop executable file
      **/
    ::std::string getWossBellhopPath() const { return bellhop_path; }

    /**
      * Configures the Woss channel bellhop running mode
      * @param mode bellhop running mode
      * @return reference to this instance
      **/
    WossTest& setWossBellhopMode(const ::std::string& mode) { bellhop_mode = mode; return *this; }

    /**
      * Gets the Woss channel simulator bellhop running mode
      * @return bellhop running mode
      **/
    ::std::string getWossBellhopMode() const { return bellhop_mode; }

    /**
      * Configures the Woss channel bellhop beam options
      * @param mode bellhop beam options
      * @return reference to this instance
      **/
    WossTest& setWossBellhopBeamOptions(const ::std::string& options) { bellhop_beam_options = options; return *this; }

    /**
      * Gets the Woss channel simulator bellhop beam options
      * @return bellhop beam options
      **/
    ::std::string getWossBellhopBeamOptions() const { return bellhop_beam_options; }

    /**
      * Configures the Woss channel bellhop bathymetry type
      * @param mode bellhop bathymetry type
      * @return reference to this instance
      **/
    WossTest& setWossBellhopBathyType(const ::std::string& type) { bellhop_bathy_type = type; return *this; }

    /**
      * Gets the Woss channel simulator bellhop bathymetry type
      * @return bellhop bathymetry type
      **/
    ::std::string getWossBellhopBathyType() const { return bellhop_bathy_type; }
    
    /**
      * Configures the Woss channel bellhop bathymetry method
      * @param mode bellhop bathymetry method
      * @return reference to this instance
      **/
    WossTest& setWossBellhopBathyMethod(const ::std::string& type) { bellhop_bathy_method = type; return *this; }

    /**
      * Gets the Woss channel simulator bellhop bathymetry method
      * @return bellhop bathymetry method
      **/
    ::std::string getWossBellhopBathyMethod() const { return bellhop_bathy_method; }

    /**
      * Configures the Woss channel bellhop altimetry type
      * @param mode bellhop altimetry type
      * @return reference to this instance
      **/
    WossTest& setWossBellhopAltimType(const ::std::string& type) { bellhop_altim_type = type; return *this; }

    /**
      * Gets the Woss channel simulator bellhop altimetry type
      * @return bellhop altimetry type
      **/
    ::std::string getWossBellhopAltimType() const { return bellhop_altim_type; }

    /**
      * Configures the Woss channel bellhop array model result syntax
      * @param BellhopArrSyntax bellhop array model result syntax
      * @return reference to this instance
      **/
    WossTest& setWossBellhopArraySyntax(BellhopArrSyntax syntax) { bellhop_arr_syntax = syntax; return *this; }

    /**
      * Gets the Woss channel bellhop array model result syntax
      * @return bellhop array model result syntax
      **/
    BellhopArrSyntax getWossBellhopArraySyntax() const { return bellhop_arr_syntax; }

    /**
      * Configures the Woss channel bellhop pressure model result syntax
      * @param BellhopShdSyntax bellhop pressure model result syntax
      * @return reference to this instance
      **/
    WossTest& setWossBellhopShdSyntax(BellhopShdSyntax syntax) { bellhop_shd_syntax = syntax; return *this; }

    /**
      * Gets the Woss channel bellhop pressure model result syntax
      * @return bellhop pressure model result syntax
      **/
    BellhopShdSyntax getWossBellhopShdSyntax() const { return bellhop_shd_syntax; }

    /**
      * Configures the Woss SimTime
      * @param time bellhop simulation times
      * @return reference to this instance
      **/
    WossTest& setWossSimTime(const SimTime& time) { sim_time = time; return *this; }

    /**
      * Gets the Woss channel SimTime
      * @return simulation times
      **/
    SimTime getWossSimTime() const { return sim_time; }

    /**
      * Configures the Woss channel simulation box max depth
      * @param depth simulation box max depth [m]
      * @return reference to this instance
      **/
    WossTest& setWossBoxDepth(double depth) { box_depth = depth; return *this; }

    /**
      * Gets the Woss channel simulation box max depth
      * @return simulation box max depth [m]
      **/
    double getWossBoxDepth() const { return box_depth; }

    /**
      * Configures the Woss channel simulation box max range
      * @param range simulation box max range [m]
      * @return reference to this instance
      **/
    WossTest& setWossBoxRange(double range) { box_range = range; return *this; }

    /**
      * Gets the Woss channel simulation box max range
      * @return simulation box max range [m]
      **/
    double getWossBoxRange() const { return box_range; }


    /**
      * Configures the WossManager debug flag
      * @param flag debug flag
      * @return reference to this instance
      **/
    WossTest& setWossManagerDebug(bool flag) { woss_manager_debug = flag; return *this; }

    /**
      * Gets the WossManager debug flag
      * @return debug flag
      **/
    bool getWossManagerDebug() const { return woss_manager_debug; }

    /**
      * Configures the WossManager time evolution feature active
      * @param flag time evolution active flag
      * @return reference to this instance
      **/
    WossTest& setWossManagerTimeEvoActive(bool flag) { time_evolution_active = flag; return *this; }

    /**
      * Gets the WossManager time evolution feature active
      * @return time evolution active flag
      **/
    bool getWossManagerTimeEvoActive() const { return time_evolution_active; }

    /**
      * Configures the WossManager number of threads
      * @param threads number of threads
      * @return reference to this instance
      **/
    WossTest& setWossManagerThreads(int threads) { concurrent_threads = threads; return *this; }

    /**
      * Gets the WossManager number of threads
      * @return number of threads
      **/
    int getWossManagerThreads() const { return concurrent_threads; }

    /**
      * Configures the WossManager space sampling
      * @param sampling space sampling in [m]
      * @return reference to this instance
      **/
    WossTest& setWossManagerSpaceSampling(double sampling) { woss_manager_space_sampling = sampling; return *this; }

    /**
      * Gets the WossManager space sampling
      * @return space sampling in [m]
      **/
    double getWossManagerSpaceSampling() const { return woss_manager_space_sampling; }

    /**
      * Configures the WossManager multithread feature
      * @param flag use multithread flag
      * @return reference to this instance
      **/
    WossTest& setWossManagerUseMultiThread(bool flag) { woss_manager_use_multithread = flag; return *this; }

    /**
      * Gets the WossManager multithread flag
      * @return use multithread flag
      **/
    bool getWossManagerUseMultiThread() const { return woss_manager_use_multithread; }


    /**
      * Configures the WossTransducerHander debug flag
      * @param flag debug flag
      * @return reference to this instance
      **/
    WossTest& setWossTransducerHandlerDebug(bool flag) { woss_transducer_handler_debug = flag; return *this; }

    /**
      * Gets the WossTransducerHander debug flag
      * @return debug flag
      **/
    bool getWossTransducerHandlerDebug() const { return woss_transducer_handler_debug; }


    /**
      * Configures the WossController debug flag
      * @param flag debug flag
      * @return reference to this instance
      **/
    WossTest& setWossControllerDebug(bool flag) { woss_controller_debug = flag; return *this; }

    /**
      * Gets the WossController debug flag
      * @return debug flag
      **/
    bool getWossControllerDebug() const { return woss_controller_debug; }


    protected:

    bool debug; //!< debug flag

    SSP ssp_proto; //!< SSP prototype which will be plugged into the WOSS framework.
    Sediment sediment_proto; //!< Sediment prototype which will be plugged into the WOSS framework.
    Pressure pressure_proto; //!< Pressure prototype which will be plugged into the WOSS framework.
    TimeArr time_arr_proto; //!< TimeArr prototype which will be plugged into the WOSS framework.
    Transducer transducer_proto; //!< Transducer prototype which will be plugged into the WOSS framework.
    AltimBretschneider altim_bret_proto; //!< AltimBretschneider prototype which will be plugged into the WOSS framework.
    int woss_random_gen_stream; //!< random generator used by the WOSS framework.
    RandomGenerator random_gen_proto; //!< RandomGenerator prototype which will be plugged into the WOSS framework.
    TestTimeReference time_ref_proto; //!< TimeReference prototype which will be plugged into the WOSS framework.

    DefHandler *def_handler; //!< Definition Handler object allocated by the helper.

    bool res_db_creator_debug; //!< enable/disable the debug prints of the woss results database creator.
    bool res_db_debug; //!< enable/disable the debug prints of the woss result databases.
    bool res_db_use_binary; //!< set up the woss result database format: binary or textual.
    bool res_db_use_time_arr; //!< set up the woss result databases data format: power delay profile.
    bool res_db_use_pressure; //!< set up the woss result databases data format: single pressure tap.
    double res_db_space_sampling; //!< set up the woss result database 2D spatial sampling
    ::std::string res_db_file_path; //!< set up the woss result database file path
    ::std::string res_db_file_name; //!< set up the woss result database file name (without path)
    ResPressureBinDbCreator *res_db_creator_press_bin; //!< the helper will automatically allocate the desired result database creator based on current configuration.
    ResPressureTxtDbCreator *res_db_creator_press_txt; //!< the helper will automatically allocate the desired result database creator based on current configuration.
    ResTimeArrBinDbCreator *res_db_creator_time_arr_bin; //!< the helper will automatically allocate the desired result database creator based on current configuration.
    ResTimeArrTxtDbCreator *res_db_creator_time_arr_txt; //!< the helper will automatically allocate the desired result database creator based on current configuration.

#if defined (WOSS_NETCDF_SUPPORT)
#if defined (WOSS_NETCDF4_SUPPORT)
    DECK41DbType sedim_deck41_db_type; //!< DECK41 db data format type
#endif // defined (WOSS_NETCDF4_SUPPORT)
    bool sedim_db_creator_debug; //!< enable/disable the debug prints of the woss sediment database creator.
    bool sedim_db_debug; //!< enable/disable the debug prints of the woss sediment database.
    ::std::string sedim_db_coord_file_path; //!< setup the path of the sediment database indexed by geographical coordinates with decimal degrees resolution
    ::std::string sedim_db_marsden_file_path; //!< setup the path of the sediment database indexed by geographical coordinates with marsden square resolution
    ::std::string sedim_db_marsden_one_file_path; //!< setup the path of the sediment database indexed by geographical coordinates with marsden one square resolution
    SedimDeck41DbCreator *sedim_db_creator; //!< the helper will automatically allocate the woss sediment database creator

    bool ssp_db_creator_debug; //!< enable/disable the debug prints of the woss SSP database creator.
    bool ssp_db_debug; //!< enable/disable the debug prints of the woss SSP database.
    ::std::string ssp_db_file_path; //!< setup the path of the woss monthly SSP database indexed by geographical coordinates
#if defined (WOSS_NETCDF4_SUPPORT)
    WOADbType ssp_woa_db_type; //!< WOA SSP Db Type
#endif // defined (WOSS_NETCDF_SUPPORT)
    SspWoa2005DbCreator *ssp_db_creator; //!< the helper will automatically allocate the woss sediment database creator

    bool bathy_db_creator_debug; //!< enable/disable the debug prints of the woss Bathymetry database creator.
    bool bathy_db_debug; //!< enable/disable the debug prints of the woss Bathymetry database (GEBCO).
    GEBCO_BATHY_TYPE bathy_db_gebco_format; //!< setup the the GEBCO database format
    ::std::string bathy_db_file_path; //!< setup the path of the woss GEBCO database
    BathyGebcoDbCreator *bathy_db_creator; //!< the helper will automatically allocate the woss GEBCO bathymetry db creator
#endif // defined (WOSS_NETCDF_SUPPORT)

    bool woss_db_manager_debug; //!< enable/disable the debug prints of the woss DB manager object.

    WossDbManager *woss_db_manager; //!< the helper will automatically allocate the woss DB manager

    bool woss_creator_debug; //!< enable/disable the debug prints of the woss creator.
    bool woss_debug; //!< enable/disable the debug prints of all woss objects
    bool woss_clear_work_dir; //!< woss object configuration: clear the temporary files created by the underlying channel simulator
    double evolution_time_quantum;  //!< woss object configuration: evolution time quantum
    int total_runs;  //!< woss object configuration: total number of channel simulator runs
    double frequency_step;  //!< woss object configuration: number of frequency steps
    double total_range_steps;  //!< woss object configuration: tx - rx range sampling in meters
    double tx_min_depth_offset;  //!< woss object configuration: transmitter min offset in meters
    double tx_max_depth_offset;  //!< woss object configuration: transmitter max offset in meters
    int total_transmitters;  //!< woss object configuration: total number of transmitting sources
    int total_rx_depths;  //!< woss object configuration: receiver depth sampling
    double rx_min_depth_offset;  //!< woss object configuration: receiver min depth offset in meters
    double rx_max_depth_offset;  //!< woss object configuration: receiver max depth offset in meters
    int total_rx_ranges;  //!< woss object configuration: receiver range sampling
    double rx_min_range_offset;  //!< woss object configuration: receiver min range offset in meters
    double rx_max_range_offset;  //!< woss object configuration: receiver max range offset in meters
    double total_rays;  //!< woss object configuration: total number of rays (0 = automatic)
    double min_angle;  //!< woss object configuration: minimum vertical angle in decimal degrees
    double max_angle;  //!< woss object configuration: maximum vertical angle in decimal degrees
    bool use_thorpe_att; //!< woss object configuration: use thorpe attenuation in channel computation
    double ssp_depth_precision;  //!< woss object configuration: SSP depth precision in meters
    double normalized_ssp_depth_steps;  //!< woss object configuration: SSP depth quantization steps
    ::std::string work_dir_path;  //!< woss object configuration: path of the temporary directory
    ::std::string bellhop_path;  //!< woss object configuration: path of the Bellhop executable
    ::std::string bellhop_mode;  //!< woss object configuration: Bellhop mode string
    ::std::string bellhop_beam_options;  //!< woss object configuration: Bellhop beam option string
    ::std::string bellhop_bathy_type;  //!< woss object configuration: Bellhop bathymetry type string
    ::std::string bellhop_bathy_method; //!< woss object configuration: Bathymetry write method string
    ::std::string bellhop_altim_type;  //!< woss object configuration: Bellhop Altimetry type string
    BellhopArrSyntax bellhop_arr_syntax; //!< woss object configration: Bellhop Arr file syntax to be used during .arr file parsing
    BellhopShdSyntax bellhop_shd_syntax; //!< woss object configration: Bellhop Shd file syntax to be used during .shd file parsing
    SimTime sim_time;  //!< woss object configuration: woss simulation times (start and end sim times)
    BellhopCreator *bellhop_creator; //!< the helper will automatically allocate the woss creator
    double box_depth; //!< woss object configuration: maximum depth to trace rays to; deeper rays will be ignored
    double box_range; //!< woss object configuration: maximum range to trace rats to; longer rays will be ignored

    bool woss_manager_debug; //!< enable/disable the debug prints of the woss manager.
    bool time_evolution_active; //!< enable/disable the time evolution feature.
    int concurrent_threads; //!< number of concurrent threads (0 = auto).
    double woss_manager_space_sampling; //!< woss manager space sampling in meters
    bool woss_manager_use_multithread; //!< enable/disable the multithread feature
    WossManagerSimple<WossManagerResDb> *woss_manager_simple; //!<  the helper will automatically allocate the desired woss manager based on current configuration.
    WossManagerSimple<WossManagerResDbMT> *woss_manager_multi; //!<  the helper will automatically allocate the desired woss manaeger based on current configuration.

    bool woss_transducer_handler_debug; //!< enable/disable the debug prints of the woss transducer handler.
    TransducerHandler *woss_transducer_handler; //!< the helper will automatically allocate the woss Transducer handler.

    bool woss_controller_debug; //!< enable/disable the debug prints of the woss conteroller
    WossController *woss_controller; //!< the helper will automatically allocate the woss controller.

    bool initialized; //!< initialization complete flag.

    /**
      * Perform the configuration phase.
      * Must be run before the initialization phase.
      **/
    void config();

    /**
      * Performs the initialization phase.
      * Must be called after the configuration phase.
      **/
    void init();

    /**
      * Virtual function that allows the derived class to 
      * perform its custom configuration phase
      **/
    virtual void doConfig() = 0;

    /**
      * Virtual function that allows the derived class to 
      * perform its custom initialization phase
      **/
    virtual void doInit() = 0;

    /**
      * Virtual function that allows the derived class to 
      * perform its custom run phase
      **/
    virtual void doRun() = 0;


    private:

    /**
      * Deleted copy constructor
      * @param copy const reference to a WossTest object
      **/
    WossTest(const WossTest& copy);

    /**
      * Deleted assignement operator
      * @param copy const reference to a WossTest object
      **/
    WossTest& operator=(const WossTest& copy);

  };

}


#endif /* WOSS_TEST_H */


