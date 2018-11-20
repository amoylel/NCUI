// Created by amoylel on 07/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_SLN2GYP_B8184381_1228_4EBE_AA05_54CBACDAC6AC_HPP__
#define AMO_SLN2GYP_B8184381_1228_4EBE_AA05_54CBACDAC6AC_HPP__

#include <memory>
#include <string>

#include <amo/json.hpp>
#include <amo/json_helper.hpp>
#include <amo/filestream.hpp>
#include <amo/path.hpp>
#include <amo/logger.hpp>
#include <amo/file.hpp>
#include <amo/string.hpp>
#include "../tools/sln2gyp/tmpl/default_gyp.hpp"
#include <amo/sln2gyp/vsproject.hpp>



namespace amo {

    class sln2gyp {
    public:
        class settings : public json_object {
        public:
        
            settings() {
                include_dirs.set_array();
                absolute_include_dirs.set_array();
                exclude_projects.set_array();
            }
            
            void onFromJson(amo::json& json)override {
                amo::path p(sln);
                
                if (p.is_relative()) {
                    sln = amo::path(config_path).parent().append(p).to_string();
                }
                
            }
            
            AMO_ENTITY_ARGS_GET_BEGIN(settings)
            AMO_ENTITY_ARGS_GET(sln)
            AMO_ENTITY_ARGS_GET(mode)
            AMO_ENTITY_ARGS_GET(include_dirs)
            AMO_ENTITY_ARGS_GET(absolute_include_dirs)
            AMO_ENTITY_ARGS_GET(config_path)
            AMO_ENTITY_ARGS_GET(exclude_projects)
            AMO_ENTITY_ARGS_GET_END()
            
            AMO_ENTITY_ARGS_SET_BEGIN(settings)
            AMO_ENTITY_ARGS_SET(sln)
            AMO_ENTITY_ARGS_SET(mode)
            AMO_ENTITY_ARGS_SET(include_dirs)
            AMO_ENTITY_ARGS_SET(absolute_include_dirs)
            
            AMO_ENTITY_ARGS_SET(config_path)
            AMO_ENTITY_ARGS_SET(exclude_projects)
            AMO_ENTITY_ARGS_SET_END()
            
            
            
        public:
            std::string sln;				//  vs 解决方案路径
            int mode;						//  0 创建 1 更新
            amo::json include_dirs;				//  文件包含目录，相对sln所在目录
            amo::json absolute_include_dirs;	// 文件包含目录，绝对目录
            std::string config_path;		// 配置文件路径, 绝对路径
            amo::json exclude_projects;		// 排除的项目
        };
        
        class project : public json_object {
        public:
            AMO_ENTITY_ARGS_GET_BEGIN(project)
            AMO_ENTITY_ARGS_GET(name)
            AMO_ENTITY_ARGS_GET(path)
            AMO_ENTITY_ARGS_GET(relative_path)
            
            AMO_ENTITY_ARGS_GET_END()
            
            AMO_ENTITY_ARGS_SET_BEGIN(project)
            AMO_ENTITY_ARGS_SET(name)
            AMO_ENTITY_ARGS_SET(path)
            AMO_ENTITY_ARGS_SET(relative_path)
            
            AMO_ENTITY_ARGS_SET_END()
            
            std::string name;			// 项目名
            std::string path;			// 项目路径, 绝对路径
            std::string relative_path;	// 项目路径，相对于解决方案的路径
            
        };
    public:
    
        sln2gyp(std::shared_ptr<sln2gyp::settings> settings_): m_settings(settings_) {
        
        }
        
        // 解析 SLN文件, 获取所有的vcxproj文件
        void analyze() {
            amo::path sln_path(m_settings->sln);
            amo::path parent_path = sln_path.parent();
            
            amo::file fi(sln_path);
            std::string str = fi.get_filestream()->read_all();
            
            std::regex reg("=.+?\.vcxproj\"");
            std::smatch m;
            
            while (std::regex_search(str, m, reg)) {
                std::string mstr = m[0].str();
                amo::string_utils::trim_left(mstr, "=");
                amo::string_utils::replace(mstr, "\"", "");
                amo::string_utils::replace(mstr, " ", "");
                std::vector<std::string> vec =  amo::string_utils::split(mstr, "\,");
                
                if (vec.size() == 2) {
                    std::shared_ptr<sln2gyp::project> project(new sln2gyp::project());
                    project->name = vec[0];
                    project->path = parent_path.append_c(vec[1]).string();
                    project->relative_path = vec[1];
                    m_projects.push_back(project);
                    
                    $cdevel("found project:{}-->{}", project->name, project->path);
                } else {
                    $cwarn("unsupported project:{}", mstr);
                }
                
                str = str.substr(m.prefix().length() + m[0].length());
                
            }
        }
        
        void build() {
            analyze();
            update_project_gyp();
            update_project_common_gyp();
            update_project_gypi();
            update_sln_gyp();
        }
        
        void update() {
            update_project_gypi();
        }
        
        
        bool is_exclude_project(std::shared_ptr<amo::sln2gyp::project> project) {
            std::vector<amo::json> vec = m_settings->exclude_projects.to_array();
            amo::path p2(project->path);
            
            for (auto& p : vec) {
                if (p.is_string()) {
                    if (project->name == p.get<std::string>()) {
                        return true;
                    }
                } else if (p.is_object()) {
                    std::string name = p.getString("name");
                    std::string path = p.getString("path");
                    
                    if (project->name == name && project->path == path) {
                        return true;
                    }
                } else {
                    continue;
                }
            }
            
            return false;
        }
        
        void update_sln_gyp() {
            amo::cdevel << amo::blank_line << amo::endl;
            
            std::string project_lines;
            amo::string PROJECT_LINE_TEMPLATE("\"./{project_line}\"", false);
            
            for (size_t i = 0; i < m_projects.size(); ++i) {
            
            
            
                auto& p = m_projects[i];
                
                if (is_exclude_project(p)) {
                    $cdevel("exclude project:{}", p->path);
                    continue;
                }
                
                amo::path  gyp_path = amo::path(p->relative_path);
                gyp_path.rename_extension(".gyp");
                
                project_lines += "\"./" + gyp_path.canonicalize(false).to_string() + "\"";
                
                if (i < m_projects.size() - 1) {
                    project_lines += ",";
                }
                
                project_lines += "\n        ";
            }
            
            amo::json json;
            
            json.put("project_lines", project_lines);
            
            std::string out_str = amo::string(::sln2gyp_tmpl::SLN_GYP_TEMPLATE, false).format(
                                      json);
                                      
            amo::path output_path(m_settings->sln);
            output_path.rename_extension(".gyp");
            
            std::ofstream ofs(output_path, std::ios::out | std::ios::binary);
            ofs << out_str;
            ofs.close();
            return ;
        }
        
        void update_project_gyp() {
        
            amo::cdevel << amo::blank_line << amo::endl;
            
            for (auto& p : m_projects) {
                make_project_gyp(p);
            }
            
            
        }
        
        // 生成项目的gyp文件
        bool make_project_gyp(std::shared_ptr<sln2gyp::project> project) {
        
            amo::path path(project->path);
            amo::path slnDir = amo::path(m_settings->sln).parent();
            
            amo::file fi(path);
            std::string str = fi.get_filestream()->read_all();
            
            amo::vsproject prj(project->path);
            
            std::regex reg("<ConfigurationType>.+?</ConfigurationType>");
            std::smatch m;
            std::vector<std::string> sources;
            
            while (std::regex_search(str, m, reg)) {
                std::string mstr = m[0].str();
                amo::string_utils::trim_left(mstr, "<ClCompile Include=");
                amo::string_utils::trim_right(mstr, " />");
                amo::string_utils::replace(mstr, "\"", "");
                sources.push_back(mstr);
                str = str.substr(m.prefix().length() + m[0].length());
                
            }
            
            std::string source_lines;
            
            for (size_t i = 0; i < sources.size(); ++i) {
                auto& p = sources[i];
                source_lines += "\"" + p + "\"";
                
                if (i < sources.size() - 1) {
                    source_lines += ",";
                }
                
                source_lines += "\n            ";
            }
            
            std::string dependencies;
            
            {
                std::vector<std::string> vec = prj.get_depends();
                
                for (auto& p : vec) {
                
                    amo::path dpath(p);
                    dpath = path.parent().append_c(p);
                    dpath.absolute();
                    
                    $cdevel("found depend project:{}", dpath.to_string());
                    
                    dpath.rename_extension(".gyp");
                    dpath.relative_path_to(slnDir);
                    std::string strPath = dpath.to_string();
                    
                    if (amo::string_utils::start_with(strPath, "./")) {
                        strPath = strPath.substr(2);
                    }
                    
                    dependencies += "'<(DEPTH)/";
                    dependencies += strPath;
                    dependencies += ":";
                    dependencies += dpath.stem();
                    dependencies += "',\n                ";
                }
            }
            
            std::string type = prj.get_type();
            std::string product_name = project->name;
            
            if (amo::string_utils::begin_with(product_name, "lib")) {
                if (type == "shared_library") {
                    product_name = product_name.substr(3);
                }
            }
            
            amo::json json;
            json.put("name", project->name);
            json.put("source_lines", source_lines);
            json.put("project_name", project->name);
            json.put("target_name", project->name);
            json.put("product_name", product_name);
            json.put("type", type);
            json.put("dependencies", dependencies);
            
            std::string out_str = amo::string(::sln2gyp_tmpl::PROJECT_GYP_TEMPLATE, false).format(
                                      json);
            amo::path output_path(project->path);
            output_path = output_path.parent();
            output_path.append(project->name);
            output_path.add_extension(".gyp");
            std::ofstream ofs(output_path, std::ios::out | std::ios::binary);
            ofs << out_str;
            ofs.close();
            
            $cdevel("write GYP file:{}", output_path.to_string());
            return true;
        }
        
        // 生成通用配置文件
        void update_project_common_gyp() {
            amo::cdevel << amo::blank_line << amo::endl;
            
            for (auto& p : m_projects) {
                make_project_common_gyp(p);
            }
            
            
        }
        
        // 生成项目的gyp文件
        bool make_project_common_gyp(std::shared_ptr<sln2gyp::project> project) {
        
            amo::path path(project->path);
            amo::file fi(path);
            std::string str = fi.get_filestream()->read_all();
            
            amo::path sln_dir = amo::path(m_settings->sln).parent();
            amo::path project_dir = amo::path(project->path).parent();
            
            // 当前项目路径回到解决方案路径所需要的路径
            std::string SolutionDir = sln_dir.relative_path_to_copy(
                                          project_dir).canonicalize(false).to_string();
            amo::string_utils::trim_right(SolutionDir, " \t/");
            SolutionDir += "/";
            
            std::string ProjectDir = "./";
            
            
            amo::json json;
            
            std::string include_dirs;
            
            std::vector<amo::json> vec = m_settings->include_dirs.to_array();
            
            
            for (auto& p : vec) {
                include_dirs += "'" + p.get<std::string>() + "',\n    ";
            }
            
            json.put("SolutionDir", SolutionDir);
            json.put("ProjectDir", ProjectDir);
            json.put("include_dirs", include_dirs);
            std::string out_str = amo::string(::sln2gyp_tmpl::COMMON_GYP_TEMPLATE, false).format(
                                      json);
            amo::path output_path(project->path);
            output_path = output_path.parent();
            output_path.append("common");
            output_path.add_extension(".gyp");
            std::ofstream ofs(output_path, std::ios::out | std::ios::binary);
            ofs << out_str;
            ofs.close();
            
            $cdevel("write common GYP file:{}", output_path.to_string());
            return true;
        }
        
        void update_project_gypi() {
            amo::cdevel << amo::blank_line << amo::endl;
            
            for (auto& p : m_projects) {
                make_project_gypi(p);
            }
            
            
        }
        
        // 通过项目路径生成gypi文件
        bool make_project_gypi(std::shared_ptr<sln2gyp::project> project) {
        
            amo::path path(project->path);
            amo::file fi(path);
            std::string str = fi.get_filestream()->read_all();
            
            std::regex reg("<ClCompile Include=.+? />");
            std::smatch m;
            std::vector<std::string> sources;
            
            while (std::regex_search(str, m, reg)) {
                std::string mstr = m[0].str();
                amo::string_utils::trim_left(mstr, "<ClCompile Include=");
                amo::string_utils::trim_right(mstr, " />");
                amo::string_utils::replace(mstr, "\"", "");
                amo::string_utils::replace(mstr, "\\", "/");
                sources.push_back(mstr);
                str = str.substr(m.prefix().length() + m[0].length());
                
            }
            
            std::string source_lines;
            
            for (size_t i = 0; i < sources.size(); ++i) {
                auto& p = sources[i];
                source_lines += "\"" + p + "\"";
                
                if (i < sources.size() - 1) {
                    source_lines += ",";
                }
                
                source_lines += "\n            ";
            }
            
            amo::json json;
            json.put("name", project->name);
            json.put("source_lines", source_lines);
            
            std::string out_str = amo::string(::sln2gyp_tmpl::GYPI_TEMPLATE, false).format(json);
            amo::path output_path(project->path);
            output_path = output_path.parent();
            output_path.append(project->name);
            output_path.add_extension(".gypi");
            std::ofstream ofs(output_path, std::ios::out | std::ios::binary);
            ofs << out_str;
            ofs.close();
            $cdevel("write GYPI file:{}", output_path.to_string());
            return true;
        }
    protected:
        std::shared_ptr<sln2gyp::settings> m_settings;
        
        std::vector<std::shared_ptr<sln2gyp::project> > m_projects;
    };
}

#endif // AMO_SLN2GYP_B8184381_1228_4EBE_AA05_54CBACDAC6AC_HPP__

