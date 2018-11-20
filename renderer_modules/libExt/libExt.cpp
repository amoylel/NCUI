// libExt.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "libExt.h"
#include "transfer/UUIDTransfer.h"
#include "transfer/PathTransfer.h"
#include "sqlite/SqliteTransfer.h"
#include "rcedit/RceditTransfer.h"
#include "transfer/FileTransfer.hpp"
#include "transfer/Base64Transfer.h"
#include "transfer/MD5Transfer.h"
#include "transfer/InterProcessTransfer.h"
#include "transfer/ProcessTransfer.h"
#include "transfer/RegistryTransfer.h"
#include "transfer/DemoTransfer.h"



AMO_REGISTER_TRANSFER_BEGIN(LIBEXT_API)
AMO_CLASS_REGISTER(RceditTransfer)
AMO_CLASS_REGISTER(PathTransfer)
AMO_CLASS_REGISTER(SqliteTransfer)
AMO_CLASS_REGISTER(FileTransfer)
AMO_CLASS_REGISTER(MD5Transfer)
AMO_CLASS_REGISTER(UUIDTransfer)
AMO_CLASS_REGISTER(Base64Transfer)
AMO_CLASS_REGISTER(InterProcessTransfer)
AMO_CLASS_REGISTER(ProcessTransfer)
AMO_CLASS_REGISTER(RegistryTransfer)
AMO_CLASS_REGISTER(DemoTransfer)
AMO_REGISTER_TRANSFER_END()

//LIBEXT_API bool registerTransfer(std::shared_ptr<amo::TransferRegister> info) {
//
//
//    AMO_CLASS_REGISTER_BEGIN()
//    AMO_CLASS_REGISTER(FileTransfer)
//    AMO_CLASS_REGISTER(Base64Transfer)
//    AMO_CLASS_REGISTER(MD5Transfer)
//    AMO_CLASS_REGISTER(UUIDTransfer)
//    AMO_CLASS_REGISTER(PathTransfer)
//    AMO_CLASS_REGISTER(SqliteTransfer)
//    AMO_CLASS_REGISTER(RceditTransfer)
//    AMO_CLASS_REGISTER(Base64Transfer)
//    AMO_CLASS_REGISTER_END()
//}
//
//LIBEXT_API void unregisterTransfer(const std::string & strClass) {
//    amo::ClassTransfer::removeTransferByName(strClass);
//}