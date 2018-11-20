#include "stdafx.h"
#include "UIThreadTransfer.h"
#include "transfer/TransferMgr.h"

namespace amo {

    UIThreadTransfer::UIThreadTransfer()
        : ThreadTransfer<ThreadUI>("Task") {
        
    }
    
    Any UIThreadTransfer::onCreateClass(IPCMessage::SmartType msg) {
        std::shared_ptr<ThreadTransfer> pThread(new UIThreadTransfer());
        pThread->registerFunction();
        pThread->createThread();
        addTransfer(pThread);
        return  pThread->getFuncMgr().toSimplifiedJson();
    }
    
    std::string UIThreadTransfer::getClass() const {
        return "Task";
    }
    
    amo::Transfer* UIThreadTransfer::getInterface(const std::string& name) {
        if (name == UIThreadTransfer::getClass()) {
            return this;
        }
        
        return ThreadBaseTransfer::getInterface(name);
    }
    
    std::shared_ptr< TransferMgr> UIThreadTransfer::getTransferMgr() {
        return BrowserTransferMgr::getInstance();
    }
    
}

