// startup.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "startup.h"


#include "stdafx.h"

#include <iostream>
#include <windows.h>
#include "libNCUI.h"
#include <amo/path.hpp>

#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <shellapi.h>

#include <amo/string.hpp>
#include <amo/uuid.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <tchar.h>
#include <amo/loader.hpp>
#include <amo/string.hpp>
#include <amo/timer.hpp>
#include <amo/app.hpp>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "shlwapi.lib")



#ifndef DEBUG_OUT
#define DEBUG_OUT(val) OutputDebugStringA((std::string(#val)+ "\n").c_str());
#endif

class StringLoader {
public:
    StringLoader(HINSTANCE hInstance) {
        m_hInstance = hInstance;
    }
    
    std::wstring load(UINT id) {
        wchar_t str[32768] = { 0 };
        ::LoadStringW(m_hInstance, id, str, 32768);
        return str;
    }
private:
    HINSTANCE m_hInstance;
};

class LoaderLite {
public:
    LoaderLite() {
        lib_handle = NULL;
    }
    
    ~LoaderLite() {
        unload();
    }
    
    bool has_symbol(const std::string& func_name) {
        return  load_symbol(func_name) != NULL;
    }
    
    
    bool load(const std::string& lib_name) {
#if defined(WIN32)
        std::wstring str = amo::string_utils::utf8_to_wide(lib_name);
        lib_handle = LoadLibraryW(str.c_str());
#else
        lib_handle = dlopen(lib_name.c_str(), RTLD_LAZY);
#endif
        return lib_handle != NULL;
    }
    
    void unload() {
        if (!is_loaded()) {
            return;
        }
        
#if defined(WIN32)
        FreeLibrary((HMODULE)lib_handle);
#elif !defined(_ANDROID)
        dlclose(lib_handle);
#endif
        lib_handle = NULL;
    }
    
    
    void* load_symbol(const std::string& fun_name) {
#if defined(WIN32)
        return (void *)GetProcAddress((HMODULE)lib_handle, fun_name.c_str());
#elif !defined(_ANDROID)
        return dlsym(lib_handle, fun_name);
#endif
    }
    
    bool is_loaded() const {
        return lib_handle != NULL;
    }
    
private:
    void* lib_handle;
};

typedef bool(*fnSetMessageQueue)(const std::string&);
using namespace boost::interprocess;
struct ArgsSettings {
public:
    ArgsSettings(HINSTANCE hInstance_) {
        hInstance = hInstance_;
        StringLoader strLoader(hInstance);
        bChildProcess = false;
        bNodeDebug = false;
        bShowSplash = false;
        
        strAppSettings = amo::string_utils::wide_to_utf8(strLoader.load(108));
        strBrowserSettings = amo::string_utils::wide_to_utf8(strLoader.load(110));
        strSplashSettings = amo::string_utils::wide_to_utf8(strLoader.load(111));
        
        
        m_bManifest = (strAppSettings.find("\"manifest\":true") != -1);
        
        
        // 读取磁盘中的配置文件
        if (m_bManifest) {
            amo::u8path p(amo::u8path::getExeDir());
            p.append("manifest.json");
            
            
            if (p.file_exists()) {
                OutputDebugStringA("read manifest.json\n");
                //$cdevel("读取manifest.json");
                std::ifstream ifs(p.generic_wstring());
                std::stringstream buffer;
                buffer << ifs.rdbuf();
                std::string strJson(buffer.str());
                manifestJson = amo::u8json(strJson);
                
                if (!manifestJson.is_valid()) {
                    OutputDebugStringA("invalid manifest.json\n");
                    //$cwarn("manifest.json文件格式错误");
                    manifestJson = amo::u8json();
                }
                
                
                //OutputDebugStringA(strJson.c_str());
                
                //$cdevel(strJson.c_str());
            } else {
                OutputDebugStringA("manifest.json not exist");
                //$cdevel("manifest.json不存在");
            }
        }
        
        std::string sss = manifestJson.to_string();
        
        if (m_bManifest && manifestJson.is_valid() && !manifestJson.empty()) {
            amo::u8json appSettingsJson = manifestJson.get_child("appSettings");
            amo::u8json browserWindowSettingsJson =
                manifestJson.get_child("browserWindowSettings");
                
            amo::u8json splashWindowSettingsJson =
                manifestJson.get_child("splashWindowSettings");
                
            if (appSettingsJson.is_valid()) {
                amo::u8json json(strAppSettings);
                
                if (!json.is_valid()) {
                    json = amo::u8json();
                }
                
                json.join(appSettingsJson);
                strAppSettings = json.to_string();
            }
            
            if (browserWindowSettingsJson.is_valid()) {
                amo::u8json json(strBrowserSettings);
                
                if (!json.is_valid()) {
                    json = amo::u8json();
                }
                
                json.join(browserWindowSettingsJson);
                strBrowserSettings = json.to_string();
            }
            
            if (splashWindowSettingsJson.is_valid()) {
                amo::u8json json(strSplashSettings);
                
                if (!json.is_valid()) {
                    json = amo::u8json();
                }
                
                json.join(splashWindowSettingsJson);
                strSplashSettings = json.to_string();
            }
            
        }
        
        amo::u8json appJson = amo::u8json(strAppSettings);
        appJson.put("startTime", amo::timer::now());  //添加启动时间
        
        if (!m_bManifest) {
            // 将manifest改为true,以防止程序不加载res里面的配置
            appJson.put("manifest", true);
            //m_bManifest = true;
        }
        
        amo::u8string strApp(appJson.to_string(), true);
        strAppSettings = strApp.replace(amo::u8string(" ", true), amo::u8string("",
                                        true)).to_utf8();
                                        
        amo::u8string strBrowser(strBrowserSettings, true);
        strBrowserSettings = strBrowser.replace(amo::u8string(" ", true),
                                                amo::u8string("", true));
                                                
        amo::u8string strSplash(strSplashSettings, true);
        strSplashSettings = strSplash.replace(amo::u8string(" ", true),
                                              amo::u8string("", true));
                                              
                                              
        bUseNode = (strAppSettings.find("\"useNode\":true") != -1);
        bNodeProcess = (strAppSettings.find("\"useNodeProcess\":true") != -1);
        bNodeDebug = (strAppSettings.find("\"debugNode\":true") != -1);
        bShowSplash = (strAppSettings.find("\"showSplash\":true") != -1);
        
        if (bUseNode) {
            m_strJsFile = amo::u8json(strAppSettings).getString("main");
        }
        
        parseArgsSettings();
    }
    
    ~ArgsSettings() {
        closeMessageQueue();
    }
    
    void closeMessageQueue() {
    
        shared_memory_object::remove(strSharedMemoryName.c_str());
        std::string strReaderName = strMessageQueue + "1";
        std::string strWriterName = strMessageQueue + "2";
        message_queue::remove(strReaderName.c_str());
        message_queue::remove(strWriterName.c_str());
        
    }
    
    
    int parseArgsSettings() {
        // 解析命令行参数
        LPWSTR *szArgList = NULL;
        int argCount = 0;
        
        szArgList = CommandLineToArgvW(GetCommandLine(), &argCount);
        
        if (szArgList == NULL) {
            // 命令行参数不能为空
            MessageBox(NULL, L"Unable to parse command line", L"Error", MB_OK);
            return 10;
        }
        
        for (int i = 0; i < argCount; i++) {
            std::wstring args = szArgList[i];
            
            if (args.find(L"--type=") != -1) {
                // 如果有进程类型type说明是CEF的子进程
                bChildProcess = true;
            } else if (args.find(L"--message_queue=") != -1) {
                // 有message_queue说明是CEF主进程
                int nIndex = args.find(L"--message_queue=");
                std::wstring sbuStr = args.substr(nIndex + 16);
                //MessageBox(NULL, sbuStr.c_str(), L"ErrorAAAAA", MB_OK);
                strMessageQueue = sbuStr;
            }
        }
        
        LocalFree(szArgList);
        return true;
    }
    
    // 创建共享内存
    void createSharedMemory(DWORD nProcessID = 0) {
    
        DWORD id = nProcessID;
        
        
        if (id == 0) {
            id = GetCurrentProcessId();
        }
        
        //strMessageQueue = amo::uid().to_string();
        //strMessageQueue = amo::uuid().to_string();
        strMessageQueue = amo::u8string::from_number(id);
        std::stringstream stream;
        stream << "libNCUI_process_" << id;
        
        strSharedMemoryName = stream.str();
        std::string strReaderName = strMessageQueue + "1";
        std::string strWriterName = strMessageQueue + "2";
        
        try {
            closeMessageQueue();
            
            // 如果使用nodejs ，需要使用消息队列，生成队列名
            segment.reset(new managed_shared_memory(create_only,
                                                    strSharedMemoryName.c_str(),
                                                    1024));
            segment->construct<std::string>("messageQueueName")(
                strMessageQueue.to_ansi());
                
            m_pMessageQueueReader.reset(new message_queue(create_only,
                                        strReaderName.c_str(),
                                        100,
                                        10000));
            m_pMessageQueueWriter.reset(new message_queue(create_only,
                                        strWriterName.c_str(),
                                        100,
                                        10000));
                                        
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            MessageBox(NULL, _T("create shared memory failed "), _T(""), MB_OK);
        }
        
    }
    
    bool m_bManifest; // 是否读取磁盘配置文件
    amo::u8json manifestJson;
    std::string m_strJsFile; // utf8
    amo::u8string strMessageQueue;	//消息队列名称
    bool bChildProcess;		//进程类型
    std::string strAppSettings;	//程序设置, utf8
    std::string strBrowserSettings; //窗口设置, utf8
    std::string strSplashSettings; //窗口设置, utf8
    bool bUseNode;	// 是否使用NodeJS
    bool bNodeProcess;// 是否在单独的进程中运行nodejs
    bool bNodeDebug; // 是否调用NodeJS
    bool bShowSplash; // 是否显示Splash
    std::string strSharedMemoryName;		//共享内存名
    HINSTANCE hInstance;
    std::shared_ptr<managed_shared_memory> segment;
    std::shared_ptr<message_queue>  m_pMessageQueueReader;
    std::shared_ptr<message_queue> m_pMessageQueueWriter;
};

std::shared_ptr<ArgsSettings> args;


// 不使用NodeJS
void runCefInCefProcess() {



    LoaderLite loader;
    amo::u8path p(amo::u8path::getExeDir());
    p.append("libNCUI.dll");
    loader.load(p.c_str());
    typedef bool(*FuncUpdateConfig)(const std::string&);
    
    // app
    FuncUpdateConfig fnUpdateAppSettings = (FuncUpdateConfig)
                                           loader.load_symbol("updateAppSettings");
    std::string strAppArgs = amo::u8string(args->strAppSettings, true).to_utf8();
    fnUpdateAppSettings(strAppArgs);
    
    // BrowserWindow
    FuncUpdateConfig fnUpdateBrowserSettings = (FuncUpdateConfig)
            loader.load_symbol("updateBrowserSettings");
    std::string strBrowserArgs = amo::u8string(args->strBrowserSettings,
                                 true).to_utf8();
    fnUpdateBrowserSettings(strBrowserArgs);
    
    // SplashWindow
    FuncUpdateConfig fnUpdateSplashSettings = (FuncUpdateConfig)
            loader.load_symbol("updateSplashSettings");
    std::string strSplashArgs = amo::u8string(args->strSplashSettings,
                                true).to_utf8();
    fnUpdateSplashSettings(strSplashArgs);
    
    fnSetMessageQueue setMessageQueue = (fnSetMessageQueue)
                                        loader.load_symbol("setMessageQueue");
    setMessageQueue(args->strMessageQueue);
    
    typedef int(*fnRun)(HINSTANCE);
    fnRun run = (fnRun)loader.load_symbol("run");
    run(args->hInstance);
    args->closeMessageQueue();
    loader.unload();
}


// 在当前进程中运行NodeJS
void runNodeInCefProcess() {

    if (args->bChildProcess) {
        runCefInCefProcess();
    } else {
        args->createSharedMemory();
        runCefInCefProcess();
    }
    
}

// 在单独进程中运行NodeJS
void runNodeInNodeProcess() {

    if (args->bChildProcess) {
    
        // 如果是Cef的子进程，
        runCefInCefProcess();
    } else {
        if (!args->strMessageQueue.empty()) {
            // cef 主进程
            runCefInCefProcess();
        } else {
            // 调用Cef进程
            if (args->bNodeDebug) {
                amo::u8path::getExeDir();
                amo::u8path p(amo::u8path::getExeDir());
                p.append("node.exe");
                
                amo::u8string strDebugParam("--debug-brk ", true);
                strDebugParam += amo::u8string(args->m_strJsFile, true);
                strDebugParam += amo::u8string(" --ncui-debug ", true);
                
                std::wstring wPath = p.generic_wstring();
                std::wstring wParam = strDebugParam.to_wide();
                SHELLEXECUTEINFOW ShExecInfo = { 0 };
                ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
                ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
                ShExecInfo.hwnd = NULL;
                ShExecInfo.lpVerb = L"open";//多种类别有 "explorer" "print" 等
                ShExecInfo.lpFile = wPath.c_str();//exe 路径
                ShExecInfo.lpParameters = wParam.c_str();//参数
                ShExecInfo.lpDirectory = NULL;
                ShExecInfo.nShow = SW_SHOW;//
                ShExecInfo.hInstApp = NULL;
                ShellExecuteExW(&ShExecInfo);
                
                DWORD id = GetProcessId(ShExecInfo.hProcess);
                args->createSharedMemory(id);
                amo::u8string exeName(amo::u8path::getFullExeName(), true);
                amo::u8string params("--message_queue=", true);
                params += args->strMessageQueue;
                ::ShellExecuteW(NULL,
                                L"open",
                                exeName.to_wide().c_str(),
                                params.to_wide().c_str(),
                                NULL,
                                SW_SHOWNORMAL);
                                
                WaitForSingleObject(ShExecInfo.hProcess,
                                    INFINITE);					//等待完成   第二个参数是超时时间（毫秒）超时后返回超时代码
            } else {
            
                args->createSharedMemory();
                amo::u8string exeName(amo::u8path::getFullExeName(), true);
                amo::u8string params("--message_queue=", true);
                params += args->strMessageQueue;
                ::ShellExecuteW(NULL,
                                L"open",
                                exeName.to_wide().c_str(),
                                params.to_wide().c_str(),
                                NULL,
                                SW_SHOWNORMAL);
                                
                int argc = 4;
                amo::u8string fullExeName(amo::u8path::getFullExeName(), true);
                
                char** argv = new char*[argc + 1];
                
                for (int i = 0; i < argc; i++) {
                    DWORD size = 1000;
                    argv[i] = new char[size];
                    memset(argv[i], 1000, 0);
                    
                    if (i == 0) {
                        strcpy(argv[i], fullExeName.to_ansi().c_str());
                    }
                    
                    /*  if (i == 1) {
                          strcpy(argv[i], "--debug-brk");
                      }*/
                    
                    if (i == 1) {
                        strcpy(argv[i], args->m_strJsFile.c_str());
                    }
                    
                }
                
                argv[argc] = nullptr;
                
                amo::u8path p(amo::u8path::getExeDir());
                p.append("node_runner.dll");
                amo::loader nodeLoader;
                bool bOk2 = nodeLoader.load(p.generic_wstring());
                std::vector<amo::u8string> vec;
                nodeLoader.exports(vec);
                nodeLoader.exec<int>("Start", argc, argv);
            }
            
        }
    }
    
}



STARTUP_API int runNCUI(void) {

    // 防止单程进程下使用nodejs的退出异常
    amo::app::dump(true);
    amo::app::dump(false);
    
    HINSTANCE hInstance = ::GetModuleHandle(NULL);
    amo::u8path::set_work_path_to_app_path();
    args.reset(new ArgsSettings(hInstance));
    
    if (args->bUseNode && args->bNodeProcess) {
        runNodeInNodeProcess();
    } else if (args->bUseNode) {
        runNodeInCefProcess();
    } else {
        runCefInCefProcess();
    }
    
    args.reset();
    return 0;
}

