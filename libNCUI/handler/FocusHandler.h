// Created by amoylel on 6/22/2017.
// Copyright (c) 2017 amoylel. All rights reserved.

#ifndef AMO_FOCUSHANDLER_H__
#define AMO_FOCUSHANDLER_H__

#include "handler/CefHeader.hpp"

#include "handler/HandlerDelegate.hpp"

namespace amo {

    /*!
    * @class	FocusHandlerDelegate
    *
    * @brief	焦点事件处理类，该类中的方法会在UI线程中被调用.
    */
    
    class FocusHandlerDelegate {
    public:
        FocusHandlerDelegate() {}
        
        virtual ~FocusHandlerDelegate() {}
        
        virtual void OnTakeFocus(CefRefPtr<CefBrowser> browser,
                                 bool next) {
        }
        
        virtual bool OnSetFocus(CefRefPtr<CefBrowser> browser,
                                CefFocusHandler::FocusSource source) {
            return false;
        }
        
        virtual void OnGotFocus(CefRefPtr<CefBrowser> browser) {
        }
        
        
        
        
    };
    
    /*!
    * @class	FocusHandler
    *
    * @brief	焦点事件处理类，该类中的方法会在UI线程中被调用.
    */
    
    class FocusHandler : public CefFocusHandler
        , public HandlerDelegate < FocusHandlerDelegate > {
    public:
        FocusHandler() {}
        
        virtual ~FocusHandler() {}
        
        /*!
        * @fn	virtual void FocusHandler::OnTakeFocus(
        * 		CefRefPtr<CefBrowser> browser,
        * 		bool next);
        *
        * @brief	切换焦点时触发
        * 			B UI.
        *
        * @param	browser	The browser.
        * @param	next   	true to next.
        */
        
        virtual void OnTakeFocus(CefRefPtr<CefBrowser> browser,
                                 bool next) override;
                                 
        /*!
        * @fn	virtual bool FocusHandler::OnSetFocus(
        * 		CefRefPtr<CefBrowser> browser,
        * 		FocusSource source);
        *
        * @brief	页面请求设置焦点时触发
        * 			B UI.
        *
        * @param	browser	The browser.
        * @param	source 	Source for the.
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        virtual bool OnSetFocus(CefRefPtr<CefBrowser> browser,
                                FocusSource source) override;
                                
        /*!
        * @fn	virtual void FocusHandler::OnGotFocus(CefRefPtr<CefBrowser> browser);
        *
        * @brief	页面获得焦点时触发
        * 			B UI.
        *
        * @param	browser	The browser.
        */
        
        virtual void OnGotFocus(CefRefPtr<CefBrowser> browser) override;
        
        IMPLEMENT_REFCOUNTING(FocusHandler);
    };
}
#endif // AMO_FOCUSHANDLER_H__
