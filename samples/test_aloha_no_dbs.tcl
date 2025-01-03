# WOSS - World Ocean Simulation System -
# 
# Copyright (C) 2009 Federico Guerra 
# and regents of the SIGNET lab, University of Padova
# 
# Author: Federico Guerra - federico@guerra-tlc.com
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation;
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

# This software has been developed by Federico Guerra
# and SIGNET lab, University of Padova, 
# in collaboration with the NATO Centre for Maritime Research 
# and Experimentation (http://www.cmre.nato.int ; 
# E-mail: pao@cmre.nato.int), 
# whose support is gratefully acknowledged.

# Module libraries

load libMiracle.so
load libmiraclecbr.so
load libmphy.so
load libmmac.so
load libMiracleBasicMovement.so
load libMiracleIp.so
load libMiracleIpRouting.so
load libmiracleport.so
load libmll.so

load libUwmStd.so
load libWOSS.so
load libWOSSPhy.so


# load libmlltracer.so
load libcbrtracer.so
load libUwmStdPhyBpskTracer.so

############################

#######################

set ns [new Simulator]
$ns use-Miracle

######################################
# global allocations & misc options
######################################

set opt(start_clock) [clock seconds]

set opt(start_lat)     42.59 
set opt(start_long)    10.125
set opt(nn)            4
set opt(pktsize)       512
set opt(cbr_period)    400
set opt(stoptime)      100000.0
set opt(rep_num)       10

#sink depth [m]
set opt(sink_depth)           5.5
set opt(sink_min_angle)       0.0
set opt(sink_max_angle)       90.0

set opt(node_min_angle)       -89.0
set opt(node_max_angle)       89.0

set opt(starttime)         0.1
set opt(txduration) [expr $opt(stoptime) - $opt(starttime)]

set opt(maxinterval_)   10.0

set opt(freq)           11500.0
set opt(bw)             5000.0
set opt(bitrate)        4800.0

set opt(tracefilename) "/tmp/${argv0}.tr"
# set opt(tracefilename) "/dev/null"
set opt(tracefile) [open $opt(tracefilename) w]

#set opt(cltracefilename) "/tmp/${argv0}.cltr"
set opt(cltracefilename) "/dev/null"
set opt(cltracefile) [open $opt(cltracefilename) w]


set opt(db_res_path) "."


set woss_utilities [new "WOSS/Utilities"]


WOSS/Definitions/RandomGenerator/NS2 set rep_number_ $opt(rep_num)
WOSS/Definitions/RandomGenerator/C   set seed_       $opt(rep_num)

#### we create the mandatory prototype objects that will be used by the whole framework.
#### We also do the mandatory intialization of the chosen random generator.
set ssp_creator         [new "WOSS/Definitions/SSP"]
set sediment_creator    [new "WOSS/Definitions/Sediment"]
set pressure_creator    [new "WOSS/Definitions/Pressure"]
set time_arr_creator    [new "WOSS/Definitions/TimeArr"]
#set altimetry_creator   [new "WOSS/Definitions/Altimetry/Bretschneider"]
set altimetry_creator   [new "WOSS/Definitions/Altimetry/Flat"]
set time_reference      [new "WOSS/Definitions/TimeReference/NS2"]
set transducer_creator  [new "WOSS/Definitions/Transducer"]
set rand_generator      [new "WOSS/Definitions/RandomGenerator/NS2"]
#set rand_generator      [new "WOSS/Definitions/RandomGenerator/C"]
$rand_generator initialize

#### we plug the chosen prototypes into the woss::DefinitionHandler
set def_handler [new "WOSS/Definitions/Handler"]
$def_handler setSSPCreator         $ssp_creator
$def_handler setSedimentCreator    $sediment_creator
$def_handler setPressureCreator    $pressure_creator
$def_handler setTimeArrCreator     $time_arr_creator
$def_handler setTransducerCreator  $transducer_creator
$def_handler setTimeReference      $time_reference
$def_handler setRandomGenerator    $rand_generator
$def_handler setAltimetryCreator   $altimetry_creator

#WOSS/Creator/Database/Textual/Results/TimeArr set debug           0
#WOSS/Creator/Database/Textual/Results/TimeArr set woss_db_debug   0
#WOSS/Creator/Database/Textual/Results/TimeArr set space_sampling 0.0

#set db_res_arr [new "WOSS/Creator/Database/Textual/Results/TimeArr"]
#$db_res_arr setDbPathName "${opt(db_res_path)}/test_aloha_no_dbs_res_arr.txt"


#### We now allocate a ResTimeArrBinDbCreator that will properly create and initialize
#### a Database that will store (in a binary file) all the channel responses
WOSS/Creator/Database/Binary/Results/TimeArr set debug           0
WOSS/Creator/Database/Binary/Results/TimeArr set woss_db_debug   0
WOSS/Creator/Database/Binary/Results/TimeArr set space_sampling  0.0

set db_res_arr [new "WOSS/Creator/Database/Binary/Results/TimeArr"]
$db_res_arr setDbPathName "${opt(db_res_path)}/test_aloha_no_dbs_res_arr.dat"


WOSS/Definitions/Altimetry/Flat set evolution_time_quantum   -1
WOSS/Definitions/Altimetry/Flat set range                    -1
WOSS/Definitions/Altimetry/Flat set total_range_steps        -1
WOSS/Definitions/Altimetry/Flat set depth                    0.0
set cust_altimetry   [new "WOSS/Definitions/Altimetry/Flat"]

#WOSS/Definitions/Altimetry/Bretschneider set evolution_time_quantum   -1
#WOSS/Definitions/Altimetry/Bretschneider set range                    -1
#WOSS/Definitions/Altimetry/Bretschneider set total_range_steps        -1
#WOSS/Definitions/Altimetry/Bretschneider set characteristic_height    1.5
#WOSS/Definitions/Altimetry/Bretschneider set average_period           3.0
#set cust_altimetry   [new "WOSS/Definitions/Altimetry/Bretschneider"]

$cust_altimetry setDebug 0

#### We create the mandatory woss::WossDbManager and we set a custom sediment and SSP for ALL
#### the channel computations involved.
#### we also create a custom bathymetry: it is a line that starts at ($opt(start_lat), $opt(start_long)), it is valid
#### for all bearings, and has four range/depth points. WossDbManager will provide bathymetry for (lat, long) points
#### selecting the closest point from its custom bathymetry. 
WOSS/Database/Manager set debug 0
set db_manager [new "WOSS/Database/Manager"]
$db_manager setCustomSediment   "Test Sedim" 1560 200 1.5 0.9 0.8 1.0
$db_manager setCustomAltimetry  $cust_altimetry
$db_manager setCustomSSP        0 "./ssp-test.txt"
$db_manager setCustomBathymetry $opt(start_lat) $opt(start_long) -500.0 4 0.0 100.0 500.0 200.0 1500.0 200.0 2500.0 100.0


WOSS/Creator/Bellhop set debug                        0.0
WOSS/Creator/Bellhop set woss_debug                   0.0
WOSS/Creator/Bellhop set woss_clean_workdir           1.0
WOSS/Creator/Bellhop set bellhop_arr_syntax           2
WOSS/Creator/Bellhop set evolution_time_quantum      -1.0
WOSS/Creator/Bellhop set total_runs                   5
WOSS/Creator/Bellhop set frequency_step               0.0
WOSS/Creator/Bellhop set total_range_steps            3000.0
WOSS/Creator/Bellhop set tx_min_depth_offset          0.0
WOSS/Creator/Bellhop set tx_max_depth_offset          0.0
WOSS/Creator/Bellhop set total_transmitters           1
WOSS/Creator/Bellhop set total_rx_depths              2
WOSS/Creator/Bellhop set rx_min_depth_offset          -0.1
WOSS/Creator/Bellhop set rx_max_depth_offset          0.1
WOSS/Creator/Bellhop set total_rx_ranges              2
WOSS/Creator/Bellhop set rx_min_range_offset          -0.1
WOSS/Creator/Bellhop set rx_max_range_offset          0.1
WOSS/Creator/Bellhop set total_rays                   0.0
WOSS/Creator/Bellhop set min_angle                    -180.0
WOSS/Creator/Bellhop set max_angle                    180.0
WOSS/Creator/Bellhop set ssp_depth_precision          1.0e-8
WOSS/Creator/Bellhop set normalized_ssp_depth_steps   100000


#### We set values for BellhopCreator. note the 0 0 in some commands: this means we're setting params 
#### for all tx and rx woss::Location
set woss_creator [new "WOSS/Creator/Bellhop"]
$woss_creator setWorkDirPath        "/dev/shm/woss/aloha_no_dbs/"
$woss_creator setBellhopPath        ""
$woss_creator setBellhopMode        0 0 "A"
$woss_creator setBeamOptions        0 0 "B"
$woss_creator setBathymetryType     0 0 "LL"
$woss_creator setBathymetryMethod   0 0 "D"
$woss_creator setAltimetryType      0 0 "L"
$woss_creator setSimulationTimes    0 0 1 12 2009 0 0 1 1 12 2009 0 0 1


### choose between single-threaded or multithreaded WossManager
### by uncomment/comment the followings lines

#WOSS/Manager/Simple set debug                       0
#WOSS/Manager/Simple set is_time_evolution_active  -1.0
#WOSS/Manager/Simple set space_sampling              0.0
#set woss_manager [new "WOSS/Manager/Simple"]

WOSS/Manager/Simple/MultiThread set debug                     0.0
WOSS/Manager/Simple/MultiThread set is_time_evolution_active  -1.0
WOSS/Manager/Simple/MultiThread set space_sampling            0.0
set woss_manager [new "WOSS/Manager/Simple/MultiThread"]
$woss_manager setConcurrentThreads 0


#### we create the mandatory woss::TransducerHandler
WOSS/Definitions/TransducerHandler set debug 0
set transducer_handler [new "WOSS/Definitions/TransducerHandler"]


#### we connect everything to the woss::WossController and we initialize it
WOSS/Controller set debug 0
set woss_controller [new "WOSS/Controller"]
#uncomment if you wish to store results to a db on filesystem
#$woss_controller setTimeArrResultsDbCreator  $db_res_arr
$woss_controller setWossDbManager            $db_manager
$woss_controller setWossManager              $woss_manager
$woss_controller setWossCreator              $woss_creator
$woss_controller setTransducerhandler        $transducer_handler
$woss_controller initialize


WOSS/PlugIn/ChannelEstimator set debug_ 0


WOSS/ChannelEstimator set debug_           0.0
WOSS/ChannelEstimator set space_sampling_  0.0
WOSS/ChannelEstimator set avg_coeff_       0.5
set channel_estimator [ new "WOSS/ChannelEstimator"]


WOSS/Module/Channel set channel_eq_snr_threshold_db_ 	 0
WOSS/Module/Channel set channel_symbol_resolution_     5e-3
###
### a negative value means we use only a channel tap
WOSS/Module/Channel set channel_eq_time_ 	             -1
WOSS/Module/Channel set debug_                    	   0
WOSS/Module/Channel set windspeed_                       0.0
WOSS/Module/Channel set shipping_                        0.0
WOSS/Module/Channel set practical_spreading_             1.75
WOSS/Module/Channel set prop_speed_                      1500.0

set channel [new "WOSS/Module/Channel"]
$channel setWossManager      $woss_manager
$channel setChannelEstimator $channel_estimator


WOSS/MPropagation set debug_ 0
set propagation [new "WOSS/MPropagation"]
$propagation setWossManager $woss_manager


set data_mask [new "MSpectralMask/Rect"]
$data_mask setFreq       $opt(freq)
$data_mask setBandwidth  $opt(bw)


Module/CBR set packetSize_          $opt(pktsize)
Module/CBR set period_              $opt(cbr_period)
Module/CBR set PoissonTraffic_      1


#### MaxTxSPL_dB_ was previously named MaxTxPower_dB_
#### MinTxSPL_dB_ was previously named MinTxPower_dB_
#### TxSPLMargin_dB_ was previously named TxPowerMargin_dB_
#### SPLOptimization_, CentralFreqOptimization_, BandwidthOptimization_ added ( == 0 off, != 0 on)
WOSS/Module/MPhy/BPSK  set debug_                     0
WOSS/Module/MPhy/BPSK  set AcquisitionThreshold_dB_   10.0 
WOSS/Module/MPhy/BPSK  set BitRate_                   $opt(bitrate)
WOSS/Module/MPhy/BPSK  set MaxTxSPL_dB_               190
WOSS/Module/MPhy/BPSK  set MinTxSPL_dB_               10
WOSS/Module/MPhy/BPSK  set MaxTxRange_                10000
WOSS/Module/MPhy/BPSK  set PER_target_                0.01
WOSS/Module/MPhy/BPSK  set TxSPLMargin_dB_            10
WOSS/Module/MPhy/BPSK  set RxSnrPenalty_dB_           -10.0
WOSS/Module/MPhy/BPSK  set SPLOptimization_           1
WOSS/Module/MPhy/BPSK  set CentralFreqOptimization_   0
WOSS/Module/MPhy/BPSK  set BandwidthOptimization_     0

########################################
# Random Number Generators
########################################

global def_rng
set def_rng [new RNG]
$def_rng default

for {set k 0} {$k < $opt(rep_num)} {incr k} {
     $def_rng next-substream
}

###############################
# Procedure for creating nodes
###############################

proc createNode { id }  {

     global channel propagation data_mask ns cbr position node port portnum ipr ipif channel_estimator ipr
     global phy_data posdb opt rvposx rvposy rvposz mhrouting mll mac woss_utilities woss_creator db_manager

     set node($id) [$ns create-M_Node $opt(tracefile) $opt(cltracefile)] 

     #### we don't need the IP routing anymore layer for basic sample.
     #### ChannelEstimatorPlugin now expects as input MAC addresses.
     #### MLL is therefore needed to properly map IP to MAC addresses.
     set cbr($id)       [new "Module/CBR"] 
     set port($id)      [new "Module/Port/Map"]
     set ipif($id)      [new "Module/IP/Interface"]
     set mll($id)       [new "Module/MLL"] 
     set mac($id)       [new "Module/MMac/ALOHA"] 
     set phy_data($id)  [new "WOSS/Module/MPhy/BPSK"]

     $node($id) addModule 6 $cbr($id)       0 "CBR"
     $node($id) addModule 5 $port($id)      0 "PRT"
     $node($id) addModule 4 $ipif($id)      0 "IPF"   
     $node($id) addModule 3 $mll($id)       0 "MLL"
     $node($id) addModule 2 $mac($id)       0 "MAC"
     $node($id) addModule 1 $phy_data($id)  0 "DPHY"

     $node($id) setConnection $cbr($id)  $port($id)       1
     $node($id) setConnection $port($id) $ipif($id)       0
     $node($id) setConnection $ipif($id) $mll($id)        0 
     $node($id) setConnection $mll($id)  $mac($id)        0
     $node($id) setConnection $mac($id)  $phy_data($id)   0
     $node($id) addToChannel $channel    $phy_data($id)   1

     set portnum($id) [$port($id) assignPort $cbr($id)]
     if {$id > 254} {
      puts "hostnum > 254!!! exiting"
      exit
     }

     $ipif($id) addr "1.0.0.${id}"
     #interface can reach directly all nodes if needed
     $ipif($id) subnet "0.0.0.0"

     set position($id) [new "WOSS/Position"]
     $node($id) addPosition $position($id)
 
     set coord_x    [ expr 1000.0 * $id ]
     set curr_lon   [ $woss_utilities getLongfromDistBearing $opt(start_lat) $opt(start_long) 90.0  $coord_x ]
     set curr_depth [expr 50.0 + $id]

     #### please Note: if you want to use the legacy underwatermiracle, you should use standard Position class and
     #### setX_ , setY_ , setZ_ ( negative values ) for space positioning!
     $position($id) setLatitude_  $opt(start_lat)
     $position($id) setLongitude_ $curr_lon
     $position($id) setAltitude_  [expr -1.0 * $curr_depth]

     #### we create the ChannelEstimatorPlugIn, we set the global ChannelEstimator and we map the MAC address
     #### to the WossPosition
     set ch_estimator_plugin($id) [ new "WOSS/PlugIn/ChannelEstimator"]
     $ch_estimator_plugin($id) setChannelEstimator $channel_estimator
     $ch_estimator_plugin($id) insertNode [$mac($id) addr] $position($id)
     $node($id) addPlugin $ch_estimator_plugin($id) 19 "CHE"

     puts  "node $id at ([$position($id) getLatitude_], [$position($id) getLongitude_], [$position($id) getAltitude_])\n"

     #### we set custom angles for this particular woss::Location and all rx woss::Location
     $woss_creator setCustomAngles $position($id) 0 $opt(node_min_angle) $opt(node_max_angle)

     set interf_data($id) [new "MInterference/MIV"]
     $interf_data($id) set maxinterval_ $opt(maxinterval_)
     $interf_data($id) set debug_       0

     $phy_data($id) setSpectralMask       $data_mask
     $phy_data($id) setPropagation        $propagation
     $phy_data($id) setInterference       $interf_data($id)
}


proc createSink { } {

     global channel propagation tone_mask data_mask ns cbr_sink position_sink node_sink port_sink portnum_sink 
     global phy_data_sink posdb_sink opt rvposx rvposy rvposz mhrouting mll_sink mac_sink ipr_sink ipif_sink 
     global woss_creator woss_utilities channel db_manager channel_estimator

     set node_sink [$ns create-M_Node $opt(tracefile) $opt(cltracefile)] 

     set port_sink      [new "Module/Port/Map"]
     set ipif_sink      [new "Module/IP/Interface"]
     set mll_sink       [new "Module/MLL"] 
     set mac_sink       [new "Module/MMac/ALOHA"] 
     set phy_data_sink  [new "WOSS/Module/MPhy/BPSK"]

     $node_sink addModule 5 $port_sink      0 "PRT"
     $node_sink addModule 4 $ipif_sink      0 "IPF"   
     $node_sink addModule 3 $mll_sink       0 "MLL"
     $node_sink addModule 2 $mac_sink       0 "MAC"
     $node_sink addModule 1 $phy_data_sink  0 "DPHY"

     for { set cnt 0} {$cnt < $opt(nn)} {incr cnt} {
       set cbr_sink($cnt)  [new "Module/CBR"] 

       $node_sink addModule 6 $cbr_sink($cnt) 0 "CBR"

       $node_sink setConnection $cbr_sink($cnt)  $port_sink     1

       set portnum_sink($cnt) [$port_sink assignPort $cbr_sink($cnt)]
       if {$cnt > 254} {
        puts "hostnum > 254!!! exiting"
        exit
       }
     }

     $node_sink setConnection $port_sink $ipif_sink       0
     $node_sink setConnection $ipif_sink $mll_sink        0 
     $node_sink setConnection $mll_sink  $mac_sink        0
     $node_sink setConnection $mac_sink  $phy_data_sink   0
     $node_sink addToChannel  $channel   $phy_data_sink   1

     $ipif_sink addr "1.0.0.253"
     #interface can reach directly all nodes if needed
     $ipif_sink subnet "0.0.0.0"

     set position_sink [new "WOSS/Position"]
     $node_sink addPosition $position_sink

     set coord_x [ expr (($opt(nn) - 1.0) * 1000.0) / 2.0 ]
     set curr_lon [ $woss_utilities getLongfromDistBearing $opt(start_lat) $opt(start_long) 90.0  $coord_x ]

     $position_sink setLatitude_  $opt(start_lat)
     $position_sink setLongitude_ $curr_lon
     $position_sink setAltitude_  [expr -1.0 * $opt(sink_depth)]

     set ch_estimator_plugin_sink [ new "WOSS/PlugIn/ChannelEstimator"]
     $ch_estimator_plugin_sink setChannelEstimator $channel_estimator
     $ch_estimator_plugin_sink insertNode [$mac_sink addr] $position_sink
     $node_sink addPlugin $ch_estimator_plugin_sink 19 "CHE"

     puts  "node_sink at ([$position_sink getLatitude_], [$position_sink getLongitude_], [$position_sink getAltitude_])"

     $woss_creator setCustomAngles $position_sink 0 $opt(sink_min_angle) $opt(sink_max_angle)

     set interf_data_sink [new "MInterference/MIV"]
     $interf_data_sink set maxinterval_ $opt(maxinterval_)
     $interf_data_sink set debug_       0

     $phy_data_sink setSpectralMask       $data_mask
     $phy_data_sink setPropagation        $propagation
     $phy_data_sink setInterference       $interf_data_sink

}

proc connectNodes {id1} {
    global ipif ipr portnum cbr cbr_sink ipif_sink portnum_sink ipr_sink

    $cbr($id1) set destAddr_ [$ipif_sink addr]
    $cbr($id1) set destPort_ $portnum_sink($id1)
    $cbr_sink($id1) set destAddr_ [$ipif($id1) addr]
    $cbr_sink($id1) set destPort_ $portnum($id1)  
}
###############################
# create nodes
###############################

for {set id 0} {$id < $opt(nn)} {incr id}  {
    createNode $id
}

createSink

###############################
# fill ARP tables
###############################

### we create the ARP table. mandatory it we want ChannelEstimator to work
for {set id1 0} {$id1 < $opt(nn)} {incr id1}  {
    for {set id2 0} {$id2 < $opt(nn)} {incr id2}  {
      $mll($id1) addentry [$ipif($id2) addr] [$mac($id2) addr]
    }   
    $mll($id1) addentry [$ipif_sink addr] [ $mac_sink addr]
    $mll_sink addentry [$ipif($id1) addr] [ $mac($id1) addr]
}

###############################
# setup flows
###############################


for {set id1 0} {$id1 < $opt(nn)} {incr id1}  {
    connectNodes $id1
}

###############################
# finish 
###############################

proc finish {} {
    global ns opt cbr mac propagation cbr_sink mac_sink phy_data phy_data_sink channel propagation
    global woss_manager db_manager

    puts "\n"

    for {set id3 0} {$id3 < $opt(nn)} {incr id3}  {

      set cbr_throughput   [$cbr_sink($id3) getthr]
      set cbr_delay        [$cbr_sink($id3) getftt]
      set cbr_per          [$cbr_sink($id3) getper]
      set cbr_pkts         [$cbr($id3) getsentpkts]
      set cbr_rxpkts       [$cbr_sink($id3) getrecvpkts]

      puts "cbr($id3)      app data pkts created       : $cbr_pkts"
      puts "cbr_sink($id3) app data pkts received      : $cbr_rxpkts"
      puts "cbr_sink($id3) throughput                  : $cbr_throughput"
      puts "cbr_sink($id3) normalized throughput       : [expr $cbr_throughput / (($opt(pktsize) * 8.0) / $opt(cbr_period))]"
      puts "cbr_sink($id3) delay                       : $cbr_delay"
      puts "cbr_sink($id3) packet error rate           : $cbr_per"
      puts ""
    }

    set opt(end_clock) [clock seconds]

    puts  "done in [expr $opt(end_clock) - $opt(start_clock)] seconds!"
    puts  "tracefile: $opt(tracefilename)"

    $ns flush-trace
    close $opt(tracefile)
    
    puts "Delete WOSS objects to force file operations"
    delete $woss_manager
    delete $db_manager
}

###################
# start CBR(s)
###################

for {set id1 0} {$id1 < $opt(nn)} {incr id1}  {
    $ns at $opt(starttime)   "$cbr($id1) start"
    $ns at $opt(stoptime)    "$cbr($id1) stop"
}

###################
# start simulation
###################

puts "\nSimulating...\n"

$ns at [expr $opt(stoptime) + 1000.0]  " finish; $ns halt"
$ns run

