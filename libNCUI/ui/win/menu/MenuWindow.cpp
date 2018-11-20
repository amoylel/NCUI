#include "stdAfx.h"
#include "ui/win/menu/MenuWindow.h"
#include <algorithm>

#include "ui/win/BrowserWindowSkin.h"
#include <amo/format.hpp>
#include <amo/json.hpp>
#include "ui/win/menu/MenuElementUI.h"
#include "ui/win/menu/MenuUI.h"
#include "settings/MenuWindowSettings.h"
#include "ui/win/Imm32Manager.h"

namespace amo {


    ContextMenuObserver s_context_menu_observer;
    enum MenuAlignment {
        eMenuAlignment_Left = 1 << 1,
        eMenuAlignment_Top = 1 << 2,
        eMenuAlignment_Right = 1 << 3,
        eMenuAlignment_Bottom = 1 << 4,
    };
    
    MenuWindow::MenuWindow(std::shared_ptr<MenuWindowSettings> pBrowserSettings,
                           HWND hParent)
        : LayeredWindow(pBrowserSettings)
        , m_hParent(hParent)
        , m_pOwner(NULL)
        , m_pLayout(NULL)
        , m_pMenuSettings(pBrowserSettings) {
    }
    
    BOOL MenuWindow::Receive(ContextMenuParam param) {
        switch (param.wParam) {
        case 1:
            Close();
            break;
            
        case 2: {
            HWND hParent = GetParent(m_hWnd);
            
            while (hParent != NULL) {
                if (hParent == param.hWnd) {
                    Close();
                    break;
                }
                
                hParent = GetParent(hParent);
            }
        }
        break;
        
        default:
            break;
        }
        
        return TRUE;
    }
    
    void MenuWindow::Init(MenuElementUI* pOwner, POINT point) {
        m_BasedPoint = point;
        RECT workRect = { 0 };
        ::SystemParametersInfo(SPI_GETWORKAREA, 0, &workRect, SPIF_SENDCHANGE);
        
        m_pOwner = pOwner;
        m_pLayout = NULL;
        
        s_context_menu_observer.AddReceiver(this);
        
        Create((m_pOwner == NULL) ? m_hParent :
               m_pOwner->GetManager()->GetPaintWindow(), NULL,
               WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST, CDuiRect());
        // HACK: Don't deselect the parent's caption
        HWND hWndParent = m_hWnd;
        
        while (::GetParent(hWndParent) != NULL) {
            hWndParent = ::GetParent(hWndParent);
        }
        
        ::ShowWindow(m_hWnd, SW_SHOW);
#if defined(WIN32) && !defined(UNDER_CE)
        ::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
#endif
        
        
        
        LayeredWindow::InitWindow();
        m_pLayout->SetItemTextColor(0xff333333);
        m_pLayout->SetHotItemBkColor(0xffd8d8d8);
        m_pLayout->SetSelectedItemBkColor(0xffd8d8d8);
        DWORD bkcolor = (DWORD)m_pMenuSettings->windowColor;
        m_pLayout->SetBkColor(bkcolor);
        
        m_pLayout->SetBorderSize(1);
        m_pLayout->SetBorderColor(0xffb0b0b0);
        m_pLayout->SetInset({ 1, 1, 1, 1 });
        //m_pLayout->SetBorderRound({ m_pMenuSettings->roundcorner,
        //                            m_pMenuSettings->roundcorner
        //                          });
        //
        
        if (m_pMenuSettings->transparent) {
        
            setLayered(true);
            
        }
        
        
        // 关闭菜单窗口的输入法
        m_pIMM32.reset(new IMM32Manager(m_hWnd));
        m_pIMM32->disableIME();
        
    }
    
    LPCTSTR MenuWindow::GetWindowClassName() const {
        return _T("MenuWnd");
    }
    
    DuiLib::CControlUI* MenuWindow::CreateControl(LPCTSTR pstrClass) {
        if (_tcsicmp(pstrClass, kMenuUIInterfaceName) == 0) {
            return new CMenuUI();
        } else if (_tcsicmp(pstrClass, kMenuElementUIInterfaceName) == 0) {
            MenuElementUI* element =  new MenuElementUI(m_pMenuSettings);
            return element;
        }
        
        return NULL;
    }
    
    void MenuWindow::OnFinalMessage(HWND hWnd) {
    
        RemoveObserver();
        
        if (m_pOwner != NULL) {
            for (int i = 0; i < m_pOwner->GetCount(); i++) {
                if (static_cast<MenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(
                                                    kMenuElementUIInterfaceName)) != NULL) {
                    (static_cast<MenuElementUI*>(m_pOwner->GetItemAt(i)))->SetOwner(
                        m_pOwner->GetParent());
                    (static_cast<MenuElementUI*>(m_pOwner->GetItemAt(i)))->SetVisible(false);
                    (static_cast<MenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(
                                                     kMenuElementUIInterfaceName)))->SetInternVisible(false);
                }
            }
            
            m_pOwner->m_pWindow = NULL;
            m_pOwner->m_uButtonState &= ~UISTATE_PUSHED;
            m_pOwner->Invalidate();
        }
        
        LayeredWindow::OnFinalMessage(hWnd);
        delete this;
    }
    
    void MenuWindow::InitWindow() {
        return;
        
    }
    
    LRESULT MenuWindow::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                            BOOL& bHandled) {
                                            
                                            
        if (uMsg == WM_RBUTTONDOWN || uMsg == WM_CONTEXTMENU
                || uMsg == WM_RBUTTONUP || uMsg == WM_RBUTTONDBLCLK) {
            bHandled = TRUE;
            return TRUE;
        } else if (uMsg == WM_KEYDOWN) {
            if (wParam == VK_ESCAPE) {
                Close();
                bHandled = TRUE;
                return TRUE;
            }
        }
        
        /*	LRESULT lRes = 0;
        	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;*/
        return LayeredWindow::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
    }
    
    LRESULT MenuWindow::CreateMainMenuWindow(UINT uMsg, WPARAM wParam,
            LPARAM lParam, BOOL& bHandled) {
        LayeredWindow::OnCreate(uMsg, wParam, lParam, bHandled);
        CControlUI* control = m_PaintManager.FindControl(_T("menu"));
        m_pLayout = dynamic_cast<CMenuUI*>(control);
        
        //m_pLayout = dynamic_cast<CMenuUI*>(m_PaintManager.GetRoot());
#if defined(WIN32) && !defined(UNDER_CE)
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY),
                         &oMonitor);
        CDuiRect rcWork = oMonitor.rcWork;
#else
        CDuiRect rcWork;
        GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif
        SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
        szAvailable = m_pLayout->EstimateSize(szAvailable);
        m_PaintManager.SetInitSize(szAvailable.cx, szAvailable.cy);
        
        
        DWORD dwAlignment = eMenuAlignment_Left | eMenuAlignment_Top;
        
        
        
        SIZE szInit = m_PaintManager.GetInitSize();
        CDuiRect rc;
        CDuiPoint point = m_BasedPoint;
        rc.left = point.x;
        rc.top = point.y;
        rc.right = rc.left + szInit.cx;
        rc.bottom = rc.top + szInit.cy;
        
        int nWidth = rc.GetWidth();
        int nHeight = rc.GetHeight();
        
        if (m_BasedPoint.x + nWidth > rcWork.right) {
            dwAlignment |= eMenuAlignment_Right;
        }
        
        if (m_BasedPoint.y + nHeight > rcWork.bottom) {
            dwAlignment |= eMenuAlignment_Bottom;
        }
        
        
        if (dwAlignment & eMenuAlignment_Right) {
            rc.right = point.x;
            rc.left = rc.right - nWidth;
        }
        
        if (dwAlignment & eMenuAlignment_Bottom) {
            rc.bottom = point.y;
            rc.top = rc.bottom - nHeight;
        }
        
        
        SetForegroundWindow(m_hWnd);
        MoveWindow(m_hWnd, rc.left, rc.top, rc.GetWidth(), rc.GetHeight(), FALSE);
        SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left, rc.top, rc.GetWidth(),
                     rc.GetHeight(), SWP_SHOWWINDOW);
                     
        return TRUE;
    }
    
    LRESULT MenuWindow::CreateSubMenuWindow(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                            BOOL& bHandled) {
        LayeredWindow::OnCreate(uMsg, wParam, lParam, bHandled);
        CControlUI* control = m_PaintManager.FindControl(_T("menu"));
        m_pLayout = dynamic_cast<CMenuUI*>(control);
        //m_pLayout = dynamic_cast<CMenuUI*>(m_PaintManager.GetRoot());
        int nCount = m_pOwner->GetCount();
        int nCount1 = m_pLayout->GetCount();
        
        for (int i = 0; i < m_pOwner->GetCount(); i++) {
            if (m_pOwner->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL) {
                (static_cast<MenuElementUI*>(m_pOwner->GetItemAt(i)))->SetOwner(m_pLayout);
                m_pLayout->Add(static_cast<CControlUI*>(m_pOwner->GetItemAt(i)));
            }
        }
        
        
        
        
        int nCount3 = m_pLayout->GetCount();
        int nCount4 = m_pOwner->GetCount();
        
        // Position the popup window in absolute space
        RECT rcOwner = m_pOwner->GetPos();
        RECT rc = rcOwner;
        
        int cxFixed = 0;
        int cyFixed = 0;
        
#if defined(WIN32) && !defined(UNDER_CE)
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY),
                         &oMonitor);
        CDuiRect rcWork = oMonitor.rcWork;
#else
        CDuiRect rcWork;
        GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWork);
#endif
        SIZE szAvailable = { rcWork.right - rcWork.left, rcWork.bottom - rcWork.top };
        
        for (int it = 0; it < m_pOwner->GetCount(); it++) {
            if (m_pOwner->GetItemAt(it)->GetInterface(kMenuElementUIInterfaceName) !=
                    NULL) {
                CControlUI* pControl = static_cast<CControlUI*>(m_pOwner->GetItemAt(it));
                SIZE sz = pControl->EstimateSize(szAvailable);
                cyFixed += sz.cy;
                
                if (cxFixed < sz.cx) {
                    cxFixed = sz.cx;
                }
            }
        }
        
        //cyFixed += 4;
        //cxFixed += 4;
        
        RECT rcWindow;
        GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWindow);
        
        rc.top = rcOwner.top;
        rc.bottom = rc.top + cyFixed;
        ::MapWindowRect(m_pOwner->GetManager()->GetPaintWindow(), HWND_DESKTOP, &rc);
        rc.left = rcWindow.right;
        rc.right = rc.left + cxFixed;
        //rc.right += 2;
        
        bool bReachBottom = false;
        bool bReachRight = false;
        LONG chRightAlgin = 0;
        LONG chBottomAlgin = 0;
        
        RECT rcPreWindow = { 0 };
        ContextMenuObserver::Iterator<BOOL, ContextMenuParam> iterator(
            s_context_menu_observer);
        ReceiverImplBase<BOOL, ContextMenuParam>* pReceiver = iterator.next();
        
        while (pReceiver != NULL) {
            MenuWindow* pContextMenu = dynamic_cast<MenuWindow*>(pReceiver);
            
            if (pContextMenu != NULL) {
                GetWindowRect(pContextMenu->GetHWND(), &rcPreWindow);
                
                bReachRight = rcPreWindow.left >= rcWindow.right;
                bReachBottom = rcPreWindow.top >= rcWindow.bottom;
                
                if (pContextMenu->GetHWND() == m_pOwner->GetManager()->GetPaintWindow()
                        || bReachBottom || bReachRight) {
                    break;
                }
            }
            
            pReceiver = iterator.next();
        }
        
        if (bReachBottom) {
            rc.bottom = rcWindow.top;
            rc.top = rc.bottom - cyFixed;
        }
        
        if (bReachRight) {
            rc.right = rcWindow.left;
            rc.left = rc.right - cxFixed;
        }
        
        if (rc.bottom > rcWork.bottom) {
            rc.bottom = rc.top;
            rc.top = rc.bottom - cyFixed;
        }
        
        if (rc.right > rcWork.right) {
            rc.right = rcWindow.left;
            rc.left = rc.right - cxFixed;
            
            rc.top = rcWindow.bottom;
            rc.bottom = rc.top + cyFixed;
        }
        
        if (rc.top < rcWork.top) {
            rc.top = rcOwner.top;
            rc.bottom = rc.top + cyFixed;
        }
        
        if (rc.left < rcWork.left) {
            rc.left = rcWindow.right;
            rc.right = rc.left + cxFixed;
        }
        
        MoveWindow(m_hWnd, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
                   FALSE);
        return TRUE;
    }
    
    LRESULT MenuWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                 BOOL& bHandled) {
        if (m_pOwner != NULL) {
            CreateSubMenuWindow(uMsg, wParam, lParam, bHandled);
        } else {
            CreateMainMenuWindow(uMsg, wParam, lParam, bHandled);
        }
        
        if (isLayered()) {
            m_pLayout->SetBkColor(0xccffffff);
        } else {
            //m_pLayout->SetBkColor(0xffffffff);
        }
        
        bHandled = true;
        return TRUE;
    }
    
    LRESULT MenuWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL& bHandled) {
        if (m_pOwner != NULL && m_pLayout != NULL) {
            for (int i = 0; i < m_pLayout->GetCount(); i++) {
                if (m_pLayout->GetItemAt(i) != NULL) {
                    MenuElementUI* pControl = dynamic_cast<MenuElementUI*>(m_pLayout->GetItemAt(i));
                    pControl->SetOwner(m_pOwner);
                    pControl->SetOwner(m_pOwner);
                }
            }
            
            while (m_pLayout->GetCount() > 0) {
                m_pLayout->RemoveAt(0, true);
                
            }
            
            m_pOwner->SetManager(m_pOwner->GetManager(), m_pOwner->GetParent(), false);
            m_pOwner->SetPos(m_pOwner->GetPos(), false);
            m_pOwner->SetFocus();
        } else {
            ContextMenuParam param;
            param.hWnd = GetHWND();
            param.wParam = 4;
            s_context_menu_observer.RBroadcast(param);
        }
        
        return LayeredWindow::OnClose(uMsg, wParam, lParam, bHandled);
    }
    
    LRESULT MenuWindow::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL& bHandled) {
        HWND hFocusWnd = (HWND)wParam;
        
        BOOL bInMenuWindowList = FALSE;
        ContextMenuParam param;
        param.hWnd = GetHWND();
        
        ContextMenuObserver::Iterator<BOOL, ContextMenuParam> iterator(
            s_context_menu_observer);
        ReceiverImplBase<BOOL, ContextMenuParam>* pReceiver = iterator.next();
        
        while (pReceiver != NULL) {
            MenuWindow* pContextMenu = dynamic_cast<MenuWindow*>(pReceiver);
            
            if (pContextMenu != NULL && pContextMenu->GetHWND() == hFocusWnd) {
                bInMenuWindowList = TRUE;
                break;
            }
            
            pReceiver = iterator.next();
        }
        
        if (!bInMenuWindowList) {
            param.wParam = 1;
            s_context_menu_observer.RBroadcast(param);
            
        }
        
        return TRUE;
    }
    
    LRESULT MenuWindow::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL& bHandled) {
                                  
                                  
                                  
        if (wParam >= 'A' && wParam <= 'z') {
        
            CDuiPtrArray* arr = m_PaintManager.FindSubControlsByClass(m_pLayout,
                                kMenuElementUIClassName);
                                
            for (int i = 0; i < arr->GetSize(); ++i) {
                MenuElementUI* element = NULL;
                element = reinterpret_cast<MenuElementUI*>(arr->GetAt(i));
                
                if (element == NULL) {
                    continue;
                }
                
                TCHAR ch = toupper(element->GetShortcut());
                
                if (ch != wParam) {
                    continue;
                }
                
                if (!element->IsVisible()) {
                    continue;
                }
                
                if (!element->IsEnabled()) {
                    continue;
                }
                
                POINT pt = { 0 };
                pt.x = element->GetPos().left + 5;
                pt.y = element->GetPos().top + 5;
                
                TEventUI event = { 0 };
                event.Type = UIEVENT_BUTTONDOWN;
                event.pSender = element;
                event.chKey = (TCHAR)wParam;
                event.ptMouse = pt;
                event.wKeyState = wParam;
                event.dwTimestamp = ::GetTickCount();
                element->Event(event);
                return TRUE;
                
                
            }
            
            
        }
        
        
        return __super::OnKeyDown(uMsg, wParam, lParam, bHandled);
    }
    
    DuiLib::CDuiString MenuWindow::GetSkinFolder() {
        return _T("");
    }
    
    std::string MenuWindow::ParseSkinFile(amo::u8json& menu) {
        std::string strMenu;
        
        if (menu.is_array()) {
            std::vector<amo::u8json> vec = menu.to_array();
            
            for (auto& p : vec) {
                std::string sb = p.to_string();
                $clog(amo::cdevel << sb << amo::endl;);
                amo::u8string menuElenent(skinMenuElement, true);
                
                if (!p.contains_key("enabled")) {
                    p.put("enabled", true);
                }
                
                if (!p.contains_key("visible")) {
                    p.put("visible", true);
                }
                
                p.put("string", p.get<std::string>("text"));
                
                if (p.contains_key("shortcut")) {
                    std::string text = p.get<std::string>("text");
                    text += "(";
                    text += toupper(p.get<std::string>("shortcut").at(0));
                    text += ")";
                    p.put("text", text);
                }
                
                amo::u8json json;
                
                if (p.contains_key("menu")) {
                    amo::u8json subMenu = p.get_child("menu");
                    std::string out = ParseSkinFile(subMenu);
                    p.put("children", out);
                    json.put("bkimage", "arrow.png");
                }
                
                // 判断是否有子菜单
                // 判断是否有分隔符
                
                
                
                if (p.contains_key("separator") && p.get<bool>("separator")) {
                    json.put("separatorColor", m_pMenuSettings->separatorColor);
                }
                
                if (p.contains_key("icon")) {
                    std::string icon = p.getString("icon");
                    json.put("icon", icon);
                }
                
                std::string selected;
                
                if (p.contains_key("selected")) {
                    if (p.is_bool("selected") && p.getBool("selected")) {
                        selected = "checkbox";
                    } else if (p.is_string("selected")) {
                        selected = p.getString("selected");
                    }
                }
                
                
                if (selected == "checkbox") {
                    json.put("icon", "checkbox.png");
                } else if (selected == "radio") {
                    json.put("icon", "radio.png");
                }
                
                
                if (!json.empty()) {
                    amo::u8string misc(skinMenuElementMisc, true);
                    misc = misc.format(json);
                    p.put("misc", misc.to_utf8());
                }
                
                
                //            if (p.contains_key("separator") && p.get<bool>("separator")) {
                //                amo::string separator(skinSeparator, true);
                //                separator = separator.format(m_pMenuSettings->m_json);
                //                p.put("separator", separator.to_ansi());
                //            }
                //else{
                //	p.put("separator", "0x00ffffff");
                //}
                
                strMenu += menuElenent.format(p).to_utf8();
                
                
            }
        }
        
        return strMenu;
    }
    
    
    DuiLib::CDuiString MenuWindow::GetSkinFile() {
        //return _T("menutest2.xml");
        
        if (m_pOwner == NULL) {
            std::string menuSettings = "";
            amo::u8json menu = m_pMenuSettings->settings.get_child("menu");
            
            if (menu.is_array()) {
                menuSettings += ParseSkinFile(menu);
            }
            
            std::wstring ws = amo::string_utils::utf8_to_wide(menu.to_string());
            std::wstring ws2 = amo::string_utils::utf8_to_wide(menuSettings);
            amo::u8string skin(menuSettings, true);
            m_pMenuSettings->settings.put("element", skin.to_utf8());
            
            amo::u8string skinTemplate(skinMenuWindow, true);
            skin = skinTemplate.format(m_pMenuSettings->settings);
            //skin = amo::format(skinMenuWindow, m_pMenuSettings->m_json);
            //auto ssss = amo::string_utils::utf8_to_wide(skin.to_utf8());
            return skin.to_unicode().c_str();
            
        } else {
        
            amo::u8json json;
            amo::u8string skinTemplate(skinMenuWindow, true);
            amo::u8string  skin = skinTemplate.format(json);
            //auto ssss = amo::string_utils::utf8_to_wide(skin.to_utf8());
            return skin.to_unicode().c_str();
        }
    }
    
    DWORD MenuWindow::GetBkColor() const {
        return m_pLayout->GetBkColor();
    }
    
    void MenuWindow::SetBkColor(DWORD dwBackColor) {
        m_pLayout->SetBkColor(dwBackColor);
    }
    
    LPCTSTR MenuWindow::GetBkImage() {
        return m_pLayout->GetBkImage();
    }
    
    void MenuWindow::SetBkImage(LPCTSTR pStrImage) {
        m_pLayout->SetBkImage(pStrImage);
    }
    
    
    
}
