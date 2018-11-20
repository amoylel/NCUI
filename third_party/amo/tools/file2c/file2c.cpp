// file2c.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <amo/json.hpp>
#include <amo/timer.hpp>
#include <amo/filetoc.hpp>
#include <amo/program_options.hpp>

int main(int argc, char** argv) {

    amo::timer t;
    amo::log::initialize(true, true);
    amo::path::set_work_path_to_app_path();
    
    std::string config = "config.json";
    
    if (argc > 1) {
        config = argv[1];
        amo::path::set_work_path(config);
    }
    
    
    
    amo::string_utils::trim_left(config, "\t \"");
    amo::string_utils::trim_right(config, "\t \"");
    
    
    amo::json settings = amo::json::from_file(config);
    
    if (!settings.is_valid() || !settings.get_error_msg().empty()) {
        $cerr("配置文件错误: {}", settings.get_error_msg());
        return -1;
    }
    
    amo::file2c ff(settings);
    ff.build();
    $cdevel("用时：{}", t.elapsed());
    return 0;
}

