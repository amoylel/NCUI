// Created by amoylel on 7/02/2017.
// Copyright (c) 2017 amoylel. All rights reserved.
#ifndef AMO_BROWSERWINDOWMANAGER_H__
#define AMO_BROWSERWINDOWMANAGER_H__

#include <mutex>
#include <memory>
#include <list>


#include <amo/singleton.hpp>
#include "handler/LifeSpanHandler.h"

namespace amo {
    class LayeredWindow;
    class BrowserWindow;
    class NativeWindow;
    class ImeWebkitInfo;
    class BrowserWindowSettings;
    
}

namespace amo {
#if CHROME_VERSION_BUILD >= 2987
    class BrowserWindowCreator : public CefBaseRefCounted {
#else
    class BrowserWindowCreator : public CefBase {
#endif
    
    
    public:
        BrowserWindowCreator();
        std::shared_ptr<LocalWindow> createBrowserWindow(
            std::shared_ptr<BrowserWindowSettings> info);
            
        std::shared_ptr<LocalWindow> createNativeWindow(
            std::shared_ptr<NativeWindowSettings> info);
            
        std::shared_ptr<LocalWindow> createLocalWindow(
            std::shared_ptr<LocalWindow> window);
            
            
        void createLocalWindowWithoutReturn(
            std::shared_ptr<LocalWindow> window);
            
        std::shared_ptr<LocalWindow> findWindow(int32_t nID);
        std::shared_ptr<LocalWindow> findWindow(const std::string& strID);
        std::shared_ptr<LocalWindow> findWindow(HWND hWnd);
        std::shared_ptr<LocalWindow> getFirstWindow();
        std::shared_ptr<LocalWindow> getMainWindow();
        void clearMainWindow();
        
        std::vector<std::shared_ptr<LocalWindow>> allBrowserWindow();
        void onWindowClosed(LayeredWindow* window);
        void coseAllWindow(bool bFroce = false);
        void showAllWindow(bool bVisibed = true);
        bool hasBrowserWindow() const;
        std::function<void()> getAllBrowserWindowClosedCallback() const;
        void setAllBrowserWindowClosedCallback(std::function<void()> val);
        bool preTranslateMessage(CefEventHandle os_event);
        
        void quitMessageLoop();
        IMPLEMENT_REFCOUNTING(BrowserWindowCreator);
    public:
        /*! @brief	浏览器窗口集合. */
        std::list<std::shared_ptr<LocalWindow> > m_WindowMap;
        
        std::function<void()> m_fnAllBrowserWindowClosed;
        
        
    };
    
    class BrowserWindowManager : public amo::singleton<BrowserWindowManager>
        , public LifeSpanHandlerDelegate {
    public:
#if CHROME_VERSION_BUILD >= 2357
        typedef CefLifeSpanHandler::WindowOpenDisposition WindowOpenDisposition;
#endif
    public:
        BrowserWindowManager();
        ~BrowserWindowManager();
        
        
#if CHROME_VERSION_BUILD >= 2357
        virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   const CefString& target_url,
                                   const CefString& target_frame_name,
                                   WindowOpenDisposition target_disposition,
                                   bool user_gesture,
                                   const CefPopupFeatures& popupFeatures,
                                   CefWindowInfo& windowInfo,
                                   CefRefPtr<CefClient>& client,
                                   CefBrowserSettings& settings,
                                   bool* no_javascript_access) override;
#else
        virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   const CefString& target_url,
                                   const CefString& target_frame_name,
                                   const CefPopupFeatures& popupFeatures,
                                   CefWindowInfo& windowInfo,
                                   CefRefPtr<CefClient>& client,
                                   CefBrowserSettings& settings,
                                   bool* no_javascript_access);
#endif
                                   
                                   
        void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
        void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
        
        
        std::shared_ptr<LocalWindow> createBrowserWindow(
            std::shared_ptr<BrowserWindowSettings> info);
        std::shared_ptr<LocalWindow> createNativeWindow(
            std::shared_ptr<NativeWindowSettings> info);
            
        void closeAllWindow(bool bFroce = false);
        void showAllWindow(bool bVisibed = true);
        
        std::shared_ptr<LocalWindow> findWindow(int32_t nID);
        std::shared_ptr<LocalWindow> findWindow(const std::string& strID);
        std::shared_ptr<LocalWindow> findWindow(HWND hWnd);
        std::shared_ptr<LocalWindow> getFirstWindow();
        std::shared_ptr<LocalWindow> findValidWindow(int nBrowserID);
        
        std::vector<std::shared_ptr<LocalWindow>> AllWindows();
        std::shared_ptr<LocalWindow> getFocusedWindow();
        std::shared_ptr<LocalWindow> getMainWindow();
        
        HWND getHWNDByBrowserID(int32_t nID);
        
        void init();
        
        void quit();
        
        
        bool preTranslateMessage(CefEventHandle os_event);
        
        void onAllBrowserWindowClosed();
        void onAllNativeWindowClosed();
        
    private:
        /*! @brief	浏览器计数. */
        int m_BrowserCount;
        /*! @brief	弹出窗口创建类. */
        CefRefPtr<BrowserWindowCreator> m_pWindowCreator;
        
    };
}
#endif // AMO_BROWSERWINDOWMANAGER_H__
