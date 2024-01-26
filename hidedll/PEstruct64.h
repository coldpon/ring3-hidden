#pragma once
#ifndef PE_STRUCT64_H
#define PE_STRUCT64_H

#include<Windows.h>


struct imageDataDirectory {    //目录项64位和32位一样  
	DWORD   VirtualAddress;
	DWORD   Size;
};

#define IMAGE_NUMBEROF_DIRECTORY_ENTRIES    16





struct dosHeader64 {    //dos头
	WORD e_magic; // 5A4D *EXE标志，“MZ”
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
	DWORD e_lfanew; //0000 00B0 *PE头相对于文件的偏移地址（程序2=0140)
};

struct standardPE64 {        //标准pe头
	WORD    Machine; //             *程序运行的CPU型号：0x0 任何处理器 / 0x14C 386及后续处理器
	WORD    NumberOfSections; //    ******文件中存在的节的总数, 如果要新增节或者合并节 就要修改这个值.
	DWORD   TimeDateStamp; //       *时间戳：文件的创建时间(和操作系统的创建时间无关)，编译器填写的.
	DWORD   PointerToSymbolTable;
	DWORD   NumberOfSymbols;
	WORD    SizeOfOptionalHeader;// *****可选PE头的大小，32位PE文件默认E0h 64位PE文件默认为F0h  大小可以自定义.
	WORD    Characteristics; //     *每个位有不同的含义，可执行文件值为10F 即0 1 2 3 8
};


struct optionalPE64 {         //六十四位可选pe头
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

struct sectionTable64 {     //节表   与32位一样
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

struct importtable64 {     //导入表 和32位一样
	union {
		DWORD characteristics;
		DWORD originalFirstThunk;         //rva 指向image_thunk_data结构
	};
	DWORD Timedatastamp;     //时间戳
	DWORD forworderchain;
	DWORD name;           //rva 指向dll名字以0结尾
	DWORD firstthunk;       //rva 指向image_thunk_data结构

};

struct image_thunk_by_name {
	WORD hint;     //可能为空 如果不为空是函数在导出表中的索引
	BYTE name[1];  //函数名称  以零结尾
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


