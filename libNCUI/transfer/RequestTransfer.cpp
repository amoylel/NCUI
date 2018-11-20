#include "stdafx.h"
#include "transfer/RequestTransfer.h"

namespace amo {

    RequestTransfer::RequestTransfer()
        : ClassTransfer("Request") {
        
    }
    
    RequestTransfer::RequestTransfer(CefRefPtr<CefRequest> pRequest)
        : ClassTransfer("Request")
        , m_pRequest(pRequest) {
        
    }
    
    Any RequestTransfer::IsReadOnly(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::GetURL(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::SetURL(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::GetMethod(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::SetMethod(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::SetReferrer(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::GetReferrerURL(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::GetReferrerPolicy(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::GetPostData(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::SetPostData(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::GetHeaderMap(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::SetHeaderMap(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::Set(IPCMessage::SmartType msg) {
        return Undefined();
        
    }
    
    Any RequestTransfer::GetFlags(IPCMessage::SmartType msg) {
        return Undefined();
        
    }
    
    Any RequestTransfer::SetFlags(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::GetFirstPartyForCookies(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::SetFirstPartyForCookies(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::GetResourceType(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::GetTransitionType(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any RequestTransfer::GetIdentifier(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
}

