// Created by amoylel on 6/22/2017.
// Copyright (c) 2017 amoylel. All rights reserved.

#ifndef AMO_CLIENTHANDLER_H__
#define AMO_CLIENTHANDLER_H__

#include <memory>

#include "handler/CefHeader.hpp"
#include "ipc/Any.hpp"

#include "handler/HandlerDelegate.hpp"
#include "handler/MessageRouterBrowserSide.h"

#include "handler/BrowserManager.hpp"
#include "handler/LifeSpanHandler.h"
#include <functional>






namespace amo {

    class ProcessExchanger;
    
    class ContextMenuHandler;
    class DialogHandler;
    class DisplayHandler;
    class DownloadHandler;
    class DragHandler;
    class FocusHandler;
    class GeolocationHandler;
    class JSDialogHandler;
    class KeyboardHandler;
    class LifeSpanHandler;
    class LoadHandler;
    class RenderHandler;
    class RequestHandler;
    class MessageRouterBrowserSide;
    
    class ContextMenuHandlerDelegate;
    class DialogHandlerDelegate;
    class DisplayHandlerDelegate;
    class DownloadHandlerDelegate;
    class DragHandlerDelegate;
    class FocusHandlerDelegate;
    class GeolocationHandlerDelegate;
    class JSDialogHandlerDelegate;
    class KeyboardHandlerDelegate;
    class LifeSpanHandlerDelegate;
    class LoadHandlerDelegate;
    class RenderHandlerDelegate;
    class RequestHandlerDelegate;
    class MessageRouterBrowserSide;
    class MessageHandlerDelegate;
    
    class ClientHandlerDelegate {
    public:
        ClientHandlerDelegate() {}
        
        virtual ~ClientHandlerDelegate() {}
        
        virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                              CefProcessId source_process,
                                              CefRefPtr<CefProcessMessage> message) {
            return false;
        }
        
    };
    
    
    class BrowserTempInfo {
    public:
        BrowserTempInfo() {
        
        
        }
        BrowserTempInfo(CefRefPtr<CefBrowser> browser,
                        std::function<bool(int, IPCMessage::SmartType)> fn) {
            pBrowser = browser;
            m_fnExec = fn;
        }
        
        CefRefPtr<CefBrowser> pBrowser;
        std::function<bool(int, IPCMessage::SmartType)> m_fnExec;
    };
    
    // Cef各类接口处理类
    class ClientHandler : public CefClient
        , public HandlerDelegate<ClientHandlerDelegate>
        , public LifeSpanHandlerDelegate
        , public BrowserManager<PID_BROWSER> {
    public:
        typedef std::set<MessageRouterBrowserSide::Handler*> MessageHandlerSet;
        
        static bool SingleProcessMode(bool single_instance = false);
    public:
        ClientHandler();
        
        virtual ~ClientHandler();
        
        static void createPipeClient(IPCMessage::SmartType msg);
        
        static  Any createPipeClientImpl(IPCMessage::SmartType msg);
        
        
        void BrowserThreadActivityDetector();
        
        virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override;
        virtual CefRefPtr<CefDialogHandler> GetDialogHandler() override;
        virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override;
        virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() override;
        virtual CefRefPtr<CefDragHandler> GetDragHandler() override;
        virtual CefRefPtr<CefFocusHandler> GetFocusHandler() override;
#if CEF_VERSION_REGION(0,3325)
        virtual CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() override;
#endif
        
        virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() override;
        virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override;
        virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
        virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override;
        virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;
        virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override;
        virtual CefRefPtr<MessageRouterBrowserSide> GetMessageRouterBrowserSide() ;
        
        /*!
         * @fn	virtual bool OnProcessMessageReceived(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefProcessId source_process,
         * 		CefRefPtr<CefProcessMessage> message);
         *
         * @brief	处理来自子进程的消息.
         *
         * @param	browser		  	The browser.
         * @param	source_process	Source process.
         * @param	message		  	The message.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                              CefProcessId source_process,
                                              CefRefPtr<CefProcessMessage> message) override;
                                              
        /*!
         * @fn	bool RegisterContextMenuHandlerDelegate(
         * 		ContextMenuHandlerDelegate* delegate,
         * 		int nIndex = -1);
         *
         * @brief	注册右键菜单事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         * @param	nIndex				(Optional) the index.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterContextMenuHandlerDelegate(ContextMenuHandlerDelegate* delegate,
                                                int nIndex = -1);
                                                
        /*!
         * @fn	void UnregisterContextMenuHandlerDelegate(
         * 		ContextMenuHandlerDelegate* delegate);
         *
         * @brief	反注册右键菜单事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterContextMenuHandlerDelegate(ContextMenuHandlerDelegate* delegate);
        
        /*!
         * @fn	bool RegisterDialogHandlerDelegate(
         * 		DialogHandlerDelegate* delegate,
         * 		int nIndex = -1);
         *
         * @brief	注册文件对话框处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         * @param	nIndex				(Optional) the index.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterDialogHandlerDelegate(DialogHandlerDelegate* delegate,
                                           int nIndex = -1);
                                           
        /*!
         * @fn	void UnregisterDialogHandlerDelegate(DialogHandlerDelegate* delegate);
         *
         * @brief	反注册文件对话框处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterDialogHandlerDelegate(DialogHandlerDelegate* delegate);
        
        /*!
         * @fn	bool RegisterDisplayHandlerDelegate(
         * 		DisplayHandlerDelegate* delegate,
         * 		int nIndex = -1);
         *
         * @brief	注册显示事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         * @param	nIndex				(Optional) the index.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterDisplayHandlerDelegate(DisplayHandlerDelegate* delegate,
                                            int nIndex = -1);
                                            
        /*!
         * @fn	void UnregisterDisplayHandlerDelegate(DisplayHandlerDelegate* delegate);
         *
         * @brief	反注册显示事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterDisplayHandlerDelegate(DisplayHandlerDelegate* delegate);
        
        /*!
         * @fn	bool RegisterDownloadHandlerDelegate(
         * 		DownloadHandlerDelegate* delegate,
         * 		int nIndex = -1);
         *
         * @brief	注册下载事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         * @param	nIndex				(Optional) the index.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterDownloadHandlerDelegate(DownloadHandlerDelegate* delegate,
                                             int nIndex = -1);
                                             
        /*!
         * @fn	void UnregisterDownloadHandlerDelegate(DownloadHandlerDelegate* delegate);
         *
         * @brief	反注册下载事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterDownloadHandlerDelegate(DownloadHandlerDelegate* delegate);
        
        /*!
         * @fn	bool RegisterDragHandlerDelegate(
         * 		DragHandlerDelegate* delegate,
         * 		int nIndex = -1);
         *
         * @brief	注册拖拽事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         * @param	nIndex				(Optional) the index.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterDragHandlerDelegate(DragHandlerDelegate* delegate,
                                         int nIndex = -1);
                                         
        /*!
         * @fn	void UnregisterDragHandlerDelegate(DragHandlerDelegate* delegate);
         *
         * @brief	反注册拖拽事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterDragHandlerDelegate(DragHandlerDelegate* delegate);
        
        /*!
         * @fn	bool RegisterFocusHandlerDelegate(
         * 		FocusHandlerDelegate* delegate,
         * 		int nIndex = -1);
         *
         * @brief	注册焦点事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         * @param	nIndex				(Optional) the index.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterFocusHandlerDelegate(FocusHandlerDelegate* delegate,
                                          int nIndex = -1);
                                          
        /*!
         * @fn	void UnregisterFocusHandlerDelegate(FocusHandlerDelegate* delegate);
         *
         * @brief	反注册焦点事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterFocusHandlerDelegate(FocusHandlerDelegate* delegate);
        
        /*!
         * @fn	bool RegisterGeolocationHandlerDelegate(
         * 		GeolocationHandlerDelegate* delegate,
         * 		int nIndex = -1);
         *
         * @brief	注册地理位置处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         * @param	nIndex				(Optional) the index.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterGeolocationHandlerDelegate(GeolocationHandlerDelegate* delegate,
                                                int nIndex = -1);
                                                
        /*!
         * @fn	void UnregisterGeolocationHandlerDelegate(GeolocationHandlerDelegate* delegate);
         *
         * @brief	反注册地理位置处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterGeolocationHandlerDelegate(GeolocationHandlerDelegate* delegate);
        
        /*!
         * @fn	bool RegisterJSDialogHandlerDelegate(
         * 		JSDialogHandlerDelegate* delegate,
         * 		int nIndex = -1);
         *
         * @brief	消息框处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         * @param	nIndex				(Optional) the index.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterJSDialogHandlerDelegate(JSDialogHandlerDelegate* delegate,
                                             int nIndex = -1);
                                             
        /*!
         * @fn	void UnregisterJSDialogHandlerDelegate(JSDialogHandlerDelegate* delegate);
         *
         * @brief	反注册JS消息框处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterJSDialogHandlerDelegate(JSDialogHandlerDelegate* delegate);
        
        /*!
         * @fn	bool RegisterKeyboardHandlerDelegate(
         * 		KeyboardHandlerDelegate* delegate,
         * 		int nIndex = -1);
         *
         * @brief	注册键盘事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         * @param	nIndex				(Optional) the index.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterKeyboardHandlerDelegate(KeyboardHandlerDelegate* delegate,
                                             int nIndex = -1);
                                             
        /*!
         * @fn	void UnregisterKeyboardHandlerDelegate(KeyboardHandlerDelegate* delegate);
         *
         * @brief	反注册键盘事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterKeyboardHandlerDelegate(KeyboardHandlerDelegate* delegate);
        
        /*!
         * @fn	bool RegisterLifeSpanHandlerDelegate(
         * 		LifeSpanHandlerDelegate* delegate,
         * 		int nIndex = -1);
         *
         * @brief	注册浏览器窗口事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         * @param	nIndex				(Optional) the index.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterLifeSpanHandlerDelegate(LifeSpanHandlerDelegate* delegate,
                                             int nIndex = -1);
                                             
        /*!
         * @fn	void UnregisterLifeSpanHandlerDelegate(LifeSpanHandlerDelegate* delegate);
         *
         * @brief	反注册浏览器窗口事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterLifeSpanHandlerDelegate(LifeSpanHandlerDelegate* delegate);
        
        /*!
         * @fn	bool RegisterLoadHandlerDelegate(
         * 		LoadHandlerDelegate* delegate,
         * 		int nIndex = -1);
         *
         * @brief	注册页面加载事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         * @param	nIndex				(Optional) the index.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterLoadHandlerDelegate(LoadHandlerDelegate* delegate,
                                         int nIndex = -1);
                                         
        /*!
         * @fn	void UnregisterLoadHandlerDelegate(LoadHandlerDelegate* delegate);
         *
         * @brief	反注册页面加载事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterLoadHandlerDelegate(LoadHandlerDelegate* delegate);
        
        /*!
         * @fn	bool RegisterRenderHandlerDelegate(
         * 		RenderHandlerDelegate* delegate,
         * 		int nIndex = -1);
         *
         * @brief	注册渲染事件处理函数
         * 			离屏模式下可用.
         *
         * @param 	delegate	If non-null, the delegate.
         * @param	nIndex				(Optional) the index.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterRenderHandlerDelegate(RenderHandlerDelegate* delegate,
                                           int nIndex = -1);
                                           
        /*!
         * @fn	void UnregisterRenderHandlerDelegate(RenderHandlerDelegate* delegate);
         *
         * @brief	反注册渲染事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterRenderHandlerDelegate(RenderHandlerDelegate* delegate);
        
        /*!
         * @fn	bool RegisterRequestHandlerDelegate(
         * 		RequestHandlerDelegate* delegate,
         * 		int nIndex = -1);
         *
         * @brief	注册Request事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         * @param	nIndex				(Optional) the index.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterRequestHandlerDelegate(RequestHandlerDelegate* delegate,
                                            int nIndex = -1);
                                            
        /*!
         * @fn	void UnregisterRequestHandlerDelegate(RequestHandlerDelegate* delegate);
         *
         * @brief	反注册Request事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterRequestHandlerDelegate(RequestHandlerDelegate* delegate);
        
        /*!
         * @fn	bool RegisterMesssageHandlerDelegate(
         * 		MessageHandlerDelegate* delegate,
         * 		bool first = false);
         *
         * @brief	注册消息事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         * @param	first				(Optional) true to first.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterMesssageHandlerDelegate(MessageHandlerDelegate* delegate,
                                             bool first = false);
                                             
        /*!
         * @fn	void UnregisterMesssageHandlerDelegate(MessageHandlerDelegate* delegate);
         *
         * @brief	反注册消息事件处理函数.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterMesssageHandlerDelegate(MessageHandlerDelegate* delegate);
        
        /*!
         * @fn	void CopyDelegates();
         *
         * @brief	 复制DummyClientHandler中注册的Handler.
         */
        
        void CopyDelegates();
        
        /*!
         * @fn	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
         *
         * @brief	Executes the after created action.
         *
         * @param	browser	The browser.
         */
        
        virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
        
        /*!
         * @fn	virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
         *
         * @brief	Executes the close operation.
         *
         * @param	browser	The browser.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
        
        /*!
         * @fn	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
         *
         * @brief	Executes the before close action.
         *
         * @param	browser	The browser.
         */
        
        virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
        
        /*!
         * @fn	bool ProcessSyncMessage(int nID, IPCMessage::SmartType msg);
         *
         * @brief	处理同步消息.
         *
         * @param	nID	The identifier.
         * @param	msg	The message.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool ProcessSyncMessage(int nID, IPCMessage::SmartType msg);
        
        
        IMPLEMENT_REFCOUNTING(ClientHandler);
        
        
    public:
        static BrowserTempInfo GetBrowserInfoFromTempByID(int nBrowserID);
        static void RemoveBrowserFromTempByID(int nBrowserID);
        static void AddExchanger(int nBrowserID);
    private:
    
        /*! @brief	菜单事件处理类. */
        CefRefPtr<ContextMenuHandler> m_pContextMenuHandler;
        /*! @brief	对话框事件处理类. */
        CefRefPtr<DialogHandler> m_pDialogHandler;
        /*! @brief	显示事件处理类. */
        CefRefPtr<DisplayHandler> m_pDisplayHandler;
        /*! @brief	下载事件处理类. */
        CefRefPtr<DownloadHandler> m_pDownloadHandler;
        /*! @brief	拖拽事件处理类. */
        CefRefPtr<DragHandler> m_pDragHandler;
        /*! @brief	焦点事件处理类. */
        CefRefPtr<FocusHandler> m_pFocusHandler;
        /*! @brief	位置事件处理类. */
        CefRefPtr<GeolocationHandler> m_pGeolocationHandler;
        /*! @brief	JS消息框处理类. */
        CefRefPtr<JSDialogHandler> m_pJSDialogHandler;
        /*! @brief	键盘事件处理类. */
        CefRefPtr<KeyboardHandler> m_pKeyboardHandler;
        /*! @brief	页面窗口事件处理类. */
        CefRefPtr<LifeSpanHandler> m_pLifeSpanHandler;
        /*! @brief	页面加载事件处理类. */
        CefRefPtr<LoadHandler> m_pLoadHandler;
        /*! @brief	页面渲染事件处理类. */
        CefRefPtr<RenderHandler> m_pRenderHandler;
        /*! @brief	页面请求事件处理类. */
        CefRefPtr<RequestHandler> m_pRequestHandler;
        /*! @brief	消息事件处理类. */
        CefRefPtr<MessageRouterBrowserSide> m_pMessageRouter;
        /*! @brief	消息处理类集合. */
        static MessageHandlerSet m_MessageSet;
        /*! @brief	The temporary browser map. */
        static std::unordered_map<int, BrowserTempInfo > m_oTempBrowserMap;
    };
}

#endif // AMO_CLIENTHANDLER_H__
