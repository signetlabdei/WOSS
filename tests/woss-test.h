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
    bool res_db_use_time_arr; //!< set up the woss result databases data format: power delay profile or single pressure tap.
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
    double spp_depth_precision;  //!< woss object configuration: SSP depth precision in meters
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


