// Created by amoylel on 29/09/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_CODING_TYPE_C502EDF8_1E8B_45A8_8C79_769DB4CF83AF_HPP__
#define AMO_CODING_TYPE_C502EDF8_1E8B_45A8_8C79_769DB4CF83AF_HPP__


namespace amo {
    enum StringCodingType {
        CODING_TYPE_UNKNOWN,
        CODING_TYPE_ANSI,
        CODING_TYPE_UTF8,
    };
    
    class string_coding_unknown {
    public:
        static const StringCodingType coding_type = CODING_TYPE_UNKNOWN;
    };
    
    class string_coding_ansi {
    public:
        static const StringCodingType coding_type = CODING_TYPE_ANSI;
    };
    
    class string_coding_utf8 {
    public:
        static const StringCodingType coding_type = CODING_TYPE_UTF8;
    };
    
    
    
}



#endif //AMO_CODING_TYPE_C502EDF8_1E8B_45A8_8C79_769DB4CF83AF_HPP__