#include "ChronThreadControl.h"

#include <tlhelp32.h>


BOOL PauseAllThreads(std::vector<DWORD> threadIDs, DWORD numThreads)
{
	HANDLE threadHndl;

	for(int i = 0; i < numThreads; i++)
	{
		threadHndl = OpenThread(THREAD_SUSPEND_RESUME | THREAD_SET_CONTEXT | THREAD_GET_CONTEXT, TRUE, threadIDs[i]);

		if(threadHndl == 0)
			return FALSE;
		else 
			SuspendThread(threadHndl);
	}

	return TRUE;
}

BOOL ResumeAllThreads(std::vector<DWORD> threadIDs, DWORD numThreads)
{
	HANDLE threadHndl;

	for(int i = 0; i < numThreads; i++)
	{
		threadHndl = OpenThread(THREAD_SUSPEND_RESUME | THREAD_SET_CONTEXT | THREAD_GET_CONTEXT, TRUE, threadIDs[i]);

		if(threadHndl == 0)
			return FALSE;
		else 
			ResumeThread(threadHndl);
	}

	return TRUE;
}

BOOL ListProcessThreads(DWORD dwOwnerPID, DWORD *numThreads, std::vector<DWORD>* threadIDs) 
{ 
  HANDLE hThreadSnap = INVALID_HANDLE_VALUE; 
  THREADENTRY32 te32; 
 
  // Take a snapshot of all running threads  
  hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, 0 ); 
  if( hThreadSnap == INVALID_HANDLE_VALUE ) 
    return( FALSE ); 
 
  // Fill in the size of the structure before using it. 
  te32.dwSize = sizeof(THREADENTRY32 ); 
 
  // Retrieve information about the first thread,
  // and exit if unsuccessful
  if( !Thread32First( hThreadSnap, &te32 ) ) 
  {
	MessageBox(NULL,TEXT("Thread32First"),TEXT("ListError"),MB_OK);
	//printError( "Thread32First" ); // Show cause of failure
    CloseHandle( hThreadSnap );    // Must clean up the
                                   //   snapshot object!
    return( FALSE );
  }

  // Now walk the thread list of the system,
  // and display information about each thread
  // associated with the specified process
  do 
  { 
    if( te32.th32OwnerProcessID == dwOwnerPID )
    {
		if (threadIDs)
			(*threadIDs).push_back(te32.th32ThreadID);
		if (numThreads)
			(*numThreads)++;
    }
  } while( Thread32Next(hThreadSnap, &te32 ) ); 

//  Don't forget to clean up the snapshot object.
  CloseHandle( hThreadSnap );
  return( TRUE );
}