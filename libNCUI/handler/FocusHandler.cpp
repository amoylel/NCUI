#include "stdAfx.h"
#include "handler/FocusHandler.h"
#include <amo/logger.hpp>


namespace amo {

    void FocusHandler::OnGotFocus(CefRefPtr<CefBrowser> browser) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();

        for (; it != m_Delegates.end(); ++it)
            (*it)->OnGotFocus(browser);
    }

    bool FocusHandler::OnSetFocus(CefRefPtr<CefBrowser> browser,
                                  FocusSource source) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        bool bHandled = false;
        DelegateSet::iterator it = m_Delegates.begin();

        for (; it != m_Delegates.end(); ++it) {
            bHandled = (*it)->OnSetFocus(browser, source);

            if (bHandled) return true;
        }

        return CefFocusHandler::OnSetFocus(browser, source);
    }

    void FocusHandler::OnTakeFocus(CefRefPtr<CefBrowser> browser,
                                   bool next) {
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();

        for (; it != m_Delegates.end(); ++it)
            (*it)->OnTakeFocus(browser, next);
    }

}