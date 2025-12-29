# WOSS - World Ocean Simulation System -
# 
# Copyright (C) 2020 2025 Federico Guerra 
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


AM_CXXFLAGS = @UW_WOSS_CXX_FLAGS@ @UW_WOSS_WARN@
AM_CPPFLAGS = @NETCDF_CPPFLAGS@ @NETCDF4_CPPFLAGS@ @UW_WOSS_CPPFLAGS@ @UW_WOSS_PHY_CPPFLAGS@
AM_LDFLAGS = @NETCDF_LDFLAGS@ @NETCDF4_LDFLAGS@ @UW_WOSS_LDFLAGS@ @UW_WOSS_PHY_LDFLAGS@
LDADD = @NETCDF_LIBADD@ @NETCDF4_LIBADD@ @UW_WOSS_LIBADD@ @UW_WOSS_PHY_LIBADD@


#TEST_EXTENSIONS = .sh

# These are the tests programs.
TESTPROGRAMS = woss-coord-definitions-test-bin woss-bellhop-test-bin woss-bellhop-script-bin woss-unit-test-bin

if NETCDF_BUILD
#TESTPROGRAMS += 
endif # NETCDF_BUILD


check_PROGRAMS = $(TESTPROGRAMS)

TESTS = $(TESTPROGRAMS)

# Here's the source code for the programs.
woss_coord_definitions_test_bin_SOURCES = woss-test.cpp woss-coord-definitions-test.cpp

woss_bellhop_test_bin_SOURCES = woss-test.cpp woss-bellhop-test.cpp

woss_bellhop_script_bin_SOURCES = woss-test.cpp woss-bellhop-script.cpp

woss_unit_test_bin_SOURCES = woss-test.cpp woss-unit-test.cpp

EXTRA_DIST = woss-test.h

# Cleaning up files created during the process.
#CLEANFILES =
