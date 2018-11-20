#include "stdafx.h"
#include "transfer/BrowserTransfer.h"
#include "transfer/TransferMappingMgr.hpp"
#include "transfer/FrameTransfer.h"
#include "transfer/BrowserHostTransfer.h"

namespace amo {

    BrowserTransfer::BrowserTransfer()
        : ClassTransfer("Browser") {
        
    }
    
    BrowserTransfer::BrowserTransfer(CefRefPtr<CefBrowser> pBrowser)
        : ClassTransfer("Browser")
        , m_pBrowser(pBrowser) {
        assert(m_pBrowser);
    }
    
    
    Any BrowserTransfer::GetHost(IPCMessage::SmartType msg) {
        auto manager = TransferMappingMgr<BrowserHostTransfer>::getInstance();
        return manager->toSimplifiedJson(m_pBrowser->GetHost());
    }
    
    Any BrowserTransfer::CanGoBack(IPCMessage::SmartType msg) {
        return m_pBrowser->CanGoBack();
    }
    
    Any BrowserTransfer::GoBack(IPCMessage::SmartType msg) {
        m_pBrowser->GoBack();
        return Undefined();
    }
    
    Any BrowserTransfer::CanGoForward(IPCMessage::SmartType msg) {
        return m_pBrowser->CanGoForward();
    }
    
    Any BrowserTransfer::GoForward(IPCMessage::SmartType msg) {
        m_pBrowser->GoForward();
        return Undefined();
    }
    
    Any BrowserTransfer::IsLoading(IPCMessage::SmartType msg) {
        return m_pBrowser->IsLoading();
    }
    
    Any BrowserTransfer::Reload(IPCMessage::SmartType msg) {
        m_pBrowser->Reload();
        return Undefined();
    }
    
    Any BrowserTransfer::ReloadIgnoreCache(IPCMessage::SmartType msg) {
        m_pBrowser->ReloadIgnoreCache();
        return Undefined();
    }
    
    Any BrowserTransfer::StopLoad(IPCMessage::SmartType msg) {
        m_pBrowser->StopLoad();
        return Undefined();
    }
    
    Any BrowserTransfer::GetIdentifier(IPCMessage::SmartType msg) {
        return m_pBrowser->GetIdentifier();
    }
    
    Any BrowserTransfer::IsSame(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        if (args->isValid(0)) {
            return false;
        }
        
        amo::u8json json = args->getJson(0);
        
        if (!json.is_valid()) {
            return false;
        }
        
        auto manager = TransferMappingMgr<BrowserTransfer>::getInstance();
        // 获取CefBrowser
        CefRefPtr<CefBrowser> pOther = manager->formJson(json);
        return m_pBrowser->IsSame(pOther);
    }
    
    Any BrowserTransfer::IsPopup(IPCMessage::SmartType msg) {
        return m_pBrowser->IsPopup();
    }
    
    Any BrowserTransfer::HasDocument(IPCMessage::SmartType msg) {
        return m_pBrowser->HasDocument();
    }
    
    Any BrowserTransfer::GetMainFrame(IPCMessage::SmartType msg) {
    
        CefRefPtr<CefFrame> pFrame = m_pBrowser->GetMainFrame();
        
        if (!pFrame) {
            return Undefined();
        }
        
        auto manager = TransferMappingMgr<FrameTransfer>::getInstance();
        return manager->toSimplifiedJson(pFrame);
    }
    
    Any BrowserTransfer::GetFocusedFrame(IPCMessage::SmartType msg) {
        CefRefPtr<CefFrame> pFrame =  m_pBrowser->GetFocusedFrame();
        
        if (!pFrame) {
            return Undefined();
        }
        
        auto manager = TransferMappingMgr<FrameTransfer>::getInstance();
        return manager->toSimplifiedJson(pFrame);
    }
    
    Any BrowserTransfer::GetFrame(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        Any& val = args->getValue(0);
        
        if (!args->isValid(0)) {
            return Undefined();
        }
        
        CefRefPtr<CefFrame> pFrame;
        
        if (val.type() == AnyValueType<int>::value) {
            pFrame = m_pBrowser->GetFrame((int)val);
        } else if (val.type() == AnyValueType<std::string>::value) {
            pFrame = m_pBrowser->GetFrame(val.As<std::string>());
        }
        
        if (!pFrame) {
            return Undefined();
        }
        
        auto manager = TransferMappingMgr<FrameTransfer>::getInstance();
        return manager->toSimplifiedJson(pFrame);
    }
    
    Any BrowserTransfer::GetFrameCount(IPCMessage::SmartType msg) {
        return (int)m_pBrowser->GetFrameCount();
    }
    
    Any BrowserTransfer::GetFrameIdentifiers(IPCMessage::SmartType msg) {
        std::vector<Any> retArr;
        std::vector<int64_t> vec;
        m_pBrowser->GetFrameIdentifiers(vec);
        
        // FrameID 不怎么可能超过32位
        for (auto&p : vec) {
            retArr.push_back((int)p);
        }
        
        return retArr;
    }
    
    Any BrowserTransfer::GetFrameNames(IPCMessage::SmartType msg) {
        std::vector<Any> retArr;
        std::vector<CefString> vec;
        m_pBrowser->GetFrameNames(vec);
        
        for (auto&p : vec) {
            retArr.push_back(p.ToString());
        }
        
        return retArr;
    }
    
    Any BrowserTransfer::SendProcessMessage(IPCMessage::SmartType msg) {
        // 不支持
        return Undefined();
    }
    
    Any BrowserTransfer::currentFrame(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        int64_t nFrameID = args->getInt64(IPCArgsPosInfo::FrameID);
        int nBrowserID = args->getInt(IPCArgsPosInfo::BrowserID);
        CefRefPtr<CefFrame> pFrame = m_pBrowser->GetFrame(nFrameID);
        
        if (!pFrame) {
            return Undefined();
        }
        
        auto manager = TransferMappingMgr<FrameTransfer>::getInstance();
        return manager->toSimplifiedJson(pFrame);
    }
    
}

