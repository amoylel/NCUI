#include "stdAfx.h"

#include "ui/win/MessageWindow.h"

#include <limits>
#include <utility>

#include <windows.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#endif


#include <amo/logger.hpp>
#include "ui/win/BrowserWindowSkin.h"


namespace amo {


    std::unordered_map<HWND, int64_t> MessageWindow::boxs;
    
    UINT MessageWindow::Show(HWND pMainWnd,
                             const amo::u8json& settings,
                             LPCTSTR lpText,
                             LPCTSTR lpCaption,
                             UINT uType,
                             UINT uIcon) {
                             
        std::shared_ptr<NativeWindowSettings> pSettings;
        pSettings.reset(new NativeWindowSettings());
        pSettings->updateArgsSettings(settings.to_string());
        pSettings->hasShadow = true;
        MessageWindow* msgBox = new MessageWindow(pSettings);
        BOOL bVisible = IsWindowVisible(pMainWnd);
        BOOL bIconic = IsIconic(pMainWnd);
        
        if (bVisible == FALSE) {
            ::ShowWindow(pMainWnd, SW_SHOW);    //显示隐藏窗口
        }
        
        if (bIconic == TRUE) {
            ::ShowWindow(pMainWnd, SW_RESTORE);    //显示最小化窗口
        }
        
        msgBox->Create(pMainWnd, lpCaption, UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
        msgBox->setCaption(lpCaption);
        msgBox->setType(uType);
        msgBox->setText(lpText);
        msgBox->SetIcon(uIcon);
        ////msgBox->CenterWindow();
        msgBox->setTopmost(true);
        boxs.insert(std::pair<HWND, int64_t>(pMainWnd, (int64_t)msgBox));
        UINT uRet = msgBox->ShowModal();
        return uRet;
    }
    
    UINT MessageWindow::ShowPrompt(HWND pMainWnd,
                                   const amo::u8json& settings,
                                   LPCTSTR lpText,
                                   CDuiString* lpdefaultPrompt /*= _T("")*/,
                                   LPCTSTR lpCaption /*= _T("提示")*/,
                                   UINT uType /*= MB_OK*/,
                                   UINT uIcon /*= MB_ICONWARNING*/) {
        std::shared_ptr<NativeWindowSettings> pSettings;
        pSettings.reset(new NativeWindowSettings());
        pSettings->updateArgsSettings(settings.to_string());
        pSettings->hasShadow = true;
        MessageWindow* msgBox = new MessageWindow(pSettings);
        msgBox->Create(pMainWnd, lpCaption, UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
        msgBox->setCaption(lpCaption);
        msgBox->setType(uType);
        msgBox->setPrompt(lpdefaultPrompt);
        msgBox->setText(lpText);
        msgBox->SetIcon(uIcon);
        msgBox->CenterWindow();
        msgBox->setTopmost(true);
        boxs.insert(std::pair<HWND, int64_t>(pMainWnd, (int64_t)msgBox));
        UINT uRet = msgBox->ShowModal();
        return uRet;
    }
    
    void MessageWindow::Hide(HWND pMainWnd) {
        auto iter = boxs.find(pMainWnd);
        
        if (iter == boxs.end()) {
            return;
        }
        
        if (iter->second == 0) {
            return;
        }
        
        MessageWindow* msgBox = reinterpret_cast<MessageWindow*>(iter->second);
        msgBox->Close();
        return;
    }
    
    
    
    MessageWindow::MessageWindow(std::shared_ptr<NativeWindowSettings> pSettings)
        : LayeredWindow(pSettings) {
        m_pButtonOK = NULL;
        m_pButtonCancel = NULL;
        m_pLabelText = NULL;
        m_pEditPrompt = NULL;
        m_pStrPrompt = NULL;
        m_pLabelCaption = NULL;
    }
    
    MessageWindow::~MessageWindow() {
        return;
    }
    
    LPCTSTR MessageWindow::GetWindowClassName() const {
        return _T("MessageWindow");
    }
    
    
    void MessageWindow::OnFinalMessage(HWND hWnd) {
        auto iter = boxs.find(::GetParent(hWnd));
        
        if (iter != boxs.end()) {
            boxs.erase(iter);
        }
        
        WindowImplBase::OnFinalMessage(hWnd);
        delete this;
    }
    
    CDuiString MessageWindow::GetSkinFile() {
    
        return amo::u8string(skinMessageBox, true).format(
                   m_pNativeSettings->settings).to_unicode().c_str();
    }
    
    CDuiString MessageWindow::GetSkinFolder() {
        return  _T("");
    }
    
    
    
    LRESULT MessageWindow::OnSysCommand(UINT uMsg,
                                        WPARAM wParam,
                                        LPARAM lParam,
                                        BOOL& bHandled) {
                                        
        if (wParam == SC_CLOSE) {
            bHandled = TRUE;
            SendMessage(WM_CLOSE);
            return 0;
        }
        
#if defined(WIN32) && !defined(UNDER_CE)
        BOOL bZoomed = ::IsZoomed(m_hWnd);
        LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
        
#else
        return __super::OnSysCommand(uMsg, wParam, lParam, bHandled);
#endif
        
        return 0;
    }
    
    void MessageWindow::SetCenterWindow() {
        ASSERT(::IsWindow(m_hWnd));
        ASSERT((GetWindowStyle(m_hWnd)&WS_CHILD) == 0);
        RECT rcDlg = { 0 };
        ::GetWindowRect(m_hWnd, &rcDlg);
        RECT rcArea = { 0 };
        RECT rcCenter = { 0 };
        HWND hWnd = *this;
        HWND hWndParent = ::GetParent(m_hWnd);
        HWND hWndCenter = ::GetWindowOwner(m_hWnd);
        
        if (hWndCenter != NULL) {
            hWnd = hWndCenter;
        }
        
        // 处理多显示器模式下屏幕居中
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST),
                         &oMonitor);
        rcArea = oMonitor.rcWork;
        
        if (hWndCenter == NULL || IsIconic(hWndCenter)) {
            rcCenter = rcArea;
        } else {
            ::GetWindowRect(hWndCenter, &rcCenter);
        }
        
        int DlgWidth = rcDlg.right - rcDlg.left;
        int DlgHeight = rcDlg.bottom - rcDlg.top;
        
        // Find dialog's upper left based on rcCenter
        int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
        int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;
        
        int top = rcCenter.top + (rcCenter.bottom - rcCenter.top) * 2 / 7;
        
        if (yTop > top) {
            yTop = top;
        }
        
        // The dialog is outside the screen, move it inside
        if (xLeft < rcArea.left) {
            xLeft = rcArea.left;
        } else if (xLeft + DlgWidth > rcArea.right) {
            xLeft = rcArea.right - DlgWidth;
        }
        
        if (yTop < rcArea.top) {
            yTop = rcArea.top;
        } else if (yTop + DlgHeight > rcArea.bottom) {
            yTop = rcArea.bottom - DlgHeight;
        }
        
        
        ::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1,
                       SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
    
    LRESULT MessageWindow::ResponseDefaultKeyEvent(WPARAM wParam) {
        if (wParam == VK_RETURN) {
            Close(IDOK);
        } else if (wParam == VK_ESCAPE) {
            Close(IDCANCEL);
            return TRUE;
        }
        
        return TRUE;
    }
    
    
    void MessageWindow::InitWindow() {
        auto& pm = m_PaintManager;
        m_pButtonOK = static_cast<CButtonUI*>(pm.FindControl(_T("okbtn")));
        m_pButtonCancel = static_cast<CButtonUI*>(pm.FindControl(_T("cancelbtn")));
        m_pLabelText = static_cast<CLabelUI*>(pm.FindControl(_T("text")));
        m_pEditPrompt = static_cast<CEditUI*>(pm.FindControl(_T("prompt_edit")));
        m_pLabelCaption  = static_cast<CLabelUI*>(pm.FindControl(_T("caption")));
        ASSERT(m_pButtonOK != NULL);
        ASSERT(m_pButtonCancel != NULL);
        ASSERT(m_pLabelText != NULL);
        ASSERT(m_pEditPrompt != NULL);
        ASSERT(m_pLabelCaption != NULL);
        
        
        CDuiString hotbkcolor(amo::u8string(m_pNativeSettings->primaryColor,
                                            true).to_wide().c_str());
                                            
        m_pButtonOK->SetText(DuiLib::CDuiString(L"确定"));
        m_pButtonOK->SetAttribute(_T("hotbkcolor"),
                                  hotbkcolor.GetData());
        uint32_t color = m_pButtonOK->GetHotBkColor();
        //color &= 0x77ff00ff;
        m_pButtonOK->SetHotBkColor(color);
        //m_pButtonOK->SetHotBkColor(0xff0A67FB);
        
        m_pButtonCancel->SetText(DuiLib::CDuiString(L"取消"));
        m_pButtonCancel->SetAttribute(_T("hotbkcolor"),
                                      hotbkcolor.GetData());
        m_pButtonCancel->SetAttribute(_T("pushedtextcolor"), hotbkcolor.GetData());
        m_pButtonCancel->SetHotTextColor(0xffffffff);
        
        /*m_pButtonCancel->SetHotBkColor(m_pNativeSettings->primaryColor);
        m_pButtonCancel->SetPushedTextColor(m_pNativeSettings->primaryColor);*/
        
        
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
        
        SetCenterWindow();
    }
    
    
    void MessageWindow::Notify(TNotifyUI& msg) {
        if (_tcsicmp(msg.sType, _T("click")) == 0) {
            if (_tcsicmp(msg.pSender->GetName(), _T("okbtn")) == 0) {
                if (m_pEditPrompt != NULL && m_pStrPrompt != NULL) {
                    *m_pStrPrompt = m_pEditPrompt->GetText();
                }
                
                Close(IDOK);
                return;
                
            } else if (_tcsicmp(msg.pSender->GetName(), _T("cancelbtn")) == 0
                       || _tcsicmp(msg.pSender->GetName(), _T("closebtn")) == 0) {
                Close(IDCANCEL);
                return;
            }
            
        }
        
        return __super::Notify(msg);
        
    }
    
    
    
    void MessageWindow::setType(UINT uType) {
        if (uType == MB_OK) {
            if (m_pButtonCancel != NULL) {
                m_pButtonCancel->SetVisible(false);
            }
            
        } else if (uType == MB_OKCANCEL) {
            //保持默认xml配置
        } else {
            return ;
        }
    }
    
    void MessageWindow::setText(CDuiString msg) {
        auto& pm = m_PaintManager;
        CLabelUI* pTextControl = static_cast<CLabelUI*>(pm.FindControl(_T("text")));
        CContainerUI* pTextLayout
            = static_cast<CContainerUI*>(pm.FindControl(_T("TextLayout")));
            
        if (m_pEditPrompt != NULL && !m_pEditPrompt->IsVisible()) {
            pTextControl->SetFixedHeight(80);
        }
        
        //msg = _T("adsfads<n>adsf234<n>3485345<n> 在一在在d");
        if (pTextControl != NULL) {
            //在显示HTML的时候，字体设置将失效，只能使用默认字体
            if (m_pStrPrompt != NULL) {
                pTextControl->SetAttribute(_T("textcolor"), _T("#ff999999"));
                pTextControl->SetFont(3);
                pTextControl->SetShowHtml(false);
            } else {
                pTextControl->SetShowHtml(true);
            }
            
            pTextControl->SetText(msg.GetData());
        }
    }
    
    void MessageWindow::setCaption(CDuiString caption) {
        LPCTSTR lpCaption = caption.GetData();
        CLabelUI* pCaptionControl
            = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("caption")));
            
        if (pCaptionControl != NULL) {
            pCaptionControl->SetText(lpCaption);
        }
    }
    
    void MessageWindow::SetIcon(UINT uIcon) {
        CContainerUI* pTextLayout
            = static_cast<CContainerUI*>(m_PaintManager.FindControl(_T("TextLayout")));
            
        if (pTextLayout != NULL) {
            switch (uIcon) {
            case MB_ICONASTERISK: {
                break;
            }
            
            case MB_ICONQUESTION: {
                break;
            }
            
            case MB_ICONWARNING: {
                break;
            }
            
            case MB_ICONERROR: {
                break;
            }
            
            default:
                break;
            }
        }
    }
    
    void MessageWindow::setPrompt(CDuiString* prompt) {
        m_pStrPrompt = prompt;
        
        if (m_pLabelText != NULL) {
            m_pLabelText->SetVisible(false);
        }
        
        if (m_pEditPrompt != NULL && m_pStrPrompt != NULL) {
            m_pEditPrompt->SetVisible(true);
            m_pEditPrompt->SetText(*m_pStrPrompt);
        }
    }
    
}