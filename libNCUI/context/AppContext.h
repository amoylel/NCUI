// Created by amoylel on 06/10/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_APPCONTEXT_H__
#define AMO_APPCONTEXT_H__

#include <string>
#include <direct.h>
#include <shlobj.h>
#include <process.h>
#include <TlHelp32.h>

#include "handler/CefHeader.hpp"

#include <amo/singleton.hpp>
#include <amo/string.hpp>
#include "handler/ClientApp.h"
#include "handler/DummyClientHandler.h"
#include "settings/BrowserWindowSettings.h"
#include "node/NodeMessageHandler.h"
#include "settings/AppSettings.h"
#include <amo/timer.hpp>


/*!
 * @enum	ProcessType
 *
 * @brief	进程类型.
 */

enum ProcessType {
    BrowserProcess,
    RendererProcess,
    ZygoteProcess,
    OtherProcess,
};

namespace amo {
    class SplashWindowSettings;
    class BrowserWindowManager;
    class SharedMemory;
    /*!
     * @class	AppContext
     *
     * @brief	一些全局属性.
     */
    class AppContext : public amo::singleton < AppContext > {
    public:
        AppContext();
        ~AppContext();
        
        /*!
         * @fn	void AppContext::startNodeThread();
         *
         * @brief	开始Node.
         */
        
        void startNodeThread();
        
        /*!
         * @fn	void AppContext::initialize(HINSTANCE hInstance);
         *
         * @brief	初始化APP.
         *
         * @param	hInstance	The instance.
         */
        
        void initialize(HINSTANCE hInstance);
        
        /*!
         * @fn	void AppContext::initCommandLine(int argc, const char* const* argv);
         *
         * @brief	初始化命令行参数.
         *
         * @param	argc	The argc.
         * @param	argv	The argv.
         */
        
        void initCommandLine(int argc, const char* const* argv);
        
        /*!
         * @fn	int AppContext::executeProcess(CefMainArgs& main_args);
         *
         * @brief	运行CEF子程序.
         *
         * @param [in,out]	main_args	The main arguments.
         *
         * @return	An int.
         */
        
        int executeProcess(CefMainArgs& main_args);
        
        /*!
         * @fn	void AppContext::run(CefMainArgs& main_args);
         *
         * @brief	开始运行CEF程序.
         *
         * @param [in,out]	main_args	The main arguments.
         */
        
        void run(CefMainArgs& main_args);
        
        /*!
         * @fn	bool AppContext::startHook();
         *
         * @brief	开始Windows窗口消息监听.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool startHook();
        
        /*!
         * @fn	void AppContext::stopHook();
         *
         * @brief	停止Windows窗口消息监听.
         */
        
        void stopHook();
        
        /*!
         * @fn	CefRefPtr<ClientApp> AppContext::getClientApp();
         *
         * @brief	Gets client application.
         *
         * @return	The client application.
         */
        
        CefRefPtr<ClientApp> getClientApp();
        
        /*!
         * @fn	std::shared_ptr<DummyClientHandler> AppContext::getClientHandler();
         *
         * @brief	获取虚拟ClientHandler,每个新创建的ClientHandler都会复制当前Handler的内容.
         *
         * @return	The client handler.
         */
        
        std::shared_ptr<DummyClientHandler> getClientHandler();
        
        /*!
         * @fn	std::shared_ptr<NodeMessageHandler> AppContext::getNodeMessageHandler();
         *
         * @brief	获取Node消息Handler.
         *
         * @return	The node message handler.
         */
        
        std::shared_ptr<NodeMessageHandler> getNodeMessageHandler();
        
        /*!
         * @fn	std::shared_ptr<AppSettings> AppContext::getDefaultAppSettings();
         *
         * @brief	获取默认App设置.
         *
         * @return	The default application settings.
         */
        
        std::shared_ptr<AppSettings>  getDefaultAppSettings();
        
        /*!
         * @fn	std::shared_ptr<BrowserWindowSettings> AppContext::getDefaultBrowserSettings();
         *
         * @brief	获取默认浏览器设置.
         *
         * @return	The default browser settings.
         */
        
        std::shared_ptr<BrowserWindowSettings> getDefaultBrowserSettings();
        
        /*!
         * @fn	std::shared_ptr<SplashWindowSettings> AppContext::getDefaultSplashSettings();
         *
         * @brief	获取默认启动画面设置.
         *
         * @return	The default splash settings.
         */
        
        std::shared_ptr<SplashWindowSettings> getDefaultSplashSettings();
        
        /*!
         * @fn	std::shared_ptr<BrowserWindowManager> AppContext::getBrowserWindowManager();
         *
         * @brief	获取窗口管理器.
         *
         * @return	The browser window manager.
         */
        
        std::shared_ptr<BrowserWindowManager> getBrowserWindowManager();
        
        /*!
         * @fn	ProcessType AppContext::getProcessType();
         *
         * @brief	获取当前进程类型.
         *
         * @return	The process type.
         */
        
        ProcessType getProcessType();
        
    private:
    
        /*!
         * @fn	static LRESULT CALLBACK AppContext::mouseProc(int nCode, WPARAM wParam, LPARAM lParam);
         *
         * @brief	过虑鼠标消息.
         *
         * @param	nCode 	The code.
         * @param	wParam	The wParam field of the message.
         * @param	lParam	The lParam field of the message.
         *
         * @return	A CALLBACK.
         */
        
        static LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam);
        
        /*!
         * @fn	void AppContext::runNodeThread();
         *
         * @brief	运行NodeJS.
         */
        
        void runNodeThread();
        
        /*!
         * @fn	void AppContext::onUpdateAppSettings(BasicSettings* settings);
         *
         * @brief	AppSettings更新时的回调函数.
         *
         * @param [in,out]	settings	If non-null, options for controlling the operation.
         */
        
        void onUpdateAppSettings(BasicSettings* settings);
        
        /*!
         * @fn	bool AppContext::needQuitWithNode();
         *
         * @brief	回调函数，当与Node通信的管道创建后回调，是否退出程序。
         * 			 必须等待通道建好，否则会造成死锁，无法退出Node进程.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool needQuitWithNode();
        
        /*!
         * @fn	bool AppContext::needQuitWithOutNode();
         *
         * @brief	不使用Node的情况下是否退出程序.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool needQuitWithOutNode();
    public:
        AMO_CEF_IMPL_NO_REFCOUNTING(NodeMessageHandler)
        
    private:
        /*! @brief	消息钩子. */
        static HHOOK g_hHook;
        /*! @brief	子进程退出代码. */
        int m_nProcessExitCode;
        /*! @brief	全局ClientApp. */
        CefRefPtr<ClientApp> m_pClientApp;
        /*! @brief	命令行参数. */
        CefRefPtr<CefCommandLine>   m_pCommandLine;
        /*! @brief	浏览器全局设置. */
        std::shared_ptr<AppSettings> m_pAppSettings;
        /*! @brief	系统配置. */
        std::shared_ptr<BrowserWindowSettings> m_pBrowserSettings;
        /*! @brief	系统配置. */
        std::shared_ptr<SplashWindowSettings> m_pSplashSettings;
        /*! @brief	浏览器事件回调句柄ClientHandler; */
        std::shared_ptr<DummyClientHandler> m_pClientHandler;
        /*! @brief	与NodeJs交互Handler. */
        std::shared_ptr<NodeMessageHandler> m_pNodeMessageHandler;
        /*! @brief	Node 线程. */
        std::shared_ptr<amo::looper_executor> pNodeThread;
        
        /*! @brief	共享内存，用于处理单例模式. */
        std::shared_ptr<SharedMemory> m_pSharedMemory;
        
        std::shared_ptr<  amo::loader> nodeDll;
    };
    
}

#endif // AMO_APPCONTEXT_H__
