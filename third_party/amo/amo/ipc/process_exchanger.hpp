// Created by amoylel on 02/03/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_PROCESS_EXCHANGER_CD4EBA6A_9AED_450C_A212_3365E43BFA8D_HPP__
#define AMO_PROCESS_EXCHANGER_CD4EBA6A_9AED_450C_A212_3365E43BFA8D_HPP__





#include <mutex>
#include <functional>
#include <thread>
#include <string>
#include <memory>
#include <unordered_map>

#include <amo/singleton.hpp>
#include <amo/pipe.hpp>

#include <amo/logger.hpp>
#include <tchar.h>
#include <chrono>
#include <amo/ipc/any.hpp>
#include <amo/pipe.hpp>
#include <amo/ipc/ipc_args_pos_info.hpp>
#include <amo/timer.hpp>
#include <map>

namespace {

}

namespace amo {

    enum ipc_type {
        ipc_server,
        ipc_client
    };
    
    class process_exchanger {
    public:
        static std::vector<int8_t> intToBytes(int n) {
            std::vector<int8_t> b(4, 0);
            
            for (int i = 0; i < 4; i++) {
                b[i] = (int8_t)(n >> (24 - i * 8));
            }
            
            return b;
        }
        
        static int bytesToInt(int8_t* b) {
            int mask = 0x000000ff;
            return ((b[0] & mask) << 24) + ((b[1] & mask) << 16) + ((b[2] & mask) << 8) +
                   (b[3] & mask);
        }
        
        static int bytesToInt(std::vector<int8_t>& b) {
            int mask = 0x000000ff;
            return ((b[0] & mask) << 24) + ((b[1] & mask) << 16) + ((b[2] & mask) << 8) +
                   (b[3] & mask);
        }
    public:
        static uint64_t& ipcTimeOut() {
            static uint64_t nIPCTimeout = 0;
            return nIPCTimeout;
        }
    public:
    
        process_exchanger() {
            m_nBrowserID = 0;
        }
        
        /*!
         * @fn	template<typename R> R process_exchanger::exchange();
         *
         * @brief	从管道中读取指定类型数据.
         *
         * @tparam	R	Type of the r.
         *
         * @return	A R.
         */
        
        template<typename R>
        R exchange();
        
        /*!
         * @fn	template<> any process_exchanger::exchange<any>()
         *
         * @brief	从管道中读取任意类型数据  exchange any 特化版本.
         *
         * @tparam	any	Type of any.
         *
         * @return	any.
         */
        
        template<> any exchange<any>() {
            std::unique_lock<std::recursive_mutex> lock(m_mutexServer);
            
            do {
                if (!m_pPipeServer) {
                    break;
                }
                
                if (!readHeader()) {
                    break;
                }
                
                char value_type = readValueType();
                int length = getMessageLength();
                
                if (length == 0) {
                    break;
                }
                
                std::vector<int8_t> msg(length + 1, 0);
                memset(msg.data(), 0, msg.size());
                m_pPipeServer->read(msg.data(), length);
                
                if (!readTail()) {
                    break;
                }
                
                //$clog(amo::cdevel << func_orient << "read data success::" << std::string((char*)msg.data()) << amo::endl;);
                return any(value_type, std::string((char*)msg.data()));		// 返回any类型
            } while (false);
            
            $clog(amo::cdevel << func_orient << "read data error " << amo::endl;);
            return nothing();
        }
        
        /*!
         * @fn	bool process_exchanger::isReady()
         *
         * @brief	Query if this object is ready.
         *
         * @return	true if ready, false if not.
         */
        
        bool isReady() {
            std::unique_lock<std::recursive_mutex> lock(m_mutexServer);
            
            do {
                if (!m_pPipeServer) {
                    break;    //管道是否存在
                }
                
                if (!m_pPipeServer->is_ready()) {
                    break;
                }
                
                return true;
                
            } while (false);
            
            return false;
        }
        
        /*!
         * @fn	template<typename T> bool process_exchanger::exchange(const T& t)
         *
         * @brief	向管道中写入数据.
         *
         * @tparam	T	Generic type parameter.
         * @param	t	The T to process.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        template<typename T>
        bool exchange(const T& t) {
            std::unique_lock<std::recursive_mutex> lock(m_mutexClient);
            
            do {
                if (!m_pPipeClient) {
                    break;    //判断管道是否存在
                }
                
                int nCount = 0;
                
                /*! @brief	写入头部. */
                nCount = m_pPipeClient->write(&IPCPipeMessageHeader, 1);
                
                if (nCount != 1) {
                    break;
                }
                
                /*! @brief	序列化消息. */
                std::string msg = any_to_string(t);
                /*! @brief	获取消息长度. */
                std::vector<int8_t> msg_length = intToBytes(msg.size());
                /*! @brief	写入数据类型. */
                nCount = m_pPipeClient->write(&any_value_type<T>::value, 1);
                
                if (nCount != 1) {
                    break;
                }
                
                /*! @brief	写入消息长度. */
                nCount = m_pPipeClient->write(msg_length.data(), 4);
                
                if (nCount != 4) {
                    break;
                }
                
                /*! @brief	写入消息. */
                nCount = m_pPipeClient->write(msg.data(), msg.size());
                
                if (nCount != msg.size()) {
                    break;
                }
                
                /*! @brief	写入尾部. */
                nCount = m_pPipeClient->write(&IPCPipeMessageTail, 1);
                
                if (nCount != 1) {
                    break;
                }
                
                return true;
            } while (false);
            
            $clog(amo::cdevel << func_orient << "write data fail:: " << any_to_string(
                      t) << amo::endl;);
            return false;
        }
        
        /*!
         * @fn	template<> bool process_exchanger::exchange<any>(const any& t)
         *
         * @brief	Exchanges the given t.
         *
         * @tparam	any	Type of any.
         * @param	t	any to process.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        template<> bool exchange<any>(const any& t) {
            std::unique_lock<std::recursive_mutex> lock(m_mutexClient);
            
            do {
                if (!m_pPipeClient) {
                    break;    //判断管道是否存在
                }
                
                int nCount = 0;
                /*! @brief	写入头部. */
                nCount = m_pPipeClient->write(&IPCPipeMessageHeader, 1);
                
                if (nCount != 1) {
                    break;
                }
                
                /*! @brief	序列化消息. */
                const std::string& msg = t.value();
                /*! @brief	获取消息长度. */
                std::vector<int8_t> msg_length = intToBytes(msg.size());
                /*! @brief	写入数据类型. */
                nCount = m_pPipeClient->write(&t.type(), 1);
                
                if (nCount != 1) {
                    break;
                }
                
                /*! @brief	写入消息长度. */
                nCount = m_pPipeClient->write(msg_length.data(), 4);
                
                if (nCount != 4) {
                    break;
                }
                
                /*! @brief	写入消息. */
                nCount = m_pPipeClient->write(msg.data(), msg.size());
                
                if (nCount != msg.size()) {
                    break;
                }
                
                /*! @brief	写入尾部. */
                nCount = m_pPipeClient->write(&IPCPipeMessageTail, 1);
                
                if (nCount != 1) {
                    break;
                }
                
                return true;
            } while (false);
            
            $clog(amo::cdevel << func_orient << "write data fail:: " << t.value() <<
                  amo::endl;);
            return false;
        }
        
        template<> bool exchange<ipc_msg::SmartType >(
            const ipc_msg::SmartType&  t) {
            std::unique_lock<std::recursive_mutex> lock(m_mutexClient);
            
            //$clog(amo::cdevel << func_orient << "write data begin:" << t->toJson().to_string() << amo::endl;);
            
            do {
                int nCount = 0;
                
                if (!m_pPipeClient) {
                    break;    //判断管道是否存在
                }
                
                /*! @brief	写入头部. */
                nCount = m_pPipeClient->write(&IPCPipeMessageHeader, 1);
                
                if (nCount != 1) {
                    break;
                }
                
                
                //std::vector<int8_t> id_length = IntToBytes(0);
                /*! @brief	写入数据类型. */
                nCount = m_pPipeClient->write(&any_value_type<amo::ipc_msg>::value, 1);
                
                if (nCount != 1) {
                    break;
                }
                
                /*! @brief	序列化消息. */
                std::string msg = any_to_string(*t);
                /*! @brief	获取消息长度. */
                std::vector<int8_t> msg_length = intToBytes(msg.size());
                /*! @brief	写入消息长度. */
                nCount = m_pPipeClient->write(msg_length.data(), 4);
                
                if (nCount != 4) {
                    break;
                }
                
                /*! @brief	写入消息. */
                nCount = m_pPipeClient->write(msg.data(), msg.size());
                
                if (nCount != msg.size()) {
                    break;
                }
                
                /*! @brief	写入尾部. */
                nCount = m_pPipeClient->write(&IPCPipeMessageTail, 1);
                
                if (nCount != 1) {
                    break;
                }
                
                //$clog(amo::cdevel << func_orient << "write data success:" << t->toJson().to_string() << amo::endl;);
                return true;
            } while (false);
            
            $clog(amo::cdevel << func_orient << "write data fail:: " <<
                  t->toJson().to_string() << amo::endl;);
            return false;
            
            
        }
        
        /*!
         * @fn	bool process_exchanger::ReadHeader()
         *
         * @brief	从管道中读取头部，判断是否合法.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool readHeader() {
            char head = '\0';
            m_pPipeServer->read(&head, 1);
            
            if (head != IPCPipeMessageHeader) {
                return false;
            }
            
            return true;
        }
        
        /*!
         * @fn	char process_exchanger::ReadValueType()
         *
         * @brief	读取数据类型.
         *
         * @return	The value type.
         */
        
        char readValueType() {
            char type = '\0';
            m_pPipeServer->read(&type, 1);
            return type;
        }
        
        /*!
         * @fn	bool process_exchanger::ReadTail()
         *
         * @brief	读取尾部，判断是否合法.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool readTail() {
            char tail = '\0';
            m_pPipeServer->read(&tail, 1);
            
            if (tail != IPCPipeMessageTail) {
                return false;
            }
            
            return true;
        }
        
        int getValueID() {
            std::vector<int8_t> length(4, 0);
            m_pPipeServer->read(length.data(), 4);
            return bytesToInt(length);
        }
        
        /*!
         * @fn	int process_exchanger::GetMessageLength()
         *
         * @brief	读取消息长度.
         *
         * @return	The message length.
         */
        
        int getMessageLength() {
            std::vector<int8_t> length(4, 0);
            m_pPipeServer->read(length.data(), 4);
            return bytesToInt(length);
        }
        
        /*!
         * @fn	void process_exchanger::SetPipeServer(
         * 		std::shared_ptr<amo::pipe<amo::pipe_type::server> > ptr)
         *
         * @brief	设置管道服务端.
         *
         * @param	ptr	The pointer.
         */
        
        void setPipeServer(std::shared_ptr<amo::pipe<amo::pipe_type::server> > ptr) {
            m_pPipeServer = ptr;
        }
        
        /*!
         * @fn	void process_exchanger::SetPipeClient(
         * 		std::shared_ptr<amo::pipe<amo::pipe_type::client> > ptr)
         *
         * @brief	设置管道客户端.
         *
         * @param	ptr	The pointer.
         */
        
        void setPipeClient(std::shared_ptr<amo::pipe<amo::pipe_type::client> > ptr) {
            m_pPipeClient = ptr;
        }
        
        /*!
         * @fn	void process_exchanger::setProcessSyncMessageCallback(
         * 		std::function<bool(int, ipc_msg::SmartType)> fn)
         *
         * @brief	收到同步消息时怎么处理.
         *
         * @param	fn	The function.
         */
        
        void setProcessSyncMessageCallback(
            std::function<bool(int, ipc_msg::SmartType)> fn) {
            m_fnSyncMessageCallback = fn;
        }
        
        /*!
         * @fn	std::function<bool(int, ipc_msg::SmartType)>& getProcessSyncMessageCallback()
         *
         * @brief	Callback, called when the get process synchronise message.
         *
         * @return	The process synchronise message callback.
         */
        
        std::function<bool(int, ipc_msg::SmartType)>&
        getProcessSyncMessageCallback() {
            return m_fnSyncMessageCallback;
        }
        
        /*!
         * @fn	void process_exchanger::setBrowserID(int browserID)
         *
         * @brief	Sets browser identifier.
         *
         * @param	browserID	Identifier for the browser.
         */
        
        void setBrowserID(int browserID) {
            m_nBrowserID = browserID;
        }
        
        /*!
         * @fn	int process_exchanger::getBrowserID() const
         *
         * @brief	Gets browser identifier.
         *
         * @return	The browser identifier.
         */
        
        int getBrowserID() const {
            return m_nBrowserID;
        }
        
        /*!
         * @fn	std::shared_ptr<amo::pipe<amo::pipe_type::server> > process_exchanger::getPipeServer()
         *
         * @brief	Gets pipe server.
         *
         * @return	The pipe server.
         */
        
        std::shared_ptr<amo::pipe<amo::pipe_type::server> > getPipeServer() {
            return m_pPipeServer;
        }
        
        /*!
         * @fn	std::shared_ptr<amo::pipe<amo::pipe_type::client> > process_exchanger::getPipeClient()
         *
         * @brief	Gets pipe client.
         *
         * @return	The pipe client.
         */
        
        std::shared_ptr<amo::pipe<amo::pipe_type::client> > getPipeClient() {
            return m_pPipeClient;
        }
        
        //template<typename R>
        //R waitResult();
        //
        //
        //// 通过管道消息 创建进程消息
        //template<> any waitResult() {
        //    // 没有函数调过好像
        //    while (true) {
        //        any any = TryGetResult<any>();
        //
        //        if (any.type() == any_value_type<amo::nothing>::value) {
        //            std::this_thread::sleep_for(std::chrono::microseconds(50));
        //            continue;
        //        }
        //
        //        return any;
        //    }
        //}
        
        template<typename R>
        R tryGetResult();
        
        template<> any tryGetResult() {
            if (!isReady()) {
                return amo::nothing();
            }
            
            if (!m_fnSyncMessageCallback) {
                return amo::nothing();
            }
            
            any ret = exchange<any>();
            
            if (ret.type() == any_value_type<ipc_msg>::value) {
                ipc_msg::SmartType msg(new ipc_msg());
                *msg = ret;
                $clog(amo::cdevel << func_orient << ret.value() << amo::endl;);
                m_fnSyncMessageCallback(getBrowserID(), msg);
                return amo::nothing();
            } else {
                return ret;
            }
        }
        
        any tryProcessMessage() {
            $clog(amo::cdevel << func_orient << amo::endl;);
            
            if (!isReady()) {
                //$clog(amo::cdevel << func_orient << "Not Ready" << amo::endl;);
                return nothing();
            }
            
            if (!m_fnSyncMessageCallback) {
                return nothing();
            }
            
            any ret = exchange<any>();
            
            if (ret.type() == any_value_type<amo::ipc_msg>::value) {
                // 如果是一个进程消息，说明对方也在执行同步调用，那么执行这个调用
                ipc_msg::SmartType msg(new amo::ipc_msg());
                *msg = ret;
                m_fnSyncMessageCallback(getBrowserID(), msg);
                //$clog(amo::cdevel << __FUNCTION__ << ", " << msg->getID() << amo::endl;);
                return nothing();
            } else if (ret.type() == any_value_type<ipc_result>::value) {
                return ret;
            } else {
                int iii = 44;
                ++iii;
                
                //throw std::runtime_error("处理到不应该出现的消息，只能收到ProcessMessage类");
                throw std::runtime_error("invaild message，just accept ProcessMessage ");
            }
            
            $clog(amo::cdevel << func_orient << "tryProcessMessage fail::: " <<
                  ret.value() << amo::endl;);
            return nothing();
        }
    protected:
        /*! @brief	管道服务端，用来接收数据. */
        std::shared_ptr<amo::pipe<amo::pipe_type::server> > m_pPipeServer;
        /*! @brief	管道客户羰，用来写入数据. */
        std::shared_ptr<amo::pipe<amo::pipe_type::client> > m_pPipeClient;
        /*! @brief	数据读取锁，同时只能有一个线程读取数据. */
        std::recursive_mutex m_mutexServer;
        /*! @brief	数据写入锁，同时只能有一个线程写入数据. */
        std::recursive_mutex m_mutexClient;
        /*! @brief	The synchronise message callback. */
        std::function<bool(int, ipc_msg::SmartType)> m_fnSyncMessageCallback;
        /*! @brief	Identifier for the browser. */
        int m_nBrowserID;
    };
    
    
    template<typename T>
    class process_exchanger_manager {
    public:
        int m_nN;
        
        process_exchanger_manager() {
            m_nTimeout = process_exchanger::ipcTimeOut();
        }
        
        /*!
         * @fn	template<typename R> R process_exchanger_manager::exchange(int id)
         *
         * @brief	读取管道数据.
         *
         * @tparam	R	Type of the r.
         * @param	id	The identifier.
         *
         * @return	A R.
         */
        
        template<typename R>
        R exchange(int id) {
            auto iter = m_mpBrowserExchanger.find(id);
            
            if (iter == m_mpBrowserExchanger.end() || !iter->second) {
                return R();
            }
            
            R ret = iter->second->exchange<R>();
            return ret;
        }
        
        /*!
         * @fn	template<> any process_exchanger_manager::exchange(int id)
         *
         * @brief	读取管道数据.
         *
         * @param	id	The identifier.
         *
         * @return	any.
         */
        
        template<>
        any exchange(int id) {
            auto iter = m_mpBrowserExchanger.find(id);
            
            if (iter == m_mpBrowserExchanger.end() || !iter->second) {
                return nothing();
            }
            
            any ret = iter->second->exchange<any>();
            return ret;
        }
        
        bool isReady(int id) {
            auto iter = m_mpBrowserExchanger.find(id);
            
            if (iter == m_mpBrowserExchanger.end() || !iter->second) {
                return false;
            }
            
            return iter->second->isReady<R>();
        }
        
        /*!
         * @fn	template<typename T> bool process_exchanger_manager::exchange(int id, const T& t)
         *
         * @brief	写入管道数据.
         *
         * @tparam	T	Generic type parameter.
         * @param	id	The identifier.
         * @param	t 	The T to process.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        template<typename T>
        bool exchange(int id, const T& t) {
            auto iter = m_mpBrowserExchanger.find(id);
            
            if (iter == m_mpBrowserExchanger.end() || !iter->second) {
                $clog(amo::cdevel << func_orient << "not found pipe " << amo::endl;);
                return false;
            }
            
            return iter->second->exchange(t);
        }
        
        /*!
         * @fn	void process_exchanger_manager::addExchanger(int id, std::shared_ptr<T> ptr)
         *
         * @brief	添加数据交换管道到管理器中.
         *
         * @param	id 	The identifier.
         * @param	ptr	The pointer.
         */
        
        void addExchanger(int id, std::shared_ptr<T> ptr) {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_mpBrowserExchanger.insert(std::pair<int, std::shared_ptr<T> >(id, ptr));
        }
        
        /*!
         * @fn	void process_exchanger_manager::RemoveExchanger(int id)
         *
         * @brief	从管理器中移除数据交换管道.
         *
         * @param	id	The identifier.
         */
        
        void removeExchanger(int id) {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_mpBrowserExchanger.erase(id);
        }
        
        /*!
         * @fn	std::shared_ptr<T> process_exchanger_manager::findExchanger(int id)
         *
         * @brief	Searches for the first exchanger.
         *
         * @param	id	The identifier.
         *
         * @return	The found exchanger.
         */
        
        std::shared_ptr<T> findExchanger(int id) {
            auto iter = m_mpBrowserExchanger.find(id);
            
            if (iter == m_mpBrowserExchanger.end()) {
                return std::shared_ptr<T>();
            }
            
            return iter->second;
        }
        
        template<typename R>
        R waitResult(int id, int message_id);
        
        /*!
         * @fn	template<> any process_exchanger_manager::waitResult(int id, int message_id)
         *
         * @brief	Wait result.
         *
         * @param	id		  	The identifier.
         * @param	message_id	Identifier for the message.
         *
         * @return	any.
         */
        
        template<>
        any waitResult(int id, int message_id) {
            // 先看管道是否存在
            if (m_mpBrowserExchanger.find(id) == m_mpBrowserExchanger.end()) {
                return undefined();
            }
            
            
            amo::timer t;
            
            while (true) {
            
                if (m_nTimeout > 0) {
                    if (t.elapsed() > m_nTimeout) {
                    
                        $clog(amo::cdevel << func_orient << ", " << m_nN << ", deadlock, ID: " <<
                              message_id << amo::endl;);
						$clog(amo::cdevel << func_orient << ", " << m_nN << ", deadlock, ID: " <<
							message_id << amo::endl;);
						$clog(amo::cdevel << func_orient << ", " << m_nN << ", deadlock, ID: " <<
							message_id << amo::endl;);
						$clog(amo::cdevel << func_orient << ", " << m_nN << ", deadlock, ID: " <<
							message_id << amo::endl;);
                        m_oDeadlockIDs.insert(std::make_pair(id, message_id));
                        
                        if (getDeadlockCallback()) {
                            return  getDeadlockCallback()();
                        } else {
                            return deadlock();
                        }
                    }
                } else {
                    if (t.elapsed() > 5000) {
                        // 打印消息出来提示死锁
						$clog(amo::cdevel << func_orient << ", " << m_nN << ", deadlock, ID: " <<
							message_id << amo::endl;);
						$clog(amo::cdevel << func_orient << ", " << m_nN << ", deadlock, ID: " <<
							message_id << amo::endl;);
						$clog(amo::cdevel << func_orient << ", " << m_nN << ", deadlock, ID: " <<
							message_id << amo::endl;);
						$clog(amo::cdevel << func_orient << ", " << m_nN << ", deadlock, ID: " <<
							message_id << amo::endl;);
                              
                    }
                }
                
                any val = findCache(id, message_id);
                
                if (val.type() == any_value_type<amo::ipc_result>::value) {
                    removeCache(id, message_id);
                    amo::ipc_result result = val;
                    //$clog(amo::cdevel << __FUNCTION__ << ", " <<  result.getID() << amo::endl;);
                    return result.getResult();
                }
                
                for (auto& p : m_mpBrowserExchanger) {
                
                
                    if (p.first == id) {
                        any ret = p.second->tryGetResult<any>();
                        
                        if (ret.type() == any_value_type<amo::nothing>::value) {
                            std::this_thread::sleep_for(std::chrono::microseconds(10));
                        } else if (ret.type() == any_value_type<amo::ipc_result>::value) {
                            amo::ipc_result result = ret;
                            
                            if (result.getID() == message_id) {
                                $clog(amo::cdevel << __FUNCTION__ << ", " << result.getID() << amo::endl;);
                                return result.getResult();
                            }
                            
                            insertCache(p.first, result.getID(), ret);
                        } else {
                            // 不应该出现
                            $clog(amo::cdevel << func_orient << amo::endl;);
                        }
                    } else {
                        tryProcessMessage(p.first);
                        //// 处理同步消息
                        //any ret = p.second->tryProcessMessage();
                        //
                        //if (ret.type() == any_value_type<amo::ipc_result>::value) {
                        //    amo::ipc_result result = ret;
                        //
                        //    if (result.getID() == message_id) {
                        //        // 这里不应该出现
                        //        $clog(amo::cdevel << __FUNCTION__ << ", " << result.getID() << amo::endl;);
                        //        return result.GetResult();
                        //    }
                        //
                        //    InsertCache(p.first, result.getID(), ret);
                        //}
                    }
                }
            }
            
            return amo::nothing();
        }
        
        /*!
         * @fn	void process_exchanger_manager::InsertCache(int browserID, int messageID, any& ret)
         *
         * @brief	Inserts a cache.
         *
         * @param	browserID  	Identifier for the browser.
         * @param	messageID  	Identifier for the message.
         * @param [in,out]	ret	The ret.
         */
        
        void insertCache(int browserID, int messageID, any& ret) {
            $clog(amo::cdevel << func_orient << ret.value() << amo::endl;);
            
            if (m_oDeadlockIDs.find(std::make_pair(browserID,
                                                   messageID)) != m_oDeadlockIDs.end()) {
                //$cwarn("处理到已放弃的死锁ID，该消息不会被缓存");
				$cwarn("invalid ID, discard");
                m_oDeadlockIDs.erase(std::make_pair(browserID, messageID));
                return;
            }
            
            auto iter = m_oResultCache.find(browserID);
            
            if (iter == m_oResultCache.end()) {
                std::map<int, any> val;
                val[messageID] = ret;
                m_oResultCache.insert(std::pair<int, std::map<int, any> >(browserID,
                                      val));
                return;
            } else {
                auto& p = iter->second;
                p[messageID] = ret;
                
                if (p.size() > 200) {
                    //$cdevel("移除缓存:{0}", p.begin()->first);
                    p.erase(p.begin());
                }
            }
        }
        
        /*!
         * @fn	any process_exchanger_manager::FindCache(int browserID, int messageID)
         *
         * @brief	Searches for the first cache.
         *
         * @param	browserID	Identifier for the browser.
         * @param	messageID	Identifier for the message.
         *
         * @return	The found cache.
         */
        
        any findCache(int browserID, int messageID) {
            auto iter = m_oResultCache.find(browserID);
            
            if (iter == m_oResultCache.end()) {
                return amo::nothing();
            }
            
            auto& p = iter->second;
            auto it = iter->second.find(messageID);
            
            if (it == p.end()) {
                return amo::nothing();
            }
            
            return it->second;
        }
        
        /*!
         * @fn	void process_exchanger_manager::RemoveCache(int browserID, int messageID)
         *
         * @brief	Removes the cache.
         *
         * @param	browserID	Identifier for the browser.
         * @param	messageID	Identifier for the message.
         */
        
        void removeCache(int browserID, int messageID) {
            auto iter = m_oResultCache.find(browserID);
            
            if (iter == m_oResultCache.end()) {
                return;
            }
            
            iter->second.erase(messageID);
        }
        any tryProcessMessage(int id) {
            any ret = tryProcessMessageImpl(id);
            
            while (true) {
                any ret = tryProcessMessageImpl(id);
                
                if (ret.is<ipc_result>()) {
                    continue;
                }
                
                break;
            }
            
            return ret;
        }
        
        
        
        std::function<any()> getDeadlockCallback() const {
            return m_fnDeadlockCallback;
        }
        void setDeadlockCallback(std::function<any()> val) {
            m_fnDeadlockCallback = val;
        }
    protected:
        any tryProcessMessageImpl(int id) {
        
            std::shared_ptr<T> ptr = findExchanger(id);
            
            if (!ptr) {
                $clog(amo::cdevel << func_orient << "nothing" << amo::endl;);
                return nothing();
            }
            
            any ret = ptr->tryProcessMessage();
            
            if (ret.type() == any_value_type<ipc_result>::value) {
                ipc_result result = ret;
                
                insertCache(id, result.getID(), ret);
                
                //$clog(amo::cdevel << func_orient << "处理到其他结果" << amo::endl;);
                // 一直处理，直到管道中没有消息为止
                //tryProcessMessage(id);
            } else if (ret.type() != any_value_type<nothing>::value) {
                assert(false);
            }
            
            $clog(amo::cdevel << func_orient << ret.value() << amo::endl;);
            return ret;
        }
    private:
        /*! @brief	保存管道. */
        std::unordered_map<int, std::shared_ptr<T> > m_mpBrowserExchanger;
        /*! @brief	读写锁. */
        std::mutex m_mutex;
        /*! @brief	同步调用结果缓存. */
        std::unordered_map<int, std::map<int, any> > m_oResultCache;
        
        /** @brief	进程死锁回调函数. */
        std::function<any()> m_fnDeadlockCallback;
        
        /** @brief	发生死锁的消息ID(BrowserID + MessageID)，如果执行的结果里面有这个ID，说明该消息已经被放弃了，直接丢掉就可以了. */
        std::set<std::pair<int, int> > m_oDeadlockIDs;
        
        /** @brief	任务超时时间，如果超过这个时间任务还未完成，那么认为程序发生了死锁将放弃该任务的执行，默认为1000ms. */
        uint64_t m_nTimeout;
    };
    
    
}

#endif //AMO_PROCESS_EXCHANGER_CD4EBA6A_9AED_450C_A212_3365E43BFA8D_HPP__
