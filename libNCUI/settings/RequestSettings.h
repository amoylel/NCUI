// Created by amoylel on 11/26/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_REQUESTSETTINGS_H__
#define AMO_REQUESTSETTINGS_H__

#include <vector>

#include <amo/json.hpp>
#include <amo/string.hpp>
#include "settings/BasicSettings.h"

namespace amo {

    /**
     * @class	HTTP请求参数
     *
     * @id settingsRequestSettings
     *
     * @chapter settings
     *
     * @brief	手动HTTP请求时需要用到的参数.
     *
     */
    
    class RequestSettings : public BasicSettings {
    public:
        RequestSettings();
        
        /*!
         * @fn	void RequestSettings::InitDefaultRequestSettings();
         *
         * @ignore
         *
         * @brief	Init default request settings.
         */
        
        void initDefaultRequestSettings();
        virtual void afterUpdateArgsSettings() override;
        virtual amo::u8json toJson() override;
        
        std::string makeUrlWithArgs();
        std::vector<std::pair<std::string, std::string> > getHeaderMap();
        std::vector<amo::u8string> getFiles();
    public:
    
        /*! @var #String=GET type HTPP请求类型，POST 或者 GET */
        std::string type;
        
        /*! @var #String	url HTTP请求地址. */
        std::string url;
        
        /*! @var #JsonObject data	要发送的数据. */
        amo::u8json data;
        
        /** @var #JsonObject header	HTTP请求的头部参数. */
        amo::u8json header;
        
        /** @var #Array files 	要上传的文件列表，文件以字符串表示. */
        amo::u8json files;
    };
}


#endif // AMO_REQUESTSETTINGS_H__
