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
 * @file   time-definitions-tcl.cpp
 * @author Federico Guerra
 *
 * \brief  Implementation of woss::TimeTcl class 
 *
 * Implementation of woss::TimeTcl class 
 */


#ifdef WOSS_NS_MIRACLE_SUPPORT


#include "time-definitions-tcl.h"
#include <cstdlib>

using namespace woss;


static class TimeClass : public TclClass {
public:
  TimeClass() : TclClass("WOSS/Definitions/Time") {}
  TclObject* create(int, const char*const*) {
    return( new TimeTcl() );
  }
} class_Time;


int TimeTcl::command(int argc, const char*const* argv) 
{
  if ( argc == 8 ) {
    if(strcasecmp(argv[1], "setTime") == 0) { 
       
      setYear( atoi(argv[4]) );  
      setMonth( atoi(argv[3]) );
      setDay( atoi(argv[2]) );
      setHours ( atoi(argv[5]) );
      setMinutes ( atoi(argv[6]) );
      setSeconds ( atoi(argv[7]) );
      
      return TCL_OK; 
    }    
  }
  else if ( argc == 3 ) {
    if(strcasecmp(argv[1], "setYear") == 0) { 
       
      setYear( atoi(argv[2]) );  
     
      return TCL_OK; 
    }
    else if(strcasecmp(argv[1], "setMonth") == 0) { 
       
      setMonth( atoi(argv[2]) );  
     
      return TCL_OK; 
    }
    else if(strcasecmp(argv[1], "setDay") == 0) { 
       
      setDay( atoi(argv[2]) );  
     
      return TCL_OK; 
    }
    else if(strcasecmp(argv[1], "setHours") == 0) { 
       
      setHours( atoi(argv[2]) );  
     
      return TCL_OK; 
    }    
    else if(strcasecmp(argv[1], "setMinutes") == 0) { 
       
      setMinutes( atoi(argv[2]) );  
     
      return TCL_OK; 
    }
    else if(strcasecmp(argv[1], "setSeconds") == 0) { 
       
      setSeconds( atoi(argv[2]) );  
     
      return TCL_OK; 
    }    
  } 
  return TclObject::command(argc, argv);
}

#endif // WOSS_NS_MIRACLE_SUPPORT
