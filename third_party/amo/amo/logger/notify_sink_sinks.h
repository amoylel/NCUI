// Created by amoylel on 01/15/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_NOTIFY_SINK_SINKS_H__
#define AMO_NOTIFY_SINK_SINKS_H__



#pragma once

#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>
#include <spdlog/details/file_helper.h>
#include <spdlog/fmt/fmt.h>

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <mutex>
#include <string>
#include <cerrno>

namespace spdlog {
    namespace sinks {
        /*
        * Trivial file sink with single file as target
        */
        template<class Mutex>
        class notify_sink : public base_sink < Mutex > {
        public:
            explicit notify_sink()   {
            
            }
            void flush() override {
                _file_helper.flush();
            }
            
        protected:
            void _sink_it(const details::log_msg& msg) override {
                _file_helper.write(msg);
            }
        private:
            details::file_helper _file_helper;
        };
        
        typedef notify_sink<std::mutex> notify_sink_mt;
        typedef notify_sink<details::null_mutex> notify_sink_st;
        
    }
}

#endif // AMO_NOTIFY_SINK_SINKS_H__
