// Created by amoylel on 30/03/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_TYPE_TRAITS_9AAE7F9D_49C3_43EE_BBFB_2D1B44666281_HPP__
#define AMO_TYPE_TRAITS_9AAE7F9D_49C3_43EE_BBFB_2D1B44666281_HPP__

#include <string>

namespace amo {


    template<typename T> struct is_not_void {
        static const bool value = true;
    };
    template<> struct is_not_void < void > {
        static const bool value = false;
    };
    
    //typedef std::string u8string;	// 以utf8编码的字符串
    typedef std::string astring;	// 以ansi编码的字符串
    // 与平台编码相同的字符串，windows为GB2312 linux为UTF8
    typedef std::string ustring;
    
    
} // namespace amo




#endif //AMO_TYPE_TRAITS_9AAE7F9D_49C3_43EE_BBFB_2D1B44666281_HPP__

