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
 * @file   bellhop-creator-tcl.h
 * @author Federico Guerra
 * 
 * \brief Provides the interface for woss::BellhopCreator class
 *
 * Provides the interface for the woss::BellhopCreator class
 */


#ifndef WOSS_BELLHOP_CREATOR_DEFINITIONS_TCL_H
#define WOSS_BELLHOP_CREATOR_DEFINITIONS_TCL_H


#ifdef WOSS_NS_MIRACLE_SUPPORT


#include <tclcl.h>
#include <bellhop-creator.h>


namespace woss {
  
    
  /**
  * \brief Tcl hooks for BellhopCreator class
  *
  * BellhopCreatorTcl implements tcl hooks for BellhopCreator class
  */
  class BellhopCreatorTcl : public BellhopCreator, public TclObject {

    
    public:
    
      
    /**
    * BellhopCreatorTcl default constructor
    */
    BellhopCreatorTcl();
    
    virtual ~BellhopCreatorTcl() { }

    /**
    * TCL command interpreter. It implements the following OTcl methods:
    * <ul>
 <li><b>setSimulationTimes &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt;
    *                            &lt;<i>start day [1-31]</i>&gt; &lt;<i>start month [1-12]</i>&gt; &lt;<i>start year</i>&gt; 
    *                            &lt;<i>start hours [0-23]</i>&gt; &lt;<i>start minutes [0-59]</i>&gt; &lt;<i>start seconds [0-59]</i>&gt;
    *                            &lt;<i>end day [1-31]</i>&gt; &lt;<i>end month [1-12]</i>&gt; &lt;<i>end year</i>&gt; 
    *                            &lt;<i>end hours [0-23]</i>&gt; &lt;<i>end minutes [0-59]</i>&gt; &lt;<i>end seconds [0-59]</i>&gt; </b>: 
    *     sets the SimTime for given tx and rx woss::Location;
    *  <li><b>setWorkDirPath &lt;<i>directory pathname</i>&gt; </b>: 
    *     sets the work directory pathname for all Woss instances;
    *  <li><b>setTotalRuns &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; lt;<i>total number of runs</i>&gt; </b>:
    *     sets the total number of channel simulator runs for given tx and rx woss::Location;
    *  <li><b>setEvolutionTimeQuantum &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; lt;<i>time threshold in seconds</i>&gt; </b>:
    *     sets the time evolution threshold for time evolution purposes and for given tx and rx woss::Location;
    *  <li><b>setFrequencyStep &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; lt;<i>frequency step in hZ</i>&gt; </b>:
    *     sets the frequency step in hZ, for multi-frequencies calculations purposes and for given tx and rx woss::Location;
    *  <li><b>setCustomAngles &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt;
    *                      &lt;<i>minimum angle [decimal degrees]</i>&gt; &lt;<i>maximum angle [decimal degrees]</i>&gt; </b>: 
    *     sets the CustomAngles for given tx and rx woss::Location;
    *  <li><b>setBellhopMode &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i>mode string</i>&gt; </b>: 
    *     sets the Bellhop run mode (A, a , C, S , I) for given tx and rx woss::Location. See Bellhop documentation for more info
    *  <li><b>setBeamOptions &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i>beam options string</i>&gt; </b>: 
    *     sets the Bellhop beam options ( G, C, R , B) for given tx and rx woss::Location. See Bellhop documentation for more info
    *  <li><b>setBathymetryType &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i>bathymetry type string</i>&gt; </b>: 
    *     sets the Bellhop bathymetry type (L, C) for given tx and rx woss::Location. See Bellhop documentation for more info
    *  <li><b>setBathymetryMethod &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i>bathymetry write method string</i>&gt; </b>: 
    *     sets the bathymetry write method (S, D) for given tx and rx woss::Location. S = Slope bathymetry, D = Discrete jumps bathymetry.
    *  <li><b>setAltimetryType &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i>altimetry type string</i>&gt; </b>: 
    *     sets the Bellhop altimetry type (L, C) for given tx and rx woss::Location. See Bellhop documentation for more info
    *  <li><b>setBellhopPath &lt;<i>bellhop program filesystem path</i>&gt; </b>: 
    *     sets the path of Bellhop program
    *  <li><b>setRangeSteps &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i> total range steps </i>&gt; </b>: 
    *     sets the total number of range steps to be used in every Bellhop run, for given tx and rx woss::Location;
    *  <li><b>setTotalTransmitters &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i> total transmitting sources </i>&gt; </b>: 
    *     sets the number of transmitting sources (depth steps) to be used in every Bellhop run, for given tx and rx woss::Location;
    *  <li><b>setTxMinDepthOffset &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i> tx min depth offset </i>&gt; </b>: 
    *     sets the minimum tx depth offset to be used in every Bellhop run, for given tx and rx woss::Location;
    *  <li><b>setTxMaxDepthOffset &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i> tx max depth offset </i>&gt; </b>: 
    *     sets the maximum tx depth offset to be used in every Bellhop run, for given tx and rx woss::Location;
    *  <li><b>setRxTotalDepths &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i> total rx depth steps </i>&gt; </b>: 
    *         sets the total number of rx depth steps to be used in every Bellhop run, for given tx and rx woss::Location;
    *  <li><b>setRxMinDepthOffset &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i> rx min depth offset </i>&gt; </b>: 
    *         sets the minimum rx depth offset to be used in every Bellhop run, for given tx and rx woss::Location;
    *  <li><b>setRxMaxDepthOffset &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i> rx max depth offset  </i>&gt; </b>: 
    *         sets the maximum rx depth offset to be used in every Bellhop run, for given tx and rx woss::Location;
    *  <li><b>setRxTotalRanges &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i> total rx range steps </i>&gt; </b>: 
    *         sets the total number or rx range steps to be used in every Bellhop run, for given tx and rx woss::Location;
    *  <li><b>setRxMinRangeOffset &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i> rx min range offset </i>&gt; </b>: 
    *         sets the minimum rx range offset to be used in every Bellhop run, for given tx and rx woss::Location;
    *  <li><b>setRxMaxRangeOffset &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i> rx max range offset </i>&gt; </b>: 
    *         sets the maximum rx range offset to be used in every Bellhop run, for given tx and rx woss::Location;
    *  <li><b>setRaysNumber &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i> total rays number </i>&gt; </b>: 
    *         sets the total number of rays to be used in every Bellhop run, for given tx and rx woss::Location;
    *  <li><b>setSSPDepthPrecision &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i> precision [m] </i>&gt; </b>: 
    *         sets the SSP depth precision to be used in every Bellhop run, for given tx and rx woss::Location;
    *  <li><b>setBoxRange &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i> precision [m] </i>&gt; </b>: 
    *         sets the box range to be used in every Bellhop run, for given tx and rx woss::Location;
    *  <li><b>setBoxDepth &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i> precision [m] </i>&gt; </b>: 
    *         sets the box depth to be used in every Bellhop run, for given tx and rx woss::Location;
    *  <li><b>setNormalizedSSPDepthSteps &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i> total depth steps </i>&gt; </b>: 
    *         sets the total number of depth steps to be used in every Bellhop run if used woss::SSP are transformable, for given tx and rx woss::Location;
    *         @see woss::SSP::transform
    *  <li><b>setCustomTrasducerType &lt;<i>tx woss::Location*</i>&gt; &lt;<i>rx woss::Location*</i>&gt; &lt;<i>transducer type</i>&gt; 
              &lt;<i>vertical rotation</i>&gt; &lt;<i>horiz rotation</i>&gt; &lt;<i>mult constant</i>&gt; &lt;<i>add costant</i>&gt; </b>: 
    *         imports on the fly and sets CustomTransducer to be used in every Bellhop run, for given tx and rx woss::Location;
    *        <b>If the user wants dynamic orientation of the beam pattern, the user should call this command for every different
    *        <i>tx</i> location instead of using ALL_LOCATIONS</b>.
    *     @see woss::Transducer::import
    * </ul>
    * 
    * <b> PLEASE NOTE: if you want to set tx or rx woss::Location to represent all possible locations, use the value 0
    *     instead of passing a Tcl object; </b>
    *
    * Moreover it inherits all the OTcl method of WossCreator
    * 
    * 
    * @param argc number of arguments in <i>argv</i>
    * @param argv array of strings which are the command parameters (Note that argv[0] is the name of the object)
    * 
    * @return TCL_OK or TCL_ERROR whether the command has been dispatched succesfully or not
    * 
    **/
    virtual int command( int argc, const char*const* argv );

    
    protected:
    
    
    double debug_;
    
    double woss_debug_;
    
    double woss_clean_workdir_;
    
    int bellhop_arr_syntax_;

    int bellhop_shd_syntax_;
    
  };

  
}


#endif // WOSS_NS_MIRACLE_SUPPORT

#endif /* WOSS_BELLHOP_CREATOR_DEFINITIONS_TCL_H */

