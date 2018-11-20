#include "stdafx.h"
#include "transfer/Base64Transfer.h"
#include <amo/base64.hpp>

namespace amo {
    Base64Transfer::Base64Transfer()
        : RunnableTransfer("base64") {
    }
    
    
    std::string Base64Transfer::getClass() const {
        return "base64";
    }
    
    amo::Transfer * Base64Transfer::getInterface(const std::string& name) {
        if (name == Base64Transfer::getClass()) {
            return this;
        }
        
        return RunnableTransfer::getInterface(name);
    }
    
    
    Any Base64Transfer::decode(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string str = args->getString(0);
        return amo::base64::decode(str);
    }
    
    Any Base64Transfer::encode(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        std::string str = args->getString(0);
        return amo::base64::encode(str);
    }
    
    
}

