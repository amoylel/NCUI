// Created by amoylel on 08/04/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_BINRESOURCEUPDATER_A2B15882_E519_4C02_B1F5_26C55FD00441_H__
#define AMO_BINRESOURCEUPDATER_A2B15882_E519_4C02_B1F5_26C55FD00441_H__

#include <windows.h>
#include <amo/file.hpp>
#include <amo/filestream.hpp>


namespace amo {
    class BinResourceUpdater {
    public:
        BinResourceUpdater(const std::string& exeName);
        
        
        bool updateZipResource(const std::string& zipname,
                               int resourceID = 131,
                               int lang_id = LANG_CHINESE);
    private:
    
        std::string m_filename; // utf8
    };
}

#endif // AMO_BINRESOURCEUPDATER_A2B15882_E519_4C02_B1F5_26C55FD00441_H__
