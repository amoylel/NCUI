// Created by amoylel on 01/30/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_ADB1039_HPP__
#define AMO_ADB1039_HPP__

#include <amo/adb.hpp>

namespace amo {
    class adb1039 : public adb {
    public:
        adb1039(const std::string& deviceID = "")
            : adb(deviceID) {
            setAdbPath("adb/1039/adb.exe");
            
        }
    };
}

#endif // AMO_ADB1039_HPP__