// Created by amoylel on 06/30/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NATIVEWINDOW_H__
#define AMO_NATIVEWINDOW_H__
#include "ui/win/LayeredWindow.h"
#include "transfer/NativeWindowTransfer.h"
#include "ui/win/LocalWindow.h"

namespace amo {
    class NativeWindowSettings;
    class NativeWindow : public LocalWindow, virtual public NativeWindowTransfer {
    public:
        NativeWindow(std::shared_ptr<NativeWindowSettings> pBrowserSettings);
        ~NativeWindow();
        virtual CDuiString GetSkinFolder() override;
        virtual CDuiString GetSkinFile() override;
        virtual void OnFinalMessage(HWND hWnd) override;
        virtual LPCTSTR GetWindowClassName(void) const override;
        
        int64_t getFrameID() const;
        void setFrameID(int64_t val);
        
        int getBrowserID() const;
        void setBrowserID(int val);
        
        virtual std::shared_ptr<NativeWindow> toNativeWindow() override;
        
        
        virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL& bHandled) override;
                                  
    private:
    
        int m_nBrowserID;
        int64_t m_nFrameID;
        
        
    };
}

#endif // AMO_NATIVEWINDOW_H__
