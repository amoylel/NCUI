// Created by amoylel on 08/28/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_CONTEXTMENUWINDOW_H__
#define AMO_CONTEXTMENUWINDOW_H__






#include "ui/win/menu/MenuWindow.h"
#include "transfer/MenuTransfer.h"



namespace amo {
    class MenuWindowSettings;
    
    class ContextMenuWindow
        : public MenuWindow
        , public MenuTransfer {
    public:
        ContextMenuWindow(std::shared_ptr<MenuWindowSettings> pBrowserSettings);
        ~ContextMenuWindow();
        
        virtual void OnFinalMessage(HWND hWnd) override;
        
        virtual BOOL Receive(ContextMenuParam param) override;
        
        bool onBeforeCallback(const amo::u8string& action);
        
        int getBrowserID() const;
        void setBrowserID(int val);
        
        int64_t getFrameID() const;
        void setFrameID(int64_t val);
        
        
    protected:
        int m_nBrowserID;
        int64_t m_nFrameID;
        bool m_bClosed;
        int64_t m_nObjectID;
        
    };
}



#endif // AMO_CONTEXTMENUWINDOW_H__