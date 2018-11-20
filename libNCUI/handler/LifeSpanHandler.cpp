#include "stdAfx.h"
#include "handler/LifeSpanHandler.h"

#include <amo/logger.hpp>

#include "handler/MessageRouterBrowserSide.h"
namespace amo {
    int LifeSpanHandler::m_BrowserCount = 0;
    
    void LifeSpanHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        
        if (m_pMessageRouter) {
            m_pMessageRouter->OnBeforeClose(browser);
        }
        
        DelegateSet::iterator it = m_Delegates.begin();
        
        //TODO: 退出时错误，有可能在循环的时时候，窗口被移除数据，导致程序出错
        // 好像已经改了，不知道改到没有
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnBeforeClose(browser);
        }
    }
    
    bool LifeSpanHandler::DoClose(CefRefPtr<CefBrowser> browser) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        // 通知浏览器父窗口即将关闭
#if CHROME_VERSION_BUILD >= 2272
        //TODO: 这里关闭浏览器
#else
        browser->GetHost()->ParentWindowWillClose();
#endif
        
        bool bHandled = false;
        // Set a flag to indicate that the window close should be allowed.
        m_bIsClosing = true;
        
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->DoClose(browser);
            
            if (bHandled) {
                return true;
            }
        }
        
        
        
        return CefLifeSpanHandler::DoClose(browser);
    }
    
#if CHROME_VERSION_BUILD >= 2704
    
    
#else
    bool LifeSpanHandler::RunModal(CefRefPtr<CefBrowser> browser) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
    
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->RunModal(browser);
    
            if (bHandled) {
                return true;
            }
        }
    
        return CefLifeSpanHandler::RunModal(browser);
    }
#endif
    
    
    void LifeSpanHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnAfterCreated(browser);
        }
    }
    
    
#if CHROME_VERSION_BUILD >= 2357
    bool LifeSpanHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        const CefString& target_url,
                                        const CefString& target_frame_name,
                                        WindowOpenDisposition target_disposition,
                                        bool user_gesture,
                                        const CefPopupFeatures& popupFeatures,
                                        CefWindowInfo& windowInfo,
                                        CefRefPtr<CefClient>& client,
                                        CefBrowserSettings& settings,
                                        bool* no_javascript_access) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnBeforePopup(browser, frame, target_url, target_frame_name,
                                            target_disposition, user_gesture, popupFeatures, windowInfo,
                                            client, settings, no_javascript_access);
                                            
            if (bHandled) {
                return true;
            }
        }
        
        return CefLifeSpanHandler::OnBeforePopup(browser, frame, target_url,
                target_frame_name,
                target_disposition, user_gesture, popupFeatures, windowInfo,
                client, settings, no_javascript_access);
    }
#else
    
    bool LifeSpanHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        const CefString& target_url,
                                        const CefString& target_frame_name,
                                        const CefPopupFeatures& popupFeatures,
                                        CefWindowInfo& windowInfo,
                                        CefRefPtr<CefClient>& client,
                                        CefBrowserSettings& settings,
                                        bool* no_javascript_access) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
    
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnBeforePopup(browser, frame, target_url, target_frame_name,
                                            popupFeatures, windowInfo, client, settings, no_javascript_access);
    
            if (bHandled) {
                return true;
            }
        }
    
        return CefLifeSpanHandler::OnBeforePopup(browser, frame, target_url,
                target_frame_name,
                popupFeatures, windowInfo, client, settings, no_javascript_access);
    }
#endif
    
    
    void LifeSpanHandler::SetMessageRouter(CefRefPtr<MessageRouterBrowserSide>
                                           router) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        m_pMessageRouter = router;
    }
}