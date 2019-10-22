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

if NS_MIRACLE_BUILD 

lib_LTLIBRARIES = libWOSSPhy.la

libWOSSPhy_la_SOURCES = ./tcl_hooks/altimetry-definitions-tcl.cpp ./tcl_hooks/altimetry-definitions-tcl.h \
			./tcl_hooks/bathymetry-gebco-db-creator-tcl.cpp ./tcl_hooks/bathymetry-gebco-db-creator-tcl.h \
			./tcl_hooks/bathymetry-utm-csv-db-creator-tcl.cpp ./tcl_hooks/bathymetry-utm-csv-db-creator-tcl.h \
			./tcl_hooks/definitions-handler-tcl.cpp ./tcl_hooks/definitions-handler-tcl.h \
			./tcl_hooks/pressure-definitions-tcl.cpp ./tcl_hooks/pressure-definitions-tcl.h \
			./tcl_hooks/random-generator-definitions-tcl.cpp ./tcl_hooks/random-generator-definitions-tcl.h \
         		./tcl_hooks/sediment-definitions-tcl.cpp ./tcl_hooks/sediment-definitions-tcl.h \
			./tcl_hooks/ssp-definitions-tcl.cpp ./tcl_hooks/ssp-definitions-tcl.h \
			./tcl_hooks/time-arrival-definitions-tcl.cpp ./tcl_hooks/time-arrival-definitions-tcl.h \
			./tcl_hooks/time-definitions-tcl.cpp ./tcl_hooks/time-definitions-tcl.h \
			./tcl_hooks/transducer-definitions-tcl.cpp ./tcl_hooks/transducer-definitions-tcl.h \
			./tcl_hooks/transducer-handler-tcl.cpp ./tcl_hooks/transducer-handler-tcl.h \
			./tcl_hooks/woss-controller-tcl.cpp ./tcl_hooks/woss-controller-tcl.h \
			./tcl_hooks/bellhop-creator-tcl.cpp ./tcl_hooks/bellhop-creator-tcl.h \
			./tcl_hooks/woss-db-manager-tcl.cpp ./tcl_hooks/woss-db-manager-tcl.h \
			./tcl_hooks/res-pressure-bin-db-creator-tcl.cpp ./tcl_hooks/res-pressure-bin-db-creator-tcl.h \
			./tcl_hooks/res-pressure-txt-db-creator-tcl.cpp ./tcl_hooks/res-pressure-txt-db-creator-tcl.h \
			./tcl_hooks/res-time-arr-bin-db-creator-tcl.cpp ./tcl_hooks/res-time-arr-bin-db-creator-tcl.h \
			./tcl_hooks/res-time-arr-txt-db-creator-tcl.cpp ./tcl_hooks/res-time-arr-txt-db-creator-tcl.h \
			./tcl_hooks/sediment-deck41-db-creator-tcl.cpp ./tcl_hooks/sediment-deck41-db-creator-tcl.h \
			./tcl_hooks/ssp-woa2005-db-creator-tcl.cpp ./tcl_hooks/ssp-woa2005-db-creator-tcl.h \
			./tcl_hooks/woss-manager-simple-tcl.cpp ./tcl_hooks/woss-manager-simple-tcl.h \
			./tcl_hooks/woss-utilities-tcl.cpp ./tcl_hooks/woss-utilities-tcl.h \
			uw-woss-pkt-hdr.h \
                        uw-woss-mpropagation.h uw-woss-mpropagation.cpp \
                        uw-woss-bpsk.h uw-woss-bpsk.cpp \
                        uw-woss-channel.h uw-woss-channel.cpp \
                        uw-woss-channel-estimator.h uw-woss-channel-estimator.cpp \
                        uw-woss-clmsg-channel-estimation.h uw-woss-clmsg-channel-estimation.cpp \
                        uw-woss-position.h uw-woss-position.cpp \
			uw-woss-waypoint-position.h uw-woss-waypoint-position.cpp \
                        uw-woss-cbr.h uw-woss-cbr.cpp \
                        uw-woss-time-reference.h uw-woss-time-reference.cpp \
		        uw-woss-random-generator.h uw-woss-random-generator.cpp \
                        initlib.cc 


libWOSSPhy_la_CPPFLAGS = @NS_CPPFLAGS@ @NSMIRACLE_CPPFLAGS@ @UW_WOSS_PHY_CPPFLAGS@ @NETCDF_CPPFLAGS@ @NETCDF4_CPPFLAGS@ @UW_WOSS_PHY_WARN@
libWOSSPhy_la_LDFLAGS =  @NS_LDFLAGS@  @NSMIRACLE_LDFLAGS@  @UW_WOSS_PHY_LDFLAGS@  @NETCDF_LDFLAGS@  @NETCDF4_LDFLAGS@
libWOSSPhy_la_LIBADD =   @NS_LIBADD@   @NSMIRACLE_LIBADD@   @UW_WOSS_PHY_LIBADD@   @NETCDF_LIBADD@   @NETCDF4_LIBADD@

nodist_libWOSSPhy_la_SOURCES = woss-phy-embeddedtcl.cc

BUILT_SOURCES = woss-phy-embeddedtcl.cc

CLEANFILES = woss-phy-embeddedtcl.cc
endif 

TCL_FILES = uw-woss-phy-defaults.tcl

if NS_MIRACLE_BUILD

woss-phy-embeddedtcl.cc: Makefile $(TCL_FILES)
		         cat $(TCL_FILES) | @TCL2CPP@ WossPhyInitTclCode > woss-phy-embeddedtcl.cc

endif

EXTRA_DIST = ./tcl_hooks/altimetry-definitions-tcl.cpp ./tcl_hooks/altimetry-definitions-tcl.h \
		./tcl_hooks/bathymetry-gebco-db-creator-tcl.cpp ./tcl_hooks/bathymetry-gebco-db-creator-tcl.h \
		./tcl_hooks/bathymetry-utm-csv-db-creator-tcl.cpp ./tcl_hooks/bathymetry-utm-csv-db-creator-tcl.h \
		./tcl_hooks/bellhop-creator-tcl.cpp ./tcl_hooks/bellhop-creator-tcl.h \
		./tcl_hooks/definitions-handler-tcl.cpp ./tcl_hooks/definitions-handler-tcl.h \
		./tcl_hooks/pressure-definitions-tcl.cpp ./tcl_hooks/pressure-definitions-tcl.h \
		./tcl_hooks/random-generator-definitions-tcl.cpp ./tcl_hooks/random-generator-definitions-tcl.h \
		./tcl_hooks/res-pressure-bin-db-creator-tcl.cpp ./tcl_hooks/res-pressure-bin-db-creator-tcl.h \
		./tcl_hooks/res-pressure-txt-db-creator-tcl.cpp ./tcl_hooks/res-pressure-txt-db-creator-tcl.h \
		./tcl_hooks/res-time-arr-bin-db-creator-tcl.cpp ./tcl_hooks/res-time-arr-bin-db-creator-tcl.h \
		./tcl_hooks/res-time-arr-txt-db-creator-tcl.cpp ./tcl_hooks/res-time-arr-txt-db-creator-tcl.h \
		./tcl_hooks/sediment-deck41-db-creator-tcl.cpp ./tcl_hooks/sediment-deck41-db-creator-tcl.h \
		./tcl_hooks/sediment-definitions-tcl.cpp ./tcl_hooks/sediment-definitions-tcl.h \
		./tcl_hooks/ssp-definitions-tcl.cpp ./tcl_hooks/ssp-definitions-tcl.h \
		./tcl_hooks/ssp-time-evo-db-creator-tcl.cpp ./tcl_hooks/ssp-time-evo-db-creator-tcl.h \
		./tcl_hooks/ssp-woa2005-db-creator-tcl.cpp ./tcl_hooks/ssp-woa2005-db-creator-tcl.h \
		./tcl_hooks/time-arrival-definitions-tcl.cpp ./tcl_hooks/time-arrival-definitions-tcl.h \
		./tcl_hooks/time-definitions-tcl.cpp ./tcl_hooks/time-definitions-tcl.h \
		./tcl_hooks/transducer-definitions-tcl.cpp ./tcl_hooks/transducer-definitions-tcl.h \
		./tcl_hooks/transducer-handler-tcl.cpp ./tcl_hooks/transducer-handler-tcl.h \
		./tcl_hooks/woss-controller-tcl.cpp ./tcl_hooks/woss-controller-tcl.h \
		./tcl_hooks/woss-db-manager-tcl.cpp ./tcl_hooks/woss-db-manager-tcl.h \
		./tcl_hooks/woss-manager-simple-tcl.cpp ./tcl_hooks/woss-manager-simple-tcl.h \
		./tcl_hooks/woss-utilities-tcl.cpp ./tcl_hooks/woss-utilities-tcl.h \
             uw-woss-pkt-hdr.h \
             uw-woss-mpropagation.h uw-woss-mpropagation.cpp \
             uw-woss-bpsk.h uw-woss-bpsk.cpp \
             uw-woss-channel.h uw-woss-channel.cpp \
             uw-woss-channel-estimator.h uw-woss-channel-estimator.cpp \
             uw-woss-clmsg-channel-estimation.h uw-woss-clmsg-channel-estimation.cpp \
             uw-woss-position.h uw-woss-position.cpp \
	     uw-woss-waypoint-position.h uw-woss-waypoint-position.cpp \
	     uw-woss-cbr.h uw-woss-cbr.cpp \
             uw-woss-time-reference.h uw-woss-time-reference.cpp \
	     uw-woss-random-generator.h uw-woss-random-generator.cpp \
	     initlib.cc \
	     $(TCL_FILES)




