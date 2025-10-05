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
 * @file   res-reader.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::ResReader class
 *
 * Provides the interface for the woss::ResReader class
 */


#ifndef WOSS_RES_READER_DEFINITIONS_H
#define WOSS_RES_READER_DEFINITIONS_H


#include <string>
#include <pressure-definitions.h>
#include <time-arrival-definitions.h>
#include <memory>


namespace woss {

  class Woss;

  /**
  * \brief Abstract class for channel simulator result files processing
  *
  * ResReader class has the task to read and process channel simulator result files
  */
  class ResReader {

    public:

   /**
    * ResReader constructor
    * @param woss const pointer to a const Woss object
    */
    ResReader( const Woss& woss );

    virtual ~ResReader() = default;

    /**
    * Initializes the ResReader object
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool initialize() = 0;

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
    virtual std::unique_ptr<Pressure> readAvgPressure( double frequency, double tx_depth, double start_rx_depth, double start_rx_range, double end_rx_depth, double end_rx_range ) = 0;

    /**
    * Gets a Pressure value of given range, depths
    * @param frequency frequency [hZ]
    * @param tx_depth transmitter depth [m]
    * @param rx_depth receiver depth [m]
    * @param rx_range receiver range [m]
    * @return a std::unique_ptr to a valid Pressure value; a not valid Pressure if shd_file hasn't been read yet
    **/
    virtual std::unique_ptr<Pressure> readPressure( double frequency, double tx_depth, double rx_depth, double rx_range ) const = 0;

    /**
    * Gets a TimeArr value for given range, depths
    * @param frequency frequency [hZ]
    * @param tx_depth transmitter depth [m]
    * @param rx_depth receiver depth [m]
    * @param rx_range receiver range [m]
    * @return a std::unique_ptr to a valid TimeArr value; a not valid TimeArr if arr_file hasn't been read yet
    **/
    virtual std::unique_ptr<TimeArr> readTimeArr(double frequency, double tx_depth, double rx_depth, double rx_range ) const = 0;

    /**
    * Gets the Woss pointer
    * @returns const pointer to linked Woss object
    **/
    const Woss& getWossRef() const { return woss_ref; }

    /**
    * Sets the file(s) pathname
    * @param name const reference to a valid pathname
    * @return reference to <b>*this</b>
    **/
    ResReader& setFileName( const std::string& name ) { file_name = name; return *this; }

    /**
    * Gets the file(s) pathname
    * @returns string pathname
    **/
    std::string getFileName() const { return file_name; }

    protected:

    /**
    * Const pointer to Woss owner
    **/
    const Woss& woss_ref;

    /**
    * File(s) pathname
    **/
    std::string file_name;

  };

}

#endif /* WOSS_RES_READER_DEFINITIONS_H */
