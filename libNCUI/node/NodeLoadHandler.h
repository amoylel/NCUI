// Created by amoylel on 07/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODELOADHANDLER_H__
#define AMO_NODELOADHANDLER_H__
#include <memory>

#include "handler/LoadHandler.h"


namespace amo {
    class NodeHandlerHelper;
    class NodeLoadHandler : public LoadHandlerDelegate {
    public:
        NodeLoadHandler(std::shared_ptr<NodeHandlerHelper>& pHelper);
        
        
        virtual void OnLoadingStateChange(CefRefPtr<CefBrowser> browser,
                                          bool isLoading,
                                          bool canGoBack,
                                          bool canGoForward);
                                          
                                          
        virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame);
                                 
                                 
        virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               int httpStatusCode);
                               
                               
        virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 CefLoadHandler::ErrorCode errorCode,
                                 const CefString& errorText,
                                 const CefString& failedUrl);
                                 
    private:
        std::shared_ptr<NodeHandlerHelper>& m_pHelper;
    };
}

#endif // AMO_NODELOADHANDLER_H__
