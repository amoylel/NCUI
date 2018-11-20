// Created by amoylel on 01/30/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_ADB1036_HPP__
#define AMO_ADB1036_HPP__

#include <amo/adb.hpp>

namespace amo {
    class adb1036 : public adb {
    public:
        adb1036(const std::string& deviceID = "")
            : adb(deviceID) {
            setAdbPath("adb/1036/adb.exe");
            
        }
    };
}

#endif // AMO_ADB1036_HPP__
