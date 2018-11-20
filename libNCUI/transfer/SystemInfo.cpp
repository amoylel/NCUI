#include "stdafx.h"
#include "SystemInfo.h"

namespace amo {

    BOOL SystemInfo::safeGetNativeSystem(LPSYSTEM_INFO lpSystemInfo) {
        BOOL bSucceed = FALSE;
        
        if (NULL == lpSystemInfo) {
            return FALSE;
        }
        
        HMODULE hModule;
        hModule = LoadLibraryA("Kernel32.dll");
        
        if (hModule) {
            typedef VOID(WINAPI * LPFN_GetNativeSystemInfo)(LPSYSTEM_INFO lpSystemInfo);
            LPFN_GetNativeSystemInfo fnGetNativeSystemInfo = (LPFN_GetNativeSystemInfo)
                    GetProcAddress(hModule, ("GetNativeSystemInfo"));
                    
            if (fnGetNativeSystemInfo) {
                fnGetNativeSystemInfo(lpSystemInfo);
                bSucceed = TRUE;
            } else {
                GetSystemInfo(lpSystemInfo);
            }
        }
        
        if (hModule) {
            FreeLibrary(hModule);
        }
        
        return bSucceed;
    }
    
    amo::u8json SystemInfo::getSystemResource() {
        amo::u8json json;
        SYSTEM_INFO systemInfo;
        
        std::string systembit, systemName;
        
        
        safeGetNativeSystem(&systemInfo);
        m_nProcessorCount = systemInfo.dwNumberOfProcessors;
        
        if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
                systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) {
            systembit = "64";
        } else {
            systembit = "32";
        }
        
        
        
        
        MEMORYSTATUSEX statex;
        statex = getTotalMemoryStatus();
        
        
        
        json.put("version", getOSName());
        json.put("osbit", systembit);
        json.put("cpuusage", getTotalCpuStatus());
        json.put("memoryusagepercent", (UINT)statex.dwMemoryLoad);
        json.put("totalphys", statex.ullTotalPhys / 1024);
        json.put("availphys", statex.ullAvailPhys / 1024);
        json.put("totalpagefile", statex.ullTotalPageFile / 1024);
        json.put("availpagefile", statex.ullAvailPageFile / 1024);
        json.put("totalvirtual", statex.ullTotalVirtual / 1024);
        json.put("availvirtual", statex.ullAvailVirtual / 1024);
        return json;
    }
    
    UINT SystemInfo::getTotalCpuStatus() {
        HANDLE hEvent;
        BOOL res;
        
        FILETIME preidleTime;
        FILETIME prekernelTime;
        FILETIME preuserTime;
        
        
        FILETIME idelTime;
        FILETIME kernelTime;
        FILETIME userTime;
        __int64 idle;
        __int64 kernel;
        __int64 user;
        UINT cpu;
        
        
        res = GetSystemTimes(&idelTime, &kernelTime, &userTime);
        
        preidleTime = idelTime;
        prekernelTime = kernelTime;
        preuserTime = userTime;
        
        hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        
        WaitForSingleObject(hEvent, 20);
        res = GetSystemTimes(&idelTime, &kernelTime, &userTime);
        
        idle = compareFileTime(preidleTime, idelTime);
        kernel = compareFileTime(prekernelTime, kernelTime);
        user = compareFileTime(preuserTime, userTime);
        
        cpu = (UINT)((kernel + user - idle) * 100 / (kernel + user));
        
        return cpu;
    }
    
    std::string SystemInfo::getOSName() {
        std::string osname;
        SYSTEM_INFO info;
        safeGetNativeSystem(&info);
        OSVERSIONINFOEX os;
        os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        
        osname = ("unknown OperatingSystem.");
#pragma warning(disable: 4996)
        
        if (GetVersionEx((OSVERSIONINFO *)&os)) {
        
        
            switch (os.dwMajorVersion) {
            case 4: {
                switch (os.dwMinorVersion) {
                case 0:
                    if (os.dwPlatformId == VER_PLATFORM_WIN32_NT) {
                        osname = ("Microsoft Windows NT 4.0");    //1996年7月发布
                    } else if (os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
                        osname = ("Microsoft Windows 95");
                    }
                    
                    break;
                    
                case 10:
                    osname = ("Microsoft Windows 98");
                    break;
                    
                case 90:
                    osname = ("Microsoft Windows Me");
                    break;
                }
                
                break;
            }
            
            case 5: {
                switch (os.dwMinorVersion) {
                case 0:
                    osname = ("Microsoft Windows 2000"); //1999年12月发布
                    break;
                    
                case 1:
                    osname = ("Microsoft Windows XP"); //2001年8月发布
                    break;
                    
                case 2:
                    if (os.wProductType == VER_NT_WORKSTATION
                            && info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
                        osname = ("Microsoft Windows XP Professional x64 Edition");
                    } else if (GetSystemMetrics(SM_SERVERR2) == 0) {
                        osname = ("Microsoft Windows Server 2003");    //2003年3月发布
                    } else if (GetSystemMetrics(SM_SERVERR2) != 0) {
                        osname = ("Microsoft Windows Server 2003 R2");
                    }
                    
                    break;
                }
                
                break;
            }
            
            case 6: {
                switch (os.dwMinorVersion) {
                case 0:
                    if (os.wProductType == VER_NT_WORKSTATION) {
                        osname = ("Microsoft Windows Vista");
                    } else {
                        osname = ("Microsoft Windows Server 2008");    //服务器版本
                    }
                    
                    break;
                    
                case 1: {
                    if (os.wProductType == VER_NT_WORKSTATION) {
                        osname = ("Microsoft Windows 7");
                    } else {
                        osname = ("Microsoft Windows Server 2008 R2");
                    }
                    
                    break;
                }
                
                case 2: {
                    if (os.wProductType == VER_NT_WORKSTATION) {
                        osname = ("Windows 8");
                    } else {
                        osname = ("Windows Server 2012");
                    }
                    
                    break;
                }
                
                case 3: {
                    if (os.wProductType == VER_NT_WORKSTATION) {
                        osname = ("Windows 8.1");
                    } else {
                        osname = ("Windows Server 2012 R2");
                    }
                    
                    break;
                }
                }
                
                break;
            }
            
            case 10: {
                switch (os.dwMinorVersion) {
                case 0:
                    if (os.wProductType == VER_NT_WORKSTATION) {
                        osname = ("Windows 10");
                    } else {
                        osname = ("Windows Server 2016 Technical Preview");
                    }
                    
                    break;
                }
                
                break;
            }
            
            }
        }
        
        return osname;
    }
    
    std::string SystemInfo::getOSVerMark() {
        std::string vmark;
        OSVERSIONINFOEX os;
        os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        vmark = ("");
        
        if (GetVersionEx((OSVERSIONINFO*)&os)) {
            switch (os.dwMajorVersion) {
            case 5:
                switch (os.dwMinorVersion) {
                case 0:                  //Windows 2000
                    if (os.wSuiteMask == VER_SUITE_ENTERPRISE) {
                        vmark = ("Advanced Server");
                    }
                    
                    break;
                    
                case 1:                  //Windows XP
                    if (os.wSuiteMask == VER_SUITE_EMBEDDEDNT) {
                        vmark = ("Embedded");
                    } else if (os.wSuiteMask == VER_SUITE_PERSONAL) {
                        vmark = ("Home Edition");
                    } else {
                        vmark = ("Professional");
                    }
                    
                    break;
                    
                case 2:
                    if (GetSystemMetrics(SM_SERVERR2) == 0
                            && os.wSuiteMask == VER_SUITE_BLADE) { //Windows Server 2003
                        vmark = ("Web Edition");
                    } else if (GetSystemMetrics(SM_SERVERR2) == 0
                               && os.wSuiteMask == VER_SUITE_COMPUTE_SERVER) {
                        vmark = ("Compute Cluster Edition");
                    } else if (GetSystemMetrics(SM_SERVERR2) == 0
                               && os.wSuiteMask == VER_SUITE_STORAGE_SERVER) {
                        vmark = ("Storage Server");
                    } else if (GetSystemMetrics(SM_SERVERR2) == 0
                               && os.wSuiteMask == VER_SUITE_DATACENTER) {
                        vmark = ("Datacenter Edition");
                    } else if (GetSystemMetrics(SM_SERVERR2) == 0
                               && os.wSuiteMask == VER_SUITE_ENTERPRISE) {
                        vmark = ("Enterprise Edition");
                    } else if (GetSystemMetrics(SM_SERVERR2) != 0
                               && os.wSuiteMask == VER_SUITE_STORAGE_SERVER) {
                        vmark = ("Storage Server");
                    }
                    
                    break;
                }
                
                break;
                
            case 6:
                switch (os.dwMinorVersion) {
                case 0:
                    if (os.wProductType != VER_NT_WORKSTATION
                            && os.wSuiteMask == VER_SUITE_DATACENTER) {
                        vmark = ("Datacenter Server");
                    } else if (os.wProductType != VER_NT_WORKSTATION
                               && os.wSuiteMask == VER_SUITE_ENTERPRISE) {
                        vmark = ("Enterprise");
                    } else if (os.wProductType == VER_NT_WORKSTATION
                               && os.wSuiteMask == VER_SUITE_PERSONAL) { //Windows Vista
                        vmark = ("Home");
                    }
                    
                    break;
                }
                
                break;
            }
        }
        
        return vmark;
    }
    
    MEMORYSTATUSEX SystemInfo::getTotalMemoryStatus() {
        MEMORYSTATUSEX statex;
        
        statex.dwLength = sizeof(statex);
        
        GlobalMemoryStatusEx(&statex);
        
        return statex;
    }
    
    int64_t SystemInfo::compareFileTime(FILETIME time1, FILETIME time2) {
        int64_t a = (int64_t)time1.dwHighDateTime << 32 | time1.dwLowDateTime;
        int64_t b = (int64_t)time2.dwHighDateTime << 32 | time2.dwLowDateTime;
        
        return (b - a);
    }
    
}

