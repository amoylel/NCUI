// Created by amoylel on 07/19/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_COMMANDLINETRANSFER_H__
#define AMO_COMMANDLINETRANSFER_H__

#include <amo/singleton.hpp>

#include "handler/CefHeader.hpp"
#include "transfer/ClassTransfer.hpp"


namespace amo {

    class CommandLineTransfer
        : public ClassTransfer
        , public amo::singleton < CommandLineTransfer > {
    public:
        CommandLineTransfer();
        CommandLineTransfer(CefRefPtr<CefCommandLine> pCommand);
        
        
        Any IsValid(IPCMessage::SmartType msg);
        Any IsReadOnly(IPCMessage::SmartType msg);
        Any Copy(IPCMessage::SmartType msg);
        Any InitFromArgv(IPCMessage::SmartType msg);
        Any InitFromString(IPCMessage::SmartType msg);
        Any Reset(IPCMessage::SmartType msg);
        Any GetArgv(IPCMessage::SmartType msg);
        Any GetCommandLineString(IPCMessage::SmartType msg);
        Any GetProgram(IPCMessage::SmartType msg);
        Any SetProgram(IPCMessage::SmartType msg);
        Any HasSwitches(IPCMessage::SmartType msg);
        Any HasSwitch(IPCMessage::SmartType msg);
        Any GetSwitchValue(IPCMessage::SmartType msg);
        Any GetSwitches(IPCMessage::SmartType msg);
        Any AppendSwitch(IPCMessage::SmartType msg);
        Any AppendSwitchWithValue(IPCMessage::SmartType msg);
        Any HasArguments(IPCMessage::SmartType msg);
        Any GetArguments(IPCMessage::SmartType msg);
        Any AppendArgument(IPCMessage::SmartType msg);
        Any PrependWrapper(IPCMessage::SmartType msg);
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(CommandLineTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(IsValid, TransferExecSync)
        
        AMO_CEF_MESSAGE_TRANSFER_END()
    private:
        CefRefPtr<CefCommandLine> m_pCommandline;
    };
}

#endif // AMO_COMMANDLINETRANSFER_H__
