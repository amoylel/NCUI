// Created by amoylel on 11/07/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_REGULAR_HPP__
#define AMO_REGULAR_HPP__

#include <string>
#include <regex>

namespace amo {
    class regular {
    public:
        regular(const std::string str)
            : m_str(str) {
            
        }
        
        bool is_number() {
            return  std::regex_match(m_str, std::regex("^(-|\\+)?\\d+$"));
        }
        
        bool is_qq_number() {
            // QQ号5到11位
            return  std::regex_match(m_str, std::regex("^[1-9]\\d{4,10}$"));
        }
        
        bool is_mobile_number() {
            // 这个不行
            return std::regex_match(m_str, std::regex("^1[0-9][0-9]{1}[0-9]{8}$|^15[0-9]{1}[0-9]{8}$|^18[0-9]{1}[0-9]{8}$"));
        }
    private:
        std::string m_str;
    };
}


#endif // AMO_REGULAR_HPP__