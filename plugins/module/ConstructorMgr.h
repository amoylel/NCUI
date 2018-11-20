// Created by amoylel on 07/12/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_CONSTRUCTORMGR_H__
#define AMO_CONSTRUCTORMGR_H__

#include <amo/singleton.hpp>


#include "nan/nan.h"

namespace amo {
    class ConstructorMgr : public amo::singleton<ConstructorMgr> {
    public:
        void addConstructor(const std::string& name,
                            std::shared_ptr<Nan::Persistent<v8::Function> > function);
        bool hasConstruct(const std::string& name);
        std::shared_ptr<Nan::Persistent<v8::Function> > getConstructor(
            const std::string& name);
    protected:
        std::unordered_map<std::string,
            std::shared_ptr<Nan::Persistent<v8::Function> > > m_oConstructorMap;
    };
}

#endif // AMO_CONSTRUCTORMGR_H__

