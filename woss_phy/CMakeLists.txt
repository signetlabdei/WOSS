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

# libWOSSPhy library

# ============================================================================
# libWOSSPhy shared library
# ============================================================================

set(WOSS_PHY_SOURCES
    uw-woss-bpsk.h uw-woss-bpsk.cpp
    uw-woss-cbr.h uw-woss-cbr.cpp
    uw-woss-channel.h uw-woss-channel.cpp
    uw-woss-channel-estimator.h uw-woss-channel-estimator.cpp
    uw-woss-clmsg-channel-estimation.h uw-woss-clmsg-channel-estimation.cpp
    uw-woss-position.h uw-woss-position.cpp
    uw-woss-waypoint-position.h uw-woss-waypoint-position.cpp
    uw-woss-mpropagation.h uw-woss-mpropagation.cpp
    uw-woss-time-reference.h uw-woss-time-reference.cpp
    uw-woss-random-generator.h uw-woss-random-generator.cpp
    uw-woss-pkt-hdr.h
    ./tcl_hooks/altimetry-definitions-tcl.cpp ./tcl_hooks/altimetry-definitions-tcl.h
    ./tcl_hooks/bathymetry-gebco-db-creator-tcl.cpp ./tcl_hooks/bathymetry-gebco-db-creator-tcl.h
    ./tcl_hooks/bathymetry-utm-csv-db-creator-tcl.cpp ./tcl_hooks/bathymetry-utm-csv-db-creator-tcl.h
    ./tcl_hooks/definitions-handler-tcl.cpp ./tcl_hooks/definitions-handler-tcl.h
    ./tcl_hooks/pressure-definitions-tcl.cpp ./tcl_hooks/pressure-definitions-tcl.h
    ./tcl_hooks/random-generator-definitions-tcl.cpp ./tcl_hooks/random-generator-definitions-tcl.h
    ./tcl_hooks/sediment-definitions-tcl.cpp ./tcl_hooks/sediment-definitions-tcl.h
    ./tcl_hooks/ssp-definitions-tcl.cpp ./tcl_hooks/ssp-definitions-tcl.h
    ./tcl_hooks/time-arrival-definitions-tcl.cpp ./tcl_hooks/time-arrival-definitions-tcl.h
    ./tcl_hooks/time-definitions-tcl.cpp ./tcl_hooks/time-definitions-tcl.h
    ./tcl_hooks/transducer-definitions-tcl.cpp ./tcl_hooks/transducer-definitions-tcl.h
    ./tcl_hooks/transducer-handler-tcl.cpp ./tcl_hooks/transducer-handler-tcl.h
    ./tcl_hooks/woss-controller-tcl.cpp ./tcl_hooks/woss-controller-tcl.h
    ./tcl_hooks/bellhop-creator-tcl.cpp ./tcl_hooks/bellhop-creator-tcl.h
    ./tcl_hooks/woss-db-manager-tcl.cpp ./tcl_hooks/woss-db-manager-tcl.h
    ./tcl_hooks/res-pressure-bin-db-creator-tcl.cpp ./tcl_hooks/res-pressure-bin-db-creator-tcl.h
    ./tcl_hooks/res-pressure-txt-db-creator-tcl.cpp ./tcl_hooks/res-pressure-txt-db-creator-tcl.h
    ./tcl_hooks/res-time-arr-bin-db-creator-tcl.cpp ./tcl_hooks/res-time-arr-bin-db-creator-tcl.h
    ./tcl_hooks/res-time-arr-txt-db-creator-tcl.cpp ./tcl_hooks/res-time-arr-txt-db-creator-tcl.h
    ./tcl_hooks/sediment-deck41-db-creator-tcl.cpp ./tcl_hooks/sediment-deck41-db-creator-tcl.h
    ./tcl_hooks/ssp-woa2005-db-creator-tcl.cpp ./tcl_hooks/ssp-woa2005-db-creator-tcl.h
    ./tcl_hooks/woss-manager-simple-tcl.cpp ./tcl_hooks/woss-manager-simple-tcl.h
    ./tcl_hooks/woss-utilities-tcl.cpp ./tcl_hooks/woss-utilities-tcl.h
    initlib.cc
)

# --- embedded TCL generation ---
find_program(TCL2CPP_EXECUTABLE
    NAMES tcl2c++
    HINTS ${TCLCL_PATH}
    REQUIRED
)

set(TCL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/uw-woss-phy-defaults.tcl)
set(EMBEDDED_TCL_CC ${CMAKE_CURRENT_BINARY_DIR}/woss-phy-embeddedtcl.cc)
set(EMBEDDED_TCL_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/generate_embedded_tcl.sh)

file(WRITE ${EMBEDDED_TCL_SCRIPT}
    "#!/bin/sh\ncat \"$1\" | \"$2\" WossPhyInitTclCode > \"$3\"\n"
)

add_custom_command(
    OUTPUT  ${EMBEDDED_TCL_CC}
    COMMAND sh ${EMBEDDED_TCL_SCRIPT}
            ${TCL_FILES}
            ${TCL2CPP_EXECUTABLE}
            ${EMBEDDED_TCL_CC}
    DEPENDS ${TCL_FILES}
    COMMENT "Generating woss-phy-embeddedtcl.cc"
)

list(APPEND WOSS_PHY_SOURCES ${EMBEDDED_TCL_CC})
# -------------------------

add_library(WOSSPhy SHARED ${WOSS_PHY_SOURCES})

target_include_directories(WOSSPhy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../woss
    ${CMAKE_CURRENT_SOURCE_DIR}/../woss/woss_def
    ${CMAKE_CURRENT_SOURCE_DIR}/../woss/woss_db
)

target_compile_options(WOSSPhy PRIVATE -Wno-write-strings -Wno-overloaded-virtual)

if(NETCDF_FOUND OR NETCDF4_FOUND)
    # Fix: netCDF headers have unused variable warnings that cause -Werror to fail
    target_compile_options(WOSSPhy PRIVATE -Wno-unused-variable)
endif()

target_include_directories(WOSSPhy PUBLIC
    ${NSALLINONE_PATH}/include
    ${NS_PATH}
    ${NS_PATH}/mac
    ${NS_PATH}/propagation
    ${NS_PATH}/mobile
    ${NS_PATH}/pcap
    ${NS_PATH}/tcp
    ${NS_PATH}/sctp
    ${NS_PATH}/common
    ${NS_PATH}/link
    ${NS_PATH}/queue
    ${NS_PATH}/trace
    ${NS_PATH}/adc
    ${NS_PATH}/apps
    ${NS_PATH}/routing
    ${NS_PATH}/tools
    ${NS_PATH}/classifier
    ${NS_PATH}/mcast
    ${TCLCL_PATH}
    ${OTCL_PATH}
    ${NSMIRACLE_PATH}
    # Add NS MIRACLE include paths (equivalent to nsmiracle.m4 logic)
    ${NSMIRACLE_PATH}/nsmiracle
    ${NSMIRACLE_PATH}/cbr
    ${NSMIRACLE_PATH}/ip
    ${NSMIRACLE_PATH}/link
    ${NSMIRACLE_PATH}/mac802_11
    ${NSMIRACLE_PATH}/marq
    ${NSMIRACLE_PATH}/mobility
    ${NSMIRACLE_PATH}/mphy
    ${NSMIRACLE_PATH}/mmac
    ${NSMIRACLE_PATH}/phy802_11
    ${NSMIRACLE_PATH}/port
    ${NSMIRACLE_PATH}/tcp
    ${NSMIRACLE_PATH}/wirelessch
    ${NSMIRACLE_PATH}/aodv
    ${NSMIRACLE_PATH}/mll
    ${NSMIRACLE_PATH}/routing
    ${NSMIRACLE_PATH}/uwm
)

if(NETCDF_FOUND)
    target_include_directories(WOSSPhy PUBLIC ${NETCDF_INCLUDE_DIR})
    target_link_libraries(WOSSPhy PUBLIC ${NETCDF_CXX_LIBRARY} ${NETCDF_LIBRARY})
elseif(NETCDF4_FOUND)
    target_include_directories(WOSSPhy PUBLIC ${NETCDF4_INCLUDE_DIR})
    target_link_libraries(WOSSPhy PUBLIC ${NETCDF4_CXX_LIBRARY} ${NETCDF4_LIBRARY} ${HDF5_LIBRARY})
endif()

# Link against libWOSS (WOSS uses PUBLIC for netCDF, so WOSSPhy gets them transitively)
target_link_libraries(WOSSPhy PUBLIC WOSS)
