#include "stdafx.h"
#include "node/NodeKeyboardHandler.h"
#include "node/NodeHandlerHelper.h"

namespace amo {

    NodeKeyboardHandler::NodeKeyboardHandler(std::shared_ptr<NodeHandlerHelper>& pHelper)
        : m_pHelper(pHelper) {
        
    }
    
    
    bool NodeKeyboardHandler::OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                                            const CefKeyEvent& event,
                                            CefEventHandle os_event,
                                            bool* is_keyboard_shortcut) {
        return false;
    }
    
    bool NodeKeyboardHandler::OnKeyEvent(CefRefPtr<CefBrowser> browser,
                                         const CefKeyEvent& event,
                                         CefEventHandle os_event) {
        return false;
    }
    
}

