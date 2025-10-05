# Test framework

WOSS has the ability of running self tests in order to verify its features on the installed system.

The class woss::WossTest is the base class that allow the user to define new tests, while taking care of creating all the required objects and their configuration.

Tests can be compiled and run with the syntax `make check`

## Command line acoustic channel simulation

From WOSS 2.0.0 it is also possible to generate acoustic channel simulations via command line.

The first thing to do is to build the script by running the command:

`make check`

This will build the executable `woss-bellhop-script-bin` in the `tests\.lib` sub directory.

The executable binary acceps the following command line syntax:

`woss-bellhop-script-bin <debug_level> <seed> <db_path> <res_path> <tx_lat> <tx_lon> <tx_depth> <rx_lat> <rx_lon> <rx_depth> <frequency> <custom_bathy> <custom_sedim> <custom_ssp>`

Where:
- `<debug_level>`: mandatory. Enable test debug prints. Allowed values, 0 (disabled) or > 0 (test debug enabled) or > 10 (main woss debug enabled) or > 100 (all woss debug enabled)
- `<seed>`: mandatory. The seed of the random generator used by the simulation. Same seed will get same results.
- `<db_path>`: mandatory. The path to the WOSS Dbs.
- `<bathy_db_file_path>`: mandatory. Sub path of the bathymetry NetCDF db file.
- `<ssp_db_file_path>`: mandatory. Sub path of the SSP NetCDF db file.
- `<res_path>`: mandatory. The path where the channel results will be stored.
- `<tx_lat>`: mandatory. Transmitter latitude, in decimal degrees.
- `<tx_lon>`: mandatory. Transmitter longitude, in decimal degrees.
- `<tx_depth>`: mandatory. Trasnmitter depth, in meters.
- `<rx_lat>`: mandatory. Receiver latitude, in decimal degrees.
- `<rx_lon>`: mandatory. Receiver longitude, in decimal degrees.
- `<rx_depth>`: mandatory. Receiver depth, in meters.
- `<frequency>`: mandatory. Channel frequency, in hZ.
- `<custom_bathy>`: optional. Custom bathymetry that will be used by the channel simulation.  
  String syntax: `"tot_ranges|range1[m]|depth1[m]|....|rangeN[m]|depthN[m]"`  
  Use `"#"` to skip this input parameter and go to the next ones.  
  If the parameter is not present, the bathymetry from the GEBCO Dd will be used instead.
- `<custom_sedim>`: optional. Custom sediment that will be used by the channel simulation.  
  String syntax: `"SedimName|vel_comp[m/s]|vel_shear[m/s]|dens[g/cm3]|atten_comp[db/wavelength]|atten_shear[db/wavelength]|bottom depth[m]"`  
  Use `"#"` to skip this input parameter and go to the next ones.  
  If the parameter is not present, the sediment from DECK 41 Db will be used instead.
- `<custom_ssp>`
  String syntax: `"tot_depths|depth1[m]|speed1[m/s]|...|depthN[m]|speedN[m/s]"`
  Use `"#"` to skip this input parameter.  
  If the parameter is not present, the SSP from WOA Db will be used instead.

### Syntax examples:

- `./woss-bellhop-script-bin 1 1 /home/user/ns/ocean_database/dbs/ /bathymetry/GEBCO_2025_sub_ice.nc /ssp/WOA2023/WOA2023_SSP_June.nc ./bh_script_out/ 42.59 10.125 60 42.59 10.131 2 1500`
- `./woss-bellhop-script-bin 1 1 /home/user/ns/ocean_database/dbs/ /bathymetry/GEBCO_2025_sub_ice.nc /ssp/WOA2023/WOA2023_SSP_June.nc ./bh_script_out/ 42.59 10.125 60 42.59 10.131 2 1500 "5|0|100|300|200|400|500|600|550|800|760"`
- `./woss-bellhop-script-bin 1 1 /home/user/ns/ocean_database/dbs/ /bathymetry/GEBCO_2025_sub_ice.nc /ssp/WOA2023/WOA2023_SSP_June.nc ./bh_script_out/ 42.59 10.125 60 42.59 10.131 2 1500 "#" "TestSediment|1560.0|200.0|1.5|0.9|0.8|300.0"`
- `./woss-bellhop-script-bin 1 1 /home/user/ns/ocean_database/dbs/ /bathymetry/GEBCO_2025_sub_ice.nc /ssp/WOA2023/WOA2023_SSP_June.nc ./bh_script_out/ 42.59 10.125 60 42.59 10.131 2 1500 "5|0|100|300|200|400|500|600|550|800|760" "TestSediment|1560.0|200.0|1.5|0.9|0.8|300.0" "12|0|1508.42|10|1508.02|20|1507.71|30|1507.53|50|1507.03|75|1507.56|100|1508.08|125|1508.49|150|1508.91|200|1509.75|250|1510.58|300|1511.42"`