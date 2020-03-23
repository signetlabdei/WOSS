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


AC_INIT(WOSS,1.8.0)
AM_INIT_AUTOMAKE

AC_MSG_NOTICE([WOSS AC_PACKAGE_VERSION])

AC_PROG_CXX
AC_PROG_MAKE_SET

AC_DISABLE_STATIC 
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

AC_PATH_NS_ALLINONE([have_nsallinone=yes],[have_nsallinone=no])

AC_ARG_WITH_NSMIRACLE
AC_CHECK_NSMIRACLE([have_nsmiracle=yes],[have_nsmiracle=no])
#if test x$have_nsmiracle != xyes ; then
#  AC_MSG_ERROR([Could not find nsmiracle,
#    is --with-nsmiracle set correctly?])
#fi  

AC_ARG_WITH_NETCDF
AC_CHECK_NETCDF([have_netcdf=yes],[have_netcdf=no])
#if test x$have_netcdf != xyes ; then
#  AC_MSG_ERROR([Could not find netcdf,
#    is --with-netcdf set correctly?])
#fi  


AC_ARG_WITH(pthread, [--with-pthread      enables pthread extensions])


UW_WOSS_CPPFLAGS="$UW_WOSS_CPPFLAGS "'-I$(top_srcdir)/woss/woss_def'
UW_WOSS_CPPFLAGS="$UW_WOSS_CPPFLAGS "'-I$(top_srcdir)/woss/woss_db'
UW_WOSS_WARN="$UW_WOSS_WARN "'-Wall -Werror'


UW_WOSS_PHY_CPPFLAGS="$UW_WOSS_PHY_CPPFLAGS "'-I$(top_srcdir)/woss/woss_def'
UW_WOSS_PHY_CPPFLAGS="$UW_WOSS_PHY_CPPFLAGS "'-I$(top_srcdir)/woss/woss_db'
UW_WOSS_PHY_CPPFLAGS="$UW_WOSS_PHY_CPPFLAGS "'-I$(top_srcdir)/woss'
UW_WOSS_PHY_WARN="$UW_WOSS_PHY_WARN "'-Wall -Werror -Wno-write-strings'

UW_WOSS_LDFLAGS='-L$(top_srcdir)/woss/woss_def'
UW_WOSS_LDFLAGS="$UW_WOSS_LDFLAGS "'-L$(top_srcdir)/woss/woss_db'

UW_WOSS_PHY_LDFLAGS="$UW_WOSS_PHY_LDFLAGS "'-L$(top_srcdir)/woss/woss_def'
UW_WOSS_PHY_LDFLAGS="$UW_WOSS_PHY_LDFLAGS "'-L$(top_srcdir)/woss/woss_db'
UW_WOSS_PHY_LDFLAGS="$UW_WOSS_PHY_LDFLAGS "'-L$(top_srcdir)/woss'

UW_WOSS_PHY_LIBADD="$UW_WOSS_PHY_LIBADD -lWOSS"


if test "$have_netcdf" == "yes"
then
  UW_WOSS_CPPFLAGS="$UW_WOSS_CPPFLAGS "'-DWOSS_NETCDF_SUPPORT'
  UW_WOSS_PHY_CPPFLAGS="$UW_WOSS_PHY_CPPFLAGS "'-DWOSS_NETCDF_SUPPORT'
  AC_MSG_NOTICE([NETCDF support will be built.])
else
  AC_MSG_NOTICE([NETCDF support will NOT be built.])
fi

if test "$have_nsallinone" == "yes"
then
  if test "$have_nsmiracle" == "yes"
  then
    UW_WOSS_CPPFLAGS="$UW_WOSS_CPPFLAGS "'-DWOSS_NS_MIRACLE_SUPPORT'
    UW_WOSS_PHY_CPPFLAGS="$UW_WOSS_PHY_CPPFLAGS "'-DWOSS_NS_MIRACLE_SUPPORT'
    AC_MSG_NOTICE([NS MIRACLE support will be built.])
  else
    AC_MSG_NOTICE([NS ALLINONE path is correct, but NS MIRACLE is NOT, NS MIRACLE support will NOT be built.])
  fi
else
  AC_MSG_NOTICE([NS ALLINONE path is NOT correct, NS MIRACLE support will NOT be built.])
fi

if test "$with_pthread" == "yes"
then
  AC_MSG_NOTICE([Pthread support will be built.])
  UW_WOSS_CPPFLAGS="$UW_WOSS_CPPFLAGS "'-DWOSS_MULTITHREAD -pthread'
  UW_WOSS_PHY_CPPFLAGS="$UW_WOSS_PHY_CPPFLAGS "'-DWOSS_MULTITHREAD -pthread'
  UW_WOSS_LIBADD="$UW_WOSS_LIBADD "'-lpthread'
  UW_WOSS_PHY_LIBADD="$UW_WOSS_PHY_LIBADD "'-lpthread'
else
  AC_MSG_NOTICE([Pthread support will NOT be built.])
fi


AC_SUBST(UW_WOSS_CPPFLAGS)
AC_SUBST(UW_WOSS_PHY_CPPFLAGS)
AC_SUBST(UW_WOSS_WARN)
AC_SUBST(UW_WOSS_PHY_WARN)

AC_SUBST(UW_WOSS_LDFLAGS)
AC_SUBST(UW_WOSS_PHY_LDFLAGS)

AC_SUBST(UW_WOSS_LIBADD)
AC_SUBST(UW_WOSS_PHY_LIBADD)

AM_CONDITIONAL([NS_MIRACLE_BUILD], [test "x$have_nsmiracle" = xyes -a "x$have_nsallinone" = xyes])

AC_DEFINE(CPP_NAMESPACE,std)

AC_CONFIG_FILES([Makefile
                            doxy/Makefile
                            doxy/imgs/Makefile
                            woss/woss_def/Makefile
                            woss/woss_db/Makefile
                            woss/Makefile
                            m4/Makefile
                            samples/Makefile
                            woss_phy/Makefile
                            woss_phy/tcl_hooks/Makefile
                            ])
		
AC_OUTPUT
