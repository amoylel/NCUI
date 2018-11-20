#include "stdAfx.h"
#include "handler/MessageRouterBrowserSide.h"



namespace amo {
    bool MessageRouterBrowserSide::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
            CefProcessId source_process,
            CefRefPtr<CefProcessMessage> message) {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        return m_pMessageRouter->OnProcessMessageReceived(browser, source_process, message);
    }

    void MessageRouterBrowserSide::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame) {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        return m_pMessageRouter->OnBeforeBrowse(browser, frame);
    }

    void MessageRouterBrowserSide::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser) {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        return m_pMessageRouter->OnRenderProcessTerminated(browser);
    }

    void MessageRouterBrowserSide::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        return m_pMessageRouter->OnBeforeClose(browser);
    }

    int MessageRouterBrowserSide::GetPendingCount(CefRefPtr<CefBrowser> browser,
            Handler* handler) {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        return m_pMessageRouter->GetPendingCount(browser, handler);
    }

    void MessageRouterBrowserSide::CancelPending(CefRefPtr<CefBrowser> browser,
            Handler* handler) {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        return m_pMessageRouter->CancelPending(browser, handler);
    }

    bool MessageRouterBrowserSide::RemoveHandler(Handler* handler) {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        return m_pMessageRouter->RemoveHandler(handler);
    }

    bool MessageRouterBrowserSide::AddHandler(Handler* handler,
            bool first) {
        std::unique_lock<std::recursive_mutex> lock(m_mutex);
        return m_pMessageRouter->AddHandler(handler, first);
    }

    MessageRouterBrowserSide::MessageRouterBrowserSide() {
        CefMessageRouterConfig config;
        m_pMessageRouter = CefMessageRouterBrowserSide::Create(config);
    }

    MessageRouterBrowserSide::MessageRouterBrowserSide(const CefMessageRouterConfig& config) {
        m_pMessageRouter = CefMessageRouterBrowserSide::Create(config);
    }
}