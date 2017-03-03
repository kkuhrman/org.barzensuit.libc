# org.barzensuit.bzenc

Copyright (C) 2017 Kuhrman Technology Solutions LLC.

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Overview

Part of the Barzensuit RTC Project. This is the core C API (see documentation for details on the Barzensuit Project).

# Requirements

@todo: dependencies

# Implementation

libbzenc is provides the C language API to the core Barzensuit Project dependencies.

# Installation

It is highly recommend that Gnu autotools, gcc, lzip and m4 are installed in order to build the Barzensuit software and dependent libraries.

# Log Files

@todo: replace manual procedure with something a bit more elegant.

If user wished to have messages sent to log files s/he must create a directory and set write permissions for user. Set and export environment variable BZEN_LOG_DIR. Otherwise bzentest_log will fail and any messages sent to log facility will be ignored.
