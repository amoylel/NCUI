// Created by amoylel on 05/03/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_PACKER_EE51D0C3_85DC_4165_8CA4_5A52D911E1AC_HPP__
#define AMO_PACKER_EE51D0C3_85DC_4165_8CA4_5A52D911E1AC_HPP__

#ifndef ST_ASIO_MSG_BUFFER_SIZE
#define ST_ASIO_MSG_BUFFER_SIZE	0xfffff
#endif

#include <functional>
#include <string>
#include <vector>

#include <amo/utility.hpp>
#include <amo/asio/config.hpp>
#include <amo/asio/st_asio_wrapper/ext/packer.h>


namespace amo {
    namespace tcp {
    
    
        //protocol: length + body
        typedef st_asio_wrapper::ext::packer packer;
        
        //protocol: length + body
        //T can be replaceable_buffer (an alias of auto_buffer) or shared_buffer, the latter makes output messages seemingly copyable.
        typedef st_asio_wrapper::ext::replaceable_packer<st_asio_wrapper::replaceable_buffer>
        replaceable_packer;
        
        //protocol: fixed length
        typedef st_asio_wrapper::ext::fixed_length_packer fixed_length_packer;
        
        //protocol: [prefix] + body + suffix
        typedef st_asio_wrapper::ext::prefix_suffix_packer prefix_suffix_packer;
    }
}

namespace amo {

    //class packer : public st_asio_wrapper::ext::prefix_suffix_packer {
    //public:
    //
    //    packer::packer() {
    //        this->prefix_suffix("ab27ae3a-07ad-4a9f-a2b2-d239f038f961",
    //                            "504661c9-1476-4440-bc15-5828c69d2827");
    //    }
    //
    //    virtual bool pack_heartbeat(msg_type& msg) override  {
    //        if (get_heartbeat_callback()) {
    //            std::string str = m_fnHeartbeat();
    //            const char*  pstr[1] = { 0 };
    //            pstr[0] = str.c_str();
    //            size_t len[1] = { str.length() };
    //            return pack_msg(msg, pstr, len, 1);
    //        } else {
    //            return  st_asio_wrapper::ext::prefix_suffix_packer::pack_heartbeat(msg);
    //        }
    //    }
    //public:
    //    std::function<std::string()> get_heartbeat_callback() const {
    //        return m_fnHeartbeat;
    //    }
    //    void set_heartbeat_callback(std::function<std::string()> val) {
    //        m_fnHeartbeat = val;
    //    }
    //protected:
    //    std::function<std::string()> m_fnHeartbeat;	// 心跳包,处理函数
    //};
}



#endif //AMO_PACKER_EE51D0C3_85DC_4165_8CA4_5A52D911E1AC_HPP__

