/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2009 Federico Guerra
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
 * @file   bellhop-woss.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::BellhopWoss class
 *
 * Provides the interface for woss::BellhopWoss class
 */


#ifndef WOSS_BELLHOP_H
#define WOSS_BELLHOP_H


#include <iomanip>
#include <sediment-definitions.h>
#include <transducer-definitions.h>
#include "ac-toolbox-arr-asc-reader.h"
#include "ac-toolbox-arr-bin-reader.h"
#include "ac-toolbox-shd-reader.h"
#include "ac-toolbox-woss.h"


namespace woss {
  
  
  typedef ::std::map< double, SSP* > NormSSPMap;
  typedef NormSSPMap::iterator NSMIter;
  typedef NormSSPMap::const_iterator NSMCIter;
  typedef NormSSPMap::reverse_iterator NSMRIter;
  typedef NormSSPMap::const_reverse_iterator NSMCRIter;

  /*
    * .arr file syntax to be used during parsing
    */
  enum BellhopArrSyntax {
    BELLHOP_CREATOR_ARR_FILE_SYNTAX_0 = 0, ///< Pre 31 august 2016 syntax, without imaginary time delay
    BELLHOP_CREATOR_ARR_FILE_SYNTAX_1, ///< Post 31 August 2016 syntax, with imaginary time delay
    BELLHOP_CREATOR_ARR_FILE_SYNTAX_2, ///< Post 31 March 2019 syntax, with different header syntax
    BELLHOP_CREATOR_ARR_FILE_INVALID ///< invalid syntax, must always be the last element
  };
  
  /**
  * \brief Implempentation of ACToolboxWoss for Bellhop raytracing program
  *
  * BellhopWoss class has the task to create enviromental file, run Bellhop and read its results.
  **/
  class BellhopWoss : public ACToolboxWoss { 


    public:


    /**
    * BellhopWoss default constructor. Default constructed objects are not valid
    **/
    BellhopWoss();

    /**
    * BellhopWoss constructor
    * @param tx const reference to a valid CoordZ object ( transmitter )
    * @param rx const reference to a valid CoordZ object ( receiver )
    * @param start_t const reference to a valid Time object for SSP's averaging purposes ( start date time )
    * @param end_t const reference to a valid Time object for SSP's averaging purposes ( end date time )
    * @param start_freq start frequency [Hz]
    * @param end_freq end frequency [Hz]
    * @param freq_step frequency step [Hz]
    **/
    BellhopWoss(const CoordZ& tx, const CoordZ& rx, const Time& start_t, const Time& end_t, double start_freq, double end_freq, double freq_step ) ;

    virtual ~BellhopWoss();

    
    /**
    * Gets enviromental data and writes the env file
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool initialize();

    /**
    * Initializes the ShdResReader object for given frequency
    * @param curr_frequency frequency in use [Hz]
    * @returns <i>true</i> if method succeeded, <i>false</i> otherwise
    **/
    virtual bool initPressResReader( double curr_frequency );

    /**
    * Initializes the ArrAscResReader or ArrBinResReader object for given frequency
    * @param curr_frequency frequency in use [Hz]
    * @returns <i>true</i> if method succeeded, <i>false</i> otherwise
    **/
    virtual bool initTimeArrResReader( double curr_frequency ); 


    /**
    * Runs the channel simulator. It is mandatory to set <b>has_run</b> to <i>true</i> before
    * returning from this function
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/   
    virtual bool run();


    /**
    * Performs a time evoulion of all time-dependant parameters
    * @param time_value constant reference to a valid Time object (between start_time and end_time)
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/  
    virtual bool timeEvolve( const Time& time_value );

    /**
    * Checks the validity of Woss
    * @return <i>true</i> if it's valid, <i>false</i> otherwise
    **/
    virtual bool isValid() const;

    
    virtual Pressure* getAvgPressure( double frequency, double tx_depth, double start_rx_depth = WOSS_MIN_DEPTH, double start_rx_range = WOSS_MIN_RANGE, double end_rx_depth = WOSS_MAX_DEPTH , double end_rx_range = WOSS_MAX_RANGE ) const;

    virtual Pressure* getPressure( double frequency, double tx_depth, double rx_depth, double rx_range ) const;

  
    virtual TimeArr* getTimeArr( double frequency, double tx_depth, double rx_depth, double rx_range ) const;

     /**
     * Sets the thorpe attenuation flag
     * @param flag boolean flag
     * @returns reference to <b>*this</b>
     */    
    BellhopWoss& setThorpeAttFlag( bool flag ) { use_thorpe_att = flag; return *this; }

    /**
    * Sets the transmitter mimimum depth offset [m]
    * @param offset 0 <= depth offset <= 0 [m]
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setTxMinDepthOffset( double offset ) { tx_min_depth_offset = offset; return *this; }

    /**
    * Sets the transmitter maximum depth offset [m]
    * @param offset 0 <= depth offset <= 0 [m]
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setTxMaxDepthOffset( double offset ) { tx_max_depth_offset = offset; return *this; }

    /**
    * Sets the number of transmitters
    * @param sources number of transmitters
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setTotalTransmitters( int sources ) { total_transmitters = sources; return *this; }

    /**
    * Sets the receiver mimimum depth offset [m]
    * @param offset 0 <= depth offset <= 0 [m]
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setRxMinDepthOffset( double offset ) { rx_min_depth_offset = offset; return *this; }

    /**
    * Sets the receiver maximum depth offset [m]
    * @param offset 0 <= depth offset <= 0 [m]
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setRxMaxDepthOffset( double offset ) { rx_max_depth_offset = offset; return *this; }
    
    /**
    * Sets the receiver mimimum range offset [m]
    * @param offset -total_distance <= range offset <= total_distance [m]
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setRxMinRangeOffset( double offset ) { rx_min_range_offset = offset; return *this; }
    
    /**
    * Sets the receiver maximum depth offset [m]
    * @param offset -total_distance <= range offset <= total_distance [m]
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setRxMaxRangeOffset( double offset ) { rx_max_range_offset = offset; return *this; }

    /**
    * Sets the number of receiver depths
    * @param number number of receiver depths
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setRxTotalDepths( int number ) { total_rx_depths = number; return *this; }

    /**
    * Sets the number of receiver ranges
    * @param number number of receiver ranges
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setRxTotalRanges( int number ) { total_rx_ranges = number; return *this; }

    /**
    * Sets the number of launched rays
    * @param number number of launched rays (>= 0)
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setRaysNumber( int number ) { total_rays = number; return *this; }

    /**
    * Sets the mimimum launch angle for all BellhopWoss that will be created
    * @param angle number of launched rays (>= 0)
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setMinAngle( double angle ) { min_angle = angle; return *this; }

    /**
    * Sets the maximum launch angle for all BellhopWoss that will be created
    * @param angle number of launched rays (>= 0)
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setMaxAngle( double angle ) { max_angle = angle; return *this; }

    /**
    * Sets the maximum depth for Bellhop rays
    * @param depth maximum ray depth [m]
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setBoxDepth( double depth ) { box_depth = depth; return *this; }

    /**
    * Sets the maximum range for Bellhop rays
    * @param range maximum ray range [m]
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setBoxRange( double range ) { box_range = range; return *this; }

    /**
    * Sets the path of bellhop program
    * @param path filesystem path
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setBellhopPath( const ::std::string& path ) { bellhop_path = path; return *this; }

    /**
    * Sets the bellhop arr file syntax
    * @param syntax syntax to be used
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setBellhopArrSyntax( BellhopArrSyntax syntax ) { bellhop_arr_syntax = syntax; return *this; }
    
    /**
    * Sets the beam option string . See Bellhop documentation for more info
    * @param options beam options ( G, C, R , B)
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setBeamOptions( const ::std::string& options ) { beam_options = options; return *this; }

    /**
    * Sets the Bellhop run mode string. See Bellhop documentation for more info
    * @param mode Bellhop run mode ( A, a , C, I , S) 
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setBhMode( const ::std::string& mode );

    /**
    * Sets the bathymetry type ( L, C). See Bellhop documentation for more info
    * @param type beam options
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setBathymetryType( const ::std::string& type ) { bathymetry_type = type; return *this; }

    /**
    * Sets the bathymetry write method (S, D)
    * @param type beam options
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setBathymetryMethod( const ::std::string& type ) { bathymetry_method = type; return *this; }

     /**
      * Sets the altimetry type ( L, C). See Bellhop documentation for more info
      * @param type beam options
      * @return reference to <b>*this</b>
      */
    BellhopWoss& setAltimetryType( const ::std::string& type ) { altimetry_type = type; return *this; }
     
     
    /**
    * Sets the transducer type. See TransducerDbCreator for more info
    * @param type tranducer type
    * @return reference to <b>*this</b>
    */
    BellhopWoss& setTransducer( const Transducer* const ptr ) { transducer = ptr; return *this; }
    
    
    /**
    * Sets the depth steps (and possibly transform) of all SSP in use
    * @param type beam options
    * @return reference to <b>*this</b>
    */ 
    BellhopWoss& setTransformSSPDepthSteps( int depth_steps ) { transform_ssp_depth_steps = depth_steps; return *this; }

  
    /**
    * Sets the transmitter beam pattern
    * @param type valid woss::BeamPattern
    * @return reference to <b>*this</b>
    */ 
    BellhopWoss& setBeamPatternParam( double init_bearing, double vert_rot = 0.0, double horiz_rot = 0.0, double mult = 1.0, double add = 0.0 ) {
      bp_initial_bearing = init_bearing; bp_vertical_rotation = vert_rot; bp_horizontal_rotation = horiz_rot;
      bp_mult_costant = mult; bp_add_costant = add; return *this; }
    
  
  //   BellhopWoss& useBeamBlock( bool command ) { using_beam_block = command; }

    /**
     * Gets the thorpe attenuation flag
     * @returns boolean flag
     */
    bool getThorpeAttFlag() const { return use_thorpe_att; }

    /**
    * Gets the transmitter mimimum depth offset [m]
    * @returns depth offset [m]
    */
    double getTxMinDepthOffset() const { return tx_min_depth_offset; }

    /**
    * Gets the transmitter maximum depth offset [m]
    * @returns depth offset [m]
    */
    double getTxMaxDepthOffset() const { return tx_max_depth_offset; }

    /**
    * Gets the number of transmitters
    * @returns number of transmitters
    */
    int getTotalTransmitters() const { return total_transmitters; }

    /**
    * Gets the receiver mimimum depth offset [m]
    * @returns depth offset [m]
    */
    double getRxMinDepthOffset() const { return rx_min_depth_offset; }

    /**
    * Gets the receiver maximum depth offset [m]
    * @returns depth offset [m]
    */
    double getRxMaxDepthOffset() const { return rx_max_depth_offset; }

    /**
    * Gets the receiver mimimum range offset [m]
    * @returns range offset [m]
    */
    double getRxMinRangeOffset() const { return rx_min_range_offset; }

    /**
    * Gets the receiver maximum range offset [m]
    * @returns range offset [m]
    */
    double getRxMaxRangeOffset() const { return rx_max_range_offset; }

    /**
    * Gets the total number of receiver depths
    * @returns number of receiver depths
    */
    int getRxTotalDepths() const { return total_rx_depths; }

    /**
    * Gets the total number of receiver ranges
    * @returns number of receiver ranges
    */
    int getRxTotalRanges() const { return total_rx_ranges; }

    /**
    * Gets the total number of launched angles
    * @returns number of launched angles
    */
    int getRaysNumber() const { return total_rays; }

    /**
    * Gets the minimum launch angle
    * @returns minimum launch angle
    */
    double getMinAngle() const { return min_angle; }

    /**
    * Gets the maximum launch angle
    * @returns maximum launch angle
    */
    double getMaxAngle() const { return max_angle; }

    /**
    * Gets the maximum ray depth
    * @returns maximum ray depth
    */
    double getBoxDepth() const { return box_depth; }
    
    /**
    * Gets the maximum ray range
    * @returns maximum ray range
    */
    double getBoxRange() const { return box_range; }

    /**
    * Returns the depth steps (and possibly transform) of all SSP in use
    * @return transformed SSP depth steps
    */ 
    int getTransformSSPDepthSteps() const  { return transform_ssp_depth_steps; }
    

    /**
    * Gets the path of bellhop program
    * @return path of bellhop program
    */
    ::std::string getBellhopPath() const { return bellhop_path; }
    
   /**
    * Gets the .arr file sintax
    * @return arr file syntax
    */
    BellhopArrSyntax getBellhopArrSyntax() const { return bellhop_arr_syntax; }
    
    /**
    * Gets the beam option string
    * @returns "R" , "B", "C" or "G" 
    */
    ::std::string getBeamOptions() const { return beam_options; }

    /**
    * Gets bathymetry type string
    * @returns "C" or "L"
    */
    ::std::string getBathymetryType() const { return bathymetry_type; }

    /**
    * Gets bathymetry write method string
    * @returns "S" or "D"
    */
    ::std::string getBathymetryMethod() const { return bathymetry_method; }

    /**
    * Gets altimetry type string
    * @returns "C" or "L"
    */
    ::std::string getAltimetryType() const { return altimetry_type; }

    /**
    * Gets bathymetry type string
    * @returns "C" or "L"
    */
    const Transducer* const getTransducer() const { return transducer; }
    
    
    /**
    * Checks the validity of Bellhop run mode
    * @return <i>true</i> if it's valid, <i>false</i> otherwise
    **/
    bool isValidBhMode( const ::std::string& ) const ;

    
  //   bool usingBeamBlock() const { return using_beam_block; }

    /**
    * Signals if the instance is using Bellhop in SHD mode ( C, S , I)
    * @return <i>true</i> if it is using it, <i>false</i> otherwise
    */
    bool usingPressMode() const { return using_press_mode; }

    /**
    * Signals if the instance is using Bellhop in ARR mode ( A, a)
    * @return <i>true</i> if it is using it, <i>false</i> otherwise
    */
    bool usingTimeArrMode() const { return using_time_arrival_mode; }

    /**
    * Signals if the instance is using Bellhop with SSP matrix file
    * @return <i>true</i> if it is using it, <i>false</i> otherwise
    */
    bool usingSSPFile() const { return using_ssp_file; }


    protected:

    /**
     * let bellhop calculate thorpe attenuation
     **/
    bool use_thorpe_att;
    
    /**
    * Bellhop beam options ( G, C, R, B )
    **/
    ::std::string beam_options;
    
    /**
    * Bathymetry type ( L, C )
    **/
    ::std::string bathymetry_type;

    /**
    * Bathymetry write method ( S, D )
    **/
    ::std::string bathymetry_method;

    /**
     * Altimetry type ( L, C )
     **/
    ::std::string altimetry_type;    
    
    /**
    * Bellhop run mode ( A, a, C, I, S )
    **/
    ::std::string bellhop_op_mode;


    /**
    * Pathname of Bellhop configuration file
    **/ 
    ::std::string bellhop_env_file;

    /**
    * Pathname of Bellhop bathymetry file
    **/ 
    ::std::string bathymetry_file;
    
    /**
    * Pathname of Bellhop altimetry file
    **/ 
    ::std::string altimetry_file;
    
    /**
    * Pathname of Bellhop beam pattern file
    **/     
    ::std::string beam_pattern_file;
    

    /**
    * Pathname of Bellhop SSP file
    **/ 
    ::std::string ssp_file;
    
    /**
    * Pathname of Bellhop SHD file
    **/ 
    ::std::string shd_file;
    
    /**
    * Pathname of Bellhop ARR file
    **/ 
    ::std::string arr_file;

    /**
    * Pathname of Bellhop program
    **/
    ::std::string bellhop_path;
    
    /**
     * .arr file syntax to be used during file parsing
     */
    BellhopArrSyntax bellhop_arr_syntax;
    
    /**
    * Current working path
    **/    
    ::std::string curr_path;
    
    ::std::string transducer_type;
    
    
    /**
    * Transmitter minimum depth offset [m]
    **/  
    double tx_min_depth_offset;

    /**
    * Transmitter maximum depth offset [m]
    **/ 
    double tx_max_depth_offset;

    /**
    * Number of transmitter
    **/  
    int total_transmitters;


    /**
    * Number of receiver depths
    **/  
    int total_rx_depths;

    /**
    * Receiver minimum depth offset [m]
    **/
    double rx_min_depth_offset;

    /**
    * Receiver maximum depth offset [m]
    **/
    double rx_max_depth_offset;


   /**
    * Number of receiver ranges.
    *
    * On some configuration (linux distribution / cpu ) bellhop will output an empty file
    * with a value of <i>total_rx_ranges</i> = 1.
    **/  
    int total_rx_ranges;

    /**
    * Receiver minimum range offset [m]
    **/
    double rx_min_range_offset;

    /**
    * Receiver maximum range offset [m]
    **/
    double rx_max_range_offset;


    /**
    * Number of launched rays
    **/ 
    int total_rays;
    
    
    /**
    * Minimum launching angle [decimal degrees]
    **/ 
    double min_angle;

    /**
    * Maximum launching angle [decimal degrees]
    **/ 
    double max_angle;


    /**
    * Minimum depth of normalized SSP matrix [m]
    **/ 
    double min_normalized_ssp_depth;

    /**
    * Maximum depth of normalized SSP matrix [m]
    **/ 
    double max_normalized_ssp_depth;

    /**
    * Depth steps of all SSP involved
    **/ 
    int curr_norm_ssp_depth_steps;
    
    
    /**
    * transformed SSP depth steps
    **/ 
    int transform_ssp_depth_steps;

    
    /**
    * Transmitter beam pattern
    **/     
    const Transducer* transducer;
    
    
    double bp_initial_bearing;
    
    double bp_vertical_rotation;
    
    double bp_horizontal_rotation;
    
    double bp_mult_costant;
    
    double bp_add_costant;
        
    
    bool using_ssp_file;

    bool using_press_mode;

    bool using_time_arrival_mode;

  //   bool using_beam_block; 

    /**
    * SSP matrix normalized for Bellhop requirements (same depths and same depth-steps for all SSP involved)
    **/ 
    NormSSPMap normalized_ssp_map;


    /**
    * Bellhop box depth [m]
    **/ 
    double box_depth;

    /**
    * Bellhop box range [m]
    **/ 
    double box_range;

    
    /**
    * ofstream for file writing
    **/
    ::std::ofstream f_out;

    
    /**
    * Normalizes the SSP matrix for Bellhop requirements (same depths and same depth-steps for all SSP involved)
    **/
    virtual void normalizeDbSSP();

    /**
     * Deletes all normalized SSPs
     **/    
    virtual void resetNormalizedDbSSP();
    
//     bool initTransducer();
    
    /**
    * Initializes the ResReader object for given frequency
    * @param curr_frequency frequency in use [Hz]
    * @returns <i>true</i> if method succeeded, <i>false</i> otherwise
    **/
    bool initResReader( double curr_frequency );
  
    /**
    * Initializes the Bellhop box
    * @param depth box depth [m]
    * @param range box range [m]
    **/
    void initBox( double depth, double range );

    /**
    * Initializes the configuration file(s)
    * @param curr_frequency frequency in use [Hz]
    * @param curr_run current run number
    **/
    void initCfgFiles( double curr_frequency, int curr_run );
   
    
    /**
    * Writes the configuration file(s)
    * @param curr_frequency frequency in use [Hz]
    * @param curr_run current run number
    **/
    void writeCfgFiles( double curr_frequency, int curr_run ); 

    /**
    * Writes all configuration files
    **/
    void writeAllCfgFiles(); 

    /**
    * Writes the normalized SSP matrix in the configuration file(s)
    * @param curr_run current run number
    **/  
    void writeNormalizedSSP( int curr_run );

    /**
    * Writes the db Sediment in the configuration file(s)
    **/  
    void writeSediment();

    /**
    * Writes the header in the configuration file(s)
    * @param curr_frequency frequency in use [Hz]
    * @param curr_run current run number
    **/
    void writeHeader( double curr_frequency, int curr_run );

    /**
    * Writes the transmitter info in the configuration file(s)
    **/ 
    void writeTransmitter();

    /**
    * Writes the receiver info in the configuration file(s)
    **/ 
    void writeReceiver(); 

    /**
    * Writes the ray options in the configuration file(s)
    **/ 
    void writeRayOptions();

    /**
    * Writes the box options in the configuration file(s)
    **/ 
    void writeBox();

    void writeBeamBlock(); 

    /**
    * Writes db created bathymetry in the configuration file(s)
    **/ 
    void writeBathymetryFile();    
  
    /**
    * Writes transmitter beam pattern file(s)
    **/ 
    void writeBeamPatternFile(); 
    
    /**
    * Writes db created altimetry in the configuration file(s)
    * @param curr_run current run number
    **/ 
    void writeAltimetryFile( int curr_run );    

    
    /**
    * Writes all configuration files
    **/
    void removeAllCfgFiles();
    
    /**
    * Removes the configuration file(s)
    * @param curr_frequency frequency in use [Hz]
    * @param curr_run current run number
    **/
    void removeCfgFiles( double curr_frequency, int curr_run ); 
    
    
    /**
    * Checks parameters passed by a WossManager request and modifies them in place if they are not consistent with data
    * @param frequency frequency [Hz]
    * @param tx_depth transmitter depth [m]
    * @param rx_start_depth receiver start depth [m]
    * @param rx_start_range receiver start range [m]
    * @param rx_end_depth receiver end depth [m]
    * @param rx_end_range receiver end range [m]
    **/ 
    void checkBoundaries( double& frequency, double& tx_depth, double& rx_start_depth, double& rx_start_range, double& rx_end_depth, double& rx_end_range ) const;

    /**
    * Checks the consistency of tx and rx depth offsets and modifies them if needed
    **/ 
    void checkDepthOffsets();

    /**
    * Checks the consistency of input offsets and modifies them if needed
    * @param coords 3-D coordinates of the point to be tested
    * @param min_offset min offset to be tested [m]
    * @param max_offset max offset to be tested [m]
    * @param min_depth_value min depth value of point to be tested [m]
    * @param max_depth_value max depth value of point to be tested [m]
    **/ 
    void checkDepthOffsets( const CoordZ& coords, double& min_offset, double& max_offset, double min_depth_value, double max_depth_value );

    /**
    * Checks the consistency of given launching angle and modifies them if needed
    **/ 
    void checkAngles();

    /**
    * Checks the consistency of rx range offsets and modifies them if needed
    **/ 
    void checkRangeOffsets();
  
  
  };


  //inline functions
  //////////
  inline bool BellhopWoss::isValidBhMode( const ::std::string& mode ) const {
    return (  mode == "a" || mode == "A" || mode == "C" || mode == "I" || mode == "S" );
  }


  inline void BellhopWoss::initBox( double depth, double range ) {
	  if(box_depth < 0) //box depth not set by user - default to depth + 10%
		  box_depth = depth + depth / 10.0;

	  if(box_range < 0) //box range not set by user - default to range + 10%
		  box_range = range + range / 10.0;

  }


  inline void BellhopWoss::writeSediment() {
    f_out << "\'A*\' 0.0" << ::std::setw(30) << "! BOTTOM TYPE" << ::std::endl
          << max_normalized_ssp_depth << " " << sediment_value->getStringValues() <<  "  /  ! " 
          << sediment_value->getType() << " BOTTOM TYPE " << ::std::endl;
  }


  inline void BellhopWoss::writeHeader( double curr_frequency, int curr_run ) {
    f_out.precision(18);
    f_out << "\'BELLHOP - woss id = " << woss_id << "; run = " << curr_run << "\'" << ::std::endl
          << curr_frequency << ::std::setw(30) << "! FREQUENCY [HZ]" << ::std::endl
          << 1 << ::std::setw(30) << "! NMEDIA" << ::std::endl;   
  }


  inline void BellhopWoss::writeTransmitter() {
    f_out << total_transmitters << ::std::setw(30) << "! NUMBER OF SOURCES" << ::std::endl;
    if (total_transmitters == 1) f_out << tx_coordz.getDepth() + tx_min_depth_offset << "  " << "/" << ::std::setw(30) << "! SOURCE'S DEPTH" << ::std::endl;
    else f_out << tx_coordz.getDepth() + tx_min_depth_offset << "  " << tx_coordz.getDepth() + tx_max_depth_offset 
              << "  " << "/" << ::std::setw(30) << "! SOURCES' DEPTHS" << ::std::endl;
  }


  inline void BellhopWoss::writeReceiver() {  
    f_out << total_rx_depths << ::std::setw(30) << "! NUMBER OF RX DEPTH(S)" << ::std::endl;
    if (total_rx_depths == 1) f_out << rx_coordz.getDepth() + rx_min_depth_offset << "  " << "/" << ::std::setw(30) << "! RX'S DEPTH" << ::std::endl;
    else f_out << rx_coordz.getDepth() + rx_min_depth_offset << "  " << rx_coordz.getDepth() + rx_max_depth_offset << "  " << "/" 
              << ::std::setw(30) << "! RX'S DEPTHS" << ::std::endl;

    f_out << total_rx_ranges << ::std::setw(30) << "!NUMBER OF RX RANGE(S)" << ::std::endl;
    if (total_rx_ranges == 1) f_out << ((total_great_circle_distance + rx_min_range_offset) / 1000.0) << "  " << "/" << ::std::setw(30) << "! RX'S RANGE" << ::std::endl;
    else f_out << ((total_great_circle_distance + rx_min_range_offset) / 1000.0) << "  " << ((total_great_circle_distance + rx_max_range_offset)/1000.0) << "  " << "/" 
              << ::std::setw(30) << "! RX'S RANGES" << ::std::endl;
  }


  inline void BellhopWoss::writeRayOptions() {
    f_out << "\'"<< bellhop_op_mode << beam_options; 
    
    if ( transducer->isValid() ) f_out << "*";   
    
    f_out << "\'" << ::std::setw(30) << "! RAY OPTIONS" << ::std::endl 
          << total_rays << ::std::setw(30) << "! NUMBER OF RAYS" << ::std::endl
          << min_angle << ::std::setw(4) << max_angle << ::std::setw(4) << "/" << ::std::setw(30) << "! START, END ANGLES" << ::std::endl;   
  }


  inline void BellhopWoss::writeBox() {
    f_out << "0.0" << " " << box_depth << " " << (box_range/1000.0)  << " ,  "
                  << "! RAY-STEP , BOX DEPTH, BOX RANGE" << ::std::endl;
  }


  inline void BellhopWoss::writeBeamBlock() {  
    f_out << "\'MS\' 1.0 100.0 0," << ::std::endl << "3 5" << ::std::endl;
  }


}


#endif /* WOSS_BELLHOP_H */




