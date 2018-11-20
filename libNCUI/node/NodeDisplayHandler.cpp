#include "stdafx.h"
#include "node/NodeDisplayHandler.h"
#include "node/NodeHandlerHelper.h"

namespace amo {

    NodeDisplayHandler::NodeDisplayHandler(std::shared_ptr<NodeHandlerHelper>&
                                           pHelper)
        : m_pHelper(pHelper) {
        
    }
    
    
    void NodeDisplayHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            const CefString& url) {
        m_pHelper->getBroadcaster()->broadcast("AddressChange",
                                               m_pHelper->getJson(browser),
                                               m_pHelper->getJson(frame),
                                               url.ToString());
        return;
    }
    
    void NodeDisplayHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                           const CefString& title) {
        m_pHelper->getBroadcaster()->broadcast("TitleChange",
                                               m_pHelper->getJson(browser),
                                               title.ToString());
        return;
    }
    
    bool NodeDisplayHandler::OnTooltip(CefRefPtr<CefBrowser> browser,
                                       CefString& text) {
        m_pHelper->getBroadcaster()->broadcast("Tooltip",
                                               m_pHelper->getJson(browser),
                                               text.ToString());
        return false;
    }
    
    void NodeDisplayHandler::OnStatusMessage(CefRefPtr<CefBrowser> browser,
            const CefString& value) {
        m_pHelper->getBroadcaster()->broadcast("StatusMessage",
                                               m_pHelper->getJson(browser),
                                               value.ToString());
        return;
    }
    
    
#if CHROME_VERSION_BUILD >= 3282
    bool NodeDisplayHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
            cef_log_severity_t level, const CefString& message, const CefString& source,
            int line) {
        return false;
    }
#else
    bool NodeDisplayHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
            const CefString& message,
            const CefString& source,
            int line) {
        /* m_pHelper->getBroadcaster()->broadcast("ConsoleMessage",
        m_pHelper->getJson(browser),
        source.ToString(),
        source.ToString(),
        line);*/
        return false;
    }
#endif
    
    
}

