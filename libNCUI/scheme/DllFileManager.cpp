#include "stdafx.h"
#include "scheme/DllFileManager.h"
#include <amo/loader.hpp>

namespace amo {

    DllFileManager::DllFileManager() {
    
    }
    
    std::shared_ptr<amo::loader> DllFileManager::get(const amo::u8string&
            strPath) {
        auto iter = m_map.find(strPath);
        
        if (iter != m_map.end()) {
            return iter->second;
        }
        
        std::shared_ptr<amo::loader> loader(new amo::loader(strPath));
        bool bOk = loader->load();
        
        if (bOk) {
            m_map[strPath] = loader;
            return loader;
        }
        
        return std::shared_ptr<amo::loader>();
    }
    
    bool DllFileManager::add(const amo::u8string& strPath,
                             std::shared_ptr<amo::loader> dll) {
                             
        m_map[strPath] = dll;
        return true;
    }
    
}
