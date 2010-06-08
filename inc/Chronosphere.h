/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
 * Copyright (C) Superymk <superymkfounder@hotmail.com>
 */
#pragma once 

#define CHRON_SUCCESS		0x10000000
#define CHRON_UNSUCCESSFUL	0x80000000
#define CHRON_NO_MEM		0x80000001
#define CHRON_E_LISTTHREAD	0x80000002

typedef ULONG		CHRON_STATUS;

CHRON_STATUS ChronInit(VOID);
CHRON_STATUS ChronFinalize(VOID);
CHRON_STATUS SaveProcessState(DWORD pid, const char* filepath);
CHRON_STATUS LoadProcessState(const char* filepath);