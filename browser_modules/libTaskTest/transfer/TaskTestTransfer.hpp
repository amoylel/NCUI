// Created by amoylel on 11/03/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_TASKTESTTRANSFER_0FF9EB2B_35C9_426A_8D71_7AA12E10C0F5_HPP__
#define AMO_TASKTESTTRANSFER_0FF9EB2B_35C9_426A_8D71_7AA12E10C0F5_HPP__


#include "transfer/RunnableTransfer.hpp"


namespace amo {
    class TaskTestTransfer
        : public RunnableTransfer
        , public amo::singleton<TaskTestTransfer> {
    public:
        TaskTestTransfer(): RunnableTransfer("TaskTest") {
        
            m_constStaticTestID = 1;
            m_staticTestID = 2;
            m_memberTestID = 3;
        }
        
        Any onCreateClass(IPCMessage::SmartType msg) {
            auto pTransfer = ClassTransfer::createTransfer<TaskTestTransfer>();
            return pTransfer->getFuncMgr().toSimplifiedJson();
        }
        
        std::string getClass() const {
            return "TaskTest";
        }
        
        amo::Transfer * getInterface(const std::string& name) {
            if (name == getClass()) {
                return this;
            }
            
            return RunnableTransfer::getInterface(name);
        }
        
        
        Any add(IPCMessage::SmartType msg) {
            auto args = msg->getArgumentList();
            int a = args->getInt(0);
            int b = args->getInt(1);
            return a + b;
        }
        
        Any sub(IPCMessage::SmartType msg) {
            auto args = msg->getArgumentList();
            int a = args->getInt(0);
            int b = args->getInt(1);
            return a - b;
        }
        
        Any mul(IPCMessage::SmartType msg) {
            auto args = msg->getArgumentList();
            int a = args->getInt(0);
            int b = args->getInt(1);
            
            return a * b;
        }
        
        Any div(IPCMessage::SmartType msg) {
            auto args = msg->getArgumentList();
            int a = args->getInt(0);
            int b = args->getInt(1);
            
            if (b == 0) {
                return Undefined();
            }
            
            return a + b;
        }
        
        Any sleepForWeakup(IPCMessage::SmartType msg) {
            TransferEventInfo info;
            info.suspend = true;
            sendEvent(info);
            return Undefined();
        }
        
        AMO_CONFIG_GETTER_SETTER(staticTestID)
        AMO_CONFIG_GETTER_SETTER(constStaticTestID)
        AMO_CONFIG_GETTER_SETTER(memberTestID)
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(TaskTestTransfer, RunnableTransfer)
        // 成员函数
        AMO_CEF_MESSAGE_TRANSFER_FUNC(add, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(sub, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(mul, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(div, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(sleepForWeakup,
                                      TransferFuncNormal | TransferExecNormal)
                                      
        AMO_CEF_MESSAGE_TRANSFER_FUNC(staticTestID,
                                      TransferFuncClassProperty | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(constStaticTestID,
                                      TransferFuncConstProperty | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(memberTestID,
                                      TransferFuncMemberProperty | TransferExecSync)
                                      
        AMO_CEF_MESSAGE_TRANSFER_END()
    private:
        int m_staticTestID;
        int m_constStaticTestID;
        int m_memberTestID;
    };
}

#endif // AMO_TASKTESTTRANSFER_0FF9EB2B_35C9_426A_8D71_7AA12E10C0F5_HPP__

