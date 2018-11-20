// Created by amoylel on 07/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_VSPROJECT_7BF1C946_CEC5_4A06_A164_54A4F7B4E0A7_HPP__
#define AMO_VSPROJECT_7BF1C946_CEC5_4A06_A164_54A4F7B4E0A7_HPP__

#include <string>
#include <regex>

#include <amo/filestream.hpp>
#include <amo/path.hpp>
#include <amo/json_helper.hpp>

namespace amo {
    class vsproject {
    public:
        vsproject(const std::string& filename)
            : m_filename(filename) {
            amo::path p(m_filename);
            
            if (p.exists()) {
                m_project_content = amo::filestream(filename).read_all();
            }
        }
        
        std::string get_target_name() {
            return this->getConfigByType("ProjectName");
        }
        
        std::string get_type() {
            std::string s_type =  this->getConfigByType("ConfigurationType");
            
            if (s_type == "DynamicLibrary") {
                s_type = "shared_library";
            } else if (s_type == "StaticLibrary") {
                s_type = "static_library";
            } else if (s_type == "Application") {
                s_type = "executable";
            } else {
                s_type = "none";
            }
            
            return s_type;
        }
        
        std::vector<std::string> get_depends() {
            std::vector<std::string> vec =
                getConfigsByReg("<ProjectReference Include=\".+?\">");
            std::vector<std::string> retval;
            
            for (auto& p : vec) {
                std::string mstr = p;
                std::string head = std::string("<ProjectReference Include=\"");
                std::string tail = std::string("\">");
                mstr = mstr.substr(head.size());
                mstr = mstr.substr(0, mstr.size() - tail.size());
                retval.push_back(mstr);
            }
            
            return retval;
        }
        
        
        std::string operator[](const std::string& type) {
            return getConfigByType(type);
        }
        
        std::vector<std::string> getConfigsByReg(const std::string strReg) {
        
        
            std::regex reg(strReg);
            std::smatch m;
            std::vector<std::string> vec;
            std::string str = m_project_content;
            
            while (std::regex_search(str, m, reg)) {
                std::string mstr = m[0].str();
                vec.push_back(mstr);
                str = str.substr(m.prefix().length() + m[0].length());
                
            }
            
            return vec;
        }
        
        std::string getConfigByType(const std::string type) {
            std::string str = "<";
            str += type;
            str += ">.+?</";
            str += type;
            str += ">";
            std::regex reg(str);
            std::smatch m;
            std::vector<std::string> sources;
            
            
            while (std::regex_search(m_project_content, m, reg)) {
                std::string mstr = m[0].str();
                std::string head = std::string("<") + type + ">";
                std::string tail = std::string("</") + type + ">";
                mstr = mstr.substr(head.size());
                mstr = mstr.substr(0, mstr.size() - tail.size());
                /*  amo::string_utils::replace(mstr, std::string("<") + type + ">");
                  amo::string_utils::replace(mstr, std::string("</") + type + ">");*/
                return mstr;
            }
            
            return "";
        }
    protected:
        std::string m_filename;				// 项目路径
        std::string m_project_content;		// 文件内容
        
    };
}

#endif // AMO_VSPROJECT_7BF1C946_CEC5_4A06_A164_54A4F7B4E0A7_HPP__

