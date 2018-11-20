// Created by amoylel on 07/10/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODETYPECONVERTOR_H__
#define AMO_NODETYPECONVERTOR_H__

#include <string>
#include <v8.h>
#include <amo/json.hpp>

#include "ipc/Any.hpp"
#include <unordered_map>
#include "NodeV8Handler.h"

namespace amo {

    class NodeTypeConvertor {
    public:
        NodeTypeConvertor(const std::string& strName = "");
        void ParseJsonToObject(amo::u8json& json, v8::Local<v8::Value>& retval);
        
        // 获取V8类型
        std::string V8ValueType(v8::Local<v8::Value> object);
        
        v8::Local<v8::Value> toV8Value(amo::Any& ret);
        Any toAny(v8::Local<v8::Value>& pValue);;
        static std::string toString(v8::Local<v8::Value> value);
        
        static NodeV8Handler*  getClassObject(int64_t nID);
        static void addClassObject(int64_t nID, NodeV8Handler* pObject);
        static void removeClassObject(int64_t nID);
    protected:
        static std::unordered_map<int64_t, NodeV8Handler* > m_oObjectMap;
        
        std::string m_strName;
        
    };
}

#endif // AMO_NODETYPECONVERTOR_H__