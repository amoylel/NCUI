#include "stdafx.h"
#include "module/dll/TypeMapManager.h"

namespace amo {

    TypeMapManager::TypeMapManager() {
    
        /*typedef signed char        int8_t;
        typedef short              int16_t;
        typedef int                int32_t;
        typedef long long          int64_t;
        typedef unsigned char      uint8_t;
        typedef unsigned short     uint16_t;
        typedef unsigned int       uint32_t;
        typedef unsigned long long uint64_t;
        long;
        unsigned long;
        
         */
        
        m_oJsTypeMap.insert(std::make_pair("INT", JS_INT));
        m_oJsTypeMap.insert(std::make_pair("UINT", JS_UINT));
        
        m_oJsTypeMap.insert(std::make_pair("INT8", JS_INT8));
        m_oJsTypeMap.insert(std::make_pair("INT16", JS_INT16));
        m_oJsTypeMap.insert(std::make_pair("INT32", JS_INT32));
        m_oJsTypeMap.insert(std::make_pair("INT64", JS_INT64));
        
        
        m_oJsTypeMap.insert(std::make_pair("UINT8", JS_UINT8));
        m_oJsTypeMap.insert(std::make_pair("UINT16", JS_UINT16));
        m_oJsTypeMap.insert(std::make_pair("UINT32", JS_UINT32));
        m_oJsTypeMap.insert(std::make_pair("UINT64", JS_UINT64));
        
        m_oJsTypeMap.insert(std::make_pair("DWORD", JS_UINT64));
        m_oJsTypeMap.insert(std::make_pair("NIL", JS_NIL));
        m_oJsTypeMap.insert(std::make_pair("VOID", JS_NIL));
        
        m_oJsTypeMap.insert(std::make_pair("INT_PTR", JS_INT_PTR));
    }
    
    int64_t TypeMapManager::add(const TypeItem& item) {
        int64_t id = amo::uid::generate_uid();
        m_oTypeMap[id] = item;
        return id;
    }
    
    void TypeMapManager::remove(int64_t nID) {
        auto iter = m_oTypeMap.find(nID);
        
        if (iter == m_oTypeMap.end()) {
            return;
        }
        
        TypeItem& item = iter->second;
        JS_TYPE nType = (JS_TYPE)item.nType;
        
        if (nType > EM_PTR) {
            item.release();
        }
        
        {
        
        }
        
        m_oTypeMap.erase(nID);
    }
    
    TypeItem& TypeMapManager::get(int64_t nID) {
        auto iter = m_oTypeMap.find(nID);
        
        if (iter == m_oTypeMap.end()) {
            return m_oInvalidItem;
        }
        
        return iter->second;
    }
    
    amo::TypeItem& TypeMapManager::get(std::string& nID) {
        return get(amo::u8string(nID, true).to_number<int64_t>());
    }
    
}

