// ncui.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ncui.h"
#include "node.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>

#include "handler/NodeProcessHandler.h"




XNODE_API void setMessageQueue(const std::string& strName) {
    amo::NodeProcessHandler::setMessageQueue(strName);
}
