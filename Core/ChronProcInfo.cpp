#include <windows.h>
#include <TlHelp32.h>
#include <stdio.h>
#include <string.h>
#include <winnt.h>
#include <stddef.h>
#include <psapi.h>

#include <iostream>
#include <fstream>
#include "ChronProcInfo.h"
using namespace std;

static PVOID GetFileGlobalSegVA(const char * path, DWORD* vSize);
static const char* GetFileNameViaPID(DWORD pid);
static const char* FormatPath(char* filePath);

/* return NULL on error */
PVOID GetProcGlobalSegVA(DWORD pid, DWORD* vSize, char * formatPath)
{   
	const char* filePath = FormatPath((char*)GetFileNameViaPID(pid));

	if(formatPath)
		memcpy(formatPath, filePath, MAX_PATH);
	return GetFileGlobalSegVA(filePath, vSize);
}

static const char* FormatPath(char* filePath)
{
	static char formatPath[MAX_PATH] = {0};

	for( int i = 0; i < strlen(filePath); i++)
		if (filePath[i] == '\\')
			formatPath[i] = '/';
		else
			formatPath[i] = filePath[i];
	return formatPath;
}
static PVOID GetFileGlobalSegVA(const char * path, DWORD* vSize)
{
	static BYTE buffer[8192] = {0};
	ifstream in(path, ios::in); 

	IMAGE_DOS_HEADER mzh;
	PIMAGE_DOS_HEADER pmzh = &mzh;
	IMAGE_NT_HEADERS nth;
	PIMAGE_NT_HEADERS pnth = &nth;
	IMAGE_SECTION_HEADER sec;
	PIMAGE_SECTION_HEADER psec = &sec;
	DWORD section_count, nth_loc, file_size, base_addr;
	DWORD gsVA = 0;


	in.seekg(0, ios::beg); 
	in.read((char*)pmzh, sizeof(IMAGE_DOS_HEADER));
	
	in.seekg(nth_loc = pmzh->e_lfanew, ios::beg);
	in.read((char*)pnth, sizeof(IMAGE_NT_HEADERS));

	section_count = pnth->FileHeader.NumberOfSections;
	base_addr = pnth->OptionalHeader.ImageBase;
	
	for (int i = 0; i < section_count; ++i) {
		in.seekg(nth_loc + offsetof(IMAGE_NT_HEADERS, OptionalHeader) + 
			pnth->FileHeader.SizeOfOptionalHeader + i*sizeof(IMAGE_SECTION_HEADER), ios::beg);
		in.read((char*)psec, sizeof(IMAGE_SECTION_HEADER));

		if (!strncmp((const char*)psec->Name, ".data", 6)) {
			gsVA = psec->VirtualAddress + base_addr;
			if (vSize)
				*vSize = psec->Misc.VirtualSize;
			return (PVOID) gsVA;
		}
	}

	return NULL;
}

static const char* GetFileNameViaPID(DWORD pid)
{
	static char szProcessName[MAX_PATH] = "unknown ";

    //   Get   a   handle   to   the   process.
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                             FALSE, pid   );
    //   Get   the   process   name.

    if (hProcess)
    {
		HMODULE   hMod;
		DWORD   cbNeeded;

		if (EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded))
			GetModuleFileNameEx(hProcess, hMod, szProcessName, sizeof(szProcessName));
     }

	CloseHandle( hProcess); 
	return szProcessName;
}