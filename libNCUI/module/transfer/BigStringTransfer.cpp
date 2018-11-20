#include "stdafx.h"
#include "module/transfer/BigStringTransfer.h"
#include "ipc/BlobManager.hpp"

namespace amo {


    BigStringTransfer::BigStringTransfer()
        : ClassTransfer("BigStr") {
        
    }
    
    BigStringTransfer::BigStringTransfer(const std::string& str,
                                         const int32_t delay_destroy)
        : ClassTransfer("BigStr") {
        
        auto mgr = BigStrManager<PID_RENDERER>::getInstance();
        auto  m_blob = mgr->create(
                           std::to_string(getObjectID()),
                           str.size(),
                           delay_destroy);
        m_blob->write(str);
    }
    
    Any BigStringTransfer::onCreateClass(IPCMessage::SmartType msg) {
        std::string str = msg->getArgumentList()->getString(0);
        Any& val = msg->getArgumentList()->getValue(1);
        int32_t delay = 10000;
        
        if (val.isValid() && val.is<int>()) {
            delay = val.As<int>();
            
            if (delay <= 0) {
                delay = 10000;
            }
        }
        
        auto pTransfer = ClassTransfer::createTransfer<BigStringTransfer>(str, delay);
        return pTransfer->getFuncMgr().toSimplifiedJson();
    }
    
    std::string BigStringTransfer::getClass() const {
        return "BigStr";
    }
    
    amo::Transfer * BigStringTransfer::getInterface(const std::string& name) {
        if (name == BigStringTransfer::getClass()) {
            return this;
        }
        
        return ClassTransfer::getInterface(name);
    }
    
    
    Any BigStringTransfer::getData(IPCMessage::SmartType msg) {
        auto mgr = BlobManager<PID_RENDERER>::getInstance();
        auto m_blob = mgr->find(std::to_string(getObjectID()));
        
        if (!m_blob) {
            return std::string();
        }
        
        return m_blob->read();
    }
    
    
    
}

