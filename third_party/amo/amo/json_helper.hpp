// Created by amoylel on 22/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_JSON_HELPER_3B605D67_47AD_49F6_B1F6_61A7895C13E0_HPP__
#define AMO_JSON_HELPER_3B605D67_47AD_49F6_B1F6_61A7895C13E0_HPP__

#include <amo/impl/basic_json_helper.hpp>

namespace amo {
    typedef amo::basic_json_object<string_coding_ansi> json_object;
    typedef amo::basic_json_object<string_coding_utf8> u8json_object;
}

#endif //AMO_JSON_HELPER_3B605D67_47AD_49F6_B1F6_61A7895C13E0_HPP__