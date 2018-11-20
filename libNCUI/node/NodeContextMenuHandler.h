
// Created by amoylel on 07/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODECONTEXTMENUHANDLER_H__
#define AMO_NODECONTEXTMENUHANDLER_H__

#include <memory>
#include "handler/ContextMenuHandler.h"



namespace amo {
    class NodeHandlerHelper;
    class NodeContextMenuHandler : public ContextMenuHandlerDelegate {
    public:
        NodeContextMenuHandler(std::shared_ptr<NodeHandlerHelper>& pHelper);
        
        
        virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         CefRefPtr<CefContextMenuParams> params,
                                         CefRefPtr<CefMenuModel> model);
                                         
                                         
        virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefRefPtr<CefContextMenuParams> params,
                                          int command_id,
                                          CefContextMenuHandler::EventFlags event_flags);
                                          
                                          
        virtual void OnContextMenuDismissed(CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame);
                                            
    private:
        std::shared_ptr<NodeHandlerHelper>& m_pHelper;
    };
}

#endif // AMO_NODECONTEXTMENUHANDLER_H__
