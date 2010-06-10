#include "WinFunc.h"

#include <tchar.h>

pfnNtQueryInformationProcess gNtQueryInformationProcess;
pfnNtQueryInformationThread gNtQueryInformationThread;

static HMODULE hNtDll = NULL;
static BOOL Load_NtQueryInformationProcess()
{
	// Load NTDLL Library and get entry address
    // for NtQueryInformationProcess
	if (!hNtDll)
		hNtDll = LoadLibrary(_T("ntdll.dll"));
    if (!hNtDll) return FALSE;

    gNtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hNtDll,
                                                        "NtQueryInformationProcess");
    if(gNtQueryInformationProcess == NULL) {
        FreeLibrary(hNtDll);
        return FALSE;
    }
    return TRUE;
}

static BOOL Load_NtQueryInformationThread()
{
	// Load NTDLL Library and get entry address
    // for NtQueryInformationProcess
	if (!hNtDll)
		hNtDll = LoadLibrary(_T("ntdll.dll"));
    if (!hNtDll) return FALSE;

    gNtQueryInformationThread = (pfnNtQueryInformationThread)GetProcAddress(hNtDll,
                                                        "NtQueryInformationThread");
    if(gNtQueryInformationThread == NULL) {
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
	return Load_NtQueryInformationProcess() && 
		Load_NtQueryInformationThread();
}

VOID UnloadWinFunc(VOID)
{
	if (gNtQueryInformationProcess)
		gNtQueryInformationProcess = NULL;

	if(gNtQueryInformationThread)
		gNtQueryInformationThread = NULL;

	if (!hNtDll)
		FreeLibrary(hNtDll);
}