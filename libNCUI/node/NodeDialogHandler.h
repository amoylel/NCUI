
// Created by amoylel on 07/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODEDIALOGHANDLER_H__
#define AMO_NODEDIALOGHANDLER_H__

#include <memory>
#include "handler/DialogHandler.h"

namespace amo {
    class NodeHandlerHelper;
    class NodeDialogHandler : public DialogHandlerDelegate {
    public:
        NodeDialogHandler(std::shared_ptr<NodeHandlerHelper>& pHelper);
        
#if CHROME_VERSION_BUILD >= 2272
        
        virtual bool OnFileDialog(CefRefPtr<CefBrowser> browser,
                                  CefDialogHandler::FileDialogMode mode,
                                  const CefString& title,
                                  const CefString& default_file_path,
                                  const std::vector<CefString>& accept_filters,
                                  int selected_accept_filter,
                                  CefRefPtr<CefFileDialogCallback> callback) override;
                                  
#else
        virtual bool OnFileDialog(CefRefPtr<CefBrowser> browser,
                                  CefDialogHandler::FileDialogMode mode,
                                  const CefString& title,
                                  const CefString& default_file_name,
                                  const std::vector<CefString>& accept_types,
                                  CefRefPtr<CefFileDialogCallback> callback) override;
#endif
    private:
        std::shared_ptr<NodeHandlerHelper>& m_pHelper;
    };
}

#endif // AMO_NODEDIALOGHANDLER_H__

