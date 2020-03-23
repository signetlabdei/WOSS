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
 * @file   woss-db-creator.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::WossDbCreator class
 *
 * Provides the interface for the woss::WossDbCreator class
 */


#ifndef WOSS_DB_CREATOR_H 
#define WOSS_DB_CREATOR_H


#include <string>


namespace woss {
  
    
  class WossDb;
  
  
  /**
  * \brief Abstract class that provides the interface of database creator ( Factory object )
  *
  * WossDbCreator is the prototype of any database creator included in WOSS.
  * It has the tasks of creating and initializing a specific database object, thus abstracting the caller from specific implementation parameters, 
  * and the database itself from system specifics (c++ framework, NS-2 framework, NS-3 framework and so on...) 
  * 
  * It also provides a Tcl interpreter for NS-2 implementation.
  *
  * @see BathyGebcoDbCreator, SedimDeck41DbCreator, SspWoa2005DbCreator, ResTimeArrTxtDbCreator, ResPressureTxtDbCreator
  **/
  class WossDbCreator {

		
    public: 

      
    /**
    * Default WossDbCreator creator 
    **/ 
    WossDbCreator();
    
    virtual ~WossDbCreator();
       
    
    /**
    * Abstract method. It is called to create and initialize a WossDb. The caller will be the <b>owner</b> of created object,
    * therefore object destruction is his responsibility
    * @return a pointer to a properly initialized WossDb object
    **/
    virtual WossDb* const createWossDb() = 0;
    
                           
    WossDbCreator& setDebug( bool flag ) { debug = flag; return *this; }
    
    WossDbCreator& setWossDebug( bool flag ) { woss_db_debug = flag; return *this; }
    

    WossDbCreator& setDbPathName( const ::std::string& name ) { pathname = name; return *this; }


    bool isUsingDebug() const { return debug; }
    
    bool isUsingWossDbDebug() const { return woss_db_debug; }
    

    ::std::string getDbPathName() const { return pathname; }
    
                          
    protected: 
    

    /**
    * WossDbCreator debug flag binded in Tcl
    **/
    bool debug;
    
    /**
    * WossDb debug flag binded in Tcl. All objects instantiated will have this flag
    **/
    bool woss_db_debug;
    

    /**
    * Pathname or unique identifier
    **/
    ::std::string pathname;
    
    
    /**
    * Initializes the pointed object
    * @param woss_db pointer to a recently created WossDb
    * @return <i>true</i> if the method succeed, <i>false</i> otherwise
    **/
    virtual bool initializeDb( WossDb* const woss_db ) = 0;
    

  };
  

}


#endif /* WOSS_DB_CREATOR_H */


