
#include <windows.h>
#include <vector> 

extern BOOL PauseAllThreads(std::vector<DWORD> threadIDs, DWORD numThreads);
extern BOOL ResumeAllThreads(std::vector<DWORD> threadIDs, DWORD numThreads);
extern BOOL ListProcessThreads(DWORD dwOwnerPID, DWORD *numThreads, std::vector<DWORD>* threadIDs);