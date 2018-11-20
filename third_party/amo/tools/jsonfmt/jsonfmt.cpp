// jsonfmt.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>
#include <amo/json.hpp>

#include <amo/jsonfmt.hpp>
#include <amo/program_options.hpp>

int main(int argc, char** argv) {

    amo::program_options options;
    options.p().add("input", -1);
    options.add_options()
    ("input,i", amo::po::value<std::string>(), "输入文件")
    ("version,v", "当前程序版本号")
    ("output,o", amo::po::value<std::string>(), "输出文件")
    ("show,s", amo::po::value<bool>()->default_value(true), "格式化后显示");
    
    
    
    
    
    if (!options.parse_command_line(argc, argv)) {
        std::cout << "无效命令" << std::endl;
        std::cout << options.desc() << std::endl;
        return -1;
    }
    
    auto vm = options.variables_map();
    
    if (vm.count("help")) {
        std::cout << options.desc() << std::endl;
        return 0;
    }
    
    if (vm.count("version")) {
        std::cout << "1.0.0.0" << std::endl;
        return 0;
    }
    
    
    if (vm.count("input") != 1) {
        std::cout << "请输入需要格式化的JSON文件" << std::endl;
        std::cout << options.desc() << std::endl;
        return -1;
    }
    
    
    options.to_json();
    amo::jsonfmt(options.to_json()).format();
    system("pause");
    return 0;
}

