#pragma once
#ifndef HIDE_MODULE_H
#define HIDE_MODULE_H

#include<Windows.h>



typedef struct _UNICODE_STR
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR pBuffer;
} UNICODE_STR, * PUNICODE_STR;

typedef struct _LSA_UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
}
UNICODE_STRING, * PUNICODE_STRING;

typedef struct _PEB_LDR_DATA
{
	DWORD Length; // +0x00
	bool Initialized; // +0x04
	PVOID SsHandle; // +0x08
	LIST_ENTRY InLoadOrderModuleList; // +0x0c
	LIST_ENTRY InMemoryOrderModuleList; // +0x14
	LIST_ENTRY InInitializationOrderModuleList;// +0x1c
} PEB_LDR_DATA, * PPEB_LDR_DATA; // +0x24

typedef struct _RTL_USER_PROCESS_PARAMETERS {
	BYTE			Reserved1[16];
	PVOID			Reserved2[10];
	UNICODE_STR		ImagePathName;
	UNICODE_STR		CommandLine;
} RTL_USER_PROCESS_PARAMETERS, * PRTL_USER_PROCESS_PARAMETERS;

//PEB
typedef struct __PEB
{
	BYTE Reserved1[2];
	BYTE BeingDebugged;
#ifdef _WIN64
	BYTE Reserved2[21];
#else
	BYTE Reserved2[9];
#endif
	PPEB_LDR_DATA pLdr;
	PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
	BYTE Reserved3[520];
	BYTE Reserved4[136];
	ULONG SessionId;
} _PEB, * _PPEB;

typedef struct _LDR_MODULE
{
	LIST_ENTRY          InLoadOrderModuleList;
	LIST_ENTRY          InMemoryOrderModuleList;
	LIST_ENTRY          InInitializationOrderModuleList;
	void* BaseAddress;
	void* EntryPoint;
	ULONG               SizeOfImage;
	UNICODE_STRING   FullDllName;
	UNICODE_STRING      BaseDllName;
	ULONG               Flags;
	SHORT               LoadCount;
	SHORT               TlsIndex;
	HANDLE              SectionHandle;
	ULONG               CheckSum;
	ULONG               TimeDateStamp;
} LDR_MODULE, * PLDR_MODULE;

void hideModule(LPCSTR dllname);

#endif // HIDE_MODULE_H