#include "stdafx.h"

#include "context/AppContext.h"
#include "ui/win/BrowserWindowManager.h"
#include "scheme/LocalSchemeHandler.h"
#include "handler/ClientApp.h"

#include <amo/smart_ptr.hpp>


#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include <sstream>

#include <future>
#include <thread>
#include <amo/path.hpp>
#include <amo/app.hpp>
#include "ui/win/BrowserWindow.h"
#include <amo/loader.hpp>
#include "transfer/ClassTransfer.hpp"
#include "transfer/AppTransfer.h"
#include "ui/win/SplashWindow.h"
#include "settings/SplashWindowSettings.h"
#include "transfer/SplashTransfer.h"
#include "ui/win/SharedMemory.h"
#include <amo/directory.hpp>
#include <amo/path.hpp>
#include <spdlog/sinks/null_sink.h>
#include "ui/win/Bitmap.hpp"
#include <amo/filestream.hpp>
#include "ui/win/clipboard/ClipboardMonitor.h"

#pragma comment(lib, "Gdiplus.lib")

ULONG_PTR				m_gdiplusToken;
Gdiplus::GdiplusStartupInput StartupInput;

namespace {

    // These flags must match the Chromium values.
    const char kProcessType[] = "type";
    const char kRendererProcess[] = "renderer";
#if defined(OS_LINUX)
    const char kZygoteProcess[] = "zygote";
#endif
    
}

namespace amo {

    HHOOK AppContext::g_hHook = NULL;
    
    LRESULT CALLBACK AppContext::mouseProc(int nCode,
                                           WPARAM wParam,
                                           LPARAM lParam) {
        MOUSEHOOKSTRUCT* pMs = (MOUSEHOOKSTRUCT*)lParam;
        CWPRETSTRUCT* pp = (CWPRETSTRUCT*)lParam;
        MSG* msg = (MSG*)lParam;
        
        // 处理鼠标消息
        if (msg->message >= WM_MOUSEFIRST && msg->message <= WM_MOUSELAST) {
            if (BrowserWindowManager::getInstance()->preTranslateMessage(msg)) {
                return TRUE;		// 表示已经处理过该消息，中断消息循环
            }
        }
        
        return CallNextHookEx(g_hHook, nCode, wParam, lParam);
    }
    
    void AppContext::startNodeThread() {
    
        if (getDefaultAppSettings()->useNode) {
            // 允许向Node发送消息
            getNodeMessageHandler()->enableNodeJS();
            
            // 如果不是在单独的进程中运行Node,那么创建一个线程来运行它
            if (!getDefaultAppSettings()->useNodeProcess) {
                pNodeThread.reset(new amo::looper_executor());
                pNodeThread->execute(std::bind(&AppContext::runNodeThread,
                                               this));
            }
            
            //std::this_thread::sleep_for(std::chrono::milliseconds(30));
            //开始接收NodeJS消息
            getNodeMessageHandler()->startReadMessage();
        }
    }
    
    
    void AppContext::runNodeThread() {
    
        int argc = 3;
        
        // nodejs 使用ansi 命令行参数
        amo::u8string exeDir(amo::u8path::getExeDir(), true);
        
        // NodeJS 文件
        amo::u8string mainJs(getDefaultAppSettings()->main, true);
        
        char** argv = new char*[argc + 1];
        
        for (int i = 0; i < argc; i++) {
            DWORD size = 1000;
            argv[i] = new char[size];
            memset(argv[i], 1000, 0);
            
            if (i == 0) {
                strcpy(argv[i], exeDir.to_ansi().c_str());
            }
            
            if (i == 1) {
                strcpy(argv[i], mainJs.to_ansi().c_str());
            }
            
            //
            if (i == 2) {
                strcpy(argv[i], "--debug-brk=5858");
            }
        }
        
        argv[argc] = nullptr;
        
        amo::u8path p(amo::u8path::getExeDir());
        p.append("node_runner.dll");
        nodeDll.reset(new amo::loader(amo::u8string(p, true)));
        bool bOk = nodeDll->load();
        
        if (!bOk) {
            $cdevel("load node_runner.dll failed");
            return;
        }
        
        std::vector<amo::string> vec;
        nodeDll->exports(vec);
        
        for (auto& p : vec) {
            $cdevel("export func ：{}", p.to_locale());
            
        }
        
        
        nodeDll->exec<int>("Start", argc, argv);
        
        if (nodeDll) {
            $cdevel("run node::Start complete ：{}", nodeDll->get_last_error());
        }
    }
    
    void AppContext::onUpdateAppSettings(BasicSettings* settings) {
        // 只处理主进程的参数设置
        if (getProcessType() == BrowserProcess) {
            if (!m_pSharedMemory) {
                m_pSharedMemory.reset(new SharedMemory(m_pAppSettings->appID));
            }
            
            // 设置Duilib皮肤目录
            amo::u8string strSkin(m_pAppSettings->skinDir, true);
            CPaintManagerUI::SetResourcePath(strSkin.to_unicode().c_str());
            
            // 更新URL映射
            auto appSettings = getDefaultAppSettings()->settings;
            auto pAppTransfer = ClassTransfer::getUniqueTransfer<AppTransfer>();
            
            if (appSettings.contains_key("urlMappings")) {
                amo::u8json mappings = appSettings.get_child("urlMappings");
                
                if (mappings.is_array()) {
                    std::vector<amo::u8json> vec = mappings.to_array();
                    
                    for (auto& p : vec) {
                        if (p.contains_key("url") && p.contains_key("path")) {
                            std::string url = p.getString("url");
                            std::string path = p.getString("path");
                            IPCMessage::SmartType msg(new IPCMessage());
                            msg->getArgumentList()->setValue(0, url);
                            msg->getArgumentList()->setValue(1, path);
                            pAppTransfer->addUrlMapping(msg);
                            
                        }
                    }
                }
            }
            
            m_pNodeMessageHandler->setAfterCreatePipe(
                std::bind(&AppContext::needQuitWithNode, this));
                
                
        }
    }
    
    bool AppContext::needQuitWithNode() {
        //MessageBox(NULL, _T("3311"), _T(""), MB_OK);
        auto pAppSettings = getDefaultAppSettings();
        auto manager = BrowserWindowManager::getInstance();
        
        if (pAppSettings->singleInstance
                && m_pSharedMemory->getInstanceCount() > 1) {
            // 单例模式下只允许一个实例
            if (pAppSettings->useNode) {
                manager->closeAllWindow(true);
                return true;
            }
        }
        
        return false;
    }
    
    bool AppContext::needQuitWithOutNode() {
    
        auto pAppSettings = getDefaultAppSettings();
        auto manager = BrowserWindowManager::getInstance();
        
        if (pAppSettings->singleInstance
                && m_pSharedMemory->getInstanceCount() > 1) {
            // 单例模式下只允许一个实例
            if (!pAppSettings->useNode) {
                manager->closeAllWindow(true);
                return true;
            }
        }
        
        return false;
        
    }
    
    std::shared_ptr<AppSettings> AppContext::getDefaultAppSettings() {
        return m_pAppSettings;
    }
    
    ProcessType AppContext::getProcessType() {
        if (!m_pCommandLine->HasSwitch(kProcessType)) {
            //没有设置进程类型，那么为主进程
            return BrowserProcess;
        }
        
        //通过kProcessType获取进程类型
        const std::string& process_type = m_pCommandLine->GetSwitchValue(
                                              kProcessType);
                                              
        if (process_type == kRendererProcess) {
            // 渲染进程
            return RendererProcess;
        }
        
#if defined(OS_LINUX)
        else if (process_type == kZygoteProcess) {
            return ZygoteProcess;    //LINUX下可用，WINDOWS下不需要
        }
        
#endif
        return OtherProcess;
    }
    
    void AppContext::initCommandLine(int argc, const char* const* argv) {
        m_pCommandLine = CefCommandLine::CreateCommandLine();
        
        if (!m_pCommandLine) {
            $cerr("unable to initialize command line parameters");
            return;
        }
        
#if defined(OS_WIN)
        m_pCommandLine->InitFromString(::GetCommandLine());
#else
        m_pCommandLine->InitFromArgv(argc, argv);
#endif
    }
    
    std::shared_ptr<DummyClientHandler> AppContext::getClientHandler() {
        return m_pClientHandler;
    }
    
    std::shared_ptr<amo::NodeMessageHandler> AppContext::getNodeMessageHandler() {
        return m_pNodeMessageHandler;
    }
    
    CefRefPtr<ClientApp> AppContext::getClientApp() {
        return m_pClientApp;
    }
    
    AppContext::AppContext() {
    
        initCommandLine(0, NULL);
        
        m_pClientApp = new ClientApp();
        m_pAppSettings = AppSettings::getInstance();
        m_pClientHandler = DummyClientHandler::getInstance();
        m_pNodeMessageHandler.reset(new NodeMessageHandler());
        
        m_nProcessExitCode = -1;
        
        m_pAppSettings->setUpdateArgsCallback(
            std::bind(&AppContext::onUpdateAppSettings,
                      this,
                      std::placeholders::_1));
    }
    
    AppContext::~AppContext() {
        $clog(amo::cdevel << func_orient << amo::endl;);
        m_pNodeMessageHandler.reset();
        /*  auto ptr = ClassTransfer::getTransferMap();
        
          if (ptr) {
        	  ptr->clear();
          }*/
        
        m_pClientApp = NULL;
        m_pClientHandler = NULL;
        m_pNodeMessageHandler = NULL;
        m_pSharedMemory.reset();
        m_pAppSettings.reset();
        m_pBrowserSettings.reset();
        
        if (pNodeThread) {
            pNodeThread->kill();
        }
        
        pNodeThread.reset();
        
    }
    
    std::shared_ptr<BrowserWindowSettings> AppContext::getDefaultBrowserSettings() {
        if (m_pBrowserSettings) {
            return m_pBrowserSettings;
        }
        
        m_pBrowserSettings.reset(new BrowserWindowSettings());
        return m_pBrowserSettings;
    }
    
    std::shared_ptr<SplashWindowSettings> AppContext::getDefaultSplashSettings() {
        if (m_pSplashSettings) {
            return m_pSplashSettings;
        }
        
        m_pSplashSettings.reset(new SplashWindowSettings());
        return m_pSplashSettings;
    }
    
    
    int AppContext::executeProcess(CefMainArgs& main_args) {
        AMO_TIMER_ELAPSED();
        //amo::app::dump(true);
        
        if (!getDefaultAppSettings()->dump) {
            amo::app::dump(false);
        }
        
        
        
        //  官方 spdlog 不支持XP, 如果在XP下使用需要禁用log
        //  当前使用的spdlog为修改后的版本
        
        if (!amo::log::initialize(false, getDefaultAppSettings()->debugMode)) {
            return -1;
        }
        
        /*
          std::stringstream stream;
          stream << "logfile_";
          stream << amo::date_time().format("yyyyMMddhh*mm*ss", "*");
          stream << ".txt";
          auto sink3 = std::make_shared<spdlog::sinks::simple_file_sink_mt>(
                           stream.str(), true, true);*/
        
        //BUG XP下无法使用msvc_log，添加一个空输出就不死了
        if (getDefaultAppSettings()->debugMode) {
            auto sink4 = std::make_shared<	spdlog::sinks::null_sink_mt>();
            amo::log::add_sink(sink4);
        }
        
        
        amo::log::set_level(amo::log::level::trace);
        amo::log::set_pattern("[%Y-%m-%d %H:%M:%S][%l] %v");
        
        $clog(amo::cinfo << "log initialize success" << amo::endl;);
        
        // 设置进程消息超时时间
        ProcessExchanger::ipcTimeOut() = getDefaultAppSettings()->ipcTimeout;
        
        void* sandbox_info = NULL;
        // 不使用沙箱
        //#if defined(CEF_USE_SANDBOX)
        //		CefScopedSandboxInfo scoped_sandbox;
        //		sandbox_info = scoped_sandbox.sandbox_info();
        //#endif
        
        
        int exit_code = CefExecuteProcess(main_args,
                                          getClientApp().get(),
                                          sandbox_info);
                                          
        if (BrowserProcess == getProcessType()) {
            m_nProcessExitCode = exit_code;
        } else {
            amo::app::dump(false);
        }
        
        AMO_TIMER_ELAPSED();
        
        if (!getDefaultAppSettings()->single_process) {
            amo::log::finalize();
        }
        
        return exit_code;
    }
    
    
    
    
    void AppContext::run(CefMainArgs& main_args) {
    
        // 不去掉的话打印页面的时候会触发异常处理程序，导致程序关闭
        // ::SetUnhandledExceptionFilter(OurSetUnhandledExceptionFilter);
        amo::u8path::set_work_path_to_app_path();
        
        if (getDefaultAppSettings()->dump) {
            amo::app::dump(true);
        }
        
        AMO_TIMER_ELAPSED();
        
        if (!amo::log::initialize()) {
            return;
        }
        
        
        /*auto sink1 = std::make_shared<spdlog::sinks::msvc_sink_mt>();
        amo::log::add_sink(sink1);  */
        if (m_nProcessExitCode >= 0) {
            amo::log::finalize();
            return;
        }
        
        
        GdiplusStartup(&m_gdiplusToken, &StartupInput, NULL);
        
        void* sandbox_info = NULL;
        CefSettings settings = *getDefaultAppSettings();
        
#if !defined(CEF_USE_SANDBOX)
        settings.no_sandbox = true;
#endif
        settings.no_sandbox = true;
        
        
        
        settings.multi_threaded_message_loop = false;
        CefInitialize(main_args, settings, getClientApp().get(), sandbox_info);
        
        // 注册自定义协议
        amo::ClientApp::RegisterCustomSchemeFactory("local",
                "file",
                new amo::LocalSchemeHandlerFactory());
                
#if CHROME_VERSION_BUILD < 2704
        // 开启消息钩子,2704以上的版本可以使用cef提供的OnDraggableRegionsChanged回调函数进行界面操作
        // 3029及以后的版本CHROMIUM不再漏消息出来了，钩子从这个版本开始失效
        startHook();
#endif
        
        {
            auto manager = BrowserWindowManager::getInstance();
            manager->init();
            
            ClipboardMonitor::getInstance();
        }
        
        auto pAppSettings = getDefaultAppSettings();
        // 单进程模式下需要在单独的线程上创建管道，否则容易引起死锁
        ClientHandler::SingleProcessMode(pAppSettings->single_process != 0);
        
        //
        bool bNeedQuit = needQuitWithOutNode();
        AMO_TIMER_ELAPSED();
        
        //bNeedQuit = true;
        
        if (!bNeedQuit) {
            // 开启启动画面
            if (pAppSettings->showSplash) {
                auto transfer = ClassTransfer::getUniqueTransfer<SplashTransfer>();
                transfer->create(getDefaultSplashSettings());
            }
            
            
            if (!pAppSettings->useNode) {
                //通过窗口管理类创建主窗口并显示
                auto manager = BrowserWindowManager::getInstance();
                manager->createBrowserWindow(getDefaultBrowserSettings());
            } else {
                // 运行Node
                startNodeThread();
            }
            
            
            AMO_TIMER_ELAPSED();
            // 开始消息循环
            CefRunMessageLoop();
        }
        
        ::CoUninitialize();
        amo::log::finalize();
#if CHROME_VERSION_BUILD < 2704
        // 关闭钩子
        stopHook();
#endif
        
        
        
        // 停止监听Node消息
        if (getNodeMessageHandler()) {
            getNodeMessageHandler()->stopNodeProcess();
            
            if (nodeDll) {
                nodeDll->unload();
            }
            
            nodeDll.reset();
        }
        
        
        ClassTransfer::clearTransferMap();
        ClassTransfer::getTransferMap().reset();
        
        
        BrowserWindowManager::releaseInstance();
        ClipboardMonitor::releaseInstance();
        
        
        
        AMO_TIMER_ELAPSED_TEXT(runend);
        amo::log::finalize();
        ::GdiplusShutdown(m_gdiplusToken);
        CefShutdown();
        
        amo::app::dump(false);
    }
    
    bool AppContext::startHook() {
    
        if (g_hHook != NULL) {
            return true;
        }
        
        g_hHook = SetWindowsHookEx(WH_MSGFILTER,
                                   mouseProc,
                                   0,
                                   GetCurrentThreadId());
        return (g_hHook != NULL);
    }
    
    void AppContext::stopHook() {
        if (g_hHook == NULL) {
            return;
        }
        
        UnhookWindowsHookEx(g_hHook);
    }
    
    void AppContext::initialize(HINSTANCE hInstance) {
    
    
        if (getProcessType() == BrowserProcess) {
        
            CPaintManagerUI::SetInstance(hInstance);
            
            amo::u8string strSkin(m_pAppSettings->skinDir, true);
            CPaintManagerUI::SetResourcePath(strSkin.to_unicode().c_str());
            
            
            // 删除缓存
            if (getDefaultAppSettings()->clearCache) {
            
                amo::u8string cachePath(CefString(
                                            getDefaultAppSettings()->cache_path.str).ToString(), true);
                amo::u8path(cachePath).remove_all();
            }
            
            HRESULT Hr = ::CoInitialize(NULL);
            
            if (FAILED(Hr)) {
                return;
            }
            
            
            
        }
        
        AMO_TIMER_ELAPSED();
    }
    
    
    std::shared_ptr<BrowserWindowManager> AppContext::getBrowserWindowManager() {
        return BrowserWindowManager::getInstance();
    }
    
}
