// Created by amoylel on 31/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_APP_WIN_F48ECB2B_9EFD_4A7D_A761_82DC3FA201FB_HPP__
#define AMO_APP_WIN_F48ECB2B_9EFD_4A7D_A761_82DC3FA201FB_HPP__


#include <string>

#include <amo/path.hpp>
#include <amo/string.hpp>
#include <tchar.h>
#include <amo/process.hpp>
#include <amo/shell.hpp>
#include <atomic>

#include <windows.h>
#include <imagehlp.h>

#pragma comment(lib, "Dbghelp.lib")

namespace amo {
    class app {
    public:
        static std::wstring getSpecialFolder(int nType) {
            wchar_t path[MAX_PATH] = { 0 };
            SHGetSpecialFolderPathW(NULL, path, nType, FALSE);
            return path;
        }
        amo::u8string getAppPath() const {
            return m_appPath;
        }
        void setAppPath(amo::u8string val) {
            m_appPath = val;
        }
    public:
        app(const amo::u8string& appPath = amo::u8string("", true)) {
            m_appPath = appPath;
            
            if (m_appPath.empty()) {
                m_appPath = amo::u8string(amo::u8path::getFullExeName(), true);
                m_pid = GetCurrentProcessId();
            }
        }
        
        std::wstring dataDir() {
            return getSpecialFolder(CSIDL_LOCAL_APPDATA);
        }
        
        std::wstring tempDir() {
            return getSpecialFolder(CSIDL_TEMPLATES);
        }
        
        void setAutoRun(bool bAutoRun = true) {
            bool bAuto = isAutoRun();
            
            if (bAuto && bAutoRun) {
                return;
            }
            
            if (!bAuto && !bAutoRun) {
                return;
            }
            
            HKEY hKey;
            LPCTSTR lpRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
            //---------------找到系统的启动项
            long lRet = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun, 0, KEY_WRITE, &hKey);
            
            if (lRet == ERROR_SUCCESS) {
            
                amo::u8string strFileName = m_appPath;
                amo::u8path p(strFileName.str());
                p.strip_path().remove_extension();
                amo::u8string strAppName(p.c_str(), true);
                RegDeleteValueW(hKey, strAppName.to_unicode().c_str());				//删除注册表项
                
                if (bAutoRun) {
                    auto wStrFileName = strFileName.to_unicode();
                    lRet = RegSetValueExW(hKey,
                                          strAppName.to_unicode().c_str(),
                                          0,
                                          REG_SZ,
                                          (BYTE *)wStrFileName.c_str(),
                                          wStrFileName.size() * sizeof(TCHAR));		//添加或者修改注册表项
                }
                
                RegCloseKey(hKey); //关闭注册表
            }
            
            return;
        }
        
        
        bool isAutoRun() {
            do {
                HKEY hKey;
                LPCTSTR lpRun =
                    _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");//找到系统的启动项
                long lRet = RegOpenKeyExW(HKEY_CURRENT_USER, lpRun, 0, KEY_READ, &hKey);
                
                if (lRet != ERROR_SUCCESS) {
                    break;
                }
                
                DWORD dwType = REG_SZ;
                DWORD dwSize = 1024;
                TCHAR data[1024] = { 0 };
                
                amo::u8string strFileName = m_appPath;
                amo::u8path p(m_appPath.str());
                p.strip_path();
                p.remove_extension();
                amo::u8string strAppName(p.c_str(), true);
                
                lRet = RegQueryValueExW(hKey,
                                        strAppName.to_unicode().c_str(),
                                        NULL,
                                        &dwType,
                                        (LPBYTE)data,
                                        &dwSize);
                                        
                if (lRet != ERROR_SUCCESS) {
                    break;
                }
                
                if (_tcsicmp(strFileName.to_unicode().c_str(), data) != 0) {
                    break;
                }
                
                RegCloseKey(hKey); 													//关闭注册表
                return true;
                
            } while (false);
            
            
            return false;
        }
        
        
        
        bool restart(int delay = 3) {
            // 只允许调用一次
            static std::atomic_bool is_called = false;
            
            if (is_called) {
                return false;
            }
            
            is_called = true;
            newInstance(delay);
            close();
            return true;
        }
        
        // 新开实例
        bool newInstance(int delay = 3) {
            std::shared_ptr<amo::shell> shell(new amo::shell(amo::u8string("cmd.exe",
                                              true)));
            shell->addArgs(amo::u8string("/c ping 127.0.0.1 -n ", true));
            shell->addArgs(amo::u8string::from_number(delay));
            shell->addArgs(amo::u8string(" -w 1000 > nul & start ", true));
            shell->addArgs(amo::u8string(amo::u8path(m_appPath)
                                         .get_short_path()
                                         .to_windows_string(), true));
            shell->addArgs(amo::u8string(" exit", true));
            shell->show(false);
            int nRetval = shell->open();
            return nRetval > 32;
            
        }
        // 关闭程序
        void close() {
            if (m_fnClose) {
                m_fnClose();
            } else {
                //::ExitProcess(0);
            }
        }
        
        void registerCloseCallback(std::function<void()> fn) {
            m_fnClose = fn;
        }
        
        int64_t pid() const {
            return m_pid;
        }
        
        static void CreateDumpFile(LPCWSTR lpstrDumpFilePathName,
                                   EXCEPTION_POINTERS *pException) {
            // 创建Dump文件
            //
            HANDLE hDumpFile = CreateFileW(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL,
                                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
                                           
            // Dump信息
            //
            MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
            dumpInfo.ExceptionPointers = pException;
            dumpInfo.ThreadId = GetCurrentThreadId();
            dumpInfo.ClientPointers = TRUE;
            
            // 写入Dump文件内容
            //
            MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile,
                              MiniDumpNormal, &dumpInfo, NULL, NULL);
                              
            CloseHandle(hDumpFile);
        }
        
        static LONG WINAPI RedirectedSetUnhandledExceptionFilter(
            EXCEPTION_POINTERS
            * pException) {
            // When the CRT calls SetUnhandledExceptionFilter with NULL parameter
            // our handler will not get removed.
            try {
            
                amo::date_time now;
                std::string fileName = now.format("yyyy_MM_dd_hh_mm_ss_ms");
                fileName += ".dmp";
                fileName = amo::u8path::fullPathInAppDir(fileName).c_str();
                $cerr("program is crashed, careate dump file:{}", fileName);
                amo::u8string dumpFile(fileName, true);
                std::wstring wdumpFile(dumpFile.to_wide());
                CreateDumpFile(wdumpFile.c_str(), pException);
                amo::process::kill_process_by_pid(GetCurrentProcessId(), true);
                return EXCEPTION_EXECUTE_HANDLER;
            } catch (const std::exception&) {
                amo::process::kill_process_by_pid(GetCurrentProcessId(), true);
            }
            
            return 1;
        }
        
        static LONG WINAPI OurSetUnhandledExceptionFilter(
            EXCEPTION_POINTERS *pException) {
            try {
                amo::date_time now;
                std::string fileName = now.format("yyyy_MM_dd_hh_mm_ss_ms");
                fileName += ".dmp";
                fileName = amo::u8path::fullPathInAppDir(fileName).c_str();
                
                amo::u8string dumpFile(fileName, true);
                std::wstring wdumpFile(dumpFile.to_wide());
                $cerr("program is crashed, careate dump file:{}", fileName);
                CreateDumpFile(wdumpFile.c_str(), pException);
                
                
                if (!is_create_dmp_file()) {
                    amo::u8path(dumpFile).remove();
                }
                
                
                amo::process::kill_process_by_pid(GetCurrentProcessId(), false);
                return EXCEPTION_EXECUTE_HANDLER;
            } catch (const std::exception&) {
            
                amo::process::kill_process_by_pid(GetCurrentProcessId(), false);
            }
            
            return EXCEPTION_EXECUTE_HANDLER;
            
        }
        
        static void dump(bool create_dmp = true) {
            if (create_dmp) {
                is_create_dmp_file() = true;
                ::SetUnhandledExceptionFilter(OurSetUnhandledExceptionFilter);
            } else {
                is_create_dmp_file() = false;
            }
        }
        
        static bool make_daemon() {
        
            //::ShowWindow(::GetConsoleWindow(), SW_HIDE);
            HWND hwnd = NULL;
            //处理顶级窗口的类名和窗口名称匹配指定的字符串,不搜索子窗口。
            hwnd = FindWindowW(L"ConsoleWindowClass", NULL);
            
            if (hwnd) {
                ShowWindow(hwnd, SW_HIDE);               //设置指定窗口的显示状态
                return true;
            } else {
                return false;
            }
        }
    protected:
    
        static bool& is_create_dmp_file() {
            static bool val = true;
            return val;
        }
    private:
        amo::u8string m_appPath;
        
        std::function<void()> m_fnClose;
        
        int64_t m_pid;
    };
}

#endif //AMO_APP_WIN_F48ECB2B_9EFD_4A7D_A761_82DC3FA201FB_HPP__

