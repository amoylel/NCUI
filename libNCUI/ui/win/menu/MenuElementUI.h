// Created by amoylel on 06/29/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_MENUELEMENTUI_H__
#define AMO_MENUELEMENTUI_H__

#include <memory>



namespace amo {
    class MenuWindow;
    class MenuWindowSettings;
    
    extern const TCHAR* const kMenuElementUIClassName;
    extern const TCHAR* const kMenuElementUIInterfaceName;
    
    class MenuElementUI : public CListContainerElementUI {
        friend MenuWindow;
    public:
        MenuElementUI(std::shared_ptr<MenuWindowSettings> pSettings);
        ~MenuElementUI();
        
        LPCTSTR GetClass() const;
        LPVOID GetInterface(LPCTSTR pstrName);
        
        bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
        
        void DrawItemText(HDC hDC, const RECT& rcItem);
        
        SIZE EstimateSize(SIZE szAvailable);
        
        bool Activate();
        
        void DoEvent(TEventUI& event);
        
        
        MenuWindow* GetMenuWnd();
        void SetMenuWnd(MenuWindow* wnd);
        
        UINT GetButtonState() const;
        void SetButtonState(UINT state);
        
        void CreateMenuWnd();
        virtual CDuiString GetAttribute(LPCTSTR pstrName) override;
        virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) override;
    protected:
        MenuWindow* m_pWindow;
        std::shared_ptr<MenuWindowSettings> m_pMenuWindowSettings;
        CDuiString m_strID;
        CDuiString m_strAction;
		CDuiString m_strText;
    };
}

#endif // AMO_MENUELEMENTUI_H__
