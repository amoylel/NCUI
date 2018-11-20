#include "stdafx.h"
#include "transfer/ProcessTransfer.h"
#include <amo/process.hpp>


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
        pTransfer->setTriggerEventFunc(this->getTriggerEventFunc());pTransfer->setDefaultTriggerEventFunc(this->getDefaultTriggerEventFunc());
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
    
    Any ProcessTransfer::start(IPCMessage::SmartType msg) {
        if (!m_pPorcess) {
            return false;
        }
        
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        amo::string params = args->getString(0);
        return  m_pPorcess->start(params);
    }
    
    Any ProcessTransfer::getResult(IPCMessage::SmartType msg) {
        if (!m_pPorcess) {
            return Undefined();
        }
        
        TransferEventInfo info;
        info.name = "result";
        info.data = m_pPorcess->getResult()->toJson();
        triggerEvent(info);
        return  Undefined();
    }
    
}


