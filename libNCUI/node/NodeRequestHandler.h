// Created by amoylel on 07/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODEREQUESTHANDLER_H__
#define AMO_NODEREQUESTHANDLER_H__

#include <memory>

#include "handler/RequestHandler.h"


namespace amo {
    class NodeHandlerHelper;
    class NodeRequestHandler : public RequestHandlerDelegate {
    public:
        NodeRequestHandler(std::shared_ptr<NodeHandlerHelper>& pHelper);
        
        
        virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefRequest> request,
                                    bool is_redirect) override;
                                    
                                    
        virtual CefRefPtr<CefResourceHandler> GetResourceHandler(CefRefPtr<CefBrowser> browser,
                CefRefPtr<CefFrame> frame,
                CefRefPtr<CefRequest> request) override;
                
                
        virtual void OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        const CefString& old_url,
                                        CefString& new_url) override;
                                        
                                        
        virtual bool GetAuthCredentials(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        bool isProxy,
                                        const CefString& host,
                                        int port,
                                        const CefString& realm,
                                        const CefString& scheme,
                                        CefRefPtr<CefAuthCallback> callback) override;
                                        
                                        
        virtual void OnProtocolExecution(CefRefPtr<CefBrowser> browser,
                                         const CefString& url,
                                         bool& allow_os_execution) override;
                                         
#if CHROME_VERSION_BUILD >= 2272
        virtual bool OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                                    const CefString& origin_url,
                                    int64 new_size,
                                    CefRefPtr<CefRequestCallback> callback);
                                    
        virtual bool OnCertificateError(
            CefRefPtr<CefBrowser> browser,
            cef_errorcode_t cert_error,
            const CefString& request_url,
            CefRefPtr<CefSSLInfo> ssl_info,
            CefRefPtr<CefRequestCallback> callback);
            
        virtual CefRequestHandler::ReturnValue OnBeforeResourceLoad(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request,
            CefRefPtr<CefRequestCallback> callback);
            
#else
        virtual bool OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefRefPtr<CefRequest> request);
            
        virtual bool OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                                    const CefString& origin_url,
                                    int64 new_size,
                                    CefRefPtr<CefQuotaCallback> callback);
            
        virtual bool OnCertificateError(cef_errorcode_t cert_error,
                                        const CefString& request_url,
                                        CefRefPtr<CefAllowCertificateErrorCallback> callback);
#endif
            
            
#if CHROME_VERSION_BUILD >= 2454
        virtual bool OnBeforePluginLoad(CefRefPtr<CefBrowser> browser,
                                        const CefString& url,
                                        const CefString& policy_url,
                                        CefRefPtr<CefWebPluginInfo> info);
#else
        virtual bool OnBeforePluginLoad(CefRefPtr<CefBrowser> browser,
                                        const CefString& url,
                                        const CefString& policy_url,
                                        CefRefPtr<CefWebPluginInfo> info);
#endif
                                        
                                        
                                        
        virtual void OnPluginCrashed(CefRefPtr<CefBrowser> browser,
                                     const CefString& plugin_path);
                                     
        virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                               CefRequestHandler::TerminationStatus status);
                                               
    private:
        std::shared_ptr<NodeHandlerHelper>& m_pHelper;
    };
}


#endif // AMO_NODEREQUESTHANDLER_H__





