// Created by amoylel on 31/05/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef _ZIPFILEMANAGER_FC0545A3_D3E6_4654_A628_BEA17FA3ED8F_H__
#define _ZIPFILEMANAGER_FC0545A3_D3E6_4654_A628_BEA17FA3ED8F_H__





#include <amo/singleton.hpp>
#include "utility/libzippp.h"

namespace amo {
    class ZipFileManager : public amo::singleton<ZipFileManager> {
    public:
        ZipFileManager();
        std::shared_ptr<libzippp::ZipArchive> get(const amo::u8string& u8Path);
        bool add(const amo::u8string& strPath,
                 std::shared_ptr<libzippp::ZipArchive> zf);

		void setPassword(const amo::u8string& strPath, const std::string& password);
    private:
        std::unordered_map<amo::u8string, std::shared_ptr<libzippp::ZipArchive> > m_map;
		std::unordered_map<amo::u8string, std::string > m_passwords;
    };

	

}



#endif //_ZIPFILEMANAGER_FC0545A3_D3E6_4654_A628_BEA17FA3ED8F_H__