#include <vector>

#include "Chronosphere.h"
#include "WinFunc.h"
#include "ChronThreadControl.h"
#include "Windef.h"
#include "ChronProcInfo.h"

#define THREAD_NUM	9 //TODO: A better solution to retrieve thread amount

// Heap Related
// Ugly design, this will be replaced by GetProcessHeaps & HeapWalk()
char HeapContent[0x300000];

static PPEB GetPebBaseAddress(HANDLE hProcess);
static PTEB GetTebBaseAddress(HANDLE hThread);
static BOOL SetPrivilege(
    HANDLE hToken,          // token handle
    LPCTSTR Privilege,      // Privilege to enable/disable
    BOOL bEnablePrivilege   // TRUE to enable.  FALSE to disable
);

CHRON_STATUS ChronInit(VOID)
{
	BOOL status = LoadWinFunc();
	return (status == TRUE)?CHRON_SUCCESS:CHRON_UNSUCCESSFUL;
}
CHRON_STATUS ChronFinalize(VOID)
{
	UnloadWinFunc();
	return CHRON_SUCCESS;
}
/* Save the state of the indicate process.
 * Details:
 * 1) Assume the target process has already been created,
 * So get its handle by invoking OpenProcess()
 * 2) Suspend all threads of this process.
 * 3) Dump all thread's stack
 * 4) Dump all thread's context by invoking GetThreadContext()
 * 5) Dump the global data segment (.data) of this process
 * 6) Walk heap table of this process and dump the allocated 
 * heaps
 * 7) Write the indicated file with the process's absolute 
 * path and filename.
 * 8) Serialize all pieces to the file (given in <filepath>
 * 9) Close process handles and file handles.
 *
 * RETURNS:
 * CHRON_SUCCESS on success
 * CHRON_ERR_LISTTHREAD on enumerating threads error.
 * CHRON_ERR_SUSPENDTHREAD on enumerating threads error.
 * CHRON_ERR_GETCONTEXT on retrieving thread context error.
 * CHRON_ERR_ALLOCMEM if memory can't be allocated
 * CHRON_ERR_OPEN_PROCESS if process can't be opened.
 * CHRON_IO_ERROR if any i/o error occurs.
 * CHRON_ERR_READMEM on reading memory error
 */
PROC_STATE processState;
CHRON_STATUS SaveProcessState(DWORD pid, const char* filepath)
{
	BOOL status = FALSE;
	HANDLE hProcess = NULL, hToken = NULL;
	PPEB peb = NULL;
	std::vector<DWORD> threadIDs;
	DWORD numThreads = 0;
	HANDLE hThread;
	//PROC_STATE processState;
	PBYTE buf = NULL;
	DWORD read = 0, bufLength = 0;
	CONTEXT tContext = {0};
	char exefilePath[MAX_PATH];

	// Prompt privilege to be SeDebugPrivilege
	if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken))
	{
		if (GetLastError() == ERROR_NO_TOKEN)
        {
            if (!ImpersonateSelf(SecurityImpersonation))
				return CHRON_UNSUCCESSFUL;

            if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, &hToken))
				return CHRON_UNSUCCESSFUL;
         }
		else
			return CHRON_UNSUCCESSFUL;
	}
	if(!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
		return CHRON_UNSUCCESSFUL;

	// Open Process
	hProcess = OpenProcess( PROCESS_ALL_ACCESS | PROCESS_VM_OPERATION 
		& ~PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid );

	if(!hProcess)
		return CHRON_ERR_OPEN_PROCESS;

	peb = GetPebBaseAddress(hProcess);
	Sleep(1000);//Ensure all threads are completely created

	status = ListProcessThreads(pid, &numThreads,&threadIDs);
	if(!status)
	{
		MessageBox(0, "SaveProcessState: Error in enumerating threads", "SaveProcessState", 0);
		return CHRON_ERR_LISTTHREAD;
	}

	status = PauseAllThreads(threadIDs, numThreads);
	if(!status)
	{
		MessageBox(0, "SaveProcessState: Error in suspending threads", "SaveProcessState", 0);
		return CHRON_ERR_SUSPENDTHREAD;
	}

	// Dump threads' stack
	for(DWORD threadIdx = 0; threadIdx < numThreads; ++threadIdx)
	{
		hThread = OpenThread(THREAD_QUERY_INFORMATION | READ_CONTROL, FALSE,threadIDs[threadIdx]);
		PTEB teb = GetTebBaseAddress(hThread);

		ReadProcessMemory(hProcess, &teb->Tib.StackBase, &processState.ThreadInfo[threadIdx].StackBase, 4, &read);
		ReadProcessMemory(hProcess, &teb->Tib.StackLimit, &processState.ThreadInfo[threadIdx].StackLimit, 4, &read);

		buf = processState.ThreadInfo[threadIdx].Content;
		bufLength = (DWORD)processState.ThreadInfo[threadIdx].StackBase - 
			(DWORD)processState.ThreadInfo[threadIdx].StackLimit;
		
		ReadProcessMemory(hProcess, processState.ThreadInfo[threadIdx].StackLimit, buf, 
			bufLength, &read);
		if(!read)
			return CHRON_ERR_READMEM;

		CloseHandle(hThread);
	}

	// Dump threads' context
	for(DWORD threadIdx = 0; threadIdx < numThreads; ++threadIdx)
	{
		hThread = OpenThread(THREAD_QUERY_INFORMATION | READ_CONTROL | THREAD_SET_CONTEXT | THREAD_GET_CONTEXT, FALSE, threadIDs[threadIdx]);
		tContext.ContextFlags = CONTEXT_FULL;

		if(GetThreadContext(hThread,&tContext) == 0)
			return CHRON_ERR_GETCONTEXT;
		processState.ThreadInfo[threadIdx].Context = tContext;
	
		CloseHandle(hThread);
	}

	// Dump process global data
	DWORD gsVSize = 0;
	PVOID gsVA = NULL;
	gsVA = GetProcGlobalSegVA(pid, &gsVSize, exefilePath);

	ReadProcessMemory(hProcess, gsVA, processState.GlobalSegContent, 
			gsVSize, &read);
	if(!read)
		return CHRON_ERR_READMEM;

	// Dump process heap 
	// Ugly design, this will be replaced by GetProcessHeaps & HeapWalk()
	//TODO : Need Fix, We don't support heap dump at the moment.
	DWORD heapAddr;

	ReadProcessMemory(hProcess, &peb->ProcessHeaps, &heapAddr, 4, &read);
	ReadProcessMemory(hProcess, (PVOID)heapAddr, &heapAddr, 4, &read);
	ReadProcessMemory(hProcess, (PVOID)heapAddr, HeapContent, 0x100000, &read);

	// Fill other elements in <processState>
	memcpy(processState.ImageFilePath, exefilePath, MAX_PATH);

	 // close handles
    CloseHandle(hToken);
    CloseHandle(hProcess);

	// Serialize all data to some file
	HANDLE hFile = NULL;
	DWORD writeByte = 0;

	hFile = CreateFile(filepath,               // file to create
		CREATE_ALWAYS,          // creat a new one and will overwrite the old one
		FILE_SHARE_READ | FILE_SHARE_WRITE,       // share for reading and writting
		NULL,                  // default security
		CREATE_ALWAYS,         // creat a new one and will overwrite the old one
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template
			
	if (hFile == INVALID_HANDLE_VALUE) 
		return CHRON_IO_ERROR;

	WriteFile(hFile, &processState, sizeof(PROC_STATE), &writeByte, NULL);
	if (writeByte != sizeof(PROC_STATE))
		return CHRON_IO_ERROR;

	WriteFile(hFile, HeapContent, HEAP_SIZE, &writeByte, NULL);
	if (writeByte != HEAP_SIZE)
		return CHRON_IO_ERROR;

 	return CHRON_SUCCESS;
}

/* Load the state of the indicate process.
 * Details:
 * 1) Create the a new instance of the frozen process
 * 2) Suspend all threads of this process
 * 2) Fill all threads' stack
 * 3) Fill all threads' context by invoking GetThreadContext()
 * 4) Fill the global data segment (.data) of this process
 * 5) Walk heap table of this process and fill the allocated 
 * heaps
 * 6) Resume all threads of this process
 * 7) Close process handles and file handles.
 * RETURNS:
 * CHRON_SUCCESS on success
 * CHRON_ERR_CREATE_PROCESS on create threads error.
 * CHRON_ERR_LISTTHREAD on enumerating threads error.
 * CHRON_ERR_SUSPENDTHREAD on enumerating threads error.
 * CHRON_ERR_RESUMETHREAD on resuming threads error
 * CHRON_ERROR_ALLOCMEM on allocating memory error
 * CHRON_ERR_WRITEMEM on writing memory error
 * CHRON_ERR_SETCONTEXT on filling thread context error.
 * CHRON_IO_ERROR if any i/o error occurs.
*/
CHRON_STATUS LoadProcessState(const char* filepath)
{
	BOOL status = FALSE;
	PROCESS_INFORMATION pi = {0};
	STARTUPINFO si = {0};
	HANDLE hProcess = NULL, hToken = NULL;
	std::vector<DWORD> threadIDs;
	DWORD numThreads = 0, pid;
	PBYTE buf = NULL;
	DWORD read = 0, bufLength = 0, write = 0;
	HANDLE hThread;
	PPEB peb = NULL;

	// TODO - Deserialize all data from some file
	HANDLE hFile = NULL;
	DWORD readBytes = 0;

	hFile = CreateFile(filepath,               // file to create
		OPEN_ALWAYS,          // creat a new one and will overwrite the old one
		FILE_SHARE_READ | FILE_SHARE_WRITE,       // share for reading and writting
		NULL,                  // default security
		CREATE_ALWAYS,         // creat a new one and will overwrite the old one
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template
			
	if (hFile == INVALID_HANDLE_VALUE) 
		return CHRON_IO_ERROR;

	status = ReadFile(hFile, &processState, sizeof(PROC_STATE), &readBytes, NULL);
	if (status && readBytes != sizeof(PROC_STATE))
		return CHRON_IO_ERROR;

	status = ReadFile(hFile, HeapContent, HEAP_SIZE, &readBytes, NULL);
	if (status && readBytes != HEAP_SIZE)
		return CHRON_IO_ERROR;

	// Create new process instance
	if( !CreateProcess(TEXT(processState.ImageFilePath), // No module name (use command line)
        NULL,        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE | PROCESS_VM_WRITE,//CREATE_NEW_CONSOLE,     // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    ) 
        return CHRON_ERR_CREATE_PROCESS;

	peb = GetPebBaseAddress(hProcess);
	Sleep(1000);
	hProcess = pi.hProcess;
	pid = pi.dwProcessId;

	status = ListProcessThreads(pid, &numThreads,&threadIDs);
	if(!status)
	{
		MessageBox(0, "SaveProcessState: Error in enumerating threads", "SaveProcessState", 0);
		return CHRON_ERR_LISTTHREAD;
	}

	status = PauseAllThreads(threadIDs, numThreads);
	if(!status)
	{
		MessageBox(0, "SaveProcessState: Error in suspending threads", "SaveProcessState", 0);
		return CHRON_ERR_SUSPENDTHREAD;
	}

	// Fill all thread stacks
	for(DWORD threadIdx = 0; threadIdx < numThreads; ++threadIdx)
	{
		buf = processState.ThreadInfo[threadIdx].Content;
		bufLength = (DWORD)processState.ThreadInfo[threadIdx].StackBase - 
			(DWORD)processState.ThreadInfo[threadIdx].StackLimit;

		if(!VirtualAllocEx(hProcess, processState.ThreadInfo[threadIdx].StackLimit, bufLength, 
			MEM_COMMIT, PAGE_EXECUTE_READWRITE)) 
			return CHRON_ERR_ALLOCMEM;
				
		if(!WriteProcessMemory(hProcess, processState.ThreadInfo[threadIdx].StackLimit, buf, 
			bufLength, &write))
			return CHRON_ERR_WRITEMEM;
	}

	// Fill threads' context
	for(DWORD threadIdx = 0; threadIdx < numThreads; ++threadIdx)
	{
		hThread = OpenThread(THREAD_QUERY_INFORMATION | READ_CONTROL | THREAD_SET_CONTEXT | THREAD_GET_CONTEXT, FALSE, threadIDs[threadIdx]);

		if(SetThreadContext(hThread,&processState.ThreadInfo[threadIdx].Context) == 0)
			return CHRON_ERR_SETCONTEXT;
	
		CloseHandle(hThread);
	}

	// Dump process global data
	DWORD gsVSize = 0;
	PVOID gsVA = NULL;
	gsVA = GetProcGlobalSegVA(pid, &gsVSize, NULL);

	WriteProcessMemory(hProcess, gsVA, processState.GlobalSegContent, 
			gsVSize, &write);
	if(!write)
		return CHRON_ERR_WRITEMEM;

	// Dump process heap 
	// Ugly design, this will be replaced by GetProcessHeaps & HeapWalk()
	//TODO : Need Fix, We don't support heap dump at the moment.
	DWORD heapAddr;

	ReadProcessMemory(hProcess, &peb->ProcessHeaps, &heapAddr, 4, &read);
	ReadProcessMemory(hProcess, (PVOID)heapAddr, &heapAddr, 4, &read);
	WriteProcessMemory(hProcess, (PVOID)heapAddr, HeapContent, 0x100000, &write);
	if(!write)
		return CHRON_ERR_WRITEMEM;

	status = ResumeAllThreads(threadIDs, numThreads);
	if(!status)
	{
		MessageBox(0, "LoadProcessState: Error in resuming threads", "LoadProcessState", 0);
		return CHRON_ERR_RESUMETHREAD;
	}
	return CHRON_UNSUCCESSFUL;
}

static PPEB GetPebBaseAddress(HANDLE hProcess)
{
	PROCESS_BASIC_INFORMATION pbi;

	gNtQueryInformationProcess(hProcess,ProcessBasicInformation, &pbi, sizeof(pbi),NULL);
	return (PPEB)pbi.PebBaseAddress;
}

static PTEB GetTebBaseAddress(HANDLE hThread)
{
	THREAD_BASIC_INFORMATION tbi;

	gNtQueryInformationThread(hThread, ThreadBasicInformation, &tbi, sizeof(tbi), NULL);
	return (PTEB)tbi.TebBaseAddress;
}

static BOOL SetPrivilege(
    HANDLE hToken,          // token handle
    LPCTSTR Privilege,      // Privilege to enable/disable
    BOOL bEnablePrivilege   // TRUE to enable.  FALSE to disable
)
{
    TOKEN_PRIVILEGES tp;
    LUID luid;
    TOKEN_PRIVILEGES tpPrevious;
    DWORD cbPrevious=sizeof(TOKEN_PRIVILEGES);

    if(!LookupPrivilegeValue( NULL, Privilege, &luid )) return FALSE;

    // 
    // first pass.  get current privilege setting
    // 
    tp.PrivilegeCount           = 1;
    tp.Privileges[0].Luid       = luid;
    tp.Privileges[0].Attributes = 0;

    AdjustTokenPrivileges(
            hToken,
            FALSE,
            &tp,
            sizeof(TOKEN_PRIVILEGES),
            &tpPrevious,
            &cbPrevious
            );

    if (GetLastError() != ERROR_SUCCESS) return FALSE;

    // 
    // second pass.  set privilege based on previous setting
    // 
    tpPrevious.PrivilegeCount       = 1;
    tpPrevious.Privileges[0].Luid   = luid;

    if(bEnablePrivilege) {
        tpPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
    }
    else {
        tpPrevious.Privileges[0].Attributes ^= (SE_PRIVILEGE_ENABLED &
            tpPrevious.Privileges[0].Attributes);
    }

    AdjustTokenPrivileges(
            hToken,
            FALSE,
            &tpPrevious,
            cbPrevious,
            NULL,
            NULL
            );

    if (GetLastError() != ERROR_SUCCESS) return FALSE;

    return TRUE;
}
