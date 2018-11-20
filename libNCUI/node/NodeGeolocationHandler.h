// Created by amoylel on 07/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODEGEOLOCATIONHANDLER_H__
#define AMO_NODEGEOLOCATIONHANDLER_H__

#include <memory>
#include "handler/GeolocationHandler.h"


namespace amo {
    class NodeHandlerHelper;
    class NodeGeolocationHandler : public GeolocationHandlerDelegate {
    public:
        NodeGeolocationHandler(std::shared_ptr<NodeHandlerHelper>& pHelper);
        
        
        
        
#if CHROME_VERSION_BUILD >= 2272
        virtual bool OnRequestGeolocationPermission(CefRefPtr<CefBrowser> browser,
                const CefString& requesting_url,
                int request_id,
                CefRefPtr<CefGeolocationCallback> callback) override;
                
#else
        virtual void OnRequestGeolocationPermission(CefRefPtr<CefBrowser> browser,
                const CefString& requesting_url,
                int request_id,
                CefRefPtr<CefGeolocationCallback> callback);
                
#endif
                
#if CHROME_VERSION_BUILD >= 2623
        virtual void OnCancelGeolocationPermission(CefRefPtr<CefBrowser> browser,
                int request_id) override;
#else
                
        virtual void OnCancelGeolocationPermission(CefRefPtr<CefBrowser> browser,
                const CefString& requesting_url,
                int request_id) override;
#endif
                
                
    private:
        std::shared_ptr<NodeHandlerHelper>& m_pHelper;
    };
}

#endif // AMO_NODEGEOLOCATIONHANDLER_H__


