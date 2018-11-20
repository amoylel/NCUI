#include "stdafx.h"
#include "ui/win//LocalWindow.h"
#include "settings/NativeWindowSettings.h"
#include "ipc/Any.hpp"
#include "BrowserWindow.h"
#include "NativeWindow.h"
#include "transfer/ControlTransfer.h"

namespace amo {



    LocalWindow::LocalWindow(std::shared_ptr<NativeWindowSettings> pBrowserSettings)
        : LayeredWindow(pBrowserSettings)
        , m_nTransferClassID(0)
        , m_nTransferObjectID(0)
        , m_bIPCReady(false) {
        
    }
    
    void LocalWindow::InitWindow() {
    
        ::PostMessage(m_hWnd, WM_WINDOW_CREATED, NULL, NULL);
        return LayeredWindow::InitWindow();
    }
    
    LRESULT LocalWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL& bHandled) {
                                   
                                   
        // 移除映射
        for (auto& p : m_oTransferedSet) {
            auto manager = TransferMappingMgr<ControlTransfer>::getInstance();
            manager->removeMapping(p);
        }
        
        m_oTransferedSet.clear();
        
        std::shared_ptr<ClassTransfer> pTransfer;
        pTransfer = ClassTransfer::findTransfer(getTransferObjectID());
        std::shared_ptr<UIMessageBroadcaster> runner;
        runner.reset(new UIMessageBroadcaster(getTransferClassID()));
        
        if (pTransfer) {
        
            runner->broadcast("closed",
                              pTransfer->getFuncMgr().toSimplifiedJson());
        }
        
        return LayeredWindow::OnDestroy(uMsg, wParam, lParam, bHandled);
    }
    
    LRESULT LocalWindow::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                      BOOL& bHandled) {
        //$cdevel("LPARAM:0x{}", amo::string::from_number((int)wParam, 16).str());
        
        // 0xf014 == SC_
        // 禁止改变大小
        if (!m_pNativeSettings->resizable && (wParam == SC_MAXIMIZE)) {
            return TRUE;
        }
        
        // 禁止最小化
        if (!m_pNativeSettings->minimizable
                && (wParam == SC_MINIMIZE || wParam == 0xf022)) {
            return TRUE;
        }
        
        // 如果设置了最大宽高，那么不允许窗口最大化
        bool hasSetMaximize =
            (m_PaintManager.GetMaxInfo().cx > 0 || m_PaintManager.GetMaxInfo().cy > 0);
            
        // 禁止最大化
        if ((!m_pNativeSettings->maximizable || hasSetMaximize) &&
                (wParam == SC_MAXIMIZE || wParam == 0xf032)) {
            // || wParam == 0xf014
            // 0xf032 == SC_MAXIMIZE2
            return TRUE;
        }
        
        if (wParam == SC_MAXIMIZE) {
            //int i = 23;
            //$cdevel("LPARAM:0x{}", amo::string::from_number((int)wParam, 16).str());
        }
        
        // 禁止关闭
        if (!m_pNativeSettings->closable && wParam == SC_CLOSE) {
            // 255 强制关闭窗口
            if (wParam != 255) {
                return TRUE;
            }
        }
        
        if (wParam == 0xf014) {
            //$cdevel("LPARAM:0x{}", amo::string::from_number((int)wParam, 16).str());
        }
        
        BOOL bZoomed = ::IsZoomed(*this);
        HRESULT lRes = WindowImplBase::OnSysCommand(uMsg, wParam, lParam, bHandled);
        std::shared_ptr<UIMessageBroadcaster> runner;
        runner.reset(new UIMessageBroadcaster(m_nTransferObjectID));
        
        if (::IsZoomed(*this) != bZoomed && ::IsIconic(*this) == FALSE) {
            if (bZoomed == TRUE) {
                runner->broadcast("restore");
            } else {
                runner->broadcast("maximize");
            }
        } else {
            switch (wParam) {
            case SC_SIZE:
                runner->broadcast("resize");
                break;
                
            case SC_MOVE:
            
                runner->broadcast("move");
                break;
                
            case SC_MINIMIZE:
            case 0xf022:
                runner->broadcast("minimize");
                break;
                
            case SC_CLOSE:
                break;
                
            default:
                break;
            }
        }
        
        return lRes;
    }
    
    LRESULT LocalWindow::HandleCustomMessage(UINT uMsg, WPARAM wParam,
            LPARAM lParam, BOOL& bHandled) {
            
        //return FALSE;
        
        switch (uMsg) {
        case WM_IPC_READY: {
            m_bIPCReady = true;
            break;
        }
        
        case WM_WINDOW_CREATED: {
    
            std::shared_ptr<ClassTransfer> pTransfer;
            pTransfer = ClassTransfer::findTransfer(m_nTransferObjectID);
            
            if (pTransfer) {
                broadcastMessage(getTransferClassID(),
                                 "create",
                                 pTransfer->getFuncMgr().toSimplifiedJson());
            }
            
            break;
        }
        
        case WM_MOVE: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            amo::u8json json;
            json.put("x", x);
            json.put("y", y);
            broadcastMessage(getTransferObjectID(), "move", json);
            break;
        }
        
        case WM_WINDOWPOSCHANGING: {
            //$cdevel("WM_WINDOWPOSCHANGING");
            /*WINDOWPOS * winPos = (LPWINDOWPOS)lParam;
            ::MoveWindow(NULL, winPos->x, winPos->y, winPos->cx, winPos->cy, TRUE);*/
            break;
        }
        
        case WM_SIZE: {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            amo::u8json json;
            json.put("width", width);
            json.put("height", height);
            
            broadcastMessage(getTransferObjectID(), "resize", json);
            break;
        }
        
        case WM_KILLFOCUS: {
            broadcastMessage(getTransferObjectID(), "blur");
            break;
        }
        
        case WM_SETFOCUS: {
            broadcastMessage(getTransferObjectID(), "focus");
            break;
        }
        
        case WM_CLOSE: {
            broadcastMessage(getTransferObjectID(), "close");
            break;
        }
        
        default:
            break;
        }
        
        return FALSE;
        return LayeredWindow::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
    }
    
    LRESULT LocalWindow::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                   BOOL& bHandled) {
        switch (wParam) {
        case VK_ESCAPE:
            if (m_pNativeSettings->esc) {
                ::PostMessage(m_hWnd, WM_CLOSE, NULL, NULL);
                return TRUE;
            }
            
            break;
            
        default:
            break;
        }
        
        return LayeredWindow::OnKeyDown(uMsg, wParam, lParam, bHandled);
    }
    
    void LocalWindow::Notify(TNotifyUI& msg) {
    
    
        auto controlMgr = TransferMappingMgr<ControlTransfer>::getInstance();
        amo::u8json json;
        std::string strName = amo::u8string(msg.pSender->GetName().GetData()).to_utf8();
        std::string eventType = amo::u8string(msg.sType.GetData()).to_utf8();
        
        // 加了计时器 timer 触发 太多，先屏蔽掉
        if (eventType != "timer") {
            json.put("name", strName);
            json.put("type", eventType);
            json.put("x", msg.ptMouse.x);
            json.put("y", msg.ptMouse.y);
            json.put("sender", controlMgr->toSimplifiedJson(msg.pSender));
            addTransferedControl(msg.pSender);
            broadcastMessage(getTransferObjectID(), eventType, json);
        }
        
        
        
        return WindowImplBase::Notify(msg);
    }
    
    bool LocalWindow::preTranslateMessage(CefEventHandle os_event) {
        return false;
    }
    
    
    Any LocalWindow::showTitleBar(IPCMessage::SmartType msg) {
    
        // 由BrowserWindow 继承实现
        return Undefined();
    }
    Any LocalWindow::topmost(IPCMessage::SmartType msg) {
        setTopmost(msg->getArgumentList()->getBool(0));
        return Undefined();
    }
    
    Any LocalWindow::isTopmost(IPCMessage::SmartType msg) {
        if (::GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST) {
            return true;
        } else {
            return false;
        }
    }
    
    
    Any LocalWindow::setFullScreen(IPCMessage::SmartType msg) {
        bool bFull = msg->getArgumentList()->getBool(0);
        
        if (bFull && !m_pNativeSettings->fullscreenable) {
            // 禁止全屏
            return Undefined();
        }
        
        setFullscreen(bFull);
        return Undefined();
    }
    
    Any LocalWindow::isFullScreen(IPCMessage::SmartType msg) {
        return m_isFullScreen;
    }
    
    Any LocalWindow::setMinimumSize(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        int width = args->getInt(0);
        int height = args->getInt(1);
        
        if (width <= 0 || height <= 0) {
            width = height = 0;
        }
        
        // DEBUG 模式下不能为0，会被断言
#ifdef _DEBUG
        
        if (width == 0 || height == 0) {
            return Undefined();
        }
        
#endif
        
        m_PaintManager.SetMinInfo(width, height);
        RECT rect = { 0 };
        ::GetWindowRect(m_hWnd, &rect);
        ::MoveWindow(m_hWnd,
                     rect.left,
                     rect.top,
                     rect.right - rect.left,
                     rect.bottom - rect.top,
                     TRUE);
                     
        return Undefined();
    }
    
    Any LocalWindow::getMinimumSize(IPCMessage::SmartType msg) {
        SIZE sz = m_PaintManager.GetMinInfo();
        amo::u8json json;
        json.put("width", sz.cx);
        json.put("height", sz.cy);
        return json;
    }
    
    Any LocalWindow::setMaximumSize(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        int width = args->getInt(0);
        int height = args->getInt(1);
        
        if (width <= 0 || height <= 0) {
            width = height = 0;
        }
        
        // DEBUG 模式下不能为0，会被断言
#ifdef _DEBUG
        
        if (width == 0 || height == 0) {
            return Undefined();
        }
        
#endif
        
        
        if ((width != 0 && height != 0) || !getNativeSettings()->resizable) {
            resizableInCaption(false);
        } else  {
            resizableInCaption(true);
        }
        
        
        m_PaintManager.SetMaxInfo(width, height);
        RECT rect = { 0 };
        ::GetWindowRect(m_hWnd, &rect);
        ::MoveWindow(m_hWnd,
                     rect.left,
                     rect.top,
                     rect.right - rect.left,
                     rect.bottom - rect.top,
                     TRUE);
        return Undefined();
    }
    
    Any LocalWindow::getMaximumSize(IPCMessage::SmartType msg) {
        SIZE sz = m_PaintManager.GetMaxInfo();
        amo::u8json json;
        json.put("width", sz.cx);
        json.put("height", sz.cy);
        return json;
    }
    
    Any LocalWindow::setResizable(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        bool resizeable = args->getBool(0);
        m_pNativeSettings->resizable = resizeable;
        //覆盖掉maximizable之有设置
        m_pNativeSettings->maximizable = true;
        
        
        if (!resizeable) {
            RECT rcSizeBox = { 0, 0, 0, 0 };
            m_PaintManager.SetSizeBox(rcSizeBox);
            
            LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
            styleValue &= ~UI_WNDSTYLE_FRAME;
            styleValue |= UI_WNDSTYLE_DIALOG;
            ::SetWindowLong(*this, GWL_STYLE,
                            styleValue);
                            
        } else {
            RECT rcSizeBox = { 4, 4, 4, 4 };
            m_PaintManager.SetSizeBox(rcSizeBox);
            
            LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
            styleValue &= ~UI_WNDSTYLE_DIALOG;
            styleValue |= UI_WNDSTYLE_FRAME;
            ::SetWindowLong(*this, GWL_STYLE,
                            styleValue);
                            
        }
        
        return Undefined();
    }
    
    
    
    Any LocalWindow::setMaximizable(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        if (!getNativeSettings()->resizable) {
            return false;
        }
        
        bool maximizable = args->getBool(0);
        getNativeSettings()->maximizable = maximizable;
        resizableInCaption(maximizable);
        return Undefined();
    }
    
    Any LocalWindow::center(IPCMessage::SmartType msg) {
        CenterWindow();
        return Undefined();
    }
    
    
    Any LocalWindow::setHasShadow(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        bool hasShadow = args->getBool(0);
        m_pNativeSettings->hasShadow = hasShadow;
        m_Shadow.Show(m_pNativeSettings->hasShadow);
        return Undefined();
    }
    
    
    Any LocalWindow::toggleFullScreen(IPCMessage::SmartType msg) {
        setFullscreen(!m_isFullScreen);
        return Undefined();
    }
    
    Any LocalWindow::toggleVisible(IPCMessage::SmartType msg) {
        BOOL bVisible = ::IsWindowVisible(m_hWnd);
        
        if (bVisible == TRUE) {
            hide(msg);
        } else {
            show(msg);
        }
        
        //::ShowWindow(m_hWnd, bVisible == TRUE ? SW_HIDE : SW_SHOW);
        return Undefined();
        
    }
    
    
    HWND LocalWindow::getNativeHWND() {
        return GetHWND();
    }
    
    std::shared_ptr<NativeWindowSettings> LocalWindow::getNativeSettings() {
        return m_pNativeSettings;
    }
    
    Any LocalWindow::showModal(IPCMessage::SmartType msg) {
        /* if (getNativeSettings()->modal) {
             ShowModal();
         }*/
        /* runOnUIThread([&]() {
          ShowModal();
         });*/
        
        return Undefined();
    }
    
    Any LocalWindow::setOpacity(IPCMessage::SmartType msg) {
        int opacity = 255;
        Any& val = msg->getArgumentList()->getValue(0);
        
        if (val.isValid()) {
            opacity = val.As<int>();
        }
        
        if (opacity > 255) {
            opacity = 255;
        }
        
        if (opacity < 0) {
            opacity = 0;
        }
        
        m_PaintManager.SetOpacity((int8_t)opacity);
        return Undefined();
    }
    
    Any LocalWindow::getOpacity(IPCMessage::SmartType msg) {
        return (int)m_PaintManager.GetOpacity();
    }
    
    
    Any LocalWindow::disableIME(IPCMessage::SmartType msg) {
        imm32_manager_->cleanupComposition();
        imm32_manager_->disableIME();
        return Undefined();
    }
    
    Any LocalWindow::enableIME(IPCMessage::SmartType msg) {
        imm32_manager_->enableIME();
        return Undefined();
    }
    
    Any LocalWindow::computeCursor(IPCMessage::SmartType msg) {
        m_pNativeSettings->compute_cursor = msg->getArgumentList()->getBool(0);
        return Undefined();
    }
    
    Any LocalWindow::isComputeCursor(IPCMessage::SmartType msg) {
        return m_pNativeSettings->compute_cursor;
    }
    
    Any LocalWindow::setTheme(IPCMessage::SmartType msg) {
        amo::u8json json = msg->getArgumentList()->getJson(0);
        m_pNativeSettings->updateArgsSettings(json);
        
        CControlUI*  pTitleBar = static_cast<CControlUI*>(m_PaintManager.FindControl(
                                     _T("titleLayout")));
        CDuiString bkcolor(amo::u8string(m_pNativeSettings->primaryColor,
                                         true).to_wide().c_str());
                                         
        CDuiString bkcolor2(amo::u8string(m_pNativeSettings->primaryColor2,
                                          true).to_wide().c_str());
                                          
        if (pTitleBar != NULL) {
            pTitleBar->SetAttribute(_T("bkcolor"), bkcolor.GetData());
            pTitleBar->SetAttribute(_T("bkcolor2"), bkcolor2.GetData());
        }
        
        return Undefined();
        
    }
    
    bool LocalWindow::isFocusedWindow() {
        HWND hWnd = m_hWnd;
        HWND hFocusWnd = ::GetFocus();
        
        while (hFocusWnd != NULL) {
            if (hWnd == hFocusWnd) {
                return true;
            }
            
            hFocusWnd = ::GetParent(hFocusWnd);
        }
        
        return false;
    }
    
    
    bool LocalWindow::isNativeWindow() const {
        return m_bNativeWindow;
    }
    
    void LocalWindow::setNativeWindow(bool val) {
        m_bNativeWindow = val;
    }
    
    std::shared_ptr<amo::NativeWindow> LocalWindow::toNativeWindow() {
        return std::shared_ptr<NativeWindow>();
    }
    
    std::shared_ptr<amo::BrowserWindow> LocalWindow::toBrowserWindow() {
        return std::shared_ptr<BrowserWindow>();
    }
    
    
    LRESULT LocalWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                BOOL & bHandled) {
        /*if (!m_pNativeSettings->resizeable) {
            return TRUE;
        }*/
        /*   RECT rt = { 0 };
           ::GetClientRect(m_hWnd, &rt);*/
        return LayeredWindow::OnSize(uMsg, wParam, lParam, bHandled);
    }
    
    
    
    
    
    int64_t LocalWindow::getTransferClassID() const {
        return m_nTransferClassID;
    }
    
    void LocalWindow::setTransferClassID(int64_t val) {
        m_nTransferClassID = val;
    }
    
    int64_t LocalWindow::getTransferObjectID() const {
        return m_nTransferObjectID;
    }
    
    void LocalWindow::setTransferObjectID(int64_t val) {
        m_nTransferObjectID = val;
        m_pNativeSettings->id = std::to_string(m_nTransferObjectID);
    }
    
    void LocalWindow::addTransferedControl(CControlUI * pControl) {
        m_oTransferedSet.insert(pControl);
    }
    
    
    
    
    void LocalWindow::syncBroadcastMessage(const int64_t& nID,
                                           const std::string & msg,
                                           amo::u8json & data) {
        /*     return;*/
        
        std::shared_ptr<UIMessageBroadcaster> runner;
        runner.reset(new UIMessageBroadcaster(nID));
        runner->syncBroadcast(msg, data);
    }
    
    void LocalWindow::syncBroadcastMessage(const int64_t& nID,
                                           const std::string & msg) {
        //return;
        static int i = 0;
        
        
        if (msg == "focus") {
            ++i;
            //return;
        }
        
        //return;
        std::shared_ptr<UIMessageBroadcaster> runner;
        runner.reset(new UIMessageBroadcaster(nID));
        runner->syncBroadcast(msg);
    }
    
    void LocalWindow::broadcastMessage(const int64_t& nID,
                                       const std::string & msg,
                                       amo::u8json & data) {
        /*     return;*/
        
        std::shared_ptr<UIMessageBroadcaster> runner;
        runner.reset(new UIMessageBroadcaster(nID));
        runner->broadcast(msg, data);
    }
    
    void LocalWindow::broadcastMessage(const int64_t& nID,
                                       const std::string & msg) {
        //return;
        static int i = 0;
        
        
        if (msg == "focus") {
            ++i;
            //return;
        }
        
        //return;
        std::shared_ptr<UIMessageBroadcaster> runner;
        runner.reset(new UIMessageBroadcaster(nID));
        runner->broadcast(msg);
    }
    
    
    
}

