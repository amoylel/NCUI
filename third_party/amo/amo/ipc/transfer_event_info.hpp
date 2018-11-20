// Created by amoylel on 14/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_TRANSFER_EVENT_INFO_BFBB87F3_E4CA_454C_B861_C70A4B4221AB_HPP__
#define AMO_TRANSFER_EVENT_INFO_BFBB87F3_E4CA_454C_B861_C70A4B4221AB_HPP__





#include <amo/json.hpp>
#include <amo/string.hpp>
#include <amo/ipc/any.hpp>
#include <amo/ipc/utility.hpp>

namespace amo {

    class transfer_event_info {
    public:
        transfer_event_info() {
        
        }
        transfer_event_info(const amo::string& name_,
                            bool suspend_ = false,
                            bool toAll_ = false,
                            int browser_ = -1,
                            int64_t frame_ = -1)
            : name(name_)
            , suspend(suspend_)
            , toAll(toAll_)
            , browser(browser_)
            , frame(frame_) {
        }
        
        // 这里进去的如果是一个字符串，或者JSON，必须是utf8编码的，（数据在传输过程中不会进行编码转换）
        void setData(const any& val) {
            data = val;
        }
        
        // 暂时用不着
        amo::json toJson() const {
            amo::json json;
            json.put("name", name);
            json.put("suspend", suspend);
            json.put("toAll", toAll);
            json.put("browser", browser);
            json.put("frame", frame);
            amo::ipc::util().add_any_to_json(json, "data", data);
            
            
            return json;
        }
        
        std::string toString() const {
            return toJson().to_string();
        }
        
        transfer_event_info fromJson(const amo::json& json) {
            transfer_event_info info;
            
            if (json.is_valid()) {
                return info;
            }
            
            info.name = json.getString("name");
            info.suspend = json.getBool("suspend");
            info.toAll = json.getBool("toAll");
            info.browser = json.getInt("browser", -1);
            info.frame = json.getInt64("frame", -1);
            //TODO: 这里要改
            info.data = json.getString("data");
            return info;
        }
        
    public:
        /** @brief	事件名称. */
        std::string name;
        /** @brief	暂停线程. */
        bool suspend;
        /** @brief	发送给所有JS对象. */
        bool toAll;
        /** @brief	数据. */
        any data;
        /** @brief	目标浏览器ID. */
        int browser;
        /** @brief	目标FrameID. */
        int64_t frame;
        
    };
}


#endif //AMO_TRANSFER_EVENT_INFO_BFBB87F3_E4CA_454C_B861_C70A4B4221AB_HPP__