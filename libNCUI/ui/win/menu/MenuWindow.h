// Created by amoylel on 06/29/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_MENUWINDOW_H__
#define AMO_MENUWINDOW_H__


#include <memory>


#include "ui/win/LayeredWindow.h"
#include "ui/win/menu/ObserverImplBase.hpp"
#include <amo/json.hpp>
namespace amo {

    class CMenuUI;
    class MenuElementUI;
    class MenuWindowSettings;
    class IMM32Manager;
    /////////////////////////////////////////////////////////////////////////////////////
    //
    struct ContextMenuParam {
        // 1: remove all
        // 2: remove the sub menu
        // 3: 菜单点击事件
        // 4: 窗口关闭事件 WM_CLOSE
        WPARAM wParam;
        HWND hWnd;
        CDuiString text;
        void* data;
        ContextMenuParam() {
            wParam = 0;
            hWnd = NULL;
            data = NULL;
        }
    };
    
    
    
    typedef class ObserverImpl<BOOL, ContextMenuParam> ContextMenuObserver;
    typedef class ReceiverImpl<BOOL, ContextMenuParam> ContextMenuReceiver;
    
    //extern ContextMenuObserver s_context_menu_observer;
    
    class MenuWindow : public LayeredWindow, public ContextMenuReceiver {
    public:
        MenuWindow(std::shared_ptr<MenuWindowSettings> pBrowserSettings,
                   HWND hParent = NULL);
        void Init(MenuElementUI* pOwner, POINT point);
        LPCTSTR GetWindowClassName() const override;
        virtual CControlUI* CreateControl(LPCTSTR pstrClass) override;
        virtual void OnFinalMessage(HWND hWnd) override;
        virtual void InitWindow() override;
        virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                            BOOL& bHandled) override;
        virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                 BOOL& bHandled) override;
        virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL& bHandled) override;
        virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL& bHandled) override;
        virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL& bHandled) override;
        virtual CDuiString GetSkinFolder() override;
        virtual CDuiString GetSkinFile() override;
        
        std::string ParseSkinFile(amo::u8json& menu);
        
        
        DWORD GetBkColor() const;
        void SetBkColor(DWORD dwBackColor);
        
        LPCTSTR GetBkImage();
        void SetBkImage(LPCTSTR pStrImage);
        
        
        virtual BOOL Receive(ContextMenuParam param) override;
    protected:
        LRESULT CreateMainMenuWindow(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                     BOOL& bHandled);
        LRESULT CreateSubMenuWindow(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL& bHandled);
    public:
        //ShadowWindow m_Shadow;
        HWND m_hParent;
        POINT m_BasedPoint;
        CDuiString m_sType;
        MenuElementUI* m_pOwner;
        CMenuUI* m_pLayout;
        
        std::shared_ptr<MenuWindowSettings> m_pMenuSettings;
        std::shared_ptr<IMM32Manager> m_pIMM32;
    public:
    };
    
    
    
    
}

#endif // AMO_MENUWINDOW_H__
