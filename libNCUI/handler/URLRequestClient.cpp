#include "stdafx.h"
#include <amo/logger.hpp>
#include "handler/URLRequestClient.h"

namespace amo {
    bool URLRequestClient::GetAuthCredentials(bool isProxy,
            const CefString& host,
            int port,
            const CefString& realm,
            const CefString& scheme,
            CefRefPtr<CefAuthCallback> callback) {
            
            
        if (m_fnOnGetAuthCredentials) {
            return m_fnOnGetAuthCredentials(isProxy,
                                            host,
                                            port,
                                            realm,
                                            scheme,
                                            callback);
        }
        
        return false;
    }
    
    void URLRequestClient::OnDownloadData(CefRefPtr<CefURLRequest> request,
                                          const void* data, size_t data_length) {
        if (m_bDiscard) {
            request->Cancel();
        }
        
        if (!m_bDiscard && m_fnOnDownloadData) {
            m_fnOnDownloadData(request, data, data_length);
        }
        
        return;
    }
    
    void URLRequestClient::SetOnDownloadProgressCallback(ProcessCbType fn) {
        m_fnOnDownloadProgress = fn;
    }
    
    void URLRequestClient::SetOnGetAuthCredentialsCalllback(AuthCbType fn) {
        m_fnOnGetAuthCredentials = fn;
    }
    
    void URLRequestClient::SetOnUploadProgressCallback(ProcessCbType fn) {
        m_fnOnUploadProgress = fn;
    }
    
#if CHROME_VERSION_BUILD >= 2272
    void URLRequestClient::OnDownloadProgress(CefRefPtr<CefURLRequest> request,
            int64 current,
            int64 total) {
        if (m_bDiscard) {
            request->Cancel();
        }
        
        if (!m_bDiscard && m_fnOnDownloadProgress) {
            m_fnOnDownloadProgress(request, current, total);
        }
        
        return;
    }
    
    void URLRequestClient::OnUploadProgress(CefRefPtr<CefURLRequest> request,
                                            int64 current,
                                            int64 total) {
        if (m_bDiscard) {
            request->Cancel();
        }
        
        if (!m_bDiscard && m_fnOnUploadProgress) {
            m_fnOnUploadProgress(request, current, total);
        }
        
        return;
    }
    
#else
    
    void URLRequestClient::OnDownloadProgress(CefRefPtr<CefURLRequest> request,
            uint64 current,
            uint64 total) {
        if (m_bDiscard) {
            request->Cancel();
        }
    
        if (!m_bDiscard && m_fnOnDownloadProgress) {
            m_fnOnDownloadProgress(request, (int64)current, (int64)total);
        }
    
        return;
    }
    
    void URLRequestClient::OnUploadProgress(CefRefPtr<CefURLRequest> request,
                                            uint64 current,
                                            uint64 total) {
        if (m_bDiscard) {
            request->Cancel();
        }
    
        if (!m_bDiscard && m_fnOnUploadProgress) {
            m_fnOnUploadProgress(request, (int64)current, (int64)total);
        }
    
        return;
    }
    
#endif
    
    
    void URLRequestClient::OnRequestComplete(CefRefPtr<CefURLRequest> request) {
        //CefRefPtr<CefResponse> response = request->GetResponse();
        //int i = response->GetStatus();
        //CefURLRequest::Status c = request->GetRequestStatus();
        //CefURLRequest::ErrorCode code = request->GetRequestError();
        //CefResponse::HeaderMap mp;
        //CefString statusText = response->GetStatusText();
        //response->GetHeaderMap(mp);
        //auto iter = mp.begin();
        //for (; iter != mp.end(); ++iter)
        //{
        //	$clog(amo::cdevel << iter->first << "    :    " << iter->second << amo::endl;);
        //}
        ////int i = 3;
        
        if (!m_bDiscard && m_fnOnRequestComplete) {
            m_fnOnRequestComplete(request);
        }
        
        return;
    }
    
    void URLRequestClient::SetOnDownloadDataCallback(DataCbType fn) {
        m_fnOnDownloadData = fn;
    }
    
    
    
    void URLRequestClient::SetOnRequestCompleteCallback(CompleteCbType fn) {
        m_fnOnRequestComplete = fn;
    }
    
    void URLRequestClient::SetDiscard(bool bdiscard) {
        m_bDiscard = bdiscard;
    }
    
    URLRequestClient::URLRequestClient() {
        m_bDiscard = false;
    }
}