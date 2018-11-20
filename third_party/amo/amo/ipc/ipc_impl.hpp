// Created by amoylel on 14/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_IPC_IMPL_7497D7AA_5B23_46D5_B122_044A05144A5F_HPP__
#define AMO_IPC_IMPL_7497D7AA_5B23_46D5_B122_044A05144A5F_HPP__

#include <functional>
#include <mutex>
#include <string>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <chrono>
#include <memory>
#include <fstream>

#include <boost/interprocess/ipc/message_queue.hpp>
#include <amo/json.hpp>
#include <amo/ipc/function_wrapper.hpp>
#include <amo/timer.hpp>
#include <amo/ipc/process_exchanger.hpp>

#include <amo/ipc/transfer_manager.hpp>
#include <amo/ipc/ipc_emitter.hpp>
#include <amo/md5.hpp>

namespace {
    const static std::string IPC_HEARTBEAT = "f6a37d9c-09e7-4111-bc89-6e4d27dc645b";
    const static std::string IPC_DISCONNECT =
        "8a11d60a-b4a9-4a96-bc78-84515e3ac9e2";
    const static std::string IPC_MSG_END = "f316f167-225a-47fb-8c40-a638ee07e7c5";
}



namespace amo {
    template<ipc_type>
    class ipc_exchanger_manager;
    
    template<> class ipc_exchanger_manager<ipc_server>
        : public  process_exchanger_manager<process_exchanger>
        , public amo::singleton<ipc_exchanger_manager<ipc_server> > {
        
    };
    template<> class ipc_exchanger_manager<ipc_client>
        : public  process_exchanger_manager<process_exchanger>
        , public amo::singleton<ipc_exchanger_manager<ipc_client> > {
        
    };
    
    
    
    template<ipc_type> class ipc_transfer_mgr;
    template<> class ipc_transfer_mgr<ipc_server>
        : public transfer_manager
        , public amo::singleton<ipc_transfer_mgr<ipc_server> > {
    public:
        virtual void returnSyncResult(int nBrowserID, amo::ipc_result& ret) {
        
            auto mgr = ipc_exchanger_manager<ipc_server>::getInstance();
            mgr->exchange(nBrowserID, ret);
            return;
        }
        
        virtual void returnAsyncResult(int nBrowserID, amo::any& ret, int id,
                                       int frame_id) {
            std::shared_ptr < ipc_emitter > runner(new ipc_emitter(
                    nBrowserID,
                    frame_id));
            runner->setValue(ipc_args_pos_info::TransferName, "ipcMain");
            runner->setValue(ipc_args_pos_info::AsyncCallback, id);
            runner->execute("asyncExecuteResult", ret);
            return;
        }
    };
    template<> class ipc_transfer_mgr<ipc_client>
        : public transfer_manager
        , public  amo::singleton<ipc_transfer_mgr<ipc_client> > {
    public:
        virtual void returnSyncResult(int nBrowserID, amo::ipc_result& ret) {
        
            auto mgr = ipc_exchanger_manager<ipc_client>::getInstance();
            mgr->exchange(nBrowserID, ret);
            return;
        }
        
        virtual void returnAsyncResult(int nBrowserID, amo::any& ret, int id,
                                       int frame_id) {
            std::shared_ptr<ipc_emitter> runner(new ipc_emitter(
                                                    nBrowserID,
                                                    frame_id));
            runner->setValue(ipc_args_pos_info::TransferName, "ipcMain");
            runner->setValue(ipc_args_pos_info::AsyncCallback, id);
            runner->execute("asyncExecuteResult", ret);
            return;
        }
    };
    
}


using namespace boost::interprocess;


namespace amo {


    class ipc_emitter;
    
    class ipc_base  {
    public:
        typedef std::shared_ptr<process_exchanger_manager  < process_exchanger > >
        SmartProcessExchangeMgr;
        typedef  std::shared_ptr<std::function<any(ipc_msg::SmartType)> >
        smart_event_fn_type;
        
    public:
    
        static bool createMessageQueue(const std::string& strMessageQueue,
                                       int nTimeOut = 3000) {
            amo::timer t;
            $cdevel("create message queue:{0}", strMessageQueue);
            
            while (t.elapsed() < nTimeOut) {
            
                if (strMessageQueue.empty()) {
                    return false;
                }
                
                std::shared_ptr<message_queue> pMessageQueueReader;
                std::shared_ptr<message_queue> pMessageQueueWriter;
                
                std::string strReaderName = strMessageQueue + "1";
                std::string strWriterName = strMessageQueue + "2";
                
                try {
                    // 先看能不能打开，如果能，直接返回true
                    pMessageQueueReader.reset(new message_queue(open_only,
                                              strReaderName.c_str()));
                    pMessageQueueWriter.reset(new message_queue(open_only,
                                              strWriterName.c_str()));
                    $cdevel("open message queue success:{0}", strMessageQueue);
                    return true;
                } catch (const std::exception& e) {
                    $cdevel("open message queue fail:{}", e.what());
                }
                
                try {
                    pMessageQueueReader.reset(new message_queue(create_only,
                                              strReaderName.c_str(),
                                              1000,
                                              1000));
                    pMessageQueueWriter.reset(new message_queue(create_only,
                                              strWriterName.c_str(),
                                              1000,
                                              1000));
                                              
                    auto a1 = pMessageQueueWriter->get_max_msg();
                    auto a2 = pMessageQueueWriter->get_max_msg_size();
                    auto a3 = pMessageQueueWriter->get_num_msg();
                    $cdevel("create message queue success:{0}", strMessageQueue);
                    return true;
                } catch (const std::exception& e) {
                    $cerr("create message queue fail:{0}",  e.what());
                    
                }
                
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            
            
            return false;
        }
        
        static void closeMessageQueue(const std::string& strMessageQueue) {
            if (strMessageQueue.empty()) {
                return;
            }
            
            std::string strReaderName = strMessageQueue + "2";
            std::string strWriterName = strMessageQueue + "1";
            
            message_queue::remove(strReaderName.c_str());
            message_queue::remove(strWriterName.c_str());
            
            
        }
        
        
        
    public:
    
        ipc_base(const std::string& strName, int ipcID = -9999) {
            setIPCID(ipcID);
            fininshed = false;
            m_nMessageTimerID = 0;
            m_nHeartbeatTimerID = 0;
            m_bClient = false;
            m_bPipeCreated = false;
            m_strMessageQueue = strName;
            m_strMessageQueue = amo::md5::encode(strName);
            m_nLastMsgTimestamp = amo::timer::now();
            m_bPipeDisconnected = false;
        }
        
        ~ipc_base() {
            closeMessageQueue();
        }
        
        int64_t getLastMsgTimestamp() const {
            return m_nLastMsgTimestamp;
        }
        void setLastMsgTimestamp(int64_t val) {
            m_nLastMsgTimestamp = val;
        }
        
        void disconnect() {
            sendMessage(IPC_DISCONNECT);
            stopReadMessage();
            closeMessageQueue();
        }
        
        int getIPCID() const {
            return m_nIPCID;
        }
        
        void setIPCID(int val) {
            m_nIPCID = val;
        }
        
        bool processSyncMessage(int nID, ipc_msg::SmartType msg) {
            if (nID != getIPCID()) {
                return false;
            }
            
            onProcessMessageRecv(msg);
            return true;
        }
        
        void onProcessMessageRecv(ipc_msg::SmartType msg) {
        
            //$clog(amo::cdevel << __FUNCTION__ << msg->toJson().to_string() << amo::endl;);
            //CefRefPtr<CefProcessMessage> message = amo::createCefProcessMessage(anyMessage);
            const std::string& message_name = msg->getMessageName();
            
            if (message_name == IPC_CREATE_PIPE_CLIENT) {
                //消息管道主进程服务端
                std::shared_ptr<amo::pipe<amo::pipe_type::server> > pPipeServer;
                //消息管道主进程客户端
                std::shared_ptr<amo::pipe<amo::pipe_type::client> > pPipeClient;
                std::shared_ptr<process_exchanger> pProcessExchanger(
                    new process_exchanger());			//消息管道数据交换类
                std::shared_ptr<any_args_list> args = msg->getArgumentList();
                
                std::string strPipeClientName = IPCRendererPipePrefix + args->getString(0);
                $cdevel("connect pipe:{}", strPipeClientName);
                std::string strPipeServerName = IPCBrowserPipePrefix +  args->getString(0);
                pPipeClient.reset(new amo::pipe<amo::pipe_type::client>
                                  (strPipeClientName));
                pPipeServer.reset(new amo::pipe<amo::pipe_type::server>
                                  (strPipeServerName, IPCDefaultPipeSize));
                bool bOK = pPipeClient->connect();
                $cdevel("connect pipe {}", (bOK ? "success" : "fail"));
                
                
                bOK = pPipeServer->connect();
                $cdevel("main process pipe server connect{}", (bOK ? "success" : "fail"));
                
                pProcessExchanger->setPipeClient(pPipeClient);
                pProcessExchanger->setPipeServer(pPipeServer);
                pProcessExchanger->setBrowserID(getIPCID());
                pProcessExchanger->setProcessSyncMessageCallback(std::bind(
                            &ipc_base::processSyncMessage, this, std::placeholders::_1,
                            std::placeholders::_2));
                m_pProcessExchangerManager->addExchanger(getIPCID(),
                        pProcessExchanger);
                        
                        
                        
                if (bOK) {
                    // 通知其他线程
                    m_bPipeCreated = true;
                    m_is_finished.notify_all();
                }
                
            } else if (message_name == IPC_PROCESS_SYNC_EXECUTE) {
                m_pProcessExchangerManager->tryProcessMessage(getIPCID());
            }
            
            if (message_name == IPC_NATIVE_EXECUTE
                    || message_name == IPC_NATIVE_SYNC_EXECUTE
                    || message_name == IPC_NATIVE_ASYNC_EXECUTE) {
                m_transferMgr->onMessageTransfer(msg);
                
            }
        }
        
        bool sendProcessMessage(ipc_msg::SmartType msg) {
        
            try {
                if (!m_pMessageQueueWriter) {
                    return false;
                }
                
                std::string str = msg->toJson().to_string();
                return sendMessage(str);
            } catch (std::exception& e) {
                std::string str = e.what();
            }
            
            return true;
        }
        
        bool sendMessage(ipc_msg::SmartType msg) {
            return sendProcessMessage(msg);
        }
        
        bool sendMessage(const std::string& msg) {
        
            try {
                if (!m_pMessageQueueWriter) {
                
                    return false;
                }
                
                size_t index = 0;
                size_t msg_size = m_pMessageQueueWriter->get_max_msg_size();
                
                
                int nRetryCount = 0;
                
                while (index < msg.size()) {
                    boost::posix_time::ptime pt =
                        boost::posix_time::microsec_clock::universal_time()
                        + boost::posix_time::milliseconds(200);
                        
                    if (nRetryCount > 10) {
                        $cerr("send message error");
                        return false;
                    }
                    
                    if (index + msg_size <= msg.size()) {
                        bool bOk =  m_pMessageQueueWriter->timed_send(msg.data() + index, msg_size, 0,
                                    pt);
                                    
                        if (!bOk) {
                            ++nRetryCount;
                        } else {
                            index += msg_size;
                        }
                        
                    } else {
                        bool bOk =  m_pMessageQueueWriter->timed_send(msg.data() + index,
                                    msg.size() - index,
                                    0,
                                    pt);
                                    
                        if (!bOk) {
                            ++nRetryCount;
                        } else {
                            index = msg.size();
                        }
                        
                        
                        
                    }
                    
                }
                
                // 发送结束符
                if (msg != IPC_HEARTBEAT) {
                    boost::posix_time::ptime pt =
                        boost::posix_time::microsec_clock::universal_time()
                        + boost::posix_time::milliseconds(200);
                        
                    return   m_pMessageQueueWriter->timed_send(IPC_MSG_END.data(),
                             IPC_MSG_END.size(), 0,
                             pt);
                }
            } catch (std::exception& e) {
                std::string str = e.what();
                $cerr("send message error:{}", str);
            }
            
            return true;
        }
        
        std::shared_ptr<ipc_emitter> getIPCEmitter()  {
            std::shared_ptr<ipc_emitter> pRunner;
            pRunner.reset(new ipc_emitter());
            pRunner->setProcessExchangeManager(m_pProcessExchangerManager);
            
            pRunner->setProcessMessageSender(std::bind(
                                                 &ipc_base::sendProcessMessage,
                                                 this,
                                                 std::placeholders::_1));
                                                 
            pRunner->setValue(ipc_args_pos_info::TransferName, "ipc");
            pRunner->setValue(ipc_args_pos_info::EventObjectID, 0);
            pRunner->setValue(ipc_args_pos_info::TransferID, 0);
            return pRunner;
        }
        
        
        bool tryReadMessage() {
        
            try {
            
                if (m_heartbeat_timer.elapsed() > 50000 && !m_bPipeCreated) {
                    return false;
                }
                
                if (!m_pMessageQueueReader) {
                    return false;
                }
                
                sendMessage(IPC_HEARTBEAT);
                
                // 如果超过10秒没有收到心跳包，那么认为连接中断，返回中断事件
                if (m_heartbeat_timer.elapsed() > 20000) {
                    transfer_event_info info;
                    info.name = "ipc.disconnect";
                    info.data = amo::string("IPC disconnect", false).to_utf8();
                    info.toAll = true;
                    ipc_msg::SmartType msg(new ipc_msg());
                    msg->getArgumentList()->setValue(0, info.name);
                    msg->getArgumentList()->setValue(1, info.toJson());
                    msg->getArgumentList()->setValue(ipc_args_pos_info::FuncName, info.name);
                    
                    if (m_fnDefaultMsgFunc && *m_fnDefaultMsgFunc) {
                        (*m_fnDefaultMsgFunc)(msg);
                    }
                    
                    m_bPipeCreated = false;
                    m_bPipeDisconnected = true;
                    return false;
                }
                
                boost::posix_time::ptime pt =
                    boost::posix_time::microsec_clock::universal_time()
                    + boost::posix_time::milliseconds(10);
                    
                std::vector<char> vec(m_pMessageQueueReader->get_max_msg_size(), 0);
                unsigned int priority = 0;
                message_queue::size_type recvd_size = 0;
                
                /*std::shared_ptr<std::vector<char> > msg(new std::vector<char>(12453254 * 2 + 1,
                                                        0));*/
                
                std::stringstream msg;
                
                int nTotalCount = 0;
                
                std::vector<std::shared_ptr<std::vector<char> > > msgs;
                
                while (m_pMessageQueueReader && m_pMessageQueueReader->timed_receive(vec.data(),
                        vec.size(),
                        recvd_size,
                        priority,
                        pt))  {
                        
                    if (recvd_size == 0) {
                        return false;
                    }
                    
                    m_nLastMsgTimestamp = amo::timer::now();
                    
                    if (recvd_size == IPC_HEARTBEAT.size()
                            && IPC_HEARTBEAT == vec.data()) {
                        m_heartbeat_timer.restart();
                        memset(vec.data(), 0, vec.size());
                        continue;
                    }
                    
                    if (recvd_size == IPC_DISCONNECT.size()
                            && IPC_DISCONNECT == vec.data()) {
                        return false;
                    }
                    
                    bool bEndMsg = false;
                    
                    if (recvd_size >= IPC_MSG_END.size()) {
                        bEndMsg = memcmp(vec.data() + recvd_size - IPC_MSG_END.size(),
                                         IPC_MSG_END.data(), IPC_MSG_END.size()) == 0;
                                         
                    }
                    
                    if (bEndMsg) {
                        recvd_size = recvd_size - IPC_MSG_END.size();
                    }
                    
                    nTotalCount += recvd_size;
                    
                    
                    try {
                        if (recvd_size > 0) {
                            std::string ss = std::string(vec.data(), recvd_size);
                            //$cdevel(ss);
                            msg << ss;
                            /*msgs.push_back(std::shared_ptr<std::vector<char> >(new std::vector<char>
                                           (vec.begin(), vec.begin() + recvd_size)));*/
                        }
                    } catch (const std::exception& e) {
                        $cerr("length:{}", nTotalCount);
                        $cerr("read error 1:{}", e.what());
                    }
                    
                    
                    if (!bEndMsg) {
                        memset(vec.data(), 0, vec.size());
                        continue;
                    }
                    
                    
                    //msg.reset(new std::vector<char>(nTotalCount + 1, 0));
                    
                    int nIndex = 0;
                    
                    amo::json json(msg);
                    
                    ipc_msg::SmartType ipcMessage(new ipc_msg());
                    
                    try {
                        *ipcMessage = ipc_msg::fromJson(json);
                    } catch (const std::exception& e) {
                        $cerr("read error 3:{}", e.what());
                    }
                    
                    
                    
                    
                    /*if (nTotalCount >= 1000000) {
                        ipcMessage = ipc_msg::fromJson2(json);
                        json.to_file("D:\\77711.txt");
                        std::fstream fs2("D:\\77722.txt", std::ios::out);
                        fs2 << msg.str();
                        $cdevel("count:{}", msg.gcount());
                    
                        fs2.close();
                        $cerr("长度1111:{}", nTotalCount);
                    }*/
                    
                    msg.str("");
                    msg.clear();
                    msg = std::stringstream();
                    msgs.clear();
                    nTotalCount = 0;
                    onProcessMessageRecv(ipcMessage);
                    
                }
            } catch (const std::exception& e) {
                $cerr("read error :{}", e.what());
                return false;
            }
            
            
            return true;
        }
        
        void stopReadMessage() {
            m_timer.stop_timer(m_nMessageTimerID);
            
            m_heartbeat_timer.stop_timer(m_nHeartbeatTimerID);
        }
        
        bool OnMessageQueueProc(amo::timer::tid id) {
            if (id == m_nMessageTimerID) {
                return  this->tryReadMessage();
            }
            
            return true;
        }
        
        bool OnHeartbeatProc(amo::timer::tid id) {
            if (m_bPipeDisconnected) {
                return false;
                
            }
            
            if (id == m_nHeartbeatTimerID) {
                if (amo::timer::now() - m_nLastMsgTimestamp > 20000) {
                
                    transfer_event_info info;
                    info.name = "ipc.disconnect";
                    info.data = amo::string("IPC disconnect", false).to_utf8();
                    info.toAll = true;
                    ipc_msg::SmartType msg(new ipc_msg());
                    msg->getArgumentList()->setValue(0, info.name);
                    msg->getArgumentList()->setValue(1, info.toJson());
                    msg->getArgumentList()->setValue(ipc_args_pos_info::FuncName, info.name);
                    
                    if (m_fnDefaultMsgFunc && *m_fnDefaultMsgFunc) {
                        (*m_fnDefaultMsgFunc)(msg);
                    }
                    
                    m_bPipeCreated = false;
                    m_bPipeDisconnected = true;
                    return false;
                }
            }
            
            return true;
        }
        
        
        void startReadMessage() {
        
            if (m_strMessageQueue.empty()) {
                return;
            }
            
            std::string strReaderName = m_strMessageQueue + (m_bClient ? "1" : "2");
            std::string strWriterName = m_strMessageQueue + (m_bClient ? "2" : "1");
            
            try {
            
                createMessageQueue(m_strMessageQueue);
                m_pMessageQueueReader.reset(new message_queue(open_only,
                                            strReaderName.c_str()));
                m_pMessageQueueWriter.reset(new message_queue(open_only,
                                            strWriterName.c_str()));
                /*auto a1 = 	m_pMessageQueueWriter->get_max_msg();
                auto a2 = m_pMessageQueueWriter->get_max_msg_size();
                auto a3 = m_pMessageQueueWriter->get_num_msg();*/
                
                if (m_nMessageTimerID == 0) {
                    m_nMessageTimerID = m_timer.set_timer(30,
                                                          std::bind(&ipc_base::OnMessageQueueProc, this,
                                                                  std::placeholders::_1));
                                                                  
                }
                
                if (m_nHeartbeatTimerID == 0) {
                    m_nHeartbeatTimerID = m_heartbeat_timer.set_timer(5000,
                                          std::bind(&ipc_base::OnHeartbeatProc, this,
                                                    std::placeholders::_1));
                                                    
                }
                
                return;
            } catch (std::exception& e) {
                $clog(amo::cerr << e.what() << amo::endl;);
                closeMessageQueue();
                MessageBox(NULL, L"open message queue fail", L"Error", MB_OK);
                return;
            }
        }
        
        void stopNodeProcess() {
            stopReadMessage();
            getIPCEmitter()->execute("quit");
            closeMessageQueue();
        }
        
        void add_transfer(std::shared_ptr<transfer> pTransfer) {
            if (m_transferMgr) {
                m_transferMgr->addTransfer(m_nIPCID, pTransfer);
            }
        }
        
        void remove_transfer(const std::string& strName) {
            if (m_transferMgr) {
                m_transferMgr->removeTransfer(m_nIPCID, strName);
            }
        }
        
        smart_event_fn_type getDefaultMsgFunc() const {
        
            return m_fnDefaultMsgFunc;
        }
        
        void setDefaultMsgFunc(smart_event_fn_type val) {
            m_fnDefaultMsgFunc = val;
            
            if (m_transferMgr) {
                m_transferMgr->setDefaultMsgFunc(val);
            }
        }
        
    protected:
        void closeMessageQueue() {
            m_timer.stop_timer();
            
            closeMessageQueue(m_strMessageQueue);
            m_pMessageQueueWriter.reset();
            m_pMessageQueueReader.reset();
            m_strMessageQueue.clear();
            
            if (m_transferMgr) {
                m_transferMgr->removeTransfer(m_nIPCID);
            }
            
            
        }
        
        
    protected:
        std::function<bool(ipc_msg::SmartType)>m_fnSendMessageToNode;
        
        any * m_pAnyResult;
        std::recursive_mutex  m_mutex;
        std::condition_variable_any m_is_finished;
        bool fininshed;
        
        std::set<transfer*> m_oMessageTransferSet;
        
        int m_nIPCID;
        
        std::shared_ptr<ipc_emitter> m_pNodeRunner;
        
        /** @brief	消息读取定时器. */
        amo::timer::tid m_nMessageTimerID;
        
        /** @brief	心跳检测定时器. */
        amo::timer::tid m_nHeartbeatTimerID;
        
        /** @brief	消息接收器. */
        std::shared_ptr<message_queue>  m_pMessageQueueReader;
        
        /** @brief	消息发送器. */
        std::shared_ptr<message_queue> m_pMessageQueueWriter;
        
        /** @brief	消息队列名称. */
        std::string m_strMessageQueue;
        
        std::function<bool()> m_fnAfterCreatePipe;
        
        amo::timer m_timer;
        
        amo::timer m_heartbeat_timer;
        /** @brief	当前是否为Client端. */
        bool m_bClient;
        
        /** @brief	管道是否已创建. */
        bool m_bPipeCreated;
        
        // IPC连接是否断开
        bool m_bPipeDisconnected;
        
        SmartProcessExchangeMgr  m_pProcessExchangerManager;
        
        std::shared_ptr<transfer_manager> m_transferMgr;
        
        /** @brief	默认消息处理函数，如果没有找到消息处理函数将触发该函数. */
        smart_event_fn_type m_fnDefaultMsgFunc;
        
        /** @brief	最后一次收到消息的时间. */
        int64_t m_nLastMsgTimestamp;
    };
    
    
    template<ipc_type> class ipc_impl;
    template<> class ipc_impl<ipc_server>
        : public ipc_base {
    public:
        ipc_impl(const std::string& strName, int ipcID = -9999)
            : ipc_base(strName, ipcID) {
            m_pProcessExchangerManager =
                ipc_exchanger_manager<ipc_server>::getInstance();
            m_transferMgr = ipc_transfer_mgr<ipc_server>::getInstance();
            
        }
        // 等待客户端连接
        bool wait(int timeout = 10) {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            closeMessageQueue(m_strMessageQueue);
            startReadMessage();
            
            amo::chrono::time_point<amo::chrono::system_clock> pt =
                amo::chrono::system_clock::now();
            pt += std::chrono::seconds(timeout);
            m_is_finished.wait_until(lock, pt, std::bind(&ipc_impl::isPipeCreated,
                                     this));
            m_heartbeat_timer.restart();
            return m_bPipeCreated;
            
        }
        
    private:
        bool isPipeCreated() {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            return m_bPipeCreated;
        }
    };
    
    template<> class ipc_impl<ipc_client>
        : public ipc_base {
    public:
        ipc_impl(const std::string& strName, int ipcID = -9999)
            : ipc_base(strName,  ipcID) {
            m_bClient = true;
            m_pProcessExchangerManager =
                ipc_exchanger_manager<ipc_client>::getInstance();
            m_transferMgr = ipc_transfer_mgr<ipc_client>::getInstance();
        }
        
        // 连接到服务器
        
        bool connect() {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            startReadMessage();
            std::stringstream str;
            str << getIPCID() << std::endl;
            std::string strPipeServerName = IPCRendererPipePrefix + str.str();
            std::string strPipeClientName = IPCBrowserPipePrefix + str.str();
            
            ipc_msg::SmartType anyMessage(new amo::ipc_msg());
            anyMessage->setMessageName(IPC_CREATE_PIPE_CLIENT);
            anyMessage->getArgumentList()->setValue(0, (amo::any)str.str());
            
            std::shared_ptr<amo::pipe<amo::pipe_type::server> >  m_pRenderPipeServer(
                new amo::pipe<amo::pipe_type::server>(strPipeServerName, IPCDefaultPipeSize));
            std::shared_ptr<amo::pipe<amo::pipe_type::client> >  m_pBrowserPipeClient(
                new amo::pipe<amo::pipe_type::client>(strPipeClientName));
                
            sendProcessMessage(anyMessage);
            
            $clog(amo::cdevel << func_orient << "create pipe server :" <<
                  strPipeServerName << amo::endl);
            /* MessageBoxA(NULL, "3333", "223", MB_OK);*/
            //等待管道建立
            bool rec = m_pRenderPipeServer->connect();
            bool bOk = m_pBrowserPipeClient->connect();
            
            
            if (!rec || !bOk) {
                $clog(amo::cdevel << "connect pipe error" << amo::endl);
                MessageBoxA(NULL, "connect pipe error", "error", MB_OK);
            }
            
            
            amo::shared_ptr<process_exchanger> pRendererProcessExchanger(
                new process_exchanger());
            pRendererProcessExchanger->setPipeServer(m_pRenderPipeServer);
            pRendererProcessExchanger->setPipeClient(m_pBrowserPipeClient);
            pRendererProcessExchanger->setProcessSyncMessageCallback(std::bind(
                        &ipc_base::processSyncMessage, this, std::placeholders::_1,
                        std::placeholders::_2));
                        
            pRendererProcessExchanger->setBrowserID(getIPCID());
            m_pProcessExchangerManager->addExchanger(getIPCID(),
                    pRendererProcessExchanger);
            m_bPipeCreated = true;
            m_heartbeat_timer.restart();
            return true;
        }
        
    };
}



#endif //AMO_IPC_IMPL_7497D7AA_5B23_46D5_B122_044A05144A5F_HPP__



