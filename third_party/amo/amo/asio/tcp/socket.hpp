// Created by amoylel on 25/06/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_SOCKET_AC0DCC52_C0BA_462C_81F3_399EC6E771C2_HPP__
#define AMO_SOCKET_AC0DCC52_C0BA_462C_81F3_399EC6E771C2_HPP__


#include <amo/asio/config.hpp>
#include <amo/asio/utility/unified_out.hpp>

#include <iostream>
#include <amo/asio/tcp/packer.hpp>
#include <amo/asio/tcp/unpacker.hpp>



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


#include <boost/any.hpp>

#include <amo/nil.hpp>



namespace amo {
    namespace tcp {
    
    
        template<typename BaseSocket>
        class socket : public BaseSocket {
        public:
            typedef std::function<bool(socket*, typename BaseSocket::out_msg_type&)>
            msg_callback_type;
            typedef std::function<void(socket*, const std::string&, const boost::any&)>
            event_callback_type;
            
            
        public:
            socket(boost::asio::io_context & io_context_) : BaseSocket(
                    io_context_) {
                    
            }
            socket(st_asio_wrapper::tcp::i_server& server_) : BaseSocket(server_) {
            
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
            
            /**
             * @fn	std::string socket::local_ip() const
             *
             * @brief	获取Socket的本地IP地址.
             *
             * @return	#std::string.
             */
            
            std::string local_ip() const {
                return local_endpoint().address().to_string();
            }
            
            /**
             * @fn	std::string socket::remote_ip() const
             *
             * @brief	获取Socket的远端IP地址.
             *
             * @return	#std::string.
             */
            
            std::string remote_ip() const {
                return remote_endpoint().address().to_string();
            }
            
            /**
             * @fn	unsigned short socket::local_port() const
             *
             * @brief	获取Socket的本地端口号.
             *
             * @return	#short.
             */
            
            unsigned short local_port() const {
                return local_endpoint().port();
            }
            
            /**
             * @fn	unsigned short socket::remote_port() const
             *
             * @brief	获取Socket的远端端口号.
             *
             * @return	#short.
             */
            
            unsigned short remote_port() const {
                return remote_endpoint().port();
            }
            
            /**
             * @fn	std::string socket::local_address() const
             *
             * @brief	获取Socket的本地IP:Port.
             *
             * @return	#std::string.
             */
            
            std::string local_address() const {
                return local_ip() + ":" + std::to_string(local_port());
            }
            
            /**
             * @fn	std::string socket::remote_address() const
             *
             * @brief	获取Socket的远端IP:Port.
             *
             * @return	#std::string.
             */
            
            std::string remote_address() const {
                return remote_ip() + ":" + std::to_string(remote_port());
            }
        protected:
        
            virtual bool do_start() {
            
#if ST_ASIO_HEARTBEAT_INTERVAL > 0
                start_heartbeat(ST_ASIO_HEARTBEAT_INTERVAL);
#endif
                return BaseSocket::do_start();
            }
            
            virtual bool on_msg(typename BaseSocket::out_msg_type& msg) override {
                if (m_fn_msg_callback) {
                    boost::system::error_code ec;
                    
                    /* auto ep = this->lowest_layer().remote_endpoint(ec);
                    
                     if (!ec) {
                         unified_out::debug_out("[%s" ":%d] ""recv("  ST_ASIO_SF "): %s",
                                                ep.address().to_string().c_str(), ep.port(), msg.size(), msg.data());
                     }
                     */
                    m_fn_msg_callback(this, msg);
                    
                    return true;
                }
                
                return BaseSocket::on_msg(msg);
            }
            
            virtual bool on_msg_handle(typename BaseSocket::out_msg_type& msg) override {
                if (m_fn_msg_callback) {
                    //unified_out::debug_out("recv(" ST_ASIO_SF "): %s", msg.size(), msg.data());
                    
                    /*  boost::system::error_code ec;
                      auto ep = this->lowest_layer().remote_endpoint(ec);
                    
                      if (!ec) {
                          unified_out::debug_out("[%s" ":%d] ""recv("  ST_ASIO_SF "): %s",
                                                 ep.address().to_string().c_str(), ep.port(), msg.size(), msg.data());
                      }*/
                    
                    m_fn_msg_callback(this, msg);
                    return true;
                }
                
                return BaseSocket::on_msg(msg);
            }
            
            
            virtual bool do_send_msg(typename BaseSocket::in_msg_type& msg) override {
                return BaseSocket::do_send_msg(msg);
            }
            
            virtual bool do_send_msg() override {
                return BaseSocket::do_send_msg();
            }
            
            virtual void do_recv_msg() override {
                return BaseSocket::do_recv_msg();
            }
            
            virtual void on_connect() override {
                if (m_fn_event_callback) {
                    m_fn_event_callback(this, "connect", amo::nil());
                }
                
                return BaseSocket::on_connect();
            }
            
            virtual void on_unpack_error() override {
                if (m_fn_event_callback) {
                    m_fn_event_callback(this, "unpack_error", amo::nil());
                }
                
                return BaseSocket::on_unpack_error();
            }
            
            virtual void on_recv_error(const boost::system::error_code& ec) override {
                if (m_fn_event_callback) {
                    m_fn_event_callback(this, "recv_error", ec);
                }
                
                return BaseSocket::on_recv_error(ec);
            }
            
            virtual void on_async_shutdown_error() override {
                if (m_fn_event_callback) {
                    m_fn_event_callback(this, "async_shutdown_error", amo::nil());
                }
                
                return BaseSocket::on_async_shutdown_error();
            }
            
            virtual bool on_heartbeat_error() override {
                if (m_fn_event_callback) {
                    m_fn_event_callback(this, "heartbeat_error", amo::nil());
                }
                
                return BaseSocket::on_heartbeat_error();
            }
            
            virtual void on_close() override {
                if (m_fn_event_callback) {
                    m_fn_event_callback(this, "close", amo::nil());
                }
                
                return BaseSocket::on_close();
            }
            
            virtual void send_heartbeat() override {
                if (m_fn_event_callback) {
                    m_fn_event_callback(this, "send_heartbeat", amo::nil());
                }
                
                return BaseSocket::send_heartbeat();
            }
        public:
            msg_callback_type get_on_message_callback() const {
                return m_fn_msg_callback;
            }
            
            void set_on_message_callback(
                msg_callback_type val) {
                m_fn_msg_callback = val;
            }
            
            event_callback_type get_event_callback()  const {
                return m_fn_event_callback;
            }
            
            void set_event_callback(event_callback_type val) {
                m_fn_event_callback = val;
            }
            
        protected:
            msg_callback_type m_fn_msg_callback;
            event_callback_type m_fn_event_callback;
        };
        
        
        template<typename Packer = amo::tcp::packer, typename Unpacker = amo::tcp::unpacker>
        class client_socket : public
            amo::tcp::socket<st_asio_wrapper::tcp::client_socket_base<Packer, Unpacker> > {
        public:
            typedef amo::tcp::socket<st_asio_wrapper::tcp::client_socket_base<Packer, Unpacker> >
            socket_type;
        public:
            client_socket(boost::asio::io_context & io_context_)
                : amo::tcp::socket<st_asio_wrapper::tcp::client_socket_base<Packer, Unpacker> >
                  (io_context_) {
                  
            }
        };
        
        
        
        template<typename Packer = amo::tcp::packer, typename Unpacker = amo::tcp::unpacker>
        class server_socket : public
            amo::tcp::socket<st_asio_wrapper::tcp::server_socket_base<Packer, Unpacker> > {
        public:
            typedef amo::tcp::socket<st_asio_wrapper::tcp::server_socket_base<Packer, Unpacker> >
            socket_type;
        public:
            /* server_socket(boost::asio::io_context & io_context_)
            	 : amo::socket<server_socket_base<Packer, Unpacker> >(io_context_) {
            
             }*/
            
            server_socket(st_asio_wrapper::tcp::i_server& server_)
                : amo::tcp::socket<st_asio_wrapper::tcp::server_socket_base<Packer, Unpacker> >
                  (server_) {
                  
            }
        };
    }
}



#endif //AMO_SOCKET_AC0DCC52_C0BA_462C_81F3_399EC6E771C2_HPP__