#include "stdafx.h"
#include "transfer/URLRequestTransfer.h"
#include "transfer/RequestTransfer.h"
#include "transfer/URLRequestClientTransfer.h"
#include "transfer/TransferMappingMgr.hpp"
#include "handler/URLRequestClient.h"
#include "handler/BrowserManager.hpp"
#include "ipc/UIMessageBroadcaster.hpp"
#include "settings/RequestSettings.h"
#include <amo/File2.hpp>
#include <vector>

namespace amo {

    URLRequestTransfer::URLRequestTransfer()
        : ClassTransfer("URLRequest") {
        addModule("EventEmitter");
    }
    
    URLRequestTransfer::URLRequestTransfer(CefRefPtr<CefURLRequest> pURLRequest,
                                           int32_t nTimeOut)
        : ClassTransfer("URLRequest")
        , m_pURLRequest(pURLRequest)
        , m_bTimeOut(true) {
        addModule("EventEmitter");
        
        
        
    }
    
    URLRequestTransfer::~URLRequestTransfer() {
        m_timer.stop_timer();
    }
    
    Any URLRequestTransfer::GetRequest(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any URLRequestTransfer::GetClient(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    Any URLRequestTransfer::GetRequestStatus(IPCMessage::SmartType msg) {
        return (int)m_pURLRequest->GetRequestStatus();
    }
    
    Any URLRequestTransfer::GetRequestError(IPCMessage::SmartType msg) {
        return (int)m_pURLRequest->GetRequestError();
    }
    
    Any URLRequestTransfer::GetResponse(IPCMessage::SmartType msg) {
    
        return Undefined();
    }
    
    Any URLRequestTransfer::Cancel(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
    void URLRequestTransfer::OnRequestComplete(CefRefPtr<CefURLRequest> request) {
        std::shared_ptr<UIMessageBroadcaster> emitter = getMessageEmitter();
        auto v1 = request->GetRequestStatus();
        auto v2 = request->GetRequestError();
        m_bTimeOut = false;
        emitter->broadcast("request.complete",
                           m_downloadData);
                           
    }
    
    void URLRequestTransfer::OnUploadProgress(CefRefPtr<CefURLRequest> request,
            int64 current,
            int64 total) {
        m_bTimeOut = false;
        std::shared_ptr<UIMessageBroadcaster> emitter = getMessageEmitter();
        
        emitter->broadcast("request.upload.progress",
                           current,
                           total);
                           
    }
    
    void URLRequestTransfer::OnDownloadProgress(CefRefPtr<CefURLRequest> request,
            int64 current,
            int64 total) {
        m_bTimeOut = false;
        std::shared_ptr<UIMessageBroadcaster> emitter = getMessageEmitter();
        emitter->broadcast("request.download.progress",
                           current,
                           total);
    }
    
    void URLRequestTransfer::OnDownloadData(CefRefPtr<CefURLRequest> request,
                                            const void* data,
                                            size_t data_length) {
        m_bTimeOut = false;
        m_downloadData.append((char*)data, data_length);
    }
    
    bool URLRequestTransfer::OnGetAuthCredentials(bool isProxy,
            const CefString& host,
            int port,
            const CefString& realm,
            const CefString& scheme,
            CefRefPtr<CefAuthCallback> callback) {
        return false;
    }
    
    CefRefPtr<CefFrame> URLRequestTransfer::getFrame() const {
        return m_pFrame;
    }
    
    void URLRequestTransfer::setFrame(CefRefPtr<CefFrame> val) {
        m_pFrame = val;
    }
    
    std::shared_ptr<amo::UIMessageBroadcaster>
    URLRequestTransfer::getMessageEmitter() {
    
        std::shared_ptr<UIMessageBroadcaster> runner;
        runner.reset(new UIMessageBroadcaster(getObjectID()));
        return runner;
        
        /*  std::shared_ptr<UIMessageBroadcaster> emitter(new UIMessageBroadcaster(
                      m_pFrame));
          emitter->setValue(IPCArgsPosInfo::TransferName, "ipcRenderer");
          emitter->setValue(IPCArgsPosInfo::EventObjectID, getObjectID());
          return emitter;*/
    }
    
    std::shared_ptr<amo::RequestSettings> URLRequestTransfer::getRequestSettings()
    const {
        return m_pRequestSettings;
    }
    
    void URLRequestTransfer::setRequestSettings(
        std::shared_ptr<amo::RequestSettings> val) {
        m_pRequestSettings = val;
    }
    
    
    
    
    Any URLRequestTransfer::onCreateClass(IPCMessage::SmartType msg) {
    
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        amo::u8json json = args->getJson(0);
        std::string test = json.to_string();
        std::shared_ptr<RequestSettings> pSettings;
        pSettings.reset(new RequestSettings());
        pSettings->updateArgsSettings(json);
        
        CefRefPtr<CefFrame> pFrame = BrowserManager<PID_BROWSER>::GetFrameByID(
                                         args->getInt(IPCArgsPosInfo::FrameID));
                                         
        auto rMgr = TransferMappingMgr<RequestTransfer>::getInstance();
        
        auto rcMgr = TransferMappingMgr<URLRequestClientTransfer>::getInstance();
        
        CefRefPtr<CefRequest> pRequest = CefRequest::Create();
        
        CefRequest::HeaderMap header;
        
        auto headers = pSettings->getHeaderMap();
        
        for (auto& p : headers) {
            header.insert({ p.first, p.second });
        }
        
        
        CefRefPtr<CefPostData> pData = CefPostData::Create();
        auto files = pSettings->getFiles();
        
        for (auto& p : files) {
            CefRefPtr<CefPostDataElement> element = CefPostDataElement::Create();
            element->SetToFile(p.to_utf8());
            pData->AddElement(element);
        }
        
        pRequest->SetMethod(pSettings->type); // post
        pRequest->SetURL(pSettings->makeUrlWithArgs());
        pRequest->SetHeaderMap(header);
        
        if (!files.empty()) {
            pRequest->SetPostData(pData);
            pRequest->SetFlags(UR_FLAG_REPORT_UPLOAD_PROGRESS);
        }
        
        
        CefRefPtr<URLRequestClient> pClient = new URLRequestClient();
        CefRefPtr<CefURLRequest> pURLRequest;
#if CHROME_VERSION_BUILD >= 2357
        pURLRequest = CefURLRequest::Create(pRequest,
                                            pClient,
                                            CefRequestContext::GetGlobalContext());
#else
        pURLRequest = CefURLRequest::Create(pRequest,
                                            pClient);
#endif
                                            
        using MGR = TransferMappingMgr < URLRequestTransfer >;
        std::shared_ptr<URLRequestTransfer>  pTransfer;
        pTransfer = MGR::getInstance()->toTransfer(pURLRequest);
        pTransfer->setFrame(pFrame);
        pTransfer->setRequestSettings(pSettings);
        
        if (args->getValue(1).is<int>()) {
            pTransfer->setTimeOut(args->getInt(1));
        }
        
        
        pClient->SetOnDownloadDataCallback(
            std::bind(&URLRequestTransfer::OnDownloadData,
                      pTransfer,
                      std::placeholders::_1,
                      std::placeholders::_2,
                      std::placeholders::_3));
                      
        pClient->SetOnDownloadProgressCallback(
            std::bind(&URLRequestTransfer::OnDownloadProgress,
                      pTransfer,
                      std::placeholders::_1,
                      std::placeholders::_2,
                      std::placeholders::_3));
                      
        pClient->SetOnGetAuthCredentialsCalllback(
            std::bind(&URLRequestTransfer::OnGetAuthCredentials,
                      pTransfer,
                      std::placeholders::_1,
                      std::placeholders::_2,
                      std::placeholders::_3,
                      std::placeholders::_4,
                      std::placeholders::_5,
                      std::placeholders::_6));
                      
        pClient->SetOnRequestCompleteCallback(
            std::bind(&URLRequestTransfer::OnRequestComplete,
                      pTransfer,
                      std::placeholders::_1));
                      
        pClient->SetOnUploadProgressCallback(
            std::bind(&URLRequestTransfer::OnUploadProgress,
                      pTransfer,
                      std::placeholders::_1,
                      std::placeholders::_2,
                      std::placeholders::_3));
                      
        return MGR::getInstance()->toSimplifiedJson(pTransfer);
        
    }
    
    bool URLRequestTransfer::onCheckTimeOut(int64_t id) {
        if (m_bTimeOut) {
            getMessageEmitter()->broadcast("request.timeout");
            getMessageEmitter()->broadcast("request.complete");
        }
        
        return false;
    }
    
    
    void URLRequestTransfer::setTimeOut(int32_t nTimeOut) {
        if (nTimeOut > 0) {
            m_timer.set_timer(nTimeOut, std::bind(&URLRequestTransfer::onCheckTimeOut, this,
                                                  std::placeholders::_1), true);
        }
    }
    
    void URLRequestTransfer::onBeforeRelease() {
    
        m_timer.stop_timer();
        
        if (m_pURLRequest) {
            auto rcMgr = TransferMappingMgr<URLRequestClientTransfer>::getInstance();
            
            if (m_pURLRequest->GetClient()) {
                rcMgr->removeMapping(m_pURLRequest->GetClient());
            }
            
            using MGR = TransferMappingMgr < URLRequestTransfer >;
            MGR::getInstance()->removeMapping(m_pURLRequest);
        }
        
        
        
    }
    
    
}

