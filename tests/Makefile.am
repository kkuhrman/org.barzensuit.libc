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

AM_CFLAGS = $(WARN_CFLAGS) $(WERROR_CFLAGS) $(CFLAG_VISIBILITY) -pthread

# Add source file sub-directories to compiler search path
AM_CPPFLAGS = \
	-Iinc -I$(top_srcdir)/inc \
	-Ilib/gnulib -I$(top_srcdir)/lib/gnulib \
	-Isrc -I$(top_srcdir)/src

AM_LDFLAGS = -no-install
LDADD = ../src/libbzenc.la

check_PROGRAMS = \
	bzentest_dbug \
	bzentest_environment \
	bzentest_log \
	bzentest_nfl \
	bzentest_sbuf \
	bzentest_socket_create_local \
	bzentest_socket_create_inet \
	bzentest_socket_fail \
	bzentest_strm \
	bzentest_ut \
	bzentest_thread \
	bzentest_scratch \
	bzentest_yaml

TESTS = \
	bzentest_dbug \
	bzentest_environment \
	bzentest_log \
	bzentest_nfl \
	bzentest_sbuf \
	bzentest_socket_create_local \
	bzentest_socket_create_inet \
	bzentest_socket_fail \
	bzentest_strm \
	bzentest_ut \
	bzentest_thread \
	bzentest_scratch \
	bzentest_yaml

EXTRA_DIST = $(TESTS)

TESTS_ENVIRONMENT = \
	BZENTEST_TEMP_DIR=$(datarootdir)/bzentest \
	BZENTEST_SOCKET_LOCAL_ADDR="bzentestsock"; \
	export BZENTEST_TEMP_DIR BZENTEST_SOCKET_LOCAL_ADDR;

TESTS_ENVIRONMENT += \
	BZEN_LOG_DIR=$(datarootdir)/bzenlog; \
	export BZEN_LOG_DIR;

EXTRA_DIST += \
	$(top_srcdir)/tests/bzentest_yaml/invalidyaml.conf \
	$(top_srcdir)/tests/bzentest_yaml/validyanl.conf

TESTS_ENVIRONMENT += \
	BZENTEST_FILE_YAML_INVALID=$(top_srcdir)/tests/bzentest_yaml/invalidyaml.conf \
	BZENTEST_FILE_YAML_VALID=$(top_srcdir)/tests/bzentest_yaml/validyaml.conf; \
	export BZENTEST_FILE_YAML_INVALID BZENTEST_FILE_YAML_VALID;
