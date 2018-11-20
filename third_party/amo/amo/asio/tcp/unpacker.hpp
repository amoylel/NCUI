// Created by amoylel on 05/03/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_UNPACKER_9B6F63B4_95A8_4AD0_92D9_07C7B1D0AFE5_HPP__
#define AMO_UNPACKER_9B6F63B4_95A8_4AD0_92D9_07C7B1D0AFE5_HPP__

#include <string>
#include <functional>

#include <amo/utility.hpp>

#include <amo/asio/st_asio_wrapper/ext/unpacker.h>
#include <amo/asio/st_asio_wrapper/base.h>


using namespace st_asio_wrapper;

namespace amo {
    namespace tcp {
        //protocol: length + body
        typedef  st_asio_wrapper::ext::unpacker unpacker;
        //protocol: length + body
        //T can be replaceable_buffer (an alias of auto_buffer) or shared_buffer, the latter makes output messages seemingly copyable,
        typedef  st_asio_wrapper::ext::replaceable_unpacker<st_asio_wrapper::replaceable_buffer> replaceable_unpacker;
        //protocol: fixed length
        //non-copy
        typedef  st_asio_wrapper::ext::fixed_length_unpacker fixed_length_unpacker;
        //protocol: [prefix] + body + suffix
        typedef  st_asio_wrapper::ext::prefix_suffix_unpacker prefix_suffix_unpacker;
        
    }
}
namespace amo {

    /*class unpacker : public st_asio_wrapper::ext::prefix_suffix_unpacker {
    public:
        unpacker() {
            this->prefix_suffix("ab27ae3a-07ad-4a9f-a2b2-d239f038f961",
                                "504661c9-1476-4440-bc15-5828c69d2827");
        }
    
        virtual bool parse_msg(size_t bytes_transferred,
                               container_type& msg_can) override {
            if (!st_asio_wrapper::ext::prefix_suffix_unpacker::parse_msg(bytes_transferred,
                    msg_can)) {
                return false;
            }
    
            for (auto iter = msg_can.begin(); iter != msg_can.end(); ++iter) {
                if (get_unpack_msg_callback()) {
                    get_unpack_msg_callback()(*iter);
                }
            }
    
            return true;
    
        }
    
        std::function < void(const  std::string&) > get_unpack_msg_callback() const {
            return m_fnUnpackMsg;
        }
        void set_unpack_msg_callback(std::function < void(const  std::string&) > val) {
            m_fnUnpackMsg = val;
        }
    protected:
        std::function<void(const std::string&)> m_fnUnpackMsg;
    
    };*/
    
}



#endif //AMO_UNPACKER_9B6F63B4_95A8_4AD0_92D9_07C7B1D0AFE5_HPP__