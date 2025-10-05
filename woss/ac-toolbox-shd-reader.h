/* WOSS - World Ocean Simulation System -
 * 
 * Copyright (C) 2009 2025 Federico Guerra
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
#include <memory>
#include <pressure-definitions.h>
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

    ShdData() = default;

    ~ShdData() = default;
    
    /**
    * Record byte length of a binary SHD file. See Bellhop code for more info
    */
    int32_t record_length = 0;

    /**
    * Plot typename. See Bellhop code for more info
    */
    std::unique_ptr<char[]> plot_type = nullptr;

   /**
    * Frequency value [Hz]
    */
    float frequency = 0.0;

    /**
    * Total number of theta values. See Bellhop code for more info
    */
    int32_t Ntheta = 0;

    /**
    * Pointer to an array of theta values
    */
    std::unique_ptr<float[]> theta = nullptr;

    /**
    * Total number of transmitter depths
    */
    int32_t Nsd;

    /**
    * Pointer to an array of transmitter depths [m]
    */
    std::unique_ptr<float[]> tx_depths = nullptr;

    /**
    * Total number of receiver depths
    */
    int32_t Nrd = 0;

    /**
    * Pointer to an array of receiver depths [m]
    */
    std::unique_ptr<float[]> rx_depths = nullptr;

    /**
    * Total number of receiver ranges
    */
    int32_t Nrr = 0;

    /**
    * Pointer to an array of receiver ranges [m]
    */
    std::unique_ptr<float[]> rx_ranges = nullptr;

    /**
    * Total number of receiver per range
    */
    int32_t Nrx_per_range = 0;

    /**
    * Pointer to an array of complex\<double\> values [m]
    */
    std::unique_ptr< std::complex<double>[] > press_values = nullptr;

    /**
    * Initializes the struct 
    */
    void initialize() { plot_type.reset(); Ntheta = 0; theta.reset(); Nrx_per_range = 0; record_length = 0; tx_depths.reset();
                        rx_depths.reset(); rx_ranges.reset(); Nrr = 0; Nrd = 0; Nsd = 0; press_values.reset(); frequency = 0.0; }

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
    int getIndex( float value, float array[], int32_t array_size ) const;

  };

  /**
  * \brief class for storing data of any acoustic toolbox SHD file
  *
  * class ShdData stores Pressure provided by any acoustic toolbox SHD file
  */
  class ShdData_v1 {

    public:

    ShdData_v1() = default;

    ~ShdData_v1() = default;

    /**
    * Record byte length of a binary SHD file. See Bellhop code for more info
    */
    int32_t record_length = 0;

    /**
    * Plot typename. See Bellhop code for more info
    */
    std::unique_ptr< char[] > plot_type = nullptr;

    /**
    * Frequency value [Hz]
    */
    double frequency = 0.0;

    int32_t Nfreq = 0;

    std::unique_ptr< double[] > frequencies = nullptr;

    /**
    * Total number of theta values. See Bellhop code for more info
    */
    int32_t Ntheta = 0;

    /**
    * Pointer to an array of theta values
    */
    std::unique_ptr< double[] > theta = nullptr;

    /**
    * Total number of transmitter depths
    */
    int32_t Nsd = 0;

    /**
    * Pointer to an array of transmitter depths [m]
    */
    std::unique_ptr< float[] > tx_depths = nullptr;

    /**
    * Total number of receiver depths
    */
    int32_t Nrd = 0;

    /**
    * Pointer to an array of receiver depths [m]
    */
    std::unique_ptr< float[] > rx_depths = nullptr;

    /**
    * Total number of receiver ranges
    */
    int32_t Nrr = 0;

    /**
    * Pointer to an array of receiver ranges [m]
    */
    std::unique_ptr<double[]> rx_ranges = nullptr;

    /**
    * Total number of receiver per range
    */
    int32_t Nrx_per_range = 0;


    double stabil_atten = 0.0;

    /**
    * Pointer to an array of complex\<double\> values [m]
    */
    std::unique_ptr< std::complex<double>[] > press_values = nullptr;

    /**
    * Initializes the struct 
    */
    void initialize() { plot_type.reset(); frequency = 0.0; Nfreq = 0; frequencies.reset(); Ntheta = 0; 
                        theta.reset(); Nrx_per_range = 0; record_length = 0; tx_depths.reset(); stabil_atten = 0.0;
                        rx_depths.reset(); rx_ranges.reset(); Nrr = 0; Nrd = 0; Nsd = 0; press_values.reset(); }

    /**
    * Returns the press_values index associated to given parameters
    * @param tx_freq transmitter frequency [hZ]
    * @param tx_depth transmitter depth [m]
    * @param rx_depth receiver depth [m]
    * @param rx_range receiver range [m]
    * @param theta theta value
    * @returns valid press_values index value
    */
    int getPressureIndex( double tx_freq, double tx_depth, double rx_depth, double rx_range, double theta = 0.0 ) const;

    /**
    * Returns the index of given array associated to given value
    * @param value test value
    * @param array valid pointer to an array 
    * @param array_size size of passed array
    * @returns valid array index value
    */
    int getIndex( float value, float array[], int32_t array_size ) const;

    /**
    * Returns the index of given array associated to given value
    * @param value test value
    * @param array valid pointer to an array 
    * @param array_size size of passed array
    * @returns valid array index value
    */
    int getIndex( double value, double array[], int32_t array_size ) const;
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
    * ShdResReader constructor
    * @param woss const pointer to a const Woss object
    */
    ShdResReader( const Woss& woss );

    virtual ~ShdResReader() override = default;

    /**
    * Initializes the ShdResReader object, reads SHD file, and stores read Pressure values
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool initialize() override;

    /**
    * Gets the average Pressure value in given rx range-depth box
    * @param frequency frequency [hZ]
    * @param tx_depth transmitter depth [m]
    * @param start_rx_depth start receiver depth [m]
    * @param start_rx_range start receiver range [m]
    * @param end_rx_depth end receiver depth [m]
    * @param end_rx_range end receiver range [m]
    * @return a std::unique_ptr to a valid Pressure value; a not valid Pressure if shd_file hasn't been read yet
    **/
    virtual std::unique_ptr<Pressure> readAvgPressure( double frequency, double tx_depth, double start_rx_depth, double start_rx_range, double end_rx_depth, double end_rx_range ) override;

    /**
    * Gets a Pressure value of given range, depths
    * @param frequency frequency [hZ]
    * @param tx_depth transmitter depth [m]
    * @param rx_depth receiver depth [m]
    * @param rx_range receiver range [m]
    * @return a std::unique_ptr to a valid Pressure value; a not valid Pressure if shd_file hasn't been read yet
    **/
    virtual std::unique_ptr<Pressure> readPressure( double frequency, double tx_depth, double rx_depth, double rx_range ) const override;

    
    /**
    * SHD files don't hold any time arrivals information. A special TimeArr is constructed from Pressure associated
    * to given paramaters.
    * @param frequency frequency [hZ]
    * @param tx_depth transmitter depth [m]
    * @param rx_depth receiver depth [m]
    * @param rx_range receiver range [m]
    * @return a std::unique_ptr to a TimeArr that holds a single Pressure value and no delay information
    **/
    virtual std::unique_ptr<TimeArr> readTimeArr( double frequency, double tx_depth, double rx_depth, double rx_range ) const override; 

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
    std::ifstream file_reader;

    /**
    * Struct that holds Pressure data read from SHD file
    */
    ShdData shd_file;

    /**
    * Struct that holds Pressure data read from SHD file with V1 syntax
    */
    ShdData_v1 shd_file_v1;

    double last_tx_depth;

    double last_start_rx_depth;

    double last_start_rx_range;

    double last_end_rx_depth;

    double last_end_rx_range;

    std::complex<double> last_ret_value;

    /**
    * Gets the average Pressure value in given rx range-depth box from ShdData Pressure array
    * @param frequency frequency [hZ]
    * @param tx_depth transmitter depth [m]
    * @param start_rx_depth start receiver depth [m]
    * @param start_rx_range start receiver range [m]
    * @param end_rx_depth end receiver depth [m]
    * @param end_rx_range end receiver range [m]
    * @param theta theta value
    * @return a valid Pressure value; a not valid Pressure if shd_file hasn't been read yet
    **/
    std::complex<double> readMapAvgPressure(double frequency, double tx_depth, double start_rx_depth, double start_rx_range, double end_rx_depth, double end_rx_range, double theta = 0.0 );

    /**
    * Gets the Pressure value from ShdData Pressure array associated to given parameters
    * @param frequency frequency [hZ]
    * @param tx_depth transmitter depth [m]
    * @param rx_depth start receiver depth [m]
    * @param rx_range start receiver range [m]
    * @param theta theta value
    * @return a valid Pressure value; a not valid Pressure if shd_file hasn't been read yet
    **/  
    std::complex<double> accessMap(double frequency, double tx_depth, double rx_depth, double rx_range, double theta = 0.0 ) const;

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

}

#endif /* AC_TOOLBOX_RES_READER_DEFINITIONS_H */

