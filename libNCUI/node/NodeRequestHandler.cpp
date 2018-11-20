#include "stdafx.h"
#include "node/NodeRequestHandler.h"
#include "node/NodeHandlerHelper.h"
#include "handler/MessageRouterBrowserSide.h"

namespace amo {

    NodeRequestHandler::NodeRequestHandler(std::shared_ptr<NodeHandlerHelper>& pHelper)
        : m_pHelper(pHelper) {
        
    }
    
    
    bool NodeRequestHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame,
                                            CefRefPtr<CefRequest> request,
                                            bool is_redirect) {
        return false;
    }
    
    CefRefPtr<CefResourceHandler> NodeRequestHandler::GetResourceHandler(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request) {
        return NULL;
    }
    
    void NodeRequestHandler::OnResourceRedirect(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            const CefString& old_url,
            CefString& new_url) {
        return ;
    }
    
    bool NodeRequestHandler::GetAuthCredentials(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            bool isProxy,
            const CefString& host,
            int port,
            const CefString& realm,
            const CefString& scheme,
            CefRefPtr<CefAuthCallback> callback) {
        return false;
    }
    
    void NodeRequestHandler::OnProtocolExecution(CefRefPtr<CefBrowser> browser,
            const CefString& url,
            bool& allow_os_execution) {
        return ;
    }
    
#if CHROME_VERSION_BUILD >= 2272
    bool NodeRequestHandler::OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                                            const CefString& origin_url,
                                            int64 new_size,
                                            CefRefPtr<CefRequestCallback> callback) {
        return false;
    }
    
    bool NodeRequestHandler::OnCertificateError(CefRefPtr<CefBrowser> browser,
            cef_errorcode_t cert_error,
            const CefString& request_url,
            CefRefPtr<CefSSLInfo> ssl_info,
            CefRefPtr<CefRequestCallback> callback) {
        return false;
    }
    
    CefRequestHandler::ReturnValue  NodeRequestHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request,
            CefRefPtr<CefRequestCallback> callback) {
        return RV_CONTINUE;
    }
    
#else
    bool NodeRequestHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request) {
        return false;
    }
    
    bool NodeRequestHandler::OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                                            const CefString& origin_url,
                                            int64 new_size,
                                            CefRefPtr<CefQuotaCallback> callback) {
        return false;
    }
    
    bool NodeRequestHandler::OnCertificateError(cef_errorcode_t cert_error,
            const CefString& request_url,
            CefRefPtr<CefAllowCertificateErrorCallback> callback) {
        return false;
    }
#endif
    
    
#if CHROME_VERSION_BUILD >= 2454
    bool NodeRequestHandler::OnBeforePluginLoad(CefRefPtr<CefBrowser> browser,
            const CefString& url,
            const CefString& policy_url,
            CefRefPtr<CefWebPluginInfo> info) {
        return false;
    }
#else
    bool NodeRequestHandler::OnBeforePluginLoad(CefRefPtr<CefBrowser> browser,
            const CefString& url,
            const CefString& policy_url,
            CefRefPtr<CefWebPluginInfo> info) {
        return false;
    }
#endif
    
    
    
    void NodeRequestHandler::OnPluginCrashed(CefRefPtr<CefBrowser> browser,
            const CefString& plugin_path) {
    }
    
    void NodeRequestHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
            CefRequestHandler::TerminationStatus status) {
    }
    
    
}

