// Created by amoylel on 12/16/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_UNIFIED_OUT_HPP__
#define AMO_UNIFIED_OUT_HPP__

#include <stdio.h>
#include <boost/asio.hpp>
#include <boost/next_prior.hpp>
#include <string.h>
#include <sstream>
#include <assert.h>
#include <time.h>
#include <amo/logger.hpp>

#ifndef ST_ASIO_CUSTOM_LOG
#define ST_ASIO_CUSTOM_LOG
#endif

#ifndef ST_ASIO_UNIFIED_OUT_BUF_NUM
#define ST_ASIO_UNIFIED_OUT_BUF_NUM	2048
#endif
//#define ST_ASIO_FORCE_TO_USE_MSG_RECV_BUFFER //force to use the msg recv buffer

class custom_log_formater {
public:
    static void all_out(const char* head, char* buff, size_t buff_len,
                        const char* fmt, va_list& ap) {
        assert(NULL != buff && buff_len > 0);
        
        std::stringstream os;
        os.rdbuf()->pubsetbuf(buff, buff_len);
        
        /*if (NULL != head) {
        os << '[' << head << "] ";
        }*/
        
        char time_buff[64];
        time_t now = time(NULL);
#ifdef _MSC_VER
        ctime_s(time_buff, sizeof(time_buff), &now);
#else
        ctime_r(&now, time_buff);
#endif
        size_t len = strlen(time_buff);
        assert(len > 0);
        
        if ('\n' == *boost::next(time_buff, --len)) {
            *boost::next(time_buff, len) = '\0';
        }
        
        //os /*<< time_buff*/ << " -> ";
        
#if defined _MSC_VER || (defined __unix__ && !defined __linux__)
        os.rdbuf()->sgetn(buff, buff_len);
#endif
        len = (size_t)os.tellp();
        
        if (len >= buff_len) {
            *boost::next(buff, buff_len - 1) = '\0';
        } else
#if BOOST_WORKAROUND(BOOST_MSVC, >= 1400) && !defined(UNDER_CE)
            vsnprintf_s(boost::next(buff, len), buff_len - len, _TRUNCATE, fmt, ap);
            
#else
            vsnprintf(boost::next(buff, len), buff_len - len, fmt, ap);
#endif
    }
};

#define custom_all_out_helper(head, buff, buff_len) va_list ap; va_start(ap, fmt); custom_log_formater::all_out(head, buff, buff_len, fmt, ap); va_end(ap)
#define custom_all_out_helper2(head) char output_buff[ST_ASIO_UNIFIED_OUT_BUF_NUM]; custom_all_out_helper(#head, output_buff, sizeof(output_buff)); $clog(amo::head << func_orient <<  (output_buff) << amo::endl;);

// 重定向网络日志到程序日志中
class unified_out {
public:
    static void fatal_out(const char* fmt, ...) {
        custom_all_out_helper2(cfatal);
    }
    static void error_out(const char* fmt, ...) {
        custom_all_out_helper2(cerr);
    }
    static void warning_out(const char* fmt, ...) {
        custom_all_out_helper2(cwarn);
    }
    static void info_out(const char* fmt, ...) {
    
        custom_all_out_helper2(cinfo);
    }
    static void debug_out(const char* fmt, ...) {
        custom_all_out_helper2(cdebug);
    }
};

#endif // AMO_UNIFIED_OUT_HPP__

