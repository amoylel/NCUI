#include "stdafx.h"
#include "node/NodeHandlerHelper.h"
#include "transfer/TransferMappingMgr.hpp"
#include "ipc/UIMessageEmitter.hpp"
#include "transfer/BrowserTransfer.h"
#include "transfer/FrameTransfer.h"

namespace amo {

    NodeHandlerHelper::NodeHandlerHelper() {
        m_nObjectID = 0;
    }
    
    
    std::shared_ptr<amo::UIMessageBroadcaster> NodeHandlerHelper::getBroadcaster() {
        std::shared_ptr<UIMessageBroadcaster> runner;
        runner.reset(new UIMessageBroadcaster(getObjectID()));
        return runner;
    }
    
    void NodeHandlerHelper::setObjectID(int64_t nID) {
        m_nObjectID = nID;
    }
    
    int64_t NodeHandlerHelper::getObjectID() const {
        return m_nObjectID;
    }
    
    std::shared_ptr<amo::BrowserTransfer> NodeHandlerHelper::getTransfer(CefRefPtr<CefBrowser> pBrowser) {
        return TransferMappingMgr<BrowserTransfer>::getInstance()->toTransfer(pBrowser);
    }
    
    amo::u8json NodeHandlerHelper::getJson(CefRefPtr<CefFrame> pFrame) {
        return getTransfer(pFrame)->getFuncMgr().toSimplifiedJson();
    }
    
    amo::u8json NodeHandlerHelper::getJson(CefRefPtr<CefBrowser> pBrowser) {
        return getTransfer(pBrowser)->getFuncMgr().toSimplifiedJson();
    }
    
    std::shared_ptr<amo::FrameTransfer> NodeHandlerHelper::getTransfer(CefRefPtr<CefFrame> pFrame) {
        return TransferMappingMgr<FrameTransfer>::getInstance()->toTransfer(pFrame);
    }
    
    
}

