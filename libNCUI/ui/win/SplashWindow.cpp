#include "stdafx.h"
#include "ui/win/SplashWindow.h"
#include "ui/win/BrowserWindowSkin.h"
#include "settings/SplashWindowSettings.h"
#include <functional>



namespace amo {



    SplashWindow::SplashWindow(std::shared_ptr<SplashWindowSettings>
                               pSplashSettings)
        : NativeWindow(pSplashSettings) {
        setSplashSettings(pSplashSettings);
    }
    
    DuiLib::CDuiString SplashWindow::GetSkinFolder() {
        return _T("");
    }
    
    DuiLib::CDuiString SplashWindow::GetSkinFile() {
        return amo::u8string(skinSplashWindow, true).format(
                   getSplashSettings()->settings).to_unicode().c_str();
    }
    
    LPCTSTR SplashWindow::GetWindowClassName(void) const {
    
        return _T("SplashWindow");
    }
    
    DuiLib::CControlUI* SplashWindow::CreateControl(LPCTSTR pstrClass) {
        if (_tcsicmp(pstrClass, _T("PictureView")) == 0) {
            return new PictureView();
        }
        
        return NULL;
    }
    
    void foo() {
    
    }
    LRESULT SplashWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL& bHandled) {
                                  
        LRESULT lRes = NativeWindow::OnPaint(uMsg, wParam, lParam, bHandled);
        runOnUIThread(std::bind(&SplashWindow::showPictureView, this));
        
        return lRes;
    }
    
    void SplashWindow::InitWindow() {
    
        NativeWindow::InitWindow();
        m_pPictureView = static_cast<PictureView*>(
                             m_PaintManager.FindControl(_T("pictureView")));
        ASSERT(m_pPictureView);
        m_pPictureView->SetVisible(false);
        RECT rc = {0};
        ::GetClientRect(m_hWnd, &rc);
        std::string imagePath = AppSettings::getInstance()->toAbsolutePath(
                                    getSplashSettings()->image);
        amo::u8string file(imagePath, true);
        m_pPictureView->setPicture(file.to_wide());
        setTopmost(true);
    }
    
    
    void SplashWindow::showPictureView() {
    
        if (!m_pPictureView->IsVisible()) {
            m_pPictureView->SetVisible(true);
        }
        
    }
    
    void SplashWindow::OnFinalMessage(HWND hWnd) {
        NativeWindow::OnFinalMessage(hWnd);
        // 删除
        delete this;
    }
    
    std::shared_ptr<amo::SplashWindowSettings> SplashWindow::getSplashSettings()
    const {
        return m_pSplashSettings;
    }
    
    void SplashWindow::setSplashSettings(std::shared_ptr<amo::SplashWindowSettings>
                                         val) {
        m_pSplashSettings = val;
    }
    
}
