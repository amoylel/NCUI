// Created by amoylel on 6/22/2017.
// Copyright (c) 2017 amoylel. All rights reserved.

#ifndef AMO_LOADHANDLER_H__
#define AMO_LOADHANDLER_H__

#include "handler/CefHeader.hpp"

#include "handler/HandlerDelegate.hpp"

namespace amo {

    /*!
     * @class	LoadHandlerDelegate
     *
     * @brief	页面加载事件处理类，该类中的方法会在UI线程或渲染进程中的主线程中调用.
     */
    
    class LoadHandlerDelegate {
    public:
        LoadHandlerDelegate() {}
        
        virtual ~LoadHandlerDelegate() {}
        
        virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                          bool isLoading, bool canGoBack, bool canGoForward) {
        }
        
        virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame) {
        }
        
        virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame, int httpStatusCode) {
        }
        
        virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame, CefLoadHandler::ErrorCode errorCode,
                                 const CefString& errorText, const CefString& failedUrl) {
        }
        
    };
    
    /*!
     * @class	LoadHandler
     *
     * @brief	页面加载事件处理类，该类中的方法会在UI线程或渲染进程中的主线程中调用.
     */
    
    class LoadHandler : public CefLoadHandler
        , public HandlerDelegate < LoadHandlerDelegate > {
    public:
        LoadHandler() {}
        
        virtual ~LoadHandler() {}
        
        /*!
         * @fn	virtual void LoadHandler::OnLoadingStateChange(
         * 		CefRefPtr<CefBrowser> browser,
         * 		bool isLoading,
         * 		bool canGoBack, bool canGoForward);
         *
         * @brief	页面加载状态发生改变时触发
         * 			B UI / R MAIN
         *			在页面加载过程中，该函数会被调用两次:
         *			一、用户发起时
         *			二、加载完成或取消时.
         *
         * @param	browser			The browser.
         * @param	isLoading   	true if this object is loading.
         * @param	canGoBack   	true if this object can go back.
         * @param	canGoForward	true if this object can go forward.
         */
        
        virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                          bool isLoading,
                                          bool canGoBack,
                                          bool canGoForward) override;
                                          
                                          
        /*!
         * @fn	virtual void LoadHandler::OnLoadStart(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame);
         *
         * @brief	页面开始加载时触发
         * 			B UI / R MAIN.
         *
         * @param	browser	The browser.
         * @param	frame  	The frame.
         */
        
        
        
        
#if CHROME_VERSION_BUILD >= 2743
        virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 TransitionType transition_type)override;
#else
        virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame) override;
#endif
                                 
                                 
        /*!
         * @fn	virtual void LoadHandler::OnLoadEnd(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		int httpStatusCode);
         *
         * @brief	页面加载完成时触发
         * 			B UI / R MAIN.
         *
         * @param	browser		  	The browser.
         * @param	frame		  	The frame.
         * @param	httpStatusCode	The HTTP status code.
         */
        
        virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame, int httpStatusCode)override;
                               
        /*!
         * @fn	virtual void LoadHandler::OnLoadError(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		ErrorCode errorCode,
         * 		const CefString& errorText,
         * 		const CefString& failedUrl);
         *
         * @brief	页面加载出现错误时触发
         * 			B UI / R MAIN.
         *
         * @param	browser  	The browser.
         * @param	frame	 	The frame.
         * @param	errorCode	The error code.
         * @param	errorText	The error text.
         * @param	failedUrl	URL of the failed.
         */
        
        virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame, ErrorCode errorCode,
                                 const CefString& errorText, const CefString& failedUrl)override;
                                 
        IMPLEMENT_REFCOUNTING(LoadHandler);
    };
    
}

#endif // AMO_LOADHANDLER_H__
