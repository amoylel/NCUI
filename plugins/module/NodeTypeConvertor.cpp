#include "NodeTypeConvertor.h"
#include "NodeV8Handler.h"
#include "module/ConstructorMgr.h"

namespace amo {

    NodeTypeConvertor::NodeTypeConvertor(const std::string& strName /*= ""*/) {
    
    }
    
    void NodeTypeConvertor::ParseJsonToObject(amo::u8json& json, v8::Local<v8::Value>& retval) {
    
    }
    
    std::string NodeTypeConvertor::V8ValueType(v8::Local<v8::Value> object) {
    
        if (object->IsBoolean()) {
            return "Bool";
        }
        
        if (object->IsInt32()) {
            return "Int";
        } else  if (object->IsArray()) {
            return "Array";
        } else if (object->IsDate()) {
            return "Date";
        } else if (object->IsFunction()) {
            return "Function";
        } else if (object->IsUint32()) {
            return "UInt";
        } else if (object->IsNull()) {
            return "NULL";
        } else if (object->IsObject()) {
            return "Object";
        } else if (object->IsString()) {
            return "String";
        } else if (object->IsUndefined()) {
            return "Undefined";
        } else {
            return "Unknown";
        }
    }
    
    v8::Local<v8::Value> NodeTypeConvertor::toV8Value(amo::Any& any) {
        v8::Local<v8::Value> retval;
        
        switch (any.type()) {
        case AnyValueType<amo::Undefined>::value:
            retval = Nan::Undefined();
            break;
            
        case AnyValueType<amo::Nil>::value:
            retval = Nan::Null();
            break;
            
        case AnyValueType<bool>::value:
            retval = Nan::New((bool)any);;
            break;
            
        case AnyValueType<int>::value:
            retval = Nan::New((int)any);
            break;
            
        case AnyValueType<double>::value:
            retval = Nan::New((double)any);
            break;
            
        case AnyValueType<std::string>::value:
            retval = Nan::New(any.As<std::string>()).ToLocalChecked();
            break;
            
        case  AnyValueType<amo::u8json>::value: {
            amo::u8json json = (amo::u8json)any;
            bool transferObject = json.get<bool>("transferObject");
            std::string strClassName = json.get<std::string>("name");
            int64_t nObjectID = json.get<int64_t>("id");
            auto manager = ConstructorMgr::getInstance();
            
            if (manager->hasConstruct(strClassName)) {
            
                v8::Local<v8::Function> cons = Nan::New<v8::Function>(*manager->getConstructor(strClassName));
                v8::Local<v8::Value> data[1] = { Nan::New(any.value()).ToLocalChecked() };
                retval = cons->NewInstance(1, data);
                
            } else {
                retval = Nan::Undefined();
                
            }
        }
        break;
        
        
        case AnyValueType<std::vector<Any>>::value: {
            std::vector<Any> vec = any;
            
            v8::Local<v8::Array> arr = v8::Array::New(v8::Isolate::GetCurrent(), vec.size());
            
            
            if (!vec.empty()) {
            
                v8::Local<v8::Array> retvalArr
                    = v8::Array::New(v8::Isolate::GetCurrent(), vec.size());
                    
                for (size_t i = 0; i < vec.size(); ++i) {
                
                    v8::Local<v8::Value> object = toV8Value(vec[i]);
                    retvalArr->Set(i, object);
                }
                
                retval = retvalArr;
            } else {
                retval = Nan::Undefined();
            }
            
        }
        break;
        
        
        default:
            retval = Nan::Undefined();
            break;
        }
        
        return retval;
    }
    
    
    
    Any NodeTypeConvertor::toAny(v8::Local<v8::Value>& pValue) {
        Any val;
        
        if (pValue->IsUndefined()) {
            val = Undefined();
        } else if (pValue->IsNull()) {
            val = Nil();
        } else if (pValue->IsBoolean()) {
            val = pValue->BooleanValue();
        } else if (pValue->IsInt32()) {
            val = pValue->Int32Value();
        } else if (pValue->IsString()) {
            val = toString(pValue);
        } else if (pValue->IsUint32()) {
            val = (int)pValue->Uint32Value();
        } else if (pValue->IsNumber()) {
            val = pValue->NumberValue();
        } else if (pValue->IsObject()) {
            Nan::MaybeLocal<v8::String> val2 = Nan::JSON().Stringify(pValue->ToObject());
            val = toString(val2.ToLocalChecked());
        } else {
            val = Undefined();
        }
        
        return val;
    }
    
    std::string NodeTypeConvertor::toString(v8::Local<v8::Value> value) {
        v8::String::Utf8Value utf8_value(value);
        return std::string(*utf8_value);
    }
    
    amo::NodeV8Handler* NodeTypeConvertor::getClassObject(int64_t nID) {
        auto iter = m_oObjectMap.find(nID);
        
        if (iter == m_oObjectMap.end()) {
            return NULL;
        }
        
        return iter->second;
    }
    
    void NodeTypeConvertor::addClassObject(int64_t nID, NodeV8Handler* pObject) {
        m_oObjectMap[nID] = pObject;
    }
    
    void NodeTypeConvertor::removeClassObject(int64_t nID) {
        m_oObjectMap.erase(nID);
    }
    
    std::unordered_map<int64_t, NodeV8Handler* > NodeTypeConvertor::m_oObjectMap;
    
}

