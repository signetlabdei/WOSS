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

# libWOSS library and DbCreator programs

# ============================================================================
# libWOSS shared library
# ============================================================================

set(WOSS_SOURCES
    woss.h woss.cpp
    res-reader.h res-reader.cpp
    woss-creator-container.h woss-creator.h woss-creator.cpp
    woss-manager.h woss-manager.cpp woss-manager-simple.h
    ac-toolbox-woss.h ac-toolbox-woss.cpp
    ac-toolbox-shd-reader.h ac-toolbox-shd-reader.cpp
    ac-toolbox-arr-asc-reader.h ac-toolbox-arr-asc-reader.cpp
    ac-toolbox-arr-bin-reader.h ac-toolbox-arr-bin-reader.cpp
    bellhop-woss.h bellhop-woss.cpp bellhop-creator.h bellhop-creator.cpp
    woss-controller.h woss-controller.cpp
    initlib.cc
    # woss_def sources
    woss_def/location-definitions.h woss_def/location-definitions.cpp
    woss_def/random-generator-definitions.h woss_def/random-generator-definitions.cpp
    woss_def/transducer-definitions.h woss_def/transducer-definitions.cpp
    woss_def/transducer-handler.h woss_def/transducer-handler.cpp
    woss_def/altimetry-definitions.h woss_def/altimetry-definitions.cpp
    woss_def/custom-precision-double.h woss_def/custom-precision-double.cpp
    woss_def/singleton-definitions.h
    woss_def/location-definitions.h woss_def/location-definitions.cpp
    woss_def/coordinates-definitions.h woss_def/coordinates-definitions.cpp
    woss_def/pressure-definitions.h woss_def/pressure-definitions.cpp
    woss_def/sediment-definitions.h woss_def/sediment-definitions.cpp
    woss_def/ssp-definitions.h woss_def/ssp-definitions.cpp
    woss_def/time-arrival-definitions.h woss_def/time-arrival-definitions.cpp
    woss_def/time-definitions.h woss_def/time-definitions.cpp
    # woss_db sources
    woss_db/woss-db.h woss_db/woss-db.cpp
    woss_db/woss-db-creator.h woss_db/woss-db-creator.cpp
    woss_db/bathymetry-gebco-db.h woss_db/bathymetry-gebco-db.cpp
    woss_db/bathymetry-gebco-db-creator.h woss_db/bathymetry-gebco-db-creator.cpp
    woss_db/bathymetry-utm-csv-db.h woss_db/bathymetry-utm-csv-db.cpp
    woss_db/bathymetry-utm-csv-db-creator.h woss_db/bathymetry-utm-csv-db-creator.cpp
    woss_db/ssp-woa2005-db.h woss_db/ssp-woa2005-db.cpp
    woss_db/ssp-woa2005-db-creator.h woss_db/ssp-woa2005-db-creator.cpp
    woss_db/sediment-deck41-db-logic-control.h woss_db/sediment-deck41-db-logic-control.cpp
    woss_db/sediment-deck41-db.h woss_db/sediment-deck41-db.cpp
    woss_db/sediment-deck41-db-creator.h woss_db/sediment-deck41-db-creator.cpp
    woss_db/sediment-deck41-coord-db.h woss_db/sediment-deck41-coord-db.cpp
    woss_db/sediment-deck41-marsden-one-db.h woss_db/sediment-deck41-marsden-one-db.cpp
    woss_db/sediment-deck41-marsden-db.h woss_db/sediment-deck41-marsden-db.cpp
    woss_db/res-time-arr-txt-db.h woss_db/res-time-arr-txt-db.cpp
    woss_db/res-time-arr-txt-db-creator.h woss_db/res-time-arr-txt-db-creator.cpp
    woss_db/res-time-arr-bin-db.h woss_db/res-time-arr-bin-db.cpp
    woss_db/res-time-arr-bin-db-creator.h woss_db/res-time-arr-bin-db-creator.cpp
    woss_db/res-pressure-txt-db.h woss_db/res-pressure-txt-db.cpp
    woss_db/res-pressure-txt-db-creator.h woss_db/res-pressure-txt-db-creator.cpp
    woss_db/res-pressure-bin-db.h woss_db/res-pressure-bin-db.cpp
    woss_db/res-pressure-bin-db-creator.h woss_db/res-pressure-bin-db-creator.cpp
    woss_db/woss-db-manager.h woss_db/woss-db-manager.cpp
    woss_db/woss-db-custom-data-container.h
)

add_library(WOSS SHARED ${WOSS_SOURCES})

target_include_directories(WOSS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/woss_def
    ${CMAKE_CURRENT_SOURCE_DIR}/woss_db
)

if(NETCDF_FOUND OR NETCDF4_FOUND)
    target_compile_options(WOSS PRIVATE -Wno-unused-variable)
endif()

if(NETCDF_FOUND)
    target_include_directories(WOSS PUBLIC ${NETCDF_INCLUDE_DIR})
    target_link_libraries(WOSSPhy PUBLIC ${NETCDF_CXX_LIBRARY} ${NETCDF_LIBRARY})
elseif(NETCDF4_FOUND)
    target_include_directories(WOSS PUBLIC ${NETCDF4_INCLUDE_DIR})
    target_link_libraries(WOSS PUBLIC ${NETCDF4_CXX_LIBRARY} ${NETCDF4_LIBRARY} ${HDF5_LIBRARY})
endif()
