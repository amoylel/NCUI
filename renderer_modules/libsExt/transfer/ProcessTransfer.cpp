#include "stdafx.h"
#include "transfer/ProcessTransfer.h"
#include <amo/process.hpp>
#include <TlHelp32.h>


namespace amo {

    ProcessTransfer::ProcessTransfer()
        : RunnableTransfer("Process") {
        
    }
    
    ProcessTransfer::ProcessTransfer(const std::string& strAppPath)
        : RunnableTransfer("Process") {
        m_pPorcess.reset(new amo::process(strAppPath));
    }
    
    Any ProcessTransfer::onCreateClass(IPCMessage::SmartType msg) {
        amo::string str(msg->getArgumentList()->getString(0), true);
        auto pTransfer = ClassTransfer::createTransfer<ProcessTransfer>(str.str());
        pTransfer->setTriggerEventFunc(this->getTriggerEventFunc());
        pTransfer->setDefaultTriggerEventFunc(this->getDefaultTriggerEventFunc());
        return pTransfer->getFuncMgr().toSimplifiedJson();
    }
    
    std::string ProcessTransfer::getClass() const {
        return "Process";
    }
    
    amo::Transfer* ProcessTransfer::getInterface(const std::string& name) {
        if (name == ProcessTransfer::getClass()) {
            return this;
        }
        
        return RunnableTransfer::getInterface(name);
    }
    
    
    Any ProcessTransfer::findPIDByName(IPCMessage::SmartType msg) {
        amo::string str(msg->getArgumentList()->getString(0), true);
        std::vector<Any> vec;
        auto ids =   amo::process::find_pid_by_process_name(str.str());
        
        for (auto& p : ids) {
            vec.push_back(p);
        }
        
        return vec;
        
    }
    
    Any ProcessTransfer::terminateProcessByName(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        amo::string processName(args->getString(0), true);
        
        amo::process::terminate_process_by_name(processName.to_ansi());
        return Undefined();
    }
    
    
    Any ProcessTransfer::killProcessByName(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        bool killSubProcess = true;
        amo::string processName(args->getString(0), true);
        
        if (args->isValid(1) && args->getValue(1).is<bool>()) {
            killSubProcess = args->getValue(1).As<bool>();
        }
        
        return  amo::process::kill_process_by_name(processName, killSubProcess);
    }
    
    
    
    Any ProcessTransfer::start(IPCMessage::SmartType msg) {
        if (!m_pPorcess) {
            return false;
        }
        
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        amo::string params(args->getString(0), true);
        return  m_pPorcess->start(params);
    }
    
    Any ProcessTransfer::getResult(IPCMessage::SmartType msg) {
        if (!m_pPorcess) {
            return Undefined();
        }
        
        TransferEventInfo info;
        info.name = "process.result";
        info.data = (amo::u8json)(m_pPorcess->getResult()->toJson());
        triggerEvent(info);
        return  Undefined();
    }
    
}


