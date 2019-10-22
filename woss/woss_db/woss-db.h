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
 * @file   woss-db.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossDb class
 *
 * Provides the interface for the woss::WossDb class
 */


#ifndef WOSS_DB_H 
#define WOSS_DB_H

#include <fstream>

#if defined(WOSS_NETCDF4_SUPPORT)
#include <ncFile.h>
#elif defined(WOSS_NETCDF_SUPPORT)
#include <netcdfcpp.h>
#endif

#include <definitions.h>
#include <coordinates-definitions.h>


namespace woss {
    

  class Sediment;
  class Pressure;
  class SSP;
  class TimeArr;
  class Time;
  
  
  /**
  * Optional default pathname of a WossDb. The object created is not valid
  **/
  #define DB_NAME_NOT_SET "DB_NAME_NOT_SET"


  /**
  * A pair of strings containing:
  * @param first pathname
  * @param second filename
  **/
  typedef ::std::pair< ::std::string, ::std::string > PathName; // path , filename


  /**
  * \brief Abstract class that provides the interface of databases
  *
  * WossDb is the foundation of any database included in WOSS.
  * It has the tasks of opening, setting up and closing a connection do a database. Technology dependant issues should be 
  * putted in a class that inherits from WossDb.No specific data behaviour is defined here, this will be provided 
  * in other abstract classes: this has been specifically done to divide db technology from data. 
  * A custom database would have to inherit from a technology dependant class and from a data behaviour class
  * @see WossBathymetryDb, WossSedimentDb, WossSSPDb, WossResArrDb, WossResPressDb
  **/
  class WossDb {

    
    public:

      
    /**
    * WossDb constructor
    * @param name pathname of database
    **/
    WossDb( const ::std::string& name ); 

    virtual ~WossDb() { }


    /**
    * Sets the pathname (or custom network address) of database file
    * @param name pathname database pathname
    **/
    void setDbName( const ::std::string& pathname ) { db_name = pathname; }

    /**
    * Gets the pathname (or custom network address) of database file
    * @returns pathname database pathname
    **/
    ::std::string getDbName() const { return db_name; }


    /**
    * Sets debug flag
    * @param flag debug flag
    **/
    void setDebug( double flag = true ) { debug = flag; }
    
    /**
    * Gets debug flag
    * @return <i>true</i> if using debug, <i>false</i> otherwise
    **/
    bool isUsingDebug() const { return debug; }


    /**
    * Checks the validity of WossDb
    * @return <i>true</i> if pathname is valid, <i>false</i> otherwise
    **/
    virtual bool isValid() { return( db_name.length() > 0 ); }


    /**
    * Opens the connection to the pathname provided
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool openConnection() = 0;

    
    /**
    * Post openConnection() actions. E.g. creating variables, initializing special pointers etc...
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool finalizeConnection() = 0;
    

    /**
    * Closes the connection to the open database
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool closeConnection() = 0;
    
      
    protected:

      
    /**
    * Pathname to database. It could be a filesystem pathname, or a network identifier etc...
    **/ 
    ::std::string db_name;


    /**
    * Debug flag
    **/ 
    bool debug;

    
    /**
    * Gets path and database name separated
    * @return PathName value
    * @see PathName
    **/
    PathName getPathName( const ::std::string& complete_path );

    
  };

  ///////////////////////////////////////////////////
#ifdef WOSS_NETCDF_SUPPORT
  /**
  * \brief NetCDF implementation of WossDb
  *
  * WossNetcdfDb is the NetCDF specialization of WossDb class. It sets up connection to the file and properly initializes 
  * a NcFile* pointer. NetCDF variables however are not superimposed by this class. User has the task to correctly create
  * and initialize them with the method finalizeConnection()
  * @see BathyGebcoDb, SedimDeck41CoordDb, SedimDeck41MarsdenDb, SedimDeck41MarsdenOneDb, SspWoa2005Db
  **/
  class WossNetcdfDb : public WossDb {

    
    public:
      
      
    /**
    * WossNetcdfDb constructor
    * @param name pathname of database
    **/
    WossNetcdfDb( const ::std::string& name );

    virtual ~WossNetcdfDb();

    
    /**
    * Opens the connection to the NetCDF pathname provided
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool openConnection();

    
    /**
    * Closes the connection to the NetCDF pathname provided
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool closeConnection();
    
    
    protected:
      

    /**
    * NcFile pointer to a NetCDF database descriptor. It will be properly initialized by openConnection()
    **/
#if defined(WOSS_NETCDF4_SUPPORT)
    netCDF::NcFile* netcdf_db;
#else
    NcFile* netcdf_db;
#endif // defined(WOSS_NETCDF4_SUPPORT)

  };
#endif // WOSS_NETCDF_SUPPORT
  

  /**
  * \brief Textual implementation of WossDb
  *
  * WossNetcdfDb is the textual specialization of WossDb class. It sets up connection to the file and properly initializes 
  * a fstream object. No behaviour is superimposed by this class. User has the task to define method finalizeConnection()
  * @see ResTimeArrTxtDb, ResPressureTxtDb
  **/
  class WossTextualDb : public WossDb {

    
    public:
      
    /**
    * WossTextualDb constructor
    * @param name pathname of database
    **/
    WossTextualDb( const ::std::string& name );

    virtual ~WossTextualDb();

    
    /**
    * Opens the connection to the textual file provided
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool openConnection();


    /**
    * Closes the connection to the textual file provided
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool closeConnection();

    
    protected:


    /**
    * fstream object to a textual database. It will be properly initialized by openConnection()
    **/
    ::std::fstream textual_db;
    
    
  };

  /////// DATA TYPE INTERFACE /////////////////////////////

  /**
  * \brief Data behaviour class for bathymetry database
  *
  * WossBathymetryDb is the prototype of any bathymetry dabase in WOSS
  * @see BathyGebcoDb
  **/
  class WossBathymetryDb {

    
    public:

      
    WossBathymetryDb() { }

    virtual ~WossBathymetryDb() { }


    /**
    * Inserts the given woss::Bathymetry value in the database for given coordinates
    * @param coordinates const reference to a valid woss::Coord object
    * @param bathymetry_value const reference to woss::Bathymetry value to be inserted
    * @returns <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool insertValue( const Coord& coordinates, const Bathymetry& bathymetry_value ) = 0;

    /**
    * Returns the positive depth value ( woss::Bathymetry ) for given coordinates, if present in the database
    * @param coords const reference to a valid Coord object
    * @return <i>positive</i> depth value [m] if coordinates are found, <i>HUGE_VAL</i> otherwise
    **/
    virtual Bathymetry getValue( const Coord& coords ) const = 0;

    
  };


  /**
  * \brief Data behaviour class for Sediment database
  *
  * WossSedimentDb is the prototype of any Sediment dabase in WOSS
  * @see SedimDeck41Db
  **/
  class WossSedimentDb {

    
    public:
      

    WossSedimentDb() { }

    virtual ~WossSedimentDb() { }


    /**
    * Inserts the given woss::Sediment value in the database for given coordinates
    * @param coordinates const reference to a valid woss::Coord object
    * @param bathymetry_value const reference to woss::Sediment value to be inserted
    * @returns <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool insertValue( const Coord& coordinates, const Sediment& sediment_value ) = 0;
    
    
    /**
    * Returns a pointer to a heap-created Sediment value for given coordinates and depth, if present in the database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coords const reference to a valid CoordZ object
    * @return <i>valid</i> Sediment if coordinates are found, <i>not valid</i> otherwise
    **/
    virtual Sediment* getValue( const CoordZ& coords ) const = 0; 

    /**
    * Returns a pointer to a heap-created Sediment value for given coordinates and depth vector, if at least one set of coordinates 
    * is present in the database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coordz_vector const reference to a valid CoordZ vector
    * @return <i>valid</i> Sediment if at least one set of coordinates is found, <i>not valid</i> otherwise
    **/
    virtual Sediment* getValue( const CoordZVector& coordz_vector ) const = 0; 
    
    
  };


  /**
  * \brief Data behaviour class for SSP database
  *
  * WossSSPDb is the prototype of any SSP dabase in WOSS
  * @see SspWoa2005Db
  **/
  class WossSSPDb {

    
    public:

      
    WossSSPDb() { }

    virtual ~WossSSPDb() { }


    /**
    * Inserts the given woss::SSP value in the database for given coordinates
    * @param coordinates const reference to a valid woss::Coord object
    * @param time_value const reference to a valid woss::Time object
    * @param ssp_value const reference to woss::SSP value to be inserted
    * @returns <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool insertValue( const Coord& coordinates, const Time& time_value, const SSP& ssp_value ) = 0;
    
    
    /**
    * Returns a pointer to a heap-created SSP object for given coordinates and date time if both present in the database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coords const reference to a valid Coord object
    * @param time const reference to a valid Time object
    * @param ssp_depth_precision ssp depth precision [m]
    * @return <i>valid</i> SSP if coordinates and time date are found, <i>not valid</i> otherwise
    **/  
    virtual SSP* getValue( const Coord& coords, const Time& time, long double ssp_depth_precision ) const = 0;

    
  };


  /**
  * \brief Data behaviour class for storing calculated TimeArr
  *
  * WossResTimeArrDb is the prototype of any class that implements a database database for calculated TimeArr
  * @see ResTimeArrTxtDb
  **/
  class WossResTimeArrDb {

    
    public:

      
    WossResTimeArrDb() { }

    virtual ~WossResTimeArrDb() { }

    /**
    * Returns a heap-created TimeArr value for given frequency, transmitter and receiver 
    * coordinates if present in the database. 
    * <b>User is responsible of pointer's ownership</b>
    * @param coord_tx const reference to a valid CoordZ object
    * @param coord_rx const reference to a valid CoordZ object
    * @param frequency used frequency [hz]
    * @return <i>valid</i> TimeArr if parameters are found, <i>not valid</i> otherwise
    **/  
    virtual TimeArr* getValue( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value ) const = 0;

    
    /**
    * Inserts the given TimeArr value in the database at given frequency, transmitter and receiver coordinates
    * @param coord_tx const reference to a valid CoordZ object
    * @param coord_rx const reference to a valid CoordZ object
    * @param frequency used frequency [hz]
    * @param channel computed TimeArr
    * @returns <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool insertValue( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value, const TimeArr& channel ) = 0;

    
  };


  /**
  * \brief Data behaviour class for storing calculated Pressure
  *
  * WossResPressDb is the prototype of any class that implements a database database for calculated Pressure
  * @see ResPressureTxtDb
  **/
  class WossResPressDb {

    
    public:
      

    WossResPressDb() {  }

    virtual ~WossResPressDb() { }


    /**
    * Returns a heap-created Pressure value for given frequency, transmitter and receiver coordinates if present in the database.
    * <b>User is responsible of pointer's ownership</b>
    * @param coord_tx const reference to a valid CoordZ object
    * @param coord_rx const reference to a valid CoordZ object
    * @param frequency used frequency [hz]
    * @return <i>valid</i> TimeArr if parameters are found, <i>not valid</i> otherwise
    **/  
    virtual Pressure* getValue( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value ) const = 0;


    /**
    * Inserts the given Pressure value in the database at given frequency, transmitter and receiver coordinates
    * @param coord_tx const reference to a valid CoordZ object
    * @param coord_rx const reference to a valid CoordZ object
    * @param frequency used frequency [hz]
    * @param pressure computed Pressure
    * @returns <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool insertValue( const CoordZ& coord_tx, const CoordZ& coord_rx, const double frequency, const Time& time_value,  const Pressure& pressure ) = 0;


  };

}


#endif /* WOSS_DB_H */


