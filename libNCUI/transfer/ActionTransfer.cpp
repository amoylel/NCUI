#include "stdafx.h"
#include "transfer/ActionTransfer.h"
#include "ui/win/ActionPreprocessor.h"


namespace amo {

    ActionTransfer::ActionTransfer()
        : ClassTransfer("action") {
    }
    
    Any ActionTransfer::execute(IPCMessage::SmartType msg) {
        int nBrowserID = msg->getArgumentList()->getInt(IPCArgsPosInfo::BrowserID);
        int64_t nFrameID = msg->getArgumentList()->getInt(IPCArgsPosInfo::FrameID);
        ActionPreprocessor processor(nBrowserID, nFrameID);
        std::string action = msg->getArgumentList()->getString(0);
        return processor.process(action);
    }
    
}

