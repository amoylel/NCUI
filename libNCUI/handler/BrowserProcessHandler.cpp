#include "stdAfx.h"
#include "handler/BrowserProcessHandler.h"



namespace amo {
    void BrowserProcessHandler::OnRenderProcessThreadCreated(
        CefRefPtr<CefListValue> extra_info) {
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnRenderProcessThreadCreated(extra_info);
        }
    }
    
    void BrowserProcessHandler::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine>
            command_line) {
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnBeforeChildProcessLaunch(command_line);
        }
    }
    
    void BrowserProcessHandler::OnContextInitialized() {
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnContextInitialized();
        }
    }
    
#if CHROME_VERSION_BUILD >= 2272
    CefRefPtr<CefPrintHandler> BrowserProcessHandler::GetPrintHandler() {
        return CefBrowserProcessHandler::GetPrintHandler();
    }
    
    
    
#endif
    
}
