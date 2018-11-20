// Created by amoylel on 17/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef _JSONFMT_245FD2EA_783E_4475_89BA_685A78D83223_HPP__
#define _JSONFMT_245FD2EA_783E_4475_89BA_685A78D83223_HPP__

#include <string>
#include <amo/json.hpp>
#include <amo/string.hpp>
#include <amo/json_helper.hpp>

#include <amo/jsonfmt.hpp>
#include <amo/path.hpp>
#include <fstream>

namespace amo {


    class jsonfmt {
    public:
        class settings : public json_object {
        public:
        
            settings() {
                show = false;
            }
            
            void onFromJson(amo::json& json)override {
                amo::path p(input);
                
                if (output.empty()) {
                    output = input;
                }
                
                
            }
            
            AMO_ENTITY_ARGS_GET_BEGIN(settings)
            AMO_ENTITY_ARGS_GET(show)
            
            AMO_ENTITY_ARGS_GET(input)
            AMO_ENTITY_ARGS_GET(output)
            AMO_ENTITY_ARGS_GET_END()
            
            AMO_ENTITY_ARGS_SET_BEGIN(settings)
            
            AMO_ENTITY_ARGS_SET(show)
            
            AMO_ENTITY_ARGS_SET(input)
            AMO_ENTITY_ARGS_SET(output)
            AMO_ENTITY_ARGS_SET_END()
            
            
            
        public:
        
            std::string input;		// 输入文件
            std::string output;		// 输出文件，如果不填默认为写回输入文件
            bool show;				// 格式化后是否显示
        };
    public:
        jsonfmt(const amo::json& str)  {
            m_settings = jsonfmt::settings::fromJson(str);
        }
        
        
        bool format() {
            amo::json json = amo::json::from_file(m_settings->input);
            
            if (!json.is_valid()) {
                std::cout << "invalid json file: "
                          << m_settings->input
                          << ", error msg: "
                          << json.get_error_msg()
                          << std::endl;
                return false;
            }
            
            std::string  str = json.format(4);
            std::ofstream ofs(m_settings->output, std::ios::out | std::ios::binary);
            ofs << str;
            ofs.close();
            
            if (m_settings->show) {
                std::cout << str << std::endl;
            }
            
            return true;
        }
        
    private:
        std::shared_ptr<settings> m_settings;
    };
}



#endif //_JSONFMT_245FD2EA_783E_4475_89BA_685A78D83223_HPP__