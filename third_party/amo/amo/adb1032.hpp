// Created by amoylel on 11/05/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_ADB1032_85FDA4DC_356E_46B9_8E27_CEECFD8B4E8D_HPP__
#define AMO_ADB1032_85FDA4DC_356E_46B9_8E27_CEECFD8B4E8D_HPP__

#include <amo/adb.hpp>

namespace amo {
    class adb1032 : public adb {
    public:
        adb1032(const std::string& deviceID = "")
            : adb(deviceID) {
            setAdbPath("adb/1032/adb.exe");
        }
    };
}



#endif //AMO_ADB1032_85FDA4DC_356E_46B9_8E27_CEECFD8B4E8D_HPP__