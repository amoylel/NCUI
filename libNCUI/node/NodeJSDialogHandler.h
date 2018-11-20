// Created by amoylel on 07/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODEJSDIALOGHANDLER_H__
#define AMO_NODEJSDIALOGHANDLER_H__



#include <memory>
#include "handler/JSDialogHandler.h"


namespace amo {
    class NodeHandlerHelper;
    class NodeJSDialogHandler : public JSDialogHandlerDelegate {
    public:
        NodeJSDialogHandler(std::shared_ptr<NodeHandlerHelper>& pHelper);
        
#if CHROME_VERSION_BUILD >= 2704
        virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
                                const CefString& origin_url,
                                CefJSDialogHandler::JSDialogType dialog_type,
                                const CefString& message_text,
                                const CefString& default_prompt_text,
                                CefRefPtr<CefJSDialogCallback> callback,
                                bool& suppress_message) override;
#else
        virtual bool OnJSDialog(CefRefPtr<CefBrowser> browser,
                                const CefString& origin_url,
                                const CefString& accept_lang,
                                CefJSDialogHandler::JSDialogType dialog_type,
                                const CefString& message_text,
                                const CefString& default_prompt_text,
                                CefRefPtr<CefJSDialogCallback> callback,
                                bool& suppress_message) override;
#endif
                                
                                
                                
        virtual bool OnBeforeUnloadDialog(CefRefPtr<CefBrowser> browser,
                                          const CefString& message_text,
                                          bool is_reload,
                                          CefRefPtr<CefJSDialogCallback> callback) override;
                                          
                                          
        virtual void OnResetDialogState(CefRefPtr<CefBrowser> browser) override;
        
        
        virtual void OnDialogClosed(CefRefPtr<CefBrowser> browser) override;
        
    private:
        std::shared_ptr<NodeHandlerHelper>& m_pHelper;
    };
}


#endif // AMO_NODEJSDIALOGHANDLER_H__



