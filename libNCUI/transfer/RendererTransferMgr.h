// Created by amoylel on 14/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef LIBNCUI_RENDERERTRANSFERMGR_FCE5686C_EBC2_42B4_93C2_44FF8DA6148B_H__
#define LIBNCUI_RENDERERTRANSFERMGR_FCE5686C_EBC2_42B4_93C2_44FF8DA6148B_H__


#include "transfer/TransferMgr.h"

namespace amo {
    /*!
    * @class	RendererTransferMgr
    *
    * @brief	管理渲染进程的TransferMap.
    */
    class RendererTransferMgr
        : public TransferMgr
        , public amo::singleton < RendererTransferMgr > {
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


#endif //LIBNCUI_RENDERERTRANSFERMGR_FCE5686C_EBC2_42B4_93C2_44FF8DA6148B_H__