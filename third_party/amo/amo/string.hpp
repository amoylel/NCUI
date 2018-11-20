// Created by amoylel on 13/06/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_STRING_7A730025_82D8_415C_B6F8_9B46F48786FB_HPP__
#define AMO_STRING_7A730025_82D8_415C_B6F8_9B46F48786FB_HPP__







#include <string>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <vector>

#include <amo/config.hpp>
#include <amo/stdint.hpp>
#include <amo/lexical_cast.hpp>
#include <amo/string/string_utils.hpp>
#include <amo/string/tiny_string.hpp>
#include <amo/string/basic_string.hpp>


namespace amo {
    typedef amo::basic_string<string_coding_ansi> string;
    typedef amo::basic_string<string_coding_utf8> u8string;
}

#endif //AMO_STRING_7A730025_82D8_415C_B6F8_9B46F48786FB_HPP__

