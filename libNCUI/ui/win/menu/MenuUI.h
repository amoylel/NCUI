// Created by amoylel on 06/29/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_MENUUI_H__
#define AMO_MENUUI_H__

namespace amo {
    extern const TCHAR* const kMenuUIClassName ;
    extern const TCHAR* const kMenuUIInterfaceName ;
    class CMenuUI : public CListUI {
    public:
        CMenuUI();
        ~CMenuUI();
        
        LPCTSTR GetClass() const;
        LPVOID GetInterface(LPCTSTR pstrName);
        
        virtual void DoEvent(TEventUI& event);
        
        virtual bool Add(CControlUI* pControl);
        virtual bool AddAt(CControlUI* pControl, int iIndex);
        
        virtual int GetItemIndex(CControlUI* pControl) const;
        virtual bool SetItemIndex(CControlUI* pControl, int iIndex);
        virtual bool Remove(CControlUI* pControl, bool bDoNotDestroy = false);
        
        SIZE EstimateSize(SIZE szAvailable);
        
        void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    };
}

#endif // AMO_MENUUI_H__
