#include "stdafx.h"
#include "ui/win/menu/MenuUI.h"
#include "ui/win/menu/MenuElementUI.h"

namespace amo {

    const TCHAR* const kMenuUIClassName = _T("MenuUI");
    const TCHAR* const kMenuUIInterfaceName = _T("Menu");
    
    CMenuUI::CMenuUI() {
        if (GetHeader() != NULL) {
            GetHeader()->SetVisible(false);
        }
        
        this->SetAttribute(_T("bkcolor"), _T("#ffffffff"));
        this->SetAttribute(_T("itemselectedbkcolor"), _T("#88d8d8d8"));
        this->SetAttribute(_T("inset"), _T("1,1,1,1"));
        this->SetAttribute(_T("itemtextpadding"), _T("20,0,20,0"));
        this->SetAttribute(_T("bordercolor"), _T("#ffc4c4c4"));
        this->SetAttribute(_T("bordersize"), _T("1"));
    }
    
    CMenuUI::~CMenuUI()
    {}
    
    LPCTSTR CMenuUI::GetClass() const {
        return kMenuUIClassName;
    }
    
    LPVOID CMenuUI::GetInterface(LPCTSTR pstrName) {
        if (_tcsicmp(pstrName, kMenuUIInterfaceName) == 0) {
            return static_cast<CMenuUI*>(this);
        }
        
        return CListUI::GetInterface(pstrName);
    }
    
    void CMenuUI::DoEvent(TEventUI& event) {
        return __super::DoEvent(event);
    }
    
    bool CMenuUI::Add(CControlUI* pControl) {
        MenuElementUI* pMenuItem = static_cast<MenuElementUI*>(
			pControl->GetInterface(kMenuElementUIInterfaceName));
        
        if (pMenuItem == NULL) {
            return false;
        }
        
        for (int i = 0; i < pMenuItem->GetCount(); ++i) {
            if (pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL) {
                (static_cast<MenuElementUI*>(pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(false);
            }
        }
        
        return CListUI::Add(pControl);
    }
    
    bool CMenuUI::AddAt(CControlUI* pControl, int iIndex) {
        MenuElementUI* pMenuItem = static_cast<MenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
        
        if (pMenuItem == NULL) {
            return false;
        }
        
        for (int i = 0; i < pMenuItem->GetCount(); ++i) {
            if (pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName) != NULL) {
                (static_cast<MenuElementUI*>(pMenuItem->GetItemAt(i)->GetInterface(kMenuElementUIInterfaceName)))->SetInternVisible(false);
            }
        }
        
        return CListUI::AddAt(pControl, iIndex);
    }
    
    int CMenuUI::GetItemIndex(CControlUI* pControl) const {
        MenuElementUI* pMenuItem = static_cast<MenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
        
        if (pMenuItem == NULL) {
            return -1;
        }
        
        return __super::GetItemIndex(pControl);
    }
    
    bool CMenuUI::SetItemIndex(CControlUI* pControl, int iIndex) {
        MenuElementUI* pMenuItem = static_cast<MenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
        
        if (pMenuItem == NULL) {
            return false;
        }
        
        return __super::SetItemIndex(pControl, iIndex);
    }
    
    bool CMenuUI::Remove(CControlUI* pControl, bool bDoNotDestroy) {
        MenuElementUI* pMenuItem = static_cast<MenuElementUI*>(pControl->GetInterface(kMenuElementUIInterfaceName));
        
        if (pMenuItem == NULL) {
            return false;
        }
        
        return __super::Remove(pControl, bDoNotDestroy);
    }
    
    SIZE CMenuUI::EstimateSize(SIZE szAvailable) {
        int cxFixed = 0;
        int cyFixed = 0;
        
        for (int it = 0; it < GetCount(); it++) {
            CControlUI* pControl = static_cast<CControlUI*>(GetItemAt(it));
            
            if (!pControl->IsVisible()) {
                continue;
            }
            
            SIZE sz = pControl->EstimateSize(szAvailable);
            cyFixed += sz.cy;
            
            if (cxFixed < sz.cx) {
                cxFixed = sz.cx;
            }
        }
        
        return CDuiSize(cxFixed, cyFixed);
    }
    
    void CMenuUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) {
        CListUI::SetAttribute(pstrName, pstrValue);
    }
}

