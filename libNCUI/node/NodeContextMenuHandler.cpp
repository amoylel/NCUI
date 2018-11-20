#include "stdafx.h"
#include "node/NodeContextMenuHandler.h"
#include "node/NodeHandlerHelper.h"

namespace amo {

    NodeContextMenuHandler::NodeContextMenuHandler(std::shared_ptr<NodeHandlerHelper>& pHelper)
        : m_pHelper(pHelper) {
        
    }
    
    void NodeContextMenuHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefContextMenuParams> params,
            CefRefPtr<CefMenuModel> model) {
        return;
    }
    
    bool NodeContextMenuHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefContextMenuParams> params,
            int command_id,
            CefContextMenuHandler::EventFlags event_flags) {
        return false;
    }
    
    void NodeContextMenuHandler::OnContextMenuDismissed(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame) {
        return;
    }
    
}

