#include "stdafx.h"
#include "node/NodeMessageHandler.h"

#include <mutex>
#include <functional>
#include <condition_variable>

#include "handler/CefHeader.hpp"
#include "transfer/TransferMgr.h"
#include "context/AppContext.h"
#include "ui/win/tray/Tray.h"
#include "ipc/UIMessageEmitter.hpp"
#include "transfer/BrowserWindowTransfer.h"
#include "ui/win/tray/NotifyWindow.h"

namespace amo {

    NodeMessageHandler::NodeMessageHandler() {
        m_nBrowserID = -9999;
        fininshed = false;
        m_nPickMessageTimer = 0;
        m_pBrowserTransfer = ClassTransfer::getUniqueTransfer<BrowserWindowTransfer>();
        
        m_pBrowserTransfer->setBeforeResultCallback(
            std::bind(&NodeMessageHandler::onBeforeResultCallback, this,
                      std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
                      
        BrowserTransferMgr::getInstance()->registerClass(m_nBrowserID);
        
        
        getNodeRunner().reset(new UIMessageEmitter());
    }
    
    NodeMessageHandler::~NodeMessageHandler() {
        // MessageBoxA(NULL, "~NodeClientHandler", "setMessageQueue", MB_OK);
        closeMessageQueue();
    }
    
    
    void NodeMessageHandler::enableNodeJS() {
        UIMessageEmitter::getNodeMessageSender()
            = std::bind(&NodeMessageHandler::sendMessageToNode,
                        this,
                        std::placeholders::_1);
    }
    
    bool NodeMessageHandler::processSyncMessage(int nID,
            IPCMessage::SmartType msg) {
        if (nID != m_nBrowserID) {
            return false;
        }
        
        //CefRefPtr<CefProcessMessage> message = amo::createCefProcessMessage(anyMessage);
        onNodeMessageRecv(msg);
        return true;
    }
    
    void NodeMessageHandler::nativeMessageAdapter(
        std::function<bool(IPCMessage::SmartType)> cb) {
        m_fnSendMessageToNode = cb;
        UIMessageEmitter::getNodeMessageSender() = cb;
        
        getNodeRunner().reset(new UIMessageEmitter());
    }
    
    void NodeMessageHandler::onNodeMessageRecv(IPCMessage::SmartType msg) {
    
        //$clog(amo::cdevel << __FUNCTION__ << msg->toJson().to_string() << amo::endl;);
        //CefRefPtr<CefProcessMessage> message = amo::createCefProcessMessage(anyMessage);
        const std::string& message_name = msg->getMessageName();
        
        if (message_name == MSG_CREATE_PIPE_CLIENT) {
            std::shared_ptr<amo::pipe<amo::pipe_type::server> >
            m_pBrowserPipeServer;			//消息管道主进程服务端
            
            std::shared_ptr<amo::pipe<amo::pipe_type::client> >
            m_pRenderPipeClient;			//消息管道主进程客户端
            std::shared_ptr<ProcessExchanger> m_pBrowserProcessExchanger(
                new ProcessExchanger());			//消息管道数据交换类
            std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
            int id = -9999;
            std::string strPipeClientName = RendererPipePrefix + (std::string)
                                            args->getString(0);
            $clog(amo::cdevel << func_orient << "connect pipe:" << strPipeClientName <<
                  amo::endl;);
            std::string strPipeServerName = BrowserPipePrefix + (std::string)
                                            args->getString(0);
            m_pRenderPipeClient.reset(new amo::pipe<amo::pipe_type::client>
                                      (strPipeClientName));
            m_pBrowserPipeServer.reset(new amo::pipe<amo::pipe_type::server>
                                       (strPipeServerName, DefaultPipeSize));
            bool bOK = m_pRenderPipeClient->connect();
            
            $clog(amo::cdevel << func_orient << "connect pipe" << (bOK ? "success" :
                    "fail") << amo::endl;);
                    
            bOK = m_pBrowserPipeServer->connect();
            $clog(amo::cdevel << func_orient << "connect main process pipe service" <<
                  (bOK ? "success" : "fail") << amo::endl;);
            m_pBrowserProcessExchanger->setPipeClient(m_pRenderPipeClient);
            m_pBrowserProcessExchanger->setPipeServer(m_pBrowserPipeServer);
            m_pBrowserProcessExchanger->setBrowserID(id);
            m_pBrowserProcessExchanger->setProcessSyncMessageCallback(std::bind(
                        &NodeMessageHandler::processSyncMessage, this, std::placeholders::_1,
                        std::placeholders::_2));
            BrowserProcessExchangerManager::getInstance()->addExchanger(m_nBrowserID,
                    m_pBrowserProcessExchanger);
                    
                    
                    
            if (needQuit()) {
                BrowserProcessExchangerManager::getInstance()->exchange(m_nBrowserID, false);
            } else {
                auto manager = amo::BrowserTransferMgr::getInstance();
                amo::u8json arr = manager->getTransferMap(m_nBrowserID).toJson();
                BrowserProcessExchangerManager::getInstance()->exchange(m_nBrowserID, arr);
            }
            
        } else if (message_name == MSG_PROCESS_SYNC_EXECUTE) {
            BrowserProcessExchangerManager::getInstance()->tryProcessMessage(m_nBrowserID);
        }
        
        std::string strMessageName = msg->getName();
        
        if (strMessageName == MSG_NATIVE_EXECUTE
                || strMessageName == MSG_NATIVE_SYNC_EXECUTE
                || strMessageName ==
                MSG_NATIVE_ASYNC_EXECUTE)	{									//JS调用C++，此消息不向页面返回结果
            CefString handlerName = msg->getArgumentList()->getString(
                                        IPCArgsPosInfo::CustomArgs);
                                        
            if (!handlerName.empty()) {
                BrowserTransferMgr::getInstance()->onMessageTransfer(msg);
                /*for (auto & p : m_oMessageTransferSet) {
                	if (p->transferName() == handlerName.ToString() && p->onMessageTransfer(anyMessage).isValid()){
                	return;
                	}
                	}*/
            }
        }
        
    }
    
    
    
    bool NodeMessageHandler::sendMessageToUI(IPCMessage::SmartType msg) {
#if CHROME_VERSION_BUILD >= 2704
        CefPostTask(TID_UI, base::Bind(&NodeMessageHandler::onNodeMessageRecv, this,
                                       msg));
#else
        CefPostTask(TID_UI, NewCefRunnableMethod(this,
                    &NodeMessageHandler::onNodeMessageRecv, msg));
                                       
#endif
                                       
        return true;
    }
    
    
    bool NodeMessageHandler::sendMessageToNode(IPCMessage::SmartType msg) {
        /*if (!m_fnSendMessageToNode) return false;
        return m_fnSendMessageToNode(msg);*/
        //$clog(amo::cdevel << func_orient << msg->toJson().to_string() << amo::endl;);
        
        try {
            if (msg->getArgumentList()->getString(IPCArgsPosInfo::FuncName) == "quit") {
                //MessageBoxA(NULL, msg->GetName().c_str(), "setMessageQueue123", MB_OK);
            }
            
            if (!m_pMessageQueueWriter) {
                return false;
            }
            
            std::string str = msg->toJson().to_string();
            char cc[10000] = { 0 };
            strcpy(cc, str.c_str());
            m_pMessageQueueWriter->send(cc, 10000, 0);
        } catch (std::exception& e) {
            std::string str = e.what();
            MessageBoxA(NULL, str.c_str(), "setMessageQueue123", MB_OK);
        }
        
        return true;
    }
    
    bool NodeMessageHandler::onBeforeResultCallback(const std::string& message_name,
            IPCMessage::SmartType msg, amo::IPCResult& ret) {
        if (msg->getArgumentList()->getInt(IPCArgsPosInfo::BrowserID) != m_nBrowserID) {
            return false;
        }
        
        if (message_name == MSG_NATIVE_SYNC_EXECUTE) {
            BrowserProcessExchangerManager::getInstance()->exchange(m_nBrowserID, ret);
            return true;
        } else if (message_name == MSG_NATIVE_ASYNC_EXECUTE) {
            int nCallbackID = msg->getArgumentList()->getInt(IPCArgsPosInfo::AsyncCallback);
            IPCMessage::SmartType ipcMessage(new amo::IPCMessage());
            ipcMessage->setMessageName("ASYNC_EXECUTE_CALLBACK");
            ipcMessage->getArgumentList()->setValue(0, ret);
            sendMessageToNode(ipcMessage);
            return true;
        }
        
        return false;
        
    }
    
    /*bool NodeClientHandler::AddMessageTransfer(Transfer* transfer) {
    	if (transfer == NULL) return false;
    	return m_oMessageTransferSet.insert(transfer).second;
    	}*/
    
    bool NodeMessageHandler::getFunctionWrappers(
        std::unordered_map<std::string, amo::FunctionWrapperMgr>& mp) {
        auto map = BrowserTransferMgr::getInstance()->getTransferMap(
                       m_nBrowserID).transferMap();
                       
        for (auto& p : map) {
            mp[p.second->transferName()] = p.second->getFuncMgr();
        }
        
        return true;
    }
    
    
    
    
    
    void NodeMessageHandler::setMessageQueue(const std::string& strName) {
        m_strMessageQueueName = strName;
        //MessageBoxA(NULL, m_strMessageQueueName.c_str(), "setMessageQueue", MB_OK);
    }
    
    bool NodeMessageHandler::tryReadMessage() {
    
        boost::posix_time::ptime pt =
            boost::posix_time::microsec_clock::universal_time()
            + boost::posix_time::milliseconds(5);
        char str[10000] = { 0 };
        unsigned int priority = 0;
        message_queue::size_type recvd_size = 0;
        
        while (m_pMessageQueueReader->timed_receive(&str, 10000, recvd_size, priority,
                pt)) {
            if (recvd_size == 0) {
                return false;
            }
            
            amo::u8json json(str);
            IPCMessage::SmartType ipcMessage(new IPCMessage());
            *ipcMessage = IPCMessage::fromJson(json);
            
            onNodeMessageRecv(ipcMessage);
            //MessageBoxA(NULL, anyMessage->GetName().c_str(), "Error", MB_OK);
        }
        
        return true;
        
    }
    
    void NodeMessageHandler::stopReadMessage() {
    
        NotifyWindow*  pNotifyWindow = Tray::getInstance()->getNotifyWindow();
        
        if (pNotifyWindow == NULL) {
            return;
        }
        
        ::KillTimer(pNotifyWindow->GetHWND(), m_nPickMessageTimer);
    }
    
    VOID CALLBACK MessageQueueProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent,
                                   DWORD dwTime) {
        AppContext::getInstance()->getNodeMessageHandler()->tryReadMessage();
        
    }
    
    void NodeMessageHandler::startReadMessage() {
    
        //MessageBox(NULL, L"Unable to parse command line", L"Error", MB_OK);
        if (m_strMessageQueueName.empty()) {
            return;
        }
        
        std::string strReaderName = m_strMessageQueueName + "2";
        std::string strWriterName = m_strMessageQueueName + "1";
        
        try {
            /*m_pMessageQueueReader.reset(new message_queue(open_or_create, strReaderName.c_str(), 1000, 10000));
            m_pMessageQueueWriter.reset(new message_queue(open_or_create, strWriterName.c_str(), 1000, 10000));*/
            
            m_pMessageQueueReader.reset(new message_queue(open_only,
                                        strReaderName.c_str()));
            m_pMessageQueueWriter.reset(new message_queue(open_only,
                                        strWriterName.c_str()));
                                        
            NotifyWindow*  pNotifyWindow = Tray::getInstance()->getNotifyWindow();
            
            if (pNotifyWindow == NULL) {
                return;
            }
            
            if (m_nPickMessageTimer == 0) {
                m_nPickMessageTimer = ::SetTimer(pNotifyWindow->GetHWND(), 0x00000501, 100,
                                                 (TIMERPROC)MessageQueueProc);
                                                 
            }
        } catch (std::exception& e) {
            $clog(amo::cerr << e.what() << amo::endl;);
            closeMessageQueue();
            MessageBox(NULL, L"消息打开失败", L"Error", MB_OK);
            return;
        }
        
    }
    
    void NodeMessageHandler::stopNodeProcess() {
    
        /*ProcessMessage::SmartType anyMessage(new ProcessMessage());
        anyMessage->setMessageName("quit");
        sendMessageToNode(anyMessage);*/
        
        //getNodeRunner()->Execute("quit");
        //MessageBoxA(NULL, "stopNodeProcess", "setMessageQueue", MB_OK);
        
        stopReadMessage();
        
        if (getNodeRunner()) {
            //MessageBoxA(NULL, "stopNodeProcess", "setMessageQueue", MB_OK);
            getNodeRunner()->execute("quit");
        }
        
        closeMessageQueue();
    }
    
    bool NodeMessageHandler::needQuit() {
        if (m_fnAfterCreatePipe) {
            return m_fnAfterCreatePipe();
        }
        
        return false;
    }
    
    std::function<bool()> NodeMessageHandler::getAfterCreatePipe() const {
        return m_fnAfterCreatePipe;
    }
    
    void NodeMessageHandler::setAfterCreatePipe(std::function<bool()> val) {
        m_fnAfterCreatePipe = val;
    }
    
    void NodeMessageHandler::closeMessageQueue() {
        if (m_strMessageQueueName.empty()) {
            return;
        }
        
        std::string strReaderName = m_strMessageQueueName + "2";
        std::string strWriterName = m_strMessageQueueName + "1";
        
        message_queue::remove(strReaderName.c_str());
        message_queue::remove(strWriterName.c_str());
        
        m_pMessageQueueWriter.reset();
        m_pMessageQueueReader.reset();
        m_strMessageQueueName.clear();
        
    }
    
}

