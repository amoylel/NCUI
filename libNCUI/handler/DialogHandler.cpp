#include "stdAfx.h"

#include "handler/DialogHandler.h"

#include <amo/logger.hpp>



namespace amo {
#if CHROME_VERSION_BUILD >= 2272
    bool DialogHandler::OnFileDialog(CefRefPtr<CefBrowser> browser,
                                     FileDialogMode mode,
                                     const CefString& title,
                                     const CefString& default_file_path,
                                     const std::vector<CefString>& accept_filters,
                                     int selected_accept_filter,
                                     CefRefPtr<CefFileDialogCallback> callback) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();

        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnFileDialog(browser, mode, title, default_file_path,
                                           accept_filters, selected_accept_filter, callback);

            if (bHandled) return true;
        }

        return CefDialogHandler::OnFileDialog(browser, mode, title, default_file_path,
                                              accept_filters, selected_accept_filter, callback);
    }
#else
    bool DialogHandler::OnFileDialog(CefRefPtr<CefBrowser> browser,
                                     FileDialogMode mode,
                                     const CefString& title,
                                     const CefString& default_file_name,
                                     const std::vector<CefString>& accept_types,
                                     CefRefPtr<CefFileDialogCallback> callback) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();

        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnFileDialog(browser, mode, title, default_file_name,
                                           accept_types, callback);

            if (bHandled) return true;
        }

        return CefDialogHandler::OnFileDialog(browser, mode, title, default_file_name,
                                              accept_types, callback);
    }
#endif

}