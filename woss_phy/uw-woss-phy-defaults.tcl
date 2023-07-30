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


WOSS/Utilities set debug 0

WOSS/Definitions/Altimetry/Flat set evolution_time_quantum   -1
WOSS/Definitions/Altimetry/Flat set range                    -1
WOSS/Definitions/Altimetry/Flat set total_range_steps        -1
WOSS/Definitions/Altimetry/Flat set depth                    0.0

WOSS/Definitions/Altimetry/Bretschneider set evolution_time_quantum   -1
WOSS/Definitions/Altimetry/Bretschneider set range                    -1
WOSS/Definitions/Altimetry/Bretschneider set total_range_steps        -1
WOSS/Definitions/Altimetry/Bretschneider set depth                    100.0
WOSS/Definitions/Altimetry/Bretschneider set characteristic_height    1.5
WOSS/Definitions/Altimetry/Bretschneider set average_period           3.0

WOSS/Definitions/Handler set debug 0

WOSS/Definitions/TransducerHandler set debug 0

WOSS/Creator/Database/NetCDF/Bathymetry/GEBCO set debug           0
WOSS/Creator/Database/NetCDF/Bathymetry/GEBCO set woss_db_debug   0

WOSS/Creator/Database/NetCDF/Sediment/DECK41 set debug            0
WOSS/Creator/Database/NetCDF/Sediment/DECK41 set woss_db_debug    0

WOSS/Creator/Database/NetCDF/SSP/WOA2005/MonthlyAverage set debug          0
WOSS/Creator/Database/NetCDF/SSP/WOA2005/MonthlyAverage set woss_db_debug  0

WOSS/Creator/Database/NetCDF/SSP/WOA2013/MonthlyAverage set debug          0
WOSS/Creator/Database/NetCDF/SSP/WOA2013/MonthlyAverage set woss_db_debug  0

WOSS/Creator/Database/Textual/Results/TimeArr set debug           0
WOSS/Creator/Database/Textual/Results/TimeArr set woss_db_debug   0
WOSS/Creator/Database/Textual/Results/TimeArr set space_sampling  0

WOSS/Creator/Database/Textual/Results/Pressure set debug          0
WOSS/Creator/Database/Textual/Results/Pressure set woss_db_debug  0
WOSS/Creator/Database/Textual/Results/Pressure set space_sampling 0

WOSS/Creator/Database/Textual/Bathymetry/UMT_CSV set debug                0
WOSS/Creator/Database/Textual/Bathymetry/UMT_CSV set woss_db_debug        0
#Invalid Data to be filled by user in its example
WOSS/Creator/Database/Textual/Bathymetry/UMT_CSV set db_spacing             0
WOSS/Creator/Database/Textual/Bathymetry/UMT_CSV set total_northing_values  0
WOSS/Creator/Database/Textual/Bathymetry/UMT_CSV set total_easting_values   0
WOSS/Creator/Database/Textual/Bathymetry/UMT_CSV set range_easting_start    0
WOSS/Creator/Database/Textual/Bathymetry/UMT_CSV set range_easting_end      0
WOSS/Creator/Database/Textual/Bathymetry/UMT_CSV set range_northing_start   0
WOSS/Creator/Database/Textual/Bathymetry/UMT_CSV set range_northing_end     0
WOSS/Creator/Database/Textual/Bathymetry/UMT_CSV set approx_land_to_sea_surface 0

WOSS/Database/Manager set debug 0


WOSS/Creator/Bellhop set debug                        0.0
WOSS/Creator/Bellhop set woss_debug                   0.0
WOSS/Creator/Bellhop set woss_clean_workdir           0.0
WOSS/Creator/Bellhop set bellhop_arr_syntax           2
WOSS/Creator/Bellhop set bellhop_shd_syntax           1
WOSS/Creator/Bellhop set evolution_time_quantum      -1.0
WOSS/Creator/Bellhop set total_runs                   1
WOSS/Creator/Bellhop set frequency_step               0.0
WOSS/Creator/Bellhop set total_range_steps            1.0
WOSS/Creator/Bellhop set tx_min_depth_offset          0.0
WOSS/Creator/Bellhop set tx_max_depth_offset          0.0
WOSS/Creator/Bellhop set total_transmitters           1
WOSS/Creator/Bellhop set total_rx_depths              1
WOSS/Creator/Bellhop set rx_min_depth_offset          0.0
WOSS/Creator/Bellhop set rx_max_depth_offset          0.0
WOSS/Creator/Bellhop set total_rx_ranges              1
WOSS/Creator/Bellhop set rx_min_range_offset          0.0
WOSS/Creator/Bellhop set rx_max_range_offset          0.0
WOSS/Creator/Bellhop set total_rays                   0.0
WOSS/Creator/Bellhop set min_angle                    -10.0
WOSS/Creator/Bellhop set max_angle                    10.0
WOSS/Creator/Bellhop set ssp_depth_precision          1.0e-6
WOSS/Creator/Bellhop set normalized_ssp_depth_steps   20
WOSS/Creator/Bellhop set box_depth                    -3000.0
WOSS/Creator/Bellhop set box_range                    -3000.0


WOSS/Manager/Simple set debug                     0.0
WOSS/Manager/Simple set is_time_evolution_active -1.0
WOSS/Manager/Simple set space_sampling            0.0


WOSS/Controller set debug 0.0


WOSS/Definitions/RandomGenerator/C set seed_ 1

#WOSS/Manager/Simple/MultiThread set is_time_evolution_active -1.0
#WOSS/Manager/Simple/MultiThread set debug                     0.0
#WOSS/Manager/Simple/MultiThread set space_sampling            0.0

PacketHeaderManager set tab_(PacketHeader/WOSS)    1


#### MaxTxSPL_dB_ was previously named MaxTxPower_dB_
#### MinTxSPL_dB_ was previously named MinTxPower_dB_
#### TxSPLMargin_dB_ was previously named TxPowerMargin_dB_
WOSS/Module/MPhy/BPSK set MaxTxSPL_dB_              190
WOSS/Module/MPhy/BPSK set MinTxSPL_dB_              10
WOSS/Module/MPhy/BPSK set RxSnrPenalty_dB_  	      -10
WOSS/Module/MPhy/BPSK set TxSPLMargin_dB_ 	        10
WOSS/Module/MPhy/BPSK set ConsumedEnergy_ 	        0
WOSS/Module/MPhy/BPSK set debug_	                  0
WOSS/Module/MPhy/BPSK set SPLOptimization_          0
WOSS/Module/MPhy/BPSK set CentralFreqOptimization_  0
WOSS/Module/MPhy/BPSK set BandwidthOptimization_    0
WOSS/Module/MPhy/BPSK set MaxTxRange_           10000
WOSS/Module/MPhy/BPSK set PER_target_            0.01


WOSS/Module/Channel set channel_max_distance_            -1.0
WOSS/Module/Channel set channel_eq_snr_threshold_db_     -100.0
WOSS/Module/Channel set channel_symbol_resolution_       -1.0
WOSS/Module/Channel set channel_eq_time_                 -1.0
WOSS/Module/Channel set debug_                           0.0
WOSS/Module/Channel set windspeed_                       0.0
WOSS/Module/Channel set shipping_                        0.0
WOSS/Module/Channel set practical_spreading_             1.75
WOSS/Module/Channel set prop_speed_                      1500.0

WOSS/ChannelEstimator set debug_           0.0
WOSS/ChannelEstimator set space_sampling_  0.0
WOSS/ChannelEstimator set avg_coeff_       0.5

WOSS/PlugIn/ChannelEstimator set debug_    0.0


WOSS/Definitions/RandomGenerator/NS2 set rep_number_ 5 


WOSS/Position set compDistance_                0.0
WOSS/Position set verticalOrientation_         0.0
WOSS/Position set minVerticalOrientation_    -90.0
WOSS/Position set maxVerticalOrientation_     90.0


WOSS/Position/WayPoint set time_threshold_             1e-5


