#include "stdafx.h"
#include "transfer/BrowserHostTransfer.h"
#include "transfer/BrowserTransfer.h"

namespace amo {

    BrowserHostTransfer::BrowserHostTransfer()
        : ClassTransfer("BrowserHost") {
        
    }
    
    BrowserHostTransfer::BrowserHostTransfer(CefRefPtr<CefBrowserHost> pBrowserHost)
        : ClassTransfer("BrowserHost")
        , m_pBrowserHost(pBrowserHost) {
        
    }
    
    Any BrowserHostTransfer::current(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        int nBrowserID = args->getInt(IPCArgsPosInfo::BrowserID);
        int64_t nFrameID = args->getInt64(IPCArgsPosInfo::FrameID);
        CefRefPtr<CefFrame> pFrame = BrowserManager<PID_BROWSER>::GetFrameByID(
                                         nFrameID);
                                         
        if (!pFrame) {
            return Undefined();
        }
        
        CefRefPtr<CefBrowser> pBrowser = pFrame->GetBrowser();
        
        if (!pBrowser) {
            return Undefined();
        }
        
        CefRefPtr<CefBrowserHost> pHost = pBrowser->GetHost();
        
        if (!pHost) {
            return Undefined();
        }
        
        auto manager = TransferMappingMgr<BrowserHostTransfer>::getInstance();
        return manager->toSimplifiedJson(pHost);
        
    }
    
    Any BrowserHostTransfer::click(IPCMessage::SmartType msg) {
        return SendMouseClickEvent(msg);
    }
    
    Any BrowserHostTransfer::GetBrowser(IPCMessage::SmartType msg) {
        CefRefPtr<CefBrowser> pBrowser = m_pBrowserHost->GetBrowser();
        
        if (!pBrowser) {
            return Undefined();
        }
        
        auto manager = TransferMappingMgr<BrowserTransfer>::getInstance();
        return manager->toSimplifiedJson(pBrowser);
    }
    
    Any BrowserHostTransfer::CloseBrowser(IPCMessage::SmartType msg) {
        bool bForce = msg->getArgumentList()->getBool(0);
        m_pBrowserHost->CloseBrowser(bForce);
        return Undefined();
    }
    
    Any BrowserHostTransfer::SetFocus(IPCMessage::SmartType msg) {
        bool bFocus = msg->getArgumentList()->getBool(0);
        m_pBrowserHost->SetFocus(bFocus);
        return Undefined();
    }
    
    Any BrowserHostTransfer::SetWindowVisibility(IPCMessage::SmartType msg) {
        bool bVal = msg->getArgumentList()->getBool(0);
        
        return Undefined();
    }
    
    Any BrowserHostTransfer::GetWindowHandle(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::GetOpenerWindowHandle(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::GetClient(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::GetRequestContext(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::GetZoomLevel(IPCMessage::SmartType msg) {
        return m_pBrowserHost->GetZoomLevel();
        return Undefined();
    }
    
    Any BrowserHostTransfer::SetZoomLevel(IPCMessage::SmartType msg) {
        double fVal = msg->getArgumentList()->getDouble(0);
        m_pBrowserHost->SetZoomLevel(fVal);
        return Undefined();
    }
    
    Any BrowserHostTransfer::RunFileDialog(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::StartDownload(IPCMessage::SmartType msg) {
        std::string strUrl = msg->getArgumentList()->getString(0);
        m_pBrowserHost->StartDownload(strUrl);
        return Undefined();
    }
    
    Any BrowserHostTransfer::Print(IPCMessage::SmartType msg) {
        m_pBrowserHost->Print();
        return Undefined();
    }
    
    Any BrowserHostTransfer::PrintToPDF(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::Find(IPCMessage::SmartType msg) {
    
        return Undefined();
    }
    
    Any BrowserHostTransfer::StopFinding(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::ShowDevTools(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::CloseDevTools(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::GetNavigationEntries(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::SetMouseCursorChangeDisabled(IPCMessage::SmartType
            msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::IsMouseCursorChangeDisabled(IPCMessage::SmartType
            msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::ReplaceMisspelling(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::AddWordToDictionary(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::NotifyScreenInfoChanged(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::Invalidate(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::SendKeyEvent(IPCMessage::SmartType msg) {
        //m_pBrowserHost->SendKeyEvent()
        return Undefined();
    }
    
    Any BrowserHostTransfer::SendMouseClickEvent(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        int x = args->getInt(0);
        int y = args->getInt(1);
        CefMouseEvent mouse_event;
        mouse_event.x = (int)x;
        mouse_event.y = (int)y;
        mouse_event.modifiers = 16;
        m_pBrowserHost->SendMouseClickEvent(mouse_event, MBT_LEFT, false, 1);
        
        //Sleep(5);
        /*mouse_event.x = (int)x;
        mouse_event.y = (int)y;
        mouse_event.modifiers = 16;*/
        m_pBrowserHost->SendMouseClickEvent(mouse_event, MBT_LEFT, true, 1);
        return Undefined();
    }
    
    Any BrowserHostTransfer::SendMouseMoveEvent(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::SendMouseWheelEvent(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::SendFocusEvent(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::SendCaptureLostEvent(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::NotifyMoveOrResizeStarted(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::GetWindowlessFrameRate(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::SetWindowlessFrameRate(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::HandleKeyEventBeforeTextInputClient(
        IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::HandleKeyEventAfterTextInputClient(
        IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::IsWindowRenderingDisabled(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::WasResized(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::WasHidden(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::GetNSTextInputContext(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::DragTargetDragEnter(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::DragTargetDragOver(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::DragTargetDragLeave(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::DragTargetDrop(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::DragSourceEndedAt(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any BrowserHostTransfer::DragSourceSystemDragEnded(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
}

