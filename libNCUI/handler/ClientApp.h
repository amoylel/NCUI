// Created by amoylel on 6/22/2017.
// Copyright (c) 2017 amoylel. All rights reserved.

#ifndef AMO_CLIENTAPP_H__
#define AMO_CLIENTAPP_H__

#include <vector>

#include "handler/CefHeader.hpp"

#include "handler/HandlerDelegate.hpp"
#include "handler/BrowserProcessHandler.h"


namespace amo {


    class RenderProcessHandler;
    class BrowserProcessHandler;
    class ResourceBundleHandler;
    
    class RenderProcessHandlerDelegate;
    class BrowserProcessHandlerDelegate;
    class ResourceBundleHandlerDelegate;
    
    class ClientApp : public CefApp
        , public HandlerDelegate<ClientApp>
        , public BrowserProcessHandlerDelegate {
    public:
    
        /*!
         * @fn	static void ClientApp::RegisterCustomSchemes(
         * 		CefRefPtr<CefSchemeRegistrar> registrar,
         * 		std::vector<CefString>& cookiable_schemes);
         *
         * @brief	注册自定义协议
         * 			B.
         *
         * @param	registrar				 	The registrar.
         * @param 	cookiable_schemes	The cookiable schemes.
         */
        
#if CHROME_VERSION_BUILD >= 2987
        static void RegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar,
                                          std::vector<CefString>& cookiable_schemes);
#else
        static void RegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar,
                                          std::vector<CefString>& cookiable_schemes);
                                          
#endif
                                          
                                          
                                          
        /*!
         * @fn	static bool ClientApp::RegisterCustomSchemeFactory(
         * 		const CefString& scheme_name,
         * 		const CefString& domain_name,
         * 		CefRefPtr<CefSchemeHandlerFactory> factory);
         *
         * @brief	注册自定义协议处理类
         * 			B.
         *
         * @param	scheme_name	Name of the scheme.
         * @param	domain_name	Name of the domain.
         * @param	factory	   	The factory.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        static bool RegisterCustomSchemeFactory(const CefString& scheme_name,
                                                const CefString& domain_name,
                                                CefRefPtr<CefSchemeHandlerFactory> factory);
                                                
    public:
        ClientApp();
        
        virtual ~ClientApp();
        
        /*!
         * @fn	virtual void ClientApp::OnBeforeCommandLineProcessing(
         * 		const CefString& process_type,
         * 		CefRefPtr<CefCommandLine> command_line);
         *
         * @brief	在进程创建前可通过此函数查看命令行参数或修改命令行参数
         * 			 B R.
         *
         * @param	process_type	Type of the process.
         * @param	command_line	The command line.
         */
        
        virtual void OnBeforeCommandLineProcessing(const CefString& process_type,
                CefRefPtr<CefCommandLine> command_line) override;
                
        /*!
         * @fn	virtual void ClientApp::OnRegisterCustomSchemes(
         * 		CefRefPtr<CefSchemeRegistrar> registrar);
         *
         * @brief	此函数提供注册自定义协议的机会，可以在此回调函数中注册自定义协议，
        //			该方法只能在进程中的主线程中调用
        //			B R.
         *
         * @param	registrar	The registrar.
         */
#if CHROME_VERSION_BUILD  >= 3029
        virtual void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
        override;
#elif CHROME_VERSION_BUILD  >= 2987
        virtual void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
        override;
#else
        virtual void OnRegisterCustomSchemes(CefRefPtr<CefSchemeRegistrar> registrar)
        override;
#endif
        
        
        /*!
         * @fn	virtual CefRefPtr<CefResourceBundleHandler> ClientApp::GetResourceBundleHandler();
         *
         * @brief	获取资源处理句柄
         * 			如果CefSettings.pack_loading_disabled 为true, 那么此函数不能返回NULL
         * 			B R.
         *
         * @return	The resource bundle handler.
         */
        
        virtual CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() override;
        
        /*!
         * @fn	virtual CefRefPtr<CefBrowserProcessHandler> ClientApp::GetBrowserProcessHandler();
         *
         * @brief	返回浏览器主进程处理句柄
         * 			可以在主进程中的任何线程中调用
         * 			B.
         *
         * @return	The browser process handler.
         */
        
        virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;
        
        /*!
         * @fn	virtual CefRefPtr<CefRenderProcessHandler> ClientApp::GetRenderProcessHandler();
         *
         * @brief	返回渲染进程处理句柄 只能在渲染进程中的主线程中调用
         * 			R MAIN.
         *
         * @return	The render process handler.
         */
        
        virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;
        
        /*!
         * @fn	bool ClientApp::RegisterRenderProcessHandlerDelegate(
         * 		RenderProcessHandlerDelegate* delegate);
         *
         * @brief	注册 CefRenderProcessHandler 回调处理函数
         * 			尽量在创建子进程前调用，不然只能在渲染进程中寻找其他位置调用
         * 			R MAIN.
         *
         * @param 	delegate	If non-null, the delegate.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterRenderProcessHandlerDelegate(RenderProcessHandlerDelegate*
                delegate);
                
        /*!
         * @fn	void ClientApp::UnregisterRenderProcessHandlerDelegate(
         * 		RenderProcessHandlerDelegate* delegate);
         *
         * @brief	反注册 CefRenderProcessHandler 回调处理函数
         * 			R.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterRenderProcessHandlerDelegate(RenderProcessHandlerDelegate*
                delegate);
                
        /*!
         * @fn	bool ClientApp::RegisterBrowserProcessHandlerDelegate(
         * 		BrowserProcessHandlerDelegate* delegate);
         *
         * @brief	注册 CefBrowserProcessHandler 回调处理函数
         * 			B.
         *
         * @param 	delegate	If non-null, the delegate.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool RegisterBrowserProcessHandlerDelegate(BrowserProcessHandlerDelegate*
                delegate);
                
        /*!
         * @fn	void ClientApp::UnregisterBrowserProcessHandlerDelegate(
         * 		BrowserProcessHandlerDelegate* delegate);
         *
         * @brief	注册 CefBrowserProcessHandler 回调处理函数
         * 			B.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        void UnregisterBrowserProcessHandlerDelegate(BrowserProcessHandlerDelegate*
                delegate);
                
    public:
    
    
    private:
    
        /*!
         * @fn	virtual void ClientApp::OnContextInitialized();
         *
         * @brief	Executes the context initialized action.
         */
        
        virtual void OnContextInitialized()  override;
        
        IMPLEMENT_REFCOUNTING(ClientApp);
        
    private:
    
        /*! @brief	渲染进程处理程序. */
        CefRefPtr<RenderProcessHandler> m_pRenderProcessHandler;
        /*! @brief	主进程处理程序. */
        CefRefPtr<BrowserProcessHandler> m_pBrowserProcessHandler;
        /*! @brief	资源处理程序. */
        CefRefPtr<ResourceBundleHandler> m_pResourceBundleHandler;
        /*! @brief	允许Cookie的协议. */
        std::vector<CefString> m_CookieableSchemes;
        
        
    };
}
#endif // AMO_CLIENTAPP_H__
