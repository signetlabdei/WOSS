# WOSS - World Ocean Simulation System -
# 
# Copyright (C) 2009 2026 Federico Guerra 
# and regents of the SIGNET lab, University of Padova
# 
# Author: Federico Guerra - WOSS@guerra-tlc.com
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

# ============================================================================
# Project configuration and options
# ============================================================================

cmake_minimum_required(VERSION 3.10)

project(WOSS
    VERSION 2.1.0
    DESCRIPTION "World Ocean Simulation System"
    LANGUAGES CXX
)

set(PROJECT_BUG_REPORT_URL "WOSS@guerra-tlc.com")

set(CMAKE_CXX_STANDARD 17)

# Compiler warning flags (equivalent to -Wall -Werror)
add_compile_options(-Wall -Werror)

# ------------------------------------------------------------------------------
# Build Type (Fallback on Release)
# ------------------------------------------------------------------------------

set(ALLOWED_BUILD_TYPES "Debug" "Release" "RelWithDebInfo" "MinSizeRel")

# Sets RelWithDebInfo as default if not configured
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING 
        "Choose a build type between: Debug, Release, RelWithDebInfo, MinSizeRel" FORCE)
endif()

# Configures GUI/IDE (cmake-gui, ccmake) options
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${ALLOWED_BUILD_TYPES})

# Validate the cmdline input
if(CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE IN_LIST ALLOWED_BUILD_TYPES)
        message(FATAL_ERROR 
            "Invalid build type '${CMAKE_BUILD_TYPE}'.\n"
            "Allowed types: ${ALLOWED_BUILD_TYPES}"
        )
    endif()
endif()

# We need the assert() to be compiled at all levels != "Release", therefore we add -UNDEBUG
if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-UNDEBUG)
endif()

# ============================================================================
# Config report
# ============================================================================

# get the CXX flags for the current build type
string(TOUPPER "${CMAKE_BUILD_TYPE}" UPPER_BUILD_TYPE)
set(CURRENT_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${UPPER_BUILD_TYPE}}")

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(-UNDEBUG)
    set(ASSERT_STATUS "Active")
else()
    set(ASSERT_STATUS "Not Active")
endif()

# Stampa il resoconto formattato a schermo
message(STATUS "----------------------------------------------------")
message(STATUS "Project name: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "bug e-mail: ${PROJECT_BUG_REPORT_URL}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS " C++ build config:")
message(STATUS "   - Build Type:       ${CMAKE_BUILD_TYPE}")
message(STATUS "   - Compiler:         ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "   - Generic Flags:    ${CMAKE_CXX_FLAGS}")
message(STATUS "   - Specific Flags:   ${CMAKE_CXX_FLAGS_${UPPER_BUILD_TYPE}}")
message(STATUS "   - Assert Status:    ${ASSERT_STATUS}")
message(STATUS "----------------------------------------------------")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(WOSS_ENABLE_MULTITHREAD "Enable multi-thread support" ON)

# ============================================================================
# NETCDF support (equivalent to AC_ARG_WITH_NETCDF and AC_CHECK_NETCDF in m4)
# ============================================================================

set(NETCDF_FOUND FALSE)
if(NETCDF_PATH)
    # Look for netcdfcpp.h header (netcdf C++ API header)
    find_path(NETCDF_INCLUDE_DIR netcdfcpp.h
        PATHS ${NETCDF_PATH}/include /usr/include /usr/local/include
        NO_DEFAULT_PATH
    )

    # Look for netcdf library (netcdf C library)
    find_library(NETCDF_LIBRARY netcdf
        PATHS ${NETCDF_PATH}/lib /usr/lib /usr/local/lib
        NO_DEFAULT_PATH
    )

    # Look for netcdf_c++ library (netcdf C++ API library)
    find_library(NETCDF_CXX_LIBRARY netcdf_c++
        PATHS ${NETCDF_PATH}/lib /usr/lib /usr/local/lib
        NO_DEFAULT_PATH
    )

    if(NETCDF_INCLUDE_DIR AND NETCDF_LIBRARY AND NETCDF_CXX_LIBRARY)
        set(NETCDF_FOUND TRUE)
        message(STATUS "NETCDF support will be built.")
        message(STATUS "  NETCDF_INCLUDE_DIR: ${NETCDF_INCLUDE_DIR}")
        message(STATUS "  NETCDF_LIBRARY: ${NETCDF_LIBRARY}")
        message(STATUS "  NETCDF_CXX_LIBRARY: ${NETCDF_CXX_LIBRARY}")
    else()
        set(NETCDF_FOUND FALSE)
        message(STATUS "NETCDF support will NOT be built.")
    endif()
else()
    message(STATUS "NETCDF support will NOT be built.")
endif()

# ============================================================================
# Find NETCDF4 (equivalent to AC_ARG_WITH_NETCDF4 and AC_CHECK_NETCDF4 in m4)
# ============================================================================

set(NETCDF4_FOUND FALSE)
if(NETCDF4_PATH)
    # Look for ncFile.h header (netcdf4 C++ API header)
    find_path(NETCDF4_INCLUDE_DIR ncFile.h
        PATHS ${NETCDF4_PATH}/include /usr/include /usr/local/include
        NO_DEFAULT_PATH
    )

    # Look for netcdf_c++4 library (netcdf4 C++ API library)
    find_library(NETCDF4_CXX_LIBRARY netcdf_c++4
        PATHS ${NETCDF4_PATH}/lib /usr/lib /usr/local/lib
        NO_DEFAULT_PATH
    )

    # Look for netcdf library (netcdf C library, required by netcdf4)
    find_library(NETCDF4_LIBRARY netcdf
        PATHS ${NETCDF4_PATH}/lib /usr/lib /usr/local/lib
        NO_DEFAULT_PATH
    )

    # Look for hdf5 library (required by netcdf4)
    find_library(HDF5_LIBRARY hdf5
        PATHS ${NETCDF4_PATH}/lib /usr/lib /usr/local/lib
        NO_DEFAULT_PATH
    )

    if(NETCDF4_INCLUDE_DIR AND NETCDF4_CXX_LIBRARY AND NETCDF4_LIBRARY)
        set(NETCDF4_FOUND TRUE)
        message(STATUS "NETCDF4 support will be built.")
        message(STATUS "  NETCDF4_INCLUDE_DIR: ${NETCDF4_INCLUDE_DIR}")
        message(STATUS "  NETCDF4_CXX_LIBRARY: ${NETCDF4_CXX_LIBRARY}")
        message(STATUS "  NETCDF4_LIBRARY: ${NETCDF4_LIBRARY}")
        if(HDF5_LIBRARY)
            message(STATUS "  HDF5_LIBRARY: ${HDF5_LIBRARY}")
        endif()
    else()
        set(NETCDF4_FOUND FALSE)
        message(STATUS "NETCDF4 support will NOT be built.")
        if(NOT NETCDF4_INCLUDE_DIR)
            message(STATUS "  ncFile.h not found")
        endif()
        if(NOT NETCDF4_CXX_LIBRARY)
            message(STATUS "  netcdf_c++4 library not found")
        endif()
    endif()
else()
    message(STATUS "NETCDF4 support will NOT be built.")
endif()

# Validate the cmdline input
if(NETCDF_FOUND AND NETCDF4_FOUND)
    message(FATAL_ERROR 
        "  ERROR: only one between NetCDF Legacy and NetCDF4 can be configured."
    )
endif()

# ============================================================================
# Find NS-ALLINONE - equivalent to nsallinone.m4 logic
# ============================================================================

set(NSALLINONE_FOUND FALSE)
# Set NS_PATH from NS_ALLINONE_PATH (equivalent to m4 logic)
if(NSALLINONE_PATH)
    set(NS_PATH ${NSALLINONE_PATH}/ns-2.34)
    set(TCLCL_PATH ${NSALLINONE_PATH}/tclcl)
    set(OTCL_PATH ${NSALLINONE_PATH}/otcl)

    # Check if tclcl.h exists (equivalent to checking for NS headers)
    if(EXISTS ${TCLCL_PATH}/tclcl.h)
        set(NSALLINONE_FOUND TRUE)
        message(STATUS "NS ALLINONE found.")
        message(STATUS "  NS_PATH: ${NS_PATH}")
        message(STATUS "  TCLCL_PATH: ${TCLCL_PATH}")
    else()
        set(NSALLINONE_FOUND FALSE)
        message(STATUS "NS MIRACLE support will NOT be built (tclcl.h not found).")
    endif()
else()
    set(NSALLINONE_FOUND FALSE)
    message(STATUS "NS MIRACLE support will NOT be built (NS_ALLINONE_PATH not set).")
endif()

# ============================================================================
# Find NS MIRACLE (if enabled) - equivalent to nsmiracle.m4 logic
# ============================================================================

set(NS_MIRACLE_FOUND FALSE)
if(NSALLINONE_FOUND)
    if(NSMIRACLE_PATH)
        # Check if module.h exists (equivalent to checking for NS MIRACLE headers)
        if(EXISTS ${NSMIRACLE_PATH}/nsmiracle/module.h)
            set(NS_MIRACLE_FOUND TRUE)
            message(STATUS "NS MIRACLE support will be built.")
            message(STATUS "  NSMIRACLE_PATH: ${NSMIRACLE_PATH}")
        else()
            set(NS_MIRACLE_FOUND FALSE)
            message(STATUS "NS MIRACLE support will NOT be built (module.h not found).")
        endif()
    else()
        set(NS_MIRACLE_FOUND FALSE)
        message(STATUS "NS MIRACLE support will NOT be built (NSMIRACLE_PATH not set).")
    endif()
endif()

# ============================================================================
# Compile definitions (equivalent to AC_DEFINE and AM_CONDITIONAL)
# ============================================================================

# Multi-thread support always enabled
if(WOSS_ENABLE_MULTITHREAD)
    add_compile_definitions(WOSS_MULTITHREAD)
    message(STATUS "Multi-thread support will be built.")
endif()

# NETCDF support definitions
if(NETCDF_FOUND OR NETCDF4_FOUND)
    add_compile_definitions(WOSS_NETCDF_SUPPORT)
endif()
if(NETCDF4_FOUND)
    add_compile_definitions(WOSS_NETCDF4_SUPPORT)
endif()

# NS MIRACLE support definitions
if(NS_MIRACLE_FOUND AND NSALLINONE_FOUND)
    add_compile_definitions(WOSS_NS_MIRACLE_SUPPORT)
endif()

# ============================================================================
# Install prefix (equivalent to --prefix in autotools)
# ============================================================================

if(NOT CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
endif()

# ============================================================================
# Enable testing (must be called before add_subdirectory(tests))
# ============================================================================

enable_testing()

# ============================================================================
# Add subdirectories (equivalent to AC_CONFIG_FILES and SUBDIRS in Makefile.am)
# ============================================================================

add_subdirectory(woss)
if(NS_MIRACLE_FOUND AND NSALLINONE_FOUND)
    add_subdirectory(woss_phy)
endif()
add_subdirectory(tests)
add_subdirectory(samples)
add_subdirectory(doc)

# ============================================================================
# Configure file for passing variables to subdirectories
# ============================================================================

configure_file(configure.cmake.in configure.cmake @ONLY)

message(STATUS "Generating header files and makefiles")

# ============================================================================
# Install rules (equivalent to make install in autotools)
# ============================================================================

if(NS_MIRACLE_FOUND AND NSALLINONE_FOUND)
    install(TARGETS WOSS WOSSPhy
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
    )
else()
    install(TARGETS WOSS
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
    )
endif()

install(DIRECTORY woss/woss_def woss/woss_db
    DESTINATION include/woss
    FILES_MATCHING PATTERN "*.h"
)

if(NS_MIRACLE_FOUND AND NSALLINONE_FOUND)
    install(DIRECTORY woss_phy/
        DESTINATION include/woss_phy
        FILES_MATCHING PATTERN "*.h"
    )
endif()