#include "stdafx.h"
#include "transfer/NetTransfer.h"
#include "transfer/ClassTransfer.hpp"
#include "transfer/DownloaderTransfer.h"
#include "transfer/URLRequestTransfer.h"

namespace amo {

    NetTransfer::NetTransfer()
        : ClassTransfer("net") {
        
    }
    
    Any NetTransfer::download(IPCMessage::SmartType msg) {
        auto pTransfer = ClassTransfer::getUniqueTransfer<DownloaderTransfer>();
        return pTransfer->onCreateClass(msg);
        
    }
    
    Any NetTransfer::request(IPCMessage::SmartType msg) {
        auto pTransfer = ClassTransfer::getUniqueTransfer<URLRequestTransfer>();
        return pTransfer->onCreateClass(msg);
    }
    
}

