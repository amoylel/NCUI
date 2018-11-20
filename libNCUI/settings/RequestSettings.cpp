#include "stdAfx.h"
#include "settings/RequestSettings.h"

namespace amo {

    RequestSettings::RequestSettings() {
        initDefaultRequestSettings();
    }
    
    void RequestSettings::initDefaultRequestSettings() {
        DEFAULT_ARGS_SETTINGS(type, "GET");
        DEFAULT_ARGS_SETTINGS(url, "chrome://version");
        files.to_array();
        
    }
    
    void RequestSettings::afterUpdateArgsSettings() {
        STRING_ARGS_SETTING(type);
        STRING_ARGS_SETTING(url);
        JSON_ARGS_SETTING(data);
        JSON_ARGS_SETTING(header);
        JSON_ARGS_SETTING(files);
    }
    
    amo::u8json RequestSettings::toJson() {
        UPDATE_ARGS_SETTINGS(type);
        UPDATE_ARGS_SETTINGS(url);
        UPDATE_ARGS_SETTINGS(data);
        UPDATE_ARGS_SETTINGS(header);
        UPDATE_ARGS_SETTINGS(files);
        return BasicSettings::toJson();
    }
    
    std::string RequestSettings::makeUrlWithArgs() {
        std::vector<std::string> keys = data.keys();
        std::string strURL = url;
        
        if (keys.empty()) {
            return strURL;
        } else {
            strURL += "?";
        }
        
        for (size_t i = 0; i < keys.size(); ++i) {
            auto & p = keys[i];
            std::string args = p;
            args += "=";
            
            if (data.is_bool(p)) {
                args += std::to_string(data.getBool(p));
            } else if (data.is_string(p)) {
                args += data.getString(p);
            } else if (data.is_int(p)) {
                args += std::to_string(data.getInt(p));
            } else if (data.is_int64(p)) {
                args += std::to_string(data.getInt64(p));
            } else if (data.is_uint(p)) {
                args += std::to_string(data.getUint(p));
            } else if (data.is_uint64(p)) {
                args += std::to_string(data.getUint64(p));
            } else if (data.is_double(p)) {
                args += std::to_string(data.getDouble(p));
            } else if (data.is_number(p)) {
                args += std::to_string(data.getUint64(p));
            }
            
            if (i < keys.size() - 1) {
                args += "&";
            }
            
            strURL += args;
        }
        
        return strURL;
    }
    
    std::vector<std::pair<std::string, std::string> >
    RequestSettings::getHeaderMap() {
        std::vector<std::pair<std::string, std::string> > vec;
        std::vector<std::string> keys = header.keys();
        
        for (size_t i = 0; i < keys.size(); ++i) {
            auto & p = keys[i];
            vec.push_back({ p, header.getString(p) });
        }
        
        return vec;
    }
    
    std::vector<amo::u8string> RequestSettings::getFiles() {
        std::vector<amo::u8string> vec;
        
        if (!files.is_array()) {
            return vec;
        }
        
        std::vector<amo::u8json> arr = files.to_array();
        
        for (auto& p : arr) {
            vec.push_back(amo::u8string(p.get<std::string>(), true));
        }
        
        return vec;
    }
    
}

