// Created by amoylel on 08/17/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NETTRANSFER_H__
#define AMO_NETTRANSFER_H__


#include <amo/singleton.hpp>

#include "transfer/ClassTransfer.hpp"


namespace amo {

    /*!
     * @class	net
     *
     * @brief	网络相关类.<br>
     * 			工作线程:**UI线程**.
     */
    
    class NetTransfer
        : public ClassTransfer
        , public amo::singleton < NetTransfer > {
    public:
        NetTransfer();
        
        /*!
         * @fn	Any NetTransfer::download(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	创建一个Downloader对象.
         *
         * @param	#JsonObject 详见{@link Downloader=Downloader.Downloader}.
         *
         * @return	#Downloader 返回一个下载对象.
         *
         * @see Downloader
         */
        
        Any download(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any NetTransfer::request(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	创建一个URLRequest对象.
         *
         * @param	#JsonObject 详见{@link URLRequest=URLRequest.URLRequest}.
         *
         * @return	#URLRequest 返回一个URLRequest对象.
         *
         * @see URLRequest
         */
        
        Any request(IPCMessage::SmartType msg);
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(NetTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(download, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(request, TransferFuncStatic | TransferExecSync)
        
        
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    };
}

#endif // AMO_NETTRANSFER_H__
