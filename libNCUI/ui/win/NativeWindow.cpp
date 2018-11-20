#include "stdafx.h"
#include "ui/win/NativeWindow.h"
#include "ui/win/BrowserWindowSkin.h"
#include "ipc/UIMessageEmitter.hpp"
#include "handler/BrowserManager.hpp"
#include "ipc/UIMessageBroadcaster.hpp"
#include "settings/NativeWindowSettings.h"
#include "transfer/ControlTransfer.h"
#include "transfer/TransferMappingMgr.hpp"


namespace amo {



    NativeWindow::NativeWindow(std::shared_ptr<NativeWindowSettings>
                               pBrowserSettings)
        : LocalWindow(pBrowserSettings) {
        setNativeWindow(true);
        m_pNativeSettings->id = amo::u8string::from_number(getObjectID()).to_utf8();
        
        auto pWindow = ClassTransfer::getUniqueTransfer<NativeWindowTransfer>();
        int64_t nClassID = pWindow->getObjectID();
        setTransferClassID(nClassID);
        setTransferObjectID(getObjectID());
    }
    
    NativeWindow::~NativeWindow() {
        removeTransfer(getObjectID());
    }
    
    DuiLib::CDuiString NativeWindow::GetSkinFolder() {
        return _T("");
    }
    
    DuiLib::CDuiString NativeWindow::GetSkinFile() {
    
    
        std::string skin = m_pNativeSettings->settings.getString("xml");
        
        if (!skin.empty()) {
            return amo::u8string(skin, true).to_unicode().c_str();
        } else {
            return amo::u8string(skinNativeWindow, true).format(
                       m_pNativeSettings->settings).to_unicode().c_str();
        }
    }
    
    LPCTSTR NativeWindow::GetWindowClassName(void) const {
        return amo::u8string(m_pNativeSettings->windowClass, true).to_unicode().c_str();
    }
    
    int NativeWindow::getBrowserID() const {
        return m_nBrowserID;
    }
    
    void NativeWindow::setBrowserID(int val) {
        m_nBrowserID = val;
    }
    
    
    
    std::shared_ptr<amo::NativeWindow> NativeWindow::toNativeWindow() {
        return std::dynamic_pointer_cast<NativeWindow>(shared_from_this());
    }
    
    LRESULT NativeWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam,
                                    BOOL& bHandled) {
                                    
        return LocalWindow::OnDestroy(uMsg, wParam, lParam, bHandled);
    }
    
    void NativeWindow::OnFinalMessage(HWND hWnd) {
        LocalWindow::OnFinalMessage(hWnd);
        removeTransfer(this->getObjectID());
    }
    
    int64_t NativeWindow::getFrameID() const {
        return m_nFrameID;
    }
    
    void NativeWindow::setFrameID(int64_t val) {
        m_nFrameID = val;
    }
    
}

