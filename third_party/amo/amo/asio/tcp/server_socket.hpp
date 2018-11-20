// Created by amoylel on 13/06/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_SERVER_SOCKET_D2BF681E_D3A9_4084_B60F_3DC871B87AE1_HPP__
#define AMO_SERVER_SOCKET_D2BF681E_D3A9_4084_B60F_3DC871B87AE1_HPP__






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


#include <memory>

#include <amo/asio/tcp/packer.hpp>
#include <amo/asio/tcp/unpacker.hpp>
#include <amo/asio/st_asio_wrapper/ext/tcp.h>


#include <amo/asio/tcp/server_socket_proxy.hpp>

using namespace st_asio_wrapper;
using namespace st_asio_wrapper::tcp;
using namespace st_asio_wrapper::ext;
using namespace st_asio_wrapper::ext::tcp;



typedef server_socket_base<amo::tcp::packer, amo::tcp::unpacker>
normal_socket_base;
namespace amo {

    template<typename Packer, typename Unpacker>
    class server_socket : public server_socket_base<Packer, Unpacker>
        , public std::enable_shared_from_this<server_socket<Packer, Unpacker> > {
    public:
    
        server_socket(i_server& server_) : normal_socket_base(server_) {
        
        }
        
    protected:
    
        virtual bool do_start() {
        
#if ST_ASIO_HEARTBEAT_INTERVAL > 0
            start_heartbeat(ST_ASIO_HEARTBEAT_INTERVAL);
#endif
            return normal_socket_base::do_start();
        }
        
        /*!
         * @fn	boost::asio::ip::tcp::endpoint server_socket::remote_endpoint() const
         *
         * @brief	获取当前socket远端IP信息.
         *
         * @return	#endpoint;
         */
        
        boost::asio::ip::tcp::endpoint remote_endpoint() const {
            return this->lowest_layer().remote_endpoint();
        }
        
        /*!
         * @fn	boost::asio::ip::tcp::endpoint server_socket::local_endpoint() const
         *
         * @brief	获取当前socket本地IP信息.
         *
         * @return	#endpoint;
         */
        
        boost::asio::ip::tcp::endpoint local_endpoint() const {
            return this->lowest_layer().local_endpoint();
        }
        
        
        /*!
         * @fn	virtual bool server_socket::on_msg(out_msg_type& msg) override
         *
         * @brief	收到消息时触发.
         *
         * @param	#std::string	收到的消息.
         *
         * @return	#bool true表示消息已处理，false表示消息未处理.
         */
        
        virtual bool on_msg(out_msg_type& msg) override {
        
            if (m_proxy.lock()) {
                m_proxy.lock()->on_msg(msg);
            }
            
            if (m_fnOnMessageCallback) {
                boost::system::error_code ec;
                
                auto ep = this->remote_endpoint();
                
                if (!ec) {
                    if (msg.find("{\"notify\":\"gps\"") != 0) {
                        unified_out::debug_out("[%s" ":%d] " "recv("  ST_ASIO_SF "): %s",
                                               ep.address().to_string().c_str(), ep.port(), msg.size(), msg.data());
                    }
                }
                
                m_fnOnMessageCallback(this, msg);
                
                return true;
            }
            
            return server_socket_base::on_msg(msg);
        }
        
        
        /*!
         * @fn	virtual bool server_socket::on_msg_handle(out_msg_type& msg) override
         *
         * @brief	收到消息时触发.
         *
         * @param	#std::string	收到的消息.
         *
         * @return	#bool true表示消息已处理，false表示消息未处理.
         */
        
        virtual bool on_msg_handle(out_msg_type& msg) override {
        
            if (m_proxy.lock()) {
                m_proxy.lock()->on_msg(msg);
            }
            
            if (m_fnOnMessageCallback) {
                //unified_out::debug_out("recv(" ST_ASIO_SF "): %s", msg.size(), msg.data());
                
                boost::system::error_code ec;
                auto ep = this->lowest_layer().remote_endpoint(ec);
                
                if (!ec) {
                    unified_out::debug_out("[%s" ":%d] ""recv("  ST_ASIO_SF "): %s",
                                           ep.address().to_string().c_str(), ep.port(), msg.size(), msg.data());
                }
                
                m_fnOnMessageCallback(this, msg);
                return true;
            }
            
            return server_socket_base::on_msg(msg);
        }
        
        
        virtual bool do_send_msg(in_msg_type& msg) override {
            return server_socket_base::do_send_msg(msg);
        }
        
        virtual bool do_send_msg() override {
            return server_socket_base::do_send_msg();
        }
        
        virtual void do_recv_msg() override {
            return server_socket_base::do_recv_msg();
        }
        
        /*!
         * @fn	virtual void server_socket::on_connect() override
         *
         * @brief	当socket 连接时触发.
         */
        
        virtual void on_connect() override {
            return server_socket_base::on_connect();
        }
        
        /*!
         * @fn	virtual void server_socket::on_unpack_error() override
         *
         * @brief	数据解包失败时触发.
         */
        
        virtual void on_unpack_error() override {
            return server_socket_base::on_unpack_error();
        }
        
        /*!
         * @fn	virtual void server_socket::on_recv_error(const boost::system::error_code& ec) override
         *
         * @brief	数据打包失败时触发.
         *
         * @param	错误代码.
         */
        
        virtual void on_recv_error(const boost::system::error_code& ec) override {
            return server_socket_base::on_recv_error(ec);
        }
        
        /*!
         * @fn	virtual void server_socket::on_async_shutdown_error() override
         *
         * @brief	异步关闭socket失败时触发.
         */
        
        virtual void on_async_shutdown_error() override {
            return server_socket_base::on_async_shutdown_error();
        }
        
        /*!
         * @fn	virtual bool server_socket::on_heartbeat_error() override
         *
         * @brief	发送心跳包失败时触发.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool on_heartbeat_error() override {
            return server_socket_base::on_heartbeat_error();
        }
        
    public:
        std::function<bool(server_socket*, out_msg_type &)> get_on_message_callback()
        const {
            return m_fnOnMessageCallback;
        }
        
        void set_on_message_callback(std::function<bool(server_socket*, out_msg_type &)>
                                     val) {
            m_fnOnMessageCallback = val;
        }
        
        std::weak_ptr<amo::tcp::server_socket_proxy<Packer, Unpacker> >
        get_socket_proxy() const {
            return m_proxy;
        }
        void set_socket_proxy(
            std::weak_ptr<amo::tcp::server_socket_proxy<Packer, Unpacker> > val) {
            m_proxy = val;
        }
        
    protected:
        std::function<bool(server_socket*, out_msg_type&)> m_fnOnMessageCallback;
        
        std::weak_ptr<amo::tcp::server_socket_proxy<Packer, Unpacker> > m_proxy;
        
        
        
    };
    
}


#endif //AMO_SERVER_SOCKET_D2BF681E_D3A9_4084_B60F_3DC871B87AE1_HPP__

