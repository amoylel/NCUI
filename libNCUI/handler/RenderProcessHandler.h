// Created by amoylel on 6/22/2017.
// Copyright (c) 2017 amoylel. All rights reserved.

#ifndef AMO_RENDERPROCESSHANDLER_H__
#define AMO_RENDERPROCESSHANDLER_H__

#include <memory>
#include <map>

#include <amo/string.hpp>
#include "handler/CefHeader.hpp"
#include "handler/HandlerDelegate.hpp"
#include "ipc/Any.hpp"
#include "handler/BrowserManager.hpp"

namespace amo {
    class V8ExtentionHandler;
    class BrowserWindowSettings;
    class RenderProcessHandlerDelegate {
    public:
        RenderProcessHandlerDelegate() {}
        
        virtual ~RenderProcessHandlerDelegate() {}
        
        virtual void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) {}
        
        virtual void OnWebKitInitialized() {}
        
        virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) {}
        
        virtual void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) {}
        
        virtual void OnNodeCefCreated(CefCommandLine& args) {}
        
        virtual CefRefPtr<CefLoadHandler> GetLoadHandler() {
            return NULL;
        }
        
        virtual bool OnBeforeNavigation(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefRequest> request,
                                        CefRenderProcessHandler::NavigationType navigation_type,
                                        bool is_redirect) {
            return false;
        }
        
        virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefRefPtr<CefV8Context> context) {}
                                      
        virtual void OnContextReleased(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       CefRefPtr<CefV8Context> context) {}
                                       
        virtual void OnUncaughtException(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         CefRefPtr<CefV8Context> context,
                                         CefRefPtr<CefV8Exception> exception,
                                         CefRefPtr<CefV8StackTrace> stackTrace) {}
                                         
        virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefRefPtr<CefDOMNode> node) {}
                                          
        virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                              CefProcessId source_process,
                                              CefRefPtr<CefProcessMessage> message) {
            return false;
        }
        
    };
    
    
    class RenderProcessHandler : public CefRenderProcessHandler
        , public HandlerDelegate < RenderProcessHandlerDelegate >
        , public BrowserManager < PID_RENDERER > {
        
    public:
    
        /*!
         * @fn	RenderProcessHandler::RenderProcessHandler();
         *
         * @brief	Default constructor.
         */
        
        RenderProcessHandler();
        
        /*!
         * @fn	virtual RenderProcessHandler::~RenderProcessHandler();
         *
         * @brief	Destructor.
         */
        
        virtual ~RenderProcessHandler();
        
        /*!
         * @fn	virtual void RenderProcessHandler::OnNodeCefCreated(CefCommandLine& args);
         *
         * @brief	Executes the node cef created action.
         *
         * @param 	args	The arguments.
         */
        
        virtual void OnNodeCefCreated(CefCommandLine& args);
        
        /*!
         * @fn	virtual void RenderProcessHandler::OnRenderThreadCreated(
         * 		CefRefPtr<CefListValue> extra_info) override;
         *
         * @brief	Executes the render thread created action.
         *
         * @param	extra_info	Information describing the extra.
         */
        
        virtual void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) override;
        
        /*!
         * @fn	virtual void RenderProcessHandler::OnWebKitInitialized() override;
         *
         * @brief	Executes the web kit initialized action.
         */
        
        virtual void OnWebKitInitialized() override;
        
        /*!
         * @fn	virtual void RenderProcessHandler::OnBrowserCreated(
         * 		CefRefPtr<CefBrowser> browser) override;
         *
         * @brief	Executes the browser created action.
         *
         * @param	browser	The browser.
         */
        
        virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;
        
        /*!
         * @fn	virtual void RenderProcessHandler::OnBrowserDestroyed(
         * 		CefRefPtr<CefBrowser> browser) override;
         *
         * @brief	Executes the browser destroyed action.
         *
         * @param	browser	The browser.
         */
        
        virtual void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override;
        
        /*!
         * @fn	virtual CefRefPtr<CefLoadHandler> RenderProcessHandler::GetLoadHandler() override;
         *
         * @brief	Handler, called when the get load.
         *
         * @return	The load handler.
         */
        
        virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override;
        
        /*!
         * @fn	virtual bool RenderProcessHandler::OnBeforeNavigation(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		CefRefPtr<CefRequest> request,
         * 		NavigationType navigation_type,
         * 		bool is_redirect) override;
         *
         * @brief	Executes the before navigation action.
         *
         * @param	browser		   	The browser.
         * @param	frame		   	The frame.
         * @param	request		   	The request.
         * @param	navigation_type	Type of the navigation.
         * @param	is_redirect	   	true if this object is redirect.
         *
         * @return	true if it succeeds, false if it fails.
         */
#if CEF_VERSION_REGION(0,3325)
        virtual bool OnBeforeNavigation(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefRequest> request,
                                        NavigationType navigation_type,
                                        bool is_redirect) override;
#endif
                                        
                                        
        /*!
         * @fn	virtual void RenderProcessHandler::OnContextCreated(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		CefRefPtr<CefV8Context> context) override;
         *
         * @brief	Executes the context created action.
         *
         * @param	browser	The browser.
         * @param	frame  	The frame.
         * @param	context	The context.
         */
        
        virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefRefPtr<CefV8Context> context) override;
                                      
        /*!
         * @fn	virtual void RenderProcessHandler::OnContextReleased(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		CefRefPtr<CefV8Context> context) override;
         *
         * @brief	Executes the context released action.
         *
         * @param	browser	The browser.
         * @param	frame  	The frame.
         * @param	context	The context.
         */
        
        virtual void OnContextReleased(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       CefRefPtr<CefV8Context> context) override;
                                       
        /*!
         * @fn	virtual void RenderProcessHandler::OnUncaughtException(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		CefRefPtr<CefV8Context> context,
         * 		CefRefPtr<CefV8Exception> exception,
         * 		CefRefPtr<CefV8StackTrace> stackTrace) override;
         *
         * @brief	Executes the uncaught exception action.
         *
         * @param	browser   	The browser.
         * @param	frame	  	The frame.
         * @param	context   	The context.
         * @param	exception 	The exception.
         * @param	stackTrace	The stack trace.
         */
        
        virtual void OnUncaughtException(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         CefRefPtr<CefV8Context> context,
                                         CefRefPtr<CefV8Exception> exception,
                                         CefRefPtr<CefV8StackTrace> stackTrace) override;
                                         
        /*!
         * @fn	virtual void RenderProcessHandler::OnFocusedNodeChanged(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefFrame> frame,
         * 		CefRefPtr<CefDOMNode> node) override;
         *
         * @brief	Executes the focused node changed action.
         *
         * @param	browser	The browser.
         * @param	frame  	The frame.
         * @param	node   	The node.
         */
        
        virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefRefPtr<CefDOMNode> node) override;
                                          
        /*!
         * @fn	virtual bool RenderProcessHandler::OnProcessMessageReceived(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefProcessId source_process,
         * 		CefRefPtr<CefProcessMessage> message) override;
         *
         * @brief	Executes the process message received action.
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
         * @fn	RenderProcessHandler::IMPLEMENT_REFCOUNTING(RenderProcessHandler);
         *
         * @brief	Copy constructor.
         *
         * @param	parameter1	The first parameter.
         */
        
        IMPLEMENT_REFCOUNTING(RenderProcessHandler);
        
    private:
    
        /*!
         * @fn	bool RenderProcessHandler::ProcessSyncMessage(
         * 		int nID,
         * 		IPCMessage::SmartType msg);
         *
         * @brief	处理来自主进程的同步调用JS函数.
         *
         * @param	nID	The identifier.
         * @param	msg	The message.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool ProcessSyncMessage(int nID, IPCMessage::SmartType msg);
        
        /*!
         * @fn	bool RenderProcessHandler::IsEditableElement(CefRefPtr<CefDOMNode> node);
         *
         * @brief	Query if 'node' is editable element.
         *
         * @param	node	The node.
         *
         * @return	true if editable element, false if not.
         */
        
        bool IsEditableElement(CefRefPtr<CefDOMNode> node);
        
        /*!
         * @fn	void RenderProcessHandler::createPipe(
         * 		int nBrowserID,
         * 		std::shared_ptr<ProcessExchanger> pExchanger);
         *
         * @brief	Creates a pipe.
         *
         * @param	nBrowserID	Identifier for the browser.
         * @param	pExchanger	The exchanger.
         */
        
        void createPipe(int nBrowserID, std::shared_ptr<ProcessExchanger> pExchanger);
        
        /*!
         * @fn	void RenderProcessHandler::afterCreatePipe(
         * 		CefRefPtr<CefBrowser> browser,
         * 		std::shared_ptr<ProcessExchanger> pExchanger,
         * 		Any val);
         *
         * @brief	After create pipe.
         *
         * @param	browser   	The browser.
         * @param	pExchanger	The exchanger.
         * @param	val		  	The value.
         */
        
        void afterCreatePipe(CefRefPtr<CefBrowser> browser,
                             std::shared_ptr<ProcessExchanger> pExchanger,
                             Any val);
                             
        /*!
         * @fn	void RenderProcessHandler::RenderThreadActivityDetector();
         *
         * @brief	Renders the thread activity detector.
         */
        
        void RenderThreadActivityDetector();
        
        
    public:
        /*! @brief	The message router. */
        CefRefPtr<CefMessageRouterRendererSide> m_pMessageRouter;
        
        /*! @brief	The v 8 extension hander. */
        CefRefPtr<V8ExtentionHandler> m_pV8ExtensionHander;
        
        std::map<int, std::shared_ptr<BrowserWindowSettings> > m_browserSettingsMap;
    };
}



#endif // AMO_RENDERPROCESSHANDLER_H__