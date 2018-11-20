#include "stdafx.h"
#include "transfer/CommandLineTransfer.h"


namespace amo {

    CommandLineTransfer::CommandLineTransfer()
        : ClassTransfer("CommandLine") {
        
    }
    
    CommandLineTransfer::CommandLineTransfer(CefRefPtr<CefCommandLine> pCommand)
        : ClassTransfer("CommandLine")
        , m_pCommandline(pCommand) {
        
    }
    
    Any CommandLineTransfer::IsValid(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any CommandLineTransfer::IsReadOnly(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any CommandLineTransfer::Copy(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any CommandLineTransfer::InitFromArgv(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any CommandLineTransfer::InitFromString(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any CommandLineTransfer::Reset(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any CommandLineTransfer::GetArgv(IPCMessage::SmartType msg) {
        return Undefined();
        
    }
    
    Any CommandLineTransfer::GetCommandLineString(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any CommandLineTransfer::GetProgram(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any CommandLineTransfer::SetProgram(IPCMessage::SmartType msg) {
        return Undefined();
        
    }
    
    Any CommandLineTransfer::HasSwitches(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any CommandLineTransfer::HasSwitch(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any CommandLineTransfer::GetSwitchValue(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any CommandLineTransfer::GetSwitches(IPCMessage::SmartType msg) {
        return Undefined();
        
    }
    
    Any CommandLineTransfer::AppendSwitch(IPCMessage::SmartType msg) {
        return Undefined();
        
    }
    
    Any CommandLineTransfer::AppendSwitchWithValue(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any CommandLineTransfer::HasArguments(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any CommandLineTransfer::GetArguments(IPCMessage::SmartType msg) {
        return Undefined();
        
    }
    
    Any CommandLineTransfer::AppendArgument(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any CommandLineTransfer::PrependWrapper(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
}