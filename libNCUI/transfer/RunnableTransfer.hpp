// Created by amoylel on 11/03/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_RUNABLETRANSFER_HPP__
#define AMO_RUNABLETRANSFER_HPP__

#include <memory>
#include <functional>

#include "transfer/ClassTransfer.hpp"
#include <amo/string.hpp>
#include <amo/json.hpp>
#include "transfer/TransferEventInfo.hpp"
#include "transfer/TransferHelper.hpp"
namespace amo {

    /**
     * @class	Runnable
     *
     * @brief	如果一个类想要在线程中运行的话，需要继承自Runable.<br>
     * 			工作线程:**UI线程** 或 **Renderer线程**.
     *
     * @extend Object
     */
    
    class RunnableTransfer : public ClassTransfer {
    public:
        typedef std::function<void(std::shared_ptr<RunnableTransfer>, const TransferEventInfo&)>
        EventCallbackFunc;
        
        
        
    public:
        RunnableTransfer()
            : ClassTransfer("Runnable") {
            m_nThreadID = 0;
            addModule("EventEmitter");
        }
        RunnableTransfer(const std::string& name)
            : ClassTransfer(name) {
            addModule("EventEmitter");
        }
        virtual std::string getClass() const {
            return "Runnable";
        }
        
        virtual Transfer* getInterface(const std::string& name) {
            if (name == RunnableTransfer::getClass()) {
                return this;
            }
            
            return ClassTransfer::getInterface(name);
        }
        
        virtual Any onMessageTransfer(IPCMessage::SmartType msg) override {
            std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
            
            if (multiType(args->getString(IPCArgsPosInfo::FuncName)) ==
                    TransferMultiDisabled) {
                // new 不能在多线程中执行
                // weakup 实际上是thread上的函数，这里是为了方便
                return ClassTransfer::onMessageTransfer(msg);
            } else if (m_nThreadID == 0) {
                return ClassTransfer::onMessageTransfer(msg);
            } else if (args->isValid(IPCArgsPosInfo::ThreadTransferFuncName)
                       && args->isValid(IPCArgsPosInfo::ThreadTransferID)) {
                return ClassTransfer::onMessageTransfer(msg);
            } else {
            
                auto transfer = ClassTransfer::findTransfer(m_nThreadID);
                
                if (transfer == NULL) {
                    return Undefined();
                }
                
                IPCMessage::SmartType ipcMessage = msg->clone();
                ipcMessage->setMessageName(MSG_NATIVE_EXECUTE);
                
                std::shared_ptr<AnyArgsList> ipcArgs = ipcMessage->getArgumentList();
                
                ipcArgs->setValue(IPCArgsPosInfo::ThreadTransferFuncName,
                                  args->getString(IPCArgsPosInfo::FuncName));
                ipcArgs->setValue(IPCArgsPosInfo::ThreadTransferID,
                                  args->getString(IPCArgsPosInfo::TransferID));
                ipcArgs->setValue(IPCArgsPosInfo::ThreadTransferName,
                                  args->getString(IPCArgsPosInfo::TransferName));
                                  
                                  
                ipcArgs->setValue(IPCArgsPosInfo::TransferID, transfer->getObjectID());
                ipcArgs->setValue(IPCArgsPosInfo::TransferName, transfer->transferName());
                ipcArgs->setValue(IPCArgsPosInfo::FuncName, "exec");
                
                return transfer->onMessageTransfer(ipcMessage);
            }
        }
        
        /**
         * @fn	Any RunnableTransfer::attach(IPCMessage::SmartType msg)
         * @tag single
         * @brief	将当前对象附加到一个线程中.
         *
         * @param	#Object 需要附加的线程
         *
         * @return	#Boolean true成功/false失败.
         * @example
         *
         ```
        	includes('Demo', 'Thread');
        	var demo = new Demo();
        	var thread = new Thread();
        	console.log(demo.add(1,2));
        	demo.attach(thread);
        	demo.unique('Demo.add',function(info){
        		console.log(info);
        		console.log('在线程中执行函数的返回结果:' + info.data);
        	});
        	console.log(demo.add(1,2));
         ```
         */
        
        Any attach(IPCMessage::SmartType msg) {
        
            std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
            int64_t nObjectID = args->getInt64(0);
            auto transfer =  ClassTransfer::findTransfer(nObjectID);
            
            
            if (transfer == NULL) {
                return false;
            }
            
            if (transfer->getInterface("ThreadBase") == NULL) {
                return false;
            }
            
            m_nThreadID = nObjectID;
            return true;
            
        }
        
        /**
         * @fn	Any RunnableTransfer::detach(IPCMessage::SmartType msg)
         * @tag single
         * @brief	将当前对象从一个线程中分离.
         *
         * @return	无.
         *
         * @example
         *
         ```
        	 includes('Demo', 'Thread');
        	 var demo = new Demo();
        	 var thread = new Thread();
        	 console.log(demo.add(1,2));
        	 demo.attach(thread);
        	 demo.unique('Demo.add',function(info){
        		 console.log(info);
        		 console.log('在线程中执行函数的返回结果:' + info.data);
        	 });
        	 console.log(demo.add(1,2));
        	 demo.detach();
        	 console.assert(demo.add(1,3) == 4, true);
         ```
         */
        
        Any detach(IPCMessage::SmartType msg) {
            m_nThreadID = 0;
            return Undefined();
        }
        
        /**
         * @fn	Any RunnableTransfer::weakup(IPCMessage::SmartType msg)
         * @tag single
         * @brief	唤醒当前对象所附加的线程.
         *
         * @return	无.
         * @example
         *
         ```
        	 includes('Demo', 'Thread');
        	 var demo = new Demo();
        	 var thread = new Thread();
        	 console.log(demo.add(1,2));
        	 demo.attach(thread);
        	 demo.sleepForWeakup();
        	 setTimeout(function(){
        		demo.weakup();
        	 }, 3000);
        	 demo.unique('Demo.add',function(info){
        		 console.log(info);
        		 console.log('在线程中执行函数的返回结果:' + info.data);
        	 });
        	 console.log(demo.add(1,3));
        
         ```
         */
        
        Any weakup(IPCMessage::SmartType msg) {
            if (m_nThreadID == 0) {
                return Undefined();
            }
            
            auto transfer = ClassTransfer::findTransfer(m_nThreadID);
            
            
            if (transfer == NULL) {
                return Undefined();
            }
            
            if (transfer->getInterface("ThreadBase") == NULL) {
                return Undefined();
            }
            
            IPCMessage::SmartType ipcMessage = msg->clone();
            ipcMessage->setMessageName(MSG_NATIVE_EXECUTE);
            std::shared_ptr<AnyArgsList> ipcArgs = ipcMessage->getArgumentList();
            ipcArgs->setValue(IPCArgsPosInfo::TransferID, transfer->getObjectID());
            ipcArgs->setValue(IPCArgsPosInfo::TransferName, transfer->transferName());
            ipcArgs->setValue(IPCArgsPosInfo::FuncName, "weakup");
            return transfer->onMessageTransfer(ipcMessage);
        }
        
        /**
         * @fn	Any RunnableTransfer::suspend(IPCMessage::SmartType msg)
         *
         * @brief	挂起所附加线程，不能在浏览器线程（UI/Renderer）上执行.
         *
         * @return	无.
         * @example
         *
         ```
        	includes('Demo', 'Thread');
        	 var demo = new Demo();
        	 var thread = new Thread();
        	 console.log(demo.add(1,2));
        	 demo.attach(thread);
        	 demo.suspend();   // 如果没有attach，不能在渲染线程上调用该函数，否则会阻塞渲染线程
        	 setTimeout(function(){
        		demo.weakup();
        	 }, 3000);
        	 demo.unique('Demo.add',function(info){
        		// 3秒后才能得到结果
        		 console.log(info);
        		 console.log('在线程中执行函数的返回结果:' + info.data);
        	 });
        
        	 demo.add(1,4);
        
         ```
         */
        
        Any suspend(IPCMessage::SmartType msg) {
            if (m_nThreadID == 0) {
                return Undefined();
            }
            
            auto transfer = ClassTransfer::findTransfer(m_nThreadID);
            
            
            if (transfer == NULL) {
                return Undefined();
            }
            
            if (transfer->getInterface("ThreadBase") == NULL) {
                return Undefined();
            }
            
            IPCMessage::SmartType ipcMessage = msg->clone();
            ipcMessage->setMessageName(MSG_NATIVE_EXECUTE);
            std::shared_ptr<AnyArgsList> ipcArgs = ipcMessage->getArgumentList();
            ipcArgs->setValue(IPCArgsPosInfo::TransferID, transfer->getObjectID());
            ipcArgs->setValue(IPCArgsPosInfo::TransferName, transfer->transferName());
            ipcArgs->setValue(IPCArgsPosInfo::FuncName, "suspend");
            
            return transfer->onMessageTransfer(ipcMessage);
            
        }
        
        /*!
         * @fn	Any RunnableTransfer::sendEvent(const TransferEventInfo& info)
         *
         * @ignore
         *
         * @brief	向页面发送数据，如果要求线程挂起的话，暂停当前线程.
         *
         * @param	info	The information.
         *
         * @return	Any.
         */
        
        Any sendEvent(const TransferEventInfo& info) {
            triggerEvent(info);
            
            if (info.suspend) {
                return suspend(IPCMessage::Empty());
            }
            
            return Undefined();
        }
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(RunnableTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(attach,
                                      TransferMultiDisabled | TransferFuncNormal | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(detach,
                                      TransferMultiDisabled | TransferFuncNormal | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(suspend, TransferFuncNormal | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(weakup,
                                      TransferMultiDisabled | TransferFuncNormal | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    protected:
        /** @brief	事件回调函数. */
        //EventCallbackFunc m_fnEventCallback;
        
        /*! @brief	附加到的线程ID. */
        int64_t m_nThreadID;
    };
    
    
    
}

#endif // AMO_RUNABLETRANSFER_HPP__