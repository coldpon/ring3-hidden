#pragma once
#ifndef PE_STRUCT64_H
#define PE_STRUCT64_H

#include<Windows.h>


struct imageDataDirectory {    //Ŀ¼��64λ��32λһ��  
	DWORD   VirtualAddress;
	DWORD   Size;
};

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16





struct dosHeader64 {    //dosͷ
	WORD e_magic; // 5A4D *EXE��־����MZ��
	WORD e_cblp; //0090
	WORD e_cp; //0003
	WORD e_crlc; //0000
	WORD e_cparhdr; //0004
	WORD e_minalloc; //0000
	WORD e_maxalloc; //FFFF
	WORD e_ss; //0000
	WORD e_sp; //00E8
	WORD e_csum; //0000
	WORD e_ip; //0000
	WORD e_cs; //0000
	WORD e_lfarlc; //0040
	WORD e_ovno; //0000
	WORD e_res[4]; //0000 0000 0000 0000
	WORD e_oemid;//0000
	WORD e_oeminfo; //0000
	WORD e_res2[10]; //0000 0000 0000 0000 0000 0000 0000 0000 0000
	DWORD e_lfanew; //0000 00B0 *PEͷ������ļ���ƫ�Ƶ�ַ������2=0140)
};

struct standardPE64 {        //��׼peͷ
	WORD    Machine; //             *�������е�CPU�ͺţ�0x0 �κδ����� / 0x14C 386������������
	WORD    NumberOfSections; //    ******�ļ��д��ڵĽڵ�����, ���Ҫ�����ڻ��ߺϲ��� ��Ҫ�޸����ֵ.
	DWORD   TimeDateStamp; //       *ʱ������ļ��Ĵ���ʱ��(�Ͳ���ϵͳ�Ĵ���ʱ���޹�)����������д��.
	DWORD   PointerToSymbolTable;
	DWORD   NumberOfSymbols;
	WORD    SizeOfOptionalHeader;// *****��ѡPEͷ�Ĵ�С��32λPE�ļ�Ĭ��E0h 64λPE�ļ�Ĭ��ΪF0h  ��С�����Զ���.
	WORD    Characteristics; //     *ÿ��λ�в�ͬ�ĺ��壬��ִ���ļ�ֵΪ10F ��0 1 2 3 8
};


struct optionalPE64 {         //��ʮ��λ��ѡpeͷ
	WORD                 Magic;
	BYTE                 MajorLinkerVersion;
	BYTE                 MinorLinkerVersion;
	DWORD                SizeOfCode;
	DWORD                SizeOfInitializedData;
	DWORD                SizeOfUninitializedData;
	DWORD                AddressOfEntryPoint;
	DWORD                BaseOfCode;
	ULONGLONG            ImageBase;
	DWORD                SectionAlignment;
	DWORD                FileAlignment;
	WORD                 MajorOperatingSystemVersion;
	WORD                 MinorOperatingSystemVersion;
	WORD                 MajorImageVersion;
	WORD                 MinorImageVersion;
	WORD                 MajorSubsystemVersion;
	WORD                 MinorSubsystemVersion;
	DWORD                Win32VersionValue;
	DWORD                SizeOfImage;
	DWORD                SizeOfHeaders;
	DWORD                CheckSum;
	WORD                 Subsystem;
	WORD                 DllCharacteristics;
	ULONGLONG            SizeOfStackReserve;
	ULONGLONG            SizeOfStackCommit;
	ULONGLONG            SizeOfHeapReserve;
	ULONGLONG            SizeOfHeapCommit;
	DWORD                LoaderFlags;
	DWORD                NumberOfRvaAndSizes;
	imageDataDirectory datadirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
};

#define IMAGE_SIZEOF_SHORT_NAME              8

struct sectionTable64 {     //�ڱ�   ��32λһ��
	BYTE    Name[IMAGE_SIZEOF_SHORT_NAME];
	union {
		DWORD   PhysicalAddress;
		DWORD   VirtualSize;
	} Misc;
	DWORD   VirtualAddress;
	DWORD   SizeOfRawData;
	DWORD   PointerToRawData;
	DWORD   PointerToRelocations;
	DWORD   PointerToLinenumbers;
	WORD    NumberOfRelocations;
	WORD    NumberOfLinenumbers;
	DWORD   Characteristics;
};

#define IMAGE_SIZEOF_SECTION_HEADER          40

struct importtable64 {     //����� ��32λһ��
	union {
		DWORD characteristics;
		DWORD originalFirstThunk;         //rva ָ��image_thunk_data�ṹ
	};
	DWORD Timedatastamp;     //ʱ���
	DWORD forworderchain;
	DWORD name;           //rva ָ��dll������0��β
	DWORD firstthunk;       //rva ָ��image_thunk_data�ṹ

};

struct image_thunk_by_name {
	WORD hint;     //����Ϊ�� �����Ϊ���Ǻ����ڵ������е�����
	BYTE name[1];  //��������  �����β
};



struct imagethunkdata64 {             //thunkdata64 
	union {
		ULONGLONG ForwarderString;  // PBYTE 
		ULONGLONG Function;         // PDWORD
		ULONGLONG Ordinal;
		ULONGLONG AddressOfData;    // PIMAGE_IMPORT_BY_NAME
	} u1;
};



#endif // !PE_STRUCT64_H


