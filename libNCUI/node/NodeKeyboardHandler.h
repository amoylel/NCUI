// Created by amoylel on 07/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODEKEYBOARDHANDLER_H__
#define AMO_NODEKEYBOARDHANDLER_H__


#include <memory>
#include "handler/KeyboardHandler.h"


namespace amo {
    class NodeHandlerHelper;
    class NodeKeyboardHandler : public KeyboardHandlerDelegate {
    public:
        NodeKeyboardHandler(std::shared_ptr<NodeHandlerHelper>& pHelper);
        
        
        virtual bool OnPreKeyEvent(CefRefPtr<CefBrowser> browser,
                                   const CefKeyEvent& event,
                                   CefEventHandle os_event,
                                   bool* is_keyboard_shortcut) override;
                                   
                                   
        virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser,
                                const CefKeyEvent& event,
                                CefEventHandle os_event) override;
                                
    private:
        std::shared_ptr<NodeHandlerHelper>& m_pHelper;
    };
}


#endif // AMO_NODEKEYBOARDHANDLER_H__




