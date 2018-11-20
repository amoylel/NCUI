#include "stdafx.h"
#include "ui/win/menu/MenuElementUI.h"
#include "ui/win/menu/MenuWindow.h"
#include "settings/MenuWindowSettings.h"

namespace amo {
    /////////////////////////////////////////////////////////////////////////////////////
    //
    
    // MenuElementUI
    const TCHAR* const kMenuElementUIClassName = _T("MenuElement");
    const TCHAR* const kMenuElementUIInterfaceName = _T("MenuElement");
    
    extern 	ContextMenuObserver s_context_menu_observer;
    MenuElementUI::MenuElementUI(std::shared_ptr<MenuWindowSettings> pSettings)
        : m_pMenuWindowSettings(pSettings)
        ,  m_pWindow(NULL) {
        m_cxyFixed.cy = m_pMenuWindowSettings->lineHeight;
        
        m_bMouseChildEnabled = true;
    }
    
    MenuElementUI::~MenuElementUI()
    {}
    
    LPCTSTR MenuElementUI::GetClass() const {
        return kMenuElementUIClassName;
    }
    
    LPVOID MenuElementUI::GetInterface(LPCTSTR pstrName) {
        if (_tcsicmp(pstrName, kMenuElementUIInterfaceName) == 0) {
            return static_cast<MenuElementUI*>(this);
        }
        
        return CListContainerElementUI::GetInterface(pstrName);
    }
    
    bool MenuElementUI::DoPaint(HDC hDC,
                                const RECT& rcPaint,
                                CControlUI* pStopControl) {
                                
        RECT rcTemp = { 0 };
        
        if (!::IntersectRect(&rcTemp, &rcPaint, &m_rcItem)) {
            return true;
        }
        
        CRenderClip clip;
        CRenderClip::GenerateClip(hDC, rcTemp, clip);
        MenuElementUI::DrawItemBk(hDC, m_rcItem);
        DrawItemText(hDC, m_rcItem);
        
        if (m_items.GetSize() > 0) {
            RECT rc = m_rcItem;
            rc.left += m_rcInset.left;
            rc.top += m_rcInset.top;
            rc.right -= m_rcInset.right;
            rc.bottom -= m_rcInset.bottom;
            
            if (m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible()) {
                rc.right -= m_pVerticalScrollBar->GetFixedWidth();
            }
            
            if (m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible()) {
                rc.bottom -= m_pHorizontalScrollBar->GetFixedHeight();
            }
            
            if (!::IntersectRect(&rcTemp, &rcPaint, &rc)) {
                for (int it = 0; it < m_items.GetSize(); it++) {
                    CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
                    
                    if (pControl == pStopControl) {
                        return false;
                    }
                    
                    if (!pControl->IsVisible()) {
                        continue;
                    }
                    
                    if (pControl->GetInterface(kMenuElementUIInterfaceName) != NULL) {
                        continue;
                    }
                    
                    if (!::IntersectRect(&rcTemp,
                                         &rcPaint,
                                         &pControl->GetPos())) {
                        continue;
                    }
                    
                    if (pControl->IsFloat()) {
                        if (!::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos())) {
                            continue;
                        }
                        
                        if (!pControl->Paint(hDC, rcPaint, pStopControl)) {
                            return false;
                        }
                    }
                }
            } else {
                CRenderClip childClip;
                CRenderClip::GenerateClip(hDC, rcTemp, childClip);
                
                for (int it = 0; it < m_items.GetSize(); it++) {
                    CControlUI* pControl = static_cast<CControlUI*>(m_items[it]);
                    
                    if (pControl == pStopControl) {
                        return false;
                    }
                    
                    if (!pControl->IsVisible()) {
                        continue;
                    }
                    
                    if (pControl->GetInterface(kMenuElementUIInterfaceName) != NULL) {
                        continue;
                    }
                    
                    if (!::IntersectRect(&rcTemp, &rcPaint, &pControl->GetPos())) {
                        continue;
                    }
                    
                    if (pControl->IsFloat()) {
                        if (!::IntersectRect(&rcTemp, &m_rcItem, &pControl->GetPos())) {
                            continue;
                        }
                        
                        CRenderClip::UseOldClipBegin(hDC, childClip);
                        
                        if (!pControl->Paint(hDC, rcPaint, pStopControl)) {
                            return false;
                        }
                        
                        CRenderClip::UseOldClipEnd(hDC, childClip);
                    } else {
                        if (!::IntersectRect(&rcTemp, &rc, &pControl->GetPos())) {
                            continue;
                        }
                        
                        if (!pControl->Paint(hDC, rcPaint, pStopControl)) {
                            return false;
                        }
                    }
                }
            }
        }
        
        if (m_pVerticalScrollBar != NULL) {
            if (m_pVerticalScrollBar == pStopControl) {
                return false;
            }
            
            if (m_pVerticalScrollBar->IsVisible()) {
                if (::IntersectRect(&rcTemp, &rcPaint, &m_pVerticalScrollBar->GetPos())) {
                    if (!m_pVerticalScrollBar->Paint(hDC, rcPaint, pStopControl)) {
                        return false;
                    }
                }
            }
        }
        
        if (m_pHorizontalScrollBar != NULL) {
            if (m_pHorizontalScrollBar == pStopControl) {
                return false;
            }
            
            if (m_pHorizontalScrollBar->IsVisible()) {
                if (::IntersectRect(&rcTemp, &rcPaint, &m_pHorizontalScrollBar->GetPos())) {
                    if (!m_pHorizontalScrollBar->Paint(hDC, rcPaint, pStopControl)) {
                        return false;
                    }
                }
            }
        }
        
        return true;
    }
    
    void MenuElementUI::DrawItemText(HDC hDC, const RECT& rcItem) {
        if (m_sText.IsEmpty()) {
            return;
        }
        
        if (m_pOwner == NULL) {
            return;
        }
        
        TListInfoUI* pInfo = m_pOwner->GetListInfo();
        DWORD iTextColor = pInfo->dwTextColor;
        
        if ((m_uButtonState & UISTATE_HOT) != 0) {
            iTextColor = pInfo->dwHotTextColor;
        }
        
        if (IsSelected()) {
            iTextColor = pInfo->dwSelectedTextColor;
        }
        
        if (!IsEnabled()) {
            iTextColor = pInfo->dwDisabledTextColor;
        }
        
        int nLinks = 0;
        RECT rcText = rcItem;
        rcText.left += pInfo->rcTextPadding.left;
        rcText.right -= pInfo->rcTextPadding.right;
        rcText.top += pInfo->rcTextPadding.top;
        rcText.bottom -= pInfo->rcTextPadding.bottom;
        
        
        if (pInfo->bShowHtml) {
            CRenderEngine::DrawHtmlText(hDC, m_pManager, rcText, m_sText, iTextColor,
                                        NULL, NULL, nLinks, pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);
        } else {
        
            CRenderEngine::DrawText(hDC, m_pManager, rcText, m_sText, iTextColor,
                                    pInfo->nFont, DT_SINGLELINE | pInfo->uTextStyle);
        }
    }
    
    
    SIZE MenuElementUI::EstimateSize(SIZE szAvailable) {
        SIZE cXY = { 0 };
        
        for (int it = 0; it < GetCount(); it++) {
            CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
            
            if (!pControl->IsVisible()) {
                continue;
            }
            
            SIZE sz = pControl->EstimateSize(szAvailable);
            cXY.cy += sz.cy;
            
            if (cXY.cx < sz.cx) {
                cXY.cx = sz.cx;
            }
        }
        
        if (cXY.cy == 0) {
            TListInfoUI* pInfo = m_pOwner->GetListInfo();
            
            DWORD iTextColor = pInfo->dwTextColor;
            
            if ((m_uButtonState & UISTATE_HOT) != 0) {
                iTextColor = pInfo->dwHotTextColor;
            }
            
            if (IsSelected()) {
                iTextColor = pInfo->dwSelectedTextColor;
            }
            
            if (!IsEnabled()) {
                iTextColor = pInfo->dwDisabledTextColor;
            }
            
            RECT rcText = { 0, 0, (std::max)(szAvailable.cx, m_cxyFixed.cx), 9999 };
            rcText.left += pInfo->rcTextPadding.left;
            rcText.right -= pInfo->rcTextPadding.right;
            
            if (pInfo->bShowHtml) {
                int nLinks = 0;
                CRenderEngine::DrawHtmlText(m_pManager->GetPaintDC(), m_pManager, rcText, m_sText, iTextColor, NULL, NULL, nLinks, pInfo->nFont, DT_CALCRECT | pInfo->uTextStyle & ~DT_RIGHT & ~DT_CENTER);
            } else {
                CRenderEngine::DrawText(m_pManager->GetPaintDC(), m_pManager, rcText, m_sText, iTextColor, pInfo->nFont, DT_CALCRECT | pInfo->uTextStyle & ~DT_RIGHT & ~DT_CENTER);
            }
            
            cXY.cx = rcText.right - rcText.left + pInfo->rcTextPadding.left + pInfo->rcTextPadding.right + /*20*/ 2;
            cXY.cy = rcText.bottom - rcText.top + pInfo->rcTextPadding.top + pInfo->rcTextPadding.bottom;
        }
        
        if (m_cxyFixed.cy != 0) {
            cXY.cy = m_cxyFixed.cy;
        }
        
        return cXY;
    }
    
    void MenuElementUI::DoEvent(TEventUI& event) {
        if (event.Type == UIEVENT_MOUSEENTER) {
            CListContainerElementUI::DoEvent(event);
            
            if (m_pWindow) {
                return;
            }
            
            bool hasSubMenu = false;
            
            for (int i = 0; i < GetCount(); ++i) {
                if (GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL) {
                    (static_cast<MenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetVisible(true);
                    (static_cast<MenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(true);
                    
                    hasSubMenu = true;
                }
            }
            
            if (hasSubMenu) {
                m_pOwner->SelectItem(GetIndex(), true);
                CreateMenuWnd();
            } else {
                CDuiString str = GetText();
                ContextMenuParam param;
                param.hWnd = m_pManager->GetPaintWindow();
                param.wParam = 2;
                s_context_menu_observer.RBroadcast(param);
                m_pOwner->SelectItem(GetIndex(), true);
            }
        }
        
        if (event.Type == UIEVENT_BUTTONDOWN) {
            if (IsEnabled()) {
                CListContainerElementUI::DoEvent(event);
                
                if (m_pWindow) {
                    return;
                }
                
                bool hasSubMenu = false;
                
                for (int i = 0; i < GetCount(); ++i) {
                    if (GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL) {
                        (static_cast<MenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetVisible(true);
                        (static_cast<MenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(true);
                        
                        hasSubMenu = true;
                    }
                }
                
                if (hasSubMenu) {
                    CreateMenuWnd();
                } else {
                    ContextMenuParam param;
                    param.hWnd = m_pManager->GetPaintWindow();
                    param.wParam = 3;
                    param.data = this;
                    param.text = GetText();
                    s_context_menu_observer.RBroadcast(param); // 通知点击事件
                    
                    param.wParam = 1;
                    s_context_menu_observer.RBroadcast(param);
                }
            }
            
            return;
        }
        
        CListContainerElementUI::DoEvent(event);
    }
    
    bool MenuElementUI::Activate() {
        if (CListContainerElementUI::Activate() && m_bSelected) {
            if (m_pWindow) {
                return true;
            }
            
            bool hasSubMenu = false;
            
            for (int i = 0; i < GetCount(); ++i) {
                if (GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL) {
                    (static_cast<MenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetVisible(true);
                    (static_cast<MenuElementUI*>(GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(true);
                    
                    hasSubMenu = true;
                }
            }
            
            if (hasSubMenu) {
                CreateMenuWnd();
            } else {
                CDuiString sd = GetText();
                ContextMenuParam param;
                param.hWnd = m_pManager->GetPaintWindow();
                param.wParam = 1;
                param.data = this;
                s_context_menu_observer.RBroadcast(param);
            }
            
            return true;
        }
        
        return false;
    }
    
    MenuWindow* MenuElementUI::GetMenuWnd() {
        return m_pWindow;
    }
    
    void MenuElementUI::SetMenuWnd(MenuWindow* wnd) {
        m_pWindow = wnd;
    }
    
    UINT MenuElementUI::GetButtonState() const {
        return m_uButtonState;
    }
    
    void MenuElementUI::SetButtonState(UINT state) {
        m_uButtonState = state;
    }
    
    void MenuElementUI::CreateMenuWnd() {
        if (m_pWindow) {
            return;
        }
        
        m_pWindow = new MenuWindow(m_pMenuWindowSettings, m_pManager->GetPaintWindow());
        ASSERT(m_pWindow);
        
        ContextMenuParam param;
        param.hWnd = m_pManager->GetPaintWindow();
        param.wParam = 2;
        s_context_menu_observer.RBroadcast(param);
        
        
        m_pWindow->Init(static_cast<MenuElementUI*>(this), CDuiPoint());
        
    }
    
    DuiLib::CDuiString MenuElementUI::GetAttribute(LPCTSTR pstrName) {
        if (_tcscmp(pstrName, _T("id")) == 0) {
            return m_strID;
        } else if (_tcscmp(pstrName, _T("action")) == 0) {
            return m_strAction;
        } else if (_tcscmp(pstrName, _T("string")) == 0) {
            return m_strText;
        }
        
        return __super::GetAttribute(pstrName);
    }
    
    void MenuElementUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) {
        if (_tcscmp(pstrName, _T("id")) == 0) {
            m_strID = pstrValue;
        } else if (_tcscmp(pstrName, _T("lineHeight")) == 0) {
            if (pstrValue == NULL) {
                return;
            }
            
            LPTSTR pstr = NULL;
            m_cxyFixed.cy = _tcstol(pstrValue, &pstr, 10);
            ASSERT(pstr);
        } else if (_tcscmp(pstrName, _T("action")) == 0) {
            m_strAction = pstrValue;
        } else if (_tcscmp(pstrName, _T("string")) == 0) {
            m_strText = pstrValue;
        }
        
        return __super::SetAttribute(pstrName, pstrValue);
    }
    
}

