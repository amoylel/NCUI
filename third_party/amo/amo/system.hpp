// Created by 苏元海 on 09/19/2016.
// Copyright (c) 2016 amoylel. All rights reserved.

#ifndef IME_SYSTEM_HPP__
#define IME_SYSTEM_HPP__

#include <amo/config.hpp>
#include <windows.h>
#include <chrono>

namespace amo {
    class system {
    public:
        class DiskInfo {
        public:
            DiskInfo() {
                totalBytes = 0;
                freeBytes = 0;
                freeBytesToCaller = 0;
            }
        public:
            uint64_t totalBytes;
            uint64_t freeBytes;
            uint64_t freeBytesToCaller;
        };
    public:
        static std::string ErrorCodeToMessage(DWORD code,
                                              DWORD langId = MAKELANGID(LANG_CHINESE_SIMPLIFIED, SUBLANG_CHINESE_SIMPLIFIED)) {
            std::string message;
            LPSTR lpMessageBuffer = NULL;
            
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                           NULL, code, langId, (LPSTR)&lpMessageBuffer, 0, NULL);
                           
            if (lpMessageBuffer != NULL) {
                message = std::string(lpMessageBuffer);
                LocalFree(lpMessageBuffer);
                lpMessageBuffer = NULL;
            } else {
                message = "";
            }
            
            return message;
        }
        
        static std::string getLastErrorMessage() {
            uint32_t nErrorCode = getLastError();
            return ErrorCodeToMessage(nErrorCode);
        }
        
        static uint32_t getLastError() {
            return GetLastError();
        }
        
        static BOOL SystemShutdown(UINT nFlag) {
            HANDLE hToken;
            TOKEN_PRIVILEGES tkp;
            
            /* OpenProcessToken() 这个函数的作用是打开一个进程的访问令牌
            GetCurrentProcess() 函数的作用是得到本进程的句柄*/
            if (!OpenProcessToken(GetCurrentProcess(),
                                  TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
                return FALSE;
            }
            
            // LookupPrivilegeValue() 的作用是修改进程的权限
            LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
                                 &tkp.Privileges[0].Luid);
            tkp.PrivilegeCount = 1; // one privilege to set 赋给本进程特权
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            // AdjustTokenPrivileges（）的作用是通知Windows NT修改本进程的权利
            AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
                                  (PTOKEN_PRIVILEGES)NULL, 0);
                                  
            if (GetLastError() != ERROR_SUCCESS) { //失败
                return FALSE;
            }
            
            
            if (!ExitWindowsEx(nFlag, 0)) { //参数在这里设置。强行退出WINDOWS（EWX_FORCE）。
                return FALSE;
            }
            
            return TRUE;
            
        }
        
        static bool shutdown(bool force = false) {
        
            UINT nFlag = EWX_SHUTDOWN;
            
            if (force) {
                nFlag |= EWX_FORCE;
            }
            
            BOOL bOk = SystemShutdown(nFlag);
            return bOk != FALSE;
        }
        
        static bool reboot(bool force = false) {
            UINT nFlag = EWX_REBOOT;
            
            if (force) {
                nFlag |= EWX_FORCE;
            }
            
            BOOL bOk = SystemShutdown(nFlag);
            return bOk != FALSE;
        }
        
        
        static bool set_system_time(int64_t utc_timestamp) {
        
            std::chrono::system_clock::time_point pt;
            pt += (std::chrono::milliseconds(utc_timestamp));
            
            time_t t99 = std::chrono::system_clock::to_time_t(pt);
            tm temptm = *::localtime(&t99);
            SYSTEMTIME systime = { (WORD)(1900 + temptm.tm_year),
                                   (WORD)(1 + temptm.tm_mon),
                                   (WORD)temptm.tm_wday,
                                   (WORD)temptm.tm_mday,
                                   (WORD)temptm.tm_hour,
                                   (WORD)temptm.tm_min,
                                   (WORD)temptm.tm_sec,
                                   (WORD)0
                                 };
                                 
            HANDLE hToken;
            TOKEN_PRIVILEGES tkp;
            
            
            
            
            //////////////////得获SE_SYSTEMTIME_NAME限权//////////////////
            // Get a token for this process.
            if (!OpenProcessToken(GetCurrentProcess(),
                                  TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
                return (FALSE);
            }
            
            // Get the LUID for the shutdown privilege.
            //(得获SE_SYSTEMTIME_NAME限权，才可设置系统时光胜利)
            LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME,
                                 &tkp.Privileges[0].Luid);
                                 
            tkp.PrivilegeCount = 1;  // one privilege to set
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            
            // Get the shutdown privilege for this process.
            AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
                                  (PTOKEN_PRIVILEGES)NULL, 0);
                                  
            if (GetLastError() != ERROR_SUCCESS) {
                return false;
            }
            
            //////////////////得获SE_SYSTEMTIME_NAME限权//////////////////
            
            if (0 == ::SetSystemTime(&systime)) {
                return false;
            }
            
            return true;
        }
        
        static bool set_local_time(int64_t timestamp) {
        
            std::chrono::system_clock::time_point pt;
            pt += (std::chrono::milliseconds(timestamp));
            
            time_t t99 = std::chrono::system_clock::to_time_t(pt);
            tm temptm = *::localtime(&t99);
            SYSTEMTIME systime = { (WORD)(1900 + temptm.tm_year),
                                   (WORD)(1 + temptm.tm_mon),
                                   (WORD)temptm.tm_wday,
                                   (WORD)temptm.tm_mday,
                                   (WORD)temptm.tm_hour,
                                   (WORD)temptm.tm_min,
                                   (WORD)temptm.tm_sec,
                                   (WORD) 0
                                 };
                                 
            HANDLE hToken;
            TOKEN_PRIVILEGES tkp;
            
            
            
            
            //////////////////得获SE_SYSTEMTIME_NAME限权//////////////////
            // Get a token for this process.
            if (!OpenProcessToken(GetCurrentProcess(),
                                  TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
                return (FALSE);
            }
            
            // Get the LUID for the shutdown privilege.
            //(得获SE_SYSTEMTIME_NAME限权，才可设置系统时光胜利)
            LookupPrivilegeValue(NULL, SE_SYSTEMTIME_NAME,
                                 &tkp.Privileges[0].Luid);
                                 
            tkp.PrivilegeCount = 1;  // one privilege to set
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            
            // Get the shutdown privilege for this process.
            AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
                                  (PTOKEN_PRIVILEGES)NULL, 0);
                                  
            if (GetLastError() != ERROR_SUCCESS) {
                return false;
            }
            
            //////////////////得获SE_SYSTEMTIME_NAME限权//////////////////
            
            if (0 == ::SetLocalTime(&systime)) {
                return false;
            }
            
            return true;
        }
        
        static std::shared_ptr<DiskInfo> getDiskInfo(const std::string& rootPath) {
        
            DWORD64  totalBytes;
            DWORD64  freeBytes;
            DWORD64  freeBytesToCaller;
            BOOL bOk = GetDiskFreeSpaceExA(rootPath.c_str(),
                                           (PULARGE_INTEGER)&freeBytesToCaller,
                                           (PULARGE_INTEGER)&totalBytes,
                                           (PULARGE_INTEGER)&freeBytes);
                                           
            if (bOk == FALSE) {
                return std::shared_ptr<DiskInfo>();
            }
            
            std::shared_ptr<DiskInfo> diskInfo(new DiskInfo());
            diskInfo->totalBytes = totalBytes;
            diskInfo->freeBytes = freeBytes;
            diskInfo->freeBytesToCaller = freeBytesToCaller;
            return diskInfo;
            
        }
        
        static bool is_wow64() {
            BOOL bIsWow64 = FALSE;
            //IsWow64Process is not available on all supported versions of Windows.
            //Use GetModuleHandle to get a handle to the DLL that contains the function
            //and GetProcAddress to get a pointer to the function if available.
            typedef BOOL(WINAPI * LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
            
            
            LPFN_ISWOW64PROCESS fnIsWow64Process;
            
            
            fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
                                   GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
                                   
                                   
            if (NULL != fnIsWow64Process) {
                if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64)) {
                    //handle error
                }
            }
            
            return bIsWow64 != FALSE;
        }
        
        
        int get_cup_core_num() {
#if defined(WIN32)
            SYSTEM_INFO info;
            GetSystemInfo(&info);
            return info.dwNumberOfProcessors;
#elif defined(LINUX) || defined(SOLARIS) || defined(AIX) || defined(__linux__)|| defined(__LINUX__)
            return get_nprocs();   //GNU fuction
#else
#error  invalid system
#endif
        }
    };
}

#endif // IME_SYSTEM_HPP__
