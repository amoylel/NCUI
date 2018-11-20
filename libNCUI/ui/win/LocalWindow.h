// Created by amoylel on 08/07/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_LOCALWINDOW_H__
#define AMO_LOCALWINDOW_H__

#include "ui/win/LayeredWindow.h"
#include "ui/win/NativeWindowProxy.h"
#include "ipc/Any.hpp"
#include "ipc/IPCMessage.hpp"
#include <memory>


namespace amo {

    class NativeWindow;
    class BrowserWindow;
    class NativeWindowSettings;
    class LocalWindow
        : public LayeredWindow
        , virtual public NativeWindowProxy  {
        
    public:
        LocalWindow(std::shared_ptr<NativeWindowSettings> pBrowserSettings);
        
        virtual void InitWindow() override;
        
        virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL& bHandled) override;
        virtual void Notify(TNotifyUI& msg) override;
        virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                     BOOL& bHandled) override;
        virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                            BOOL& bHandled) override;
        virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                  BOOL& bHandled) override;
                                  
        virtual bool preTranslateMessage(CefEventHandle os_event) ;
        
        virtual Any showTitleBar(IPCMessage::SmartType msg) override;
        virtual Any topmost(IPCMessage::SmartType msg) override;
        virtual Any isTopmost(IPCMessage::SmartType msg) override;
        virtual Any setFullScreen(IPCMessage::SmartType msg) override;
        virtual Any isFullScreen(IPCMessage::SmartType msg) override;
        virtual Any setMinimumSize(IPCMessage::SmartType msg) override;
        virtual Any getMinimumSize(IPCMessage::SmartType msg) override;
        virtual Any setMaximumSize(IPCMessage::SmartType msg) override;
        virtual Any getMaximumSize(IPCMessage::SmartType msg) override;
        virtual Any setResizable(IPCMessage::SmartType msg) override;
        virtual Any setMaximizable(IPCMessage::SmartType msg) override;
        
        virtual Any center(IPCMessage::SmartType msg) override;
        virtual Any setHasShadow(IPCMessage::SmartType msg) override;
        virtual Any toggleFullScreen(IPCMessage::SmartType msg) override;
        virtual Any toggleVisible(IPCMessage::SmartType msg) override;
        virtual Any showModal(IPCMessage::SmartType msg);
        virtual Any setOpacity(IPCMessage::SmartType msg) override;
        virtual Any getOpacity(IPCMessage::SmartType msg) override;
        virtual Any disableIME(IPCMessage::SmartType msg) override;
        virtual Any enableIME(IPCMessage::SmartType msg) override;
        virtual Any computeCursor(IPCMessage::SmartType msg) override;
        virtual Any isComputeCursor(IPCMessage::SmartType msg) override;
        virtual Any setTheme(IPCMessage::SmartType msg) override;
        
        virtual HWND getNativeHWND() override;
        virtual std::shared_ptr<NativeWindowSettings> getNativeSettings() override;
        
        
        bool isFocusedWindow();
        
        
        bool isNativeWindow() const;
        void setNativeWindow(bool val);
        
        virtual std::shared_ptr<NativeWindow> toNativeWindow();
        virtual std::shared_ptr<BrowserWindow> toBrowserWindow();
        
        
        virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam,
                               BOOL& bHandled) override;
                               
                               
                               
                               
                               
                               
                               
    protected:
        int64_t getTransferClassID() const;
        void setTransferClassID(int64_t val);
        int64_t getTransferObjectID() const;
        void setTransferObjectID(int64_t val);
        
        
        void addTransferedControl(CControlUI* pControl);
        
        void syncBroadcastMessage(const int64_t& nID,
                                  const std::string& msg,
                                  amo::u8json& data);
        void syncBroadcastMessage(const int64_t& nID,
                                  const std::string& msg);
                                  
        void broadcastMessage(const int64_t& nID,
                              const std::string& msg,
                              amo::u8json& data);
        void broadcastMessage(const int64_t& nID,
                              const std::string& msg);
                              
                              
    protected:
        bool m_bNativeWindow;
        int64_t m_nTransferClassID;  // 所属类ID
        int64_t m_nTransferObjectID; // 所属对象ID
        std::set<CControlUI*> m_oTransferedSet;
        bool m_bIPCReady;
    };
}

#endif // AMO_LOCALWINDOW_H__
