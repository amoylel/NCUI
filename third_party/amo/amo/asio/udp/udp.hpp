// Created by amoylel on 05/03/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_UDP_B92F6A6B_DDEB_402A_9084_A45BA5DE5731_HPP__
#define AMO_UDP_B92F6A6B_DDEB_402A_9084_A45BA5DE5731_HPP__

#include <amo/asio/config.hpp>

#include <amo/asio/utility/unified_out.hpp>
#include <amo/asio/udp/udp_packer.hpp>
#include <amo/asio/udp/udp_unpacker.hpp>
#include <amo/asio/st_asio_wrapper/ext/udp.h>
#include <amo/asio/st_asio_wrapper/udp/socket_service.h>

namespace amo {
    typedef st_asio_wrapper::udp::socket_base<amo::udp_packer, amo::udp_unpacker>
    socket_type;
    typedef st_asio_wrapper::udp::single_service_base<socket_type> service_type;
    class udp : public service_type  {
    public:
    
    public:
        udp(st_asio_wrapper::service_pump& sp_) : service_type(sp_), sp(sp_) {
        
        }
        
        ~udp() {
            stop_service();
        }
        
        bool is_running() const {
            return sp.is_running();
        }
        
        void restart_service() {
            stop_service();
            start_service();
        }
        
        void stop_service() {
            sp.stop_service();
        }
        
        void start_service() {
            return sp.start_service();
        }
        
        
#ifndef ST_ASIO_FORCE_TO_USE_MSG_RECV_BUFFER
        virtual bool on_msg(out_msg_type& msg) override {
            unified_out::debug_out("recv(" ST_ASIO_SF "): %s", msg.size(), msg.data());
            return true;
        }
#endif
        
        virtual bool on_msg_handle(out_msg_type& msg) override {
            unified_out::debug_out("recv(" ST_ASIO_SF "): %s", msg.size(), msg.data());
            return true;
        }
        
        virtual void on_recv_error(const boost::system::error_code& ec) override {
            return service_type::on_recv_error(ec);
        }
        
        virtual bool on_heartbeat_error() override {
            return service_type::on_heartbeat_error();
        }
        
        virtual void on_close() override {
            $cwarn("disconnect");
            return service_type::on_close();
        }
        
    protected:
        st_asio_wrapper::service_pump& sp;
        
        
        
    };
}

#endif //AMO_UDP_B92F6A6B_DDEB_402A_9084_A45BA5DE5731_HPP__