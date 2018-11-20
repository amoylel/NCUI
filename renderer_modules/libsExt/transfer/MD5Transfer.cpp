#include "stdafx.h"
#include "transfer/MD5Transfer.h"
#include <amo/md5.hpp>

namespace amo {
    MD5Transfer::MD5Transfer()
        : RunnableTransfer("md5") {
    }
    
    
    Any MD5Transfer::onCreateClass(IPCMessage::SmartType msg) {
        auto pTransfer = ClassTransfer::createTransfer<MD5Transfer>();
        pTransfer->setTriggerEventFunc(this->getTriggerEventFunc());
        pTransfer->setDefaultTriggerEventFunc(this->getDefaultTriggerEventFunc());
        return pTransfer->getFuncMgr().toSimplifiedJson();
    }
    
    std::string MD5Transfer::getClass() const {
        return "md5";
    }
    
    amo::Transfer * MD5Transfer::getInterface(const std::string& name) {
        if (name == MD5Transfer::getClass()) {
            return this;
        }
        
        return RunnableTransfer::getInterface(name);
    }
    
    
    Any MD5Transfer::encode(IPCMessage::SmartType msg) {
        Any& val = msg->getArgumentList()->getValue(0);
        bool isFile = msg->getArgumentList()->getBool(1);
        std::string invalidMD5 =   "00000000000000000000000000000000";
        
        if (val.is<int64_t>()) {
            auto transfer = findTransfer(val.As<int64_t>());
            
            if (!transfer) {
                return invalidMD5;
            }
            
            auto* pPath = transfer->getInterface("Path");
            
            if (pPath == NULL) {
                return invalidMD5;
            }
            
            std::string strPath = dynamic_cast<PathTransfer*>(pPath)->toString(
                                      IPCMessage::Empty());
            amo::string str(strPath, true);
            amo::md5::encode_file(str.c_str());
            
            
        } else if (val.is<std::string>()) {
            amo::string str(val.As<std::string>(), true);
            
            if (isFile) {
                return amo::md5::encode_file(str.c_str());
            } else {
                return amo::md5::encode(str);
            }
        }
        
        return invalidMD5;
        
    }
    
    
    
}

