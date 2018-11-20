// Created by amoylel on 07/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODEDOWNLOADHANDLER_H__
#define AMO_NODEDOWNLOADHANDLER_H__





#include <memory>
#include "handler/DownloadHandler.h"


namespace amo {
    class NodeHandlerHelper;
    class NodeDownloadHandler : public DownloadHandlerDelegate {
    public:
        NodeDownloadHandler(std::shared_ptr<NodeHandlerHelper>& pHelper);
        
        
        
        
        
        virtual void OnBeforeDownload(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString& suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback) override;
        
        
        virtual void OnDownloadUpdated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback) override;
        
    private:
        std::shared_ptr<NodeHandlerHelper>& m_pHelper;
    };
}



#endif // AMO_NODEDOWNLOADHANDLER_H__

