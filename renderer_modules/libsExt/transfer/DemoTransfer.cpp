#include "stdafx.h"
#include "transfer/DemoTransfer.h"

namespace amo {
    DemoTransfer::DemoTransfer()
        : RunnableTransfer("Demo") {
        m_constStaticTestID = 1;
        m_staticTestID = 2;
        m_memberTestID = 3;
    }
    
    
    Any DemoTransfer::onCreateClass(IPCMessage::SmartType msg) {
        auto pTransfer = ClassTransfer::createTransfer<DemoTransfer>();
        return pTransfer->getFuncMgr().toSimplifiedJson();
    }
    
    std::string DemoTransfer::getClass() const {
        return "Demo";
    }
    
    amo::Transfer * DemoTransfer::getInterface(const std::string& name) {
        if (name == DemoTransfer::getClass()) {
            return this;
        }
        
        return RunnableTransfer::getInterface(name);
    }
    
    
    Any DemoTransfer::add(IPCMessage::SmartType msg) {
        auto args = msg->getArgumentList();
        int a = args->getInt(0);
        int b = args->getInt(1);
        return a + b;
    }
    
    Any DemoTransfer::sub(IPCMessage::SmartType msg) {
        auto args = msg->getArgumentList();
        int a = args->getInt(0);
        int b = args->getInt(1);
        return a - b;
    }
    
    Any DemoTransfer::mul(IPCMessage::SmartType msg) {
        auto args = msg->getArgumentList();
        int a = args->getInt(0);
        int b = args->getInt(1);
        
        return a * b;
    }
    
    Any DemoTransfer::div(IPCMessage::SmartType msg) {
        auto args = msg->getArgumentList();
        int a = args->getInt(0);
        int b = args->getInt(1);
        
        if (b == 0) {
            return Undefined();
        }
        
        return a + b;
    }
    
    Any DemoTransfer::sleepForWeakup(IPCMessage::SmartType msg) {
        TransferEventInfo info;
        info.suspend = true;
        triggerEvent(info);
        return Undefined();
    }
    
}

