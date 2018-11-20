// libResource.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "libResource.h"
#include <fstream>
#include <sstream>

#include <amo/path.hpp>

// 这是导出变量的一个示例
LIBRESOURCE_API int nlibResource = 0;

// 这是导出函数的一个示例。
LIBRESOURCE_API int fnlibResource(void) {
    return 42;
}



// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 libResource.h
ClibResource::ClibResource() {
    return;
}

LIBRESOURCE_API int readAsText(const char* path, char** retval) {

    amo::u8path p = amo::u8path::fullPathInAppDir("web");
    p.append(path);
    
    std::fstream ifs(p.generic_wstring(), std::ios::in | std::ios::binary);
    
    if (!ifs.is_open()) {
        return 0;
    }
    
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    
    static std::string str;
    str = buffer.str();
    *retval = (char*)str.c_str();
    
    return str.size();
}