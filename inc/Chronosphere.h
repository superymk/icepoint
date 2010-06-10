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
#include <windows.h>

#define CHRON_SUCCESS				0x10000000
#define CHRON_UNSUCCESSFUL			0x80000000
#define CHRON_ERR_ALLOCMEM			0x80000001
#define CHRON_ERR_LISTTHREAD		0x80000002
#define CHRON_ERR_SUSPENDTHREAD		0x80000003
#define CHRON_ERR_GETCONTEXT		0x80000004
#define CHRON_ERR_SETCONTEXT		0x80000005
#define CHRON_ERR_OPEN_PROCESS		0x80000006
#define CHRON_ERR_CREATE_PROCESS	0x80000007
#define CHRON_ERR_WRITEMEM			0x80000008
#define CHRON_ERR_READMEM			0x80000009
#define CHRON_ERR_RESUMETHREAD		0x8000000A
#define CHRON_IO_ERROR				0x8000000B

#define PAGESIZE	4096
#define MAX_THREAD	10

typedef DWORD		CHRON_STATUS;

typedef struct _THREAD_INFO{   
	PVOID StackBase;
	PVOID StackLimit;
	BYTE  Content[10*PAGESIZE]; //TODO: a better solution to decrease fragments 
									//(The default thread stack size is 1MB) 
	CONTEXT Context;
} THREAD_INFO;// *PTHREAD_STACK;   

typedef struct _PROC_STATE{   
	char	ImageFilePath[MAX_PATH];
	THREAD_INFO ThreadInfo[MAX_THREAD]; //TODO: a better solution to decrease fragments 
								// We suppose the target process has 20 threads at most.
	char GlobalSegContent[5*PAGESIZE];//TODO: a better solution to decrease fragments 
								// We suppose the target process has 5 threads at most.

} PROC_STATE, *PPROC_STATE;   

// Heap Related
// Ugly design, this will be replaced by GetProcessHeaps & HeapWalk()
#define HEAP_SIZE	0x300000
extern	char HeapContent[HEAP_SIZE];

CHRON_STATUS ChronInit(VOID);
CHRON_STATUS ChronFinalize(VOID);
CHRON_STATUS SaveProcessState(DWORD pid, const char* filepath);
CHRON_STATUS LoadProcessState(const char* filepath);