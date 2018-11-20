// Created by amoylel on 06/09/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_CLIENT_SOCKET_63DEE874_EDFB_4063_A818_BA44CA9D0782_HPP__
#define AMO_CLIENT_SOCKET_63DEE874_EDFB_4063_A818_BA44CA9D0782_HPP__




#include <iostream>



#define ST_ASIO_REUSE_OBJECT //use objects pool
//#define ST_ASIO_FREE_OBJECT_INTERVAL 60 //it's useless if ST_ASIO_REUSE_OBJECT macro been defined
//#define ST_ASIO_FORCE_TO_USE_MSG_RECV_BUFFER //force to use the msg recv buffer
#define ST_ASIO_ENHANCED_STABILITY
#define ST_ASIO_FULL_STATISTIC //full statistic will slightly impact efficiency
//#define ST_ASIO_USE_STEADY_TIMER
//#define ST_ASIO_USE_SYSTEM_TIMER
#define ST_ASIO_AVOID_AUTO_STOP_SERVICE
#define ST_ASIO_DECREASE_THREAD_AT_RUNTIME




//#ifndef  ST_ASIO_HEARTBEAT_INTERVAL
//#define ST_ASIO_HEARTBEAT_INTERVAL 5
//#endif


#include <amo/asio/tcp/packer.hpp>
#include <amo/asio/tcp/unpacker.hpp>
#include <amo/asio/st_asio_wrapper/ext/tcp.h>
using namespace st_asio_wrapper;
using namespace st_asio_wrapper::tcp;
using namespace st_asio_wrapper::ext;
using namespace st_asio_wrapper::ext::tcp;



namespace amo {

    template<typename Packer, typename Unpacker>
    class client_socket : public client_socket_base<Packer, Unpacker> {
    public:
        client_socket(boost::asio::io_context & io_context_): client_socket_base(io_context_) {
        
        }
        
    protected:
    
        //        virtual bool do_start() {
        //
        //#if ST_ASIO_HEARTBEAT_INTERVAL > 0
        //            start_heartbeat(ST_ASIO_HEARTBEAT_INTERVAL);
        //#endif
        //            return normal_socket_base::do_start();
        //        }
        //
        //        virtual bool on_msg(out_msg_type& msg) override {
        //            if (m_fnOnMessageCallback) {
        //                boost::system::error_code ec;
        //
        //                auto ep = this->lowest_layer().remote_endpoint(ec);
        //
        //                if (!ec) {
        //                    if (msg.find("{\"notify\":\"gps\"") != 0) {
        //                        unified_out::debug_out("[%s" ":%d] " "recv("  ST_ASIO_SF "): %s",
        //                                               ep.address().to_string().c_str(), ep.port(), msg.size(), msg.data());
        //                    }
        //                }
        //
        //                m_fnOnMessageCallback(msg);
        //
        //                return true;
        //            }
        //
        //            return client_socket_base::on_msg(msg);
        //        }
        //
        //        virtual bool on_msg_handle(out_msg_type& msg) override {
        //            if (m_fnOnMessageCallback) {
        //                //unified_out::debug_out("recv(" ST_ASIO_SF "): %s", msg.size(), msg.data());
        //
        //                boost::system::error_code ec;
        //                auto ep = this->lowest_layer().remote_endpoint(ec);
        //
        //                if (!ec) {
        //                    unified_out::debug_out("[%s" ":%d] ""recv("  ST_ASIO_SF "): %s",
        //                                           ep.address().to_string().c_str(), ep.port(), msg.size(), msg.data());
        //                }
        //
        //                m_fnOnMessageCallback(msg);
        //                return true;
        //            }
        //
        //            return client_socket_base::on_msg(msg);
        //        }
        //
        //
        //        virtual bool do_send_msg(in_msg_type& msg) override {
        //            return client_socket_base::do_send_msg(msg);
        //        }
        //
        //        virtual bool do_send_msg() override {
        //            return client_socket_base::do_send_msg();
        //        }
        //
        //        virtual void do_recv_msg() override {
        //            return client_socket_base::do_recv_msg();
        //        }
        //
        //        virtual void on_connect() override {
        //            return client_socket_base::on_connect();
        //        }
        //
        //        virtual void on_unpack_error() override {
        //            return client_socket_base::on_unpack_error();
        //        }
        //
        //        virtual void on_recv_error(const boost::system::error_code& ec) override {
        //            return client_socket_base::on_recv_error(ec);
        //        }
        //
        //        virtual void on_async_shutdown_error() override {
        //            return client_socket_base::on_async_shutdown_error();
        //        }
        //
        //        virtual bool on_heartbeat_error() override {
        //            return client_socket_base::on_heartbeat_error();
        //        }
        
    public:
        std::function<bool(out_msg_type &)> get_on_message_callback() const {
            return m_fnOnMessageCallback;
        }
        
        void set_on_message_callback(std::function<bool(out_msg_type &)> val) {
            m_fnOnMessageCallback = val;
        }
        
    protected:
        std::function<bool(out_msg_type&)> m_fnOnMessageCallback;
        
    };
    
}

#endif // AMO_CLIENT_SOCKET_63DEE874_EDFB_4063_A818_BA44CA9D0782_HPP__


