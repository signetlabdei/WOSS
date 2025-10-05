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
 * @file   sediment-deck41-db.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::SedimDeck41Db class
 *
 * Provides the interface for the woss::SedimDeck41Db class
 */


#ifndef WOSS_SEDIMENT_DECK41_DB_H 
#define WOSS_SEDIMENT_DECK41_DB_H


#ifdef WOSS_NETCDF_SUPPORT

#include <map>
#include "sediment-deck41-db-logic-control.h"
#include "sediment-deck41-coord-db.h"
#include "sediment-deck41-marsden-db.h"
#include "sediment-deck41-marsden-one-db.h"


namespace woss {
  

  /**
  * Map that links a DECK41 integer type to the number of times it has appeared in a database query result  
  * @see deck41-db-state-logic-constrol.h
  **/
  using FrequencyMap = std::map<int, int>;
  using FMCIter = FrequencyMap::const_iterator;
  using FMIter = FrequencyMap::iterator;
  using FMRIter = FrequencyMap::reverse_iterator;
  using FMCRIter = FrequencyMap::const_reverse_iterator;


  /**
  * Map that links a condition of Deck41TypeTests to its weight for calculating weighted average value 
  * @see Deck41TypeTests
  **/
  using SedimWeightMap = std::map< char, double >;
  using SWIter = SedimWeightMap::iterator;
  using SWRIter = SedimWeightMap::reverse_iterator;


  /**
  * \brief WossDb for NetCDF DECK41 Sediment database
  *
  * SedimDeck41Db implements WossDb and WossSedimentDb for NetCDF DECK41 Sediment database. It provides logic to handle the three custom made 
  * databases: coordinates, marsden square and marsden coordinates. 
  **/
  class SedimDeck41Db : public WossDb, public WossSedimentDb {

    friend class SedimDeck41DbCreator;

    public:


  /**
    * SedimDeck41Db constructor
    * @param name pathname of database
    **/
    SedimDeck41Db( const std::string& name );

    virtual ~SedimDeck41Db() override = default;

  /**
    * Not allowed
    **/
    virtual bool openConnection() override { return false; }

  /**
    * Not allowed
    **/
    virtual bool finalizeConnection() override { return false; }


    /**
    * Closes the connection to the three databases provided
    * @return <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool closeConnection() override;


    /**
    * Inserts the given woss::Sediment value in the database for given coordinates
    * @param coordinates const reference to a valid Coord object
    * @param bathymetry_value const Reference to Sediment value to be inserted
    * @returns <i>true</i> if method was successful, <i>false</i> otherwise
    **/
    virtual bool insertValue( const Coord& coordinates, const Sediment& sediment_value ) override;
    
    
    /**
    * Returns a pointer to a heap-based Sediment for given coordinates and depth, 
    * if present in any of the three databases.
    * <b>User is responsible of pointer's ownership</b>
    * @param coords const reference to a valid CoordZ object
    * @return a std::unique_ptr to a <i>valid</i> Sediment if coordinates are found, <i>not valid</i> otherwise
    **/
    virtual std::unique_ptr<Sediment> getValue( const CoordZ& coordz ) const override;

    /**
    * Returns a pointer to a heap-based Sediment for given coordinates and depth vector, 
    * if at least one set of coordinates is present in any of the three databases.
    * <b>User is responsible of pointer's ownership</b>
    * @param coordz_vector const reference to a valid CoordZ vector
    * @return std::unique_ptr to a <i>valid</i> Sediment if at least one set of coordinates is found, <i>not valid</i> otherwise
    **/
    virtual std::unique_ptr<Sediment> getValue( const CoordZVector& coordz_vector ) const override;


    protected:


    /**
    * WossDb handling custom made NetCDF DECK41 database 
    **/
    SedimDeck41CoordDb sediment_coord_db;

    /**
    * WossDb handling custom made NetCDF DECK41 marsden square database 
    **/
    SedimDeck41MarsdenDb sediment_marsden_db;

    /**
    * WossDb handling custom made NetCDF DECK41 marsden coordinates database 
    **/
    SedimDeck41MarsdenOneDb sediment_marsden_one_db;

    /**
    * Current iteration Deck41TypeTests
    **/
    mutable Deck41TypeTests curr_tests_state;

    /**
    * Previous iteration Deck41TypeTests
    **/
    mutable Deck41TypeTests prev_tests_state;

    /**
    * Weight map for DECK41 floor type to Sediment conversion 
    **/
    static SedimWeightMap sediment_weight_map;


    /**
    * Initializes the static sediment_weight_map
    * @returns a SedimWeightMap
    **/
    static SedimWeightMap initSedimWeightMap();
    
    
    /**
    * Computes the average depth of a valid CoordZVector for Sediment creation
    * @param coordz_vector a vector of <i>valid</i> CoordZ
    * @returns the average depth of the vector [m]
    **/
    double calculateAvgDepth( const CoordZVector& coordz_vector ) const;  


    /**
    * Gets the Deck41Types of a valid CoordZVector from the SedimDeck41CoordDb database
    * @param coordz_vector a vector of <i>valid</i> CoordZ
    * @returns the corresponding Deck41Types (main sediment type, second sediment type) of the vector
    **/
    Deck41Types getDeck41TypesFromCoords( const CoordZVector& coordz_vector ) const;

    /**
    * Gets the Deck41Types of a valid CoordZVector from the SedimDeck41MarsdenDb database
    * @param coordz_vector a vector of <i>valid</i> CoordZ
    * @returns the corresponding Deck41Types (main sediment type, second sediment type) of the vector
    **/
    Deck41Types getDeck41TypesFromMarsdenCoords( const CoordZVector& coordz_vector ) const ;

    /**
    * Gets the Deck41Types of a valid CoordZVector from the SedimDeck41MarsdenOneDb database
    * @param coordz_vector a vector of <i>valid</i> CoordZ
    * @returns the corresponding Deck41Types (main sediment type, second sediment type) of the vector
    **/
    Deck41Types getDeck41TypesFromMarsdenSquare( const CoordZVector& coordz_vector ) const ;


    /**
    * Gets the Deck41Types of a valid CoordZVector searching in order in: coord db, marsden db, marsden one db.
    * On each search Deck41TypeTests curr_tests_state and prev_tests_state are updated, and based on result conditions, 
    * the process stops and returns or goes on to the next database.
    * @param coordz_vector a vector of <i>valid</i> CoordZ
    * @returns the corresponding Deck41Types (main sediment type, second sediment type) of the vector
    **/
    Deck41Types calculateDeck41Types( const CoordZVector& coordz_vector ) const;


    /**
    * Creates the corresponding Sediment from the searched Deck41Types returned by calculateDeck41Types
    * @param floor_types the Deck41Types pair resulted from the search process
    * @param avg_depth depth value [m] for Sediment calculations
    * @returns std::unique_ptr to a Sediment value
    **/
    std::unique_ptr<Sediment> calculateSediment( const Deck41Types& floor_types, double avg_depth ) const;

    /**
    * Creates a Sediment from the DECK41 integer type number
    * @param deck41_type DECK41 integer floor type number
    * @param depth depth value [m] for Sediment calculations
    * @returns std::unique_ptr to a Sediment value
    * @see Deck41TypeTests
    **/
    std::unique_ptr<Sediment> createSediment( int deck41_type, double depth ) const;

    
    /**
    * Returns the type of the FrequencyMap that has the max number of times of appeareance in a database query result  
    * @param frequency_map const FrequencyMap reference
    * @returns DECK41 integer type
    **/
    int getMaxAppereanceFrequencyValue( const FrequencyMap& frequency_map ) const;


    /**
    * Does Test A on given Deck41TypeTests reference. A positive result means that the search in the databases for a 
    * valid Deck41Types is over
    * @param test const Deck41TypeTests reference
    * @returns <i>true</i> if test succeed, <i>false</i> otherwise
    **/
    bool doTestA( const Deck41TypeTests& test ) const;

    /**
    * Does Test B on given Deck41TypeTests reference. A positive result means that the search in the databases for a 
    * valid Deck41Types should continue. If next search step fails a Sediment not valid will be returned from the search process
    * @param test const Deck41TypeTests reference
    * @returns <i>true</i> if test succeed, <i>false</i> otherwise
    **/
    bool doTestB( const Deck41TypeTests& test ) const;
  
    /**
    * Does Test C on given Deck41TypeTests reference. A positive result means that the search in the databases for a 
    * valid Deck41Types is over if the next search step Test A fails
    * @param test const Deck41TypeTests reference
    * @returns <i>true</i> if test succeed, <i>false</i> otherwise
    **/
    bool doTestC( const Deck41TypeTests& test ) const;

  };


  // inline functions
  ///////////

  inline bool SedimDeck41Db::doTestA(const Deck41TypeTests& floor) const {
    return  ((floor.getConditionA() == true) || (floor.getConditionB() == true) || (floor.getConditionE() == true) || 
              (floor.getConditionF() == true));
  }


  inline bool SedimDeck41Db::doTestB(const Deck41TypeTests& floor) const {
    return  ((floor.getConditionC() == true) || (floor.getConditionD() == true) || (floor.getConditionG() == true) );
  }


  inline bool SedimDeck41Db::doTestC(const Deck41TypeTests& floor) const {
    return ((floor.getConditionC() == true) || (floor.getConditionD() == true));
  }

}

#endif // WOSS_NETCDF_SUPPORT

#endif /* WOSS_SEDIMENT_DECK41_DB_H */


 
