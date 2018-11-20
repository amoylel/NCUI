// node_runner.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "node_runner.h"

#include <node.h>



extern "C" NODE_RUNNER_API int Start(int argc, char** argv) {
    return node::Start(argc, argv);
}

