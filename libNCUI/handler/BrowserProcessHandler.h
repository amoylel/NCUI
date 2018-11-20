// Created by amoylel on 06/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_BROWSERPROCESSHANDLER_H__
#define AMO_BROWSERPROCESSHANDLER_H__

#include "handler/CefHeader.hpp"
#include "handler/HandlerDelegate.hpp"

namespace amo {
    class BrowserProcessHandlerDelegate {
    public:
        BrowserProcessHandlerDelegate() {}
        
        virtual ~BrowserProcessHandlerDelegate() {}
        
        virtual void OnContextInitialized() {}
        
        virtual void OnBeforeChildProcessLaunch(
            CefRefPtr<CefCommandLine> command_line) {}
            
        virtual void OnRenderProcessThreadCreated(
            CefRefPtr<CefListValue> extra_info) {}
            
#if CHROME_VERSION_BUILD >= 2272
        virtual CefRefPtr<CefPrintHandler> GetPrintHandler() {
            return NULL;
        }
#endif
        
    };
    
    class BrowserProcessHandler : public CefBrowserProcessHandler
        , public HandlerDelegate < BrowserProcessHandlerDelegate > {
        
    public:
        BrowserProcessHandler() {}
        
        virtual ~BrowserProcessHandler() {}
        
        /*!
         * @fn	virtual void BrowserProcessHandler::OnContextInitialized();
         *
         * @brief	Executes the context initialized action.
         */
        
        virtual void OnContextInitialized() override;
        
        /*!
         * @fn	virtual void BrowserProcessHandler::OnBeforeChildProcessLaunch(
         * 		CefRefPtr<CefCommandLine> command_line);
         *
         * @brief	Executes the before child process launch action.
         *
         * @param	command_line	The command line.
         */
        
        virtual void OnBeforeChildProcessLaunch(
            CefRefPtr<CefCommandLine> command_line) override;
            
        /*!
         * @fn	virtual void BrowserProcessHandler::OnRenderProcessThreadCreated(
         * 		CefRefPtr<CefListValue> extra_info);
         *
         * @brief	Executes the render process thread created action.
         *
         * @param	extra_info	Information describing the extra.
         */
        
        virtual void OnRenderProcessThreadCreated(
            CefRefPtr<CefListValue> extra_info) override;
            
            
#if CHROME_VERSION_BUILD >= 2272
        virtual CefRefPtr<CefPrintHandler> GetPrintHandler() override;
#endif
        
        
        
        IMPLEMENT_REFCOUNTING(BrowserProcessHandler);
        
        
        
        
    };
    
}

#endif // AMO_BROWSERPROCESSHANDLER_H__
