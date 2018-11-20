// Created by amoylel on 07/10/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_NODETRANSFERMGR_HPP__
#define AMO_NODETRANSFERMGR_HPP__

#include "transfer/TransferMgr.h"
#include "ipc/NodeProcessExchanger.hpp"

namespace amo {
    class NodeTransferMgr : public TransferMgr, public amo::singleton < NodeTransferMgr > {
    public:
        virtual void returnSyncResult(int nBrowserID, amo::IPCResult& ret) {
            // TODO: 返回同步消息
            auto mgr = NodeProcessExchangerManager::getInstance();
            mgr->exchange(nBrowserID, ret);
            return;
        }
        
        virtual void returnAsyncResult(int nBrowserID, amo::Any& ret, int id, int frame_id) {
            std::shared_ptr<NodeMessageEmitter> runner(new NodeMessageEmitter(-9999, -1));
            runner->setValue(IPCArgsPosInfo::TransferName, "ipcMain");
            runner->setValue(IPCArgsPosInfo::AsyncCallback, id);
            runner->execute("asyncExecuteResult", ret);
            return;
        }
        
        virtual void registerClass(int nBrowserID) {
            return;
        }
    };
}

#endif // AMO_NODETRANSFERMGR_H__

