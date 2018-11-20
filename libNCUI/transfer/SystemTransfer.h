// Created by amoylel on 08/20/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_SYSTEMTRANSFER_H__
#define AMO_SYSTEMTRANSFER_H__

#include <amo/singleton.hpp>

#include "transfer/ClassTransfer.hpp"

namespace amo {

    class SystemTransfer
        : public ClassTransfer
        , public amo::singleton < SystemTransfer > {
    public:
        SystemTransfer();
        
        Any info(IPCMessage::SmartType msg);
        Any screen(IPCMessage::SmartType msg);
        Any mouse(IPCMessage::SmartType msg);
        Any keyboard(IPCMessage::SmartType msg);
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(SystemTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(info, TransferFuncConstProperty | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(screen, TransferFuncConstProperty | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(mouse, TransferFuncConstProperty | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(keyboard, TransferFuncConstProperty | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_END()
    private:
    };
}

#endif // AMO_SYSTEMTRANSFER_H__

