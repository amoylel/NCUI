// Created by amoylel on 07/04/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODEPROCESSHANDLER_H__
#define AMO_NODEPROCESSHANDLER_H__

#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>
#include <string>

#include <node.h>
#include <node_object_wrap.h>
#include <uv.h>
#include "transfer/FunctionWrapper.hpp"
#include <amo/loader.hpp>
#include "ipc/NodeMessageEmitter.hpp"

#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <vector>

namespace amo {

    using namespace boost::interprocess;
    
    using v8::Context;
    using v8::Function;
    using v8::FunctionCallbackInfo;
    using v8::FunctionTemplate;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object;
    using v8::Persistent;
    using v8::String;
    using v8::Value;
    
    class NodeProcessHandler {
    public:
        ~NodeProcessHandler();
        static void Initialize();
        
        static void UnInitialize();
        static std::string ObjectToString(Local<Value> value);
        static void setMessageQueue(const std::string& strName);
        static void closeMessageQueue();
        static void wait_for_a_while(uv_idle_t* handle);
        static bool ProcessSyncMessage(int nID, IPCMessage::SmartType anyMessage);
        static bool SendMessageToNode(IPCMessage::SmartType anyMessage);
        static bool SendMessageToUI(IPCMessage::SmartType anyMessage);
        static void OnNativeMessageRecv(IPCMessage::SmartType anyMessage);
        static void OnBeforeNativeMessage(uv_work_t* r);
        static void OnNativeMessageEnd(uv_work_t* r);
        static std::shared_ptr<NodeMessageEmitter> getNativeRunner();
        static std::unordered_map<std::string, FunctionWrapperMgr >& getClassMap();
    public:
        static std::shared_ptr<amo::loader> m_loader;												//!< 动态库函数调用器
        static std::unordered_map<std::string, FunctionWrapperMgr > m_oClassMap;			//!< 类所对应的函数
        static uv_idle_t idler;																		//!< 监视器，防止NodeJS退出
        static const int m_nPipeID = -9999;															//!< 用于同步消息时的管道ID
        static std::shared_ptr<message_queue>  m_pMessageQueueReader;
        static std::shared_ptr<message_queue> m_pMessageQueueWriter;
        static std::string m_strMessageQueueName;
    };
}

#endif // AMO_NODEPROCESSHANDLER_H__

