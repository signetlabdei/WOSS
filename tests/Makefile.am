# WOSS - World Ocean Simulation System -
# 
# Copyright (C) 2020 Federico Guerra 
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


AM_CPPFLAGS = @NS_CPPFLAGS@ @NSMIRACLE_CPPFLAGS@ @UW_WOSS_PHY_CPPFLAGS@ @NETCDF_CPPFLAGS@ @NETCDF4_CPPFLAGS@ @UW_WOSS_PHY_WARN@
AM_LDFLAGS  =  @NS_LDFLAGS@  @NSMIRACLE_LDFLAGS@  @UW_WOSS_PHY_LDFLAGS@  @NETCDF_LDFLAGS@  @NETCDF4_LDFLAGS@
LDADD       =   @NS_LIBADD@   @NSMIRACLE_LIBADD@   @UW_WOSS_PHY_LIBADD@   @NETCDF_LIBADD@   @NETCDF4_LIBADD@


#TEST_EXTENSIONS = .sh

# These are the tests programs.
TESTPROGRAMS = woss-coord-definitions-test-bin

if NETCDF_BUILD
#TESTPROGRAMS += 
endif # NETCDF_BUILD


check_PROGRAMS = $(TESTPROGRAMS)

TESTS = $(TESTPROGRAMS)

# Here's the source code for the programs.
woss_coord_definitions_test_bin_SOURCES = woss-test.cpp woss-coord-definitions-test.cpp

#
#EXTRA_DIST = 

# Cleaning up files created during the process.
#CLEANFILES =
