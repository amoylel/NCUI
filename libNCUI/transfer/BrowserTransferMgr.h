// Created by amoylel on 14/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef LIBNCUI_BROWSERTRANSFERMGR_4D82E6B8_6EAC_4447_B738_6DB5D94B7FAB_H__
#define LIBNCUI_BROWSERTRANSFERMGR_4D82E6B8_6EAC_4447_B738_6DB5D94B7FAB_H__


#include "transfer/TransferMgr.h"

namespace amo {
    /*!
    * @class	BrowserTransferMgr
    *
    * @brief	管理浏览器进程的TransferMap.
    */
    class BrowserTransferMgr
        : public TransferMgr
        , public amo::singleton < BrowserTransferMgr > {
    public:
        virtual void returnSyncResult(int nBrowserID,
                                      amo::IPCResult& ret) override;
                                      
        virtual void returnAsyncResult(int nBrowserID,
                                       Any& ret,
                                       int id,
                                       int frame_id) override;
                                       
        virtual void registerClass(int nBrowserID) override;
        
    };
    
}


#endif //LIBNCUI_BROWSERTRANSFERMGR_4D82E6B8_6EAC_4447_B738_6DB5D94B7FAB_H__