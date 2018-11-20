// Created by amoylel on 7/02/2017.
// Copyright (c) 2017 amoylel. All rights reserved.

#ifndef AMO_MESSAGEROUTERBROWSERSIDE_H__
#define AMO_MESSAGEROUTERBROWSERSIDE_H__

#include <mutex>

#include "handler/CefHeader.hpp"

namespace amo {


#if CHROME_VERSION_BUILD >= 2987
    class MessageRouterBrowserSide : public CefBaseRefCounted {
#else
    class MessageRouterBrowserSide : public CefBase {
#endif
    
    
    public:
        typedef CefMessageRouterBrowserSide::Handler Handler;
    public:
        MessageRouterBrowserSide();
        
        MessageRouterBrowserSide(const CefMessageRouterConfig& config);
        
        virtual ~MessageRouterBrowserSide() {}
        
        virtual bool AddHandler(Handler* handler, bool first);
        
        virtual bool RemoveHandler(Handler* handler);
        
        virtual void CancelPending(CefRefPtr<CefBrowser> browser, Handler* handler);
        
        virtual int GetPendingCount(CefRefPtr<CefBrowser> browser, Handler* handler);
        
        virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser);
        
        virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser);
        
        virtual void OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame);
                                    
        virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                              CefProcessId source_process,
                                              CefRefPtr<CefProcessMessage> message);
                                              
        IMPLEMENT_REFCOUNTING(MessageRouterBrowserSide);
    private:
        std::recursive_mutex m_mutex;
        CefRefPtr<CefMessageRouterBrowserSide> m_pMessageRouter;
    };
    
}
#endif // AMO_MESSAGEROUTERBROWSERSIDE_H__
