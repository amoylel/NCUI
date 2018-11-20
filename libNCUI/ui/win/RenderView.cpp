#include "stdafx.h"
#include "ui/win/RenderView.h"

#include <amo/logger.hpp>

#include "settings/BrowserWindowSettings.h"
#include "handler/ClientHandler.h"
#include "ui/win/BrowserWindowSkin.h"

namespace amo {

    RenderView::RenderView(std::shared_ptr<BrowserWindowSettings>
                           pBrowserSettings) {
        m_oBrowserSettings = pBrowserSettings;
    }
    
    void RenderView::OnFinalMessage(HWND hWnd) {
        if (m_pClientHandler) {
            m_pClientHandler->UnregisterLifeSpanHandlerDelegate(this);
        }
        
    }
    
    LRESULT RenderView::ResponseDefaultKeyEvent(WPARAM wParam) {
        if (wParam == VK_RETURN) {
            return FALSE;
        } else if (wParam == VK_ESCAPE) {
            Close();
            return TRUE;
        }
        
        return FALSE;
    }
    
    DuiLib::CDuiString RenderView::GetSkinFolder() {
        return _T("");
    }
    
    DuiLib::CDuiString RenderView::GetSkinFile() {
        return amo::u8string(skinRenderView, true).format(
                   m_oBrowserSettings->settings).to_unicode().c_str();
        //return amo::u8string(skinRenderView, true).to_unicode().c_str();
    }
    
    UINT RenderView::GetClassStyle() const {
        return CS_DBLCLKS;
    }
    
    
    
    LRESULT RenderView::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
                               BOOL& bHandled) {
        RECT rect;
        ::GetClientRect(m_hWnd, &rect);
        
        //////最小化时不移动浏览器窗口，不然还原时页面渲染时会出问题
        if (m_pBrowser/* &&  !::IsIconic(::GetParent(m_hWnd))*/)
            ::SetWindowPos(m_pBrowser->GetHost()->GetWindowHandle(),
                           NULL,
                           rect.left,
                           rect.top,
                           rect.right - rect.left,
                           rect.bottom - rect.top,
                           NULL);
                           
        SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
        
        if (!::IsIconic(*this)
                && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
            CDuiRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++;
            rcWnd.bottom++;
            HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left,
                                             rcWnd.top,
                                             rcWnd.right,
                                             rcWnd.bottom,
                                             szRoundCorner.cx,
                                             szRoundCorner.cy);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }
        
#endif
        bHandled = FALSE;
        return 0;
    }
    
    
    LRESULT RenderView::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                            BOOL& bHandled) {
        LRESULT lRes = FALSE;
        
        switch (uMsg) {
        case WM_SETFOCUS:
            if (m_pBrowser) {
                m_pBrowser->GetHost()->SetFocus(true);
            }
            
            break;
            
        default:
            break;
        }
        
        if (bHandled) {
            return lRes;
        }
        
        return WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
    }
    
    LPCTSTR RenderView::GetWindowClassName(void) const	{
        return _T("RenderView");
    }
    
    void RenderView::InitWindow() {
    
        CHorizontalLayoutUI* pRenderLayout = static_cast<CHorizontalLayoutUI*>(
                m_PaintManager.FindControl(_T("reanderLayout")));
                
        if (!pRenderLayout && !m_oBrowserSettings->offscreen) {
            pRenderLayout->SetBkColor(0xffffffff);
        }
        
        RECT rect;
        ::GetClientRect(m_hWnd, &rect);
        
        CefWindowInfo windowInfo;
        windowInfo.SetAsChild(m_hWnd, rect);
        
        if (m_pClientHandler) {
            m_pClientHandler->RegisterLifeSpanHandlerDelegate(this);
        }
        
        std::string redirectUrl = UrlResourceHandlerFactory::getInstance()->redirectUrl(
                                      m_oBrowserSettings->url);
                                      
        if (!redirectUrl.empty()) {
            m_oBrowserSettings->url = redirectUrl;
        }
        
        
        CefBrowserHost::CreateBrowser(windowInfo,
                                      m_pClientHandler.get(),
                                      m_oBrowserSettings->url,
                                      *m_oBrowserSettings,
                                      NULL);
    }
    
    LRESULT RenderView::OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled) {
        if (m_pClientHandler) {
            m_pClientHandler->UnregisterLifeSpanHandlerDelegate(this);
        }
        
        m_pClientHandler = NULL;
        return TRUE;
    }
    
    void RenderView::setClientHandler(CefRefPtr<amo::ClientHandler> handler) {
        m_pClientHandler = handler;
    }
    
    CefRefPtr<CefBrowser> RenderView::getBrowser() {
        return m_pBrowser;
    }
    
    void RenderView::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
        if (m_hWnd == ::GetParent(browser->GetHost()->GetWindowHandle())) {
            m_pBrowser = browser;
            RECT rect;
            ::GetClientRect(m_hWnd, &rect);
            ::SetWindowPos(m_pBrowser->GetHost()->GetWindowHandle(),
                           NULL,
                           rect.left,
                           rect.top,
                           rect.right - rect.left,
                           rect.bottom - rect.top,
                           NULL);
            CEF_REQUIRE_UI_THREAD();
            
        }
        
        return;
    }
    
    
    void RenderView::onFocusedNodeChanged(IPCMessage::SmartType msg) {
        return;
    }
    
    
    void RenderView::updateCaretPos(std::shared_ptr<Gdiplus::Bitmap> image) {
        return;
    }
    
    void RenderView::insertBitmap(std::shared_ptr<PaintResource> image) {
        return;
    }
    
    
    void RenderView::setTooltip(const amo::u8string& str) {
        CHorizontalLayoutUI* pRenderLayout = static_cast<CHorizontalLayoutUI*>(
                m_PaintManager.FindControl(_T("reanderLayout")));
                
        if (pRenderLayout != NULL) {
            pRenderLayout->SetToolTip(str.to_wide().c_str());
        }
    }
    
    void RenderView::needUpdate() {
        CHorizontalLayoutUI* pRenderLayout = static_cast<CHorizontalLayoutUI*>(
                m_PaintManager.FindControl(_T("reanderLayout")));
                
        if (pRenderLayout && m_oBrowserSettings->offscreen) {
            pRenderLayout->Invalidate();
        }
    }
    
    bool RenderView::DoPaintOnMemDC(HDC hDC, RECT rc) {
        return true;
    }
    
    RenderView::~RenderView() {
    }
    
}


