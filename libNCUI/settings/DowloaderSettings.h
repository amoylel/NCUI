// Created by amoylel on 08/19/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_DOWLOADERSETTINGS_H__
#define AMO_DOWLOADERSETTINGS_H__

#include <amo/json.hpp>
#include <amo/string.hpp>
#include "settings/BasicSettings.h"

namespace amo {
    class DownloaderSettings : public BasicSettings {
    public:
        DownloaderSettings();
        
        /*!
         * @fn	void DownloaderSettings::initDefaultDownloaderSettings();
         *
         * @brief	Init default downloader settings.
         */
        
        void initDefaultDownloaderSettings();
        virtual void afterUpdateArgsSettings() override;
        
    public:
        /*! @brief	下载文件的url. */
        std::string url;
        /*! @brief	文件保存的本地目录. */
        std::string file;
        /*! @brief	自动下载. */
        bool autoDownload;
        /*! @brief	强制替换已存在的文件. */
        bool forceReplace;
    };
}

#endif // AMO_DOWLOADERSETTINGS_H__

