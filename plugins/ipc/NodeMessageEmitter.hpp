// Created by amoylel on 06/30/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODEMESSAGEEMITTER_HPP__
#define AMO_NODEMESSAGEEMITTER_HPP__




#include <memory>
#include <functional>
#include <v8.h>
#include "ipc/Any.hpp"
#include "ipc/MessageLauncher.hpp"
#include <amo/loader.hpp>

namespace amo {

    // 向NodeJS发送消息
    class NodeMessageEmitter : public MessageLauncher {
    public:
    
        NodeMessageEmitter(int nPipeID = -9999, int nFrameID = -1);
        
        ~NodeMessageEmitter();
        
        std::string ObjectToString(v8::Local<v8::Value> value);
        
        virtual void onMakeProcessMessage(IPCMessage::SmartType msg, void* param) override;
        
        virtual bool exchange(int nPipeID, IPCMessage::SmartType msg) override;
        
        virtual amo::Any waitResult(int nPipeID, int nMessageID);
        
        virtual bool sendMessage(IPCMessage::SmartType anyMessage) override;
        
    protected:
    };
}
#endif // AMO_NODEMESSAGEEMITTER_HPP__
