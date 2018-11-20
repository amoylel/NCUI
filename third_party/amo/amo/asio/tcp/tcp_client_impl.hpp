// Created by amoylel on 13/06/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_TCP_CLIENT_IMPL_63D52B3F_6E63_40B8_9DAE_56BC0B691BE3_HPP__
#define AMO_TCP_CLIENT_IMPL_63D52B3F_6E63_40B8_9DAE_56BC0B691BE3_HPP__







#include <memory>

#include <unordered_map>
#include <amo/json.hpp>
#include <amo/asio/tcp/client_socket.hpp>
#include <amo/looper_executor.hpp>
#include <functional>
#include <future>

#define REGISTER_REQUEST_FUNCTION(CLASS, func, obj)\
registerRequestFunction(#func, std::bind(&##CLASS##::##func, obj, std::placeholders::_1));

#define REGISTER_REQUEST_SCANNERMGR(func)\
registerRequestFunction(#func, std::bind(&ScannerMgr::##func, m_scannerMgr, std::placeholders::_1));

#define REGISTER_NOTIFY_SCANNERMGR(func)\
registerNotifyFunction(#func, std::bind(&ScannerMgr::##func, m_scannerMgr, std::placeholders::_1));


#define REGISTER_REQUEST_APP(func)\
registerRequestFunction(#func, std::bind(&AppMgr::##func, m_appMgr, std::placeholders::_1));

#define REGISTER_NOTIFY_APP(func)\
registerNotifyFunction(#func, std::bind(&AppMgr::##func, m_appMgr, std::placeholders::_1));

namespace amo {


    namespace tcp {
        template<typename Packer, typename Unpacker>
        class client_impl : public
            single_client_base<client_socket<Packer, Unpacker> > {
        public:
        
        public:
        
            client_impl(service_pump& sp) : single_client_base(sp)
                , m_service_pump(sp) {
                
                m_executor.reset(new amo::looper_executor());
                
            }
            
            ~client_impl() {
            
            }
            
            void register_request_function(const std::string& name,
                                           std::function<amo::json(amo::json&)> fn) {
                m_request_map.insert({ name, fn });
            }
            
            void register_response_function(const std::string& name,
                                            std::function<amo::json(amo::json&)> fn) {
                m_response_map.insert({ name, fn });
            }
            
            void register_notify_function(const std::string& name,
                                          std::function<amo::json(amo::json&)> fn) {
                m_notify_map.insert({ name, fn });
            }
            //
            //bool on_msg(out_msg_type& msg) {
            //    amo::string str(msg, true);
            //    amo::json json(str.str());
            //    //std::cout << std::this_thread::get_id() << std::endl;
            //
            //
            //
            //    if (!json.is_valid() || !json.is_object()) {
            //        return false;
            //    }
            //
            //    if (json.contains_key("request")) {
            //        std::string command = json.getString("request");
            //
            //        auto iter = m_request_map.find(command);
            //        std::string UID = json.getString("UID");
            //
            //
            //        amo::json retval;
            //        retval.put("response", command);
            //        retval.put("UID", json.getString("UID"));
            //        retval.put("success", true);
            //        retval.put("message", "");
            //
            //        if (iter != m_request_map.end()) {
            //            retval.join(iter->second(json));
            //        } else {
            //            retval.put("success", false);
            //            retval.put("message", "无效请求");
            //        }
            //
            //        if (retval.contains_key("nonreturn")
            //                && retval.is_bool("nonreturn")
            //                && retval.getBool("nonreturn")) {
            //            // 如果要求不返回结果，那么直接返回。
            //            // 这种通常是要在其他线程中执行任务，并不能马上得到结果
            //            return false;
            //        }
            //
            //        recordFailedCommand(retval);
            //
            //        retval = retval.to_utf8();
            //
            //        amo::json data = retval.getJson("data");
            //
            //        if (data.is_valid()) {
            //            retval.put("data", data.to_string());
            //        }
            //
            //        this->boardcastMsg(retval.to_string());
            //
            //    } else if (json.contains_key("response")) {
            //        std::string command = json.getString("response");
            //
            //        auto iter = m_response_map.find(command);
            //
            //        if (iter != m_response_map.end()) {
            //            iter->second(json);
            //        }
            //
            //    } else if (json.contains_key("notify")) {
            //        std::string command = json.getString("notify");
            //        auto iter = m_notify_map.find(command);
            //
            //        if (iter != m_notify_map.end()) {
            //            iter->second(json);
            //        }
            //    }
            //
            //    return false;
            //}
            
            
            
        protected:
            // 有socket连接时触发
            
            /* virtual bool on_accept(typename object_pool< client_socket<Packer, Unpacker> >::object_ctype& socket_ptr) override {
            	 socket_ptr->set_on_message_callback(std::bind(&client_impl::on_msg, this,
            										 std::placeholders::_1));
            	 return server_base::on_accept(socket_ptr);
             }*/
            
        protected:
        
            void recordFailedCommand(amo::json& json) {
                if (!json.contains_key("success")) {
                    return;
                }
                
                // 如果命令执行失败，将此条命令记录下来
                if (!json.getBool("success")) {
                    $clog(amo::cerr << json.getString("message") << amo::endl;);
                }
            }
            
            
        protected:
        
            void boardcastMsg(const std::string& msg) {
                this->broadcast_msg(msg);
            }
            
            
        private:
            std::shared_ptr<amo::looper_executor> m_executor;
            
            std::unordered_map < std::string, std::function<amo::json(amo::json&)> >
            m_request_map;
            std::unordered_map < std::string, std::function<amo::json(amo::json&)> >
            m_response_map;
            std::unordered_map < std::string, std::function<amo::json(amo::json&)> >
            m_notify_map;
            
            
            service_pump& m_service_pump;
        };
    }
    
}


#endif //AMO_TCP_CLIENT_IMPL_63D52B3F_6E63_40B8_9DAE_56BC0B691BE3_HPP__
