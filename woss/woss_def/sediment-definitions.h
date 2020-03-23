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
 * @file   sediment-definitions.h
 * @author Federico Guerra
 *
 * \brief  Definitions and library for woss::Sediment class 
 *
 * Definitions and library for surficial sediment
 */


#ifndef WOSS_SEDIMENT_DEFINITIONS_H
#define WOSS_SEDIMENT_DEFINITIONS_H


#include <string>
#include "coordinates-definitions.h"


namespace woss {
  
    
  #define SEDIMENT_NOT_SET_VALUE (-10000.0)


  typedef ::std::pair < int , int > Deck41Types;


  /**
  * \brief Surficial sediment geoacoustic parameters definitions
  *
  * The Sediment class provide an interface for creating and manipulating surficialg eoacoustic parameters.
  * 
  **/
  class Sediment {

    
    public:

    /**
    * Sediment default constructor. The object created is not valid
    **/  
    Sediment();
    
    /**
    * Sediment constructor
    * @param name textual name
    * @param velc compressional wave velocity [m/s]
    * @param vels shear wave velocity [m/s]
    * @param dens sediment density [g/cm^3 or user defined]
    * @param attc compressional wave attenuation [db/wavelength or user defined]
    * @param atts shear wave attenuation [db/wavelength or user defined]
    * @param bottom_depth bottom depth [m]
    **/
    Sediment( const ::std::string& name, double velc, double vels, double dens, double attc, double atts, double bottom_depth = 1.0 );

    /**
    * Sediment copy constructor
    * @param copy Sediment to be copied
    **/
    Sediment( const Sediment& copy );
    
    
    /**
    * Sediment virtual factory method
    * @return a heap-created Sediment object
    **/  
    virtual Sediment* create() const { return new Sediment(); }
    
    /**
    * Sediment virtual factory method
    * @param name textual name
    * @param velc compressional wave velocity [m/s]
    * @param vels shear wave velocity [m/s]
    * @param dens sediment density [g/cm^3 or user defined]
    * @param attc compressional wave attenuation [db/wavelength or user defined]
    * @param atts shear wave attenuation [db/wavelength or user defined]
    * @param bottom_depth bottom depth [m]
    * @return a heap-created Sediment object
    **/
    virtual Sediment* create( const ::std::string& name, double velc, double vels, double dens, double attc, double atts, double bottom_depth = 1.0 ) const {
      return new Sediment( name, velc, vels, dens, attc, atts, bottom_depth); }

    /**
    * Sediment virtual factory method
    * @param copy Sediment to be copied
    * @return a heap-created Sediment object
    **/
    virtual Sediment* create( const Sediment& copy ) const { return new Sediment(copy); }
    
    /**
    * Sediment virtual factory method
    * @return a heap-created copy of <b>this</b> instance
    **/
    virtual Sediment* clone() const  { return new Sediment(*this); }
    
    
    virtual ~Sediment() { };

    
    /**
    * Sets textual type name
    * @param name string name
    * @return reference to <b>*this</b>
    **/
    Sediment& setType( ::std::string name ) { type = name; return *this; }

    /**
    * Sets compressional wave velocity
    * @param vel velocity [m/s]
    * @return reference to <b>*this</b>
    **/
    Sediment& setVelocityC( double vel  ) { vel_c = vel; return *this; }

    /**
    * Sets shear wave velocity
    * @param vel velocity [m/s]
    * @return reference to <b>*this</b>
    **/
    Sediment& setVelocityS( double vel ) { vel_s = vel; return *this; }

    /**
    * Sets sediment density
    * @param dens density [g/cm^3 or user defined]
    * @return reference to <b>*this</b>
    **/
    Sediment& setDensity( double dens ) { density = dens; return *this; }

    /**
    * Sets compressional wave attenuation
    * @param att attenuation [db/wavelength or user defined]
    * @return reference to <b>*this</b>
    **/
    Sediment& setAttenuationC( double att ) { att_c = att; return *this; }

    /**
    * Sets shear wave attenuation
    * @param att attenuation [db/wavelength or user defined]
    * @return reference to <b>*this</b>
    **/
    Sediment& setAttenuationS( double att ) { att_s = att; return *this; }

    /**
    * Sets bottom depth
    * @param bottom_depth positive depth [m/s]
    * @return reference to <b>*this</b>
    **/
    Sediment& setDepth( double bottom_depth ) { depth = bottom_depth; return *this; }

    /**
    * Sets all parameters at one.
    * @param name textual name
    * @param velc compressional wave velocity [m/s]
    * @param vels shear wave velocity [m/s]
    * @param dens sediment density [g/cm^3 or user defined]
    * @param attc compressional wave attenuation [db/wavelength or user defined]
    * @param atts shear wave attenuation [db/wavelength or user defined]
    * @param bottom_depth bottom depth ( > 0 ) [m] 
    * @return reference to <b>*this</b>
    **/
    Sediment& set( ::std::string name, double velc, double vels, double dens, double attc , double atts, double bottom_depth ) { 
                        type = name; vel_c = velc; vel_s = vels, density = dens;
                        att_c = attc; att_s = atts; depth = bottom_depth; return *this; }


    /**
    * Sets debug for the whole class
    * @param flag debug value
    **/
    static void setDebug( bool flag ) { debug = flag; } 
    
    
    /**
    * Gets compressional wave velocity
    * @return velocity [m/s]
    **/
    double getVelocityC() const { return vel_c; }

    /**
    * Gets shear wave velocity
    * @return velocity [m/s]
    **/
    double getVelocityS() const { return vel_s; }

    /**
    * Gets sediment density
    * @return density [g/cm^3 or user defined]
    **/
    double getDensity() const { return density; }

    /**
    * Gets compressional wave attenuation
    * @return attenuation [db/wavelength or user defined]
    **/
    double getAttenuationC() const { return att_c; }

    /**
    * Gets shear wave attenuation
    * @return attenuation [db/wavelength or user defined]
    **/
    double getAttenuationS() const { return att_s; }

    /**
    * Gets bottom depth
    * @return depth [m]
    **/
    double getDepth() const { return depth; }

    /**
    * Gets sediment type name
    * @return type
    **/
    ::std::string getType() const { return type; }


    /**
    * Checks the validity of geoacoustic parameters provided
    * @return <i>true</i> if Sediment is valid, <i>false</i> otherwise
    **/
    virtual bool isValid() const { return ( vel_c != SEDIMENT_NOT_SET_VALUE && vel_s != SEDIMENT_NOT_SET_VALUE && density != SEDIMENT_NOT_SET_VALUE
                                        && att_c != SEDIMENT_NOT_SET_VALUE && att_s != SEDIMENT_NOT_SET_VALUE ); }

    
    /**
    * Gets geoacoustic parameters in a formatted fashion
    * @return formatted string
    **/
    virtual const ::std::string getStringValues() const;
    
    
    /**
    * Assignment operator
    * @param copy const reference to a Sediment object to be copied
    * @return Sediment reference to <i>this</i>
    **/
    Sediment& operator=( const Sediment& time );
    

    /**
    * Sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Sediment operator+( const Sediment& left, const Sediment& right );

    /**
    * Subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Sediment operator-( const Sediment& left, const Sediment& right );
  
    /**
    * Division operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Sediment operator/( const Sediment& left, const Sediment& right );

    /**
    * Multiplication operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Sediment operator*( const Sediment& left, const Sediment& right );

    /**
    * Compound assignment sum operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Sediment& operator+=( Sediment& left, const Sediment& right );

    /**
    * Compound assignment subtraction operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Sediment& operator-=( Sediment& left, const Sediment& right );

    /**
    * Compound assignment division operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Sediment& operator/=( Sediment& left, const Sediment& right );

    /**
    * Compound assignment multiplication operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Sediment& operator*=( Sediment& left, const Sediment& right );
    
    
      /**
    * Compound assignment sum operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Sediment& operator+=( Sediment& left, double right );

    /**
    * Compound assignment subtraction operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Sediment& operator-=( Sediment& left, double right );

    /**
    * Compound assignment division operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Sediment& operator/=( Sediment& left, double right );

    /**
    * Compound assignment multiplication operator
    * @param left left operand reference
    * @param right right operand const reference
    * @return <i>left</i> reference after the operation
    **/
    friend Sediment& operator*=( Sediment& left, double right );
    
    
    /**
    * Equality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left == right</i>, false otherwise
    **/  
    friend bool operator==( const Sediment& left, const Sediment& right );

    /**
    * Inequality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left != right</i>, false otherwise
    **/  
    friend bool operator!=( const Sediment& left, const Sediment& right );
    
    
    /**
    * Scalar sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Sediment operator+( const double left, const Sediment& right );
  
    /**
    * Scalar subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Sediment operator-( const double left, const Sediment& right );
    
    /**
    * Scalar division operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Sediment operator/( const double left, const Sediment& right );

    /**
    * Scalar multiplication operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Sediment operator*( const double left, const Sediment& right );
    
    
    /**
    * Scalar sum operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Sediment operator+( const Sediment& left, double right );

    /**
    * Scalar subtraction operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Sediment operator-( const Sediment& left, double right );

    /**
    * Scalar division operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Sediment operator/( const Sediment& left, double right );
    
    /**
    * Scalar multiplication operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return a new const instance holding the operation result  
    **/
    friend const Sediment operator*( const Sediment& left, double right );
    

    /**
    * << operator
    * @param os left operand ostream reference
    * @param instance right operand const Sediment reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend ::std::ostream& operator<<( ::std::ostream& os, const Sediment& instance );
    

    protected:

      
    /**
    * Stringstream object 
    **/
    static ::std::stringstream sstream;
    
    
    /**
    * Sediment type name
    **/
    ::std::string type;

    
    /**
    * Bottom depth. Used for shear wave velocity calculations [m]
    **/
    double depth;
    
    /**
    * Compressional wave velocity [m/s]
    **/
    double vel_c;
    
    /**
    * Shear wave velocity [m/s]
    **/
    double vel_s; 
    
    /**
    * Sediment density [g/cm^3]
    **/
    double density;
    
    /**
    * Compressional wave attenuation [db/wavelength]
    **/
    double att_c; 

    /**
    * Shear wave attenuation [db/wavelength]
    **/
    double att_s; 

    
    /**
    * Debug flag
    **/
    static bool debug;
    
    
  };

  // non-inline friend operator declarations
  //////////
  const Sediment operator/( const double left, const Sediment& right );

  const Sediment operator*( const double left, const Sediment& right );

  const Sediment operator/( const Sediment& left, const double right );
    
  const Sediment operator*( const Sediment& left, const double right );


  Sediment& operator+=( Sediment& left, const Sediment& right );

  Sediment& operator-=( Sediment& left, const Sediment& right );

  Sediment& operator/=( Sediment& left, const Sediment& right );

  Sediment& operator*=( Sediment& left, const Sediment& right );


  Sediment& operator+=( Sediment& left, double right );

  Sediment& operator-=( Sediment& left, double right );

  Sediment& operator/=( Sediment& left, double right );

  Sediment& operator*=( Sediment& left, double right );

  //inline functions
  /////////////////
  inline bool operator==( const Sediment& left, const Sediment& right ) {
    if ( &left == &right ) return true; 
    return( left.vel_c == right.vel_c && left.vel_s == right.vel_s && left.att_c == right.att_c && left.att_s == right.att_s
        && left.density == right.density );
  }


  inline bool operator!=( const Sediment& left, const Sediment& right ) {
    if ( &left == &right ) return false; 
    return( left.vel_c != right.vel_c || left.vel_s != right.vel_s || left.att_c != right.att_c || left.att_s != right.att_s
        || left.density != right.density );
  }


  inline ::std::ostream& operator<<(::std::ostream& os, const Sediment& instance ) {
    os << "Sediment type = " << instance.type << "; velocity_c = " << instance.vel_c << "; velocity_s = " << instance.vel_s << "; density = " << instance.density
      << "; attenuation_c = " << instance.att_c << "; attenuation_s = " << instance.att_s;
    return os;
  }


  inline const Sediment operator+( const Sediment& left, const Sediment& right ) {
    return( Sediment( left.type + " + " + right.type, left.vel_c + right.vel_c, left.vel_s + right.vel_s, left.density + right.density, left.att_c + right.att_c, left.att_s + right.att_s ) );
  }


  inline const Sediment operator-( const Sediment& left, const Sediment& right ) {
    return( Sediment( left.type + " - " + right.type, left.vel_c - right.vel_c, left.vel_s - right.vel_s, left.density - right.density, left.att_c - right.att_c, left.att_s - right.att_s ) );
  }


  inline const Sediment operator/( const Sediment& left, const Sediment& right ) {
    return( Sediment( left.type + " / " + right.type, left.vel_c / right.vel_c, left.vel_s / right.vel_s, left.density / right.density, left.att_c / right.att_c, left.att_s / right.att_s ) );
  }


  inline const Sediment operator*( const Sediment& left, const Sediment& right ) {
    return( Sediment( left.type + " * " + right.type, left.vel_c * right.vel_c, left.vel_s * right.vel_s, left.density * right.density, left.att_c * right.att_c, left.att_s * right.att_s ) );
  }
  ///////////////  
    
  /**
  * \brief Gravel type implementation
  * 
  * Literature gravel implementation
  **/
  class SedimentGravel : public Sediment {

    public:

    SedimentGravel(double depth = 1.0);


    protected:

    /**
    * Shear wave velocity formula taken from the literature
    **/
    double calculateVelocityS(double vels, double bottom_depth);

  };


  /**
  * \brief Sand type implementation
  * 
  * Literature sand implementation
  **/
  class SedimentSand : public Sediment {

    
    public:

      
    SedimentSand();

    
  };


  /**
  * \brief Silt type implementation
  * 
  * Literature silt implementation
  **/
  class SedimentSilt : public Sediment {

    
    public:

      
    SedimentSilt(double depth = 1.0);


    protected:

    /**
    * Shear wave velocity formula taken from the literature
    **/
    double calculateVelocityS(double vels, double bottom_depth);

    
  };


  /**
  * \brief Clay type implementation
  * 
  * Literature clay implementation
  **/
  class SedimentClay : public Sediment {

    
    public:

      
    SedimentClay();

    
  };


  /**
  * \brief Ooze type implementation
  * 
  * Literature ooze implementation
  **/
  class SedimentOoze : public Sediment {

    
    public:
      

    SedimentOoze();

    
  };


  /**
  * \brief Mud type implementation
  * 
  * Literature mud implementation
  **/
  class SedimentMud : public Sediment {

    
    public:

      
    SedimentMud(double depth = 1.0);


    protected:

    /**
    * Shear wave velocity formula taken from the literature
    **/
    double calculateVelocityS(double vels, double bottom_depth);

    
  };


  /**
  * \brief Rocks type implementation
  * 
  * Literature rocks implementation
  *
  * @bug Needs a better geoacoustic representation
  **/
  class SedimentRocks : public Sediment {

    
    public:

      
    SedimentRocks();

    
  };


  /**
  * \brief Organic type implementation
  * 
  * Literature organic implementation
  * 
  * @bug No geoacoustic parameters avalaible
  **/
  class SedimentOrganic : public Sediment {

    
    public:

      
    SedimentOrganic();

    
  };


  /**
  * \brief Deck41 nodules type implementation
  * 
  * Literature Deck41 nodules implementation
  *
  * @bug Needs a better geoacoustic representation
  **/
  class SedimentNodules : public Sediment {

    
    public:

      
    SedimentNodules();

    
  };


  /**
  * \brief Hard bottom type implementation
  * 
  * Literature hard bottom implementation
  **/
  class SedimentHardBottom : public Sediment {

    
    public:

      
    SedimentHardBottom();

    
  };

  
}


#endif /* WOSS_SEDIMENT_DEFINITIONS_H */

