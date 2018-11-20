// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料

//#include "header/sqlite3.h"
#include "rcedit/RceditTransfer.h"
#include "rcedit/ResourceUpdater.h"



#include "sqlite/SqliteTransfer.h"
#include "sqlite/SqlMessageEmitter.hpp"

#include "sqlite/src/sqlite3pp.h"
#include "sqlite/src/sqlite3ppext.h"

#include "transfer/DemoTransfer.h"
#include "transfer/PathTransfer.h"
#include "transfer/UUIDTransfer.h"


// TODO:  在此处引用程序需要的其他头文件
