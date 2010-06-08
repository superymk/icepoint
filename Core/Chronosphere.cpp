#include <windows.h>
#include <vector>

#include "Chronosphere.h"
#include "WinFunc.h"
#include "ChronThreadControl.h"

#define THREAD_NUM	9 //TODO: A better solution to retrieve thread amount

static PPEB GetPebBaseAddress(HANDLE hProcess);

CHRON_STATUS ChronInit(VOID)
{
	BOOL status = LoadWinFunc();
	return (status == TRUE)?CHRON_SUCCESS:CHRON_UNSUCCESSFUL;
}
CHRON_STATUS ChronFinalize(VOID)
{
	BOOL status = UnloadWinFunc();
	return (status == TRUE)?CHRON_SUCCESS:CHRON_UNSUCCESSFUL;
}
/* Save the state of the indicate process.
 * Details:
 * 1) Assume the target process has already been created,
 * So get its handle by invoking OpenProcess()
 * 2) Suspend all threads of this process.
 * 3) Dump the main thread's stack
 * 4) Dump the other thread's stack
 * 5) Dump all thread's context by invoking GetThreadContext()
 * 6) Dump the global data segment (.data) of this process
 * 7) Walk heap table of this process and dump the allocated 
 * heaps
 * 8) Write the indicated file with the process's absolute 
 * path and filename.
 * 9) Serialize all pieces to the file (given in <filepath>
 * 10) Close process handles and file handles.
 *
 * RETURNS:
 * CHRON_SUCCESS on success
 * CHRON_E_LISTTHREAD on enumerating threads error.
 * CHRON_NO_MEM if memory can't be allocated
 * CHRON_FAILED_OPEN_PROCESS if process can't be opened.
 * CHRON_IO_ERROR if any i/o error occurs.
 */
CHRON_STATUS SaveProcessState(DWORD pid, const char* filepath)
{
	BOOL status = FALSE;
	HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pid ); 
	PPEB peb = GetPebBaseAddress(hProcess);
	std::vector<DWORD> threadIDs;
	DWORD numThreads;

	status = ListProcessThreads(pid, &numThreads,&threadIDs);
	if(!status)
	{
		MessageBox(0, "SaveProcessState: Error in enumerating threads", "SaveProcessState", 0);
		return CHRON_E_LISTTHREAD;
	}

	return CHRON_UNSUCCESSFUL;
}

/* Load the state of the indicate process.
 * Details:
 * 1) Create the a new instance of the frozen process
 * 2) Suspend all threads of this process
 * 3) Fill the main thread's stack
 

*/
CHRON_STATUS LoadProcessState(const char* filepath)
{
	return CHRON_UNSUCCESSFUL;
}

static PPEB GetPebBaseAddress(HANDLE hProcess)
{
	PROCESS_BASIC_INFORMATION pbi;

	gNtQueryInformationProcess(hProcess,ProcessBasicInformation,&pbi,sizeof(pbi),NULL);
	return (PPEB)pbi.PebBaseAddress;
}