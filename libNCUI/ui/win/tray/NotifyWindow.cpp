#include "stdafx.h"
#include "ui/win/tray/NotifyWindow.h"

#include "ui/win/tray/Tray.h"
#include "ipc/UIMessageBroadcaster.hpp"
#include "transfer/GlobalShortcutTransfer.h"
#include "settings/BrowserWindowSettings.h"
#include "settings/GlobalShortcutSettings.h"
#include "ui/win/ActionPreprocessor.h"
#include "ui/win/BrowserWindowSkin.h"


namespace amo {

    NotifyWindow::NotifyWindow(std::shared_ptr<BrowserWindowSettings>
                               pBrowserSettings)
        : LayeredWindow(pBrowserSettings) {
        AMO_TIMER_ELAPSED();
    }
    
    
    NotifyWindow::~NotifyWindow() {
        //m_totalMsgs.clear();
        m_hWnd = NULL;
    }
    
    
    
    
    CDuiString NotifyWindow::GetSkinFolder() {
        return _T("");
    }
    
    CDuiString NotifyWindow::GetSkinFile() {
        AMO_TIMER_ELAPSED();
        return amo::u8string(skinNotifyWindow, true).to_unicode().c_str();
    }
    
    LPCTSTR NotifyWindow::GetWindowClassName(void) const {
        return _T("NotifyWindow");
    }
    
    void NotifyWindow::InitWindow() {
        AMO_TIMER_ELAPSED();
        RECT rect = { 0 };
        RECT workRect = { 0 };
        ::SystemParametersInfo(SPI_GETWORKAREA, 0, &workRect, SPIF_SENDCHANGE);
        rect.left = workRect.right - SCREENRIGHTINTERVAL - BOXFRAMEWIDTH; //x
        rect.top = workRect.bottom - SCREENBOTTONINTERVAL - BOXFRAMEHEIGHT; //y
        rect.right = rect.left + SCREENRIGHTINTERVAL + BOXFRAMEWIDTH; //w
        rect.bottom = rect.top + SCREENBOTTONINTERVAL + BOXFRAMEHEIGHT; //h
        
        ::SetWindowPos(m_hWnd, HWND_TOPMOST, rect.left, rect.top,
                       rect.right - rect.left, rect.bottom - rect.top, TRUE);
        //m_nCaptureScreenKey = createHotKey();
        //registerHotKey(m_nCaptureScreenKey, MOD_CONTROL | MOD_ALT, 'O');
        LayeredWindow::InitWindow();
        AMO_TIMER_ELAPSED();
        return;
    }
    
    
    
    void NotifyWindow::OnFinalMessage(HWND hWnd) {
        LayeredWindow::OnFinalMessage(hWnd);
        delete this;
    }
    
    
    LRESULT NotifyWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (m_fnTrayMessageCallback &&
                m_fnTrayMessageCallback(uMsg, wParam, lParam) == TRUE) {
            return TRUE;
        }
        
        if (m_fnClipMessageCallback &&
                m_fnClipMessageCallback(m_hWnd, uMsg, wParam, lParam) == TRUE) {
            return TRUE;
        }
        
        return LayeredWindow::HandleMessage(uMsg, wParam, lParam);
    }
    
    std::function<LRESULT(INT, WPARAM, LPARAM)>
    NotifyWindow::getTrayMessageCallback() const {
        return m_fnTrayMessageCallback;
    }
    
    void NotifyWindow::setTrayMsgCallback(
        std::function<LRESULT(INT, WPARAM, LPARAM)> val) {
        m_fnTrayMessageCallback = val;
    }
    
    
    std::function<LRESULT(HWND, INT, WPARAM, LPARAM)>
    NotifyWindow::getClipMessageCallback() const {
        return m_fnClipMessageCallback;
    }
    
    void NotifyWindow::setClipMsgCallback(
        std::function<LRESULT(HWND, INT, WPARAM, LPARAM)> val) {
        m_fnClipMessageCallback = val;
    }
    
    
    LRESULT NotifyWindow::OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL& bHandled) {
                                   
        int32_t nKey = (int)wParam;
        auto iter = m_oHotKeyMap.find(nKey);
        
        if (iter == m_oHotKeyMap.end()) {
            return FALSE;
        }
        
        ActionPreprocessor processor;
        
        if (processor.process(iter->second->action)) {
            return TRUE;
        }
        
        
        int64_t nObjectID =
            ClassTransfer::getUniqueTransfer<GlobalShortcutTransfer>()->getObjectID();
        std::shared_ptr<UIMessageBroadcaster> runner(new UIMessageBroadcaster(
                    nObjectID));
                    
        runner->broadcast("hotkeydown", nKey, iter->second->settings);
        
        return TRUE;
    }
    
    
    
}