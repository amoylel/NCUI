// Created by amoylel on 05/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_PATH_E83CCAD2_20A4_48B1_B382_DB58A6DD79EA_HPP__
#define AMO_PATH_E83CCAD2_20A4_48B1_B382_DB58A6DD79EA_HPP__

#include <amo/impl/basic_path.hpp>
#include <amo/string/coding_type.hpp>

namespace amo {
    typedef amo::basic_path<string_coding_ansi> path;
    typedef amo::basic_path<string_coding_utf8> u8path;
}

#endif //AMO_PATH_E83CCAD2_20A4_48B1_B382_DB58A6DD79EA_HPP__