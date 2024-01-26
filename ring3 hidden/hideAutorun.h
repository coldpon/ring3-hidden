#pragma once
#ifndef HIDE_AUTO_RUN_H
#define HIDE_AUTO_RUN_H

#include <windows.h>
#include <stdlib.h>
#include<stdio.h>

#define OBJ_CASE_INSENSITIVE 	0x00000040L
#define InitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );        \
   (p)->RootDirectory = r;                           \
    (p)->Attributes = a;                              \
    (p)->ObjectName = n;                              \
    (p)->SecurityDescriptor = s;                      \
    (p)->SecurityQualityOfService = NULL;             \
    }
//typedef LONG NTSTATUS, * PNTSTATUS;
typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} myUNICODE_STRING;
typedef myUNICODE_STRING* PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES {
	ULONG Length;
	HANDLE RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor;
	PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES* POBJECT_ATTRIBUTES;

typedef struct _STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PCHAR Buffer;
} STRING;
typedef STRING* PSTRING;
typedef STRING OEM_STRING;
typedef STRING* POEM_STRING;
typedef STRING ANSI_STRING;
typedef STRING* PANSI_STRING;

typedef enum _KEY_INFORMATION_CLASS
{
	KeyBasicInformation,
	KeyNodeInformation,
	KeyFullInformation,
	KeyNameInformation
} KEY_INFORMATION_CLASS;

typedef NTSTATUS(STDAPICALLTYPE RTLINITANSISTRING)
(
	IN OUT PANSI_STRING DestinationString,
	IN LPCSTR SourceString
	);
typedef RTLINITANSISTRING FAR* LPRTLINITANSISTRING;

typedef NTSTATUS(STDAPICALLTYPE RTLANSISTRINGTOUNICODESTRING)
(
	IN OUT PUNICODE_STRING	DestinationString,
	IN PANSI_STRING			SourceString,
	IN BOOLEAN				AllocateDestinationString
	);
typedef RTLANSISTRINGTOUNICODESTRING FAR* LPRTLANSISTRINGTOUNICODESTRING;

typedef struct _KEY_BASIC_INFORMATION
{
	LARGE_INTEGER LastWriteTime;
	ULONG TitleIndex;
	ULONG NameLength;
	WCHAR Name[1];
} KEY_BASIC_INFORMATION;
typedef KEY_BASIC_INFORMATION* PKEY_BASIC_INFORMATION;

typedef struct _KEY_VALUE_PARTIAL_INFORMATION
{
	ULONG TitleIndex;
	ULONG Type;
	
	ULONG DataLength;
	UCHAR Data[1];
} KEY_VALUE_PARTIAL_INFORMATION;
typedef KEY_VALUE_PARTIAL_INFORMATION* PKEY_VALUE_PARTIAL_INFORMATION;

typedef enum _KEY_VALUE_INFORMATION_CLASS
{
	KeyValueBasicInformation,
	KeyValueFullInformation,
	KeyValuePartialInformation,
} KEY_VALUE_INFORMATION_CLASS;

typedef NTSTATUS(STDAPICALLTYPE NTOPENKEY)
(
	IN HANDLE				KeyHandle,
	IN ULONG				DesiredAccess,
	IN POBJECT_ATTRIBUTES	ObjectAttributes
	);
typedef NTOPENKEY FAR* LPNTOPENKEY;

typedef NTSTATUS(STDAPICALLTYPE NTCREATEKEY)
(
	IN HANDLE				KeyHandle,
	IN ULONG				DesiredAccess,
	IN POBJECT_ATTRIBUTES	ObjectAttributes,
	IN ULONG				TitleIndex,
	IN PUNICODE_STRING		Class,
	IN ULONG				CreateOptions,
	OUT PULONG				Disposition
);
typedef NTCREATEKEY FAR* LPNTCREATEKEY;

typedef NTSTATUS(STDAPICALLTYPE NTDELETEKEY)
(
	IN HANDLE KeyHandle
	);
typedef NTDELETEKEY FAR* LPNTDELETEKEY;

typedef NTSTATUS(STDAPICALLTYPE NTSETVALUEKEY)
(
	IN HANDLE			KeyHandle,
	IN PUNICODE_STRING	ValueName,
	IN ULONG			TitleIndex,
	IN ULONG			Type,
	IN PVOID			Data,
	IN ULONG			DataSize
	);
typedef NTSETVALUEKEY FAR* LPNTSETVALUEKEY;

typedef NTSTATUS(STDAPICALLTYPE NTQUERYVALUEKEY)
(
	IN HANDLE			KeyHandle,
	IN PUNICODE_STRING	ValueName,
	IN KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
	OUT PVOID			KeyValueInformation,
	IN ULONG			Length,
	OUT PULONG			ResultLength
	);
typedef NTQUERYVALUEKEY FAR* LPNTQUERYVALUEKEY;

typedef NTSTATUS(STDAPICALLTYPE NTDELETEVALUEKEY)
(
	IN HANDLE			KeyHandle,
	IN PUNICODE_STRING	ValueName
	);
typedef NTDELETEVALUEKEY FAR* LPNTDELETEVALUEKEY;

typedef NTSTATUS(STDAPICALLTYPE NTCLOSE)
(
	IN HANDLE KeyHandle
	);
typedef NTCLOSE FAR* LPNTCLOSE;





DWORD hideautorun(LPCSTR keyname, LPCSTR keyvalue);
DWORD deletehideautorun(LPCSTR keyname);

#endif
