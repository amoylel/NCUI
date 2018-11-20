#include "stdafx.h"
#include "transfer/UuidTransfer.h"


namespace amo {

    UUIDTransfer::UUIDTransfer()
        : RunnableTransfer("UUID") {
        
    }
    
    UUIDTransfer::UUIDTransfer(const std::string & str)
        : RunnableTransfer("UUID") {
        m_uuid = amo::uuid::from_string(str);
    }
    
    UUIDTransfer::~UUIDTransfer() {
    
    }
    
    Any UUIDTransfer::onCreateClass(IPCMessage::SmartType msg) {
        Any& val = msg->getArgumentList()->getValue(0);
        std::string str;
        
        if (val.isValid() && val.is<std::string>()) {
            str = val.As<std::string>();
        }
        
        std::shared_ptr<UUIDTransfer> pTransfer;
        
        if (str.empty()) {
            pTransfer = ClassTransfer::createTransfer<UUIDTransfer>();
        } else {
            pTransfer = ClassTransfer::createTransfer<UUIDTransfer>(str);
        }
        
        return pTransfer->getFuncMgr().toSimplifiedJson();
    }
    
    std::string UUIDTransfer::getClass() const {
        return "UUID";
    }
    
    amo::Transfer* UUIDTransfer::getInterface(const std::string& name) {
        if (name == UUIDTransfer::getClass()) {
            return this;
        }
        
        return RunnableTransfer::getInterface(name);
    }
    
    Any UUIDTransfer::fromString(IPCMessage::SmartType msg) {
        return createClass(msg);
    }
    
    Any UUIDTransfer::getString(IPCMessage::SmartType msg) {
        return amo::uuid().to_string();
    }
    
    Any UUIDTransfer::nil(IPCMessage::SmartType msg) {
        IPCMessage::SmartType cloneMsg = msg->clone();
        cloneMsg->getArgumentList()->setValue(0, amo::uuid::nil().to_string());
        
        return createClass(cloneMsg);
    }
    
    Any UUIDTransfer::nilString(IPCMessage::SmartType msg) {
        return amo::uuid::nil().to_string();
    }
    
    Any UUIDTransfer::toString(IPCMessage::SmartType msg) {
        return m_uuid.to_string();
    }
    
}

