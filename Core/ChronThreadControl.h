
#include <windows.h>
#include <vector> 

extern BOOL PauseAllThreads(std::vector<DWORD> threadIDs, DWORD numThreads);
extern BOOL ResumeAllThreads(std::vector<DWORD> threadIDs, DWORD numThreads);
extern BOOL ListProcessThreads(DWORD dwOwnerPID, DWORD *pnumThreads, std::vector<DWORD>* pthreadIDs);