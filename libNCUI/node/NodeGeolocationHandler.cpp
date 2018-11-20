#include "stdafx.h"
#include "node/NodeGeolocationHandler.h"
#include "node/NodeHandlerHelper.h"

namespace amo {

    NodeGeolocationHandler::NodeGeolocationHandler(std::shared_ptr<NodeHandlerHelper>& pHelper)
        : m_pHelper(pHelper) {
        
    }
    
#if CHROME_VERSION_BUILD >= 2272
    bool NodeGeolocationHandler::OnRequestGeolocationPermission(CefRefPtr<CefBrowser> browser,
            const CefString& requesting_url,
            int request_id,
            CefRefPtr<CefGeolocationCallback> callback) {
        return false;
    }
    
#else
    void NodeGeolocationHandler::OnRequestGeolocationPermission(CefRefPtr<CefBrowser> browser,
            const CefString& requesting_url,
            int request_id,
            CefRefPtr<CefGeolocationCallback> callback) {
        return;
    }
#endif
    
#if CHROME_VERSION_BUILD >= 2623
    void NodeGeolocationHandler::OnCancelGeolocationPermission(CefRefPtr<CefBrowser> browser,
            int request_id) {
        return;
    }
#else
    
    void NodeGeolocationHandler::OnCancelGeolocationPermission(CefRefPtr<CefBrowser> browser,
            const CefString& requesting_url,
            int request_id) {
        return;
    }
#endif
    
}

