// Created by amoylel on 07/20/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_URLREQUESTTRANSFER_H__
#define AMO_URLREQUESTTRANSFER_H__

#include <stdint.h>

#include <memory>


#include <amo/singleton.hpp>

#include "handler/CefHeader.hpp"
#include "transfer/ClassTransfer.hpp"



namespace amo {


    class UIMessageBroadcaster;
    class RequestSettings;
    
    /*!
     * @class	URLRequest
     *
     * @brief	网络请求类.<br>
     * 			工作线程:**UI线程**.
     */
    
    class URLRequestTransfer
        : public ClassTransfer
        , public amo::singleton < URLRequestTransfer > {
    public:
        URLRequestTransfer();
        
        /*!
         * @fn	URLRequestTransfer::URLRequest(CefRefPtr<CefURLRequest> pURLRequest, int32_t nTimeOut = 0);
         *
         * @tag constructor
         *
         * @brief	创建一个网络请求对象.
         *
         * @param	#JsonObject	创建URLRequest所需要的参数.
         *
         * @param	#Int=0 网络超时时间.
         *
         * @return #URLRequest 一个request对象
         */
        
        URLRequestTransfer(CefRefPtr<CefURLRequest> pURLRequest,
                           int32_t nTimeOut = 0);
        ~URLRequestTransfer();
        virtual Any onCreateClass(IPCMessage::SmartType msg) override;
        
        virtual void onBeforeRelease() override;
        Any GetRequest(IPCMessage::SmartType msg);
        Any GetClient(IPCMessage::SmartType msg);
        Any GetRequestStatus(IPCMessage::SmartType msg);
        Any GetRequestError(IPCMessage::SmartType msg);
        Any GetResponse(IPCMessage::SmartType msg);
        Any Cancel(IPCMessage::SmartType msg);
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(URLRequestTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetRequest, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetClient, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetRequestStatus, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetRequestError, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetResponse, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(Cancel, TransferExecSync)
        
        AMO_CEF_MESSAGE_TRANSFER_END()
        
        
        
        virtual void OnRequestComplete(CefRefPtr<CefURLRequest> request);
        
        virtual void OnUploadProgress(CefRefPtr<CefURLRequest> request,
                                      int64 current,
                                      int64 total);
                                      
        virtual void OnDownloadProgress(CefRefPtr<CefURLRequest> request,
                                        int64 current,
                                        int64 total);
        virtual void OnDownloadData(CefRefPtr<CefURLRequest> request,
                                    const void* data,
                                    size_t data_length);
                                    
        virtual bool OnGetAuthCredentials(bool isProxy,
                                          const CefString& host,
                                          int port,
                                          const CefString& realm,
                                          const CefString& scheme,
                                          CefRefPtr<CefAuthCallback> callback);
                                          
        CefRefPtr<CefFrame> getFrame() const;
        void setFrame(CefRefPtr<CefFrame> val);
        
        std::shared_ptr<UIMessageBroadcaster>  getMessageEmitter();
        
        std::shared_ptr<amo::RequestSettings> getRequestSettings() const;
        void setRequestSettings(std::shared_ptr<amo::RequestSettings> val);
        
        bool onCheckTimeOut(int64_t id);
        
        void setTimeOut(int32_t nTimeOut);
        
    private:
        CefRefPtr<CefURLRequest> m_pURLRequest;
        CefRefPtr<CefFrame> m_pFrame;
        std::shared_ptr<RequestSettings> m_pRequestSettings;
        std::string m_downloadData;
        amo::timer m_timer;
        bool  m_bTimeOut;  // 是否超时
        
    };
}

#endif // AMO_URLREQUESTTRANSFER_H__
