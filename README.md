<p align="left">
    <a href="README_EN.md">English</a>&nbsp ｜ &nbsp中文&nbsp ｜ &nbsp
</p>

## Ring3 Hidden- 文件、目录、服务及进程隐藏项目

## 项目介绍

本项目是一款专注于Windows用户态（Ring3)常见隐藏技术实现，其功能包括对文件、目录、系统服务、进程信息、自启动项、进行隐蔽操作,一些隐藏需要获得管理员权限,搭配bypassUAC会更好。

### 主要特性

**文件/目录隐藏**：通过Hook系统API实现对特定文件和目录的透明化处理，使得这些资源在标准系统工具如Explorer中不可见，也可以使用Windows自带的设置将要隐藏的文件或目录设置为系统和隐藏属性使其在Explorer.exe中不可见搭配apihook便有更好的隐藏效果,hook的api：NtQueryDirectoryFile ，NtQueryDirectoryFileEx(可以不hook);需要注意的是，有时候系统中存在多个Explorer.exe因此在注入的时候需要根据实际情况来判断或者直接注入所有的Explorer.exe。

**服务隐藏**：利用Windows自带的安全机制实现用户态的隐藏。Windows ACL机制。这种隐藏要比hook EnumServicesStatusExW更加隐蔽，这种机制同样可以用来创建不可访问的文件。

**进程隐藏**：采用API Hook，在进程创建和枚举过程中绕过检查，从而实现进程的隐身效果。Hook的api:NtQuerySystemInformation。这个hook对processhacker同样有效。

**自启动项隐藏** :原理是使用Native api进行注册表读写，这种隐藏方式效果很好，即使使用自启动项查询工具也不可见比如火绒的自启动管理和Autoruns工具。原理参考:[渗透技巧——“隐藏”注册表的创建 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/32179721)

**模块隐藏**:使用PEB断链进行模块隐藏,在大多数工具中不可见，但是其实并不可靠，很容易被检测到。其实使用dll反射注入会更好。

## 预览



https://github.com/coldpon/ring3-hidden/assets/95997133/66684fde-7295-4102-89e3-5e8fd865a2ea



## 技术总结
| hide | 方法 |
| -------- | -------- |
| 隐藏进程 | hook NtQuerySystemInformation |
| 隐藏自启动项 | NtCreateKeyEx     NtDeleteKeyEx |
| 隐藏服务 | 利用 ACL 隐藏Windows 服务 |
| 隐藏文件 | hook ntQueryDirectoryFile    setfileattributes() |
| 隐藏模块 |  PEB 断链 |


## 安装与使用

**开发环境**：Visual Studio 2022 (v143),Win10 22h2。

**测试**：将编译后的DLL注入到目标进程中，或者利用启动项加载至系统。

**配置**：提供简单的接口或配置文件来指定需要隐藏的资源。

**测试**：三环的隐藏一般不会破坏系统,但是为了开发环境的安全和稳定性请在虚拟机中测试代码并观察隐藏资源是否成功从系统视图中移除。

## 注意事项

**法律声明**：请严格遵守当地法律法规，不得将此项目用于任何非法或未经授权的目的。

## 贡献指南

欢迎所有贡献者参与项目优化、漏洞修复、增加新功能等工作。请遵循GitHub提供的常规流程提交pull request，并在issue板块讨论相关问题。

##  参考

[渗透技巧——“隐藏”注册表的创建 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/32179721)			

[ntQueryDirectoryFile 函数 (ntifs.h) - Windows drivers | Microsoft Learn](https://learn.microsoft.com/zh-cn/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ntquerydirectoryfile)

[NtQuerySystemInformation 函数 (winternl.h) - Win32 apps | Microsoft Learn](https://learn.microsoft.com/zh-cn/windows/win32/api/winternl/nf-winternl-ntquerysysteminformation)

[bytecode77/r77-rootkit: Fileless ring 3 rootkit with installer and persistence that hides processes, files, network connections, etc. (github.com)](https://github.com/bytecode77/r77-rootkit)
