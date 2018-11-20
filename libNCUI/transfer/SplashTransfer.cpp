#include "stdafx.h"
#include "transfer/SplashTransfer.h"
#include "ui/win/SplashWindow.h"
#include "settings/SplashWindowSettings.h"



namespace amo {

    VOID CALLBACK SplashWindowTimerProc(HWND hwnd,
                                        UINT uMsg,
                                        UINT_PTR idEvent,
                                        DWORD dwTime) {
                                        
        ClassTransfer::getUniqueTransfer<SplashTransfer>()->hide(
            IPCMessage::Empty());
    }
    
    VOID CALLBACK CloseSplashWindowTimerProc(HWND hwnd,
            UINT uMsg,
            UINT_PTR idEvent,
            DWORD dwTime) {
        ClassTransfer::getUniqueTransfer<SplashTransfer>()->fadeout();
    }
    
    SplashTransfer::SplashTransfer()
        : ClassTransfer("splash") {
        m_splashTimer = 0;
        m_pSplashWindow = NULL;
        m_nDelay = 0;
        m_nfadeTimes = 0;
    }
    
    SplashTransfer::~SplashTransfer() {
        closeSplash();
    }
    
    void SplashTransfer::closeSplash(int nDelay) {
        if (m_pSplashWindow == NULL) {
            return;
        }
        
        if (nDelay <= 0) {
            if (m_splashTimer != 0) {
                ::KillTimer(m_pSplashWindow->GetHWND(), m_splashTimer);
                m_splashTimer = 0;
            }
            
            m_nfadeTimes = 0;
            m_nDelay = 0;
            ::PostMessage(m_pSplashWindow->GetHWND(), WM_CLOSE, NULL, NULL);
            m_pSplashWindow = NULL;
            
        } else {
        
            if (m_nDelay == 0 && m_nfadeTimes == 0) {
                if (m_splashTimer != 0) {
                    ::KillTimer(m_pSplashWindow->GetHWND(), m_splashTimer);
                    m_splashTimer = 0;
                }
                
                m_nDelay = nDelay;
                m_nfadeTimes = 16;
                fadeout();
            }
        }
        
    }
    
    void SplashTransfer::fadeout() {
        --m_nfadeTimes;
        
        if (m_nfadeTimes == 0) {
            m_nDelay = 0;
            closeSplash(0);
        } else {
            m_pSplashWindow->setTransparent(m_nfadeTimes * 16);
            m_splashTimer = ::SetTimer(m_pSplashWindow->GetHWND(),
                                       0x00000502,
                                       m_nDelay / 16,
                                       (TIMERPROC)CloseSplashWindowTimerProc);
        }
        
    }
    
    Any SplashTransfer::show(IPCMessage::SmartType msg) {
    
        closeSplash();
        std::shared_ptr<SplashWindowSettings> pSettings(new SplashWindowSettings());
        Any& val = msg->getArgumentList()->getValue(0);
        
        if (val.type() == AnyValueType<amo::u8json>::value) {
            amo::u8json json = val;
            pSettings->updateArgsSettings(json);
            create(pSettings);
        }
        
        return Undefined();
    }
    
    Any SplashTransfer::hide(IPCMessage::SmartType msg) {
        Any& val = msg->getArgumentList()->getValue(0);
        
        if (val.type() == AnyValueType<int>::value) {
            closeSplash(msg->getArgumentList()->getInt(0));
        } else if (m_pSplashWindow != NULL) {
            closeSplash(m_pSplashWindow->getSplashSettings()->fadeout);
        }
        
        return Undefined();
    }
    
    Any SplashTransfer::isVisible(IPCMessage::SmartType msg) {
        if (!m_pSplashWindow) {
            return false;
        }
        
        return m_pSplashWindow->isVisible(msg);
    }
    
    void SplashTransfer::create(std::shared_ptr<SplashWindowSettings> pSettings) {
        m_pSplashWindow = new SplashWindow(pSettings);
        m_pSplashWindow->Create(NULL,
                                _T(""),
                                UI_WNDSTYLE_FRAME,
                                WS_EX_TOOLWINDOW,
                                0, 0, 0, 0);
                                
        if (pSettings->show) {
            //m_pSplashWindow->setPosition();
            m_pSplashWindow->ShowWindow(true);
        }
        
        if (pSettings->duration > 0) {
            m_splashTimer =::SetTimer(m_pSplashWindow->GetHWND(),
                                      0x00000502,
                                      pSettings->duration,
                                      (TIMERPROC)SplashWindowTimerProc);
        }
    }
    
}