// Created by amoylel on 07/10/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_IPCNODEV8HANDLER_H__
#define AMO_IPCNODEV8HANDLER_H__



#include <string>
#include <amo/unordered_map.hpp>
#include "transfer/ClassTransfer.hpp"

namespace amo {

    // JS 异步调用C++ 结果回调函数处理类
    class IPCNodeV8Handler   : public ClassTransfer {
    
    public:
        IPCNodeV8Handler();
        
        virtual ~IPCNodeV8Handler();
        
        // 异步调用C++结果回调处理函数
        amo::Any asyncExecuteResult(IPCMessage::SmartType msg);
        
        // 调用JS函数
        amo::Any runJSFunction(IPCMessage::SmartType msg);
        
        // 运行JS片段
        amo::Any runJsFragment(IPCMessage::SmartType msg);
        
        // 触发事件
        amo::Any triggerEvent(IPCMessage::SmartType msg);
        
        // 触发所有当前Browser中所有Frame注册的事件 ，指定Browser， 不指定Frame
        amo::Any emitEventAllFrame(IPCMessage::SmartType msg);
        
        // 获取参数个数
        int getArgsSize(std::shared_ptr<AnyArgsList> args) const;
        
        // for transfer
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(IPCNodeV8Handler, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(asyncExecuteResult, 0)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(runJSFunction, 0)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(runJsFragment, 0)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(triggerEvent, 0)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(emitEventAllFrame, 0)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
        
        
    protected:
    
    
    };
}



#endif // AMO_IPCNODEV8HANDLER_H__
