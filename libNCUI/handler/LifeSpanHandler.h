// Created by amoylel on 6/22/2017.
// Copyright (c) 2017 amoylel. All rights reserved.

#ifndef AMO_LIFESPANHANDLER_H__
#define AMO_LIFESPANHANDLER_H__

#include "handler/CefHeader.hpp"

#include "handler/HandlerDelegate.hpp"


namespace amo {
    class MessageRouterBrowserSide;
    
    /*!
     * @class	LifeSpanHandlerDelegate
     *
     * @brief	生成周期事件处理类，该类中的大部分方法会在UI线程中被调用，除非指定.
     */
    
    class LifeSpanHandlerDelegate {
    public:
        LifeSpanHandlerDelegate() {}
        
        virtual ~LifeSpanHandlerDelegate() {}
        
        
#if CHROME_VERSION_BUILD >= 2357
        virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   const CefString& target_url,
                                   const CefString& target_frame_name,
                                   CefLifeSpanHandler::WindowOpenDisposition target_disposition,
                                   bool user_gesture,
                                   const CefPopupFeatures& popupFeatures,
                                   CefWindowInfo& windowInfo,
                                   CefRefPtr<CefClient>& client,
                                   CefBrowserSettings& settings,
                                   bool* no_javascript_access) {
            return false;
        }
        
#else
        virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   const CefString& target_url,
                                   const CefString& target_frame_name,
                                   const CefPopupFeatures& popupFeatures,
                                   CefWindowInfo& windowInfo,
                                   CefRefPtr<CefClient>& client,
                                   CefBrowserSettings& settings,
                                   bool* no_javascript_access) {
            return false;
        }
#endif
        
        
        virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) {
        }
        
#if CHROME_VERSION_BUILD >= 2704
        
#else
        virtual bool RunModal(CefRefPtr<CefBrowser> browser) {
            return false;
        }
#endif
        
        
        virtual bool DoClose(CefRefPtr<CefBrowser> browser) {
            return false;
        }
        
        virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) {
        
        }
        
        
    };
    
    /*!
     * @class	LifeSpanHandler
     *
     * @brief	生成周期事件处理类，该类中的大部分方法会在UI线程中被调用，除非指定.
     */
    
    class LifeSpanHandler : public CefLifeSpanHandler
        , public HandlerDelegate < LifeSpanHandlerDelegate > {
    public:
        LifeSpanHandler() {
            m_bIsClosing = false;
        }
        
        virtual ~LifeSpanHandler() {}
        
        
        
        
#if CHROME_VERSION_BUILD >= 2357
        
        /*!
         * @fn	virtual bool LifeSpanHandler::OnBeforePopup(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		const CefString& target_url,
         * 		const CefString& target_frame_name,
         * 		WindowOpenDisposition target_disposition,
         * 		bool user_gesture,
         * 		const CefPopupFeatures& popupFeatures,
         * 		CefWindowInfo& windowInfo,
         * 		CefRefPtr<CefClient>& client,
         * 		CefBrowserSettings& settings,
         * 		bool* no_javascript_access);
         *
         * @brief	新浏览器窗口创建前触发
         * 			B IO.
         *
         * @param	browser							The browser.
         * @param	frame							The frame.
         * @param	target_url						URL of the target.
         * @param	target_frame_name				Name of the target frame.
         * @param	target_disposition				Target disposition.
         * @param	user_gesture					true to user gesture.
         * @param	popupFeatures					The popup features.
         * @param 	windowInfo				Information describing the window.
         * @param 	client					The client.
         * @param 	settings				Options for controlling the operation.
         * @param 	no_javascript_access	If non-null, the no javascript access.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   const CefString& target_url,
                                   const CefString& target_frame_name,
                                   WindowOpenDisposition target_disposition,
                                   bool user_gesture,
                                   const CefPopupFeatures& popupFeatures,
                                   CefWindowInfo& windowInfo,
                                   CefRefPtr<CefClient>& client,
                                   CefBrowserSettings& settings,
                                   bool* no_javascript_access) override;
#else
        virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   const CefString& target_url,
                                   const CefString& target_frame_name,
                                   const CefPopupFeatures& popupFeatures,
                                   CefWindowInfo& windowInfo,
                                   CefRefPtr<CefClient>& client,
                                   CefBrowserSettings& settings,
                                   bool* no_javascript_access) override;
#endif
                                   
        /*!
         * @fn	virtual void LifeSpanHandler::OnAfterCreated(
         * 		CefRefPtr<CefBrowser> browser);
         *
         * @brief	浏览器创建后触发
         * 			B UI.
         *
         * @param	browser	The browser.
         */
        
        virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
        
#if CHROME_VERSION_BUILD >= 2704
#else
        /*!
        * @fn	virtual bool LifeSpanHandler::RunModal(
        * 		CefRefPtr<CefBrowser> browser);
        *
        * @brief	显示一个模式对话框时触发
        * 			B UI.
        *
        * @param	browser	The browser.
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        virtual bool RunModal(CefRefPtr<CefBrowser> browser) override;
#endif
        
        
        /*!
         * @fn	virtual bool LifeSpanHandler::DoClose(
         * 		CefRefPtr<CefBrowser> browser);
         *
         * @brief	Executes the close operation
         * 			B UI.
         *
         * @param	browser	The browser.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
        
        /*!
         * @fn	virtual void LifeSpanHandler::OnBeforeClose(
         * 		CefRefPtr<CefBrowser> browser);
         *
         * @brief	浏览器销毁前触发
         * 			B UI.
         *
         * @param	browser	The browser.
         */
        
        virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
        
        /*!
         * @fn	virtual void LifeSpanHandler::SetMessageRouter(
         * 		CefRefPtr<MessageRouterBrowserSide> router);
         *
         * @brief	设置消息路由
         * 			B.
         *
         * @param	router	The router.
         */
        
        virtual void SetMessageRouter(CefRefPtr<MessageRouterBrowserSide> router) ;
        
        IMPLEMENT_REFCOUNTING(LifeSpanHandler);
        
    private:
        /*! @brief	The message router. */
        CefRefPtr<MessageRouterBrowserSide> m_pMessageRouter;
        /*! @brief	Number of browsers. */
        static int m_BrowserCount;
        /*! @brief	true if this object is closing. */
        bool m_bIsClosing;
    };
    
}
#endif // AMO_LIFESPANHANDLER_H__
