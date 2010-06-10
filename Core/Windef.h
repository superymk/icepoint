#ifndef _WINDEF_H
#define _WINDEF_H

typedef ULONG	 ULONG_PTR;

typedef struct _PEB
{
	UCHAR Reserved1[0x90];
	PVOID** ProcessHeaps; // 90h			//[1]
	PVOID GdiSharedHandleTable; // 94h
	PVOID ProcessStarterHelper; // 98h
	PVOID GdiDCAttributeList; // 9Ch
	PVOID LoaderLock; // A0h
	ULONG OSMajorVersion; // A4h		//[2]
	ULONG OSMinorVersion; // A8h		//[2]
	ULONG OSBuildNumber; // ACh		//[2]
	ULONG OSPlatformId; // B0h		//[2]
	ULONG ImageSubSystem; // B4h		//[1]
	ULONG ImageSubSystemMajorVersion; // B8h	//[1]
	ULONG ImageSubSystemMinorVersion; // C0h	//[1]
	ULONG GdiHandleBuffer[0x22]; // C4h
	PVOID PostProcessInitRoutine;
	PVOID TlsExpansionBitmap;		//[3]
	ULONG TlsExpansionBitmapBits[32];	//[3]
	ULONG SessionId;		//[!!]
	ULARGE_INTEGER AppCompatFlags;	//[2]
	ULARGE_INTEGER AppCompatFlagsUser;	//[2]
	PVOID pShimData;
	PVOID AppCompatInfo;
	//UNICODE_STRING CSDVersion;	//[!!] //1f0h
	//PVOID ActivationContextData;
	//PVOID ProcessAssemblyStorageMap;
	//PVOID SystemDefaultActivationContextData;
	//PVOID SystemAssemblyStorageMap;
	//ULONG MinimumStackCommit;	//208h

	UCHAR Reserved2[0x208 + 4 - 0x1f0];
} PEB, *PPEB;

typedef struct _TEB {
	_NT_TIB Tib;
    BYTE Reserved1[1952-0x1c];
    PVOID Reserved2[412];
    PVOID TlsSlots[64];
    BYTE Reserved3[8];
    PVOID Reserved4[26];
    PVOID ReservedForOle;  // Windows 2000 only
    PVOID Reserved5[4];
    PVOID TlsExpansionSlots;
} TEB, *PTEB;

typedef struct _CLIENT_ID   {   
	HANDLE   UniqueProcess;   
	HANDLE   UniqueThread;   
  }   CLIENT_ID;   
typedef   CLIENT_ID   *PCLIENT_ID;   

typedef struct _PROCESS_BASIC_INFORMATION {
    PVOID Reserved1;
    PPEB PebBaseAddress;
    PVOID Reserved2[2];
    ULONG_PTR UniqueProcessId;
    PVOID Reserved3;
} PROCESS_BASIC_INFORMATION;

typedef struct _THREAD_BASIC_INFORMATION   {   
	LONG		ExitStatus;   
	PVOID		TebBaseAddress;   
	CLIENT_ID   ClientId;   
	LONG		AffinityMask;   
	LONG		Priority;   
	LONG		BasePriority;   
}   THREAD_BASIC_INFORMATION,   *PTHREAD_BASIC_INFORMATION;   

#endif