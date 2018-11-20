#include "stdAfx.h"

#include "handler/LoadHandler.h"
#include <amo/logger.hpp>

namespace amo {
    void LoadHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  CefLoadHandler::ErrorCode errorCode,
                                  const CefString& errorText,
                                  const CefString& failedUrl) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnLoadError(browser, frame, errorCode, errorText, failedUrl);
        }
        
        
    }
    
    void LoadHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                int httpStatusCode) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        //CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(CEF_MSG_INIT_DOM_VISITOR);
        //Utils::InsertListValue(msg->GetArgumentList(), IPCArgsPosInfo::FrameId, frame->GetIdentifier());
        //browser->SendProcessMessage(PID_RENDERER, msg);
        
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnLoadEnd(browser, frame, httpStatusCode);
        }
        
        
    }
    
#if CHROME_VERSION_BUILD >= 2743
    void LoadHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame, TransitionType transition_type) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnLoadStart(browser, frame);
        }
    }
    
#else
    void LoadHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
    
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnLoadStart(browser, frame);
        }
    }
    
    
#endif
    
    
    
    void LoadHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                           bool isLoading,
                                           bool canGoBack,
                                           bool canGoForward) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnLoadingStateChange(browser, isLoading, canGoBack, canGoForward);
        }
    }
}