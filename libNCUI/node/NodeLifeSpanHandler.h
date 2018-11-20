// Created by amoylel on 07/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODELIFESPANHANDLER_H__
#define AMO_NODELIFESPANHANDLER_H__

#include "handler/LifeSpanHandler.h"

namespace amo {
    class NodeHandlerHelper;
    class NodeLifeSpanHandler : public LifeSpanHandlerDelegate {
    public:
        NodeLifeSpanHandler(std::shared_ptr<NodeHandlerHelper>& pHelper);
        
#if CHROME_VERSION_BUILD >= 2357
        virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   const CefString& target_url,
                                   const CefString& target_frame_name,
                                   CefLifeSpanHandler::WindowOpenDisposition target_disposition,
                                   bool user_gesture,
                                   const CefPopupFeatures& popupFeatures,
                                   CefWindowInfo& windowInfo,
                                   CefRefPtr<CefClient>& client,
                                   CefBrowserSettings& settings,
                                   bool* no_javascript_access) override;
                                   
#else
        virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   const CefString& target_url,
                                   const CefString& target_frame_name,
                                   const CefPopupFeatures& popupFeatures,
                                   CefWindowInfo& windowInfo,
                                   CefRefPtr<CefClient>& client,
                                   CefBrowserSettings& settings,
                                   bool* no_javascript_access) override;
#endif
                                   
        virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
        
#if CHROME_VERSION_BUILD >= 2704
        
#else
        virtual bool RunModal(CefRefPtr<CefBrowser> browser) override;
#endif
        
        virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
        virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
        
        
        
    private:
        std::shared_ptr<NodeHandlerHelper>& m_pHelper;
    };
}

#endif // AMO_NODELIFESPANHANDLER_H__
