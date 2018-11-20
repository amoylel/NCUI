#include "stdafx.h"
#include "transfer/TrayTransfer.h"
#include "ui/win/tray/Tray.h"
#include "ipc/UIMessageBroadcaster.hpp"

namespace amo {

    TrayTransfer::TrayTransfer()
        : ClassTransfer("tray") {
        
        addModule("EventEmitter");
        
    }
    
    std::shared_ptr<amo::Tray> TrayTransfer::getTray()  {
        if (!m_pTray) {
            m_pTray = Tray::getInstance();
            m_pTray->setEventCallback(std::bind(&TrayTransfer::onTrayEvent,
                                                this,
                                                std::placeholders::_1));
        }
        
        return m_pTray;
    }
    
    void TrayTransfer::setTray(std::shared_ptr<amo::Tray> val) {
        m_pTray = val;
    }
    
    void TrayTransfer::onTrayEvent(const std::string& event) {
        int64_t nObjectID = this->getObjectID();
        std::shared_ptr<UIMessageBroadcaster> runner;
        runner.reset(new UIMessageBroadcaster(nObjectID));
        runner->broadcast(event);
    }
    
    Any TrayTransfer::show(IPCMessage::SmartType msg) {
        getTray()->show(true);
        return Undefined();
    }
    
    Any TrayTransfer::hide(IPCMessage::SmartType msg) {
        getTray()->show(false);
        return Undefined();
    }
    
    Any TrayTransfer::setIcon(IPCMessage::SmartType msg) {
        amo::u8string strPath(msg->getArgumentList()->getString(0), true);
        getTray()->setTrayIcon(strPath);
        return Undefined();
    }
    
    Any TrayTransfer::setTooltip(IPCMessage::SmartType msg) {
        amo::u8string str(msg->getArgumentList()->getString(0), true);
        getTray()->setToolTip(str.to_wide());
        return Undefined();
    }
    
    
    Any TrayTransfer::blink(IPCMessage::SmartType msg) {
        bool bBlink = msg->getArgumentList()->getBool(0);
        getTray()->blink(bBlink);
        return Undefined();
    }
    
    Any TrayTransfer::isBlink(IPCMessage::SmartType msg) {
        return getTray()->isBlink();
    }
    
}

