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
        
        //for (int i = 0; i < 1; ++i) {
        //    Clipboard clipboard;
        //    std::string str1 = clipboard.readImageBase64(L".jpg");
        //    std::string str2 = clipboard.readImageBase64(L".bmp");
        //    std::string str3 = clipboard.readImageBase64(L".png");
        //    std::string str4 = clipboard.readImageBase64(L".gif");
        //
        //    clipboard.saveImage(L"D:\\2222.jpg");
        //    clipboard.saveImage(L"D:\\2222.png");
        //    clipboard.saveImage(L"D:\\2222.bmp");
        //    //clipboard.saveImage(L"D:\\2222.emf");
        //    amo::filestream ifs("D:\\test3.html", std::ios::in | std::ios::binary);
        //    std::string ssss = ifs.read_all();
        //    clipboard.writeHtml(ssss);
        //    std::string sss2 = clipboard.readHtml();
        //    std::string sss3 = clipboard.readText();
        //    HBITMAP hBitmap = clipboard.readBitmap();
        //
        //    if (hBitmap != NULL) {
        //        amo::SaveHBitmapToFile(hBitmap, L"D:\\2222.bmp", L"image/bmp");
        //        DeleteObject(hBitmap);
        //    }
        //
        //}
        
        
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
            
            
            
            Clipboard clipboard;
            //std::vector<std::wstring> files;
            //
            //std::wstring ss2 = L"D:\\2222.png";
            //files.push_back(ss2);
            //ss2 = L"D:\\2222.bmp";
            //files.push_back(ss2);
            //ss2 = L"D:\\2222.jpg";
            //files.push_back(ss2);
            //ss2 = L"D:\\Pic";
            //files.push_back(ss2);
            //ss2 = L"E:\\adb_all";
            //files.push_back(ss2);
            //ss2 = L"D:\\2222.jpg";
            
            ///*clipboard.writeFiles(files);
            //auto vv = clipboard.readFiles();*/
            //std::string base64Str = clipboard.readImageBase64();
            ///*clipboard.writeText("12324");
            //clipboard.empty();*/
            //clipboard.writeImageBase64(base64Str);
            //clipboard.saveImage(ss2);
            /*  clipboard.writeImageBase64("data:image/jpeg;base64,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");*/
            std::string ss = clipboard.readImageBase64(L".jpeg");
            clipboard.writeImageBase64(ss);
            clipboard.writeImageBase64(ss);
            std::ofstream ofs("E:\\test.txt", std::ios::out | std::ios::binary);
            ofs.write(ss.c_str(), ss.size());
            ofs.close();
            std::string  ss2 = clipboard.readImageBase64(L".jpeg");
            clipboard.saveImage(L"E:\\3233333.jpg");
            std::string  ss4 = clipboard.readImageBase64(L".jpeg");
            ClipboardMonitor::getInstance();;
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
