// Created by amoylel on 6/22/2017.
// Copyright (c) 2017 amoylel. All rights reserved.

#ifndef AMO_DISPLAYHANDLER_H__
#define AMO_DISPLAYHANDLER_H__


#include "handler/CefHeader.hpp"

#include "handler/HandlerDelegate.hpp"
namespace amo {

    /*!
     * @class	DisplayHandlerDelegate
     *
     * @brief	浏览器显示相关事件处理类，该类中的方法会在UI线程中被调用.
     */
    
    class DisplayHandlerDelegate {
    public:
        DisplayHandlerDelegate() {}
        
        virtual ~DisplayHandlerDelegate() {}
        
        virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     const CefString& url) {
        }
        
        virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
                                   const CefString& title) {
                                   
        }
        
        virtual bool OnTooltip(CefRefPtr<CefBrowser> browser,
                               CefString& text) {
            return false;
        }
        
        virtual void OnStatusMessage(CefRefPtr<CefBrowser> browser,
                                     const CefString& value) {
                                     
        }
        
#if CHROME_VERSION_BUILD >= 3282
        virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                      cef_log_severity_t level,
                                      const CefString& message,
                                      const CefString& source,
                                      int line) {
            return false;
        }
#else
        virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                      const CefString& message,
                                      const CefString& source,
                                      int line) {
            return false;
        }
        
#endif
        
        
        
    };
    
    /*!
     * @class	DisplayHandler
     *
     * @brief	浏览器显示相关事件处理类，该类中的方法会在UI线程中被调用.
     */
    
    class DisplayHandler : public CefDisplayHandler
        , public HandlerDelegate < DisplayHandlerDelegate > {
    public:
        DisplayHandler() {}
        
        virtual ~DisplayHandler() {}
        
        /*!
         * @fn	virtual void DisplayHandler::OnAddressChange(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		const CefString& url);
         *
         * @brief	URL 发生改变时触发
         * 			B UI.
         *
         * @param	browser	The browser.
         * @param	frame  	The frame.
         * @param	url	   	URL of the document.
         */
        
        virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     const CefString& url) override;
                                     
        /*!
         * @fn	virtual void DisplayHandler::OnTitleChange(
         * 		CefRefPtr<CefBrowser> browser,
         * 		const CefString& title);
         *
         * @brief	TITLE 发生改变时触发
         * 			B UI.
         *
         * @param	browser	The browser.
         * @param	title  	The title.
         */
        
        virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
                                   const CefString& title) override;
                                   
        /*!
         * @fn	virtual bool DisplayHandler::OnTooltip(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefString& text);
         *
         * @brief	显示ToolTip 触发
         * 			B UI.
         *
         * @param	browser			The browser.
         * @param 	text	The text.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool OnTooltip(CefRefPtr<CefBrowser> browser,
                               CefString& text) override;
                               
        /*!
         * @fn	virtual void DisplayHandler::OnStatusMessage(
         * 		CefRefPtr<CefBrowser> browser,
         * 		const CefString& value);
         *
         * @brief	当浏览器收到状态消息时触发
         * 			B UI.
         *
         * @param	browser	The browser.
         * @param	value  	The value.
         */
        
        virtual void OnStatusMessage(CefRefPtr<CefBrowser> browser,
                                     const CefString& value) override;
                                     
        /*!
         * @fn	virtual bool DisplayHandler::OnConsoleMessage(
         * 		CefRefPtr<CefBrowser> browser,
         * 		const CefString& message,
         * 		const CefString& source,
         * 		int line);
         *
         * @brief	浏览器需要显示Console消息时触发
         * 			B UI.
         *
         * @param	browser	The browser.
         * @param	message	The message.
         * @param	source 	Source for the.
         * @param	line   	The line.
         *
         * @return	true if it succeeds, false if it fails.
         */
#if CHROME_VERSION_BUILD >=3282
        virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                      cef_log_severity_t level,
                                      const CefString& message,
                                      const CefString& source,
                                      int line) override;
#else
                                      
        virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                      const CefString& message,
                                      const CefString& source,
                                      int line) override;
#endif
                                      
        IMPLEMENT_REFCOUNTING(DisplayHandler);
    };
}
#endif // AMO_DISPLAYHANDLER_H__
