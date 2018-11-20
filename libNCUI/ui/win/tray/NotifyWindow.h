// Created by amoylel on 07/01/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NOTIFYWINDOW_H__
#define AMO_NOTIFYWINDOW_H__

#include <functional>
#include <map>
#include "ui/win/LayeredWindow.h"



#define BUFFERSIZE 512
#define BIGBUFFERSIZE 4086
namespace amo {

    class BrowserWindowSettings;
    
    class NotifyWindow : public LayeredWindow {
    public:
        NotifyWindow(std::shared_ptr<BrowserWindowSettings> pBrowserSettings);
        ~NotifyWindow();
        
        virtual CDuiString GetSkinFolder() override;
        virtual CDuiString GetSkinFile() override;
        virtual LPCTSTR GetWindowClassName(void) const override;
        virtual void InitWindow()override;;
        virtual void OnFinalMessage(HWND hWnd)override;
        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
        virtual LRESULT OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override;
        
        std::function<LRESULT(INT, WPARAM, LPARAM)> getTrayMessageCallback() const;
        void setTrayMsgCallback(std::function<LRESULT(INT, WPARAM, LPARAM)> val);
        
        
        std::function<LRESULT(HWND, INT, WPARAM, LPARAM)> getClipMessageCallback() const;
        void setClipMsgCallback(std::function<LRESULT(HWND, INT, WPARAM, LPARAM)> val);
        
        
        
        
        
        
    private:
        /*! @brief	关闭按钮. */
        CButtonUI* m_pBtnMsgBoxClose;
        /*! @brief	忽略全部. */
        CButtonUI* m_pBtnIgnore;
        /*! @brief	查看全部. */
        CButtonUI* m_pBtnCheck;
        
        bool m_bIsMouseInFrame;
        
        /*! @brief	消息框宽. */
        const static uint32 BOXFRAMEWIDTH = 220;
        /*! @brief	消息框高. */
        const static uint32 BOXFRAMEHEIGHT = 110;
        /*! @brief	屏幕右间隔. */
        const static uint32 SCREENRIGHTINTERVAL = 10;
        /*! @brief	屏幕底部间隔. */
        const static uint32 SCREENBOTTONINTERVAL = 10;
        
        /*! @brief	托盘消息回调函数. */
        std::function<LRESULT(INT, WPARAM, LPARAM)> m_fnTrayMessageCallback;
        /*! @brief	The clip message callback. */
        std::function<LRESULT(HWND, INT, WPARAM, LPARAM)> m_fnClipMessageCallback;
        
        
    };
    
}

#endif // AMO_NOTIFYWINDOW_H__