#include "stdafx.h"
#include "NodeLoadHandler.h"
#include "node/NodeHandlerHelper.h"
#include "ipc/UIMessageEmitter.hpp"

namespace amo {

    NodeLoadHandler::NodeLoadHandler(std::shared_ptr<NodeHandlerHelper>& pHelper)
        : m_pHelper(pHelper) {
        
    }
    
    void NodeLoadHandler::OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
            bool isLoading,
            bool canGoBack,
            bool canGoForward) {
        m_pHelper->getBroadcaster()->broadcast("LoadingStateChange",
                                               m_pHelper->getJson(browser),
                                               isLoading,
                                               canGoBack,
                                               canGoForward);
                                               
    }
    
    void NodeLoadHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame) {
        m_pHelper->getBroadcaster()->broadcast("LoadStart",
                                               m_pHelper->getJson(browser),
                                               m_pHelper->getJson(frame));
    }
    
    void NodeLoadHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    int httpStatusCode) {
        m_pHelper->getBroadcaster()->broadcast("LoadEnd",
                                               m_pHelper->getJson(browser),
                                               m_pHelper->getJson(frame),
                                               httpStatusCode);
                                               
    }
    
    void NodeLoadHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefLoadHandler::ErrorCode errorCode,
                                      const CefString& errorText,
                                      const CefString& failedUrl) {
        m_pHelper->getBroadcaster()->broadcast("LoadError",
                                               m_pHelper->getJson(browser),
                                               m_pHelper->getJson(frame),
                                               (int)errorCode,
                                               errorText.ToString(),
                                               failedUrl.ToString());
    }
    
}

