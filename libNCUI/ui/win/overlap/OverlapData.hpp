// Created by amoylel on 08/19/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_OVERLAPDATA_1CD383A7_67A4_43DA_9A59_BCD4CC0F06AB_HPP__
#define AMO_OVERLAPDATA_1CD383A7_67A4_43DA_9A59_BCD4CC0F06AB_HPP__

namespace amo {

    enum OverlapType {
        OverlapDefault,
    };
    
    class OverlapData {
    public:
        virtual OverlapType type() {
            return OverlapDefault;
        }
        virtual void fill(const char* buffer, int size) = 0;
        
        virtual int size() = 0;
        
        virtual const char* data() const = 0;
        
        virtual void set_locked(bool locked) {
        }
        
        virtual void resize(int size, char val)  {
        
        }
    };
}

#endif // AMO_OVERLAPDATA_1CD383A7_67A4_43DA_9A59_BCD4CC0F06AB_HPP__

