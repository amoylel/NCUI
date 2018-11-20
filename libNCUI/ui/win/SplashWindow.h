// Created by amoylel on 10/06/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_SPLASHWINDOW_H__
#define AMO_SPLASHWINDOW_H__

#include "ui/win/LayeredWindow.h"

namespace amo {
    class SplashWindowSettings;
    class SplashWindow : public NativeWindow {
    public:
        SplashWindow(std::shared_ptr<SplashWindowSettings> pSplashSettings);
        virtual CDuiString GetSkinFolder() override;
        virtual CDuiString GetSkinFile() override;
        virtual LPCTSTR GetWindowClassName(void) const override;
        
        
        virtual CControlUI* CreateControl(LPCTSTR pstrClass) override;
        virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
        
        virtual void InitWindow() override;
        
        
        void showPictureView();
        
        
        virtual void OnFinalMessage(HWND hWnd) override;
        
        std::shared_ptr<amo::SplashWindowSettings> getSplashSettings() const;
        void setSplashSettings(std::shared_ptr<amo::SplashWindowSettings> val);
        
    private:
        std::shared_ptr<SplashWindowSettings> m_pSplashSettings;
        
        PictureView* m_pPictureView;
    };
}

#endif // AMO_SPLASHWINDOW_H__

