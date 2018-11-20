// Created by amoylel on 06/30/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_RENDERMESSAGEEMITTER_HPP__
#define AMO_RENDERMESSAGEEMITTER_HPP__

#include "handler/CefProcessExchanger.hpp"
#include "ipc/Any.hpp"
#include "ipc/MessageLauncher.hpp"
#include <amo/json.hpp>
#include "handler/CefHeader.hpp"
#include "ipc/AsyncFunctionManager.hpp"

#include "transfer/TransferMgr.h"
#include "module/basic/TypeConvertor.h"
#include "module/JsClassV8Handler.h"
#include "transfer/RendererTransferMgr.h"

namespace amo {

    /*!
     * @class	RenderMessageEmitter
     *
     * @brief	向UI线程发送消息.
     */
    
    class RenderMessageEmitter
        : public MessageLauncher {
    public:
        RenderMessageEmitter(CefRefPtr<CefFrame> pFrame)
            : MessageLauncher(pFrame->GetBrowser()->GetIdentifier(),
                              pFrame->GetIdentifier())
            , m_pFrame(pFrame) {
        }
        
        ~RenderMessageEmitter() {}
        
        /*!
         * @fn	virtual bool RenderMessageEmitter::Exchange(int nPipeID, IPCMessage::SmartType msg) override
         *
         * @brief	Exchanges.
         *
         * @param	nPipeID	Identifier for the pipe.
         * @param	msg	   	The message.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool exchange(int nPipeID,
                              IPCMessage::SmartType msg) override {
            return RendererProcessExchangerManager::getInstance()->exchange(nPipeID, msg);
        }
        
        /*!
         * @fn	virtual Any RenderMessageEmitter::WaitResult(int nPipeID, int nMessageID)
         *
         * @brief	Wait result.
         *
         * @param	nPipeID   	Identifier for the pipe.
         * @param	nMessageID	Identifier for the message.
         *
         * @return	Any.
         */
        
        virtual Any waitResult(int nPipeID, int nMessageID) {
            return RendererProcessExchangerManager::getInstance()->waitResult<Any>(nPipeID,
                    nMessageID);
        }
        
        /*!
         * @fn	template<typename ... Args> Any RenderMessageEmitter::Transfer(std::string name, Args... args)
         *
         * @brief	使用自己进程的transfer执行函数，不需要向对方进程 返回数据.
         *
         * @tparam	Args	Type of the arguments.
         * @param	name	The name.
         * @param	args	Variable arguments providing the arguments.
         *
         * @return	Any.
         */
        
        template<typename ... Args>
        Any transfer(std::string name, Args... args) {
            msg->setMessageName(MSG_NATIVE_SYNC_EXECUTE);
            makeProcessMessage(msg, args...);
            afterMakeProcessMessage(msg, name);
            int nBrowserID = m_pFrame->GetBrowser()->GetIdentifier();
            msg->getArgumentList()->setValue(IPCArgsPosInfo::CrossProcessFlag, 1);
            return RendererTransferMgr::getInstance()->onMessageTransfer(msg);
        }
        
        /*!
         * @fn	virtual void RenderMessageEmitter::OnMakeProcessMessage(IPCMessage::SmartType msg, void* param) override
         *
         * @brief	Executes the make process message action.
         *
         * @param	msg			 	The message.
         * @param [in,out]	param	If non-null, the parameter.
         */
        
        virtual void onMakeProcessMessage(IPCMessage::SmartType msg,
                                          void* param) override {
            const CefV8ValueList* arguments = reinterpret_cast<const CefV8ValueList*>
                                              (param);
            std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
            int nIndex = 0;
            
            for (size_t i = 0; i < arguments->size(); ++i) {
                CefRefPtr<CefV8Value> pValue = arguments->at(i);
                
                
                std::string sssb = TypeConvertor::V8ValueType(pValue);
                
                if (pValue->IsFunction()) {
                
#if CHROME_VERSION_BUILD >= 2987
                    CefRefPtr<CefBaseRefCounted> pHandler = pValue->GetFunctionHandler();
#else
                    CefRefPtr<CefBase> pHandler = pValue->GetFunctionHandler();
#endif
                    
                    
                    // 如果pHander存在，那么说明当前函数是一个C++创建的函数
                    JsClassV8Handler* pClassHandler = NULL;
                    
                    if (pHandler) {
                        pClassHandler = dynamic_cast<JsClassV8Handler*>(pHandler.get());
                    }
                    
                    if (pClassHandler == NULL) {
                        pHandler = pValue->GetUserData();
                        
                        if (pHandler) {
                            pClassHandler = dynamic_cast<JsClassV8Handler*>(pHandler.get());
                        }
                        
                    }
                    
                    
                    
                    if (pHandler && pClassHandler != NULL) {
                    
                        CefString name = pValue->GetFunctionName();
                        std::string transferName = pClassHandler->getHandlerName();
                        int64_t transferID = pClassHandler->getID();
                        
                        args->setValue(IPCArgsPosInfo::ThreadTransferFuncName, name.ToString());
                        args->setValue(IPCArgsPosInfo::ThreadTransferID, transferID);
                        args->setValue(IPCArgsPosInfo::ThreadTransferName, transferName);
                        
                        
                    } else {
                        // 如果是函数，那么解析为一个回调函数
                        using MGR = AsyncFunctionManager < PID_RENDERER >;
                        auto manager = MGR::getInstance();
                        int nKey = manager->registerCallabackFunction(m_pFrame,
                                   pValue);
                        args->setValue(IPCArgsPosInfo::AsyncCallback, nKey);
                    }
                    
                    
                } else {
                    TypeConvertor convertor;
                    args->setValue(nIndex++, convertor.toAny(pValue));
                }
                
                //if (pValue->IsBool()) {
                //    args->setValue(nIndex++, pValue->GetBoolValue());
                //} else if (pValue->IsInt()) {
                //    args->setValue(nIndex++, pValue->GetIntValue());
                //} else if (pValue->IsDouble()) {
                //    args->setValue(nIndex++, pValue->GetDoubleValue());
                //} else if (pValue->IsString()) {
                //    args->setValue(nIndex++, pValue->GetStringValue().ToString());
                //} else if (pValue->IsNull()) {
                //    args->setValue(nIndex++, amo::Nil());
                //} else if (pValue->IsUndefined()) {
                //    args->setValue(nIndex++, Undefined());
                //} else if (pValue->IsFunction()) {
                //    // 如果是函数，那么解析为一个回调函数
                //    using MGR = AsyncFunctionManager < PID_RENDERER > ;
                //    auto manager = MGR::getInstance();
                //    int nKey = manager->RegisterCallabackFunction(m_pFrame,
                //               pValue);
                //    args->setValue(IPCArgsPosInfo::AsyncCallback, nKey);
                //} else if (pValue->IsObject()) {
                //    // 将Object转换为JSON字符串
                //    TypeConvertor convertor;
                //    convertor.toAny(pValue);
                //    std::string ss = convertor.ObjectToJson(pValue).to_string();
                //    args->setValue(nIndex++, ss);
                //}
            }
        }
        
        /*!
         * @fn	virtual bool RenderMessageEmitter::SendMessage(IPCMessage::SmartType msg) override
         *
         * @brief	Sends a message.
         *
         * @param	msg	The message.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool sendMessage(IPCMessage::SmartType msg) override {
            CefRefPtr<CefProcessMessage> message;
            message = CefProcessMessage::Create(msg->getName());
            message->GetArgumentList()->SetString(0, msg->toJson().to_string());
            
            return m_pFrame->GetBrowser()->SendProcessMessage(PID_BROWSER,
                    message);
        }
        
        /*!
         * @fn	virtual void RenderMessageEmitter::AfterMakeProcessMessage(IPCMessage::SmartType msg, const std::string& name) override
         *
         * @brief	After make process message.
         *
         * @param	msg 	The message.
         * @param	name	The name.
         */
        
        virtual void afterMakeProcessMessage(IPCMessage::SmartType msg,
                                             const std::string& name) override {
            return MessageLauncher::afterMakeProcessMessage(msg, name);
        }
        
    protected:
        /*! @brief	The frame. */
        CefRefPtr<CefFrame> m_pFrame;
    };
}

#endif // AMO_RENDERMESSAGEEMITTER_HPP__

