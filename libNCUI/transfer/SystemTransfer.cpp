#include "stdafx.h"
#include "transfer/SystemTransfer.h"
#include "transfer/DesktopTransfer.h"
#include "transfer/MouseTransfer.h"
#include "transfer/KeyboardTransfer.h"
#include "transfer/SystemInfo.h"

namespace amo {


    SystemTransfer::SystemTransfer()
        : ClassTransfer("system") {
        addDepends("desktop");
        addDepends("mouse");
        addDepends("keyboard");
    }
    
    Any SystemTransfer::info(IPCMessage::SmartType msg) {
        return SystemInfo().getSystemResource();;
    }
    
    Any SystemTransfer::screen(IPCMessage::SmartType msg) {
        auto pTransfer = ClassTransfer::getUniqueTransfer<DesktopTransfer>();
        return pTransfer->getFuncMgr().toSimplifiedJson();
    }
    
    Any SystemTransfer::mouse(IPCMessage::SmartType msg) {
        auto pTransfer = ClassTransfer::getUniqueTransfer<MouseTransfer>();
        return pTransfer->getFuncMgr().toSimplifiedJson();
    }
    
    Any SystemTransfer::keyboard(IPCMessage::SmartType msg) {
        auto pTransfer = ClassTransfer::getUniqueTransfer<KeyboardTransfer>();
        return pTransfer->getFuncMgr().toSimplifiedJson();
    }
    
}

