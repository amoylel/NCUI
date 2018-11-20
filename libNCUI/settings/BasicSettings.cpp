#include "stdafx.h"
#include "BasicSettings.h"

namespace amo {

    BasicSettings::BasicSettings() {
    
    }
    
    bool BasicSettings::updateArgsSettings(const std::string& json_string) {
        amo::u8json json1(json_string);
        return updateArgsSettings(json1);
        /*
        if (!json1.is_valid()) {
        return false;
        }
        
        m_json.join(json1);
        afterUpdateArgsSettings();
        amo::u8json json2 = m_json;
        m_json.join(json2);
        return true;*/
    }
    
    bool BasicSettings::updateArgsSettings(amo::u8json& config) {
        if (!config.is_valid()) {
            return false;
        }
        
        std::string ss = config.to_string();
        
        if (!config.empty()) {
            settings.join(config);
        }
        
        afterUpdateArgsSettings();
        
        if (m_fnUpdateArgsCallback) {
            m_fnUpdateArgsCallback(this);
        }
        
        amo::u8json json2 = settings;
        settings.join(json2);
        
        
        return true;
    }
    
    void BasicSettings::afterUpdateArgsSettings() {
        return ;
    }
    
    std::string BasicSettings::getArgsSettings() const {
        return settings.to_string();
    }
    
    std::string BasicSettings::toAbsolutePath(const std::string& str) {
        amo::u8string strClassList(str, true);
        std::regex reg("^%\\w+%");
        std::smatch m;
        std::vector<std::string> class_list;
        
        if (std::regex_search(str, m, reg)) {
            for (auto x = m.begin(); x != m.end(); x++) {
                amo::u8string key(x->str(), true);
                key.replace(amo::u8string("%", true), amo::u8string("", true));
                
                std::string ss = settings.getString(key);
                
                if (ss.empty()) {
                    return str;
                }
                
                amo::u8string subStr = strClassList.substr(x->str().size());
                subStr.trim_left(amo::u8string("\\", true));
                subStr.trim_left(amo::u8string("/", true));
                
                amo::u8path subPath(subStr);
                amo::u8path p(amo::u8string(ss, true));
                
                p.append(subStr.str());
                return toAbsolutePath(amo::u8string(p.raw_string(), true).to_utf8());
            }
        }
        
        return str;
        
    }
    
    std::function<void(BasicSettings*)> BasicSettings::getUpdateArgsCallback()
    const {
        return m_fnUpdateArgsCallback;
    }
    
    void BasicSettings::setUpdateArgsCallback(std::function<void(BasicSettings*)>
            val) {
        m_fnUpdateArgsCallback = val;
    }
    
}

