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
 * @file   definitions.h
 * @author Federico Guerra
 *
 * \brief  Generic functions and variables  
 *
 * Collects all generic functions and variables 
 */


#ifndef WOSS_DEFINITIONS_H
#define WOSS_DEFINITIONS_H


#include <iostream>


namespace woss {  

  typedef double Bathymetry;

  /** 
  * precision used for write-buffer spacing purposes 
  **/
  static const int WOSS_DECIMAL_PRECISION = 17;

  static const int WOSS_STREAM_TAB_SPACE = 25;



  /**
  * Pause the execution of the program. Restart it by pressing any key.
  **/
  void debugWaitForUser();

  
  /////////////////////////////
  inline void debugWaitForUser()
  {
    ::std::string response;
    ::std::cout << "Press Enter to continue";
    ::std::getline( ::std::cin, response);
  }

}

#endif /* WOSS_DEFINITIONS_H */

