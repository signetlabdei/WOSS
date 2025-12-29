# WOSS Definition classes

## Altimetry models
<a name="altimetry-models"></a>

WOSS allows the user to create and simulate sea wave models. The altimetry framework supports time evolution.

### Flat Model

The basic model is the Flat Altimetry model: it represents a flat sea surface. 

### Bretschneider Model

The Bretschneider Altimetry class implements the Bretschneider (ITTC two parameters) wave spectrum _[1]_.

Its input parameters are:
- _characteristic wave height [m] - ***H*** -_
- _wave period [s] - ***T*** -_

Additional parameters are:
- _range [m]_
- _range steps_
- _scenario depth [m]_

The latter are automatically set by the WOSS framework.

#### Examples:

![Example of a Bretschneider spectrum with T=1 and H=1](../../doc/imgs/altimetry_bret_T1.png "Example of a Bretschneider spectrum with T=1 and H=1")

### References:

1. G. J. Komen et al.,  _Dynamics and modeling of ocean waves._ Cambridge University Press, 1994.


## Time evolution modeling
<a name="time-evolution-modeling"></a>

Time evolution is supported by the following object/classes:
- woss::Woss - controls its evolution by querying a new SSP for the new time evolution quantum and by asking its woss::Altimetry to evolve.
- woss::Altimetry - performs an evolution of its mathematical model.
- woss::AltimBretschneider - generates a new realization of its statistical process.

Each class supports an independent time evolution quantum, measured in seconds, and the enabling/disabling of the feature, for the maximum flexibility.

The framework controls the time evolution through the following classes:
- woss::WossManager - the class now supports queries for a specific woss::Time object or for a specific number of seconds after the start of the simulation (woss::SimTime).
- woss::WossController - for each pair of tx-rx woss::Location the user can set a different woss::SimTime object. 
- woss::WossDbManager, woss::WossDb - all databases now supports queries for a specific woss::Time object
- woss::CustomDataTimeContainer - A new template for custom db data has been introduced to properly support time evolution. 
This is best used for time dependent custom SSP data, as each returned value is an linear interpolation of the SSP related to the two most close Time values.

For a clarifying example please check out the related tcl file in ***./samples/*** directory.


# Underwater Acoustic Transducers
<a name="underwater-acoustic-transducers"></a>

> **DISCLAIMER**  
> Federico Guerra and SIGNET lab do not endorse or recommend
> the use of any of the transducer listed below. \n The copyright and
> property of of each transducer design is retained by the respective
> owners. The specifications of each transducer have been inferred from
> the transducer data sheets courtesy of the respective owners. \n The list
> below is to be taken as a best-effort list, meaning that data has been
> taken from publicly avalaible documents, at the programmer's
> discretion. \n This list is NOT final and will undergo continuous
> changes, aimed at providing support for an increasing number of
> transducers in WOSS. \n If you would like any specific, not currently
> supported transducer to be included in WOSS, please feel free to
> contribute by sending transducer data (in the format mentioned below)
> to WOSS@guerra-tlc.com.

## Technical description

WOSS accurately simulates power consumption, Sound Pressure Level (SPL) output,
and vertical beam pattern of real acoustic transducers. 

Transducers can be imported into simulation, and their beam pattern can be rotated or modified with a additive and a multiplicative constant to better suit the user needs.

Furthermore, woss::Location, WossPosition and WossWpPosition support simulation of dynamic orientation of the associated transducer to better simulate how AUV movements impact on transmission.

See woss::Location, WossPosition, WossWpPosition

Finally UwMPhyBpskTransducer and WossMPhyBpsk now supports array of transducers to permit spectral optimizations with realistic power computations and ray attenuation.

See Example with waypoints 

## Transducer file format (ASCII and binary)

The format of transducer .txt or .dat file can be changed by the user (extending the woss::Transducer class) and is the followings (`format`, comments are inline and in _italics_):

`BT-25UF` _transducer's type name_  
`25000.0 10000.0 ` _resonance frequency, -3dB bandwith around the resonance frequency_  
`338 0.1` _max input power [W], duty cycle in (0,1]_    
`1.0 1.0 1.0 1.0 ` _TVR precision [hz], OCV precision [hz], conductance precision [hz], beam pattern precision [dec degrees]_  
`5 ` _total number of TVR values_  
`10000.0   120 ` _frequency [hz], TVR [dB re uPa/V @ 1m]_  
`23000.0   140 `  
`25000.0   140 `  
`38000.0   136 `  
`50000.0   135 `  
`4 ` _total number of OCV values_  
`10000.0   -197 ` _frequency [hz], OCV [dB re V/uPa]_  
`20000.0   -190 `  
`23000.0   -190 `  
`50000.0   -212 `  
`6 ` _total number of conductance values_  
`10000.0   10 ` _frequency [hz], conductance [uS]_  
`13000.0   180 `  
`23000.0   940 `  
`30000.0   410 `  
`40000.0   390 `  
`50000.0   590 `  
`14 ` _total number of angles in the vertical/horiz beam pattern_  
`-180  0 ` _angle between [-180, 180] in decimal degrees, [dB]_  
`-150 -1 ` _if the transducer has conical or != toroidal symmetry_  
`-120 -5 ` _the rotation axis has to lie on angle = 0 axis_  
`-105 -14 `  
`-75 -14 `  
`-60 -5 `  
`-30 -1 `  
`  0  0 `  
` 30 -2 `  
` 60 -4 `  
` 90 -5 `  
`120 -4 `  
`150 -2 `  
`180  0 `  

## Transducer's list

These are the currently available transducers in WOSS (in alphabetical order).

## BTech Acoustics, LLC

### BT-1RCL @ 28 kHz

#### Datasheet:

<a href="http://acomms.whoi.edu/documents/211001-SPC%20WH-BT-1%20Datasheet.pdf">Datasheet</a>

#### SPL:

![BTech BT-1RCL SPL](../../doc/imgs/bezier_BT_1RCL.png "BTech BT-1RCL SPL")

#### Vertical beam pattern:

![BTech BT-1RCL @ 20 kHz](../../doc/imgs/BTech_BT_1RCL_20kHz.png "BTech BT-1RCL @ 20 kHz")

![BTech BT-1RCL @ 25 kHz](../../doc/imgs/BTech_BT_1RCL_25kHz.png "BTech BT-1RCL @ 25 kHz")

![BTech BT-1RCL @ 30 kHz](../../doc/imgs/BTech_BT_1RCL_30kHz.png "BTech BT-1RCL @ 30 kHz")

### BT-2RCL @ 28 kHz

#### Datasheet:

<a href="http://acomms.whoi.edu/documents/211001-SPC%20WH-BT-1%20Datasheet.pdf">Datasheet</a>

#### SPL:

![BTech BT-2RCL SPL](../../doc/imgs/bezier_BT_2RCL.png "BTech BT-2RCL SPL")

#### Vertical beam pattern:

![BTech BT-2RCL @ 20 kHz](../../doc/imgs/BTech_BT_2RCL_20kHz.png "BTech BT-2RCL @ 20 kHz")

![BTech BT-2RCL @ 25 kHz](../../doc/imgs/BTech_BT_2RCL_25kHz.png "BTech BT-2RCL @ 25 kHz")

![BTech BT-2RCL @ 30 kHz](../../doc/imgs/BTech_BT_2RCL_30kHz.png "BTech BT-2RCL @ 30 kHz")


### BT-25UF @ 25 kHz

#### Datasheet:

<a href="http://acomms.whoi.edu/documents/211006-SPC%20BT25-UF%2025kHz%20Feed%20Through%20Transducer.pdf">Datasheet</a>

#### SPL:

![BTech BT-25UF SPL](../../doc/imgs/bezier_BT_25UF.png "BTech BT-25UF SPL")

#### Vertical beam pattern:

![BTech BT-25UF @ 20 kHz](../../doc/imgs/BTech_BT_25UF_20kHz.png "BTech BT-25UF @ 20 kHz")

![BTech BT-25UF @ 25 kHz](../../doc/imgs/BTech_BT_25UF_25kHz.png "BTech BT-25UF @ 25 kHz")

![BTech BT-25UF @ 30 kHz](../../doc/imgs/BTech_BT_25UF_30kHz.png "BTech BT-25UF @ 30 kHz")

## ITC

### ITC-2003 @ 5,9.5 kHz

#### Datasheet:

<a href="http://www.itc-transducers.com/pdf/2003.pdf">Datasheet</a>

#### SPL:

![ITC ITC-2003 SPL](../../doc/imgs/bezier_ITC_2003.png "ITC ITC-2003 SPL")

#### Vertical beam pattern:

![ITC ITC-2003 @ 5 kHz](../../doc/imgs/ITC_ITC_2003_5kHz.png "ITC ITC-2003 @ 5 kHz")


### ITC-2010 @ 1,2.5 kHz

#### Datasheet:

<a href="http://www.itc-transducers.com/pdf/2010.pdf">Datasheet</a>

#### SPL:

![ITC ITC-2010 SPL](../../doc/imgs/bezier_ITC_2010.png "ITC ITC-2010 SPL")

#### Vertical beam pattern:

![ITC ITC-2010 @ 2 kHz](../../doc/imgs/ITC_ITC_2010_2kHz.png "ITC ITC-2010 @ 2 kHz")


### ITC-2015 @ 1.8,2.8 kHz

#### Datasheet:

<a href="http://www.itc-transducers.com/pdf/2015.pdf">Datasheet</a>

#### SPL:

![ITC ITC-2015 SPL](../../doc/imgs/bezier_ITC_2015.png "ITC ITC-2015 SPL")

#### Vertical beam pattern:

![ITC ITC-2015 @ 2 kHz](../../doc/imgs/ITC_ITC_2015_2kHz.png "ITC ITC-2015 @ 2 kHz")


### ITC-2044 @ 8,14 kHz

#### Datasheet:

<a href="http://www.itc-transducers.com/pdf/2044.pdf">Datasheet</a>

#### SPL:

![ITC ITC-2044 SPL](../../doc/imgs/bezier_ITC_2044.png "ITC ITC-2044 SPL")

#### Vertical beam pattern:

![ITC ITC-2044 @ 10 kHz](../../doc/imgs/ITC_ITC_2044_10kHz.png "ITC ITC-2044 @ 10 kHz")


### ITC-2062 @ .44,1.4 kHz

#### Datasheet:

<a href="http://www.itc-transducers.com/pdf/2062.pdf">Datasheet</a>

#### SPL:

![ITC ITC-2062 SPL](../../doc/imgs/bezier_ITC_2062.png "ITC ITC-2062 SPL")

#### Vertical beam pattern:

![ITC ITC-2062 @ 0.44 kHz](../../doc/imgs/ITC_ITC_2062_0.44kHz.png "ITC ITC-2062 @ 0.44 kHz")


### ITC-3001 @ 17.5 kHz

#### Datasheet:

<a href="http://www.itc-transducers.com/pdf/3001.pdf">Datasheet</a>

#### SPL:

![ITC ITC-3001 SPL](../../doc/imgs/bezier_ITC_3001.png "ITC ITC-3001 SPL")

#### Vertical beam pattern:

![ITC ITC-3001 @ 17.5 kHz](../../doc/imgs/ITC_ITC_3001_17.5kHz.png "ITC ITC-3001 @ 17.5 kHz")


### ITC-3013 @ 12.5 kHz

#### Datasheet:

<a href="http://www.itc-transducers.com/pdf/3013.pdf">Datasheet</a>

#### SPL:

![ITC ITC-3013 SPL](../../doc/imgs/bezier_ITC_3013.png "ITC ITC-3013 SPL")

#### Vertical beam pattern:

![ITC ITC-3013 @ 10 kHz](../../doc/imgs/ITC_ITC_3013_10kHz.png "ITC ITC-3013 @ 10 kHz")


### ITC-3148 @ 12.5 kHz

#### Datasheet:

<a href="http://www.itc-transducers.com/pdf/3148.pdf">Datasheet</a>

#### SPL:

![ITC ITC-3148 SPL](../../doc/imgs/bezier_ITC_3148.png "ITC ITC-3148 SPL")

#### Vertical beam pattern:

![ITC ITC-3148 @ 8 kHz](../../doc/imgs/ITC_ITC_3148_8kHz.png "ITC ITC-3148 @ 8 kHz")


### ITC-3167 @ 11 kHz

#### Datasheet:

<a href="http://www.itc-transducers.com/pdf/3167.pdf">Datasheet</a>

#### SPL:

![ITC ITC-3167 SPL](../../doc/imgs/bezier_ITC_3167.png "ITC ITC-3167 SPL")

#### Vertical beam pattern:

![ITC ITC-3167 @ 10 kHz](../../doc/imgs/ITC_ITC_3167_10kHz.png "ITC ITC-3167 @ 10 kHz")

## ITT

### SB31CT @ 10 kHz

#### Datasheet:

<a href="http://uss.es.itt.com/as/docs/Acoustic%20Tranducer%20Model%20SB31CT%20-%20Deep%20Submergence%20General%20Purpose%20Transducer.pdf">Datasheet</a>

#### SPL:

![ITT SB31CT SPL](../../doc/imgs/bezier_ITT_SB31CT.png "ITT SB31CT SPL")

#### Vertical beam pattern:

![ITT SB31CT @ 10 kHz](../../doc/imgs/ITT_SB31CT_10kHz.png "ITT SB31CT @ 10 kHz")

## Neptune Sonar Limited

***NOTE:*** vertical beam patterns inferred from datasheets generic description. 
All Neptune transducers report only one value of conductance, SPL calculations could be inaccurate

### T54 @ 13 kHz

#### Datasheet:

<a href="http://www.neptune-sonar.co.uk/t54.asp">Datasheet</a>

#### SPL:

![Neptune T54 SPL](../../doc/imgs/bezier_Neptune_T54.png "Neptune T54 SPL")

#### Vertical beam pattern:

![Neptune T54 @ 13 kHz](../../doc/imgs/Neptune_T54_13kHz.png "Neptune T54 @ 13 kHz")

### T186 @ 17 kHz

#### Datasheet:

<a href="http://www.neptune-sonar.co.uk/t816.asp">Datasheet</a>

#### SPL:

![Neptune T186 SPL](../../doc/imgs/bezier_Neptune_T186.png "Neptune T186 SPL")

#### Vertical beam pattern:

![Neptune T186 @ 17 kHz](../../doc/imgs/Neptune_T186_17kHz.png "Neptune T186 @ 17 kHz")


### T204 @ 54 kHz

#### Datasheet:

<a href="http://www.neptune-sonar.co.uk/t204.asp">Datasheet</a>

#### SPL:

![Neptune T204 SPL](../../doc/imgs/bezier_Neptune_T204.png "Neptune T204 SPL")

#### Vertical beam pattern:

![Neptune T204 @ 54 kHz](../../doc/imgs/Neptune_T204_54kHz.png "Neptune T204 @ 54 kHz")


### T216 @ 58 kHz

#### Datasheet:

<a href="http://www.neptune-sonar.co.uk/t216.asp">Datasheet</a>

#### SPL:

![Neptune T216 SPL](../../doc/imgs/bezier_Neptune_T216.png "Neptune T216 SPL")

#### Vertical beam pattern:

![Neptune T216 @ 58 kHz](../../doc/imgs/Neptune_T216_58kHz.png "Neptune T216 @ 58 kHz")


### T217 @ 25 kHz

#### Datasheet:

<a href="http://www.neptune-sonar.co.uk/t217.asp">Datasheet</a>

#### SPL:

![Neptune T217 SPL](../../doc/imgs/bezier_Neptune_T217.png "Neptune T217 SPL")

#### Vertical beam pattern:

![Neptune T217 @ 25 kHz](../../doc/imgs/Neptune_T217_25kHz.png "Neptune T217 @ 25 kHz")

### T218 @ 22 kHz

#### Datasheet:

<a href="http://www.neptune-sonar.co.uk/t218.asp">Datasheet</a>

#### SPL:

![Neptune T218 SPL](../../doc/imgs/bezier_Neptune_T218.png "Neptune T218 SPL")

#### Vertical beam pattern:

![Neptune T218 @ 22 kHz](../../doc/imgs/Neptune_T218_22kHz.png "Neptune T218 @ 22 kHz")


## RESON


### TC1026 @ 36 kHz

#### Datasheet:

<a href="http://www.reson.com/%5Cgraphics%5Cdesign%5CSpec%20sheets%5CTransducers/TC1026.pdf">Datasheet</a>

#### SPL:

![RESON TC1026 SPL](../../doc/imgs/bezier_RESON_TC1026.png "RESON TC1026 SPL")

#### Vertical beam pattern:

![RESON TC1026 @ 36 kHz](../../doc/imgs/RESON_TC1026_36kHz.png "RESON TC1026 @ 36 kHz")
