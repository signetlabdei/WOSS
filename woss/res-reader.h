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


namespace woss {
  
    
  class Woss;
  class Pressure;
  class TimeArr;

  /**
  * \brief Abstract class for channel simulator result files processing
  *
  * ResReader class has the task to read and process channel simulator result files
  */
  class ResReader {

    
    public:


    /**
    * ResReader default constructor
    */
    ResReader();

    /**
    * ResReader constructor
    * @param woss const pointer to a const Woss object
    */
    ResReader( const Woss* const woss );

    virtual ~ResReader() { }

    /**
    * Initializes the ResReader object
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool initialize() = 0;

    
    /**
    * Gets the average Pressure value in given rx range-depth box
    * @param tx_depth transmitter depth [m]
    * @param start_rx_depth start receiver depth [m]
    * @param start_rx_range start receiver range [m]
    * @param end_rx_depth end receiver depth [m]
    * @param end_rx_range end receiver range [m]
    * @return a valid Pressure value 
    **/
    virtual Pressure* readAvgPressure( double tx_depth, double start_rx_depth, double start_rx_range, double end_rx_depth, double end_rx_range ) = 0;

    /**
    * Gets a Pressure value for given range, depths
    * @param tx_depth transmitter depth [m]
    * @param rx_depth receiver depth [m]
    * @param rx_range receiver range [m]
    * @return a valid Pressure value 
    **/
    virtual Pressure* readPressure( double tx_depth, double rx_depth, double rx_range ) const = 0;
    
    
    /**
    * Gets a TimeArr value for given range, depths
    * @param tx_depth transmitter depth [m]
    * @param rx_depth receiver depth [m]
    * @param rx_range receiver range [m]
    * @return a valid Pressure value 
    **/
    virtual TimeArr* readTimeArr( double tx_depth, double rx_depth, double rx_range ) const = 0;

    
    /**
    * Sets the Woss pointer
    * @param woss const pointer to a const Woss object
    * @return reference to <b>*this</b>
    **/
    ResReader& setWossPtr( const Woss* const woss ) { woss_ptr = woss; return *this; }

    /**
    * Gets the Woss pointer
    * @returns const pointer to linked Woss object
    **/
    const Woss* const getWossPtr() { return woss_ptr; }
    
    
    /**
    * Sets the file(s) pathname
    * @param name const reference to a valid pathname
    * @return reference to <b>*this</b>
    **/
    ResReader& setFileName( const ::std::string& name ) { file_name = name; return *this; }

    /**
    * Gets the file(s) pathname
    * @returns string pathname
    **/
    ::std::string getFileName() { return file_name; }
    

    protected:

      
    /**
    * Const pointer to Woss owner
    **/
    const Woss* woss_ptr;

    
    /**
    * File(s) pathname
    **/
    ::std::string file_name;

    
  };

  
}


#endif /* WOSS_RES_READER_DEFINITIONS_H */


