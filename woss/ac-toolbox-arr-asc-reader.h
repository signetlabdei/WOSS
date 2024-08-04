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
 * @file   ac-toolbox-arr-asc-reader.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::ArrAscResReader and woss::ArrData classes
 *
 * Provides the interface for the woss::ArrAscResReader and woss::ArrData classes
 */


#ifndef AC_TOOLBOX_ARR_ASC_READER_DEFINITIONS_H
#define AC_TOOLBOX_ARR_ASC_READER_DEFINITIONS_H


#include <fstream>
#include <stdint.h>
#include <time-arrival-definitions.h>
#include "res-reader.h"


namespace woss {
  
    
  /**
  * \brief class for storing data of any acoustic toolbox ARR file
  *
  * class ArrData stores TimeArr values provided by any acoustic toolbox ARR file
  */
  class ArrData {

    
    public: 
      
      
    ArrData();
    
    
    /**
    * Destructor
    */
    ~ArrData() { delete[] tx_depths; delete[] rx_ranges; delete[] rx_depths; delete[] arr_values; }

    
    /**
    * Frequency value [Hz]
    */
    float frequency;


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
    * Pointer to an array of TimeArr values [m]
    */
    TimeArr* arr_values;


    /**
    * Initializes the struct 
    */
    void initialize() { tx_depths = NULL; rx_depths = NULL; rx_ranges = NULL; arr_values = NULL;
                        Nrr = 0; Nrd = 0; Nsd = 0; frequency = 0.0; }


    /**
    * Returns the arr_values index associated to given parameters
    * @param tx_depth transmitter depth [m]
    * @param rx_depth receiver depth [m]
    * @param rx_range receiver range [m]
    * @returns valid arr_values index value
    */
    int getTimeArrIndex( double tx_depth, double rx_depth, double rx_range ) const;

    /**
    * Returns the index of given array associated to given value
    * @param value test value
    * @param array valid pointer to an array 
    * @param array_size size of passed array
    * @returns valid array index value
    */
    int getIndex( float value, float* array, int array_size ) const;

    
  };


  /**
  * \brief Class for reading and manipulating results provided by any acoustic toolbox textual ARR file
  *
  * Class ArrAscResReader stores TimeArr provided by any acoustic toolbox textual ARR file in a ArrData. It also offers
  * TimeArr manipulation and Pressure conversion methods.
  */
  class ArrAscResReader : public ResReader {


    public:


    /**
    * ArrAscResReader default constructor
    */
    ArrAscResReader();

    /**
    * ArrAscResReader constructor
    * @param woss const pointer to a const Woss object
    */
    ArrAscResReader( const Woss* const woss );

    virtual ~ArrAscResReader();


    /**
    * Initializes the ArrAscResReader object, reads ARR file, and stores read TimeArr values
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool initialize();


    /**
    * Gets the average Pressure value in given rx range-depth box. Returned Pressure is the coherent sum of the computed 
    * average TimeArr
    * @param frequency frequency [hZ]
    * @param tx_depth transmitter depth [m]
    * @param start_rx_depth start receiver depth [m]
    * @param start_rx_range start receiver range [m]
    * @param end_rx_depth end receiver depth [m]
    * @param end_rx_range end receiver range [m]
    * @return a valid Pressure value; a not valid Pressure if arr_file hasn't been read yet
    **/  
    virtual Pressure* readAvgPressure( double frequency, double tx_depth, double start_rx_depth, double start_rx_range, double end_rx_depth, double end_rx_range );

    /**
    * Gets a Pressure value for given range, depths. Returned Pressure is the coherent sum of the computed 
    * TimeArr
    * @param frequency frequency [hZ]
    * @param tx_depth transmitter depth [m]
    * @param rx_depth receiver depth [m]
    * @param rx_range receiver range [m]
    * @return a valid Pressure value; a not valid Pressure if arr_file hasn't been read yet
    **/
    virtual Pressure* readPressure( double frequency, double tx_depth, double rx_depth, double rx_range ) const; 
    
    
    /**
    * Gets a TimeArr value for given range, depths
    * @param frequency frequency [hZ]
    * @param tx_depth transmitter depth [m]
    * @param rx_depth receiver depth [m]
    * @param rx_range receiver range [m]
    * @return a valid TimeArr value; a not valid TimeArr if arr_file hasn't been read yet
    **/
    virtual TimeArr* readTimeArr( double frequency, double tx_depth, double rx_depth, double rx_range ) const;
    

    protected:
      
    
    /**
    * Boolean associated to the reading of ARR file header
    */
    bool arr_asc_header_collected;
      
    /**
    * Boolean associated to the reading of ARR file data
    */
    bool arr_asc_file_collected;


    /**
    * Input file stream 
    */
    ::std::ifstream file_reader;

    /**
    * Total lines to skip if ARR file header has already been read
    */
    ::std::streampos skip_header;
    
    
    /**
    * Struct that holds TimeArr data read from ARR file
    */
    ArrData arr_file;

    double last_tx_depth;

    double last_start_rx_depth;

    double last_start_rx_range;

    double last_end_rx_depth;

    double last_end_rx_range;

    ::std::complex<double> last_ret_value;

    /**
    * Gets the TimeArr value from ArrData TimeArr array associated to given parameters
    * @param frequency [hZ]
    * @param tx_depth transmitter depth [m]
    * @param rx_depth start receiver depth [m]
    * @param rx_range start receiver range [m]
    * @return a valid TimeArr value; a not valid TimeArr if arr_file hasn't been read yet
    **/  
    TimeArr* accessMap( double frequency, double tx_depth, double rx_depth, double rx_range ) const {
      return( arr_file.arr_values + arr_file.getTimeArrIndex( tx_depth, rx_depth, rx_range ) );
    }
    
    
    /**
    * Gets the average Pressure value in given rx range-depth box converted from ArrData TimeArr array
    * @param frequency frequency [hZ]
    * @param tx_depth transmitter depth [m]
    * @param start_rx_depth start receiver depth [m]
    * @param start_rx_range start receiver range [m]
    * @param end_rx_depth end receiver depth [m]
    * @param end_rx_range end receiver range [m]
    * @return a valid Pressure value; a not valid Pressure if arr_file hasn't been read yet
    **/  
    ::std::complex<double> readMapAvgPressure( double frequency, double tx_depth, double start_rx_depth, double start_rx_range, double end_rx_depth, double end_rx_range );


    /**
    * Process the ARR file header
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    bool getArrAscHeader();

    /**
    * Process the ARR file data
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    bool getArrAscFile();
    
    
  };

  
}


#endif /* AC_TOOLBOX_ARR_ASC_READER_DEFINITIONS_H */ 




