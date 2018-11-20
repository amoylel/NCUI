#include "stdafx.h"
#include "transfer/InterProcessTransfer.h"
#include <amo/ipc.hpp>

namespace amo {

    InterProcessTransfer::InterProcessTransfer()
        : RunnableTransfer("InterProcess") {
        addModule("EventEmitter");
        
    }
    
    InterProcessTransfer::InterProcessTransfer(const amo::u8json& json) :
        RunnableTransfer("InterProcess") {
        addModule("EventEmitter");
        settings = json;
        
        amo::astring message_queue = settings.getString("message_queue");
        bool bClient = settings.getBool("client");
        
        typedef  std::shared_ptr<std::function<any(ipc_msg::SmartType)> >
        smart_event_fn_type;
        smart_event_fn_type fn(new std::function<any(ipc_msg::SmartType)>(std::bind(
                                   &InterProcessTransfer::onMsg, this,
                                   std::placeholders::_1)));
                                   
        if (!message_queue.empty()) {
            if (bClient) {
                m_client.reset(new amo::ipc::client(message_queue));
                m_client->setDefaultMsgFunc(fn);
            } else {
                m_server.reset(new amo::ipc::server(message_queue));
                m_server->setDefaultMsgFunc(fn);
            }
            
            setObjectName(message_queue);
        }
        
        
    }
    
    Any InterProcessTransfer::onCreateClass(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        amo::u8json settings = args->getJson(0);
        auto pTransfer = ClassTransfer::createTransfer<InterProcessTransfer>(settings);
        return pTransfer->getFuncMgr().toSimplifiedJson();
    }
    
    std::string InterProcessTransfer::getClass() const {
        return "InterProcess";
    }
    
    amo::Transfer * InterProcessTransfer::getInterface(const std::string & name) {
        if (name == InterProcessTransfer::getClass()) {
            return this;
        }
        
        return RunnableTransfer::getInterface(name);
    }
    
    
    
    Any InterProcessTransfer::connect(IPCMessage::SmartType msg) {
        int delay = msg->getArgumentList()->getInt(0);
        
        if (delay <= 3) {
            delay = 3;
        }
        
        bool bConnect = false;
        
        if (m_server) {
            bConnect = m_server->wait(delay);
        }
        
        if (m_client) {
            bConnect = m_client->connect();
        }
        
        TransferEventInfo info;
        info.name = "interprocess.connect";
        info.toAll = false;
        info.data = bConnect;
        
        triggerEvent(info);
        return bConnect;
    }
    
    Any InterProcessTransfer::exec(IPCMessage::SmartType msg) {
        if (m_server) {
            m_server->sendMessage(msg->toJson().to_string());
        }
        
        if (m_client) {
            m_client->sendMessage(msg->toJson().to_string());
        }
        
        return Undefined();
    }
    
    amo::any InterProcessTransfer::onMsg(amo::ipc_msg::SmartType msg) {
        try {
            auto args = msg->getArgumentList();
            amo::u8json json = msg->getArgumentList()->getJson(1);
            size_t size = args->getArgsSize();
            std::vector<Any> vec;
            
            for (size_t i = 0; i < size; ++i) {
                amo::any& v = args->getValue(i);
                Any val(v.type(), v.value());
                vec.push_back(val);
            }
            
            TransferEventInfo info;
            info.name = args->getString(ipc_args_pos_info::FuncName);
            info.toAll = true;
            info.data = vec;
            triggerEvent(info);
            return undefined();
        } catch (const std::exception&e) {
            $cerr(e.what());
        }
        
        return undefined();
    }
    
}

