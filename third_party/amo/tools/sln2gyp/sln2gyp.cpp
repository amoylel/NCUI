// sln2gyp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <amo/comment.hpp>
#include <amo/sln2gyp.hpp>
#include "tmpl/default_gyp.hpp"
#include <amo/timer.hpp>
#include <amo/path.hpp>

int main(int argc, char** argv) {


    amo::timer t;
    amo::log::initialize(true, true);
    amo::path::set_work_path_to_app_path();
    
    std::string config = "gyp.json";
    
    if (argc > 1) {
        config = argv[1];
    }
    
    config = "E:\\中文目录2\\SkycommAgent\\gyp.json";
    
    amo::string_utils::trim_left(config, "\t \"");
    amo::string_utils::trim_right(config, "\t \"");
    
    // 配置文件路径需要绝对路径
    amo::path config_path(config);
    config_path.absolute();
    
    if (!config_path.exists()) {
        $cerr("配置文件不存在。");
        return -1;
    }
    
    amo::json settings = amo::json::from_file(config);
    
    settings.put("config_path", config_path.to_string());
    
    
    if (!settings.is_valid() || !settings.get_error_msg().empty()) {
        $cerr("配置文件错误: {}", settings.get_error_msg());
        return -1;
    }
    
    amo::sln2gyp gyp(amo::sln2gyp::settings::fromJson(settings));
    gyp.build();
    
    system("pause");
    
    return 0;
}

