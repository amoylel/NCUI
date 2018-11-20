// Created by amoylel on 07/12/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODEMESSAGEHANDLER_H__
#define AMO_NODEMESSAGEHANDLER_H__




#include <functional>
#include <mutex>
#include <string>

#include <boost/interprocess/ipc/message_queue.hpp>

#include <amo/json.hpp>
#include "ipc/Any.hpp"

#include "transfer/FunctionWrapper.hpp"





using namespace boost::interprocess;

// for nodejs
namespace amo {


    class UIMessageEmitter;
    class BrowserWindowTransfer;
    class Transfer;
    class AppSettings;
    
    class NodeMessageHandler  {
    
    public:
        NodeMessageHandler();
        ~NodeMessageHandler();
        
        // 允许向NodeJS发送消息
        void enableNodeJS();
        
        bool processSyncMessage(int nID, IPCMessage::SmartType msg);
        
        // 设置C++调用JS函数
        void nativeMessageAdapter(std::function<bool(IPCMessage::SmartType)> cb);
        
        // JS调用C++
        void onNodeMessageRecv(IPCMessage::SmartType msg);
        
        bool sendMessageToUI(IPCMessage::SmartType msg);
        
        bool sendMessageToNode(IPCMessage::SmartType msg);
        
        bool onBeforeResultCallback(const std::string& message_name,
                                    IPCMessage::SmartType msg,
                                    amo::IPCResult& ret);
                                    
        bool getFunctionWrappers(std::unordered_map<std::string,
                                 amo::FunctionWrapperMgr>& mp);
                                 
        std::shared_ptr<UIMessageEmitter>& getNodeRunner()  {
            return m_pNodeRunner;
        }
        void setNodeRunner(std::shared_ptr<UIMessageEmitter> val) {
            m_pNodeRunner = val;
        }
        
        void setMessageQueue(const std::string& strName);
        
        bool tryReadMessage();
        
        void stopReadMessage();
        void startReadMessage();
        
        void stopNodeProcess();
        
        
        bool needQuit();
        
        std::function<bool()> getAfterCreatePipe() const;
        
        void setAfterCreatePipe(std::function<bool()> val);
        
        AMO_CEF_IMPL_NO_REFCOUNTING(NodeMessageHandler)
        
    private:
        void closeMessageQueue();
    private:
        std::function<bool(IPCMessage::SmartType)>m_fnSendMessageToNode;
        
        Any * m_pAnyResult;
        std::recursive_mutex  m_mutex;
        std::condition_variable_any m_is_finished;
        bool fininshed;
        
        std::shared_ptr<BrowserWindowTransfer> m_pBrowserTransfer;
        std::set<Transfer*> m_oMessageTransferSet;
        
        int m_nBrowserID;
        
        std::shared_ptr<UIMessageEmitter> m_pNodeRunner;
        
        // 消息读取定时器
        UINT m_nPickMessageTimer;
        
        std::shared_ptr<message_queue>  m_pMessageQueueReader;
        std::shared_ptr<message_queue> m_pMessageQueueWriter;
        std::string m_strMessageQueueName;
        
        std::function<bool()> m_fnAfterCreatePipe;
        
        
        
    };
    
}


#endif // AMO_NODEMESSAGEHANDLER_H__