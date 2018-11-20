#include "stdAfx.h"
#include "ui/win/LayeredWindow.h"
#include "Core/UIRender.h"
#include <amo/uuid.hpp>

#include "settings/NativeWindowSettings.h"
#include "settings/GlobalShortcutSettings.h"
#include "ui/win/Imm32Manager.h"
#include "Bitmap.hpp"


#define WM_DRAWWINDOW (WM_USER +9999)






namespace amo {

    std::function<bool(int32_t, amo::u8json)>
    LayeredWindow::getHotKeyEventCallback()
    const {
        return m_fnHotKeyEventCallback;
    }
    
    void LayeredWindow::setHotKeyEventCallback(
        std::function<bool(int32_t, amo::u8json)> val) {
        m_fnHotKeyEventCallback = val;
    }
    
    LayeredWindow::LayeredWindow(std::shared_ptr<NativeWindowSettings>
                                 pBrowserSettings)
        : m_nMsgIndex(CUSTOM_MSG_BEGIN)
        , m_pNativeSettings(pBrowserSettings) {
        
        m_Blend.BlendOp = AC_SRC_OVER; //theonlyBlendOpdefinedinWindows2000
        m_Blend.BlendFlags = 0; //nothingelseisspecial...
        m_Blend.AlphaFormat = AC_SRC_ALPHA; //...
        m_Blend.SourceConstantAlpha = 255; //窗口透明度
        
        m_isFullScreen = false;
        m_isTopmost = false;
        
        
        if (m_pNativeSettings->transparent && m_pNativeSettings->offscreen) {
            setLayered(true);
        } else {
            setLayered(false);
        }
        
    }
    
    LayeredWindow::~LayeredWindow() {
        $clog(amo::cdevel << __FUNCTION__ << amo::endl;);
        
    }
    
    void LayeredWindow::drawWindow() {
    
        PAINTSTRUCT ps = { 0 };
        ::BeginPaint(m_hWnd, &ps);
        RECT rcClient;
        ::GetWindowRect(m_hWnd, &rcClient);
        int nWidth = rcClient.right - rcClient.left;
        int nHeight = rcClient.bottom - rcClient.top;
        SIZE wndSize = { rcClient.right - rcClient.left, rcClient.bottom - rcClient.top };
        HDC hDC = ::GetDC(m_hWnd);
        
        HDC memDC = NULL;
        
        if (memDC == NULL) {
            memDC = ::CreateCompatibleDC(hDC);
        }
        
        BITMAPINFO bitmapinfo;
        bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bitmapinfo.bmiHeader.biBitCount = 32;
        bitmapinfo.bmiHeader.biHeight = nHeight;
        bitmapinfo.bmiHeader.biWidth = nWidth;
        bitmapinfo.bmiHeader.biPlanes = 1;
        bitmapinfo.bmiHeader.biCompression = BI_RGB;
        bitmapinfo.bmiHeader.biXPelsPerMeter = 0;
        bitmapinfo.bmiHeader.biYPelsPerMeter = 0;
        bitmapinfo.bmiHeader.biClrUsed = 0;
        bitmapinfo.bmiHeader.biClrImportant = 0;
        bitmapinfo.bmiHeader.biSizeImage = bitmapinfo.bmiHeader.biWidth
                                           * bitmapinfo.bmiHeader.biHeight
                                           * bitmapinfo.bmiHeader.biBitCount / 8;
                                           
        HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, wndSize.cx, wndSize.cy);
        HBITMAP hOldBitmap = (HBITMAP)::SelectObject(memDC, hBitmap);
        CControlUI* pRoot = m_PaintManager.GetRoot();
        
        if (pRoot == NULL) {
            return;
        }
        
        pRoot->DoPaint(memDC, pRoot->GetPos(), NULL);
        POINT ptSrc = { 0, 0 };
        SIZE sz = { nWidth, nHeight };
        POINT ptDest = { rcClient.left, rcClient.top };
        
        SIZE szLayered = { rcClient.right - rcClient.left, rcClient.bottom - rcClient.top };
        
        BOOL bOK = ::UpdateLayeredWindow(m_hWnd,
                                         hDC,
                                         NULL,//&ptDst,			// 更新后窗口左上角的坐标点 ，若不改变窗口的形状和绘制可以为NULL
                                         &sz,					// 更新后窗口的宽度和高度，可以为NULL
                                         memDC,					// 给窗口贴图的内存DC，可以为NULL
                                         &ptSrc,					// 内存DC中图像的左上角点，可以为NULL
                                         0,						//
                                         &m_Blend,
                                         ULW_ALPHA);
                                         
        DWORD wd = GetLastError();
        ::SelectObject(memDC, hOldBitmap);
        DeleteObject(hBitmap);
        
        ::ReleaseDC(m_hWnd, memDC);
        ::ReleaseDC(m_hWnd, hDC);
        ::EndPaint(m_hWnd, &ps);
        
    }
    
    LRESULT LayeredWindow::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                        BOOL& bHandled) {
        if (wParam == SC_CLOSE) {
            bHandled = TRUE;
            SendMessage(WM_CLOSE);
            return 0;
        }
        
#if defined(WIN32) && !defined(UNDER_CE)
        BOOL bZoomed = ::IsZoomed(*this);
        LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
        
        if (::IsZoomed(*this) != bZoomed) {
            CControlUI* pbtnMax
                = static_cast<CControlUI*>(
                      m_PaintManager.FindControl(_T("maxbtn")));         // max button
            CControlUI* pbtnRestore
                = static_cast<CControlUI*>(
                      m_PaintManager.FindControl(_T("restorebtn"))); // restore button
                      
            // toggle status of max and restore button
            if (pbtnMax && pbtnRestore && (::IsIconic(m_hWnd) != TRUE)) {
                pbtnMax->SetVisible(TRUE == bZoomed);
                pbtnRestore->SetVisible(FALSE == bZoomed);
            }
        }
        
#else
        LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#endif
        return lRes;
        
        //return WindowImplBase::OnSysCommand(uMsg, wParam, lParam, bHandled);
    }
    
    LRESULT LayeredWindow::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                       BOOL& bHandled) {
        if (m_isFullScreen) {
            return HTCLIENT;
        }
        
        POINT pt;
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        ::ScreenToClient(*this, &pt);
        
        RECT rcClient;
        ::GetClientRect(*this, &rcClient);
        
        if (!::IsZoomed(*this)) {
            RECT rcSizeBox = m_PaintManager.GetSizeBox();
            
            if (pt.y < rcClient.top + rcSizeBox.top) {
                if (pt.x < rcClient.left + rcSizeBox.left) {
                    return HTTOPLEFT;
                }
                
                if (pt.x > rcClient.right - rcSizeBox.right) {
                    return HTTOPRIGHT;
                }
                
                return HTTOP;
            } else if (pt.y > rcClient.bottom - rcSizeBox.bottom) {
                if (pt.x < rcClient.left + rcSizeBox.left) {
                    return HTBOTTOMLEFT;
                }
                
                if (pt.x > rcClient.right - rcSizeBox.right) {
                    return HTBOTTOMRIGHT;
                }
                
                return HTBOTTOM;
            }
            
            if (pt.x < rcClient.left + rcSizeBox.left) {
                return HTLEFT;
            }
            
            if (pt.x > rcClient.right - rcSizeBox.right) {
                return HTRIGHT;
            }
        }
        
        RECT rcCaption = m_PaintManager.GetCaptionRect();
        
        CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
        
        if (isDragable(pControl)) {
            return HTCAPTION;
        } else {
            return HTCLIENT;
        }
        
        return HTCLIENT;
        
        //return WindowImplBase::OnNcHitTest(uMsg, wParam, lParam, bHandled);
    }
    
    LRESULT LayeredWindow::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                           BOOL& bHandled) {
        if (!m_isFullScreen) {
            return WindowImplBase::OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled);
        }
        
        //处理全屏最大大小
        pmmi = (MINMAXINFO*)lParam;
        pmmi->ptMaxPosition.x = m_rectFullScreen.left;
        pmmi->ptMaxPosition.y = m_rectFullScreen.top;
        pmmi->ptMaxSize.x = m_rectFullScreen.right - m_rectFullScreen.left;
        pmmi->ptMaxSize.y = m_rectFullScreen.bottom - m_rectFullScreen.top;
        pmmi->ptMaxTrackSize.x = m_rectFullScreen.right - m_rectFullScreen.left;
        pmmi->ptMaxTrackSize.y = m_rectFullScreen.right - m_rectFullScreen.left;
        bHandled = TRUE;
        return TRUE;
    }
    
    LRESULT LayeredWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (uMsg >= CUSTOM_MSG_BEGIN && uMsg <= CUSTOM_MSG_END) {
            onUIThreadEvent(uMsg);
        }
        
        LRESULT lRes = FALSE;
        BOOL bHandled = FALSE;
        
        switch (uMsg) {
        case WM_PAINT:
            lRes = OnPaint(uMsg, wParam, lParam, bHandled);
            break;
            
        case WM_DRAWWINDOW:
            drawWindow();
            break;
            
        case WM_HOTKEY:
            lRes = OnHotKey(uMsg, wParam, lParam, bHandled);
            
        default:
            break;
        }
        
        if (bHandled) {
            return lRes;
        }
        
        return   WindowImplBase::HandleMessage(uMsg, wParam, lParam);
    }
    
    LRESULT LayeredWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                     BOOL& bHandled) {
                                     
                                     
        unregisterAllHotKey();
        return WindowImplBase::OnDestroy(uMsg, wParam, lParam, bHandled);
    }
    
    LRESULT LayeredWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL& bHandled) {
        if (isLayered()) {
            DWORD dwExStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
            dwExStyle |= WS_EX_LAYERED | 0x80000L
                         ;					//!< 修改窗口的扩展风格为透明
            ::SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle);
            ::PostMessage(m_hWnd, WM_DRAWWINDOW, NULL, NULL);
        }
        
        return FALSE;
    }
    
    
    LRESULT LayeredWindow::OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL& bHandled) {
        // 如果有热键处理回调函数，先交给回调函数处理
        if (m_fnHotKeyEventCallback) {
            int32_t nKey = (int)wParam;
            std::string strKey ;
            
            
            auto iter = m_oHotKeyMap.find(nKey);
            
            if (iter == m_oHotKeyMap.end()) {
                return FALSE;
            }
            
            m_fnHotKeyEventCallback(nKey, iter->second->settings);
            return TRUE;
        } else {
            return FALSE;
        }
    }
    
    void LayeredWindow::setLayered(bool bLayered) {
        if (m_bLayered != bLayered && m_hWnd != NULL) {
            needUpdate();
        }
        
        m_bLayered = bLayered;
        
    }
    
    bool LayeredWindow::isLayered() const {
        return m_bLayered;
    }
    
    void LayeredWindow::runOnUIThread(amo::function<void()> fn) {
        amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
        
        if (m_hWnd == NULL) {
            $clog(amo::cwarn << "RunOnUiThread INVALID HWND" << amo::endl;);
            return;
        }
        
        uint32_t msg_id = getMessageID();
        m_oMsgMap.insert(std::pair<uint32_t, amo::function<void()> >(msg_id, fn));
        ::PostMessage(m_hWnd, msg_id, 0, 0);
    }
    
    void LayeredWindow::onUIThreadEvent(uint32_t umsg) {
        amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
        auto iter = m_oMsgMap.find(umsg);
        
        if (iter == m_oMsgMap.end()) {
            return;
        }
        
        iter->second();
        m_oMsgMap.erase(iter);
    }
    
    uint32_t LayeredWindow::getMessageID() {
        amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
        ++m_nMsgIndex;
        
        if (m_nMsgIndex > CUSTOM_MSG_END) {
            m_nMsgIndex = CUSTOM_MSG_BEGIN;
        }
        
        return m_nMsgIndex;
    }
    
    
    bool LayeredWindow::isDragable(CControlUI* pControl) {
        if (!pControl) {
            return false;
        }
        
        CDuiString attr = pControl->GetCustomAttribute(_T("dragable"));
        
        if (attr == _T("drag") || attr == _T("true")) {
            return true;
        } else if (attr == _T("no-drag") ||  attr == _T("false")) {
            return false;
        } else {
            return isDragable(pControl->GetParent());
        }
        
    }
    
    void LayeredWindow::setClosedCallback(ClosedCbType fn) {
        m_fnClosedCallback = fn;
    }
    
    bool LayeredWindow::ptInWindow() {
        POINT pt = { 0 };
        ::GetCursorPos(&pt);
        return ptInWindow(pt);
    }
    
    bool LayeredWindow::ptInWindow(POINT& pt) {
        RECT rect = { 0 };
        ::GetWindowRect(m_hWnd, &rect);
        return ::PtInRect(&rect, pt) != FALSE;
    }
    
    void LayeredWindow::InitWindow() {
    
        imm32_manager_.reset(new IMM32Manager(m_hWnd));
        imm32_manager_->disableIME();
        ::GetWindowPlacement(*this, &m_wpNormalScreen);
        m_nScreenWidth = GetSystemMetrics(SM_CXSCREEN);//屏幕横向分辨率
        m_nScreenHeight = GetSystemMetrics(SM_CYSCREEN);//屏幕纵向分辨率
        
        // 设置窗口位置
        if (m_pNativeSettings->adjustPos) {
            if (m_pNativeSettings->center) {
                CenterWindow();
            } else {
                ::MoveWindow(m_hWnd, m_pNativeSettings->x,
                             m_pNativeSettings->y,
                             m_pNativeSettings->width,
                             m_pNativeSettings->height, TRUE);
            }
        }
        
        
        // 禁止改变窗口大小， 包括禁止拖动窗口，禁止最大化， 不响应NCLDBCLICK
        if (!m_pNativeSettings->resizable) {
            RECT rcSizeBox = { 0, 0, 0, 0 };
            m_PaintManager.SetSizeBox(rcSizeBox);
        }
        
        // 窗口置顶
        if (m_pNativeSettings->alwaysOnTop) {
            setTopmost(true);
        }
        
        if (!m_pNativeSettings->icon.empty()) {
            setIcon(m_pNativeSettings->icon);
        }
        
        if (!m_pNativeSettings->title.empty()) {
            amo::u8string str(m_pNativeSettings->title, true);
            SetWindowTextW(m_hWnd, str.to_unicode().c_str());
        }
        
        if (m_pNativeSettings->fullscreen && m_pNativeSettings->fullscreenable) {
            setFullscreen(true);
        }
        
        ::SetForegroundWindow(m_hWnd);
        
        
        
        
        /* ShadowWindow::Initialize(m_PaintManager.GetInstance());
         m_Shadow.Create(m_hWnd);
         RECT cornerRect = { 7, 7, 7, 7 };
         int nCorner = m_pNativeSettings->roundcorner;
         RECT rcHoleOffset = { nCorner, nCorner, nCorner, nCorner };
         m_Shadow.SetImage(_T("shadow.png"), cornerRect, rcHoleOffset);
         m_Shadow.Show(m_pNativeSettings->hasShadow);*/
        
        int nCorner = m_pNativeSettings->roundcorner;
        RECT rcHoleOffset = { nCorner, nCorner, nCorner, nCorner };
        ShadowWindow::Initialize(m_PaintManager.GetInstance());
        m_Shadow.SetPosition(0, 0);
        m_Shadow.SetImage(_T("shadow.png"));
        RECT cornerRect = { 7, 7, 7, 7 };
        
        m_Shadow.setHoleOffset(rcHoleOffset);
        m_Shadow.SetShadowCorner(cornerRect);
        m_Shadow.ShowShadow(true);
        m_Shadow.Create(&m_PaintManager);
        m_Shadow.Show(m_pNativeSettings->hasShadow);
        
        // 隐藏/显示窗口
        ShowWindow(m_pNativeSettings->show);
    }
    
    void LayeredWindow::OnFinalMessage(HWND hWnd) {
        WindowImplBase::OnFinalMessage(hWnd);
        
        if (m_fnClosedCallback) {
            m_fnClosedCallback(this);
        }
        
        
    }
    uint8_t LayeredWindow::getTransparent() const {
        return m_Blend.SourceConstantAlpha;
    }
    
    void LayeredWindow::setTransparent(uint8_t val) {
        m_Blend.SourceConstantAlpha = val;
        needUpdate();
    }
    
    void LayeredWindow::needUpdate() {
        // 重绘窗口
        m_PaintManager.NeedUpdate();
        m_PaintManager.Invalidate();
    }
    
    
    LRESULT LayeredWindow::ResponseDefaultKeyEvent(WPARAM wParam) {
        switch (wParam) {
        case VK_ESCAPE:
            return 0;	// ESC 不关闭窗口
            
        default:
            break;
        }
        
        return WindowImplBase::ResponseDefaultKeyEvent(wParam);
    }
    
    
    void LayeredWindow::setFullscreen(bool bFull) {
        if (m_isFullScreen == bFull) {
            return ;
        }
        
        m_isFullScreen = bFull;
        
        if (m_isFullScreen) {
            //保存全屏前窗口显示状态信息
            m_wpNormalScreen.length = sizeof(WINDOWPLACEMENT);
            ::GetWindowPlacement(m_hWnd, &m_wpNormalScreen);
            //获取窗口和客户区大小
            ::GetWindowRect(m_hWnd, &m_rectWindow);
            ::GetClientRect(m_hWnd, &m_rectClient);
            m_ptUpLeft.x = m_rectClient.left;
            m_ptUpLeft.y = m_rectClient.top;
            m_ptDownRight.x = m_rectClient.right;
            m_ptDownRight.y = m_rectClient.bottom;
            ::ClientToScreen(m_hWnd, &m_ptUpLeft);
            ::ClientToScreen(m_hWnd, &m_ptDownRight);
            //计算全屏时窗口位置
            m_rectFullScreen.left = m_rectWindow.left - m_ptUpLeft.x;
            m_rectFullScreen.top = m_rectWindow.top - m_ptUpLeft.y;
            m_rectFullScreen.right = m_nScreenWidth
                                     + m_rectWindow.right - m_ptDownRight.x;
            m_rectFullScreen.bottom = m_nScreenHeight
                                      + m_rectWindow.bottom - m_ptDownRight.y;
            //调用SetWindowPlacement实现全屏
            m_wpFullScreen.length = sizeof(WINDOWPLACEMENT);
            m_wpFullScreen.flags = 0;
            m_wpFullScreen.showCmd = SW_SHOWNORMAL;
            m_wpFullScreen.rcNormalPosition = m_rectFullScreen;
            ::SetWindowPos(m_hWnd,
                           HWND_TOPMOST,
                           m_rectFullScreen.left,
                           m_rectFullScreen.top,
                           m_rectFullScreen.right - m_rectFullScreen.left,
                           m_rectFullScreen.bottom - m_rectFullScreen.top,
                           SWP_FRAMECHANGED);
            SetWindowPlacement(m_hWnd, &m_wpFullScreen);
            ::PostMessage(m_hWnd, WM_ENTER_FULLSCREEN, NULL, NULL);
            
        } else {
            SetWindowPlacement(m_hWnd, &m_wpNormalScreen);
            setTopmost(m_isTopmost);
            ::PostMessage(m_hWnd, WM_LEAVE_FULLSCREEN, NULL, NULL);
        }
    }
    
    void LayeredWindow::setTopmost(bool bTopmost) {
        m_isTopmost = bTopmost;
        RECT rect;
        ::GetWindowRect(m_hWnd, &rect);
        
        if (m_isTopmost)
            ::SetWindowPos(*this,
                           HWND_TOPMOST,
                           rect.left,
                           rect.top,
                           rect.right - rect.left,
                           rect.bottom - rect.top,
                           0);
        else
            ::SetWindowPos(*this,
                           HWND_NOTOPMOST,
                           rect.left,
                           rect.top,
                           rect.right - rect.left,
                           rect.bottom - rect.top,
                           0);
    }
    
    void LayeredWindow::setIcon(const std::string& strPath) {
    
        HICON hIcon = (HICON)LoadImageA(NULL,
                                        strPath.c_str(),
                                        IMAGE_ICON,
                                        0,
                                        0,
                                        LR_LOADFROMFILE);
        ::SendMessage(m_hWnd, WM_SETICON, (WPARAM)false, (LPARAM)hIcon);
        
    }
    
    void LayeredWindow::enableResizeInCaption() {
        DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
        dwStyle &= ~UI_WNDSTYLE_DIALOG;
        dwStyle |= UI_WNDSTYLE_FRAME;
        
        ::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
    }
    
    void LayeredWindow::disableResizeInCaption() {
        DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
        dwStyle &= ~UI_WNDSTYLE_FRAME;
        dwStyle |= UI_WNDSTYLE_DIALOG;
        
        ::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
    }
    
    void LayeredWindow::resizableInCaption(bool resizable /*= true*/) {
        if (resizable) {
            enableResizeInCaption();
        } else {
            disableResizeInCaption();
        }
    }
    
    bool LayeredWindow::registerHotKey(const int32_t& nID,
                                       const uint32_t& nMod,
                                       const uint32_t& nKey) {
        this->unregisterHotKey(nID);
        
        BOOL bRet = ::RegisterHotKey(m_hWnd, nID, nMod, nKey);
        
        if (bRet != TRUE) {
            return false;
        }
        
        m_oHotKeyContainer.insert(nID);
        return true;
    }
    
    int32_t LayeredWindow::registerHotKey(const uint32_t& nMod,
                                          const uint32_t& nKey) {
        int32_t nID = createHotKey();
        
        if (registerHotKey(nID, nMod, nKey)) {
            return nKey;
        }
        
        return 0;
    }
    
    std::shared_ptr<GlobalShortcutSettings> LayeredWindow::createSettingByString(
        const std::string& strKey)  const {
        amo::u8string sKey(strKey, true);
        sKey = sKey.replace(amo::u8string(" ", true), amo::u8string("",
                            true)); // 移除所有空格
                            
        std::vector<amo::u8string> vec = sKey.split(amo::u8string("+", true));
        
        std::shared_ptr<GlobalShortcutSettings> pSettings;
        pSettings.reset(new GlobalShortcutSettings());
        
        amo::u8json json;
        
        for (size_t i = 0; i < vec.size(); ++i) {
            if (vec[i] == amo::u8string("MOD_CONTROL", true)) {
                json.put("ctrl", true);
            } else if (vec[i] == amo::u8string("MOD_SHIFT", true)) {
                json.put("shift", true);
            } else if (vec[i] == amo::u8string("MOD_WIN", true)) {
                json.put("win", true);
            } else if (vec[i] == amo::u8string("MOD_ALT", true)) {
                json.put("alt", true);
            } else if (vec[i].size() == 1) {
                json.put("key", vec[i].str());
            } else {
                return std::shared_ptr<GlobalShortcutSettings>();
            }
            
        }
        
        pSettings->updateArgsSettings(json.to_string());
        return pSettings;
    }
    
    int32_t LayeredWindow::registerHotKey(const std::string& strKey) {
    
        std::shared_ptr<GlobalShortcutSettings> pSettings;
        pSettings = createSettingByString(strKey);
        return registerHotKey(pSettings);
    }
    
    
    int32_t LayeredWindow::registerHotKey(std::shared_ptr<GlobalShortcutSettings>
                                          pSettings) {
        if (!pSettings) {
            return 0;
        }
        
        uint32_t uiModifiers = 0;
        uint32_t uiVk = 0;
        
        if (pSettings->ctrl) {
            uiModifiers |= MOD_CONTROL;
        }
        
        if (pSettings->win) {
            uiModifiers |= MOD_WIN;
        }
        
        if (pSettings->alt) {
            uiModifiers |= MOD_ALT;
        }
        
        if (pSettings->shift) {
            uiModifiers |= MOD_SHIFT;
        }
        
        // 至少要有一个控制键
        if (uiModifiers == 0) {
            return 0;
        }
        
        
        // 字符键只能有一位
        if (pSettings->key.size() != 1) {
            return 0;
        }
        
        char key = pSettings->key.at(0);
        
        if (key < '0' || key > 'z') {
            return 0;
        }
        
        int32_t nHotkeyID = 0;
        nHotkeyID = createHotKey();
        uiVk = key;
        
        if (registerHotKey(nHotkeyID, uiModifiers, uiVk)) {
            m_oHotKeyMap[nHotkeyID] = pSettings;
            return nHotkeyID;
        }
        
        return 0;
        
    }
    
    void LayeredWindow::unregisterHotKey(const int32_t& iHotkeyID) {
        ::UnregisterHotKey(m_hWnd, iHotkeyID);
        m_oHotKeyContainer.erase(iHotkeyID);
        m_oHotKeyMap.erase(iHotkeyID);
    }
    
    void LayeredWindow::unregisterHotKey(const std::string& strKey) {
    
        std::shared_ptr<GlobalShortcutSettings> pSettings;
        pSettings = createSettingByString(strKey);
        
        for (auto& p : m_oHotKeyMap) {
            if (!p.second->isSameGlobalShortcut(pSettings)) {
                continue;
                
            }
            
            unregisterHotKey(p.first);
            return;
        }
        
        
        
    }
    
    void LayeredWindow::unregisterAllHotKey() {
        while (!m_oHotKeyContainer.empty()) {
            this->unregisterHotKey(*m_oHotKeyContainer.begin());
        }
    }
    
    bool LayeredWindow::isRegistered(int32_t nHotKey) const {
        return m_oHotKeyContainer.find(nHotKey) != m_oHotKeyContainer.end();
    }
    
    bool LayeredWindow::isRegistered(const std::string& strKey) const {
        std::shared_ptr<GlobalShortcutSettings> pSettings;
        pSettings = createSettingByString(strKey);
        
        for (auto& p : m_oHotKeyMap) {
            if (!p.second->isSameGlobalShortcut(pSettings)) {
                continue;
            }
            
            return true;
        }
        
        return false;
    }
    
    int32_t LayeredWindow::createHotKey() {
    
        int32_t key = GlobalAddAtomA(amo::uuid().to_string().c_str()) - 0xc000;
        //m_oHotKeyContainer.insert(key);
        return key;
    }
}