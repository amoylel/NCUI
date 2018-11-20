// Created by amoylel on 12/04/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef LIBNCUI_INTERPROCESSTRANSFER_50BCDD1C_0EAF_44CA_8475_776EE4B9A3EA_H__
#define LIBNCUI_INTERPROCESSTRANSFER_50BCDD1C_0EAF_44CA_8475_776EE4B9A3EA_H__


#include <memory>
#include <thread>

#include <amo/singleton.hpp>
#include <amo/type_traits.hpp>
#include <amo/ipc.hpp>
#include <amo/json.hpp>

#include "transfer/ClassTransfer.hpp"



namespace amo {

    /*!
     * @class	InterProcess
     * @chapter extend
     * @brief	进程间通信类.
     */
    
    class InterProcessTransfer
        : public RunnableTransfer
        , public amo::singleton<InterProcessTransfer> {
        
    public:
        InterProcessTransfer();
        InterProcessTransfer(const amo::u8json& json);
        
        Any onCreateClass(IPCMessage::SmartType msg) override;
        virtual std::string getClass() const override;
        virtual Transfer* getInterface(const std::string& name) override;
        
        Any connect(IPCMessage::SmartType msg);
        Any exec(IPCMessage::SmartType msg);
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(InterProcessTransfer, RunnableTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(connect, TransferFuncNormal | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(exec, TransferFuncNormal | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    private:
    
        amo::any onMsg(amo::ipc_msg::SmartType msg);
    private:
        amo::u8json settings; // u8settings
        std::shared_ptr<amo::ipc::server> m_server;
        std::shared_ptr<amo::ipc::client> m_client;
        std::shared_ptr<std::thread> m_pThread;
    };
}



#endif //LIBNCUI_INTERPROCESSTRANSFER_50BCDD1C_0EAF_44CA_8475_776EE4B9A3EA_H__