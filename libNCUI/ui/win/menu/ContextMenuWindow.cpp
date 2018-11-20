#include "stdafx.h"
#include "ui/win/menu/ContextMenuWindow.h"

#include <functional>

#include <amo/string.hpp>

#include "ipc/UIMessageEmitter.hpp"
#include "ui/win/menu/MenuElementUI.h"

#include "handler/BrowserManager.hpp"
#include "settings/MenuWindowSettings.h"
#include "transfer/ClassTransfer.hpp"
#include "transfer/AppTransfer.h"
#include "ui/win/ActionPreprocessor.h"


namespace amo {



    ContextMenuWindow::ContextMenuWindow(std::shared_ptr<MenuWindowSettings>
                                         pBrowserSettings)
        : MenuWindow(pBrowserSettings)
        , m_nBrowserID(0)
        , m_nFrameID(0) {
        m_bClosed = false;
        m_nObjectID = getObjectID();
    }
    
    ContextMenuWindow::~ContextMenuWindow() {
    
    }
    
    void ContextMenuWindow::OnFinalMessage(HWND hWnd) {
        RemoveObserver();
        
        if (m_pOwner != NULL) {
            for (int i = 0; i < m_pOwner->GetCount(); i++) {
                if (static_cast<MenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(
                                                    kMenuElementUIInterfaceName)) != NULL) {
                    (static_cast<MenuElementUI*>(m_pOwner->GetItemAt(i)))->SetOwner(
                        m_pOwner->GetParent());
                    (static_cast<MenuElementUI*>(m_pOwner->GetItemAt(i)))->SetVisible(false);
                    (static_cast<MenuElementUI*>(m_pOwner->GetItemAt(i)->GetInterface(
                                                     kMenuElementUIInterfaceName)))->SetInternVisible(false);
                }
            }
            
            m_pOwner->SetMenuWnd(NULL);
            UINT state = m_pOwner->GetButtonState();
            state &= ~UISTATE_PUSHED;
            m_pOwner->SetButtonState(state);
            m_pOwner->Invalidate();
        }
        
        LayeredWindow::OnFinalMessage(hWnd);
        //removeTransfer(m_nObjectID);
    }
    
    BOOL ContextMenuWindow::Receive(ContextMenuParam param) {
        BOOL result = MenuWindow::Receive(param);
        
        CefRefPtr<CefFrame> pFrame;
        pFrame = BrowserManager<PID_BROWSER>::GetFrameByID(m_nFrameID);
        
        /*  if (!pFrame) {
              return result;
          }*/
        
        std::shared_ptr<UIMessageEmitter> runner(new UIMessageEmitter(pFrame));
        runner->setValue(IPCArgsPosInfo::TransferName, "ipcRenderer");
        runner->setValue(IPCArgsPosInfo::EventObjectID, getObjectID());
        
        if (param.wParam == 3) {
        
        
            MenuElementUI* element = reinterpret_cast<MenuElementUI*>(param.data);
            
            if (!element || !element->IsEnabled()) {
                return true;
            }
            
            amo::u8string id(element->GetAttribute(_T("id")));
            amo::u8string action(element->GetAttribute(_T("action")));
            amo::u8string text(element->GetAttribute(_T("string")));
            
            if (onBeforeCallback(action)) {
                return result;
            }
            
            amo::u8json json;
            json.put("id", id.to_utf8());
            json.put("text", text.to_utf8());
            json.put("action", action.to_utf8());
            
            runner->execute<std::string, amo::u8json>("triggerEvent",
                    "select",
                    json);
                    
        } else if ((param.wParam == 4) && !m_bClosed && param.hWnd == m_hWnd) {
        
            m_bClosed = true;
            runner->execute<std::string>("triggerEvent", "close");
        }
        
        return result;
    }
    
    bool ContextMenuWindow::onBeforeCallback(const amo::u8string& action) {
    
        ActionPreprocessor processor(m_nBrowserID, m_nFrameID);
        return processor.process(action);
    }
    
    int ContextMenuWindow::getBrowserID() const {
        return m_nBrowserID;
    }
    
    void ContextMenuWindow::setBrowserID(int val) {
        m_nBrowserID = val;
    }
    
    int64_t ContextMenuWindow::getFrameID() const {
        return m_nFrameID;
    }
    
    void ContextMenuWindow::setFrameID(int64_t val) {
        m_nFrameID = val;
    }
    
    
}

