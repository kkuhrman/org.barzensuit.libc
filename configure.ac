## Process this file with autoconf to produce a configure script.
# @copyright:	Copyright (C) 2017 Kuhrman Technology Solutions LLC.
#
# This file is part of libbzenc.
#
# @license:	GPLv3+: GNU GPL version 3
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# @author:	Karl Kuhrman
#

AC_INIT([bzenlibc], [0.1.0], [bugs@barzensuit.org])

# Must come before AM_INIT_AUTOMAKE.
AC_CONFIG_AUX_DIR([build-aux])

# These settings have automake report all warnings as errors during early 
# development and ignore several requirements of Gnu distros that will be 
# added later such as ChangeLog, AUTHORS, etc.
# @see: https://www.gnu.org/software/automake/manual/automake.html#Hello-World
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Specify location of local autoconf macros
AC_CONFIG_MACRO_DIR([m4])

# Minimum Autoconf version required.
AC_PREREQ([2.69])

# Where to generate output; srcdir location.
AC_CONFIG_HEADERS([inc/config.h:config.in])

# Safety check - make sure directory autoconf is told contains file in fact does.
AC_CONFIG_SRCDIR([inc/bzensock.h])
AC_CONFIG_SRCDIR([src/bzensock.c])

# Copyright will apply as long as these sources are in use, e.g., are
# being compiled, which is reasonable year to claim the copyright.
AC_DEFINE([COPYRIGHT_YEAR], [m4_esyscmd([date +%Y])], [year in copyright message])
AC_DEFINE([COPYRIGHT_HOLDER], ["Kuhrman Technology Solutions LLC"], [copyright holder])

# Checks for programs.
AC_PROG_CC

# Libtool tests
AM_PROG_AR
LT_INIT

# Minimum Libtool version required.
LT_PREREQ([2.4])

AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/libbzenc.pc
 tests/Makefile
])

AC_OUTPUT

AC_MSG_NOTICE([summary of build options:

  version:          ${VERSION}
  Host type:        ${host}
  Install prefix:   ${prefix}
  Compiler:         ${CC}
  Warning flags:    errors: ${WERROR_CFLAGS} warnings: ${WARN_CFLAGS}
  Library types:    Shared=${enable_shared}, Static=${enable_static}
])
