/**
 * @file:	bzenipc.h
 * @brief:	Interprocess communication and process forking functions.
 * 
 * Barzensuit comprises a modular architecture for RTC services, which relies 
 * on IPC over sockets using a protocol adapted from TCP and other common
 * standards. The procedures in the Barzensuit Service Protocol (BSP) are:
 * Engagement - Service module reports availability to service broker
 * Brokering - service broker arranges session between client and service
 * Discovery - [future release] service broker finds module to suit client request
 * Enlistment - [future release] idle module is put into action to provide service
 * Streaming - unformatted data is transmitted between client and service module
 *
 * @see:        https://www.barzensuit.org/bps
 *
 * @copyright:	Copyright (C) 2017 Kuhrman Technology Solutions LLC
 * @license:	GPLv3+: GNU GPL version 3
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _BZEN_IPC_H_
#define _BZEN_IPC_H_

#include <config.h>
#include "bzenpriv.h"

#endif /* _BZEN_IPC_H_ */
