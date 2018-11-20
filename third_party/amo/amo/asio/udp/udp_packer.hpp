// Created by amoylel on 05/03/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_UDP_PACKER_A3E43D1F_13B5_4584_8110_2B59005D49D9_HPP__
#define AMO_UDP_PACKER_A3E43D1F_13B5_4584_8110_2B59005D49D9_HPP__


#include <string>
#include <functional>

#include <amo/asio/st_asio_wrapper/ext/packer.h>

namespace amo {
    class udp_packer : public st_asio_wrapper::ext::packer {
    public:
    
    
        virtual bool pack_msg(msg_type& msg, const char* const pstr[],
                              const size_t len[], size_t num, bool native = false) override {
            return st_asio_wrapper::ext::packer::pack_msg(msg, pstr, len, num, native);
        }
        
        
        virtual bool pack_heartbeat(msg_type& msg) override {
            return st_asio_wrapper::ext::packer::pack_heartbeat(msg);
        }
    public:
        std::function<std::string()> get_heartbeat_callback() const {
            return m_fnHeartbeat;
        }
        void set_heartbeat_callback(std::function<std::string()> val) {
            m_fnHeartbeat = val;
        }
    protected:
        std::function<std::string()> m_fnHeartbeat;	// 心跳包,处理函数
    };
}




#endif //AMO_UDP_PACKER_A3E43D1F_13B5_4584_8110_2B59005D49D9_HPP__