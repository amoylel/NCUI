// Created by amoylel on 05/03/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_UDP_UNPACKER_FB3B2EA5_E440_490F_B6F8_9E2C94FCD665_HPP__
#define AMO_UDP_UNPACKER_FB3B2EA5_E440_490F_B6F8_9E2C94FCD665_HPP__


#include <functional>
#include <string>

#include <amo/asio/st_asio_wrapper/ext/unpacker.h>

namespace amo {
    class udp_unpacker : public st_asio_wrapper::ext::udp_unpacker {
    public:
        virtual void parse_msg(msg_type& msg, size_t bytes_transferred)override {
            return st_asio_wrapper::ext::udp_unpacker::parse_msg(msg, bytes_transferred);
        }
    public:
    
        std::function < void(const  std::string&) > get_unpack_msg_callback() const {
            return m_fnUnpackMsg;
        }
        void set_unpack_msg_callback(std::function < void(const  std::string&) > val) {
            m_fnUnpackMsg = val;
        }
    protected:
        std::function<void(const std::string&)> m_fnUnpackMsg;
    };
}



#endif //AMO_UDP_UNPACKER_FB3B2EA5_E440_490F_B6F8_9E2C94FCD665_HPP__