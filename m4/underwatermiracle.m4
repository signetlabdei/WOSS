#
# Copyright (c) 2007 Regents of the SIGNET lab, University of Padova.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of the University of Padova (SIGNET lab) nor the 
#    names of its contributors may be used to endorse or promote products 
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#


AC_DEFUN([AC_ARG_WITH_UNDERWATERMIRACLE],[

UNDERWATERMIRACLE_PATH=''
UNDERWATERMIRACLE_CPPLAGS=''
UNDERWATERMIRACLE_LDFLAGS=''
UNDERWATERMIRACLE_LIBADD=''

AC_ARG_WITH([underwatermiracle],
	[AS_HELP_STRING([--with-underwatermiracle=<directory>],
			[use underwatermiracle installation in <directory>])],
	[
		if test "x$withval" != "xno" ; then

   		     if test -d $withval ; then

   			UNDERWATERMIRACLE_PATH="${withval}"

			relevantheaderfile="${UNDERWATERMIRACLE_PATH}/src/underwater.h"
	    		if test ! -f "${relevantheaderfile}"  ; then
			  	AC_MSG_ERROR([could not find ${relevantheaderfile}, 
  is --with-underwatermiracle=${withval} correct?])
			fi		


			for dir in  \ 
				src
			do

			UNDERWATERMIRACLE_CPPFLAGS="$UNDERWATERMIRACLE_CPPFLAGS -I${UNDERWATERMIRACLE_PATH}/${dir}"
			UNDERWATERMIRACLE_LDFLAGS="$UNDERWATERMIRACLE_LDFLAGS -L${UNDERWATERMIRACLE_PATH}/${dir}"

			done

			for lib in \
				uwm
			do
				UNDERWATERMIRACLE_LIBADD="$UNDERWATERMIRACLE_LIBADD -l${lib}"
			done	



			UNDERWATERMIRACLE_DISTCHECK_CONFIGURE_FLAGS="--with-underwatermiracle=$withval"
			AC_SUBST(UNDERWATERMIRACLE_DISTCHECK_CONFIGURE_FLAGS)

   		     else	

			AC_MSG_ERROR([underwatermiracle path $withval is not a directory])
	
		     fi

		fi

	])


AC_SUBST(UNDERWATERMIRACLE_CPPFLAGS)
AC_SUBST(UNDERWATERMIRACLE_LDFLAGS)
AC_SUBST(UNDERWATERMIRACLE_LIBADD)

])



AC_DEFUN([AC_CHECK_UNDERWATERMIRACLE],[
	
# temporarily add NS_CPPFLAGS and UNDERWATERMIRACLE_CPPFLAGS to CPPFLAGS
BACKUP_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $NS_CPPFLAGS $UNDERWATERMIRACLE_CPPFLAGS"


AC_LANG_PUSH(C++)

AC_MSG_CHECKING([for underwatermiracle headers])


AC_PREPROC_IFELSE(
	[AC_LANG_PROGRAM([[
		#include<underwater.h>
		Underwater* uwp; 
		]],[[
		]]  )],
		[
		 AC_MSG_RESULT([yes])
		 found_underwatermiracle=yes
		[$1]
		],
		[
		 AC_MSG_RESULT([no])
		 found_underwatermiracle=no
		[$2]
		#AC_MSG_ERROR([could not find underwatermiracle])
		])


AM_CONDITIONAL([HAVE_UNDERWATERMIRACLE], [test x$found_underwatermiracle = xyes])

# Restoring to the initial value
CPPFLAGS="$BACKUP_CPPFLAGS"

AC_LANG_POP(C++)

])











