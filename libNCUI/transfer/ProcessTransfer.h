// Created by amoylel on 12/27/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_PROCESSTRANSFER_H__
#define AMO_PROCESSTRANSFER_H__

#include <amo/singleton.hpp>

#include "transfer/RunnableTransfer.hpp"

namespace amo {
    class process;
    class ProcessTransfer
        : public RunnableTransfer
        , public amo::singleton<ProcessTransfer> {
    public:
        ProcessTransfer();
        ProcessTransfer(const std::string& strAppPath);
        
        
        Any onCreateClass(IPCMessage::SmartType msg) override;
        virtual std::string getClass() const override;
        virtual Transfer* getInterface(const std::string& name) override;
        
        
        Any findPIDByName(IPCMessage::SmartType msg);
        Any start(IPCMessage::SmartType msg);
        Any getResult(IPCMessage::SmartType msg);
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(ProcessTransfer, RunnableTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(findPIDByName,
                                      TransferFuncStatic | TransferExecAsync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(start, TransferFuncNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getResult, TransferFuncNormal)
        
        
        
        AMO_CEF_MESSAGE_TRANSFER_END()
        
        
    private:
        std::shared_ptr<amo::process> m_pPorcess;
    };
}
#endif // AMO_PROCESSTRANSFER_H__
