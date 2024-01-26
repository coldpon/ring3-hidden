#include"hideService.h"
#include"hideAutorun.h"

/*
##ring3 hidden        隐藏技术总结
#隐藏进程           hook ntquerysysteminformation
#隐藏自启动项       ntcreatekeyex     ntdeletekeyex
#隐藏服务           利用 ACL 隐藏Windows 服务
#隐藏文件		hook ntQueryDirectoryFile    setfileattributes()
#隐藏模块             PEB 断链
*/

/*
##Summary of hidden technology in ring3

#Hide process				 hook ntquerysysteminformation

#Hide self launch items      use ntcreateketex ntdeleteketex

#Hiding Services				Using ACL to Hide Windows Services

#Hide file					hook ntQueryDirectoryFile  setfilehide

#Hidden module				PEB broken chain

*/

VOID hideautorunTest() {
	DWORD result = hideautorun("\0test", "C:\\test.exe");
	if (result == 0) {
		printf("hideautorun success  If you want to test, please restart your computer or cancel it\n");
	}
	system("pause");
	deletehideautorun("\0test");
}
VOID hideserviceTest() {
	DWORD result = HideService("WSearch");
	if (result == 0) {
		printf("hideservice success\n");
	}
	system("pause");
	
	restoreService("WSearch");
}

BOOL setFileHide(LPCSTR filepath) {
	return SetFileAttributesA(filepath, FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_ARCHIVE);
}

BOOL setFileHide(LPCWSTR filepath) {
	return SetFileAttributesW(filepath, FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_ARCHIVE);
}
BOOL restorefile(LPCWSTR filepath) {
	return SetFileAttributesW(filepath, FILE_ATTRIBUTE_NORMAL);
}

VOID hidefileTest() {
	BOOL status = setFileHide("C:\\test.txt");//hidefile

	BOOL status2 = setFileHide("C:\\test");//hidefolder
	if (status && status2) {
		printf("hidefile success\n");
	}
	system("pause");
	restorefile(L"C:\\test.txt");
	restorefile(L"C:\\test");
}

int main(void) {

	printf("---------hideserviceTest hide:WSearch\n");
	system("pause");
	hideserviceTest();
	printf("---------hidefiletest hide:C:\\test.txt,C:\\test\n");
	system("pause");
	hidefileTest();
	printf("---------hideautorunTest hidefilepath:C:\\test.exe\n");
	system("pause");
	hideautorunTest();

	return 0;
}