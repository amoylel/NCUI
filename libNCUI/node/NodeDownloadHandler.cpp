#include "stdafx.h"
#include "node/NodeDownloadHandler.h"
#include "node/NodeHandlerHelper.h"

namespace amo {

    NodeDownloadHandler::NodeDownloadHandler(std::shared_ptr<NodeHandlerHelper>& pHelper)
        : m_pHelper(pHelper) {
        
    }
    
    
    void NodeDownloadHandler::OnBeforeDownload(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefDownloadItem> download_item,
            const CefString& suggested_name,
            CefRefPtr<CefBeforeDownloadCallback> callback) {
            
    }
    
    void NodeDownloadHandler::OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefDownloadItem> download_item,
            CefRefPtr<CefDownloadItemCallback> callback) {
            
    }
    
}

