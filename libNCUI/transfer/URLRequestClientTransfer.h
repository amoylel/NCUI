// Created by amoylel on 07/20/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_URLREQUESTCLIENTTRANSFER_H__
#define AMO_URLREQUESTCLIENTTRANSFER_H__

#include <amo/singleton.hpp>

#include "handler/CefHeader.hpp"
#include "transfer/ClassTransfer.hpp"

namespace amo {

    class URLRequestClientTransfer
        : public ClassTransfer
        , public amo::singleton < URLRequestClientTransfer > {
    public:
        URLRequestClientTransfer();
        URLRequestClientTransfer(CefRefPtr<CefURLRequestClient> pURLRequestClient);
        
        virtual Any onCreateClass(IPCMessage::SmartType msg) override;
        
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(URLRequestClientTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    private:
        CefRefPtr<CefURLRequestClient> m_pURLRequestClient;
    };
}


#endif // AMO_URLREQUESTCLIENTTRANSFER_H__
