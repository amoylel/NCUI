#include "stdafx.h"
#include "ui/win/clipboard/ClipboardMonitor.h"
#include "transfer/ClassTransfer.hpp"
#include "ipc/UIMessageBroadcaster.hpp"
#include "transfer/ClipboardTransfer.h"


namespace amo {

    ClipboardMonitor::ClipboardMonitor() {
        m_hNextClipboard = NULL;
        m_bWatching = false;
    }
    
    bool ClipboardMonitor::startWatch() {
        if (m_bWatching) {
            return true;
        }
        
        NotifyWindow* window = Tray::getInstance()->getNotifyWindow();
        window->setClipMsgCallback(std::bind(&ClipboardMonitor::HandleMessage,
                                             this,
                                             std::placeholders::_1,
                                             std::placeholders::_2,
                                             std::placeholders::_3,
                                             std::placeholders::_4));
        m_hNextClipboard = ::SetClipboardViewer(window->GetHWND());
        //GetLastError();
        m_bWatching = true;
        return m_bWatching;
    }
    
    bool ClipboardMonitor::stopWatch() {
        if (!m_bWatching) {
            return true;
        }
        
        NotifyWindow* window = Tray::getInstance()->getNotifyWindow();
        
        ChangeClipboardChain(window->GetHWND(), m_hNextClipboard);
        m_hNextClipboard = NULL;
        m_bWatching = false;
        return true;
    }
    
    bool ClipboardMonitor::isWatching() {
        return m_bWatching;
    }
    
    LRESULT ClipboardMonitor::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
                                            LPARAM lParam) {
        switch (uMsg) {
        case WM_CHANGECBCHAIN:
            OnChangeClipboardChain(hWnd, uMsg, wParam, lParam);
            break;
            
        case WM_DRAWCLIPBOARD:
            OnDrawClipboard(hWnd, uMsg, wParam, lParam);
            break;
            
        case WM_DESTROY:
            //stopWatch();
            ChangeClipboardChain(hWnd, m_hNextClipboard);
            m_bWatching = false;
            break;
            
        default:
            break;
        }
        
        return FALSE;
        
        
        
    }
    
    LRESULT ClipboardMonitor::OnChangeClipboardChain(HWND hWnd, UINT uMsg,
            WPARAM wParam, LPARAM lParam) {
        HWND hWndRemove = (HWND)wParam;
        HWND hWndAfter = (HWND)lParam;
        
        if (m_hNextClipboard = hWndRemove) {
            m_hNextClipboard = hWndAfter;
        } else if (m_hNextClipboard != NULL) {
            ::SendMessage(m_hNextClipboard, WM_CHANGECBCHAIN, (WPARAM)hWndRemove,
                          (LPARAM)hWndAfter);
        }
        
        return FALSE;
    }
    
    LRESULT ClipboardMonitor::OnDrawClipboard(HWND hWnd, UINT uMsg, WPARAM wParam,
            LPARAM lParam) {
        Clipboard clipboard;
        int32_t effect =  clipboard.dropEffect();
        
        
        //#define	DROPEFFECT_NONE	( 0 )
        //
        //#define	DROPEFFECT_COPY	( 1 )
        //
        //#define	DROPEFFECT_MOVE	( 2 )
        //
        //#define	DROPEFFECT_LINK	( 4 )
        
        std::string eventName = "copy";
        
        switch (effect) {
        case DROPEFFECT_COPY:
            eventName = "copy";
            break;
            
        case DROPEFFECT_MOVE:
            eventName = "cut";
            break;
            
        case DROPEFFECT_LINK:
            eventName = "link";
            break;
            
        case DROPEFFECT_NONE:
        default:
            eventName = "copy";
            break;
        }
        
        if (clipboard.isEmpty()) {
            eventName = "empty";
        }
        
        auto transfer = ClassTransfer::getUniqueTransfer<ClipboardTransfer>();
        UIMessageBroadcaster broadcaster(transfer->getObjectID());
        broadcaster.broadcast(eventName);
        return FALSE;
    }
    
}


