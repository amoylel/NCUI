// Created by amoylel on 6/22/2017.
// Copyright (c) 2017 amoylel. All rights reserved.

#ifndef AMO_REQUESTHANDLER_H__
#define AMO_REQUESTHANDLER_H__


#include "handler/CefHeader.hpp"

#include "handler/HandlerDelegate.hpp"
#include "ResourceHandler.h"


namespace amo {

    class MessageRouterBrowserSide;
    class ResourceHandler;
    
    /*!
     * @class	RequestHandlerDelegate
     *
     * @brief	浏览器请求相关的事件处理类.
     */
    
    class RequestHandlerDelegate {
    public:
        RequestHandlerDelegate() {}
        
        virtual ~RequestHandlerDelegate() {}
        
        virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefRequest> request,
                                    bool is_redirect) {
            return false;
        }
        
        
        
        virtual CefRefPtr<CefResourceHandler> GetResourceHandler(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request) {
            return NULL;
        }
        
        virtual void OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        const CefString& old_url,
                                        CefString& new_url) {
        }
        
        virtual bool GetAuthCredentials(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        bool isProxy,
                                        const CefString& host,
                                        int port,
                                        const CefString& realm,
                                        const CefString& scheme,
                                        CefRefPtr<CefAuthCallback> callback) {
            return false;
        }
        
        
        
        virtual void OnProtocolExecution(CefRefPtr<CefBrowser> browser,
                                         const CefString& url,
                                         bool& allow_os_execution) {
        }
        
        
#if CHROME_VERSION_BUILD >= 2272
        virtual bool OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                                    const CefString& origin_url,
                                    int64 new_size,
                                    CefRefPtr<CefRequestCallback> callback) {
            return false;
        }
        
        virtual bool OnCertificateError(
            CefRefPtr<CefBrowser> browser,
            cef_errorcode_t cert_error,
            const CefString& request_url,
            CefRefPtr<CefSSLInfo> ssl_info,
            CefRefPtr<CefRequestCallback> callback) {
            return false;
        }
        
        virtual CefRequestHandler::ReturnValue OnBeforeResourceLoad(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request,
            CefRefPtr<CefRequestCallback> callback) {
            return RV_CONTINUE;
        }
        
#else
        virtual bool OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefRefPtr<CefRequest> request) {
            return false;
        }
        
        virtual bool OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                                    const CefString& origin_url,
                                    int64 new_size,
                                    CefRefPtr<CefQuotaCallback> callback) {
            return false;
        }
        
        virtual bool OnCertificateError(cef_errorcode_t cert_error,
                                        const CefString& request_url,
                                        CefRefPtr<CefAllowCertificateErrorCallback> callback) {
            return false;
        }
#endif
        
        
#if CHROME_VERSION_BUILD >= 2454
        virtual bool OnBeforePluginLoad(CefRefPtr<CefBrowser> browser,
                                        const CefString& url,
                                        const CefString& policy_url,
                                        CefRefPtr<CefWebPluginInfo> info) {
            return false;
        }
#else
        virtual bool OnBeforePluginLoad(CefRefPtr<CefBrowser> browser,
                                        const CefString& url,
                                        const CefString& policy_url,
                                        CefRefPtr<CefWebPluginInfo> info) {
            return false;
        }
#endif
        
        
        
        virtual void OnPluginCrashed(CefRefPtr<CefBrowser> browser,
                                     const CefString& plugin_path) {
        }
        
        virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                               CefRequestHandler::TerminationStatus status) {
        }
        
        
    };
    
    /*!
     * @class	RequestHandler
     *
     * @brief	浏览器请求相关的事件处理类.
     */
    
    class RequestHandler : public CefRequestHandler
        , public HandlerDelegate < RequestHandlerDelegate > {
    public:
        RequestHandler();
        
        virtual ~RequestHandler() {}
        
        /*!
         * @fn	virtual bool RequestHandler::OnBeforeBrowse(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		CefRefPtr<CefRequest> request,
         * 		bool is_redirect);
         *
         * @brief	浏览器导航前触发
         * 			request 在当前函数中禁止修改
         * 			B UI.
         *
         * @param	browser	   	The browser.
         * @param	frame	   	The frame.
         * @param	request	   	The request.
         * @param	is_redirect	true if this object is redirect.
         *
         * @return	true if it succeeds, false if it fails.
         */
#if CEF_VERSION_REGION(3359, 10000)
        virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefRequest> request,
                                    bool user_gesture,
                                    bool is_redirect)override;
                                    
#elif CEF_VERSION_REGION(0, 3359)
        virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefRequest> request,
                                    bool is_redirect) override;
                                    
#endif
                                    
#if CHROME_VERSION_BUILD >= 2272
                                    
        /*!
         * @fn	virtual ReturnValue RequestHandler::OnBeforeResourceLoad(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		CefRefPtr<CefRequest> request,
         * 		CefRefPtr<CefRequestCallback> callback);
         *
         * @brief	资源请求被加载之前触发
         * 			B IO.
         *
         * @param	browser 	The browser.
         * @param	frame   	The frame.
         * @param	request 	The request.
         * @param	callback	The callback.
         *
         * @return	A ReturnValue.
         */
        
        virtual ReturnValue OnBeforeResourceLoad(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request,
            CefRefPtr<CefRequestCallback> callback);
#else
        virtual bool OnBeforeResourceLoad(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefRefPtr<CefRequest> request) override;
#endif
            
        /*!
         * @fn	virtual CefRefPtr<CefResourceHandler> RequestHandler::GetResourceHandler(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		CefRefPtr<CefRequest> request);
         *
         * @brief	资源请求被加载之前触发，可自定义ResourceHandler
         * 			B IO.
         *
         * @param	browser	The browser.
         * @param	frame  	The frame.
         * @param	request	The request.
         *
         * @return	The resource handler.
         */
        
        virtual CefRefPtr<CefResourceHandler> GetResourceHandler(
            CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request) override;
            
        /*!
         * @fn	virtual void RequestHandler::OnResourceRedirect(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		const CefString& old_url,
         * 		CefString& new_url);
         *
         * @brief	资源重定向时触发
         * 			B IO.
         *
         * @param	browser		   	The browser.
         * @param	frame		   	The frame.
         * @param	old_url		   	URL of the old.
         * @param 	new_url	URL of the new.
         */
        
#if CEF_VERSION_REGION(3071, 10000)
        virtual void OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefRequest> request,
                                        CefRefPtr<CefResponse> response,
                                        CefString& new_url) override;
#elif CEF_VERSION_REGION( 2840, 3029)
                                        
                                        
                                        
        virtual void OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefRequest> request,
                                        CefString& new_url) override;
                                        
#elif CEF_VERSION_REGION( 2785, 2840)
        virtual void OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        const CefString& old_url,
                                        CefString& new_url) override;
                                        
                                        
#elif CEF_VERSION_REGION(2357,2785)
        virtual void OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefRequest> request,
                                        CefString& new_url) override;
#elif CEF_VERSION_REGION(0,2357)
        virtual void OnResourceRedirect(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        const CefString& old_url,
                                        CefString& new_url) override;
#endif
                                        
                                        
        /*!
         * @fn	virtual bool RequestHandler::GetAuthCredentials(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		bool isProxy,
         * 		const CefString& host,
         * 		int port,
         * 		const CefString& realm,
         * 		const CefString& scheme,
         * 		CefRefPtr<CefAuthCallback> callback);
         *
         * @brief	当浏览器需要用户凭据时触
         * 			B IO.
         *
         * @param	browser 	The browser.
         * @param	frame   	The frame.
         * @param	isProxy 	true if this object is proxy.
         * @param	host		The host.
         * @param	port		The port.
         * @param	realm   	The realm.
         * @param	scheme  	The scheme.
         * @param	callback	The callback.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool GetAuthCredentials(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        bool isProxy,
                                        const CefString& host,
                                        int port,
                                        const CefString& realm,
                                        const CefString& scheme,
                                        CefRefPtr<CefAuthCallback> callback) override;
                                        
                                        
#if CHROME_VERSION_BUILD >= 2272
                                        
        /*!
         * @fn	virtual bool RequestHandler::OnQuotaRequest(
         * 		CefRefPtr<CefBrowser> browser,
         * 		const CefString& origin_url,
         * 		int64 new_size,
         * 		CefRefPtr<CefRequestCallback> callback);
         *
         * @brief	当JavaScript请求一个特定的存储配额时触发
         * 			B IO.
         *
         * @param	browser   	The browser.
         * @param	origin_url	URL of the origin.
         * @param	new_size  	Size of the new.
         * @param	callback  	The callback.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                                    const CefString& origin_url,
                                    int64 new_size,
                                    CefRefPtr<CefRequestCallback> callback) override;
#else
        virtual bool OnQuotaRequest(CefRefPtr<CefBrowser> browser,
                                    const CefString& origin_url,
                                    int64 new_size,
                                    CefRefPtr<CefQuotaCallback> callback) override;
#endif
                                    
        /*!
         * @fn	virtual void RequestHandler::OnProtocolExecution(
         * 		CefRefPtr<CefBrowser> browser,
         * 		const CefString& url,
         * 		bool& allow_os_execution);
         *
         * @brief	request url 未知协议时触发
         * 			B UI.
         *
         * @param	browser					  	The browser.
         * @param	url						  	URL of the document.
         * @param 	allow_os_execution	The allow operating system execution.
         */
        
        virtual void OnProtocolExecution(CefRefPtr<CefBrowser> browser,
                                         const CefString& url,
                                         bool& allow_os_execution) override;
                                         
                                         
#if CHROME_VERSION_BUILD >= 2272
                                         
        /*!
         * @fn	virtual bool RequestHandler::OnCertificateError(
         * 		CefRefPtr<CefBrowser> browser,
         * 		cef_errorcode_t cert_error,
         * 		const CefString& request_url,
         * 		CefRefPtr<CefSSLInfo> ssl_info,
         * 		CefRefPtr<CefRequestCallback> callback);
         *
         * @brief	Executes the certificate error action
         * 			B UI.
         *
         * @param	browser	   	The browser.
         * @param	cert_error 	The cert error.
         * @param	request_url	URL of the request.
         * @param	ssl_info   	Information describing the ssl.
         * @param	callback   	The callback.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool OnCertificateError(
            CefRefPtr<CefBrowser> browser,
            cef_errorcode_t cert_error,
            const CefString& request_url,
            CefRefPtr<CefSSLInfo> ssl_info,
            CefRefPtr<CefRequestCallback> callback) override;
#else
        virtual bool OnCertificateError(cef_errorcode_t cert_error,
                                        const CefString& request_url,
                                        CefRefPtr<CefAllowCertificateErrorCallback> callback) override;
#endif
            
#if CEF_VERSION_REGION(2454, 10000)
            
            
        /*!
         * @fn	virtual bool RequestHandler::OnBeforePluginLoad(
         * 		CefRefPtr<CefBrowser> browser,
         * 		const CefString& url,
         * 		const CefString& policy_url,
         * 		CefRefPtr<CefWebPluginInfo> info);
         *
         * @brief	插件加载前触发
         * 			B IO.
         *
         * @param	browser   	The browser.
         * @param	url		  	URL of the document.
         * @param	policy_url	URL of the policy.
         * @param	info	  	The information.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        /* virtual bool OnBeforePluginLoad(CefRefPtr<CefBrowser> browser,
                                         const CefString& url,
                                         const CefString& policy_url,
                                         CefRefPtr<CefWebPluginInfo> info) override;*/
        
#elif CEF_VERSION_REGION(0, 2454)
        virtual bool OnBeforePluginLoad(CefRefPtr<CefBrowser> browser,
                                        const CefString& url,
                                        const CefString& policy_url,
                                        CefRefPtr<CefWebPluginInfo> info) override;
#endif
        
        /*!
         * @fn	virtual void RequestHandler::OnPluginCrashed(
         * 		CefRefPtr<CefBrowser> browser,
         * 		const CefString& plugin_path);
         *
         * @brief	插件崩溃时触发
         * 			B UI.
         *
         * @param	browser	   	The browser.
         * @param	plugin_path	Full pathname of the plugin file.
         */
        
        virtual void OnPluginCrashed(CefRefPtr<CefBrowser> browser,
                                     const CefString& plugin_path) override;
                                     
        /*!
         * @fn	virtual void RequestHandler::OnRenderProcessTerminated(
         * 		CefRefPtr<CefBrowser> browser,
         * 		TerminationStatus status);
         *
         * @brief	渲染进程终止时触发
         * 			B UI.
         *
         * @param	browser	The browser.
         * @param	status 	The status.
         */
        
        virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                               TerminationStatus status) override;
                                               
        /*!
         * @fn	void RequestHandler::SetMessageRouter(
         * 		CefRefPtr<MessageRouterBrowserSide> router);
         *
         * @brief	Sets message router.
         *
         * @param	router	The router.
         */
        
        void SetMessageRouter(CefRefPtr<MessageRouterBrowserSide> router);
        IMPLEMENT_REFCOUNTING(RequestHandler);
        
    private:
        CefRefPtr<MessageRouterBrowserSide> m_pMessageRouter;
        CefRefPtr<ResourceHandler> m_pResourceHandler;
    };
    
}
#endif // AMO_REQUESTHANDLER_H__
