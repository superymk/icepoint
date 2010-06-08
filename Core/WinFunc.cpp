#include "WinFunc.h"

#include <tchar.h>

pfnNtQueryInformationProcess gNtQueryInformationProcess;

static HMODULE hNtDll = NULL;
static BOOL Load_NtQueryInformationProcess()
{
	// Load NTDLL Library and get entry address
    // for NtQueryInformationProcess
    hNtDll = LoadLibrary(_T("ntdll.dll"));
    if(hNtDll == NULL) return FALSE;

    gNtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hNtDll,
                                                        "NtQueryInformationProcess");
    if(gNtQueryInformationProcess == NULL) {
        FreeLibrary(hNtDll);
        return FALSE;
    }
    return TRUE;
}
/*
 * Dump necessary undocumented WinAPIs
 * including: NtQueryInformationProcess
 */
BOOL LoadWinFunc(VOID)
{
	return Load_NtQueryInformationProcess();
}

BOOL UnloadWinFunc(VOID)
{
	if (gNtQueryInformationProcess)
		gNtQueryInformationProcess = NULL;
	else
		return FALSE;

	if (!hNtDll)
		FreeLibrary(hNtDll);
	else
		return FALSE;

	return TRUE;
}