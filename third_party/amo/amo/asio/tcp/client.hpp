// Created by amoylel on 11/06/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_CLIENT_FAC0818A_83C0_4E84_BB7E_8B0F7FF5853D_HPP__
#define AMO_CLIENT_FAC0818A_83C0_4E84_BB7E_8B0F7FF5853D_HPP__


#include <memory>

#include <unordered_map>
#include <amo/json.hpp>
#include <amo/asio/tcp/socket.hpp>
#include <amo/asio/tcp/service.hpp>


namespace amo {


    namespace tcp {
    
        template<typename Packer = amo::tcp::packer, typename Unpacker = amo::tcp::unpacker>
        class client : public
            amo::tcp::service< st_asio_wrapper::tcp::single_client_base<client_socket<Packer, Unpacker> >, client_socket<Packer, Unpacker>, Packer, Unpacker  >
            , public std::enable_shared_from_this<client<Packer, Unpacker> > {
            
        public:
            typedef typename client_socket<Packer, Unpacker>::out_msg_type out_msg_type;
            typedef typename client_socket<Packer, Unpacker>::socket_type socket_type;
            typedef typename
            object_pool< client_socket<Packer, Unpacker> >::object_ctype object_type;
            
            
            typedef std::function<bool(socket_type*, out_msg_type&)>
            msg_callback_type;
            typedef std::function<void(socket_type*, const std::string&, const boost::any&)>
            event_callback_type;
        public:
            client(service_pump& sp) :
                amo::tcp::service< st_asio_wrapper::tcp::single_client_base<client_socket<Packer, Unpacker> >, client_socket<Packer, Unpacker>, Packer, Unpacker  >
                (sp) {
                this->set_on_message_callback(
                    std::bind(&client::on_recv_msg, this,
                              std::placeholders::_1,
                              std::placeholders::_2));
                              
                this->set_event_callback(
                    std::bind(&client::on_event_msg, this,
                              std::placeholders::_1,
                              std::placeholders::_2,
                              std::placeholders::_3));
            }
            
            ~client() {
            
            }
        };
        
        
        template<typename Packer = amo::tcp::packer, typename Unpacker = amo::tcp::unpacker>
        class multi_client : public
            amo::tcp::service< st_asio_wrapper::tcp::multi_client_base<client_socket<Packer, Unpacker> >, client_socket<Packer, Unpacker>, Packer, Unpacker  >
            , public std::enable_shared_from_this<multi_client<Packer, Unpacker> > {
            
        public:
            typedef typename client_socket<Packer, Unpacker>::out_msg_type out_msg_type;
            typedef typename client_socket<Packer, Unpacker>::socket_type socket_type;
            typedef typename
            object_pool< client_socket<Packer, Unpacker> >::object_ctype object_type;
            
            
            typedef std::function<bool(socket_type*, out_msg_type&)>
            msg_callback_type;
            typedef std::function<void(socket_type*, const std::string&, const boost::any&)>
            event_callback_type;
        public:
            multi_client(service_pump& sp) :
                amo::tcp::service< st_asio_wrapper::tcp::multi_client_base<client_socket<Packer, Unpacker> >, client_socket<Packer, Unpacker>, Packer, Unpacker  >
                (sp) {
                
            }
            
            
            object_type add_remote_addr(unsigned short port,
                                        const std::string& ip = ST_ASIO_SERVER_IP) {
                object_type ptr =	this->add_socket(port, ip);
                auto id = ptr->id();
                auto ptr2 = this->find(id);
                ptr->set_on_message_callback(
                    std::bind(&multi_client::on_recv_msg, this,
                              std::placeholders::_1,
                              std::placeholders::_2));
                              
                ptr->set_event_callback(
                    std::bind(&multi_client::on_event_msg, this,
                              std::placeholders::_1,
                              std::placeholders::_2,
                              std::placeholders::_3));
                              
                return ptr;
                
            }
            
            ~multi_client() {
            
            }
            
            
            
        };
        
    }
    
}

#endif //AMO_CLIENT_FAC0818A_83C0_4E84_BB7E_8B0F7FF5853D_HPP__