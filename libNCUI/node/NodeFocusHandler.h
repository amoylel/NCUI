// Created by amoylel on 07/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODEFOCUSHANDLER_H__
#define AMO_NODEFOCUSHANDLER_H__



#include <memory>
#include "handler/FocusHandler.h"


namespace amo {
    class NodeHandlerHelper;
    class NodeFocusHandler : public FocusHandlerDelegate {
    public:
        NodeFocusHandler(std::shared_ptr<NodeHandlerHelper>& pHelper);
        
        virtual void OnTakeFocus(CefRefPtr<CefBrowser> browser, bool next) override;
        
        virtual bool OnSetFocus(CefRefPtr<CefBrowser> browser, CefFocusHandler::FocusSource source) override;
        
        virtual void OnGotFocus(CefRefPtr<CefBrowser> browser) override;
        
    private:
        std::shared_ptr<NodeHandlerHelper>& m_pHelper;
    };
}


#endif // AMO_NODEFOCUSHANDLER_H__

