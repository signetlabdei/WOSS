#!/bin/sh

aclocal -I m4 --force && libtoolize --force && automake --foreign --add-missing && autoconf
