// Created by amoylel on 03/22/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_DLLVALUEACCESSOR_H__
#define AMO_DLLVALUEACCESSOR_H__

#include "handler/CefHeader.hpp"
#include "module/dll/TypeMapManager.h"
#include "module/JSV8Handler.h"
#include <amo/string.hpp>
#include <amo/nil.hpp>
#include <string>

namespace amo {

    //
    
    class DllBoolAccessor : public JsV8Handler {
    public:
        typedef bool value_type;
    public:
        DllBoolAccessor() {
            m_nID = 0;
            value_type val = value_type(0);
            TypeItem item;
            item.data = val;
            item.nType = (JS_TYPE)TypeToInt<value_type>::value;;
            item.nAddr = (int32_t)val;
            item.m_fnRelease = amo::bind(&DllBoolAccessor::release, this);
            m_nID = TypeMapManager::getInstance()->add(item);
        }
        
        virtual CefString name() {
            return typeid(value_type()).name();
        }
        
        virtual std::string id() {
            return amo::u8string::from_number(m_nID).str();
        }
        
        virtual void release() {
            return;
        }
        
        virtual void typeName(CefRefPtr<CefV8Value> object,
                              const CefV8ValueList& arguments,
                              CefRefPtr<CefV8Value>& retval,
                              CefString& except) {
            retval = CefV8Value::CreateString(name());
        }
        
        virtual void val(CefRefPtr<CefV8Value> object,
                         const CefV8ValueList& arguments,
                         CefRefPtr<CefV8Value>& retval,
                         CefString& except) {
            TypeItem& item = TypeMapManager::getInstance()->get(m_nID);
            
            if (arguments.size() == 0) {
                value_type value = boost::any_cast<value_type>(item.data);
                retval = CefV8Value::CreateBool(value);
            } else {
                if (arguments.at(0)->IsString()) {
                    amo::u8string strNum(arguments.at(0)->GetStringValue(), true);
                    strNum = strNum.to_upper();
                    
                    if (strNum == "TRUE") {
                        item.data = true;
                    } else {
                        item.data = false;
                    }
                    
                } else if (arguments.at(0)->IsBool()) {
                    item.data = (value_type)arguments.at(0)->GetBoolValue();
                }
            }
        }
    protected:
        int64_t m_nID;
    };
    
    // 整型 int8 int16 int32 int64 long,  long long  uint8 ...
    template<typename T>
    class DllIntegerAccessor : public JsV8Handler {
    public:
        typedef T value_type;
    public:
        DllIntegerAccessor() {
            m_nID = 0;
            value_type val = value_type(0);
            TypeItem item;
            item.data = val;
            item.nType = (JS_TYPE)TypeToInt<value_type>::value;;
            item.nAddr = (int32_t)val;
            item.m_fnRelease = amo::bind(&DllIntegerAccessor<T>::release, this);
            m_nID = TypeMapManager::getInstance()->add(item);
        }
        
        virtual CefString name() {
            return typeid(value_type()).name();
        }
        
        virtual std::string id() {
            return amo::u8string::from_number(m_nID).str();
        }
        
        virtual void release() {
            return;
        }
        
        virtual void typeName(CefRefPtr<CefV8Value> object,
                              const CefV8ValueList& arguments,
                              CefRefPtr<CefV8Value>& retval,
                              CefString& except) {
            retval = CefV8Value::CreateString(name());
        }
        
        virtual void val(CefRefPtr<CefV8Value> object,
                         const CefV8ValueList& arguments,
                         CefRefPtr<CefV8Value>& retval,
                         CefString& except) {
            TypeItem& item = TypeMapManager::getInstance()->get(m_nID);
            
            if (arguments.size() == 0) {
                value_type value = boost::any_cast<value_type>(item.data);
                retval = CefV8Value::CreateInt(value);
            } else {
                if (arguments.at(0)->IsString()) {
                    amo::u8string strNum(arguments.at(0)->GetStringValue(), true);
                    item.data = strNum.to_number<value_type>();
                } else if (arguments.at(0)->IsInt()) {
                    item.data = (value_type)arguments.at(0)->GetIntValue();
                }
            }
        }
    protected:
        int64_t m_nID;
    };
    
    // 浮点型 float double long double
    template<typename T>
    class DllFloatAccessor : public JsV8Handler {
    public:
        typedef T value_type;
    public:
        DllFloatAccessor() {
            m_nID = 0;
            value_type val = value_type(0);
            TypeItem item;
            item.data = val;
            item.nType = (JS_TYPE)TypeToInt<value_type>::value;;
            item.nAddr = (int32_t)val;
            item.m_fnRelease = amo::bind(&DllFloatAccessor<T>::release, this);
            m_nID = TypeMapManager::getInstance()->add(item);
        }
        
        virtual CefString name() {
            return typeid(value_type()).name();
        }
        
        virtual std::string id() {
            return amo::u8string::from_number(m_nID).str();
        }
        virtual void release() {
            return;
        }
        
        virtual void typeName(CefRefPtr<CefV8Value> object,
                              const CefV8ValueList& arguments,
                              CefRefPtr<CefV8Value>& retval,
                              CefString& except) {
            retval = CefV8Value::CreateString(name());
        }
        
        virtual void val(CefRefPtr<CefV8Value> object,
                         const CefV8ValueList& arguments,
                         CefRefPtr<CefV8Value>& retval,
                         CefString& except) {
            TypeItem& item = TypeMapManager::getInstance()->get(m_nID);
            
            if (arguments.size() == 0) {
                value_type value =  boost::any_cast<value_type>(item.data);
                retval = CefV8Value::CreateDouble(value);
            } else {
                if (arguments.at(0)->IsString()) {
                    amo::u8string strNum(arguments.at(0)->GetStringValue(), true);
                    item.data = strNum.to_number<value_type>();
                } else if (arguments.at(0)->IsDouble()) {
                    item.data = (value_type)arguments.at(0)->GetDoubleValue();
                }
            }
        }
    protected:
        int64_t m_nID;
    };
    
    class DllStringAccessor : public JsV8Handler {
    public:
        typedef std::string value_type;
    public:
        DllStringAccessor() {
            m_nID = 0;
            value_type val = value_type();
            TypeItem item;
            item.data = val;
            item.nType = (JS_TYPE)TypeToInt<value_type>::value;;
            item.nAddr = (int32_t)0;
            item.m_fnRelease = amo::bind(&DllStringAccessor::release, this);
            m_nID = TypeMapManager::getInstance()->add(item);
        }
        
        virtual CefString name() {
            return typeid(value_type()).name();
        }
        
        virtual std::string id() {
            return amo::u8string::from_number(m_nID).str();
        }
        
        virtual void release() {
            return;
        }
        
        virtual void typeName(CefRefPtr<CefV8Value> object,
                              const CefV8ValueList& arguments,
                              CefRefPtr<CefV8Value>& retval,
                              CefString& except) {
            retval = CefV8Value::CreateString(name());
        }
        
        virtual void val(CefRefPtr<CefV8Value> object,
                         const CefV8ValueList& arguments,
                         CefRefPtr<CefV8Value>& retval,
                         CefString& except) {
            TypeItem& item = TypeMapManager::getInstance()->get(m_nID);
            
            if (arguments.size() == 0) {
                value_type value = boost::any_cast<value_type>(item.data);
                retval = CefV8Value::CreateString(value);
            } else {
                if (arguments.at(0)->IsString()) {
                    item.data = arguments.at(0)->GetStringValue();
                }
            }
        }
    protected:
        int64_t m_nID;
    };
    
    template<typename T>
    class DllNilAccessor : public JsV8Handler {
    public:
        typedef T value_type;
    public:
        DllNilAccessor() {
            m_nID = 0;
            value_type val = value_type();
            TypeItem item;
            item.data = val;
            item.nType = (JS_TYPE)TypeToInt<value_type>::value;
            item.nAddr = 0;
            item.m_fnRelease = amo::bind(&DllNilAccessor<T>::release, this);
            m_nID = TypeMapManager::getInstance()->add(item);
        }
        
        virtual CefString name() {
            return typeid(value_type()).name();
        }
        
        virtual std::string id() {
            return amo::u8string::from_number(m_nID).str();
        }
        virtual void release() {
            return;
        }
        
        virtual void typeName(CefRefPtr<CefV8Value> object,
                              const CefV8ValueList& arguments,
                              CefRefPtr<CefV8Value>& retval,
                              CefString& except) {
            retval = CefV8Value::CreateString(name());
        }
        
        virtual void val(CefRefPtr<CefV8Value> object,
                         const CefV8ValueList& arguments,
                         CefRefPtr<CefV8Value>& retval,
                         CefString& except) {
            TypeItem& item = TypeMapManager::getInstance()->get(m_nID);
            
            if (arguments.size() == 0) {
                retval = CefV8Value::CreateString("NIL");
            } else {
                amo::nil nil;
                item.data = nil;
            }
        }
    protected:
        int64_t m_nID;
    };
    
    
    template<typename T>
    class DllIntegerPointerAccessor : public JsV8Handler {
    public:
        typedef T value_type;
        typedef typename std::remove_pointer<value_type>::type real_type;
    public:
        DllIntegerPointerAccessor() {
            m_pVal = NULL;
            m_nID = 0;
            value_type val = NULL;
            TypeItem item;
            item.data = val;
            item.nType = (JS_TYPE)TypeToInt<value_type>::value;;
            item.nAddr = (int32_t)val;
            item.m_fnRelease = amo::bind(&DllIntegerPointerAccessor<T>::release,
                                         this);
            m_nID = TypeMapManager::getInstance()->add(item);
        }
        
        DllIntegerPointerAccessor(int64_t nID) {
            m_nID = nID;
        }
        virtual CefString name() {
            return typeid(value_type()).name();
        }
        
        virtual std::string id() {
            return amo::u8string::from_number(m_nID).str();
        }
        
        virtual void release() {
            return;
        }
        
        virtual void typeName(CefRefPtr<CefV8Value> object,
                              const CefV8ValueList& arguments,
                              CefRefPtr<CefV8Value>& retval,
                              CefString& except) {
            retval = CefV8Value::CreateString(name());
        }
        
        virtual void val(CefRefPtr<CefV8Value> object,
                         const CefV8ValueList& arguments,
                         CefRefPtr<CefV8Value>& retval,
                         CefString& except) {
            TypeItem& item = TypeMapManager::getInstance()->get(m_nID);
            
            if (arguments.size() == 0) {
                retval = CefV8Value::CreateInt(*boost::any_cast<value_type>(item.data));
            } else if (arguments.size() == 2) {
                item.data = (value_type)arguments.at(0)->GetIntValue();
            } else {
                if (arguments.at(0)->IsString()) {
                    amo::u8string strNum(arguments.at(0)->GetStringValue(), true);
                    item.data = new real_type(strNum.to_number<real_type>());
                } else if (arguments.at(0)->IsInt()) {
                    int value = arguments.at(0)->GetIntValue();
                    item.data = new real_type((real_type)value);
                }
            }
        }
    protected:
        int64_t m_nID;
        value_type m_pVal;
    };
    
    template<typename T>
    class DllValueAccessor { };
    
    
    // bool
    template<> class DllValueAccessor<bool> : public DllBoolAccessor {};
    // 整型
    
    template<> class DllValueAccessor<int8_t> : public DllIntegerAccessor<int8_t> {};
    template<> class DllValueAccessor<int16_t> : public DllIntegerAccessor<int16_t> {};
    template<> class DllValueAccessor<int32_t> : public DllIntegerAccessor<int32_t> {};
    template<> class DllValueAccessor<int64_t> : public DllIntegerAccessor<int64_t> {};
    
    
    //template<> class DllValueAccessor<uint> : public DllNumberAccessor < uint > {};
    template<> class DllValueAccessor<uint8_t> : public DllIntegerAccessor < uint8_t > {};
    template<> class DllValueAccessor<uint16_t> : public DllIntegerAccessor < uint16_t > {};
    template<> class DllValueAccessor<uint32_t> : public DllIntegerAccessor < uint32_t > {};
    template<> class DllValueAccessor<uint64_t> : public DllIntegerAccessor < uint64_t > {};
    
    template<> class DllValueAccessor<char> : public DllIntegerAccessor<char> {};
    template<> class DllValueAccessor<long> : public DllIntegerAccessor<long> {};
    template<> class DllValueAccessor<unsigned long> : public DllIntegerAccessor < unsigned long > {};
    
    // 浮点型
    template<> class DllValueAccessor<float> : public DllFloatAccessor<float> {};
    template<> class DllValueAccessor<double> : public DllFloatAccessor<double> {};
    template<> class DllValueAccessor<long double> : public DllFloatAccessor<long double> {};
    
    // string
    template<> class DllValueAccessor<std::string> : public DllStringAccessor {};
    
    // void 类型
    template<> class DllValueAccessor<amo::nil> : public DllNilAccessor < amo::nil > {};
    
    // 整型指针
    template<> class DllValueAccessor<int8_t*> : public DllIntegerPointerAccessor < int8_t* > {};
    template<> class DllValueAccessor<int16_t*> : public DllIntegerPointerAccessor < int16_t* > {};
    template<> class DllValueAccessor<int32_t*> : public DllIntegerPointerAccessor < int32_t* > {};
    template<> class DllValueAccessor<int64_t*> : public DllIntegerPointerAccessor < int64_t* > {};
    
}

#endif // AMO_DLLVALUEACCESSOR_H__
