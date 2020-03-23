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
 * @file   transducer-definitions.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for the woss::Transducer class
 *
 * Provides the interface for the woss::Transducer class
 */


#ifndef WOSS_TRANSDUCER_DEFINITIONS_H
#define WOSS_TRANSDUCER_DEFINITIONS_H


#include <iostream>
#include <complex>
#include <map>
#include <cmath>
#include <cassert>
#include "custom-precision-double.h"


namespace woss {
   
  
  class CoordZ;
  
  
  #define BEAM_PATTERN_CUSTOM_BEAM_PRECISION (1.0)
  #define CONDUCTANCE_CUSTOM_FREQUENCY_PRECISION (1.0)
  #define TVR_CUSTOM_FREQUENCY_PRECISION (1.0)
  #define OCV_CUSTOM_FREQUENCY_PRECISION (1.0)
  #define TRANSDUCER_NOT_SET (-1000)

  
  /**
  * \brief Transducer class
  *
  * woss::Transducer
  **/
  class Transducer {
    
    
    protected:
      
      
    /**
    * Map that links a angle with its precision to a signed power gain in decibel [db re uPa @ 1m]
    **/      
    typedef ::std::map< PDouble, double > BeamPowerMap;
    typedef BeamPowerMap::iterator BPMIter;
    typedef BeamPowerMap::reverse_iterator BPMRIter;
    typedef BeamPowerMap::const_iterator BPMCIter;
    typedef BeamPowerMap::const_reverse_iterator BPMCRIter;
    
    
    /**
    * Map that links a frequency with its precision to a conductance value [uS]
    **/      
    typedef ::std::map< PDouble, double > ConductanceMap;
    typedef ConductanceMap::iterator CMIter;
    typedef ConductanceMap::reverse_iterator CMRIter;
    typedef ConductanceMap::const_iterator CMCIter;
    typedef ConductanceMap::const_reverse_iterator CMCRIter;    
    
  
    /**
    * Map that links a frequency with its precision to a TVR value [db re uPa/V @ 1m]
    **/      
    typedef ::std::map< PDouble, double > TVRMap;
    typedef TVRMap::iterator TVRMIter;
    typedef TVRMap::reverse_iterator TVRMRIter;
    typedef TVRMap::const_iterator TVRMCIter;
    typedef TVRMap::const_reverse_iterator TVRMCRIter;    
    
 
    /**
    * Map that links a frequency with its precision to an OCV value [db re 1V/uPa]
    **/      
    typedef ::std::map< PDouble, double > OCVMap;
    typedef OCVMap::iterator OCVMIter;
    typedef OCVMap::reverse_iterator OCVMRIter;
    typedef OCVMap::const_iterator OCVMCIter;
    typedef OCVMap::const_reverse_iterator OCVMCRIter;        
  
    
    public:
    

    /**
    * Transducer default constructor. The object created is not valid
    * @param beam_precision precision of woss::PDouble objects representing angles
    * @param conduct_precison precision of woss::PDouble objects representing frequency
    * @param tvr_precision precision of woss::PDouble objects representing frequency
    * @param ocv_precision precision of woss::PDouble objects representing frequency
   **/        
    Transducer( long double beam_precision = BEAM_PATTERN_CUSTOM_BEAM_PRECISION, long double conduct_precison = CONDUCTANCE_CUSTOM_FREQUENCY_PRECISION, 
                long double tvr_precision = TVR_CUSTOM_FREQUENCY_PRECISION, long double ocv_precision = OCV_CUSTOM_FREQUENCY_PRECISION );

    /**
    * Transducer copy constructor
    * @param copy const reference a Transducer
    **/      
    Transducer( const Transducer& copy );
    
    /**
    * Transducer constructor.
    * @param beam_map map linking a woss::PDouble angle [dec degrees] (with precision beam_precision) 
    * to a signed power gain [decibel]
    * @param conductance_map map linking a woss::PDouble frequency [hz] (with precision conductance_precision) 
    * to conductance value
    * @param tvr_map map linking a woss::PDouble frequency [hz] (with precision tvr_precision) 
    * to a TVR value [db re 1 uPa/V @ 1m]
    * @param ocv_map map linking a woss::PDouble frequency [hz] (with precision ocv_precision) 
    * to a OCV value [db re 1V/uPa]
    **/     
    Transducer( BeamPowerMap& beam_map, ConductanceMap& conductance_map, TVRMap& tvr_map, OCVMap& ocv_map );
      
    
    virtual ~Transducer() { }
  
  
    /**
    * Transducer virtual factory method
    * @param beam_map map linking a woss::PDouble angle [dec degrees] (with precision beam_precision) 
    * to a signed power gain [decibel]
    * @param conductance_map map linking a woss::PDouble frequency [hz] (with precision conductance_precision) 
    * to conductance value
    * @param tvr_map map linking a woss::PDouble frequency [hz] (with precision tvr_precision) 
    * to a TVR value [db re 1 uPa/V @ 1m]
    * @param ocv_map map linking a woss::PDouble frequency [hz] (with precision ocv_precision) 
    * to a OCV value [db re 1V/uPa]
    * @return a heap-created Transducer object
    **/       
    virtual Transducer* create( long double beam_precision = BEAM_PATTERN_CUSTOM_BEAM_PRECISION, long double conduct_precison = CONDUCTANCE_CUSTOM_FREQUENCY_PRECISION, 
                                long double tvr_precision = TVR_CUSTOM_FREQUENCY_PRECISION, long double ocv_precision = OCV_CUSTOM_FREQUENCY_PRECISION ) const;

  
    /**
    * Transducer virtual factory method
    * @param beam_map map linking a woss::PDouble angle [dec degrees] (with precision beam_precision) 
    * to a signed power gain [decibel]
    * @param conductance_map map linking a woss::PDouble frequency [hz] (with precision conductance_precision) 
    * to conductance value
    * @param tvr_map map linking a woss::PDouble frequency [hz] (with precision tvr_precision) 
    * to a TVR value [db re 1 uPa/V @ 1m]
    * @param ocv_map map linking a woss::PDouble frequency [hz] (with precision ocv_precision) 
    * to a OCV value [db re 1V/uPa]
    * @return a heap-created Transducer object
    **/      
    virtual Transducer* create( BeamPowerMap& beam_map, ConductanceMap& conductance_map, TVRMap& tvr_map, OCVMap& ocv_map ) const;
    
    /**
    * Transducer virtual factory method
    * @param copy Transducer to be copied
    * @return a heap-created Transducer object
    **/    
    virtual Transducer* create( const Transducer& copy ) const;
    
    /**
    * Transducer virtual factory method
    * @return a heap-created copy of <b>this</b> instance
    **/    
    virtual Transducer* clone() const;
    
    
    /**
    * Checks the validity of the object
    * @return <i>true</i> if it has at least one value, <i>false</i> otherwise
    **/    
    virtual bool isValid() const;
   
    
    /**
    * Rotate the beam pattern 
    * @param angle angle [dec degrees]
    * @return reference to <b>*this</b>
    **/    
    Transducer& beampattern_rotate( double angle );
  
    /**
    * Adds given value to the beam pattern
    * @param value sum costant
    * @return reference to <b>*this</b>
    **/      
    Transducer& beampattern_sum( double value );

    /**
    * Multiplies the beam pattern by given value
    * @param value value
    * @return reference to <b>*this</b>
    **/          
    Transducer& beampattern_multiply( double value );
   
    
    /**
    * Returns the SPL (Sound Pressure Level) for given frequency and input power
    * @param frequency frequency [hz]
    * @param power input power [W]
    * @return reference to <b>*this</b>
    **/       
    virtual double getSPL( double frequency, double power ) const;

    /**
    * Returns the max SPL (Sound Pressure Level) for given frequency
    * @param frequency frequency [hz]
    * @return reference to <b>*this</b>
    **/       
    double getMaxSPL( double frequency ) const;
    
    /**
    * Returns the input power for given frequency and SPL 
    * @param frequency frequency [hz]
    * @param spl [db re uPa]   
    * @return power [W]
    **/     
    virtual double getPowerFromSPL( double frequency, double spl ) const;
    
    /**
    * Inserts and doesn't replace a signed power gain at given angle
    * @param angle angle value [dec degrees]. The corresponding PDouble will take SSP::beam_precision as precision
    * @param power signed power gain [db re uPa]
    * @return <i>true</i> if inserted, <i>false</i> otherwise
    **/    
    bool beampattern_insert( double angle, double power );

    
    /**
    * Replaces a signed power gain at given angle
    * @param angle angle value [dec degrees]. The corresponding PDouble will take SSP::beam_precision as precision
    * @param power signed power gain [db re uPa]
    * @return reference to <b>*this</b>
    **/     
    Transducer& beampattern_replace( double angle, double power );
    
    
    /**
    * Returns a const iterator to the signed power gain with key == of <i>angle</i> parameter
    * @param angle const reference to a double angle value
    * @returns const iterator to end() if <i>angle</i> is not found
    **/    
    BPMCIter beampattern_find( double angle ) const;

    
    /**
    * Erase the power gain with key == of <i>angle</i> parameter
    * @param angle const reference to a double angle value
    * @return reference to <b>*this</b>
    **/        
    Transducer& beampattern_erase( double angle );
    
    
    /**
    * Returns the number of angles stored
    * @return number of angles stored
    **/
    int beampattern_size() const;

    /**
    * Checks if the instance has stored values
    * @return <i>true</i> if condition applies, <i>false</i> otherwise
    **/
    bool beampattern_empty() const;
    
    
    /**
    * Clears all values
    * @return reference to <b>*this</b>
    **/       
    Transducer& beampattern_clear();
    
    
    /**
    * Returns a const iterator to the beginning of the beam pattern map
    * @returns const iterator 
    **/
    BPMCIter beampattern_begin() const;
    
    /**
    * Returns a const iterator to the end of the beam pattern map
    * @returns const iterator 
    **/    
    BPMCIter beampattern_end() const;
    
    /**
    * Returns a const reverse iterator to the reverse beginning of the beam pattern map
    * @returns const reverse iterator 
    **/    
    BPMCRIter beampattern_rbegin() const;
    
    /**
    * Returns a const reverse iterator to the reverse end of the beam pattern map
    * @returns const reverse iterator 
    **/    
    BPMCRIter beampattern_rend() const;    
    
    /**
    * Returns a const iterator to the signed power gain with key >= of <i>angle</i> parameter
    * @param angle angle [dec degrees]
    * @returns const iterator to end() if <i>angle</i> is not found
    **/ 
    BPMCIter beampattern_lower_bound( double angle ) const;
    
    /**
    * Returns a const iterator to the signed power gain with key > of <i>angle</i> parameter
    * @param angle angle [dec degrees]
    * @returns const iterator to end() if <i>angle</i> is not found
    **/     
    BPMCIter beampattern_upper_bound( double angle ) const;
    
              
    /**
    * Inserts and doesn't replace a conductance value at given frequency
    * @param angle frequency [hz]. The corresponding PDouble will take Transducer::conductance_precision as precision
    * @param conductance conductance value [uS]
    * @return <i>true</i> if inserted, <i>false</i> otherwise
    **/    
    bool conductance_insert( double frequency, double conductance );

    /**
    * Inserts and doesn't replace a complex impedance value at given frequency
    * @param angle frequency [hz]. The corresponding PDouble will take Transducer::conductance_precision as precision
    * @param conductance complex impedance [uS + j uF]
    * @return <i>true</i> if inserted, <i>false</i> otherwise
    **/       
    bool conductance_insert( double frequency, const ::std::complex<double>& impedance );

    
    /**
    * Replaces a conductance at given frequency
    * @param angle frequency [hz]. The corresponding PDouble will take Transducer::conductance_precision as precision
    * @param conductance conductance value  [uS]
    * @return reference to <b>*this</b>
    **/     
    Transducer& conductance_replace( double frequency, double conductance );
    
    /**
    * Replaces a conductance at given frequency
    * @param angle frequency [hz]. The corresponding PDouble will take Transducer::conductance_precision as precision
    * @param conductance complex impedance [uS + j uF]
    * @return reference to <b>*this</b>
    **/     
    Transducer& conductance_replace( double frequency, const ::std::complex<double>& impedance );
    
    
    /**
    * Returns a const iterator to the conductance with key == of <i>frequency</i> parameter
    * @param frequency frequency [hz]
    * @returns const iterator to end() if <i>frequency</i> is not found
    **/    
    CMCIter conductance_find( double frequency ) const;

    
    /**
    * Erase the power gain with key == of <i>angle</i> parameter
    * @param frequency frequency [hz]
    * @return reference to <b>*this</b>
    **/        
    Transducer& conductance_erase( double frequency );
    
    
    /**
    * Returns the number of frequencies stored
    * @return number of frequencies stored
    **/
    int conductance_size() const;

    /**
    * Checks if the instance has stored conductance values
    * @return <i>true</i> if condition applies, <i>false</i> otherwise
    **/
    bool conductance_empty() const;
    
    
    /**
    * Clears all values
    * @return reference to <b>*this</b>
    **/       
    Transducer& conductance_clear();
    
    
    /**
    * Returns a const iterator to the beginning of the conductance map
    * @returns const iterator 
    **/
    CMCIter conductance_begin() const;
    
    /**
    * Returns a const iterator to the end of the conductance map
    * @returns const iterator 
    **/    
    CMCIter conductance_end() const;
    
    /**
    * Returns a const reverse iterator to the reverse beginning of the conductance map
    * @returns const reverse iterator 
    **/    
    CMCRIter conductance_rbegin() const;
    
    /**
    * Returns a const reverse iterator to the reverse end of the conductance map
    * @returns const reverse iterator 
    **/    
    CMCRIter conductance_rend() const;    
    
    /**
    * Returns a const iterator to the conductance with key >= of <i>frequency</i> parameter
    * @param frequency frequency [hz]
    * @returns const iterator to end() if <i>angle</i> is not found
    **/ 
    CMCIter conductance_lower_bound( double frequency ) const;
    
    /**
    * Returns a const iterator to the conductance with key > of <i>frequency</i> parameter
    * @param frequency frequency [hz]
    * @returns const iterator to end() if <i>angle</i> is not found
    **/   
    CMCIter conductance_upper_bound( double frequency ) const;    
    
    
    /**
    * Inserts and doesn't replace a tvr at given frequency
    * @param frequency frequency value [hz]. The corresponding PDouble will take Transducer::tvr_precision as precision
    * @param tvr tvr [db re uPa/V @ 1m]
    * @return <i>true</i> if inserted, <i>false</i> otherwise
    **/    
    bool tvr_insert( double frequency, double tvr );
    
    /**
    * Replaces a tvr at given frequency
    * @param frequency frequency value [hz]. The corresponding PDouble will take Transducer::tvr_precision as precision
    * @param tvr tvr [db re uPa/V @ 1m]
    * @return reference to <b>*this</b>
    **/     
    Transducer& tvr_replace( double frequency, double tvr );
    
    
    /**
    * Returns a const iterator to the tvr with key == to <i>frequency</i> parameter
    * @param frequency frequency [hz]
    * @returns const iterator to end() if <i>angle</i> is not found
    **/    
    TVRMCIter tvr_find( double frequency ) const;

    
    /**
    * Erase the tvr with key == to <i>frequency</i> parameter
    * @param frequency frequency [hz]
    * @return reference to <b>*this</b>
    **/        
    Transducer& tvr_erase( double frequency );
    
    
    /**
    * Returns the number of frequencies stored
    * @return number of frequencies stored
    **/
    int tvr_size() const;

    /**
    * Checks if the instance has any stored tvr values
    * @return <i>true</i> if condition applies, <i>false</i> otherwise
    **/
    bool tvr_empty() const;
    
    
    /**
    * Clears all values
    * @return reference to <b>*this</b>
    **/       
    Transducer& tvr_clear();
    
    
    /**
    * Returns a const iterator to the beginning of the tvr map
    * @returns const iterator 
    **/
    TVRMCIter tvr_begin() const;
    
    /**
    * Returns a const iterator to the end of the tvr map
    * @returns const iterator 
    **/    
    TVRMCIter tvr_end() const;
    
    /**
    * Returns a const reverse iterator to the reverse beginning of the tvr map
    * @returns const reverse iterator 
    **/    
    TVRMCRIter tvr_rbegin() const;
    
    /**
    * Returns a const reverse iterator to the reverse end of the tvr map
    * @returns const reverse iterator 
    **/    
    TVRMCRIter tvr_rend() const;    
    
    /**
    * Returns a const iterator to the signed power gain with key >= of <i>frequency</i> parameter
    * @param frequency frequency [hz]
    * @returns const iterator to end() if <i>frequency</i> is not found
    **/ 
    TVRMCIter tvr_lower_bound( double frequency ) const;
    
    /**
    * Returns a const iterator to the signed power gain with key > of <i>frequency</i> parameter
    * @param frequency frequency [hz]
    * @returns const iterator to end() if <i>frequency</i> is not found
    **/     
    TVRMCIter tvr_upper_bound( double frequency ) const;      
    
    
    /**
    * Inserts and doesn't replace an OCV value at given frequency
    * @param frequency frequency value [hz]. The corresponding PDouble will take Transducer::ocv_precision as precision
    * @param ocv OCV [db re 1V/uPa]
    * @return <i>true</i> if inserted, <i>false</i> otherwise
    **/    
    bool ocv_insert( double frequency, double ocv );
    
    /**
    * Replaces an OCV value at given frequency
    * @param frequency frequency value [hz]. The corresponding PDouble will take Transducer::ocv_precision as precision
    * @param ocv OCV [db re 1V/uPa]
    * @return reference to <b>*this</b>
    **/     
    Transducer& ocv_replace( double frequency, double ocv );
    
    
    /**
    * Returns a const iterator to the OCV with key == of <i>frequency</i> parameter
    * @param frequency frequency [hz]
    * @returns const iterator to end() if <i>frequency</i> is not found
    **/    
    OCVMCIter ocv_find( double frequency ) const;

    
    /**
    * Erase the OCV with key == of <i>frequency</i> parameter
    * @param frequency frequency [hz]
    * @return reference to <b>*this</b>
    **/        
    Transducer& ocv_erase( double frequency );
    
    
    /**
    * Returns the number of OCV value stored
    * @return number of angles stored
    **/
    int ocv_size() const;

    /**
    * Checks if the instance has any OCV stored values
    * @return <i>true</i> if condition applies, <i>false</i> otherwise
    **/
    bool ocv_empty() const;
    
    
    /**
    * Clears all OCV values
    * @return reference to <b>*this</b>
    **/       
    Transducer& ocv_clear();
    
    
    /**
    * Returns a const iterator to the beginning of the OCV map
    * @returns const iterator 
    **/
    OCVMCIter ocv_begin() const;
    
    /**
    * Returns a const iterator to the end of the OCV map
    * @returns const iterator 
    **/    
    OCVMCIter ocv_end() const;
    
    /**
    * Returns a const reverse iterator to the reverse beginning of the OCV map
    * @returns const reverse iterator 
    **/    
    OCVMCRIter ocv_rbegin() const;
    
    /**
    * Returns a const reverse iterator to the reverse end of the OCV map
    * @returns const reverse iterator 
    **/    
    OCVMCRIter ocv_rend() const;    
    
    /**
    * Returns a const iterator to the OCV with key >= of <i>frequency</i> parameter
    * @param frequency frequency [hz]
    * @returns const iterator to end() if <i>angle</i> is not found
    **/ 
    OCVMCIter ocv_lower_bound( double frequency ) const;
    
    /**
    * Returns a const iterator to the OCV with key > of <i>frequency</i> parameter
    * @param frequency frequency [hz]
    * @returns const iterator to end() if <i>angle</i> is not found
    **/     
    OCVMCIter ocv_upper_bound( double frequency ) const;          
    
     
    /**
    * Clears all maps
    * @return reference to <b>*this</b>
    **/       
    Transducer& clearAll();
    
    
    /**
    * Imports values in from the given stream
    * @param stream_in const reference to an istream instance
    * @return true if method was successful, false otherwise
    **/
    virtual bool import( ::std::istream& stream_in );
    
    /**
    * Imports values in from the given binary stream
    * @param stream_in const reference to an istream instance
    * @return true if method was successful, false otherwise
    **/    
    virtual bool importBinary( ::std::fstream& stream_in );
 
    
    /**
    * Writes the beam pattern to the given stream
    * @param stream_out const reference to an ostream instance
    * @param rotation beam pattern rotation [decimal degrees]
    * @param mult_costant value to be added
    * @param add_costant value to be multiplied by
    * @return true if method was successful, false otherwise
    **/    
    bool writeVertBeamPattern( ::std::ostream& stream_out, const CoordZ& tx, const CoordZ& rx, double init_bearing, double vert_rot = 0, double horiz_rot = 0, double mult_costant = 1, double add_costant = 0 ) const;
 
    
    /**
    * Write SPL values for all frequencies to the given steram
    * @param stream_out const reference to an ostream instance
    * @return true if method was successful, false otherwise
    **/        
    bool writeSPL( ::std::ostream& stream_out, double frequency_step, double power ) const;

    
    /**
    * Write values out to the given stream
    * @param stream_out const reference to an ostream instance
    * @return true if method was successful, false otherwise
    **/
    virtual bool write( ::std::ostream& stream_out ) const;
    
    /**
    * Write values out to the given binary stream
    * @param stream_out const reference to an ostream instance
    * @return true if method was successful, false otherwise
    **/    
    virtual bool writeBinary( ::std::fstream& file_out ) const;

    
    Transducer& setMaxPower( double power );
    
    Transducer& setDutyCycle( double cycle );
    
    Transducer& setResonanceFrequency( double frequency );
    
    Transducer& setBandwith3dB( double frequency );

    Transducer& setTypeName( const ::std::string& name );
    
    
    /**
    * Sets the beam pattern precision for all PDouble angle values. If the given precision is different from
    * current value, the profile will be modified accordingly
    * @param prec beam precision [decimal degrees]
    **/
    virtual Transducer& setBeamPrecision( long double prec ); 

    /**
    * Sets the TVR precision for all PDouble frequency values. If the given precision is different from
    * current value, the profile will be modified accordingly
    * @param prec frequency [hz]
    **/
    virtual Transducer& setTVRPrecision( long double prec ); 
    
    /**
    * Sets the OCV precision for all PDouble frequency values. If the given precision is different from
    * current value, the profile will be modified accordingly
    * @param prec frequency [hz]
    **/
    virtual Transducer& setOCVPrecision( long double prec );     
    
    /**
    * Sets the conductance precision for all PDouble frequency values. If the given precision is different from
    * current value, the profile will be modified accordingly
    * @param prec frequency [hz]
    **/
    virtual Transducer& setConductancePrecision( long double prec ); 
    
    
    bool hasToroidalSymmetry() const;
    
    bool hasConicalSymmetry() const;
    
    
    /**
    * Returns max allowed input Power for this transducer
    * @returns power [W]
    **/
    double getMaxPower() const;
    
    /**
    * Returns the recommended duty cycle
    * @returns duty cycle [between 0 and 1]
    **/
    double getDutyCycle() const;
    
    /**
    * Returns the resonance frequency of the transducer
    * @returns frequency [hz]
    **/
    double getResonanceFrequency() const;    
    
    /**
    * Returns the bandwith at -3db around the resonance frequency
    * @returns bandwith [hz]
    **/
    double getBandwith3dB() const;

    /**
    * Returns the transducer typename
    * @returns name
    **/
    ::std::string getTypeName() const;

    
    /**
    * Returns the beam pattern beam precision
    * @returns beam precision [decimal degrees]
    **/
    long double getBeamPrecision() const;
    
    /**
    * Returns the TVR frequency precision
    * @returns precision [hz]
    **/
    long double getTVRPrecision() const;   

    /**
    * Returns the OCV frequency precision
    * @returns precision [hz]
    **/
    long double getOCVPrecision() const;
       
    /**
    * Returns the conductance frequency precision
    * @returns precision [hz]
    **/
    long double getConductancePrecision() const;    
    
    
    /**
    * Assignment operator
    * @param copy const reference to a Transducer object to be copied
    * @return Transducer reference to <i>this</i>
    **/
    Transducer& operator=( const Transducer& x ) ;


    /**
    * Equality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left == right</i>, false otherwise
    **/ 
    friend bool operator==( const Transducer& left, const Transducer& right );

    /**
    * Inequality operator
    * @param left left operand const reference
    * @param right right operand const reference
    * @return true if <i>left == right</i>, false otherwise
    **/ 
    friend bool operator!=( const Transducer& left, const Transducer& right );

    
    /**
    * << operator
    * @param os left operand ostream reference
    * @param instance right operand const Transducer reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend ::std::ostream& operator<<( ::std::ostream& os, const Transducer& instance );
    
    /**
    * >> operator
    * @param os left operand ostream reference
    * @param instance right operand const Transducer reference
    * @return <i>os</i> reference after the operation
    **/ 
    friend ::std::ostream& operator>>( ::std::istream& is, const Transducer& instance );
    
    
    /**
    * Sets debug flag for all instances
    * @param flag debug bool
    **/
    static void setDebug( bool flag ) { debug = flag; }
            
    
    protected:
      
      
    static const ::std::string conical_string;
    
    static const ::std::string toroidal_string;
    
    
    /**
    * debug flag
    **/    
    static bool debug;

    
    /**
    * set to true if transducer has conical symmetry along axis through angle = 0°
    **/        
    bool has_conical_symmetry;

    
    /**
    * resonance frequency of the transducer [hz]
    **/          
    double resonance_frequency;

    /**
    * the bandwith @ -3dB around the resonance frequency [hz]
    **/         
    double bandwith_3db;

    /**
    * max allowed input power [W]
    **/      
    double max_power;

    /**
    * recommended duty cycle [between 0 and 1]
    **/    
    double duty_cycle;   
          
    
    /**
    * angle precision [decimal degrees]
    **/
    long double beam_precision;

    /**
    * frequency precision [hz]
    **/    
    long double conductance_precision;
    
    /**
    * frequency precision [hz]
    **/    
    long double tvr_precision;
    
    /**
    * frequency precision [hz]
    **/    
    long double ocv_precision;
    
    /**
    * transducer's model name
    **/     
    ::std::string type_name;
    
    
    /**
    * vertical beam pattern map
    **/    
    BeamPowerMap beam_power_map;

    /**
    * conductance map
    **/      
    ConductanceMap conductance_map;
    
    /**
    * TVR map
    **/      
    TVRMap tvr_map;
    
     /**
    * OCV map
    **/     
    OCVMap ocv_map;
          
  
    /**
    * Returns an angle in [-180.0 , 180.0]
    * @param angle angle [decimal degrees]
    * @return angle [decimal degrees]
    **/    
    virtual double normalizeAngle( double angle ) const;
  
    
    /**
    * Returns a value for given frequency in the given map and with given precision. 
    * @param frequency frequency [decimal degrees]
    * @param map one of the transducer's map
    * @param map precision    
    * @return value found
    **/        
    virtual double getValue( double frequency, const ::std::map< PDouble, double >& map, long double precision, bool use_linear = false, double costant = 20.0 ) const;
    
    
    /**
    * Adds a value to the current beam pattern
    * @param value value to be added
    **/     
    virtual void beampattern_sum( double value, BeamPowerMap& map );

    /**
    * The current beam pattern is multiplied by given value
    * @param value value
    **/       
    virtual void beampattern_multiply( double value, BeamPowerMap& map );

    /**
    * Rotates the current beam pattern of a given angle
    * @param value angle [decimal degrees]
    **/     
    virtual void beampattern_rotate( double angle, BeamPowerMap& map );
    
    
    /**
    * Imports values in the given map with given precision from the given stream
    * @param stream_in const reference to an istream instance
    * @param map any of the transducer's map
    * @param precision map PDouble keys precision
    * @param is_angle flag to signal if the input map is a beam pattern map
    * @return true if method was successful, false otherwise
    **/
    virtual bool import( ::std::istream& stream_in, ::std::map< PDouble, double >& map, long double precision, bool is_angle = false );
    
    /**
    * Imports values in the given map with given precision from the given binary stream
    * @param stream_in const reference to an istream instance
    * @param map any of the transducer's map   
    * @param precision map PDouble keys precision    
    * @param is_angle flag to signal if the input map is a beam pattern map    
    * @return true if method was successful, false otherwise
    **/    
    virtual bool importBinary( ::std::fstream& file_in, ::std::map< PDouble, double >& map, long double precision, bool is_angle = false );

    
    /**
    * Writes values in the given map with given precision to the given stream
    * @param stream_out const reference to an ostream instance
    * @param map any of the transducer's map   
    * @return true if method was successful, false otherwise
    **/
    virtual bool write( ::std::ostream& stream_out, const ::std::map< PDouble, double >& map ) const;

    /**
    * Writes values in the given map with given precision to the given binary stream
    * @param stream_out const reference to an ostream instance
    * @param map any of the transducer's map      
    * @return true if method was successful, false otherwise
    **/    
    virtual bool writeBinary( ::std::fstream& file_out, const ::std::map< PDouble, double >& map ) const;
    
    
    virtual const ::std::string& getSymmetryString() const;
    
    
  };
   
  
  //////////
  //inline functions  
  inline Transducer& Transducer::setMaxPower( double power ) {
    max_power = power;
    return *this;
  }
  
  
  inline Transducer& Transducer::setDutyCycle( double cycle ) { 
    duty_cycle = cycle;
    return *this;   
  }
  
  
  inline Transducer& Transducer::setResonanceFrequency( double frequency ) {
    resonance_frequency = frequency;
    return *this;   
  }
   
   
  inline Transducer& Transducer::setBandwith3dB( double bw ) {
    bandwith_3db = bw;
    return *this;   
  }


  inline Transducer& Transducer::setTypeName( const ::std::string& name ) {
    type_name = name;
    return *this;
  }
  
  
  inline bool Transducer::hasToroidalSymmetry() const {
    return !has_conical_symmetry;
  }
    
  
  inline bool Transducer::hasConicalSymmetry() const {
    return has_conical_symmetry;
  }
  
    
  inline double Transducer::getMaxPower() const {
    return max_power;
  }
  
  
  inline double Transducer::getDutyCycle() const { 
    return duty_cycle;
  }
  
  
  inline double Transducer::getResonanceFrequency() const {
    return resonance_frequency;
  }
  
  
  inline double Transducer::getBandwith3dB() const {
    return bandwith_3db;
  }
  
  
  inline ::std::string Transducer::getTypeName() const {
    return type_name;
  }
  
  
  inline double Transducer::getMaxSPL( double frequency ) const {
    return getSPL( frequency, max_power );
  }

  
  inline Transducer& Transducer::beampattern_rotate( double angle ) {
    beampattern_rotate( angle, beam_power_map );
    return *this;
  }
  
  
  inline Transducer& Transducer::beampattern_sum( double value ) { 
    beampattern_sum( value, beam_power_map );
    return *this;    
  }


  inline Transducer& Transducer::beampattern_multiply( double value ) {
    beampattern_multiply( value, beam_power_map );
    return *this;  
  }

  
  inline bool Transducer::beampattern_insert( double angle, double power ) { 
    return beam_power_map.insert( ::std::make_pair( PDouble( normalizeAngle(angle), beam_precision), power ) ).second;
  }
 
 
  inline Transducer& Transducer::beampattern_replace( double angle, double power ) { 
    normalizeAngle(angle);
    beam_power_map[PDouble(angle, beam_precision)] = power;
    return *this;
  }
  

  inline Transducer::BPMCIter Transducer::beampattern_find( double angle ) const { 
    return beam_power_map.find(angle);
  }
  

  inline Transducer& Transducer::beampattern_erase( double angle ) { 
    beam_power_map.erase(angle);
    return *this;
  }

  inline int Transducer::beampattern_size() const { 
    return beam_power_map.size();  
  }
  

  inline bool Transducer::beampattern_empty() const { 
    return beam_power_map.empty();  
  }
  

  inline Transducer& Transducer::beampattern_clear() { 
    beam_power_map.clear();
    return *this;
  }
  

  inline Transducer::BPMCIter Transducer::beampattern_begin() const { 
    return beam_power_map.begin();  
  }
  

  inline Transducer::BPMCIter Transducer::beampattern_end() const { 
    return beam_power_map.end();  
  }
  

  inline Transducer::BPMCRIter Transducer::beampattern_rbegin() const  { 
    return beam_power_map.rbegin();  
  }
  

  inline Transducer::BPMCRIter Transducer::beampattern_rend() const  { 
    return beam_power_map.rend();  
  }
  

  inline Transducer::BPMCIter Transducer::beampattern_lower_bound( double angle ) const  { 
    return beam_power_map.lower_bound(PDouble(angle, beam_precision));  
  }
  

  inline Transducer::BPMCIter Transducer::beampattern_upper_bound( double angle ) const  { 
    return beam_power_map.upper_bound(PDouble(angle, beam_precision));  
  }
  
  

  inline bool Transducer::conductance_insert( double frequency, double conductance ) { 
    return conductance_map.insert( ::std::make_pair( PDouble( frequency, conductance_precision ), conductance ) ).second;
  }
  
  
  inline bool Transducer::conductance_insert( double frequency, const ::std::complex< double >& impedance ) { 
    return conductance_map.insert( ::std::make_pair( PDouble( frequency, conductance_precision ), impedance.real() 
                                                               / ( ::std::pow( ::std::abs(impedance), 2.0 ) ) ) ).second;
  }  
 
 
  inline Transducer& Transducer::conductance_replace( double frequency, double conductance ) { 
    conductance_map[PDouble(frequency, conductance_precision)] = conductance;
    return *this;
  }
  
  
  inline Transducer& Transducer::conductance_replace( double frequency, const ::std::complex< double >& impedance ) { 
    conductance_map[PDouble(frequency, conductance_precision)] = impedance.real() 
                                                               / ( ::std::pow( ::std::abs(impedance), 2.0 ) ) ;
    return *this;
  } 
  

  inline Transducer::CMCIter Transducer::conductance_find( double frequency ) const { 
    return conductance_map.find(frequency);
  }
  

  inline Transducer& Transducer::conductance_erase( double frequency ) { 
    conductance_map.erase(frequency);
    return *this;
  }

  inline int Transducer::conductance_size() const { 
    return conductance_map.size();  
  }
  

  inline bool Transducer::conductance_empty() const { 
    return conductance_map.empty();  
  }
  

  inline Transducer& Transducer::conductance_clear() { 
    conductance_map.clear();
    return *this;
  }
  

  inline Transducer::CMCIter Transducer::conductance_begin() const { 
    return conductance_map.begin();  
  }
  

  inline Transducer::CMCIter Transducer::conductance_end() const { 
    return conductance_map.end();  
  }
  

  inline Transducer::CMCRIter Transducer::conductance_rbegin() const  { 
    return conductance_map.rbegin();  
  }
  

  inline Transducer::CMCRIter Transducer::conductance_rend() const  { 
    return conductance_map.rend();  
  }
  

  inline Transducer::CMCIter Transducer::conductance_lower_bound( double frequency ) const  { 
    return conductance_map.lower_bound(PDouble(frequency, conductance_precision));  
  }
  

  inline Transducer::CMCIter Transducer::conductance_upper_bound( double frequency ) const  { 
    return conductance_map.upper_bound(PDouble(frequency, conductance_precision));  
  }




  inline bool Transducer::tvr_insert( double frequency, double tvr ) { 
    return tvr_map.insert( ::std::make_pair( PDouble( frequency, tvr_precision ), tvr ) ).second;
  }
 
 
  inline Transducer& Transducer::tvr_replace( double frequency, double tvr ) { 
    tvr_map[PDouble(frequency, tvr_precision)] = tvr;
    return *this;
  }
  

  inline Transducer::CMCIter Transducer::tvr_find( double frequency ) const { 
    return tvr_map.find(frequency);
  }
  

  inline Transducer& Transducer::tvr_erase( double frequency ) { 
    tvr_map.erase(frequency);
    return *this;
  }

  inline int Transducer::tvr_size() const { 
    return tvr_map.size();  
  }
  

  inline bool Transducer::tvr_empty() const { 
    return tvr_map.empty();  
  }
  

  inline Transducer& Transducer::tvr_clear() { 
    tvr_map.clear();
    return *this;
  }
  

  inline Transducer::CMCIter Transducer::tvr_begin() const { 
    return tvr_map.begin();  
  }
  

  inline Transducer::CMCIter Transducer::tvr_end() const { 
    return tvr_map.end();  
  }
  

  inline Transducer::CMCRIter Transducer::tvr_rbegin() const  { 
    return tvr_map.rbegin();  
  }
  

  inline Transducer::CMCRIter Transducer::tvr_rend() const  { 
    return tvr_map.rend();  
  }
  

  inline Transducer::CMCIter Transducer::tvr_lower_bound( double frequency ) const  { 
    return tvr_map.lower_bound(PDouble(frequency, tvr_precision));  
  }
  

  inline Transducer::CMCIter Transducer::tvr_upper_bound( double frequency ) const  { 
    return tvr_map.upper_bound(PDouble(frequency, tvr_precision));  
  }



  inline bool Transducer::ocv_insert( double frequency, double ocv ) { 
    return ocv_map.insert( ::std::make_pair( PDouble( frequency, ocv_precision ), ocv ) ).second;
  }
 
 
  inline Transducer& Transducer::ocv_replace( double frequency, double ocv ) { 
    ocv_map[PDouble(frequency, ocv_precision)] = ocv;
    return *this;
  }
  

  inline Transducer::CMCIter Transducer::ocv_find( double frequency ) const { 
    return ocv_map.find(frequency);
  }
  

  inline Transducer& Transducer::ocv_erase( double frequency ) { 
    ocv_map.erase(frequency);
    return *this;
  }

  inline int Transducer::ocv_size() const { 
    return ocv_map.size();  
  }
  

  inline bool Transducer::ocv_empty() const { 
    return ocv_map.empty();  
  }
  

  inline Transducer& Transducer::ocv_clear() { 
    ocv_map.clear();
    return *this;
  }
  

  inline Transducer::CMCIter Transducer::ocv_begin() const { 
    return ocv_map.begin();  
  }
  

  inline Transducer::CMCIter Transducer::ocv_end() const { 
    return ocv_map.end();  
  }
  

  inline Transducer::CMCRIter Transducer::ocv_rbegin() const  { 
    return ocv_map.rbegin();  
  }
  

  inline Transducer::CMCRIter Transducer::ocv_rend() const  { 
    return ocv_map.rend();  
  }
  

  inline Transducer::CMCIter Transducer::ocv_lower_bound( double frequency ) const  { 
    return ocv_map.lower_bound(PDouble(frequency, ocv_precision));  
  }
  

  inline Transducer::CMCIter Transducer::ocv_upper_bound( double frequency ) const  { 
    return ocv_map.upper_bound(PDouble(frequency, ocv_precision));  
  }


  inline Transducer& Transducer::clearAll() { 
    beam_power_map.clear();
    ocv_map.clear();
    tvr_map.clear();
    conductance_map.clear();
    return *this;
  }
  
  
  inline long double Transducer::getBeamPrecision() const { 
    return beam_precision;
  }
  
  
  inline long double Transducer::getConductancePrecision() const { 
    return conductance_precision;
  }
  
  
  inline long double Transducer::getTVRPrecision() const { 
    return tvr_precision;
  }
  
  
  inline long double Transducer::getOCVPrecision() const { 
    return ocv_precision;
  }  
    
  
  inline bool operator==( const Transducer& left, const Transducer& right ) {
    if ( &left == &right ) return true; 
    return( left.beam_power_map == right.beam_power_map && left.conductance_map == right.conductance_map
         && left.tvr_map == right.tvr_map && left.ocv_map == right.ocv_map );
  }


  inline bool operator!=( const Transducer& left, const Transducer& right ) {
    if ( &left == &right ) return false; 
    if ( &left == &right ) return true; 
    return( left.beam_power_map != right.beam_power_map || left.conductance_map != right.conductance_map
         || left.tvr_map != right.tvr_map || left.ocv_map != right.ocv_map );
  }


  inline ::std::ostream& operator<<( ::std::ostream& os, const Transducer& instance ) {
    if ( !instance.write( os ) ) os.setstate( ::std::ios_base::failbit );
    return os;
  }


  inline ::std::istream& operator>>( ::std::istream& is, Transducer& instance ) {
    if ( !instance.import( is ) ) is.setstate( ::std::ios_base::failbit );
    return is;
  }
    

}


#endif // WOSS_TRANSDUCER_DEFINITIONS_H

