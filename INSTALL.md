# Installation instructions

> **Note**  
> No super-user credentials are needed for any of these installations.

## Compatibilities

The current WOSS version has the following [compatibilities](https://woss.dei.unipd.it/#libraries)

- **Acoustic Toolbox**: July 2026
- **HDF5 library**: v2.1.1
- **NetCDF C library**: v4.10.1
- **NetCDF4 C++ library**: v4.3.1
- **GEBCO databases**: 1D and 2D, 2008, 2014, 2019, 2020, 2022, 2023, 2024, 2025, 2026

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
  - `cmake --preset ci-StdShar-GNUC --fresh -DCMAKE_INSTALL_PREFIX=<optional_netcdf4_install_path> -DHDF5_BUILD_CPP_LIB:BOOL=ON -DHDF5_BUILD_JAVA:BOOL=OFF -DHDF5_BUILD_FORTRAN:BOOL=OFF -DHDF5_BUILD_EXAMPLES:BOOL=OFF -DHDF5_BUILD_TOOLS:BOOL=OFF -DBUILD_TESTING:BOOL=ON`
  - `cmake --build --preset ci-StdShar-GNUC`
  - `ctest --preset ci-StdShar-GNUC --output-on-failure` (should any test fail, based on test failure severity you could still give the install command)
  - `cmake --install <path_to_hdf5_source_parent_dir>/build/ci-StdShar-GNUC`
- install the NETCDF C library with
  - `cmake -B ./build -DCMAKE_INSTALL_PREFIX=<optional_netcdf4_install_path> -DBUILD_SHARED_LIBS=ON -DENABLE_NETCDF_4=ON -DENABLE_LOGGING=ON -DENABLE_DAP=OFF -DENABLE_BYTERANGE=OFF -DENABLE_LIBXML2=OFF -DHDF5_ROOT=<optional_netcdf4_install_path> -DCMAKE_INCLUDE_PATH=<optional_netcdf4_install_path>/include -DCMAKE_LIBRARY_PATH=<optional_netcdf4_install_path>/lib`
  - `cmake --build ./build`
  - `cmake --build ./build --target test` (should any test fail, based on test failure severity you could still give the install command)
  - `cmake –-build ./build –-target install`
- install the NETCDF4 C++ library with
  - `./configure --prefix=<optional_netcdf4_install_path> --enable-shared CPPFLAGS="$CPPFLAGS -I<optional_netcdf4_install_path>/include" LDFLAGS="$LDFLAGS -L<optional_netcdf4_install_path>/lib"`
  - `make`
  - `make check` (should any test fail, based on test failure severity you could still give the install command)
  - `make install` 

## NS-Miracle installation

If you want to compile NS-Miracle features download the latest version of NS2 allinone and of NS-Miracle from the official [git repository](https://github.com/signetlabdei/nsmiracle):

Read the documentation provided in the above link for installation instructions.

## WOSS Databases installation

If you want to use WOSS with world data, download and extract the provided databases. 

Download the GEBCO 2025 2D Fifteen seconds zip archive from https://woss.dei.unipd.it/#libraries or any of the previous datasets from https://www.gebco.net/data_and_products/historical_data_sets
and put them in the same directory.

## How to install the WOSS library with cmake (supersedes autotools)

WOSS can also be built using CMake as an alternative to the autotools build system.

### Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Optional: Doxygen (for documentation generation)

### Build Steps

1. Extract the compressed file in a directory of your choice
2. Open a terminal and cd into that directory
3. Configure with CMake using the following options:
   - `-DNETCDF_PATH=<netcdf_install_path>` ***optional*** - Path to legacy NetCDF installation (provides netcdfcpp.h, libnetcdf.so, libnetcdf_c++.so). **Mutually exclusive with `-DNETCDF4_PATH`**
   - `-DNETCDF4_PATH=<netcdf4_install_path>` ***optional*** - Path to NetCDF4 installation (provides ncFile.h, libnetcdf.so, libnetcdf_c++4.so). **Mutually exclusive with `-DNETCDF_PATH`**
   - `-DNSALLINONE_PATH=<ns2-allinone_path>` ***optional*** - Path to NS2 allinone installation (provides tclcl.h)
   - `-DNSMIRACLE_PATH=<nsmiracle_path>` ***optional*** - Path to NS-Miracle installation (provides module.h)
   - `-DCMAKE_BUILD_TYPE=<build_type>` ***optional*** Build type, choose between: `Release`, `RelWithDebInfo`, `MinSizeRel`, `Debug`
      - `Release` adds the `-DNDEBUG` flag. All asserts are *removed*.
      - `RelWithDebInfo` is the default if not given.
      - all the other build types keep the assert active.
      - ***Suggestion***: run a specific scenario with `RelWithDebInfo` then move to `Release` once the simulation scenario is working.
   - `-DCMAKE_INSTALL_PREFIX=<install_path>` ***optional*** - Path where libraries and headers will be installed (equivalent to `--prefix` in autotools)
   - `--fresh` ***optional*** - Cleans the build directory before reconfiguring it
   - `-G "Unix Makefiles"` ***optional*** - configures CMake to build the project with *make*
   - `-G Ninja` ***optional*** - configures Cmake to build the project with *ninja*
4. Build:
   ```bash
   cmake -B ./build [options]
   cmake --build ./build -j$(nproc)
   ```
5. Run tests:
   ```bash
   ctest --test-dir ./build --output-on-failure
   ```
6. Install:
   ```bash
   cmake --install ./build
   ```
7. Clean the build directory without reconfiguring (when required):
   ```bash
   cmake --build ./build -t clean
   ```
7. Build the documentation with
   ```bash
   doxygen
   ```
   The HTML documentation will be generated in the `doc/html` directory. 

### CMake Build Examples

**Standalone build with legacy NetCDF and ninja support:**

```bash
cd /path/to/woss
cmake -B ./build -DNETCDF_PATH=<netcdf_install_path> -DCMAKE_INSTALL_PREFIX=<install_path> -G "Ninja"
cmake --build -B ./build -j$(nproc)
ctest --test-dir ./build --output-on-failure
cmake --install ./build
```

**Standalone build with NetCDF4 and ninja support:**

```bash
cd /path/to/woss
cmake -B ./build -DNETCDF4_PATH=<netcdf4_install_path> -DCMAKE_INSTALL_PREFIX=<install_path> -G "Ninja"
cmake --build ./build -j$(nproc)
ctest --test-dir ./build --output-on-failure
cmake --install ./build
```

**Full build with NetCDF4, NS2, NS-Miracle, build type and ninja support:**

```bash
cd /path/to/woss
cmake -B ./build \
    -DNETCDF4_PATH=<netcdf4_install_path> \
    -DNSALLINONE_PATH=<ns2-allinone_path> \
    -DNSMIRACLE_PATH=<nsmiracle_path> \
    -DCMAKE_INSTALL_PREFIX=<install_path> \
    -DCMAKE_BUILD_TYPE=<build_type> \
    -G "Ninja"
cmake --build ./build -j$(nproc)
ctest --test-dir ./build --output-on-failure
cmake --install ./build
```

**Clean rebuild:**

```bash
rm -rf <pathtowoss>/build
cmake -B ./build -DNETCDF4_PATH=<netcdf4_install_path> -DNSALLINONE_PATH=<ns2-allinone_path> -DNSMIRACLE_PATH=<nsmiracle_path> --fresh -G "Ninja"
cmake --build ./build -j$(nproc)
ctest --test-dir ./build --output-on-failure
cmake --install ./build
```

### Notes

- **NETCDF_PATH and NETCDF4_PATH are mutually exclusive** - you must choose one or the other, not both
- When using CMake with legacy NetCDF, the netCDF C++ library (`libnetcdf_c++.so`) is automatically linked
- When using CMake with NetCDF4, the netCDF C++4 library (`libnetcdf_c++4.so`) is automatically linked
- The `-Wno-unused-variable` flag is automatically added when NetCDF4 headers have unused variable warnings
- Both build systems produce the same libraries

## How to install the WOSS library with autotools (obsolete)

1. extract the compressed file in a directory of your choice;
2. open a terminal and cd into that directory;
3. run `./autogen.sh`
4. run `./configure` with the following options:
  - `--with-ns-allinone=<ns2-allinone_path>` ***optional*** see [Examples](#examples) below in order to avoid gcc-13 compilation error.
  - `--with-nsmiracle=<ns-miracle_path>` ***optional*** see [Examples](#examples) below in order to avoid gcc-13 compilation error.
  - `--with-netcdf4=<NetCDF4_installation_path>` ***optional***, the same mentioned in the Requirements section, if NetCDF4 was installed with no `--prefix`, the default path SHOULD be `/usr/local`
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

## CMake vs Autotools Comparison

| Feature | Autotools | CMake |
|---------|-----------|-------|
| NetCDF Support | `--with-netcdf` | `-DNETCDF_PATH` |
| NetCDF4 Support | `--with-netcdf4` | `-DNETCDF4_PATH` |
| NS2 Support | `--with-ns-allinone` | `-DNSALLINONE_PATH` |
| NS-Miracle Support | `--with-nsmiracle` | `-DNSMIRACLE_PATH` |
| Install Prefix | `--prefix` | `-DCMAKE_INSTALL_PREFIX` |
| Run Tests | `make check` | `ctest --test-dir ./build --output-on-failure` |
| Build   | `make`  | `cmake --build ./build` |
| Install | `make install` | `cmake --install ./build` |
| Clean | `make clean` | `cmake --build ./build -t clean` |
