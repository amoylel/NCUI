// Created by amoylel on 31/05/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef LIBNCUI_DLLFILEMANAGER_208B68C3_93F8_486F_A764_FC0663B6C003_H__
#define LIBNCUI_DLLFILEMANAGER_208B68C3_93F8_486F_A764_FC0663B6C003_H__







#include <memory>
#include <unordered_map>

#include <amo/loader.hpp>
#include <amo/singleton.hpp>


namespace amo {
    class DllFileManager : public amo::singleton<DllFileManager> {
    public:
        DllFileManager();
        std::shared_ptr<amo::loader> get(const amo::u8string& u8Path);
        bool add(const amo::u8string& strPath, std::shared_ptr<amo::loader> zf);
        
        
    private:
        std::unordered_map<amo::u8string, std::shared_ptr<amo::loader> > m_map;
    };
}



#endif //LIBNCUI_DLLFILEMANAGER_208B68C3_93F8_486F_A764_FC0663B6C003_H__