#include"hideModule.h"



void hideModule(LPCSTR dllname) {
	HMODULE hMod = GetModuleHandleA(dllname);
	PLIST_ENTRY Head, Cur;
	PPEB_LDR_DATA ldr = NULL;
	PLDR_MODULE ldm;
#ifdef _WIN64
	DWORD64 dwPeb = __readgsqword(0x60);
	//ldr = ((_PPEB)dwPeb)->pLdr;
	//int offect = offsetof(_PEB, pLdr);
	ldr = (PPEB_LDR_DATA) * ((DWORD64*)((BYTE*)dwPeb + 24));

#else
	__asm
	{
		mov eax, fs: [0x30]
		mov ecx, [eax + 0x0c]
		mov ldr, ecx
	}
#endif
	Head = &(ldr->InLoadOrderModuleList);
	Cur = Head->Flink;
	do
	{
		ldm = CONTAINING_RECORD(Cur, LDR_MODULE, InLoadOrderModuleList);
		if (hMod == ldm->BaseAddress)
		{

			ldm->InLoadOrderModuleList.Blink->Flink =
				ldm->InLoadOrderModuleList.Flink;
			ldm->InLoadOrderModuleList.Flink->Blink =
				ldm->InLoadOrderModuleList.Blink;
			//
			ldm->InInitializationOrderModuleList.Blink->Flink =
				ldm->InInitializationOrderModuleList.Flink;
			ldm->InInitializationOrderModuleList.Flink->Blink =
				ldm->InInitializationOrderModuleList.Blink;

			//
			ldm->InMemoryOrderModuleList.Blink->Flink =
				ldm->InMemoryOrderModuleList.Flink;
			ldm->InMemoryOrderModuleList.Flink->Blink =
				ldm->InMemoryOrderModuleList.Blink;
			break;
		}
		Cur = Cur->Flink;
	} while (Head != Cur);
}



