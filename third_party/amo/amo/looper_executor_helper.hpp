// Created by amoylel on 24/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_LOOPER_EXECUTOR_HELPER_A5C6F07C_3334_4553_97BC_0ECD908FA004_HPP__
#define AMO_LOOPER_EXECUTOR_HELPER_A5C6F07C_3334_4553_97BC_0ECD908FA004_HPP__

#include <amo/looper_executor.hpp>
#include <amo/json_helper.hpp>
#include <amo/date_time.hpp>

namespace amo {
    class thread_response_info_json
        : public amo::looper_executor::thread_response_info
        , public amo::json_object {
    public:
        thread_response_info_json() {
        
        }
        
        thread_response_info_json(amo::looper_executor::thread_response_info& rhs) {
            amo::looper_executor::thread_response_info* ptr = this;
            *ptr = rhs;
            
        }
        
        
        virtual std::string onFormatJson(const std::string& key,
                                         const std::string& val) override {
            if (key == "request_timestamp" || key == "response_timestamp") {
                return  this->get<amo::date_time>(key).format("yyyy-MM-dd hh:mm:ss.ms");
            }
            
            return json_object::onFormatJson(key, val);
        }
        
    public:
        AMO_ENTITY_ARGS_SET_BEGIN(thread_response_info_json)
        AMO_ENTITY_ARGS_SET(thread_name)
        AMO_ENTITY_ARGS_SET(thread_id)
        
        AMO_ENTITY_ARGS_SET(test_id)
        AMO_ENTITY_ARGS_SET(request_timestamp)
        AMO_ENTITY_ARGS_SET(response_timestamp)
        AMO_ENTITY_ARGS_SET(delay)
        
        AMO_ENTITY_ARGS_SET(exception_count)
        
        AMO_ENTITY_ARGS_SET_END()
        
        AMO_ENTITY_ARGS_GET_BEGIN(thread_response_info_json)
        AMO_ENTITY_ARGS_GET(thread_name)
        AMO_ENTITY_ARGS_GET(thread_id)
        
        AMO_ENTITY_ARGS_GET(test_id)
        AMO_ENTITY_ARGS_GET(request_timestamp)
        AMO_ENTITY_ARGS_GET(response_timestamp)
        AMO_ENTITY_ARGS_GET(delay)
        
        AMO_ENTITY_ARGS_GET(exception_count)
        
        
        AMO_ENTITY_ARGS_GET_END()
        
        AMO_ENTITY_FORMAT_BEGIN(thread_response_info_json)
        
        AMO_ENTITY_FORMAT_SET(thread_name, "ThreadName")
        AMO_ENTITY_FORMAT_SET(thread_id, "ThreadID")
        AMO_ENTITY_FORMAT_SET(test_id, "TestID")
        AMO_ENTITY_FORMAT_SET(request_timestamp, "TestTime")
        AMO_ENTITY_FORMAT_SET(response_timestamp, "RespTime")
        AMO_ENTITY_FORMAT_SET(delay, "RespDelay")
        AMO_ENTITY_FORMAT_SET(exception_count, "Exceptions")
        
        
        AMO_ENTITY_FORMAT_CALLBACK_SET(onFormatJson)
        AMO_ENTITY_FORMAT_END()
        
    };
}



#endif //AMO_LOOPER_EXECUTOR_HELPER_A5C6F07C_3334_4553_97BC_0ECD908FA004_HPP__