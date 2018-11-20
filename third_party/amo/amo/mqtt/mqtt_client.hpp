// Created by amoylel on 15/10/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_MQTT_CLIENT_B70AC238_C3FA_42B1_A6A4_0B305F6AABCB_HPP__
#define AMO_MQTT_CLIENT_B70AC238_C3FA_42B1_A6A4_0B305F6AABCB_HPP__

#include <string.h>
#include <string>
#include <set>
#include <mutex>
#include <iostream>

#include <mosquittopp.h>
#include <amo/thread.hpp>

#include <amo/logger.hpp>

namespace amo {

    class mqtt_client : public mosqpp::mosquittopp {
    public:
        typedef  std::function<void(int)> connect_callback_type;
        typedef  std::function<void(int, int)> connect_with_flags_callback_type;
        typedef  std::function<void(int)> disconnect_callback_type;
        typedef  std::function<void(int)> publish_callback_type;
        typedef  std::function<void(int, const char*)> log_callback_type;
        typedef  std::function<void()> error_callback_type;
        
    public:
    
        mqtt_client(const std::string id = "",
                    bool clean_session = true)
            : mosquittopp(id.c_str(), clean_session) {
            m_qos = 1;
            mosqpp::lib_init();
            loop_start();
        }
        
        
        ~mqtt_client() {
            // 必须先disconnect, 否则 loop_stop不会返回
            disconnect();
            loop_stop();
            mosqpp::lib_cleanup();
        }
        
        bool send_message(const std::string & message) {
            return send_message(get_topic(), message);
        }
        bool send_message(const char*  message, int len) {
            return send_message(get_topic(), message, len);
        }
        
        bool send_message(const std::string& topic, const std::string & message) {
            return send_message(topic, message.c_str(), message.size());
        }
        
        bool send_message(const std::string& topic, const char*  message, int len) {
            int mid = 0;
            int ret = publish(&mid, topic.c_str(), len, message, get_qos(), false);
            
            if (ret == MOSQ_ERR_SUCCESS) {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                // 要不要记录 mid  message id
                return true;
            } else {
                return false;
            }
        }
        
        int get_qos() const {
            return m_qos;
        }
        void set_qos(int val) {
            m_qos = val;
        }
        
        const std::string get_topic() const {
            return m_topic;
        }
        
        void set_topic(std::string val) {
            m_topic = val;
        }
        
        bool set_ssl(const std::string& cafile,
                     const std::string& certfile = "",
                     const std::string& keyfile = "",
                     int(*pw_callback)(char *buf, int size,
                                       int rwflag, void *userdata) = NULL) {
                                       
            return tls_set(cafile.c_str(),
                           NULL,
                           certfile.c_str(),
                           keyfile.c_str(),
                           pw_callback) == MOSQ_ERR_SUCCESS;
        }
        
        bool set_insecure(bool value) {
            return tls_insecure_set(value) == MOSQ_ERR_SUCCESS;
        }
        
        
        // callback func
        
        void register_connect_callback(connect_callback_type fn) {
            m_fn_on_connect = fn;
        }
        
        void register_connect_with_flags_callback(connect_with_flags_callback_type fn) {
            m_fn_on_connect_with_flags = fn;
        }
        void register_disconnect_callback(disconnect_callback_type fn) {
            m_fn_on_disconnect = fn;
        }
        
        void register_publish_callback(publish_callback_type fn) {
            m_fn_on_publish = fn;
        }
        
        void register_log_callback(log_callback_type fn) {
            m_fn_on_log = fn;
        }
        
        void register_on_connect_callback(error_callback_type fn) {
            m_fn_on_error = fn;
        }
        
        // callback funcs
        virtual void on_connect(int rc) override {
            /*   std::cout << "on_connect " << " [" << amo::current_thread_id() << "] " << rc <<
                         std::endl;*/
            $cdevel("connect code: {}", rc);
            
            if (m_fn_on_connect) {
                return m_fn_on_connect(rc);
            }
            
            return;
        }
        
        
        virtual void on_connect_with_flags(int rc, int flags) override {
            std::cout << "on_connect_with_flags " << " [" << amo::current_thread_id() <<
                      "] " << rc << std::endl;
                      
            $cdevel("connect code: {}， connect flag: {}", rc, flags);
            
            if (m_fn_on_connect_with_flags) {
                return m_fn_on_connect_with_flags(rc, flags);
            }
            
            return;
        }
        
        
        virtual void on_disconnect(int rc) override {
        
            $cdevel("disconnect code: {}", rc);
            
            /* std::cout << "on_disconnect " << " [" << amo::current_thread_id() << "] " << rc
                       << std::endl;
                       */
            if (m_fn_on_disconnect) {
                return m_fn_on_disconnect(rc);
            }
            
            return;
        }
        
        
        virtual void on_publish(int mid) override {
            /*std::cout << "on_publish " << " [" << amo::current_thread_id() << "] " << mid <<
                      std::endl;*/
            $cdevel("publish success: {}", mid);
            
            if (m_fn_on_publish) {
                return m_fn_on_publish(mid);
            }
            
            return;
        }
        
        
        
        virtual void on_log(int level, const char * str) override {
        
            $cdevel("[{}]{}", amo::current_thread_id(), str);
            /*std::cout << "on_log " << " [" << amo::current_thread_id() << "] " << level <<
                      " " << str << std::endl;*/
            
            if (m_fn_on_log) {
                return m_fn_on_log(level, str);
            }
            
            return;
        }
        
        
        virtual void on_error() override {
            $cerr("[{}]", amo::current_thread_id());
            /*std::cout << "on_error" << " [" << amo::current_thread_id() << "] " <<
                      std::endl;*/
            
            if (m_fn_on_error) {
                return m_fn_on_error();
            }
            
            return;
        }
        
    protected:
    
        std::recursive_mutex m_mutex;
        std::string m_topic;
        int m_qos;
        
        connect_callback_type m_fn_on_connect;
        connect_with_flags_callback_type m_fn_on_connect_with_flags;
        disconnect_callback_type m_fn_on_disconnect;
        publish_callback_type m_fn_on_publish;
        log_callback_type m_fn_on_log;
        error_callback_type m_fn_on_error;
        
    };
}


#endif //AMO_MQTT_CLIENT_B70AC238_C3FA_42B1_A6A4_0B305F6AABCB_HPP__


