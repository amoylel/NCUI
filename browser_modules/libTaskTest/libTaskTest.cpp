// libTaskTest.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "libTaskTest.h"


#include "transfer/TaskTestTransfer.hpp"



AMO_REGISTER_TRANSFER_BEGIN(LIBTASKTEST_API)
AMO_CLASS_REGISTER(TaskTestTransfer)
AMO_REGISTER_TRANSFER_END()
