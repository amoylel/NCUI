// Created by amoylel on 22/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_IPC_TRANSFER_FC22E18F_9449_4D9E_B4AC_20AF7D47EF01_HPP__
#define AMO_IPC_TRANSFER_FC22E18F_9449_4D9E_B4AC_20AF7D47EF01_HPP__

#include <amo/ipc/class_transfer.hpp>

namespace amo {
    class ipc_transfer : public class_transfer {
    
    public:
        ipc_transfer() : class_transfer("ipc") {}
        any onCreateClass(ipc_msg::SmartType msg) override {
            auto pTransfer = class_transfer::createTransfer<ipc_transfer>();
            pTransfer->setTriggerEventFunc(this->getTriggerEventFunc());
            return pTransfer->getFuncMgr().toSimplifiedJson();
        }
        virtual std::string getClass() const override {
            return "ipc";
        }
        virtual transfer* getInterface(const std::string& name) override {
            if (name == ipc_transfer::getClass()) {
                return this;
            }
            
            return class_transfer::getInterface(name);
        }
        
    public:
        IPC_MESSAGE_TRANSFER_BEGIN(ipc_transfer, class_transfer)
        //IPC_MESSAGE_TRANSFER_FUNC(foo, transfer_func_normal | transfer_exec_normal)
        IPC_MESSAGE_TRANSFER_END()
    };
}



#endif //AMO_IPC_TRANSFER_FC22E18F_9449_4D9E_B4AC_20AF7D47EF01_HPP__