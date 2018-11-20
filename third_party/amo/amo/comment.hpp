// Created by amoylel on 07/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_DELTECOMMENT_C0526DCF_D3C1_446E_8272_F264A2C991FA_HPP__
#define AMO_DELTECOMMENT_C0526DCF_D3C1_446E_8272_F264A2C991FA_HPP__

#include <regex>
#include <amo/json_helper.hpp>
#include <amo/path.hpp>
#include <amo/filestream.hpp>
namespace amo {
    class comment {
    public:
        class settings : public json_object {
        public:
            settings() {
                rules.set_array();
                files.set_array();
                rules.push_back("^[ \t]*?#.*?$");
                rules.push_back("^[ \t]*?//.*?$");
                rules.push_back("^[ \t]*?/\\*[\\s\\S]*?\\*/");
            }
            AMO_ENTITY_ARGS_GET_BEGIN(settings)
            AMO_ENTITY_ARGS_GET(files)
            AMO_ENTITY_ARGS_GET(rules)
            AMO_ENTITY_ARGS_GET_END()
            
            AMO_ENTITY_ARGS_SET_BEGIN(settings)
            AMO_ENTITY_ARGS_SET(files)
            AMO_ENTITY_ARGS_SET(rules)
            AMO_ENTITY_ARGS_SET_END()
            amo::json files;					//  需要删除注释的文件列表
            amo::json rules;					//  注释删除规则
            
        };
    public:
        comment(std::shared_ptr<comment::settings> settings_) : m_settings(settings_) {
        
        }
        
        comment() : m_settings(new settings()) {
        
        }
        
        void remove_all() {
            if (m_settings->files.is_array()) {
                std::vector<amo::json> vec = m_settings->files.to_array();
                
                for (auto& p : vec) {
                    remove_comment(p.get<std::string>());
                }
            }
        }
        
        std::string remove_comment(const std::string& file_or_str,
                                   bool is_file = false) {
            std::string str = file_or_str;
            
            if (is_file) {
                amo::u8path path(file_or_str);
                
                if (path.exists()) {
                    str = amo::u8filestream(amo::u8string(file_or_str, true).to_utf8()).read_all();
                }
            }
            
            std::vector<amo::json> rules;
            
            if (m_settings->rules.is_array()) {
                rules = m_settings->rules.to_array();
            }
            
            for (auto& p : rules) {
                std::string ss = p.get<std::string>();
                std::regex reg(ss);
                str = std::regex_replace(str, reg, "");
            }
            
            return str;
        }
        
    protected:
        std::shared_ptr<comment::settings> m_settings;
    };
}

#endif // AMO_DELTECOMMENT_C0526DCF_D3C1_446E_8272_F264A2C991FA_HPP__

