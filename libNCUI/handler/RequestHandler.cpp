#include "stdAfx.h"

#include "handler/RequestHandler.h"

#include <algorithm>

#include <amo/logger.hpp>

#include "handler/ResourceHandler.h"
#include "scheme/LocalSchemeHandler.h"

#include "handler/MessageRouterBrowserSide.h"
#include "handler/ResourceHandler.h"


namespace amo {
    void RequestHandler::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
            TerminationStatus status) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        
        if (m_pMessageRouter) {
            m_pMessageRouter->OnRenderProcessTerminated(browser);
        }
        
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnRenderProcessTerminated(browser, status);
        }
        
        
        
        //// Load the startup URL if that's not the website that we terminated on.
        //CefRefPtr<CefFrame> frame = browser->GetMainFrame();
        //std::string url = frame->GetURL();
        //std::transform(url.begin(), url.end(), url.begin(), tolower);
        
        //std::string startupURL = "about://blank";
        //if (startupURL != "chrome://crash" && !url.empty() &&
        //	url.find(startupURL) != 0) {
        //	frame->LoadURL(startupURL);
        //}
    }
    
    void RequestHandler::OnPluginCrashed(CefRefPtr<CefBrowser> browser,
                                         const CefString& plugin_path) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnPluginCrashed(browser, plugin_path);
        }
    }
    
#if CEF_VERSION_REGION(2454, 10000)
    
    
#elif CEF_VERSION_REGION(0, 2454)
    bool RequestHandler::OnBeforePluginLoad(CefRefPtr<CefBrowser> browser,
                                            const CefString& url,
                                            const CefString& policy_url,
                                            CefRefPtr<CefWebPluginInfo> info) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
    
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnBeforePluginLoad(browser, url, policy_url, info);
    
            if (bHandled) {
                return true;
            }
        }
    
        return CefRequestHandler::OnBeforePluginLoad(browser, url, policy_url, info);
    }
#endif
    
    
    
#if CHROME_VERSION_BUILD >= 2272
    bool RequestHandler::OnCertificateError(CefRefPtr<CefBrowser> browser,
                                            cef_errorcode_t cert_error,
                                            const CefString& request_url,
                                            CefRefPtr<CefSSLInfo> ssl_info,
                                            CefRefPtr<CefRequestCallback> callback) {
        $clog(amo::cdevel << func_orient << amo::string(request_url.ToString(),
                true).str() << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnCertificateError(browser, cert_error,
                                                 request_url, ssl_info, callback);
                                                 
            if (bHandled) {
                return true;
            }
        }
        
        return CefRequestHandler::OnCertificateError(browser, cert_error,
                request_url, ssl_info, callback);
    }
    
#else
    bool RequestHandler::OnCertificateError(cef_errorcode_t cert_error,
                                            const CefString& request_url,
                                            CefRefPtr<CefAllowCertificateErrorCallback> callback) {
        $clog(amo::cdevel << func_orient << request_url << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
    
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnCertificateError(cert_error, request_url, callback);
    
            if (bHandled) {
                return true;
            }
        }
    
        return CefRequestHandler::OnCertificateError(cert_error, request_url, callback);
    }
#endif
    
    
    
    
    void RequestHandler::OnProtocolExecution(CefRefPtr<CefBrowser> browser,
            const CefString& url, bool& allow_os_execution) {
        $clog(amo::cdevel << func_orient << amo::string(url.ToString(),
                true).str() << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnProtocolExecution(browser, url, allow_os_execution);
        }
    }
    
#if CHROME_VERSION_BUILD >= 2272
    
    bool RequestHandler::OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                                        const CefString& origin_url,
                                        int64 new_size,
                                        CefRefPtr<CefRequestCallback> callback) {
        $clog(amo::cdevel << func_orient << amo::string(origin_url.ToString(),
                true).str() << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnQuotaRequest(browser, origin_url, new_size, callback);
            
            if (bHandled) {
                return true;
            }
        }
        
        static const int64 max_size = 1024 * 1024 * 200;  // 20mb.
        
        // Grant the quota request if the size is reasonable.
        callback->Continue(new_size <= max_size);
        return true;
        return CefRequestHandler::OnQuotaRequest(browser, origin_url, new_size,
                callback);
    }
#else
    bool RequestHandler::OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                                        const CefString& origin_url,
                                        int64 new_size,
                                        CefRefPtr<CefQuotaCallback> callback) {
        $clog(amo::cdevel << func_orient << origin_url << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
    
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnQuotaRequest(browser, origin_url, new_size, callback);
    
            if (bHandled) {
                return true;
            }
        }
    
        static const int64 max_size = 1024 * 1024 * 200;  // 20mb.
    
        // Grant the quota request if the size is reasonable.
        callback->Continue(new_size <= max_size);
        return true;
        return CefRequestHandler::OnQuotaRequest(browser, origin_url, new_size,
                callback);
    }
#endif
    
    
    bool RequestHandler::GetAuthCredentials(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame,
                                            bool isProxy,
                                            const CefString& host,
                                            int port,
                                            const CefString& realm,
                                            const CefString& scheme,
                                            CefRefPtr<CefAuthCallback> callback) {
        $clog(amo::cdevel << func_orient << amo::string(host.ToString(),
                true).str() << ":   " << amo::string(scheme.ToString(),
                        true).str() << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->GetAuthCredentials(browser, frame, isProxy,
                                                 host, port, realm, scheme, callback);
                                                 
            if (bHandled) {
                return true;
            }
        }
        
        return CefRequestHandler::GetAuthCredentials(browser, frame,
                isProxy, host, port, realm, scheme, callback);
    }
    
    
    
#if CEF_VERSION_REGION(3071, 10000)
    
    void RequestHandler::OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame,
                                            CefRefPtr<CefRequest> request,
                                            CefRefPtr<CefResponse> response,
                                            CefString& new_url) {
        CefString old_url = request->GetURL();
        $clog(amo::cdevel << func_orient << amo::string(old_url.ToString(),
                true).str() << ":" << amo::string(new_url.ToString(), true).str() << amo::endl;
             );
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnResourceRedirect(browser, frame, old_url, new_url);
        }
    }
#elif  CEF_VERSION_REGION(2840, 3029)
    void RequestHandler::OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame,
                                            CefRefPtr<CefRequest> request,
                                            CefString& new_url) {
        CefString old_url = request->GetURL();
        $clog(amo::cdevel << func_orient << amo::string(old_url.ToString(),
                true).str() << ":" << amo::string(new_url.ToString(), true).str() << amo::endl;
             );
    
        DelegateSet::iterator it = m_Delegates.begin();
    
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnResourceRedirect(browser, frame, old_url, new_url);
        }
    }
    
#elif  CEF_VERSION_REGION(2785, 2740)
    void RequestHandler::OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame,
                                            const CefString& old_url,
                                            CefString& new_url) {
        $clog(amo::cdevel << func_orient << amo::string(old_url.ToString(),
                true).str() << ":" << amo::string(new_url.ToString(), true).str() << amo::endl;
             );
        DelegateSet::iterator it = m_Delegates.begin();
    
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnResourceRedirect(browser, frame, old_url, new_url);
        }
    }
#elif CEF_VERSION_REGION(2357, 2785)
    void RequestHandler::OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame,
                                            CefRefPtr<CefRequest> request,
                                            CefString& new_url) {
        CefString old_url = request->GetURL();
        $clog(amo::cdevel << func_orient << amo::string(old_url.ToString(),
                true).str() << ":" << amo::string(new_url.ToString(), true).str() << amo::endl;
             );
    
        DelegateSet::iterator it = m_Delegates.begin();
    
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnResourceRedirect(browser, frame, old_url, new_url);
        }
    }
    
#elif CEF_VERSION_REGION(0, 2357)
    
    void RequestHandler::OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame, const CefString& old_url, CefString& new_url) {
        $clog(amo::cdevel << func_orient << amo::string(old_url.ToString(),
                true).str() << ":" << amo::string(new_url.ToString(), true).str() << amo::endl;
             );
        DelegateSet::iterator it = m_Delegates.begin();
    
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnResourceRedirect(browser, frame, old_url, new_url);
        }
    }
    
#endif
    
    
    CefRefPtr<CefResourceHandler> RequestHandler::GetResourceHandler(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefRequest> request) {
        $clog(amo::cdevel << func_orient << amo::string(request->GetURL().ToString(),
                true).str() << amo::endl;);
        amo::u8string url(request->GetURL().ToString(), true);
        std::string ss = url;
        
        if (ss.find("@file:///") != -1) {
            return new LocalSchemeHandler();
        }
        
        
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            CefRefPtr<CefResourceHandler> pResourceHandler =
                (*it)->GetResourceHandler(browser, frame, request);
                
            if (pResourceHandler.get()) {
                return pResourceHandler;
            }
        }
        
        return NULL;
    }
    
#if CHROME_VERSION_BUILD >= 2272
    CefRequestHandler::ReturnValue RequestHandler::OnBeforeResourceLoad(
        CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        CefRefPtr<CefRequest> request,
        CefRefPtr<CefRequestCallback> callback) {
        $clog(amo::cdevel << func_orient << amo::string(request->GetURL().ToString(),
                true).str() << amo::endl;);
                
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            CefRequestHandler::ReturnValue ret =
                (*it)->OnBeforeResourceLoad(browser, frame, request, callback);
                
            if (ret == RV_CANCEL) {
                return ret;
            }
        }
        
        return CefRequestHandler::OnBeforeResourceLoad(browser, frame, request,
                callback);
    }
#else
    bool RequestHandler::OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request) {
        $clog(amo::cdevel << func_orient << request->GetURL() << amo::endl;);
    
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
    
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnBeforeResourceLoad(browser, frame, request);
    
            if (bHandled) {
                return true;
            }
        }
    
        return CefRequestHandler::OnBeforeResourceLoad(browser, frame, request);
    }
#endif
    
#if CEF_VERSION_REGION(3359, 10000)
    bool RequestHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefRequest> request,
                                        bool user_gesture,
                                        bool is_redirect) {
        $clog(amo::cdevel << func_orient << request->GetTransitionType() << amo::string(
                  request->GetURL().ToString(), true).str() << amo::endl;);
                  
                  
        CefRequest::TransitionType type = request->GetTransitionType();
        
        //if ((unsigned int)type & TT_FORWARD_BACK_FLAG)
        //	return true;  // 不能前进与后退
        if (m_pMessageRouter) {
            m_pMessageRouter->OnBeforeBrowse(browser, frame);
        }
        
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnBeforeBrowse(browser, frame, request, is_redirect);
            
            if (bHandled) {
                return true;
            }
        }
        
        return CefRequestHandler::OnBeforeBrowse(browser,
                frame,
                request,
                user_gesture,
                is_redirect);
    }
#elif CEF_VERSION_REGION(0, 3359)
    bool RequestHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefRequest> request,
                                        bool is_redirect) {
        $clog(amo::cdevel << func_orient << request->GetTransitionType() << amo::string(
                  request->GetURL().ToString(), true).str() << amo::endl;);
    
    
        CefRequest::TransitionType type = request->GetTransitionType();
    
        //if ((unsigned int)type & TT_FORWARD_BACK_FLAG)
        //	return true;  // 不能前进与后退
        if (m_pMessageRouter) {
            m_pMessageRouter->OnBeforeBrowse(browser, frame);
        }
    
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();
    
        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnBeforeBrowse(browser, frame, request, is_redirect);
    
            if (bHandled) {
                return true;
            }
        }
    
        return CefRequestHandler::OnBeforeBrowse(browser, frame, request, is_redirect);
    }
    
#endif
    
    
    
    
    
    
    void RequestHandler::SetMessageRouter(CefRefPtr<MessageRouterBrowserSide>
                                          router) {
        m_pMessageRouter = router;
    }
    
    RequestHandler::RequestHandler() {
        m_pResourceHandler = new ResourceHandler();
    }
}