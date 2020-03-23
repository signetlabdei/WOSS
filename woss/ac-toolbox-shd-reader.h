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
 * @file   ac-toolbox-shd-reader.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::ShdResReader and woss::ShdData classes
 *
 * Provides the interface for the woss::ShdResReader and woss::ShdData classes
 */


#ifndef AC_TOOLBOX_RES_READER_DEFINITIONS_H
#define AC_TOOLBOX_RES_READER_DEFINITIONS_H


#include <fstream>
#include <time-arrival-definitions.h>
#include "res-reader.h"


namespace woss {
  
    
  /**
  * \brief class for storing data of any acoustic toolbox SHD file
  *
  * class ShdData stores Pressure provided by any acoustic toolbox SHD file
  */
  class ShdData {

    
    public:
      
      
    ShdData();
    
    /**
    * Destructor
    */
    ~ShdData() { delete[] theta; delete[] tx_depths; delete[] rx_ranges; delete[] rx_depths; delete[] press_values ; }
    
    
    /**
    * Record byte length of a binary SHD file. See Bellhop code for more info
    */
    int32_t record_length;
    

    /**
    * Plot typename. See Bellhop code for more info
    */
    char* plot_type;

    
    /**
    * Frequency value [Hz]
    */
    float frequency;


    /**
    * Total number of theta values. See Bellhop code for more info
    */
    int32_t Ntheta;

    /**
    * Pointer to an array of theta values
    */
    float* theta;


    /**
    * Total number of transmitter depths
    */
    int32_t Nsd;

    /**
    * Pointer to an array of transmitter depths [m]
    */
    float* tx_depths;


    /**
    * Total number of receiver depths
    */
    int32_t Nrd;

    /**
    * Pointer to an array of receiver depths [m]
    */
    float* rx_depths;

    
    /**
    * Total number of receiver ranges
    */
    int32_t Nrr;

    /**
    * Pointer to an array of receiver ranges [m]
    */
    float* rx_ranges;

    /**
    * Total number of receiver per range
    */
    int32_t Nrx_per_range;


    /**
    * Pointer to an array of complex\<double\> values [m]
    */
    ::std::complex<double>* press_values;


    /**
    * Initializes the struct 
    */
    void initialize() { plot_type = NULL; Ntheta = 0; theta = NULL; Nrx_per_range = 0; record_length = 0; tx_depths = NULL;
                        rx_depths = NULL; rx_ranges = NULL; Nrr = 0; Nrd = 0; Nsd = 0; press_values = NULL; frequency = 0.0; }


    /**
    * Returns the press_values index associated to given parameters
    * @param tx_depth transmitter depth [m]
    * @param rx_depth receiver depth [m]
    * @param rx_range receiver range [m]
    * @param theta theta value
    * @returns valid press_values index value
    */
    int getPressureIndex( double tx_depth, double rx_depth, double rx_range, double theta = 0.0 ) const;

    /**
    * Returns the index of given array associated to given value
    * @param value test value
    * @param array valid pointer to an array 
    * @param array_size size of passed array
    * @returns valid array index value
    */
    int getIndex( float value, float* array, int32_t array_size ) const;

    
  };


  /**
  * \brief Class for reading and manipulating results provided by any acoustic toolbox SHD file
  *
  * Class ShdResReader stores Pressure provided by any acoustic toolbox SHD file in a ShdData. It also offers
  * Pressure manipulation and TimeArr conversion methods.
  */
  class ShdResReader : public ResReader {


    public:


    /**
    * ShdResReader default constructor
    */
    ShdResReader();

    /**
    * ShdResReader constructor
    * @param woss const pointer to a const Woss object
    */
    ShdResReader( const Woss* const woss );

    virtual ~ShdResReader();

    /**
    * Initializes the ShdResReader object, reads SHD file, and stores read Pressure values
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool initialize();

    /**
    * Gets the average Pressure value in given rx range-depth box
    * @param tx_depth transmitter depth [m]
    * @param start_rx_depth start receiver depth [m]
    * @param start_rx_range start receiver range [m]
    * @param end_rx_depth end receiver depth [m]
    * @param end_rx_range end receiver range [m]
    * @return a valid Pressure value; a not valid Pressure if shd_file hasn't been read yet
    **/  
    virtual Pressure* readAvgPressure( double tx_depth, double start_rx_depth, double start_rx_range, double end_rx_depth, double end_rx_range ) const;

    /**
    * Gets a Pressure value of given range, depths
    * @param tx_depth transmitter depth [m]
    * @param rx_depth receiver depth [m]
    * @param rx_range receiver range [m]
    * @return a valid Pressure value; a not valid Pressure if shd_file hasn't been read yet
    **/
    virtual Pressure* readPressure( double tx_depth, double rx_depth, double rx_range ) const;

    
    /**
    * SHD files don't hold any time arrivals information. A special TimeArr is constructed from Pressure associated
    * to given paramaters.
    * @param tx_depth transmitter depth [m]
    * @param rx_depth receiver depth [m]
    * @param rx_range receiver range [m]
    * @return a special TimeArr that holds a single Pressure value and no delay information
    **/
    virtual TimeArr* readTimeArr( double tx_depth, double rx_depth, double rx_range ) const; 
    
    
    protected:
      

    /**
    * Boolean associated to the reading of SHD file header
    */
    bool shd_header_collected;
    
    /**
    * Boolean associated to the reading of SHD file data
    */
    bool shd_file_collected;


    /**
    * Input file stream 
    */
    ::std::ifstream file_reader;


    /**
    * Struct that holds Pressure data read from SHD file
    */
    ShdData shd_file;


    /**
    * Gets the average Pressure value in given rx range-depth box from ShdData Pressure array
    * @param tx_depth transmitter depth [m]
    * @param start_rx_depth start receiver depth [m]
    * @param start_rx_range start receiver range [m]
    * @param end_rx_depth end receiver depth [m]
    * @param end_rx_range end receiver range [m]
    * @param theta theta value
    * @return a valid Pressure value; a not valid Pressure if shd_file hasn't been read yet
    **/  
    ::std::complex<double> readMapAvgPressure( double tx_depth, double start_rx_depth, double start_rx_range, double end_rx_depth, double end_rx_range, double theta = 0.0 ) const;


    /**
    * Gets the Pressure value from ShdData Pressure array associated to given parameters
    * @param tx_depth transmitter depth [m]
    * @param rx_depth start receiver depth [m]
    * @param rx_range start receiver range [m]
    * @param theta theta value
    * @return a valid Pressure value; a not valid Pressure if shd_file hasn't been read yet
    **/  
    ::std::complex<double> accessMap( double tx_depth, double rx_depth, double rx_range, double theta = 0.0 ) const;


    /**
    * Process the SHD file data
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    bool getShdFile();

    /**
    * Process the SHD file header
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    bool getShdHeader();
    
    
  };


  ////
  // inline functions

  inline ::std::complex<double> ShdResReader::accessMap( double tx_depth, double rx_depth, double rx_range, double theta ) const {
    return( shd_file.press_values[ shd_file.getPressureIndex( tx_depth, rx_depth, rx_range, theta ) ] ); 
  }


}


#endif /* AC_TOOLBOX_RES_READER_DEFINITIONS_H */

 
