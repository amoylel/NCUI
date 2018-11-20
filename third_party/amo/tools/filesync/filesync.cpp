// filesync.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <string>

#include <amo/timer.hpp>
#include <amo/logger.hpp>
#include <amo/string.hpp>

#include <amo/filesync.hpp>
#include <amo/program_options.hpp>

int main(int argc, char** argv) {

    amo::timer t;
    amo::log::initialize(true, true);
    amo::path::set_work_path_to_app_path();
    
    amo::program_options options;
    options.p().add("dirs", -1);
    options.add_options()
    ("config,c", amo::po::value<std::string>()->default_value(""),
     "配置文件")
    ("master", amo::po::value<std::string>()->default_value(""),
     "需要同步的文件夹主节点，如果不设置，那么dirs中的第一个文件夹会被设置为主节点")
    ("dirs", amo::po::value<std::vector<std::string> >(),
     "需要同步的文件夹列表")
    ("mode,m", amo::po::value<int>(),
     "同步类型，0同步，1暴力合并，默认为0")
    ("rules,r", amo::po::value<std::vector<std::string> >(),
     "文件触发规则，满足所给规则的文件会被触发事件，默认所有文件都会被触发事件")
    ("exclude_rules,e", amo::po::value<std::vector<std::string> >(),
     "文件排除规则，满足所给规则的文件不会被触发事件")
    ("builtin,b", amo::po::value<std::string>(),
     "使用内置模式，如果存在，那么将增加默认的排除规则，支持的内置模式有 dev")
    ;
    
    if (!options.parse_command_line(argc, argv)) {
        return -1;
    }
    
    
    amo::json settings;
    
    // 如果未输入参数，那么直接使用
    if (argc == 1) {
        std::string config = "filesync.json";
        
        if (!amo::path(config).exists()) {
            $cerr("默认配置文件filesync.json不存在");
            return -1;
        }
        
        settings = amo::json::from_file(config);
        
    }
    
    
    auto val = options.variables_map();
    
    // config
    std::string config = val["config"].as<std::string>();
    
    if (!config.empty()) {
        settings = amo::json::from_file(config);
    }
    
    if (!settings.is_valid() || !settings.get_error_msg().empty()) {
        $cerr("配置文件错误: {}", settings.get_error_msg());
        return -1;
    }
    
    // mode
    if (val.find("mode") != val.end()) {
        settings.put("mode", val["mode"].as<int>());
    }
    
    if (val.find("builtin") != val.end()) {
        std::string builtin = val["builtin"].as<std::string >();
        settings.put("builtin", builtin);
    }
    
    std::string master = val["master"].as<std::string>();
    
    if (!master.empty()) {
        settings.put("master", master);
    }
    
    if (val.find("dirs") != val.end()) {
        std::vector<std::string> dirs = val["dirs"].as<std::vector<std::string> >();
        
        if (!dirs.empty()) {
            amo::json arr;
            arr.set_array();
            
            if (settings.contains_key("dirs")) {
                arr = settings.getJson("dirs");
                
                if (!arr.is_array()) {
                    $cerr("要同步的文件夹列表必须为数组类型");
                    return -1;
                }
                
            }
            
            for (auto & p : dirs) {
                arr.push_back(p);
            }
            
            settings.put("dirs", arr);
        }
        
    }
    
    
    if (settings.contains_key("builtin")) {
        std::string builtin = settings.getString("builtin");
        
        if (builtin == "dev") {
        
            std::vector<std::string> rules;
            
            // 排除git目录
            rules.push_back("^.*?\.git$");
            rules.push_back("^.*?\.git/.*?$");
            // 排除vs目录
            rules.push_back("^.*?\.vs$");
            rules.push_back("^.*?\.vs/.*?$");
            // 排除DB文件
            rules.push_back("^.*?\.VC\.db$");
            // 排除obj文件
            rules.push_back("^.*?\.obj$");
            // 排除ipch文件
            rules.push_back("^.*?\.ipch");
            
            rules.push_back("^.*~.*$");
            
            amo::json arr;
            arr.set_array();
            
            if (settings.contains_key("exclude_rules")) {
                arr = settings.getJson("exclude_rules");
                
                if (!arr.is_array()) {
                    $cerr("要设置的排除规则必须为数组类型");
                    return -1;
                }
                
            }
            
            for (auto & p : rules) {
                arr.push_back(p);
            }
            
            settings.put("exclude_rules", arr);
        }
        
        
    }
    
    
    if (val.find("rules") != val.end()) {
        std::vector<std::string> dirs = val["rules"].as<std::vector<std::string> >();
        
        if (!dirs.empty()) {
            amo::json arr;
            arr.set_array();
            
            if (settings.contains_key("rules")) {
                arr = settings.getJson("rules");
                
                if (!arr.is_array()) {
                    $cerr("要设置的触发规则必须为数组类型");
                    return -1;
                }
                
            }
            
            for (auto & p : dirs) {
                arr.push_back(p);
            }
            
            settings.put("rules", arr);
        }
        
    }
    
    
    if (val.find("exclude_rules") != val.end()) {
        std::vector<std::string> dirs =
            val["exclude_rules"].as<std::vector<std::string> >();
            
        if (!dirs.empty()) {
            amo::json arr;
            arr.set_array();
            
            if (settings.contains_key("exclude_rules")) {
                arr = settings.getJson("exclude_rules");
                
                if (!arr.is_array()) {
                    $cerr("要设置的排除规则必须为数组类型");
                    return -1;
                }
                
            }
            
            for (auto & p : dirs) {
                arr.push_back(p);
            }
            
            settings.put("exclude_rules", arr);
        }
        
    }
    
    
    
    $cdevel("最终配置：{}", settings.to_string());
    
    amo::filesync ff(settings);
    
    if (settings.getInt("mode") == 0) {
        ff.start_sync();
    } else {
        ff.merge();
    }
    
    if (settings.getInt("mode") == 0) {
        while (true) {
            std::string str;
            std::cin >> str;
            
            if (str == "quit") {
                break;
            }
        }
    }
    
    $cdevel("用时：{}", t.elapsed());
    return 0;
}

