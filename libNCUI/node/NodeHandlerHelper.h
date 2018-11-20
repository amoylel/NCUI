// Created by amoylel on 07/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODEHANDLERHELPER_H__
#define AMO_NODEHANDLERHELPER_H__


#include <memory>

#include <amo/stdint.hpp>
#include <amo/json.hpp>

#include "handler/CefHeader.hpp"




namespace amo {
    class UIMessageEmitter;
    class FrameTransfer;
    class BrowserTransfer;
    class UIMessageBroadcaster;
    
    class NodeHandlerHelper {
    public:
        NodeHandlerHelper();
        
        std::shared_ptr<UIMessageBroadcaster> getBroadcaster();
        void setObjectID(int64_t nID);
        int64_t getObjectID() const;
        
        std::shared_ptr<BrowserTransfer> getTransfer(CefRefPtr<CefBrowser> pBrowser);
        std::shared_ptr<FrameTransfer> getTransfer(CefRefPtr<CefFrame> pFrame);
        
        amo::u8json getJson(CefRefPtr<CefBrowser> pBrowser);
        amo::u8json getJson(CefRefPtr<CefFrame> pFrame);
        
    protected:
        int64_t m_nObjectID;
    };
}


#endif // AMO_NODEHANDLERHELPER_H__
