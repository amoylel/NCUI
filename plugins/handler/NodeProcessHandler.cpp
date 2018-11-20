#include "NodeProcessHandler.h"

#include <amo/path.hpp>
#include "ipc/Any.hpp"
#include "ipc/IPCMessage.hpp"
#include <thread>
#include <sstream>

#include "ipc/NodeProcessExchanger.hpp"
#include "transfer/Transfer.hpp"
#include <vector>
#include "transfer/FunctionWrapper.hpp"
#include <unordered_map>
#include <amo/loader.hpp>

#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <vector>
#include <amo/string.hpp>
#include <amo/path.hpp>
#include <shellapi.h>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <amo/json.hpp>

//#include "include/v8wrap.h"

#include "nan/nan.h"
#include "transfer/NodeTransferMgr.hpp"
#include "module/IPCNodeV8Handler.h"
namespace amo {


    class NodeTask {
    public:
        NodeTask() {
            req.data = this;
            m_nID = 0;
        }
        void SetID(int id) {
            m_nID = id;
        }
        
        int GetID() const {
            return m_nID;
        }
        uv_work_t req;
        IPCMessage::SmartType anyMessage;
        int m_nID;
        
        v8::Isolate* isolate;
        v8::Persistent<v8::Function> callback;
        
    };
    
    class NodeTaskManager : public amo::singleton<NodeTaskManager> {
    public:
        NodeTaskManager() {
            m_nLastID = 0;
        }
        
        std::shared_ptr<NodeTask> New() {
            std::shared_ptr<NodeTask> pTask(new NodeTask());
            pTask->SetID(++m_nLastID);
            m_oTaskMap[pTask->GetID()] = pTask;
            return pTask;
        }
        
        void Erase(int id) {
            m_oTaskMap.erase(id);
        }
        
    private:
        int m_nLastID;
        std::unordered_map<int, std::shared_ptr<NodeTask> > m_oTaskMap;
    };
    
    class Point {
    public:
        float x;
        float y;
        
        float length() {
            return sqrt(x * x + y * y);
        }
    };
    
    NodeProcessHandler::~NodeProcessHandler() {
        closeMessageQueue();
        
    }
    
    void NodeProcessHandler::Initialize() {
    
        //if (amo::log::initialize(false, true)) {
        //    auto sink1 = std::make_shared<spdlog::sinks::msvc_sink_mt>();
        //    //auto sink2 = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logfile", "txt", 23, 59);
        //    //sink2->set_level(amo::log::level::trace);
        //    /* auto sink3 = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        //    "logfile", SPDLOG_FILENAME_T("txt"), 1048576 * 5, 3);*/
        //
        //    //amo::log::add_sink(sink1);
        //    //amo::log::add_sink(sink2);
        //    //amo::log::add_sink(sink3);
        //    amo::log::set_level(amo::log::level::trace);
        //    amo::log::set_pattern("[%Y-%m-%d %H:%M:%S][%l] %v");
        //    $clog(amo::cinfo << "日志初始化成功" << amo::endl);
        //}
        
        
        
        //v8wrap::ClassDescription<Point> desc;
        //desc.Field<float, &Point::x>("x");
        //desc.Field<float, &Point::y>("y");
        //desc.Method<float()>().Set<&Point::length>("length");
        Isolate* isolate = Isolate::GetCurrent();
        /*v8::Context::GetIsolate()
        	v8::Context::GetCurrent()->Global()->Set(v8::NewFromUtf8(v8::Isolate::GetCurrent(),String::New("Point"), desc.FunctionTemplate()->GetFunction());*/
        
        std::shared_ptr<IPCNodeV8Handler> pTransfer =
            ClassTransfer::createTransfer<IPCNodeV8Handler>();
            
            
        NodeTransferMgr::getInstance()->addTransfer(m_nPipeID, pTransfer);
        
        DWORD id = GetCurrentProcessId();
        std::stringstream stream;
        stream << "libNCUI_process_" << id;
        std::string strSharedMemoryName = stream.str();
        using namespace boost::interprocess;
        managed_shared_memory segment(open_only, strSharedMemoryName.c_str());
        std::stringstream sss;
        sss << segment.get_size();
        std::string ssssb = sss.str();
        //     MessageBoxA(NULL, strSharedMemoryName.c_str(), ("Initialize"), MB_OK);
        //     MessageBoxA(NULL, ssssb.c_str(), "223", MB_OK);
        
        std::pair<std::string*, managed_shared_memory::size_type> res;
        res = segment.find<std::string>("messageQueueName");
        
        
        if (res.first == NULL) {
            //     MessageBoxA(NULL, "not found", ("Initialize"), MB_OK);
            return;
        } else {
            sss.clear();
            sss << res.second;
            sss << "," << res.first->size();
            ssssb = sss.str();
            //       MessageBoxA(NULL, ssssb.c_str(), "223", MB_OK);
            ssssb = "";
            
            for (size_t i = 0; i < res.first->size(); ++i) {
                ssssb.push_back(res.first->at(i));
            }
            
            ssssb.push_back('\0');
            
            
            //    MessageBoxA(NULL, res.first->c_str(), ("Initialize"), MB_OK);
        }
        
        
        setMessageQueue(*res.first);
        
        
        std::stringstream str;
        str << m_nPipeID << std::endl;
        std::string strPipeServerName = RendererPipePrefix + str.str();
        std::string strPipeClientName = BrowserPipePrefix + str.str();
        
        IPCMessage::SmartType anyMessage(new amo::IPCMessage());
        anyMessage->setMessageName(MSG_CREATE_PIPE_CLIENT);
        anyMessage->getArgumentList()->setValue(0, (amo::Any)str.str());
        
        std::shared_ptr<amo::pipe<amo::pipe_type::server> >  m_pRenderPipeServer(
            new amo::pipe<amo::pipe_type::server>(strPipeServerName, DefaultPipeSize));
        std::shared_ptr<amo::pipe<amo::pipe_type::client> >  m_pBrowserPipeClient(
            new amo::pipe<amo::pipe_type::client>(strPipeClientName));
            
        SendMessageToUI(anyMessage);
        
        $clog(amo::cdevel << func_orient << "create pipe service:" << strPipeServerName
              << amo::endl);
        //  MessageBoxA(NULL, "3333", "223", MB_OK);
        //等待管道建立
        bool rec = m_pRenderPipeServer->connect();
        bool bOk = m_pBrowserPipeClient->connect();
        
        
        if (!rec || !bOk) {
            $clog(amo::cdevel << "connect pipe fail" << amo::endl);
            //    MessageBoxA(NULL, "connect pipe fail", "223", MB_OK);
        }
        
        
        amo::shared_ptr<ProcessExchanger> pRendererProcessExchanger(
            new ProcessExchanger());
        pRendererProcessExchanger->setPipeServer(m_pRenderPipeServer);
        pRendererProcessExchanger->setPipeClient(m_pBrowserPipeClient);
        pRendererProcessExchanger->setProcessSyncMessageCallback(
            std::bind(&NodeProcessHandler::ProcessSyncMessage,
                      std::placeholders::_1,
                      std::placeholders::_2));
                      
        pRendererProcessExchanger->setBrowserID(m_nPipeID);
        NodeProcessExchangerManager::get_instance()->addExchanger(m_nPipeID,
                pRendererProcessExchanger);
                
        amo::Any ret = NodeProcessExchangerManager::get_instance()->exchange <amo::Any>
                       (m_nPipeID);
                       
        if (ret.type() == amo::AnyValueType<bool>::value) {
        
            return;
        }
        
        $clog(amo::cdevel << ret.value() << amo::endl);
        
        amo::u8json jsonArr(ret.value());
        std::vector<amo::u8json> arr = jsonArr.to_array();
        
        for (auto& json : arr) {
            FunctionWrapperMgr mgr = FunctionWrapperMgr::fromJson(json);
            std::string handlerName = mgr.name();
            m_oClassMap[handlerName] = mgr;
        }
        
        uv_idle_init(uv_default_loop(), &idler);
        
        uv_idle_start(&idler, wait_for_a_while);
        return;
    }
    
    void NodeProcessHandler::UnInitialize() {
        uv_idle_stop(&idler);
    }
    
    
    std::string NodeProcessHandler::ObjectToString(Local<Value> value) {
        String::Utf8Value utf8_value(value);
        return std::string(*utf8_value);
    }
    
    
    void NodeProcessHandler::setMessageQueue(const std::string& strName) {
        m_strMessageQueueName = strName;
        std::string strReaderName = m_strMessageQueueName + "1";
        std::string strWriterName = m_strMessageQueueName + "2";
        
        m_pMessageQueueReader.reset(new message_queue(open_only,
                                    strReaderName.c_str()));
        m_pMessageQueueWriter.reset(new message_queue(open_only,
                                    strWriterName.c_str()));
    }
    
    void NodeProcessHandler::closeMessageQueue() {
    
        DWORD id = GetCurrentProcessId();
        std::stringstream stream;
        stream << "libNCUI_process_" << id;
        std::string strSharedMemoryName = stream.str();
        using namespace boost::interprocess;
        shared_memory_object::remove(strSharedMemoryName.c_str());
        
        std::string strReaderName = m_strMessageQueueName + "1";
        std::string strWriterName = m_strMessageQueueName + "2";
        
        message_queue::remove(strReaderName.c_str());
        message_queue::remove(strWriterName.c_str());
        
        m_pMessageQueueWriter.reset();
        m_pMessageQueueReader.reset();
        
        
    }
    
    void NodeProcessHandler::wait_for_a_while(uv_idle_t* handle) {
    
        boost::posix_time::ptime pt =
            boost::posix_time::microsec_clock::universal_time() +
            boost::posix_time::milliseconds(200);
        char str[10000] = { 0 };
        unsigned int priority = 0;
        message_queue::size_type recvd_size = 0;
        
        while (m_pMessageQueueReader->timed_receive(&str, 10000, recvd_size, priority,
                pt)) {
            if (recvd_size == 0) {
                assert(false && "invalid data");
                return;
            }
            
            $cdevel("recv msg from main process: {}", str);
            amo::u8json json(str);
            std::shared_ptr<IPCMessage> anyMessage(new IPCMessage());
            *anyMessage = IPCMessage::fromJson(json);
            
            if (anyMessage->getArgumentList()->getString(IPCArgsPosInfo::FuncName) ==
                    "quit") {
                //MessageBox(NULL, _T("quit"), _T(""), MB_OK);
                uv_idle_stop(handle);
                closeMessageQueue();
                return;
            } else {
                OnNativeMessageRecv(anyMessage);
            }
        }
        
    }
    
    
    
    bool NodeProcessHandler::ProcessSyncMessage(int nID,
            IPCMessage::SmartType anyMessage) {
        OnNativeMessageRecv(anyMessage);
        return true;
    }
    
    
    
    bool NodeProcessHandler::SendMessageToNode(IPCMessage::SmartType anyMessage) {
        std::shared_ptr<NodeTask> pTask = NodeTaskManager::get_instance()->New();
        pTask->anyMessage = anyMessage;
        
        uv_queue_work(uv_default_loop(),
                      &pTask->req,
                      OnBeforeNativeMessage,
                      (uv_after_work_cb)OnNativeMessageEnd);
        return true;
    }
    
    bool NodeProcessHandler::SendMessageToUI(IPCMessage::SmartType anyMessage) {
        amo::u8json json = anyMessage->toJson();
        std::string str = json.to_string();
        char cc[10000] = { 0 };
        strcpy(cc, str.c_str());
        
        try {
            m_pMessageQueueWriter->send(cc, 10000, 0);
            return true;
        } catch (std::exception& e) {
            $clog(amo::cdevel << e.what() << amo::endl);
            MessageBoxA(NULL, "SEND MESSAGE TO UI", (""), MB_OK);
            return false;
        }
    }
    
    void NodeProcessHandler::OnNativeMessageRecv(IPCMessage::SmartType anyMessage) {
        const std::string& strMessageName = anyMessage->getMessageName();
        
        
        $clog(amo::cdevel << func_orient << anyMessage->toJson().to_string() <<
              amo::endl);
              
        if (strMessageName == MSG_PROCESS_SYNC_EXECUTE) {
            NodeProcessExchangerManager::get_instance()->tryProcessMessage(m_nPipeID);
        }
        
        if (strMessageName == MSG_NATIVE_EXECUTE
                || strMessageName == MSG_NATIVE_SYNC_EXECUTE
                || strMessageName ==
                MSG_NATIVE_ASYNC_EXECUTE) {				//JS调用C++，此消息不向页面返回结果
                
            NodeTransferMgr::getInstance()->onMessageTransfer(anyMessage);
        }
        
        return;
    }
    
    void NodeProcessHandler::OnBeforeNativeMessage(uv_work_t* r) {
        // 在单独的工作线程中,不能处理JS
        NodeTask* pTask = reinterpret_cast<NodeTask*>(r->data);
        //OnNativeMessageRecv(pTask->anyMessage);
    }
    
    void NodeProcessHandler::OnNativeMessageEnd(uv_work_t* r) {
        NodeTask* pTask = reinterpret_cast<NodeTask*>(r->data);
        OnNativeMessageRecv(pTask->anyMessage);
        NodeTaskManager::get_instance()->Erase(pTask->GetID());
    }
    
    std::shared_ptr<NodeMessageEmitter> NodeProcessHandler::getNativeRunner() {
        std::shared_ptr<NodeMessageEmitter> pLauncher(new NodeMessageEmitter(m_nPipeID,
                -1));
        return pLauncher;
    }
    
    std::unordered_map<std::string, FunctionWrapperMgr >&
    NodeProcessHandler::getClassMap() {
        return m_oClassMap;
    }
    
    std::shared_ptr<amo::loader> NodeProcessHandler::m_loader;
    std::unordered_map<std::string, FunctionWrapperMgr >
    NodeProcessHandler::m_oClassMap;
    uv_idle_t NodeProcessHandler::idler;
    std::shared_ptr<boost::interprocess::message_queue>
    NodeProcessHandler::m_pMessageQueueReader;
    std::shared_ptr<boost::interprocess::message_queue>
    NodeProcessHandler::m_pMessageQueueWriter;
    std::string NodeProcessHandler::m_strMessageQueueName;
    
}

