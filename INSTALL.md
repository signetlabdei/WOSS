# Installation instructions

> **Note**  
> No super-user credentials are needed for any of these installations.

## Compatibilities

The current WOSS version has the following [compatibilities](https://woss.dei.unipd.it/#libraries)

- **Acoustic Toolbox**: December 2024
- **HDF5 library**: v1.14.6
- **NetCDF C library**: v4.9.3
- **NetCDF4 C++ library**: v4.3.1
- **GEBCO databases**: 1D and 2D, 2008, 2014, 2019, 2020, 2022, 2023, 2024, 2025

## Requirements

From WOSS 2.0.0 the support of C++17 is mandatory and it is enforced by the build system.

Download a recommended version from [Acoustic Toolbox library](https://woss.dei.unipd.it/#libraries)
and follow installation instructions. The directory path that contains
the binaries should be in the `$PATH` environment.

woss::BellhopArrSyntax::BELLHOP_CREATOR_ARR_FILE_SYNTAX_1 is compliant with Bellhop arr file syntax of the acoustic toolbox library up until the 16 Aug 2016 version.

woss::BellhopArrSyntax::BELLHOP_CREATOR_ARR_FILE_SYNTAX_2 is compliant with Bellhop arr file syntax of the acoustic toolbox library >= 31 March 2019 version. This is the new factory value. 

WOSS is now requiring NetCDF4 with HDF5 support in order to be able to read the latest GEBCO dataset.

## NetCDF installation

If you wish to compile NetCDF4 software features, download the recommended HDF5, NetCDF4 for C and C++ libraries from https://woss.dei.unipd.it/#libraries  
HDF5 and NetCDF4 have to be built with support for dynamic libraries.  
Please refer to NetCDF documentation for comprehensive set of installation instructions.  
Please note that the option `--prefix` is strongly suggested.  

A simple installation script is given here as example: 
- install the HDF5 library with
  - `./configure --enable-shared --prefix=<optional_netcdf4_install_path>`
  - `make`
  - `make check`
-  - `make install` (should any test fail, based on test failure severity you could still give the install command)
- install the NETCDF C library with
  - `./configure --prefix=<optional_netcdf4_install_path> --disable-dap --disable-byterange --enable-shared --enable-netcdf-4 --enable-logging CPPFLAGS="$CPPFLAGS -I<optional_netcdf4_install_path>/include" LDFLAGS="$LDFLAGS -L<optional_netcdf4_install_path>/lib"`
  - `make` 
  - `make check`
  - `make install` (should any test fail, based on test failure severity you could still give the install command)
- install the NETCDF4 C++ library with
  - `./configure --prefix=<optional_netcdf4_install_path> --enable-shared CPPFLAGS="$CPPFLAGS -I<optional_netcdf4_install_path>/include" LDFLAGS="$LDFLAGS -L<optional_netcdf4_install_path>/lib"`
  - `make`
  - `make check`
  - `make install` (should any test fail, based on test failure severity you could still give the install command)

## NS-Miracle installation

If you want to compile NS-Miracle features download the latest version of NS2 allinone and of NS-Miracle from the official [git repository](https://github.com/signetlabdei/nsmiracle):

Read the documentation provided in the above link for installation instructions.

## WOSS Databases installation

If you want to use WOSS with world data, download and extract the provided databases. 

Download the GEBCO 2025 2D Fifteen seconds zip archive from https://woss.dei.unipd.it/#libraries or any of the previous datasets from https://www.gebco.net/data_and_products/historical_data_sets
and put them in the same directory.

## How to install the WOSS library

1. extract the compressed file in a directory of your choice;
2. open a terminal and cd into that directory;
3. run `./autogen.sh`
4. run `./configure` with the following options:
  - `--with-ns-allinone=<ns2-allinone_path>` ***optional*** see [Examples](#examples) below in order to avoid gcc-13 compilation error.
  - `--with-nsmiracle=<ns-miracle_path>` ***optional*** see [Examples](#examples) below in order to avoid gcc-13 compilation error.
  - `--with-netcdf4=<NetCDF4_installation_path>` ***optional***, the same mentioned in the 
    Requirements section, if NetCDF4
    was installed with no `--prefix`, the default path SHOULD be `/usr/local`)
  - `--prefix=<path_where_libraries_will_be_installed>` ***optional***
   this path should be the same one used with NS-Miracle installation.
   Please refer to NS-Miracle documentation for more info
5. run `make`
6. run `make check` in order to run WOSS tests
7. run `make install`

### Examples

The following line would install WOSS with NetCDF4, but with no NS2/NS-Miracle support (stand-alone library).

`./configure --with-netcdf4=<NetCDF4_install_path> --prefix=<path_where_libraries_will_be_installed>`

The next line would install WOSS with NetCDF4, NS2 and NS-Miracle support and it removes the gcc-13 overloaded-virtual warning

`CXXFLAGS="$CXXFLAGS -Wno-overloaded-virtual" ./configure --with-netcdf4=<NetCDF4_install_path> --with-ns-allinone=<ns2-allinone_path> --with-nsmiracle=<ns-miracle_path> --prefix=<path_where_libraries_will_be_installed>`
