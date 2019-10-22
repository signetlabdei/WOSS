# WOSS - World Ocean Simulation System -
# 
# Copyright (C) 2019 Federico Guerra 
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


AC_DEFUN([AC_ARG_WITH_NETCDF4],[

NETCDF4_PATH=''
NETCDF4_CPPFLAGS='-Wno-unused-variable '
NETCDF4_LDFLAGS=''
NETCDF4_LIBADD=''

AC_ARG_WITH([netcdf4],
	[AS_HELP_STRING([--with-netcdf4=<directory>],
			[use netcdf4 installation in <directory>])],
	[
		if test "x$withval" != "xno" ; then

   		     if test -d $withval ; then

   			NETCDF4_PATH="${withval}"

			relevantheaderfile="${NETCDF4_PATH}/include/ncFile.h"
	    		if test ! -f "${relevantheaderfile}"  ; then
			  	AC_MSG_WARN([could not find ${relevantheaderfile}, 
  is --with-netcdf4=${withval} correct?])
			fi		

			NETCDF4_CPPFLAGS="$NETCDF4_CPPFLAGS -I${NETCDF4_PATH}/include"
			NETCDF4_LDFLAGS="$NETCDF4_LDFLAGS -L${NETCDF4_PATH}/lib"
			NETCDF4_LIBADD="$NETCDF4_LIBADD -lnetcdf_c++4 -lnetcdf -lhdf5"

			NETCDF4_DISTCHECK_CONFIGURE_FLAGS="--with-netcdf4=$withval"
			AC_SUBST(NETCDF4_DISTCHECK_CONFIGURE_FLAGS)

   		     else	

			AC_MSG_WARN([netcdf4 path $withval is not a directory])
	
		     fi

		fi

	])


AC_SUBST(NETCDF4_CPPFLAGS)
AC_SUBST(NETCDF4_LDFLAGS)
AC_SUBST(NETCDF4_LIBADD)

])



AC_DEFUN([AC_CHECK_NETCDF4],[
	
# temporarily add NS_CPPFLAGS and NETCDF4_CPPFLAGS to CPPFLAGS
BACKUP_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $NS_CPPFLAGS $NETCDF4_CPPFLAGS"


AC_LANG_PUSH(C++)

AC_MSG_CHECKING([for netcdf4 headers])


AC_PREPROC_IFELSE(
	[AC_LANG_PROGRAM([[
		#include <ncFile.h>
		NcFile* file; 
		]],[[
		]]  )],
		[
		 AC_MSG_RESULT([yes])
		 found_netcdf4=yes
		[$1]
		],
		[
		 AC_MSG_RESULT([no])
		 found_netcdf4=no
		[$2]
		AC_MSG_WARN([could not find netcdf4])
		])


AM_CONDITIONAL([HAVE_NETCDF4], [test x$found_netcdf4 = xyes])

# Restoring to the initial value
CPPFLAGS="$BACKUP_CPPFLAGS"

AC_LANG_POP(C++)

])











