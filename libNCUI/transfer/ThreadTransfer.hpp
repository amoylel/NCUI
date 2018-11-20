// Created by amoylel on 09/27/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_THREADTRANSFER_HPP__
#define AMO_THREADTRANSFER_HPP__

#include "transfer/ClassTransfer.hpp"


#include <memory>
#include <amo/looper_executor.hpp>
#include "transfer/TransferMgr.h"
#include "transfer/TransferEventInfo.hpp"
#include "transfer/RunnableTransfer.hpp"
#include <amo/launch.hpp>

namespace amo {
    enum ThreadEnum {
        ThreadUI,
        ThreadRenderer,
    };
    
    class ThreadBaseTransfer : public ClassTransfer {
    public:
        ThreadBaseTransfer()
            : ClassTransfer("ThreadBase") {
            addModule("EventEmitter");
        }
        
        ThreadBaseTransfer(const std::string& className)
            : ClassTransfer(className) {
            addModule("EventEmitter");
        }
        virtual std::string getClass() const override {
            return "ThreadBase";
        }
        
        virtual Transfer* getInterface(const std::string& name) override {
            if (name == ThreadBaseTransfer::getClass()) {
                return this;
            }
            
            return ClassTransfer::getInterface(name);
        }
        
    };
    
    
    /*!
    * @class	ThreadBase
    *
    * @unexport
    *
    * @brief	线程基类.
    */
    
    template<ThreadEnum>
    class ThreadTransfer : public ThreadBaseTransfer {
    public:
        ThreadTransfer()
            : ThreadBaseTransfer("ThreadBase") {
            m_nBrowserID = m_nFrameID = 0;
            m_isPausedThread = false;
            m_pLooperExecutor = getWorkThread();
            m_pLooperExecutor->set_exception_callback(
                std::bind(&ThreadTransfer::onExecption,
                          this,
                          std::placeholders::_1,
                          std::placeholders::_2));
            m_weakupData = Undefined();
            
        }
        
        ThreadTransfer(const std::string& className)
            : ThreadBaseTransfer(className) {
            m_nBrowserID = m_nFrameID = 0;
            m_isPausedThread = false;
            m_pLooperExecutor = getWorkThread();
            m_pLooperExecutor->set_exception_callback(
                std::bind(&ThreadTransfer::onExecption,
                          this,
                          std::placeholders::_1,
                          std::placeholders::_2));
            m_weakupData = Undefined();
        }
        
        static std::shared_ptr<amo::looper_executor>& getWorkThread() {
            static std::shared_ptr<amo::looper_executor> pLooperExecutor;
            
            if (!pLooperExecutor) {
                pLooperExecutor.reset(new amo::looper_executor());
            }
            
            return pLooperExecutor;
        }
        
        
        
        virtual void onBeforeRelease() override {
            if (m_pLooperExecutor) {
                m_pLooperExecutor->kill();
            }
            
            m_pLooperExecutor.reset();
        }
        
        // 创建一个新线程
        
        std::shared_ptr<amo::looper_executor> createThread() {
            m_pLooperExecutor.reset(new amo::looper_executor());
            return m_pLooperExecutor;
        }
        
        virtual  std::shared_ptr< TransferMgr> getTransferMgr() {
            return std::shared_ptr< TransferMgr>();
        }
        
        // 获取所有线程
        Any getAllThreads(IPCMessage::SmartType msg) {
            return Undefined();
        }
        
        /*!
        * @fn	Any ThreadTransfer::Exec(IPCMessage::SmartType msg)
        *
        * @tag static single
        *
        * @brief	在默认线程中执行一个函数.
        *
        * @param	#Function 要执行的函数.
        *
        * @param	#Args...	该函数需要传入的参数，具体参数及个数以需要调用的函数为准.
        *
        * @return	无.
        */
        
        Any Exec(IPCMessage::SmartType msg) {
            return execute(msg, false);
        }
        
        /*!
        * @fn	Any ThreadTransfer::Sync(IPCMessage::SmartType msg)
        *
        * @tag static sync single
        *
        * @brief	在默认线程中同步执行一个函数.
        *
        * @param	#Function 需要执行的函数.
        *
        * @param	#Args=...	该函数需要传入的参数，具体参数及个数以需要调用的函数为准.
        *
        * @return	#Any 该函数的执行结果.
        */
        
        Any Sync(IPCMessage::SmartType msg) {
            return execute(msg, true);
        }
        
        
        
        
        /*!
         * @fn	Any ThreadTransfer::weakup(IPCMessage::SmartType msg)
         * @tag single
         * @brief	唤醒线程，只能在浏览器线程（UI/Renderer）上执行.
         * @param  #Any 传递唤醒参数，该值可以做为suppend的返回值，可用于线程上的扩展与页面交换数据
         *
         * @return	无.
         */
        
        Any weakup(IPCMessage::SmartType msg) {
            m_weakupData = msg->getArgumentList()->getValue(0);
            weakupThread();
            return Undefined();
        }
        
        /*!
         * @fn	Any ThreadTransfer::suspend(IPCMessage::SmartType msg)
         * @tag
         * @brief	暂停线程，不能在浏览器线程（UI/Renderer）上执行.
         *
         *
         * @return	#Any weakup函数的第一个参数.
         */
        
        Any suspend(IPCMessage::SmartType msg) {
            if (!m_pLooperExecutor) {
                return Undefined();
            }
            
            // 只能在工作线程中挂起自己
            if (m_pLooperExecutor->get_id() != std::this_thread::get_id()) {
                return false;
            }
            
            return waitForWeakUp();
        }
        
        
        IPCMessage::SmartType makeIPCMessage(IPCMessage::SmartType msg) {
            std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
            std::string transferName = args->getString(IPCArgsPosInfo::ThreadTransferName);
            int64_t transferID = args->getInt64(IPCArgsPosInfo::ThreadTransferID);
            std::string funcName = args->getString(IPCArgsPosInfo::ThreadTransferFuncName);
            
            IPCMessage::SmartType ipcMsg = msg->clone();
            ipcMsg->setMessageName(MSG_NATIVE_EXECUTE);
            ipcMsg->getArgumentList()->setValue(IPCArgsPosInfo::TransferName, transferName);
            ipcMsg->getArgumentList()->setValue(IPCArgsPosInfo::TransferID, transferID);
            ipcMsg->getArgumentList()->setValue(IPCArgsPosInfo::FuncName, funcName);
            return ipcMsg;
            
        }
        
        Any execute(IPCMessage::SmartType msg, bool bSync = false) {
            if (!m_pLooperExecutor) {
                return Undefined();
            }
            
            std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
            std::string transferName = args->getString(IPCArgsPosInfo::ThreadTransferName);
            int nBrowserID = args->getInt(IPCArgsPosInfo::BrowserID);
            int64_t nFrameID = args->getInt64(IPCArgsPosInfo::FrameID);
            auto manager = getTransferMgr();
            
            if (manager == NULL) {
                return Undefined();
            }
            
            IPCMessage::SmartType ipcMsg = makeIPCMessage(msg);
            
            
            Transfer* pTransfer = manager->findTransfer(nBrowserID, transferName);
            
            if (pTransfer == NULL) {
                return Undefined();
            }
            
            //// 要执行的trnasfer必须继承RunnableTransfer;
            //RunnableTransfer* pRunnableTransfer = dynamic_cast<RunnableTransfer*>(pTransfer);
            //
            //if (pRunnableTransfer == NULL) {
            //    return Undefined();
            //}
            //
            ////TODO: 这样做的话，一个Transfer就只能与一个线程挂钩，否则会出错
            //pRunnableTransfer->setWeakup(std::bind(&ThreadTransfer::weakupThread, this));
            //pRunnableTransfer->setSuspend(std::bind(&ThreadTransfer::waitForWeakUp, this));
            
            if (bSync) {
                Any ret = amo::sync<Any>(m_pLooperExecutor, [ = ]()->Any {
                    Transfer* pTransfer = manager->findTransfer(nBrowserID, transferName);
                    
                    if (pTransfer == NULL) {
                        return Undefined();
                    }
                    
                    return pTransfer->onMessageTransfer(ipcMsg);
                });
                return ret;
            } else {
            
                m_pLooperExecutor->execute([ = ]() {
                    Transfer* pTransfer = manager->findTransfer(nBrowserID, transferName);
                    
                    if (pTransfer == NULL) {
                        return Undefined();
                    }
                    
                    Any ret =    pTransfer->onMessageTransfer(ipcMsg);
                    std::string funcName = ipcMsg->getArgumentList()->getString(
                                               IPCArgsPosInfo::FuncName);
                                               
                                               
                    int64_t nID = ipcMsg->getArgumentList()->getInt64(IPCArgsPosInfo::TransferID);
                    std::shared_ptr<ClassTransfer> pObject = findTransfer(nID);
                    
                    if (pObject) {
                        TransferEventInfo info;
                        info.name = transferName + "." + funcName;
                        info.toAll = true;
                        info.setData(ret);
                        pObject->triggerEvent(info);
                    }
                    
                    
                    
                    return Undefined();
                });
            }
            
            return Undefined();
        }
        
        /*!
         * @fn	Any ThreadTransfer::exec(IPCMessage::SmartType msg)
         * @tag single
         *
         * @brief	执行一个函数.
         *
         * @param	#Function 要执行的函数.
         *
         * @return	无.
         */
        
        Any exec(IPCMessage::SmartType msg) {
            return execute(msg, false);
        }
        
        /*!
         * @fn	Any ThreadTransfer::sync(IPCMessage::SmartType msg)
         * @tag sync single
         *
         * @brief	同步调用一个函数，并返回该函数的执行结果.
         *
         * @param	#Function 要同步调用的函数.
         *
         * @return	#Any.
         */
        
        Any sync(IPCMessage::SmartType msg) {
            return execute(msg, true);
        }
        
        /*!
         * @fn	Any ThreadTransfer::kill(IPCMessage::SmartType msg)
         * @tag single
         *
         * @brief	杀死当前线程.
         *
         *
         * @return	无.
         */
        
        Any kill(IPCMessage::SmartType msg) {
            if (m_pLooperExecutor) {
                m_pLooperExecutor->kill();
                m_pLooperExecutor.reset();
            }
            
            return Undefined();
        }
        
        /*!
         * @fn	Any ThreadTransfer::start(IPCMessage::SmartType msg)
         * @tag single
         *
         * @brief	启动线程，该函数一般不需要调用，线程会在创建时自动启动，除非你调用了stop.
         *
         *
         * @return	#Boolean 是否启动成功.
         */
        
        Any start(IPCMessage::SmartType msg) {
            if (m_pLooperExecutor) {
                m_pLooperExecutor->start();
                return true;
                
            }
            
            return false;
            
        }
        
        /*!
         * @fn	Any ThreadTransfer::stop(IPCMessage::SmartType msg)
         * @tag single
         * @brief	停止当前线程.
         *
         *
         * @return	#Boolean 是否停止成功.
         */
        
        Any stop(IPCMessage::SmartType msg) {
            if (m_pLooperExecutor) {
                m_pLooperExecutor->stop();
                return true;
            }
            
            return false;
        }
        
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(ThreadTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(weakup,
                                      TransferMultiDisabled | TransferFuncNormal | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(suspend, TransferFuncNormal | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(exec,
                                      TransferMultiDisabled | TransferFuncNormal | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(sync,
                                      TransferMultiDisabled | TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(start,
                                      TransferMultiDisabled | TransferFuncNormal | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(stop,
                                      TransferMultiDisabled | TransferFuncNormal | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(kill,
                                      TransferMultiDisabled | TransferFuncNormal | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(Exec,
                                      TransferMultiDisabled | TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(Sync,
                                      TransferMultiDisabled | TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
        
        
        
    public:
    
        Any waitForWeakUp() {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            m_isPausedThread = true; // 暂停线程
            // 等待其他线程将该线程恢复
            m_condition_any.wait(lock,
                                 amo::bind(&ThreadTransfer::isResumeThread, this));
            Any ret = m_weakupData;
            m_weakupData = Undefined();
            return ret;
        }
        
        void weakupThread() {
            amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
            m_isPausedThread = false;
            
            if (!m_pLooperExecutor) {
                return;
            }
            
            // 通知其他线程
            m_condition_any.notify_all();
        }
        
        bool isResumeThread() {
            amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
            return !m_isPausedThread;
        }
        
    protected:
        bool onExecption(int64_t thread_id, const std::string& ansiStr) {
            TransferEventInfo info;
            info.name = "execption";
            info.data = amo::string(ansiStr, false).to_utf8();
            triggerEvent(info);
            return true;
        }
        
    protected:
    
        /** @brief	所属浏览器id. */
        int m_nBrowserID;
        /** @brief	所属Frame ID. */
        int64_t m_nFrameID;
        /** @brief	锁. */
        std::recursive_mutex  m_mutex;
        /** @brief	条件变量. */
        std::condition_variable_any m_condition_any;
        /** @brief	当前线程是否处于暂停状态. */
        bool m_isPausedThread;
        /** @brief	线程. */
        std::shared_ptr<amo::looper_executor> m_pLooperExecutor;
        /** @brief	唤醒线程时可以传递的数据. */
        Any m_weakupData;
    };
    
    
    
    
    
    
}

#endif AMO_THREADTRANSFER_HPP__