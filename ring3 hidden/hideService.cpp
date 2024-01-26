#include"hideService.h"

/*
隐藏/恢复 服务需要管理员权限
Hidden services  /restore services  require administrator privileges
*/

DWORD HideService(LPCSTR ServiceName) {

	SC_HANDLE schManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (schManager == NULL) {

		return GetLastError();
	}
	SC_HANDLE schService = OpenServiceA(schManager, ServiceName, READ_CONTROL | WRITE_DAC);
	if (schService == NULL) {

		CloseServiceHandle(schManager);
		return GetLastError();
	}
	WCHAR sddl[] = L"D:(D;;DCWPDTSD;;;IU)(D;;DCWPDTSD;;;SU)(D;;DCWPDTSD;;;BA)(A;;CCSWLOCRRC;;;IU)(A;;CCSWLOCRRC;;;SU)(A;;CCSWRPWPDTLOCRRC;;;SY)(A;;CCDCSWRPWPDTLOCRSDRCWDWO;;;BA)";
	PSECURITY_DESCRIPTOR pSD = NULL;

	DWORD SecurityDescriptorSize = 0;

	BOOL ret = ConvertStringSecurityDescriptorToSecurityDescriptorW(sddl, SDDL_REVISION_1, &pSD, &SecurityDescriptorSize);
	if (ret == FALSE) {
		CloseServiceHandle(schManager);
		CloseServiceHandle(schService);
		return GetLastError();
	}
	BOOL status = SetServiceObjectSecurity(schService, DACL_SECURITY_INFORMATION, pSD);
	if (status == NULL) {
		CloseServiceHandle(schService);
		CloseServiceHandle(schManager);
		return GetLastError();
	}
	CloseServiceHandle(schManager);
	CloseServiceHandle(schService);
	return 0;
}


DWORD restoreService(LPCSTR ServiceName) {

	WCHAR sddl[] = L"D:(A;;CCLCSWRPWPDTLOCRRC;;;SY)(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;BA)(A;;CCLCSWLOCRRC;;;IU)(A;;CCLCSWLOCRRC;;;SU)S:(AU;FA;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;WD)";

	PSECURITY_DESCRIPTOR pSD = NULL;
	DWORD   SecurityDescriptorSize = 0;
	BOOL ret = ConvertStringSecurityDescriptorToSecurityDescriptorW(sddl, SDDL_REVISION_1, &pSD, &SecurityDescriptorSize);
	if (ret == FALSE) {
		return GetLastError();
	}
	SC_HANDLE schManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (schManager == NULL) {
		return GetLastError();
	}
	SC_HANDLE schService = OpenServiceA(schManager, ServiceName, READ_CONTROL | WRITE_DAC);
	if (schService == NULL) {
		CloseServiceHandle(schManager);
		return GetLastError();
	}
	BOOL status = SetServiceObjectSecurity(schService, DACL_SECURITY_INFORMATION, pSD);
	if (status == NULL) {
		CloseServiceHandle(schService);
		CloseServiceHandle(schManager);
		return GetLastError();
	}
	CloseServiceHandle(schService);
	CloseServiceHandle(schManager);
	return 0;
	//system("sc sdset myservice D:(A;;CCLCSWRPWPDTLOCRRC;;;SY)(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;BA)(A;;CCLCSWLOCRRC;;;IU)(A;;CCLCSWLOCRRC;;;SU)S:(AU;FA;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;WD)");
}
