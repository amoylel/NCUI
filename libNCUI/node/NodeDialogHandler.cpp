#include "stdafx.h"
#include "node/NodeDialogHandler.h"
#include "node/NodeHandlerHelper.h"

namespace amo {

    NodeDialogHandler::NodeDialogHandler(std::shared_ptr<NodeHandlerHelper>& pHelper)
        : m_pHelper(pHelper) {
        
    }
    
#if CHROME_VERSION_BUILD >= 2272
    bool NodeDialogHandler::OnFileDialog(CefRefPtr<CefBrowser> browser,
                                         CefDialogHandler::FileDialogMode mode,
                                         const CefString& title,
                                         const CefString& default_file_path,
                                         const std::vector<CefString>& accept_filters,
                                         int selected_accept_filter,
                                         CefRefPtr<CefFileDialogCallback> callback) {
        return false;
    }
#else
    bool NodeDialogHandler::OnFileDialog(CefRefPtr<CefBrowser> browser,
                                         CefDialogHandler::FileDialogMode mode,
                                         const CefString& title,
                                         const CefString& default_file_name,
                                         const std::vector<CefString>& accept_types,
                                         CefRefPtr<CefFileDialogCallback> callback) {
        return false;
    }
#endif
}

