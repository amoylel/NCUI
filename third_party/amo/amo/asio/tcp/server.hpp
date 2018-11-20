// Created by amoylel on 06/10/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_SERVER_3973A231_5170_4C74_B134_649AC9857E02_HPP__
#define AMO_SERVER_3973A231_5170_4C74_B134_649AC9857E02_HPP__

#include <amo/asio/tcp/socket.hpp>
#include <amo/asio/st_asio_wrapper/tcp/server.h>
#include <amo/asio/tcp/service.hpp>


//using namespace st_asio_wrapper;
//using namespace st_asio_wrapper::tcp;
//using namespace st_asio_wrapper::ext;
//using namespace st_asio_wrapper::ext::tcp;

namespace amo {




    namespace tcp {
    
    
        template<typename Packer = amo::tcp::packer, typename Unpacker = amo::tcp::unpacker>
        class server : public
            amo::tcp::service< st_asio_wrapper::tcp::server_base<server_socket<Packer, Unpacker> >, server_socket<Packer, Unpacker>, Packer, Unpacker  >
            , public std::enable_shared_from_this<server<Packer, Unpacker> > {
            
        public:
            typedef typename server_socket<Packer, Unpacker>::out_msg_type out_msg_type;
            typedef typename server_socket<Packer, Unpacker>::socket_type socket_type;
            typedef typename
            object_pool< server_socket<Packer, Unpacker> >::object_ctype object_type;
            
            
            typedef std::function<bool(socket_type*, out_msg_type&)>
            msg_callback_type;
            typedef std::function<void(socket_type*, const std::string&, const boost::any&)>
            event_callback_type;
        public:
            server(service_pump& sp) :
                amo::tcp::service< st_asio_wrapper::tcp::server_base<server_socket<Packer, Unpacker> >, server_socket<Packer, Unpacker>, Packer, Unpacker  >
                (sp) {
                
            }
            
            ~server() {
            
            }
            //virtual void on_event_msg(socket_type* ptr, const std::string& name,
            //                          const boost::any& val) override {
            //
            //    if (m_fn_default_event_callback) {
            //        return m_fn_default_event_callback(ptr, name, val);
            //    }
            //
            //    //std::cout << "trigger event :" << name << std::endl;
            //}
            
            /**
            * @fn	virtual bool service::on_accept(object_type& socket_ptr) override
            *
            * @brief	有socket连接时触发.
            *
            * @param	#socket 连接的socket对象 .
            *
            * @return	#bool true if it succeeds, false if it fails.
            */
            
            virtual bool on_accept(object_type& socket_ptr)  override {
            
                socket_ptr->set_on_message_callback(
                    std::bind(&server::on_recv_msg, this,
                              std::placeholders::_1,
                              std::placeholders::_2));
                              
                socket_ptr->set_event_callback(
                    std::bind(&server::on_event_msg, this,
                              std::placeholders::_1,
                              std::placeholders::_2,
                              std::placeholders::_3));
                              
                return  amo::tcp::service< st_asio_wrapper::tcp::server_base<server_socket<Packer, Unpacker> >, server_socket<Packer, Unpacker>, Packer, Unpacker  >::on_accept(
                            socket_ptr);
                            
                            
            }
            
            
        protected:
        
        };
        
        
    }
    
}


#endif // AMO_SERVER_3973A231_5170_4C74_B134_649AC9857E02_HPP__

