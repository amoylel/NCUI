#include "stdafx.h"
#include "transfer/DownloaderTransfer.h"
#include "../settings/DowloaderSettings.h"

namespace amo {

    DownloaderTransfer::DownloaderTransfer()
        : ClassTransfer("Downloader") {
        addModule("EventEmitter");
        
        if (!DownloadHandler::hasBeforeDownloadCallback()) {
            DownloadHandler::setBeforeDownloadCallback(
                std::bind(&DownloaderTransfer::OnBeforeDownload,
                          this,
                          std::placeholders::_1,
                          std::placeholders::_2,
                          std::placeholders::_3,
                          std::placeholders::_4));
        }
        
        if (!DownloadHandler::hasDownloadUpdateCallback()) {
            DownloadHandler::setDownloadUpdateCallback(
                std::bind(&DownloaderTransfer::OnDownloadUpdated,
                          this,
                          std::placeholders::_1,
                          std::placeholders::_2,
                          std::placeholders::_3));
        }
        
        
        m_nCommand = DL_NORMAL;
    }
    
    DownloaderTransfer::DownloaderTransfer(std::shared_ptr<DownloaderSettings> pSettings)
        : ClassTransfer("Downloader")
        , m_pDownloaderSettings(pSettings) {
        addModule("EventEmitter");
        
        
    }
    
    Any DownloaderTransfer::onCreateClass(IPCMessage::SmartType msg) {
    
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        amo::u8json json = args->getJson(0);
        std::shared_ptr<DownloaderSettings> pSettings;
        pSettings.reset(new DownloaderSettings());
        pSettings->updateArgsSettings(json);
        
        auto iter = m_oDownloaderMap.find(pSettings->url);
        
        if (iter != m_oDownloaderMap.end()) {
            return iter->second->getFuncMgr().toSimplifiedJson();
        }
        
        
        std::shared_ptr<DownloaderTransfer> pTransfer;
        pTransfer = ClassTransfer::createTransfer<DownloaderTransfer>(pSettings);
        int nBrowserID = args->getInt(IPCArgsPosInfo::BrowserID);
        int64_t nFrameID = args->getInt64(IPCArgsPosInfo::FrameID);
        CefRefPtr<CefBrowser> pBrowser;
        pBrowser = ClientHandler::GetBrowserByID(nBrowserID);
        CefRefPtr<CefFrame> pFrame;
        pFrame = ClientHandler::GetFrameByID(nFrameID);
        pTransfer->setBrowser(pBrowser);
        pTransfer->setFrame(pFrame);
        
        if (pSettings->autoDownload) {
            pTransfer->startDownload();
        }
        
        m_oDownloaderMap[pSettings->url] = pTransfer;
        
        return pTransfer->getFuncMgr().toSimplifiedJson();
    }
    
    Any DownloaderTransfer::start(IPCMessage::SmartType msg) {
        if (!m_pDownloaderSettings) {
            return Undefined();
        }
        
        if (m_pDownloaderSettings->autoDownload) {
            return Undefined();
        }
        
        startDownload();
        return Undefined();
    }
    
    Any DownloaderTransfer::resume(IPCMessage::SmartType msg) {
        if (m_nCommand == DL_RESUME) {
            return false;
        }
        
        m_nCommand = DL_RESUME;
        return Undefined();
    }
    
    Any DownloaderTransfer::pause(IPCMessage::SmartType msg) {
        if (m_nCommand == DL_PAUSE) {
            return false;
        }
        
        m_nCommand = DL_PAUSE;
        
        
        return Undefined();
    }
    
    Any DownloaderTransfer::cancel(IPCMessage::SmartType msg) {
        if (m_nCommand == DL_CANCEL) {
            return false;
        }
        
        m_nCommand = DL_CANCEL;
        
        getMessageEmitter()->execute("triggerEvent", "cancel");
        return Undefined();
    }
    
    void DownloaderTransfer::startDownload() {
        if (!m_pDownloaderSettings) {
            return;
        }
        
        // 如果Browser存在，那么直接下载
        if (m_pBrowser) {
            m_pBrowser->GetHost()->StartDownload(m_pDownloaderSettings->url);
            return;
        }
        
        // 找一个可用的浏览器窗口
        int nBrowserID = -9999;
        auto manager = BrowserWindowManager::getInstance();
        auto pWindow = manager->findValidWindow(nBrowserID);
        
        if (!pWindow) {
            return;
        }
        
        m_pBrowser = pWindow->toBrowserWindow()->GetWebkitView()->getBrowser();
        m_pBrowser->GetHost()->StartDownload(m_pDownloaderSettings->url);
        
        return;
    }
    
    CefRefPtr<CefFrame> DownloaderTransfer::getFrame() const {
        return m_pFrame;
    }
    
    void DownloaderTransfer::setFrame(CefRefPtr<CefFrame> val) {
        m_pFrame = val;
    }
    
    CefRefPtr<CefBrowser> DownloaderTransfer::getBrowser() const {
        return m_pBrowser;
    }
    
    void DownloaderTransfer::setBrowser(CefRefPtr<CefBrowser> val) {
        m_pBrowser = val;
    }
    
    void DownloaderTransfer::removeDownloader(const std::string& url) {
        auto iter = m_oDownloaderMap.find(url);
        
        if (iter == m_oDownloaderMap.end()) {
            return;
        }
        
        removeTransfer(iter->second->getObjectID());
        m_oDownloaderMap.erase(url);
        
        return;
    }
    
    std::shared_ptr<amo::UIMessageEmitter> DownloaderTransfer::getMessageEmitter() {
        std::shared_ptr<UIMessageEmitter> emitter(new UIMessageEmitter(m_pFrame));
        emitter->setValue(IPCArgsPosInfo::TransferName, "ipcRenderer");
        emitter->setValue(IPCArgsPosInfo::EventObjectID, getObjectID());
        return emitter;
    }
    
    bool DownloaderTransfer::OnBeforeDownload(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefDownloadItem> download_item,
            const CefString& suggested_name,
            CefRefPtr<CefBeforeDownloadCallback>& callback) {
            
        if (!m_pDownloaderSettings) {
            auto iter = m_oDownloaderMap.find(download_item->GetOriginalUrl());
            
            // 如果没有找到这个URL,如果这个回调不是由downloader 创建的下载
            if (iter == m_oDownloaderMap.end()) {
                return false;
            }
            
            bool bOk =  iter->second->OnBeforeDownload(browser,
                        download_item,
                        suggested_name,
                        callback);
                        
            // 一定是返回true
            return true;
            
        } else {
            if (m_pDownloadCallback != NULL) {
                return true;
            }
            
            m_pDownloadCallback = callback;
            
            if (m_pDownloaderSettings->file.empty()) {
                m_pDownloaderSettings->file = suggested_name.ToString();
            }
            
            callback->Continue(m_pDownloaderSettings->file, false);
            amo::u8json json = downloadItemToJson(download_item);
            getMessageEmitter()->execute("triggerEvent", "start", json);
            
            // 如果文件已经存在 ，，那么怎么办，自动重命名，覆盖？
            return true;
        }
        
        return false;
        
    }
    
    bool DownloaderTransfer::OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefDownloadItem> download_item,
            CefRefPtr<CefDownloadItemCallback>& callback) {
            
            
            
        if (!m_pDownloaderSettings) {
            auto iter = m_oDownloaderMap.find(download_item->GetOriginalUrl());
            
            if (iter == m_oDownloaderMap.end()) {
                return false;
            }
            
            return iter->second->OnDownloadUpdated(browser,
                                                   download_item,
                                                   callback);
        } else {
            m_pDownloadItemCallback = callback;
            
            // update 有可能 会先调用，所以等beforedownload调用过后再向页面回调
            if (!m_pDownloadCallback) {
                return true;
            }
            
            amo::u8json json = downloadItemToJson(download_item);
            
            if (m_nCommand == DL_CANCEL) {
                if (!download_item->IsCanceled()) {
                    callback->Cancel();
                }
                
                if (download_item->IsCanceled()) {
                    getMessageEmitter()->execute("triggerEvent", "canceled", json);
                    auto transfer = ClassTransfer::getUniqueTransfer<DownloaderTransfer>();
                    transfer->removeDownloader(m_pDownloaderSettings->url);
                    m_nCommand = DL_NORMAL;
                }
                
                return true;
            }
            
            if (m_nCommand == DL_PAUSE && !download_item->IsComplete() && !download_item->IsCanceled()) {
                callback->Pause();
                getMessageEmitter()->execute("triggerEvent", "pause");
                m_nCommand = DL_NORMAL;
                return true;
            }
            
            if (m_nCommand == DL_RESUME && !download_item->IsComplete() && !download_item->IsCanceled()) {
                callback->Resume();
                getMessageEmitter()->execute("triggerEvent", "resume");
                m_nCommand = DL_NORMAL;
                return true;
            }
            
            
            if (download_item->IsComplete()) {
                getMessageEmitter()->execute("triggerEvent", "complete", json);
                auto transfer = ClassTransfer::getUniqueTransfer<DownloaderTransfer>();
                transfer->removeDownloader(m_pDownloaderSettings->url);
            } else if (download_item->IsCanceled()) {
                getMessageEmitter()->execute("triggerEvent", "canceled", json);
                auto transfer = ClassTransfer::getUniqueTransfer<DownloaderTransfer>();
                transfer->removeDownloader(m_pDownloaderSettings->url);
                
            } else {
                getMessageEmitter()->execute("triggerEvent", "update", json);
            }
            
            return true;
            
        }
        
        return false;
    }
    
    amo::u8json DownloaderTransfer::downloadItemToJson(CefRefPtr<CefDownloadItem>& item) {
        amo::u8json json;
        json.put("isValid", item->IsValid());
        json.put("isInProgress", item->IsInProgress());
        json.put("isComplete", item->IsComplete());
        json.put("isCanceled", item->IsCanceled());
        json.put("currentSpeed", item->GetCurrentSpeed());
        json.put("percentComplete", item->GetPercentComplete());
        json.put("totalBytes", item->GetTotalBytes());
        json.put("receivedBytes", item->GetReceivedBytes());
        json.put("startTime", item->GetStartTime().GetDoubleT());
        json.put("endTime", item->GetEndTime().GetDoubleT());
        
        
        json.put("fullPath", item->GetFullPath().ToString());
        json.put("id", (int)item->GetId());
        json.put("url", item->GetURL().ToString());
        json.put("originalUrl", item->GetOriginalUrl().ToString());
        json.put("suggestedFileName", item->GetSuggestedFileName().ToString());
        json.put("contentDisposition", item->GetContentDisposition().ToString());
        json.put("mimeType", item->GetMimeType().ToString());
        
        return json;
        
        
    }
    
}

