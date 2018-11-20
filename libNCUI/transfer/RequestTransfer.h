// Created by amoylel on 07/20/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_REQUESTTRANSFER_H__
#define AMO_REQUESTTRANSFER_H__

#include <amo/singleton.hpp>

#include "handler/CefHeader.hpp"
#include "transfer/ClassTransfer.hpp"


namespace amo {

    class RequestTransfer
        : public ClassTransfer
        , public amo::singleton < RequestTransfer > {
    public:
        RequestTransfer();
        RequestTransfer(CefRefPtr<CefRequest> pRequest);
        
        Any IsReadOnly(IPCMessage::SmartType msg);
        Any GetURL(IPCMessage::SmartType msg);
        Any SetURL(IPCMessage::SmartType msg);
        Any GetMethod(IPCMessage::SmartType msg);
        Any SetMethod(IPCMessage::SmartType msg);
        Any SetReferrer(IPCMessage::SmartType msg);
        Any GetReferrerURL(IPCMessage::SmartType msg);
        Any GetReferrerPolicy(IPCMessage::SmartType msg);
        Any GetPostData(IPCMessage::SmartType msg);
        Any SetPostData(IPCMessage::SmartType msg);
        Any GetHeaderMap(IPCMessage::SmartType msg);
        Any SetHeaderMap(IPCMessage::SmartType msg);
        Any Set(IPCMessage::SmartType msg);
        Any GetFlags(IPCMessage::SmartType msg);
        Any SetFlags(IPCMessage::SmartType msg);
        Any GetFirstPartyForCookies(IPCMessage::SmartType msg);
        Any SetFirstPartyForCookies(IPCMessage::SmartType msg);
        Any GetResourceType(IPCMessage::SmartType msg);
        Any GetTransitionType(IPCMessage::SmartType msg);
        Any GetIdentifier(IPCMessage::SmartType msg);
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(RequestTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(IsReadOnly, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetURL, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(SetURL, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetMethod, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(SetMethod, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(SetReferrer, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetReferrerURL, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetReferrerPolicy, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetReferrerPolicy, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetPostData, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(SetPostData, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetHeaderMap, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(SetHeaderMap, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(Set, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetFlags, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(SetFlags, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetFirstPartyForCookies, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(SetFirstPartyForCookies, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetResourceType, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetTransitionType, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetIdentifier, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_END()
    private:
        CefRefPtr<CefRequest> m_pRequest;
    };
}

#endif // AMO_REQUESTTRANSFER_H__
