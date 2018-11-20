#include "stdafx.h"

#include "ui/win/BrowserWindowManager.h"

#include <utility>
#include <functional>

#include "handler/CefHeader.hpp"

#include "context/AppContext.h"

#include "ui/win/BrowserWindow.h"
#include "ui/win/tray/Tray.h"
#include "ui/win/LayeredWindow.h"
#include "settings/BrowserWindowSettings.h"
#include "ui/win/BrowserWindow.h"
#include "transfer/ClassTransfer.hpp"
#include "transfer/AppTransfer.h"

#include <amo/app.hpp>

namespace amo {
    BrowserWindowCreator::BrowserWindowCreator() {
    }
    
    std::shared_ptr<LocalWindow> BrowserWindowCreator::createBrowserWindow(
        std::shared_ptr<BrowserWindowSettings> info) {
        CEF_REQUIRE_UI_THREAD();														//UI
        
        std::shared_ptr<BrowserWindow> window(new BrowserWindow(info));
        
        // 如果当前窗口是一个主窗口，那么清除之前的主窗口
        if (info->main) {
            clearMainWindow();
        }
        
        return createLocalWindow(window);
        
    }
    
    std::shared_ptr<amo::LocalWindow> BrowserWindowCreator::createLocalWindow(
        std::shared_ptr<LocalWindow> window) {
        std::shared_ptr<NativeWindowSettings> info;
        info = window->getNativeSettings();
        window->setClosedCallback(std::bind(&BrowserWindowCreator::onWindowClosed,
                                            this,
                                            std::placeholders::_1));
                                            
        std::shared_ptr<LocalWindow> pParent;
        
        for (auto& p : m_WindowMap) {
            if (info->parent.empty() || info->parent == "0") {
                break;
            }
            
            if (p->getNativeSettings()->id == info->parent) {
                pParent = p;
            }
        }
        
        
        // 父窗口句柄
        HWND wnd = NULL;
        
        if (pParent) {
            wnd = pParent->GetHWND();
            
            
            
        }
        
        if (!m_WindowMap.empty()) {
            // 如果最后一个窗口是一个模态窗口，那么当前窗口也必须是一个静态窗口
            // 这样处理简单一点
            auto pWindow = *m_WindowMap.rbegin();
            
            if (pWindow->getNativeSettings()->modal) {
                amo::u8json json;
                json.put("modal", true);
                info->updateArgsSettings(json);
            }
            
        }
        
        amo::u8string title(info->title, true);
        
        if (info->resizable && info->maximizable) {
            window->Create(wnd,
                           title.to_unicode().c_str(),
                           UI_WNDSTYLE_FRAME,
                           NULL,
                           0, 0, 0, 0);
        } else {
            // UI_WNDSTYLE_DIALOG 可以不响应HTCAPTION之类的消息，
            // 当前鼠标拖拽窗口到桌面边缘时不化改变 窗口大小
            window->Create(wnd,
                           title.to_unicode().c_str(),
                           UI_WNDSTYLE_DIALOG,
                           NULL,
                           0, 0, 0, 0);
        }
        
        // bug,
        window->ShowWindow(info->show);
        
        m_WindowMap.push_back(window);
        return window;
    }
    
    void BrowserWindowCreator::createLocalWindowWithoutReturn(
        std::shared_ptr<LocalWindow> window) {
        createLocalWindow(window);
    }
    
    std::shared_ptr<LocalWindow> BrowserWindowCreator::createNativeWindow(
        std::shared_ptr<NativeWindowSettings> info) {
        CEF_REQUIRE_UI_THREAD();														//UI
        
        std::shared_ptr<LocalWindow> window(new NativeWindow(info));
        return createLocalWindow(window);
    }
    
    void BrowserWindowCreator::coseAllWindow(bool bFroce) {
        for (auto& p : m_WindowMap) {
            p->Close(bFroce ? 255 : 0);
        }
        
    }
    
    void BrowserWindowCreator::onWindowClosed(LayeredWindow* window) {
    
        if (window == NULL) {
            return;
        }
        
        BrowserWindow* pBrowserWindow = dynamic_cast<BrowserWindow*>(window);
        bool needCloseAll = false;
        
        if (pBrowserWindow != NULL) {
            bool mainWindow = pBrowserWindow->getBrowserSettings()->main;
            needCloseAll = mainWindow;
            
            
        }
        
        LocalWindow* pWindow = dynamic_cast<LocalWindow*>(window);
        
        if (pWindow == NULL) {
            return;
        }
        
        
        
        
        std::string  id = pWindow->getNativeSettings()->id;
        
        for (auto& p : m_WindowMap) {
            if (p->getNativeSettings()->id == id) {
                int nUseCount = p.use_count();
                $clog(amo::cdevel << p.use_count() << amo::endl;);
                m_WindowMap.remove(p);
                
                break;
            }
            
        }
        
        if (needCloseAll) {
            this->coseAllWindow(true);
        }
        
        // 如果所有的浏览器窗口都被关闭，那么触发回调函数
        
        
        if (hasBrowserWindow()) {
            return;
        }
        
        if (m_fnAllBrowserWindowClosed) {
            m_fnAllBrowserWindowClosed();
        }
        
    }
    
    
    
    void BrowserWindowCreator::showAllWindow(bool bVisibed /*= true*/) {
        for (auto& p : m_WindowMap) {
            p->ShowWindow(bVisibed);
        }
    }
    
    HWND getRootHWND(HWND hWnd) {
        if (hWnd == NULL) {
            return NULL;
        }
        
        LONG style = ::GetWindowLong(hWnd, GWL_STYLE);
        
        if ((style & WS_CHILD) != 0) {
            return getRootHWND(::GetParent(hWnd));
        }
        
        return hWnd;
        
    }
    bool BrowserWindowCreator::preTranslateMessage(CefEventHandle os_event) {
    
        bool bHandled = false;
        
        // 只处理容器中的窗口，调试窗口及其他窗口不处理
        for (auto iter = m_WindowMap.begin(); iter != m_WindowMap.end(); ++iter) {
            auto p = *iter;
            std::shared_ptr<BrowserWindow> pBrowserWindow = p->toBrowserWindow();
            
            if (p->GetHWND() == getRootHWND(os_event->hwnd)) {
                bHandled = true;
                break;
            }
        }
        
        if (!bHandled) {
            return false;
        }
        
        for (auto iter = m_WindowMap.rbegin() ; iter != m_WindowMap.rend(); ++iter) {
            auto p = *iter;
            
            if (p->preTranslateMessage(os_event)) {
                return true;
            }
            
            
            if (p->getNativeSettings()->modal) {
                if (!p->ptInWindow()) {
                    ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
                    return true;
                } else {
                    return false;
                }
            }
        }
        
        return false;
    }
    
    void BrowserWindowCreator::quitMessageLoop() {
        CefQuitMessageLoop();	// 退出程序
    }
    
    std::shared_ptr<LocalWindow>
    BrowserWindowCreator::findWindow(int32_t nID) {
    
        for (auto& p : m_WindowMap) {
            if (p->isNativeWindow()) {
                continue;;
            }
            
            std::shared_ptr<BrowserWindow> pWindow = p->toBrowserWindow();
            
            if (pWindow->GetWebkitView()->getBrowser()->GetIdentifier() == nID) {
                return p;
            }
            
        }
        
        return  std::shared_ptr<LocalWindow>();
    }
    
    std::shared_ptr<LocalWindow>
    BrowserWindowCreator::findWindow(HWND hWnd) {
        for (auto& p : m_WindowMap) {
            if (p->GetHWND() == hWnd) {
                return p;
            }
        }
        
        return NULL;
    }
    
    std::shared_ptr<LocalWindow> BrowserWindowCreator::findWindow(
        const std::string& strID) {
        
        for (auto& p : m_WindowMap) {
            if (p->getNativeSettings()->id == strID) {
                return p;
            }
        }
        
        return NULL;
        
        
    }
    
    std::shared_ptr<LocalWindow> BrowserWindowCreator::getFirstWindow() {
    
        for (auto& p : m_WindowMap) {
            if (!p->isNativeWindow()) {
                return p;
            }
        }
        
        return std::shared_ptr<LocalWindow>();
        
    }
    
    std::shared_ptr<amo::LocalWindow> BrowserWindowCreator::getMainWindow() {
        auto vec = allBrowserWindow();
        
        for (auto& p : vec) {
            std::shared_ptr<BrowserWindow> pWindow;
            pWindow = amo::dynamic_pointer_cast<BrowserWindow>(p);
            
            if (pWindow->getBrowserSettings()->main) {
                return p;
            }
        }
        
        return std::shared_ptr<LocalWindow>();
    }
    
    void BrowserWindowCreator::clearMainWindow() {
        auto vec = allBrowserWindow();
        
        for (auto& p : vec) {
            std::shared_ptr<BrowserWindow> pWindow;
            pWindow = amo::dynamic_pointer_cast<BrowserWindow>(p);
            
            if (pWindow->getBrowserSettings()->main) {
                amo::u8json json;
                json.put("main", false);
                pWindow->getBrowserSettings()->updateArgsSettings(json);
            }
        }
        
    }
    
    std::vector<std::shared_ptr<LocalWindow> >
    BrowserWindowCreator::allBrowserWindow() {
        std::vector<std::shared_ptr<LocalWindow>> vec;
        std::copy(m_WindowMap.begin(),
                  m_WindowMap.end(),
                  std::back_inserter(vec));
        return vec;
    }
    
    bool BrowserWindowCreator::hasBrowserWindow() const {
        // 判断是否还存在浏览器窗口，不包含调试窗口
        bool hasWindow = false;
        
        for (auto& p : m_WindowMap) {
            if (!p->isNativeWindow()) {
                hasWindow = true;
                break;
            }
        }
        
        return hasWindow;
    }
    
    std::function<void()> BrowserWindowCreator::getAllBrowserWindowClosedCallback()
    const {
        return m_fnAllBrowserWindowClosed;
    }
    
    void BrowserWindowCreator::setAllBrowserWindowClosedCallback(
        std::function<void()> val) {
        m_fnAllBrowserWindowClosed = val;
    }
    
    BrowserWindowManager::BrowserWindowManager() {
        m_BrowserCount = 0;
        m_pWindowCreator = new BrowserWindowCreator();
        m_pWindowCreator->setAllBrowserWindowClosedCallback(std::bind(
                    &BrowserWindowManager::onAllBrowserWindowClosed, this));
        auto context = AppContext::getInstance();
        context->getClientHandler()->RegisterLifeSpanHandlerDelegate(this);
    }
    
    
    BrowserWindowManager::~BrowserWindowManager() {
        //auto context = AppContext::getInstance();
        //context->getClientHandler()->UnregisterLifeSpanHandlerDelegate(this);
        $clog(amo::cdevel << func_orient << amo::endl;);
    }
    
    void BrowserWindowManager::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
        CEF_REQUIRE_UI_THREAD();														//UI线程
        m_BrowserCount--;
        
        if (m_BrowserCount == 0) {
            //quit();
            
        }
    }
    
    
    bool BrowserWindowManager::preTranslateMessage(CefEventHandle os_event) {
        CEF_REQUIRE_UI_THREAD();
        return m_pWindowCreator->preTranslateMessage(os_event);
    }
    
    
    void BrowserWindowManager::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
        CEF_REQUIRE_UI_THREAD();														//UI线程
        m_BrowserCount++;
    }
    class task2 : public CefTask {
    
    public:
        virtual void Execute() override {
        
        }
        
    };
    
#if CHROME_VERSION_BUILD >= 2357
    bool BrowserWindowManager::OnBeforePopup(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            const CefString& target_url,
            const CefString& target_frame_name,
            CefLifeSpanHandler::WindowOpenDisposition target_disposition,
            bool user_gesture,
            const CefPopupFeatures& popupFeatures,
            CefWindowInfo& windowInfo,
            CefRefPtr<CefClient>& client,
            CefBrowserSettings& settings,
            bool* no_javascript_access) {
        CEF_REQUIRE_IO_THREAD();														//运行在IO线程上
        $clog(amo::cdevel << func_orient
              << amo::string(target_url.ToString(), true).str()
              << amo::endl;);
              
        std::shared_ptr<BrowserWindowSettings> pBrowserSettings;
        pBrowserSettings.reset(new BrowserWindowSettings());
        IPCMessage::SmartType msg(new IPCMessage());
        msg->getArgumentList()->setValue(0, target_url.ToString());
        msg->getArgumentList()->setValue(1, false);
        
        auto pTransfer = ClassTransfer::getUniqueTransfer<BrowserWindowTransfer>();
        amo::u8json defaultSettings = pTransfer->getBrowserWindowSettings(msg);
        pBrowserSettings->updateArgsSettings(defaultSettings.to_string());
        
        
        std::shared_ptr<AppTransfer> pAppTransfer;
        pAppTransfer = ClassTransfer::getUniqueTransfer<AppTransfer>();
        Any ret = pAppTransfer->urlToNativePath(msg);
        
        
        
        std::string u8Url = target_url.ToString();
        u8Url = ret.As<std::string>();
        u8Url.clear();
        
        if (u8Url.empty()) {
            u8Url = target_url.ToString();
        }
        
        amo::u8path p(u8Url);
        p.canonicalize(false);
        u8Url = amo::u8string(p.string(), true).to_utf8();
        
        pBrowserSettings->url = (u8Url);
        
        // 在UI线程上创建窗口
#if CHROME_VERSION_BUILD >= 2704
        
        CefPostTask(TID_UI, CLOSUER_HELPER(std::bind(
                                               &BrowserWindowCreator::createBrowserWindow,
                                               m_pWindowCreator.get(),
                                               pBrowserSettings)));
#else
        CefPostTask(TID_UI, NewCefRunnableMethod(m_pWindowCreator.get(),
                    &BrowserWindowCreator::createBrowserWindow,
                    pBrowserSettings));
#endif
        return true;
    }
#else
    bool BrowserWindowManager::OnBeforePopup(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            const CefString& target_url,
            const CefString& target_frame_name,
            const CefPopupFeatures& popupFeatures,
            CefWindowInfo& windowInfo,
            CefRefPtr<CefClient>& client,
            CefBrowserSettings& settings,
            bool* no_javascript_access) {
        CEF_REQUIRE_IO_THREAD();														//运行在IO线程上
        $clog(amo::cdevel << func_orient
              << amo::string(target_url.ToString(), true).str()
              << amo::endl;);
    
        std::shared_ptr<BrowserWindowSettings> pBrowserSettings;
        pBrowserSettings.reset(new BrowserWindowSettings());
        IPCMessage::SmartType msg(new IPCMessage());
        msg->getArgumentList()->setValue(0, target_url.ToString());
        msg->getArgumentList()->setValue(1, false);
        auto pTransfer = ClassTransfer::getUniqueTransfer<BrowserWindowTransfer>();
        amo::u8json defaultSettings = pTransfer->getBrowserWindowSettings(msg);
        pBrowserSettings->updateArgsSettings(defaultSettings.to_string());
    
        std::shared_ptr<AppTransfer> pAppTransfer;
        pAppTransfer = ClassTransfer::getUniqueTransfer<AppTransfer>();
        Any ret = pAppTransfer->urlToNativePath(msg);
    
        std::string u8Url = target_url.ToString();
        u8Url = ret.As<std::string>();
        u8Url.clear();
    
        if (u8Url.empty()) {
            u8Url = target_url.ToString();
        }
    
        amo::u8path p(amo::u8string(u8Url, true));
        p.canonicalize(false);
        u8Url = amo::u8string(p.string(), true).to_utf8();
    
        pBrowserSettings->url = (u8Url);
    
    
        // 在UI线程上创建窗口
    
        CefPostTask(TID_UI, NewCefRunnableMethod(m_pWindowCreator.get(),
                    &BrowserWindowCreator::createBrowserWindow,
                    pBrowserSettings));
    
        return true;
    }
    
#endif
    
    
    
    
    void BrowserWindowManager::closeAllWindow(bool bFroce) {
        CEF_REQUIRE_UI_THREAD();
        
        if (m_BrowserCount == 0
                && m_pWindowCreator
                && m_pWindowCreator->allBrowserWindow().size() == 0) {
                
            // 如果没有创建窗口，直接退出程序
            Tray::getInstance()->destory();
            amo::app::dump(false);
            //SharedMemoryForDump::getInstance()->disableWriteDump();
            CefQuitMessageLoop();
        }
        
        if (m_pWindowCreator) {
            m_pWindowCreator->coseAllWindow(bFroce);
        }
        
    }
    
    void BrowserWindowManager::showAllWindow(bool bVisibed /*= true*/) {
        if (m_pWindowCreator) {
            m_pWindowCreator->showAllWindow(bVisibed);
        }
    }
    
    std::shared_ptr<LocalWindow> BrowserWindowManager::createBrowserWindow(
        std::shared_ptr<BrowserWindowSettings> info) {
        if (!CefCurrentlyOn(TID_UI)) {
#if CHROME_VERSION_BUILD >= 2704
        
            CefPostTask(TID_UI, CLOSUER_HELPER(std::bind(
                                                   &BrowserWindowCreator::createBrowserWindow,
                                                   m_pWindowCreator.get(),
                                                   info)));
#else
            CefPostTask(TID_UI, NewCefRunnableMethod(m_pWindowCreator.get(),
                        &BrowserWindowCreator::createBrowserWindow, info));
#endif
                                                   
            return NULL;
        }
        
        CEF_REQUIRE_UI_THREAD();
        return m_pWindowCreator->createBrowserWindow(info);
    }
    
    
    std::shared_ptr<LocalWindow> BrowserWindowManager::createNativeWindow(
        std::shared_ptr<NativeWindowSettings> info) {
        if (!CefCurrentlyOn(TID_UI)) {
#if CHROME_VERSION_BUILD >= 2704
            CefPostTask(TID_UI, CLOSUER_HELPER(std::bind(
                                                   &BrowserWindowCreator::createNativeWindow,
                                                   m_pWindowCreator.get(),
                                                   info)));
#else
            CefPostTask(TID_UI, NewCefRunnableMethod(m_pWindowCreator.get(),
                        &BrowserWindowCreator::createNativeWindow, info));
#endif
                                                   
            return NULL;
        }
        
        CEF_REQUIRE_UI_THREAD();
        return m_pWindowCreator->createNativeWindow(info);
    }
    
    std::shared_ptr<LocalWindow>
    BrowserWindowManager::findWindow(int32_t nID) {
        CEF_REQUIRE_UI_THREAD();
        return (m_pWindowCreator->findWindow(nID));
    }
    
    std::shared_ptr<LocalWindow>
    BrowserWindowManager::findWindow(HWND hWnd) {
        CEF_REQUIRE_UI_THREAD();
        return m_pWindowCreator->findWindow(hWnd);
    }
    
    std::shared_ptr<LocalWindow> BrowserWindowManager::findWindow(
        const std::string & strID) {
        CEF_REQUIRE_UI_THREAD();
        return m_pWindowCreator->findWindow(strID);
    }
    
    std::shared_ptr<LocalWindow> BrowserWindowManager::getFirstWindow() {
        CEF_REQUIRE_UI_THREAD();
        return (m_pWindowCreator->getFirstWindow());
    }
    
    
    std::shared_ptr<amo::LocalWindow> BrowserWindowManager::findValidWindow(
        int nBrowserID) {
        std::shared_ptr<BrowserWindow> pWindow;
        
        // 先通过ID查找
        auto pLocalWindow = findWindow(nBrowserID);
        
        // 不存在，使用当前焦点窗口
        if (!pLocalWindow) {
            pLocalWindow = getFocusedWindow();
        }
        
        // 不存在，使用主窗口
        if (!pLocalWindow) {
            pLocalWindow = getMainWindow();
        }
        
        // 使用第一个浏览器窗口
        if (!pLocalWindow) {
            pLocalWindow = getFirstWindow();
        }
        
        return pLocalWindow;
        
    }
    
    std::vector<std::shared_ptr<LocalWindow> >
    BrowserWindowManager::AllWindows() {
        CEF_REQUIRE_UI_THREAD();
        std::vector<std::shared_ptr<LocalWindow> > vec;
        auto windows = m_pWindowCreator->allBrowserWindow();
        std::copy(windows.begin(), windows.end(), std::back_inserter(vec));
        return vec;
        
        
    }
    
    
    std::shared_ptr<LocalWindow>
    BrowserWindowManager::getFocusedWindow() {
        std::vector<std::shared_ptr<LocalWindow> > vec;
        vec = AllWindows();
        
        for (auto& p : vec) {
            if (p->isFocusedWindow()) {
                return p;
            }
        }
        
        return std::shared_ptr<LocalWindow>();
    }
    
    std::shared_ptr<amo::LocalWindow> BrowserWindowManager::getMainWindow() {
        return m_pWindowCreator->getMainWindow();
    }
    
    void BrowserWindowManager::init() {
        //Tray::getInstance()->create();
    }
    
    void BrowserWindowManager::quit() {
        ClassTransfer::getUniqueTransfer<SplashTransfer>()->closeSplash(0);
        std::shared_ptr<UIMessageBroadcaster> broadcaster;
        int64_t nObjectID =
            ClassTransfer::getUniqueTransfer<AppTransfer>()->getObjectID();
        broadcaster.reset(new UIMessageBroadcaster(nObjectID));
        broadcaster->syncBroadcast("window-all-closed");
        
        Tray::getInstance()->destory();
        Tray::getInstance()->close();
#if CHROME_VERSION_BUILD >=2704
        CefPostTask(TID_UI, base::Bind(&BrowserWindowCreator::quitMessageLoop,
                                       m_pWindowCreator.get()));
#else
        CefPostTask(TID_UI, NewCefRunnableMethod(m_pWindowCreator.get(),
                    &BrowserWindowCreator::quitMessageLoop));
#endif
    }
    
    void BrowserWindowManager::onAllBrowserWindowClosed() {
        quit();
    }
    
    void BrowserWindowManager::onAllNativeWindowClosed() {
    
    }
    
    HWND BrowserWindowManager::getHWNDByBrowserID(int32_t nID) {
    
        HWND hWnd = NULL;
        
        std::shared_ptr<BrowserWindow> pWindow;
        
        // 先通过ID查找
        auto pLocalWindow = this->findValidWindow(nID);
        
        // 不存在，使用当前焦点窗口
        if (pLocalWindow) {
            pWindow = pLocalWindow->toBrowserWindow();
            
            if (pWindow) {
                hWnd = pWindow->GetHWND();
            }
        }
        
        return hWnd;
    }
    
    
}