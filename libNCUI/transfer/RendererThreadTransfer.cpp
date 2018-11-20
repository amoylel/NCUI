#include "stdafx.h"
#include "transfer/RendererThreadTransfer.h"
#include "transfer/TransferMgr.h"
#include "transfer/RendererTransferMgr.h"


namespace amo {

    RendererThreadTransfer::RendererThreadTransfer()
        : ThreadTransfer<ThreadRenderer>("Thread") {
        setWorkOnRenderer(true);
    }
    
    RendererThreadTransfer::~RendererThreadTransfer() {
    
    }
    
    Any RendererThreadTransfer::onCreateClass(IPCMessage::SmartType msg) {
        std::shared_ptr<ThreadTransfer> pThread(new RendererThreadTransfer());
        pThread->setWorkOnRenderer(true);
        pThread->registerFunction();
        pThread->createThread();
        addTransfer(pThread);
        return  pThread->getFuncMgr().toSimplifiedJson();
    }
    
    std::shared_ptr< TransferMgr> RendererThreadTransfer::getTransferMgr() {
        return RendererTransferMgr::getInstance();
    }
    
    
    
}

