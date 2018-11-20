// Created by amoylel on 06/30/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_UIMESSAGEBROADCASTER_HPP__
#define AMO_UIMESSAGEBROADCASTER_HPP__

#include "UIMessageEmitter.hpp"
#include "handler/BrowserManager.hpp"
#include <utility>
#include <algorithm>
#include "ipc/Any.hpp"
#include "transfer/EventTransfer.h"
#include "transfer/ClassTransfer.hpp"

namespace amo {

    /*!
     * @class	UIMessageBroadcaster
     *
     * @brief	A message broadcaster.
     */
    
    class UIMessageBroadcaster : public UIMessageEmitter {
    public:
        UIMessageBroadcaster(const int64_t nObjectID)
            : m_nObjectID(nObjectID) {
            
            
        }
        
        static void triggerEventToBorwser(IPCMessage::SmartType msg) {
        
        
            auto args = msg->getArgumentList();
            
            //TODO: 加入NodeJS支持
            
            std::string strEventName = args->getString(IPCArgsPosInfo::FuncName);
            
            if (strEventName == "triggerEvent") {
                // FrameID 和BrowserID 都必需存在
                std::shared_ptr<UIMessageEmitter> runner(new UIMessageEmitter());
                int64_t nFrameID = args->getInt64(IPCArgsPosInfo::FrameID);
                runner->setIPCMessage(msg->clone());
                runner->setFrame(BrowserManager<PID_BROWSER>::GetFrameByID(nFrameID));
                runner->execute();
            } else if (strEventName == "emitEventAllFrame"
                       || strEventName == "releaseAllTransfer") {
                // 不管FrameID,所有的都去一份
                std::unordered_map<int, CefRefPtr<CefBrowser> > mp
                    = BrowserManager<PID_BROWSER>::GetAllBrowser();
                    
                for (auto& p : mp) {
                    std::shared_ptr<UIMessageEmitter> runner(new UIMessageEmitter());
                    runner->setIPCMessage(msg->clone());
                    runner->setFrame(p.second->GetMainFrame());
                    
                    runner->execute();
                }
            }
            
        }
        
        /*!
         * @fn	template<typename ... Args> Any UIMessageBroadcaster::broadcast(Args... args)
         *
         * @brief	Broadcasts the given arguments.
         *
         * @tparam	Args	Type of the arguments.
         * @param	args	Variable arguments providing the arguments.
         *
         * @return	Any.
         */
        
        template<typename ... Args>
        Any broadcast(Args... args) {
        
            //return syncBroadcast(args ...);
            
            std::shared_ptr<UIMessageEmitter> runner(new UIMessageEmitter());
            runner->createIPCMessage("emitEventAllFrame",
                                     MSG_NATIVE_EXECUTE,
                                     args...);
            runner->setValue(IPCArgsPosInfo::TransferName, "ipcRenderer");
            runner->setValue(IPCArgsPosInfo::EventObjectID, m_nObjectID);
            runner->setValue(IPCArgsPosInfo::TransferID, 0);
            runner->setValue(IPCArgsPosInfo::ArgsLength, 2);
            runner->setValue(IPCArgsPosInfo::BrowserID, -9999);
            runner->setValue(IPCArgsPosInfo::FrameID, -1);
            
            typedef std::function<bool(IPCMessage::SmartType)> NodeSender;
            NodeSender& fn = getNodeMessageSender();
            
            
            if (fn) {
                runner->execute();
            }
            
            std::unordered_map<int, CefRefPtr<CefBrowser> > mp
                = BrowserManager<PID_BROWSER>::GetAllBrowser();
                
            int nIndex = 0;
            
            
            // 向所有浏览器广播消息
            for (auto& p : mp) {
                ++nIndex;
                runner->setFrame(p.second->GetMainFrame());
                runner->execute();
            }
            
            return Undefined();
        }
        
        
        template<typename ... Args>
        Any syncBroadcast(Args... args) {
            //return broadcast(args ...);
            
            std::shared_ptr<UIMessageEmitter> runner(new UIMessageEmitter());
            runner->createIPCMessage("emitEventAllFrame",
                                     MSG_NATIVE_SYNC_EXECUTE,
                                     args...);
            runner->setValue(IPCArgsPosInfo::TransferName, "ipcRenderer");
            runner->setValue(IPCArgsPosInfo::EventObjectID, m_nObjectID);
            runner->setValue(IPCArgsPosInfo::TransferID, 0);
            runner->setValue(IPCArgsPosInfo::ArgsLength, 2);
            runner->setValue(IPCArgsPosInfo::BrowserID, -9999);
            runner->setValue(IPCArgsPosInfo::FrameID, -1);
            
            typedef std::function<bool(IPCMessage::SmartType)> NodeSender;
            NodeSender& fn = getNodeMessageSender();
            
            Any ret = Undefined();
            
            if (fn) {
                ret = runner->syncExecute();
                
                /* if (ret.is<bool>() && !ret.As<bool>()) {
                	 return Undefined();
                 }*/
                
            }
            
            std::unordered_map<int, CefRefPtr<CefBrowser> > mp
                = BrowserManager<PID_BROWSER>::GetAllBrowser();
                
            int nIndex = 0;
            
            
            // 向所有浏览器广播消息
            for (auto& p : mp) {
                ++nIndex;
                runner->setFrame(p.second->GetMainFrame());
                ret = runner->syncExecute();
                
                // 如果返回值是一个BOOL类型且为False，那么停止循环
                /*if (ret.is<bool>() && !ret.As<bool>()) {
                    break;
                }*/
            }
            
            return ret;
        }
    protected:
        int64_t m_nObjectID;
    };
}

#endif // AMO_UIMESSAGEBROADCASTER_HPP__
