<p align="left">
    <a href="README_CN.md">中文</a>&nbsp ｜ &nbspEnglish&nbsp ｜ &nbsp<a href="README_JA.md">日本語</a> ｜ &nbsp<a href="README_FR.md">Français</a> ｜ &nbsp<a href="README_ES.md">Español</a>
</p>
## Ring3 Hidden - File, Directory, Service, and Process Concealment Project

### Project Introduction

This project specializes in implementing common Windows user-mode (Ring3) concealment techniques. Its functionalities encompass hiding files, directories, system services, process information, startup items, and more. Some hiding operations require administrative privileges for optimal results, which can be facilitated by pairing with a bypassUAC tool.

#### Key Features

**File/Directory Hiding**: Transparently hides specific files and directories by hooking system APIs, rendering them invisible in standard system tools such as Explorer. This can also be achieved by setting the hidden and system attributes of targeted files or directories using Windows built-in settings, combined with API hooking for better concealment. Hooked APIs include NtQueryDirectoryFile and NtQueryDirectoryFileEx (which may not always need to be hooked). Note that multiple instances of Explorer.exe might exist within the system, requiring careful injection decisions or injecting into all instances of Explorer.exe.

**Service Hiding**: Utilizes Windows' native security mechanisms to hide services at the user-level. This involves leveraging the Windows Access Control List (ACL) mechanism, which provides a more discreet form of hiding compared to hooking EnumServicesStatusExW. The same mechanism can be used to create inaccessible files.

**Process Hiding**: Implements API Hooking during the process creation and enumeration phases to bypass checks, thus achieving stealthy process invisibility. Hooked API: NtQuerySystemInformation. This hook is effective against tools like Process Hacker.

**Startup Item Concealment**: Uses Native APIs for registry read/write operations to hide startup items. This method achieves excellent concealment, making hidden startup items undetectable even by dedicated utilities such as火绒's Startup Management and Sysinternals Autoruns. Principle reference: ["渗透技巧——“隐藏”注册表的创建" on Zhihu](https://zhuanlan.zhihu.com/p/32179721).

**Module Concealment**: Relies on PEB chain manipulation for module hiding, which makes modules invisible in most tools but is generally unreliable and easily detected. DLL reflective injection is considered a better alternative.

### Preview

*(No preview provided)*

### Technical Summary

```markdown
/*
ring3 hidden        Hiding Techniques Summary
Hide Processes          hook ntquerysysteminformation
Hide Startup Items     ntcreatekeyex     ntdeletekeyex
Hide Services          Utilize ACLs to hide Windows services
Hide Files             hook ntQueryDirectoryFile    setfileattributes()
Hide Modules           PEB Chain Manipulation
*/
```

### Installation & Usage

**Development Environment**: Visual Studio 2022 (v143), Win10 22h2.

**Testing**: Inject the compiled DLL into the target process, or load it into the system via a startup entry.

**Configuration**: Provides simple interfaces or configuration files to specify resources to be hidden.

**Testing Caution**: While Ring3 hiding techniques typically do not compromise system stability, for safety and consistent testing, please conduct code tests in a virtual machine environment and observe if the hidden resources are successfully removed from system views.

### Notes

**Legal Statement**: Please strictly adhere to local laws and regulations. Do not use this project for any illegal or unauthorized purposes.

### Contribution Guide

Contributions from all participants are welcome for project optimization, vulnerability fixes, and new feature additions. Follow GitHub's standard procedures to submit pull requests and discuss related issues in the issue section.

### References

- ["渗透技巧——“隐藏”注册表的创建" on Zhihu](https://zhuanlan.zhihu.com/p/32179721)
- [ntQueryDirectoryFile function (ntifs.h) - Windows drivers | Microsoft Learn](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ntquerydirectoryfile)
- [NtQuerySystemInformation function (winternl.h) - Win32 apps | Microsoft Learn](https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntquerysysteminformation)
- [bytecode77/r77-rootkit: Fileless ring 3 rootkit with installer and persistence that hides processes, files, network connections, etc.](https://github.com/bytecode77/r77-rootkit)
