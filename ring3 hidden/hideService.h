#pragma once
#ifndef HIDE_SERVICE_H
#define HIDE_SERVICE_H

#include <windows.h>
#include <AccCtrl.h>
#include <sddl.h>
#include <aclapi.h>

DWORD HideService(LPCSTR ServiceName);

DWORD restoreService(LPCSTR ServiceName);
//system("sc sdset myservice D:(A;;CCLCSWRPWPDTLOCRRC;;;SY)(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;BA)(A;;CCLCSWLOCRRC;;;IU)(A;;CCLCSWLOCRRC;;;SU)S:(AU;FA;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;WD)");//powershell

#endif

