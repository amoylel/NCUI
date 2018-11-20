// Created by amoylel on 06/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_V8EXTENTIONHANDLER_H__
#define AMO_V8EXTENTIONHANDLER_H__





#include <memory>

#include "handler/CefHeader.hpp"

namespace amo {

    class JsV8Handler;
    class ClassTransfer;
    class UtilityV8Handler;
    
    template <class T1, class T2>
    struct pair_hash {
    
        std::size_t operator()(const std::pair<T1, T2>& p) {
            auto h1 = std::hash < T1 > {}(p.first);
            auto h2 = std::hash < T2 > {}(p.second);
            return h1 ^ h2;
        }
    };
    
    /*!
     * @class	V8ExtentionHandler
     *
     * @brief	扩展调用类.
     */
    
    class V8ExtentionHandler : public CefV8Handler {
    public:
        V8ExtentionHandler();
        ~V8ExtentionHandler();
        // 添加类
        //void addClass(const std::string& strClass, FunctionWrapperMgr& mgr);
        
        /*!
         * @fn	void V8ExtentionHandler::registerExternalTransfer(
         * 		int nBrowserID,
         * 		std::shared_ptr<ClassTransfer> pTransfer);
         *
         * @brief	Registers the external transfer.
         *
         * @param	nBrowserID	Identifier for the browser.
         * @param	pTransfer 	The transfer.
         */
        
        void registerExternalTransfer(int nBrowserID,
                                      std::shared_ptr<ClassTransfer> pTransfer);
                                      
        /*!
         * @fn	bool V8ExtentionHandler::loadExternalTransfer(
         * 		const std::string& strClass,
         * 		CefRefPtr<CefBrowser> browser);
         *
         * @brief	Loads external transfer.
         *
         * @param	strClass	The class.
         * @param	browser 	The browser.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool loadExternalTransfer(const std::string& strClass,
                                  CefRefPtr<CefBrowser> browser);
                                  
        /*!
         * @fn	CefRefPtr<JsV8Handler> V8ExtentionHandler::fromClass(
         * 		const std::string& strClass,
         * 		CefRefPtr<CefBrowser> browser);
         *
         * @brief	从类中创建Handler.
         *
         * @param	strClass	The class.
         * @param	browser 	The browser.
         *
         * @return	A CefRefPtr&lt;JsV8Handler&gt;
         */
        
        CefRefPtr<JsV8Handler> fromClass(const std::string& strClass, CefRefPtr<CefBrowser> browser);
        
        /*!
         * @fn	CefRefPtr<CefV8Value> V8ExtentionHandler::includeFromRendererThread(
         * 		const std::string& module);
         *
         * @brief	Include from renderer thread.
         *
         * @param	module	The module.
         *
         * @return	A CefRefPtr&lt;CefV8Value&gt;
         */
        
        CefRefPtr<CefV8Value> includeFromRendererThread(const std::string& module);
        
        /*!
         * @fn	CefRefPtr<CefV8Value> V8ExtentionHandler::includeFromBrowserThread(
         * 		const std::string& module);
         *
         * @brief	Include from browser thread.
         *
         * @param	module	The module.
         *
         * @return	A CefRefPtr&lt;CefV8Value&gt;
         */
        
        CefRefPtr<CefV8Value> includeFromBrowserThread(const std::string& module);
        
        /*!
         * @fn	virtual bool V8ExtentionHandler::Execute(
         * 		const CefString& name,
         * 		CefRefPtr<CefV8Value> object,
         * 		const CefV8ValueList& arguments,
         * 		CefRefPtr<CefV8Value>& retval,
         * 		CefString& exception) override;
         *
         * @brief	Executes.
         *
         * @param	name			 	The name.
         * @param	object			 	The object.
         * @param	arguments		 	The arguments.
         * @param 	retval   	The retval.
         * @param 	exception	The exception.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool Execute(const CefString& name,
                             CefRefPtr<CefV8Value> object,
                             const CefV8ValueList& arguments,
                             CefRefPtr<CefV8Value>& retval,
                             CefString& exception) override;
                             
        /*!
         * @fn	bool V8ExtentionHandler::includeDepends(const std::vector<std::string>& vec);
         *
         * @brief	Include depends.
         *
         * @param	vec	The vector.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool includeDepends(const std::vector<std::string>& vec);
        
        /*!
         * @fn	void V8ExtentionHandler::OnContextReleased(int64 nFrameID);
         *
         * @brief	Executes the context released action.
         *
         * @param	nFrameID	Identifier for the frame.
         */
        
        void OnContextReleased(int64 nFrameID);
        
        /*!
         * @fn	void V8ExtentionHandler::OnBrowserDestory(int nBrowserID);
         *
         * @brief	Executes the browser destory action.
         *
         * @param	nBrowserID	Identifier for the browser.
         */
        
        void OnBrowserDestory(int nBrowserID);
        
        /**
         * @fn	void V8ExtentionHandler::OnProcessDestory();
         *
         * @brief	进程要退出之前调用此函数，删除所有第三方Transfer.
         */
        
        void OnProcessDestory();
        /**
         * @fn	void V8ExtentionHandler::triggerEventOnRendererThread(IPCMessage::SmartType msg);
         *
         * @brief	在主渲染线程中执行函数，当前线程可能非渲染线程.
         *
         * @param	msg	The message.
         */
        
        void triggerEventOnRendererThread(IPCMessage::SmartType msg);
        
        /**
         * @fn	void V8ExtentionHandler::triggerEventOnRendererThreadImpl(IPCMessage::SmartType msg);
         *
         * @brief	在主渲染线程中执行函数实现函数，此线程为渲染线程.
         *
         * @param	msg	The message.
         */
        
        void triggerEventOnRendererThreadImpl(IPCMessage::SmartType msg);
        IMPLEMENT_REFCOUNTING(V8ExtentionHandler);
        
        
        
        std::set<std::string> getNonGlobalModules();
    private:
        //std::unordered_map<std::pair<std::string, int>,
        //    CefRefPtr<JsV8Handler>, pair_hash<std::string, int> > m_oClassHandler;
        //std::unordered_map<std::string, FunctionWrapperMgr > m_oClassSet;							//!< 保存所给类的所有成员函数
        std::unordered_map<std::string, std::shared_ptr<ClassTransfer> > m_oClassTransferMap;
        
        /*! @brief	Set the module belongs to. */
        std::set<std::pair<std::string, int64_t> > m_oModuleSet;
        
        CefRefPtr<UtilityV8Handler> m_pUtilityV8Handler;
        
        /** @brief	加载所有renderer_modules中的transfer. */
        
        std::set<int> m_oRegisteredSet;
        
        
    };
    
}
#endif // AMO_V8EXTENTIONHANDLER_H__