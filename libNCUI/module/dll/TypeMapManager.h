// Created by amoylel on 05/23/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_TYPEMAPMANAGER_H__
#define AMO_TYPEMAPMANAGER_H__



#include <amo/singleton.hpp>
#include <amo/stdint.hpp>
#include <amo/unordered_map.hpp>
#include <amo/any.hpp>
#include <amo/nil.hpp>




enum JS_TYPE {
    JS_INVALID_TYPE = 0,
    EM_PTR = (0x1 << 8),
    EM_CONST = (0x2 << 8),
    
    JS_VALUE_BEGIN = 1,
    JS_INT = JS_VALUE_BEGIN,
    JS_INT8,
    JS_INT16,
    JS_INT32,
    JS_INT64,
    
    JS_UINT,
    JS_UINT8,
    JS_UINT16,
    JS_UINT32,
    JS_UINT64,
    JS_NIL,
    JS_DOUBLE,
    JS_STRING,
    JS_BOOL,
    JS_VALUE_END,
    
    JS_POINTER_BEGIN = JS_INT | EM_PTR,
    JS_INT_PTR = JS_POINTER_BEGIN,
    JS_INT8_PTR = JS_INT8 | EM_PTR,
    JS_INT16_PTR = JS_INT16 | EM_PTR,
    JS_INT32_PTR = JS_INT32 | EM_PTR,
    JS_INT64_PTR = JS_INT64 | EM_PTR,
    JS_POINTER_END,
    
    JS_CONST_POINTER_BEGIN = JS_INT_PTR | EM_CONST,
    JS_CONST_INT_PTR = JS_CONST_POINTER_BEGIN,
    JS_CONST_INT8_PTR = JS_INT8_PTR | EM_CONST,
    JS_CONST_INT16_PTR = JS_INT16_PTR | EM_CONST,
    JS_CONST_INT32_PTR = JS_INT32_PTR | EM_CONST,
    JS_CONST_INT64_PTR = JS_INT64_PTR | EM_CONST,
    JS_CONST_POINTER_END,
    
};

template<typename T>
class TypeToInt {
public:
    const static int value = JS_INVALID_TYPE;
};

template<> class TypeToInt<int8_t> {
public:
    const static int value = JS_INT8;
};
template<> class TypeToInt<int16_t> {
public:
    const static int value = JS_INT16;
};
template<> class TypeToInt<int32_t> {
public:
    const static int value = JS_INT32;
};
template<> class TypeToInt<int64_t> {
public:
    const static int value = JS_INT64;
};

template<> class TypeToInt<uint8_t> {
public:
    const static int value = JS_UINT8;
};
template<> class TypeToInt<uint16_t> {
public:
    const static int value = JS_UINT16;
};
template<> class TypeToInt<uint32_t> {
public:
    const static int value = JS_UINT32;
};
template<> class TypeToInt<uint64_t> {
public:
    const static int value = JS_UINT64;
};

template<> class TypeToInt < double > {
public:
    const static int value = JS_DOUBLE;
};

template<> class TypeToInt < bool > {
public:
    const static int value = JS_BOOL;
};

template<> class TypeToInt < std::string > {
public:
    const static int value = JS_STRING;
};


template<> class TypeToInt<char> {
public:
    const static int value = JS_INT8;
};
template<> class TypeToInt<long> {
public:
    const static int value = JS_INT32;
};
template<> class TypeToInt<unsigned long> {
public:
    const static int value = JS_UINT64;
};

template<> class TypeToInt<amo::nil> {
public:
    const static int value = JS_NIL;
};


template<> class TypeToInt<int8_t*> {
public:
    const static int value = JS_INT8_PTR;
};
template<> class TypeToInt<int16_t*> {
public:
    const static int value = JS_INT16_PTR;
};
template<> class TypeToInt<int32_t*> {
public:
    const static int value = JS_INT32_PTR;
};
template<> class TypeToInt<int64_t*> {
public:
    const static int value = JS_INT64_PTR;
};

template<JS_TYPE> class IntToType;
template<> class IntToType <JS_NIL> {
public:
    typedef amo::nil type;
};

template<> class IntToType <JS_INT> {
public:
    typedef int type;
};

template<> class IntToType <JS_INT8> {
public:
    typedef int8_t type;
};

template<> class IntToType <JS_INT16> {
public:
    typedef int16_t type;
};

template<> class IntToType <JS_INT32> {
public:
    typedef int32_t type;
};


template<> class IntToType <JS_INT64> {
public:
    typedef int64_t type;
};

template<> class IntToType < JS_UINT > {
public:
    typedef uint32_t type;
};

template<> class IntToType < JS_BOOL > {
public:
    typedef bool type;
};

template<> class IntToType < JS_DOUBLE > {
public:
    typedef double type;
};

template<> class IntToType < JS_STRING > {
public:
    typedef std::string type;
};


namespace amo {

    class TypeItem {
    public:
        TypeItem() {
        }
        
        void release() {
            if (m_fnRelease) {
                m_fnRelease();
            }
        }
    public:
        boost::any data;
        JS_TYPE nType;
        int32_t nAddr;
        amo::function<void()> m_fnRelease;
    };
    
    class TypeMapManager : public amo::singleton < TypeMapManager > {
    public:
        TypeMapManager();
        
    public:
        JS_TYPE stringToJsType(const std::string& strType) {
            auto iter = m_oJsTypeMap.find(strType);
            
            if (iter == m_oJsTypeMap.end()) {
                return JS_INVALID_TYPE;
            }
            
            return iter->second;
        }
        
        int64_t add(const TypeItem& item);
        
        void remove(int64_t nID);
        
        TypeItem& get(int64_t nID);
        
        TypeItem& get(std::string& nID);
        
        
    public:
        std::unordered_map<int64_t, TypeItem> m_oTypeMap;
        std::unordered_map<std::string, JS_TYPE> m_oJsTypeMap;
        TypeItem m_oInvalidItem;																	//!< The invalid item
    };
}



#endif // AMO_TYPEMAPMANAGER_H__
