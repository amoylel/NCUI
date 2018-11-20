// Created by amoylel on 25/06/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_SERVICE_84826FAF_0CA1_4F48_BF0A_5A32CD9F5D79_HPP__
#define AMO_SERVICE_84826FAF_0CA1_4F48_BF0A_5A32CD9F5D79_HPP__




#include <memory>
#include <functional>
#include <unordered_map>

#include <amo/json.hpp>
#include <amo/json_helper.hpp>
#include <amo/timer.hpp>
#include <amo/ipc/any.hpp>
#include <amo/looper_executor.hpp>
#include <amo/asio/tcp/socket.hpp>

#include <amo/uid.hpp>


#ifndef REGISTER_EXT_MSG_HANDLE_BEGIN
#define REGISTER_EXT_MSG_HANDLE_BEGIN(ClassName, ptr2, service_ptr2)\
		{\
			typedef ClassName ClassType;\
			const auto& ptr = ptr2;\
			const auto& service_ptr = service_ptr2;



#endif

#ifndef REGISTER_EXT_MSG_HANDLE_END
#define REGISTER_EXT_MSG_HANDLE_END()\
		}
#endif


#ifndef REGISTER_MSG_HANDLE_BEGIN
#define REGISTER_MSG_HANDLE_BEGIN(ClassName, ptr)\
	    REGISTER_EXT_MSG_HANDLE_BEGIN(ClassName, ptr, this)

#endif

#ifndef REGISTER_MSG_HANDLE_END
#define REGISTER_MSG_HANDLE_END()\
		}
#endif


#ifndef REGISTER_DEFAULT_HANDLE
#define REGISTER_DEFAULT_HANDLE(func)\
service_ptr->register_default_msg_function(std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif

#ifndef REGISTER_DEFAULT_EVENT_HANDLE
#define REGISTER_DEFAULT_EVENT_HANDLE(func)\
service_ptr->register_default_event_function(std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2,  std::placeholders::_3));
#endif


#ifndef REGISTER_REQUEST_HANDLE
#define REGISTER_REQUEST_HANDLE(func)\
service_ptr->register_request_function(#func, std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif

#ifndef REGISTER_RESPONSE_HANDLE
#define REGISTER_RESPONSE_HANDLE(func)\
service_ptr->register_response_function(#func, std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif

#ifndef REGISTER_NOTIFY_HANDLE
#define REGISTER_NOTIFY_HANDLE(func)\
service_ptr->register_notify_function(#func, std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif

#ifndef REGISTER_EVENT_HANDLE
#define REGISTER_EVENT_HANDLE(func)\
service_ptr->register_event_function(#func, std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2,  std::placeholders::_3));
#endif

#ifndef REGISTER_REQUEST_HANDLE2
#define REGISTER_REQUEST_HANDLE2(name, func)\
service_ptr->register_request_function(#name, std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif

#ifndef REGISTER_RESPONSE_HANDLE2
#define REGISTER_RESPONSE_HANDLE2(name, func)\
service_ptr->register_response_function(#name, std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif

#ifndef REGISTER_NOTIFY_HANDLE2
#define REGISTER_NOTIFY_HANDLE2(name, func)\
service_ptr->register_notify_function(#name, std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2));
#endif

#ifndef REGISTER_EVENT_HANDLE2
#define REGISTER_EVENT_HANDLE2(name, func)\
service_ptr->register_event_function(#name, std::bind(&ClassType::func, ptr, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
#endif


namespace amo {
    typedef st_asio_wrapper::service_pump service_pump;
    
    namespace tcp {
    
    
        class message : public json_object {
        public:
            message() {
                timestamp = amo::timer::now();
                uid = amo::uid();
            }
            
            AMO_ENTITY_ARGS_GET_BEGIN(message)
            AMO_ENTITY_ARGS_GET(type)
            AMO_ENTITY_ARGS_GET(name)
            AMO_ENTITY_ARGS_GET(uid)
            AMO_ENTITY_ARGS_GET(name)
            AMO_ENTITY_ARGS_GET(timestamp)
            
            AMO_ENTITY_ARGS_GET(data)
            AMO_ENTITY_ARGS_GET_END()
            
            AMO_ENTITY_ARGS_SET_BEGIN(message)
            AMO_ENTITY_ARGS_SET(type)
            AMO_ENTITY_ARGS_SET(name)
            AMO_ENTITY_ARGS_SET(uid)
            AMO_ENTITY_ARGS_SET(name)
            AMO_ENTITY_ARGS_SET(timestamp)
            
            AMO_ENTITY_ARGS_SET(data)
            AMO_ENTITY_ARGS_SET_END()
        public:
            std::string type;		// 消息类型
            std::string name;		// 消息名称
            int64_t uid;			// 消息ID
            int64_t		timestamp;  // 消息时间
            
            std::string data;    // 消息内容
        };
        
        // 请求类消息
        class request : public message {
        public:
            request() {
                type = "request";
            }
            AMO_ENTITY_ARGS_GET_BEGIN(request)
            AMO_ENTITY_ARGS_GET_EXTEND(message)
            AMO_ENTITY_ARGS_GET_END()
            
            AMO_ENTITY_ARGS_SET_BEGIN(request)
            AMO_ENTITY_ARGS_SET_EXTEND(message)
            AMO_ENTITY_ARGS_SET_END()
        };
        
        // 响应类消息
        class response : public message {
        public:
            response() {
                type = "response";
                success = true;
            }
            
            AMO_ENTITY_ARGS_GET_BEGIN(response)
            AMO_ENTITY_ARGS_GET(success)
            AMO_ENTITY_ARGS_GET(nonreturn)
            AMO_ENTITY_ARGS_GET_EXTEND(message)
            AMO_ENTITY_ARGS_GET_END()
            
            AMO_ENTITY_ARGS_SET_BEGIN(response)
            AMO_ENTITY_ARGS_SET(success)
            AMO_ENTITY_ARGS_SET(nonreturn)
            AMO_ENTITY_ARGS_SET_EXTEND(message)
            AMO_ENTITY_ARGS_SET_END()
        public:
            bool success;	// 是否执行成功
            bool nonreturn; // 是否不返回结果
        };
        
        // 通知类消息
        class notify : public message {
        public:
            notify() {
                type = "notify";
            }
            
            AMO_ENTITY_ARGS_GET_BEGIN(notify)
            AMO_ENTITY_ARGS_GET_EXTEND(message)
            AMO_ENTITY_ARGS_GET_END()
            
            AMO_ENTITY_ARGS_SET_BEGIN(notify)
            AMO_ENTITY_ARGS_SET_EXTEND(message)
            AMO_ENTITY_ARGS_SET_END()
        };
        
        template<typename ServiceBase, typename BaseSocket, typename Packer, typename Unpacker>
        class service : public ServiceBase {
        public:
            typedef typename BaseSocket::out_msg_type
            out_msg_type;
            typedef typename BaseSocket::in_msg_type in_msg_type;
            typedef typename BaseSocket::socket_type socket_type;
            typedef typename
            object_pool< BaseSocket >::object_ctype object_type;
            typedef typename
            object_pool< BaseSocket >::object_ctype object_ctype;
            
            
            
            typedef std::function<bool(socket_type*, out_msg_type&)>
            msg_callback_type;
            typedef std::function<void(socket_type*, const std::string&, const boost::any&)>
            event_callback_type;
            
            typedef std::function<amo::any(socket_type*, const amo::json&)>
            normal_msg_handle_type;
            typedef std::function<amo::any(socket_type*, const std::string&)>
            default_msg_handle_type;
            typedef std::function<void(socket_type*,  amo::json&)>
            pre_msg_handle_type;
        public:
            service(service_pump& sp) : ServiceBase(sp)
                , m_service_pump(sp) {
                m_translate_msg = true;
                
            }
            
            ~service() {
            
            }
            
            /**
            * @fn	void service::start_service(int thread_num = ST_ASIO_SERVICE_THREAD_NUM)
            *
            * @brief	开启服务.
            *
            * @param	#int=8 线程数据.
            */
            
            void start_service(int thread_num = ST_ASIO_SERVICE_THREAD_NUM) {
                m_service_pump.start_service(thread_num);
            }
            
            /**
            * @fn	void service::run_service(int thread_num = ST_ASIO_SERVICE_THREAD_NUM)
            *
            * @brief	开启服务并阻塞线程，直到服务退出.
            *
            * @param	#int=8 线程数据.
            */
            
            void run_service(int thread_num = ST_ASIO_SERVICE_THREAD_NUM) {
                m_service_pump.run_service(thread_num);
            }
            
            /**
            * @fn	void service::stop_service()
            *
            * @brief	退出服务.
            */
            
            void stop_service() {
                m_service_pump.stop_service();
            }
            
            /**
            * @fn	bool service::is_running() const
            *
            * @brief	判断服务是否正在运行.
            *
            * @return	true if running, false if not.
            */
            
            bool is_running() const {
                return m_service_pump.is_running();
            }
            
            virtual bool on_msg(socket_type* ptr, out_msg_type& msg) {
                return false;
            }
            
            virtual bool default_msg_handle(socket_type* ptr, out_msg_type& msg) {
                if (m_fn_default_msg) {
                    return m_fn_default_msg(ptr, msg);
                } else {
                    return on_msg(ptr, msg);
                }
                
            }
            
            amo::any make_response_json(amo::json& json, const amo::any& val) {
                amo::any retval = true;
                
                switch (val.type()) {
                case amo::any_value_type<amo::json>::value:
                    json.join(val.As<amo::json>());
                    break;
                    
                case amo::any_value_type<amo::nonreturn>::value:
                    retval = amo::nonreturn();
                    break;
                    
                default:
                    retval =  add_any_to_json(json, "message", val);
                    break;
                }
                
                return retval;
            }
            
            
            amo::any add_any_to_json(amo::json& json, const std::string& key,
                                     const amo::any& val) {
                amo::any retval = true;
                
                switch (val.type()) {
                
                case amo::any_value_type<bool>::value:
                    json.put(key, val.As<bool>());
                    break;
                    
                case amo::any_value_type<char>::value:
                    json.put(key, val.As<char>());
                    break;
                    
                case amo::any_value_type<int>::value:
                    json.put(key, val.As<int>());
                    break;
                    
                case amo::any_value_type<int64_t>::value:
                    json.put(key, val.As<int64_t>());
                    break;
                    
                case amo::any_value_type<double>::value:
                    json.put(key, val.As<double>());
                    break;
                    
                case amo::any_value_type<std::string>::value:
                    json.put(key, val.As<std::string>());
                    break;
                    
                case amo::any_value_type<const char*>::value:
                    json.put(key, val.As<std::string>());
                    break;
                    
                case amo::any_value_type<amo::json>::value:
                    json.put(key, val.As<amo::json>());
                    break;
                    
                case amo::any_value_type<amo::nothing>::value:
                    break;
                    
                case amo::any_value_type<amo::nonreturn>::value:
                    retval = amo::nonreturn();
                    break;
                    
                case amo::any_value_type<amo::nil>::value:
                
                case amo::any_value_type<amo::undefined>::value:
                
                
                default:
                    json.put(key, val.value());
                    break;
                }
                
                return retval;
            }
            
            virtual bool request_msg_handle(socket_type* ptr, const amo::json& msg) {
            
                std::string name = msg.getString("name");
                
                auto iter = m_request_map.find(name);
                
                if (iter != m_request_map.end()) {
                    amo::any val = iter->second(ptr, msg);
                    
                    amo::json retval;
                    retval.put("type", "response");
                    retval.put("name", name);
                    retval.put("uid", msg.getString("uid"));
                    retval.put("success", true);
                    retval.put("message", "");
                    retval.put("timestamp", amo::timer::now());
                    
                    amo::any val1 = make_response_json(retval, val);
                    
                    if (val1.is<amo::nonreturn>()) {
                        return true;
                    }
                    
                    ptr->send_msg(retval.to_string());
                    return true;
                    
                } else {
                    // 没有注册函数处理该消息，那么将消息转给默认处理函数
                    std::string val = msg.to_string();
                    return default_msg_handle(ptr, val);
                }
            }
            
            virtual bool response_msg_handle(socket_type* ptr, const amo::json& msg) {
            
                std::string name = msg.getString("name");
                
                auto iter = m_response_map.find(name);
                
                if (iter != m_response_map.end()) {
                    amo::any val = iter->second(ptr, msg);
                    return true;
                    
                } else {
                    // 没有注册函数处理该消息，那么将消息转给默认处理函数
                    std::string val = msg.to_string();
                    return default_msg_handle(ptr, val);
                }
            }
            
            virtual bool notify_msg_handle(socket_type* ptr, const amo::json& msg) {
                std::string name = msg.getString("name");
                
                auto iter = m_notify_map.find(name);
                
                if (iter != m_notify_map.end()) {
                    amo::any val = iter->second(ptr, msg);
                    return true;
                    
                } else {
                    // 没有注册函数处理该消息，那么将消息转给默认处理函数
                    std::string val = msg.to_string();
                    return default_msg_handle(ptr, val);
                }
            }
            
            virtual bool on_recv_msg(socket_type* ptr, out_msg_type& msg) {
            
                // 消息处理流程
                // 1. 转换为json，如果是json，那么查找注册回调，如果有，则调用之，如果没有，则转给默认处理函数
                // 2. 如果不是json,那么转给默认处理函数
                try {
                
                    if (!m_translate_msg) {
                        return default_msg_handle(ptr, msg);
                    }
                    
                    amo::json json(msg);
                    
                    if (!json.is_valid()) {
                        return default_msg_handle(ptr, msg);
                    }
                    
                    if (m_fn_pre_msg_handle) {
                        m_fn_pre_msg_handle(ptr, json);
                    }
                    
                    std::string type = json.getString("type");
                    json.put("objectID", ptr->id());
                    
                    if (type == "request") {
                        request_msg_handle(ptr, json);
                        return true;
                        
                    } else if (type == "response") {
                        return response_msg_handle(ptr, json);
                    } else if (type == "notify") {
                        return notify_msg_handle(ptr, json);
                    } else {
                        return default_msg_handle(ptr, msg);
                    }
                    
                } catch (const std::exception& e) {
                
                    std::cout << e.what() << std::endl;
                }
                
                //std::cout << "recv msg :" << msg << std::endl;
                //ptr->send_msg(amo::date_time().to_string());
                return true;
            }
            
            virtual void on_event_msg(socket_type* ptr, const std::string& name,
                                      const boost::any& val) {
                                      
                try {
                    auto iter = m_event_map.find(name);
                    
                    if (iter != m_event_map.end()) {
                        iter->second(ptr, name, val);
                    } else if (m_fn_default_event_callback) {
                        m_fn_default_event_callback(ptr, name, val);
                    } else {
                        std::cout << "trigger event :" << name << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "trigger event exception :"
                              << name << "\t "
                              << e.what() << std::endl;
                }
                
            }
            
            
            /**
            * @fn	object_type service::find_socket(std::function < bool(object_type& > fn))
            *
            * @brief	自定义socket查找函数.
            *
            * @param  	#function 查找函数.
            *
            * @return	#socket 查找到的socket, 如果没有查找返回empty.
            */
            
            object_type find_socket(std::function<bool(object_type)> fn) {
                object_type retval;
                uint64_t id = -1;
                this->do_something_to_all([&](object_type & item) {
                    if (fn(item)) {
                        id = item->id();
                        return true;
                    }
                    
                    return false;
                });
                
                return this->find(id);
            }
            
            
        public:
        
            event_callback_type get_default_event_function()  const {
                return m_fn_default_event_callback;
            }
            
            void register_default_event_function(event_callback_type val) {
                m_fn_default_event_callback = val;
            }
            
            // 函数注册
            void register_request_function(const std::string& name,
                                           normal_msg_handle_type fn) {
                m_request_map.insert({ name, fn });
            }
            
            void register_response_function(const std::string& name,
                                            normal_msg_handle_type fn) {
                m_response_map.insert({ name, fn });
            }
            
            void register_notify_function(const std::string& name,
                                          normal_msg_handle_type fn) {
                m_notify_map.insert({ name, fn });
            }
            
            void register_default_msg_function(default_msg_handle_type
                                               fn) {
                m_fn_default_msg = fn;
            }
            
            void register_event_function(const std::string& name,
                                         event_callback_type fn) {
                m_event_map.insert({ name, fn });
            }
            
            void set_translate_msg(bool msg) {
                m_translate_msg = msg;
            }
            
            void is_translate_msg() const {
                return m_translate_msg;
            }
            
            void set_pre_msg_handle(pre_msg_handle_type fn) {
                m_fn_pre_msg_handle = fn;
            }
            
            pre_msg_handle_type get_pre_msg_handle() const {
                return m_fn_pre_msg_handle;
            }
            
        protected:
        
            void recordFailedCommand(amo::json& json) {
                if (!json.contains_key("success")) {
                    return;
                }
                
                // 如果命令执行失败，将此条命令记录下来
                if (!json.getBool("success")) {
                    $cerr(json.getString("message"));
                }
            }
            
        protected:
        
        
            // 将转换为JSON的数据进行预处理，如添加字段，修改字段
            pre_msg_handle_type m_fn_pre_msg_handle;
            
            default_msg_handle_type m_fn_default_msg;
            event_callback_type m_fn_default_event_callback;
            
            std::unordered_map < std::string, normal_msg_handle_type >  m_request_map;
            std::unordered_map < std::string, normal_msg_handle_type >  m_response_map;
            std::unordered_map < std::string, normal_msg_handle_type >  m_notify_map;
            
            std::unordered_map < std::string, event_callback_type >  m_event_map;
            std::unordered_map<std::string, int64_t> m_objects;
            
            service_pump& m_service_pump;
            
            // 是否尝试将数据转换为JSON格式，控制该开关可以选择是否将该消息转换为JSON，默认为true;
            bool m_translate_msg;
            
        };
        
        template<typename T>
        class service_traits {
        public:
            typedef T service_traits_type;
            
        public:
            /**
            * @fn	void service::start_service(int thread_num = ST_ASIO_SERVICE_THREAD_NUM)
            *
            * @brief	开启服务.
            *
            * @param	#int=8 线程数据.
            */
            
            void start_service(int thread_num = ST_ASIO_SERVICE_THREAD_NUM) {
                m_service->start_service(thread_num);
            }
            
            /**
            * @fn	void service::run_service(int thread_num = ST_ASIO_SERVICE_THREAD_NUM)
            *
            * @brief	开启服务并阻塞线程，直到服务退出.
            *
            * @param	#int=8 线程数据.
            */
            
            void run_service(int thread_num = ST_ASIO_SERVICE_THREAD_NUM) {
                m_service->run_service(thread_num);
            }
            
            /**
            * @fn	void service::stop_service()
            *
            * @brief	退出服务.
            */
            
            void stop_service() {
                m_service->stop_service();
            }
            
            /**
            * @fn	bool service::is_running() const
            *
            * @brief	判断服务是否正在运行.
            *
            * @return	true if running, false if not.
            */
            
            bool is_running() const {
                return m_service->is_running();
            }
            
        public:
            amo::service_pump sp;
            std::shared_ptr<service_traits_type > m_service;
        };
    }
}



#endif //AMO_SERVICE_84826FAF_0CA1_4F48_BF0A_5A32CD9F5D79_HPP__