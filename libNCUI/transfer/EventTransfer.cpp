#include "stdafx.h"
#include "transfer/EventTransfer.h"

namespace amo {

    EventTransfer::EventTransfer()
        : ClassTransfer("Event") {
        m_bStopPropagation = false;
        retval = Undefined();
    }
    
    Any EventTransfer::getReturnValue(IPCMessage::SmartType msg) {
        return retval;
    }
    
    Any EventTransfer::setReturnValue(IPCMessage::SmartType msg) {
        retval = msg->getArgumentList()->getValue(0);
        return Undefined();
    }
    
    void EventTransfer::setRetval(const Any& val) {
        retval = val;
    }
    
    Any EventTransfer::target(IPCMessage::SmartType msg) {
        if (!getTarget()) {
            return Undefined();
        }
        
        return getTarget()->getFuncMgr().toSimplifiedJson();
    }
    
    Any EventTransfer::data(IPCMessage::SmartType msg) {
        if (!m_pMsg) {
            return Undefined();
        }
        
        std::shared_ptr<AnyArgsList> args = m_pMsg->getArgumentList();
        std::vector<Any> vec;
        
        for (int i = 1; i < args->getArgsSize(); ++i) {
            vec.push_back(args->getValue(i));
        }
        
        return vec;
    }
    
    Any EventTransfer::stopPropagation(IPCMessage::SmartType msg) {
        m_bStopPropagation = true;
        return Undefined();
    }
    
    Any EventTransfer::isStopPropagation(IPCMessage::SmartType msg) {
        return m_bStopPropagation;
    }
    
    Any EventTransfer::onCreateClass(IPCMessage::SmartType msg) {
        std::shared_ptr<EventTransfer> pTransfer;
        pTransfer = ClassTransfer::createTransfer<EventTransfer>();
        return pTransfer->getFuncMgr().toSimplifiedJson();
    }
    
    std::shared_ptr<ClassTransfer> EventTransfer::getTarget() const {
        return m_pTarget;
    }
    
    void EventTransfer::setTarget(std::shared_ptr<ClassTransfer> val) {
        m_pTarget = val;
    }
    
    amo::IPCMessage::SmartType EventTransfer::getMsg() const {
        return m_pMsg;
    }
    
    void EventTransfer::setMsg(amo::IPCMessage::SmartType val) {
        m_pMsg = val;
    }
    
}

