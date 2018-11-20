#include "stdafx.h"
#include "transfer/URLRequestClientTransfer.h"
#include "handler/URLRequestClient.h"
#include "transfer/TransferMappingMgr.hpp"

namespace amo {

    URLRequestClientTransfer::URLRequestClientTransfer()
        : ClassTransfer("URLRequestClient") {
        
    }
    
    URLRequestClientTransfer::URLRequestClientTransfer(
        CefRefPtr<CefURLRequestClient> pURLRequestClient)
        : ClassTransfer("URLRequestClient")
        , m_pURLRequestClient(pURLRequestClient) {
        
    }
    
    Any URLRequestClientTransfer::onCreateClass(IPCMessage::SmartType msg) {
    
        CefRefPtr<URLRequestClient> pClient = new URLRequestClient();
        using MGR = TransferMappingMgr < URLRequestClientTransfer > ;
        return MGR::getInstance()->toSimplifiedJson(pClient);
        
    }
}

