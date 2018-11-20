// Created by amoylel on 13/06/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_TEST_ADECBAA3_BD6F_45CB_89BD_164B56B9C53E_HPP__
#define AMO_TEST_ADECBAA3_BD6F_45CB_89BD_164B56B9C53E_HPP__


#include <string>
class my_plugin_api3 {
public:
    virtual std::string name() const = 0;
    virtual float calculate(float x, float y) = 0;
    virtual ~my_plugin_api3() {}
};


class my_plugin_api2 {
public:
    virtual std::string name() const {
        return "111";
    };
    virtual float calculate(float x, float y) {
        return x + y;
    };
    virtual ~my_plugin_api2() {}
};


#endif //AMO_TEST_ADECBAA3_BD6F_45CB_89BD_164B56B9C53E_HPP__