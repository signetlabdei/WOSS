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


# Test suite

# Note: enable_testing() is called in top-level CMakeLists.txt

# ============================================================================
# Test programs (equivalent to check_PROGRAMS)
# ============================================================================

set(TEST_COMMON_SOURCES woss-test.cpp)

# woss-unit-test-bin
add_executable(woss-unit-test-bin ${TEST_COMMON_SOURCES} woss-unit-test.cpp)
target_include_directories(woss-unit-test-bin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../woss
    ${CMAKE_CURRENT_SOURCE_DIR}/../woss/woss_def
    ${CMAKE_CURRENT_SOURCE_DIR}/../woss/woss_db
)
if(NETCDF4_FOUND)
    target_compile_options(woss-unit-test-bin PRIVATE -Wno-unused-variable)
endif()
target_link_libraries(woss-unit-test-bin WOSS)

# woss-coord-definitions-test-bin
add_executable(woss-coord-definitions-test-bin ${TEST_COMMON_SOURCES} woss-coord-definitions-test.cpp)
target_include_directories(woss-coord-definitions-test-bin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../woss
    ${CMAKE_CURRENT_SOURCE_DIR}/../woss/woss_def
    ${CMAKE_CURRENT_SOURCE_DIR}/../woss/woss_db
)
if(NETCDF4_FOUND)
    target_compile_options(woss-coord-definitions-test-bin PRIVATE -Wno-unused-variable)
endif()
target_link_libraries(woss-coord-definitions-test-bin WOSS)

# woss-bellhop-test-bin
add_executable(woss-bellhop-test-bin ${TEST_COMMON_SOURCES} woss-bellhop-test.cpp)
target_include_directories(woss-bellhop-test-bin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../woss
    ${CMAKE_CURRENT_SOURCE_DIR}/../woss/woss_def
    ${CMAKE_CURRENT_SOURCE_DIR}/../woss/woss_db
)
if(NETCDF4_FOUND)
    target_compile_options(woss-bellhop-test-bin PRIVATE -Wno-unused-variable)
endif()
target_link_libraries(woss-bellhop-test-bin WOSS)

# woss-bellhop-script-bin
add_executable(woss-bellhop-script-bin ${TEST_COMMON_SOURCES} woss-bellhop-script.cpp)
target_include_directories(woss-bellhop-script-bin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../woss
    ${CMAKE_CURRENT_SOURCE_DIR}/../woss/woss_def
    ${CMAKE_CURRENT_SOURCE_DIR}/../woss/woss_db
)
if(NETCDF4_FOUND)
    target_compile_options(woss-bellhop-script-bin PRIVATE -Wno-unused-variable)
endif()
target_link_libraries(woss-bellhop-script-bin WOSS)

# ============================================================================
# Register tests (equivalent to TESTS = $(TESTPROGRAMS))
# ============================================================================

add_test(NAME unit_test COMMAND woss-unit-test-bin)
add_test(NAME coord_definitions_test COMMAND woss-coord-definitions-test-bin)
add_test(NAME bellhop_test COMMAND woss-bellhop-test-bin)
add_test(NAME bellhop_script COMMAND woss-bellhop-script-bin)

# ============================================================================
# Extra dist files
# ============================================================================

set(EXTRA_DIST woss-test.h README.md)
