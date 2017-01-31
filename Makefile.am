## Process this file with automake to produce Makefile.in
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

# @todo: DISTCHECK_CONFIGURE_FLAGS

# Include Gnu Lib macros in distribution
EXTRA_DIST = m4/gnulib-cache.m4

# automake will search for MakefileS in the following subdirs.
SUBDIRS = lib src tests

# @todo: ENABLE_DOCS

# Find libtool macros.
ACLOCAL_AMFLAGS = -I m4
