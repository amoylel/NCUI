// Created by amoylel on 11/06/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_SERVER_SOCKET_PROXY_1AAF15C1_7879_4541_8F7E_B0168C3A95F4_HPP__
#define AMO_SERVER_SOCKET_PROXY_1AAF15C1_7879_4541_8F7E_B0168C3A95F4_HPP__

#include <boost/system/error_code.hpp>


namespace amo {
    namespace tcp {
    
        template<typename Packer, typename Unpacker>
        class server_socket_proxy {
        
        public:
            /**
            * @fn	virtual bool send_heartbeat()
            *
            * @brief	发送心跳包时触发.
            *
            * @return	true if it succeeds, false if it fails.
            */
            
            virtual bool send_heartbeat() {
                return false;
            }
            
            virtual bool on_msg(typename Unpacker::msg_type& msg) {
                return true;
            }
            
            virtual bool do_send_msg() {
                return false;
            }
            
            virtual void do_recv_msg() {
                return;
            }
            
            virtual void on_send_error(const boost::system::error_code& ec) {
                return;
            }
            
            virtual void on_recv_error(const boost::system::error_code& ec) {
                return ;
            }
            
            virtual bool on_heartbeat_error() {
                return false;
            }
            
            
        };
        
    }
}



#endif //AMO_SERVER_SOCKET_PROXY_1AAF15C1_7879_4541_8F7E_B0168C3A95F4_HPP__