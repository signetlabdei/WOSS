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


AC_DEFUN([AC_ARG_WITH_NETCDF],[

NETCDF_PATH=''
NETCDF_CPPLAGS=''
NETCDF_LDFLAGS=''
NETCDF_LIBADD=''

AC_ARG_WITH([netcdf],
	[AS_HELP_STRING([--with-netcdf=<directory>],
			[use netcdf installation in <directory>])],
	[
		if test "x$withval" != "xno" ; then

   		     if test -d $withval ; then

   			NETCDF_PATH="${withval}"

			relevantheaderfile="${NETCDF_PATH}/include/netcdfcpp.h"
	    		if test ! -f "${relevantheaderfile}"  ; then
			  	AC_MSG_WARN([could not find ${relevantheaderfile}, 
  is --with-netcdf=${withval} correct?])
			fi		

			NETCDF_CPPFLAGS="$NETCDF_CPPFLAGS -I${NETCDF_PATH}/include"
			NETCDF_LDFLAGS="$NETCDF_LDFLAGS -L${NETCDF_PATH}/lib"
		        NETCDF_LIBADD="$NETCDF_LIBADD -lnetcdf_c++ -lnetcdf"

			NETCDF_DISTCHECK_CONFIGURE_FLAGS="--with-netcdf=$withval"
			AC_SUBST(NETCDF_DISTCHECK_CONFIGURE_FLAGS)

   		     else	

			AC_MSG_WARN([netcdf path $withval is not a directory])
	
		     fi

		fi

	])


AC_SUBST(NETCDF_CPPFLAGS)
AC_SUBST(NETCDF_LDFLAGS)
AC_SUBST(NETCDF_LIBADD)

])



AC_DEFUN([AC_CHECK_NETCDF],[
	
# temporarily add NS_CPPFLAGS and NETCDF_CPPFLAGS to CPPFLAGS
BACKUP_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $NS_CPPFLAGS $NETCDF_CPPFLAGS"


AC_LANG_PUSH(C++)

AC_MSG_CHECKING([for netcdf headers])


AC_PREPROC_IFELSE(
	[AC_LANG_PROGRAM([[
		#include <netcdfcpp.h>
		NcFile* file; 
		]],[[
		]]  )],
		[
		 AC_MSG_RESULT([yes])
		 found_netcdf=yes
		[$1]
		],
		[
		 AC_MSG_RESULT([no])
		 found_netcdf=no
		[$2]
		AC_MSG_WARN([could not find netcdf])
		])


AM_CONDITIONAL([HAVE_NETCDF], [test x$found_netcdf = xyes])

# Restoring to the initial value
CPPFLAGS="$BACKUP_CPPFLAGS"

AC_LANG_POP(C++)

])











