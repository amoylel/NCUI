#include "stdafx.h"
#include "transfer/RendererTransferMgr.h"



#include "ipc/UIMessageEmitter.hpp"


#include "transfer/Transfer.hpp"
#include "transfer/ClassTransfer.hpp"

#include "transfer/RendererThreadTransfer.h"
#include "module/transfer/BigStringTransfer.h"


namespace amo {
    void RendererTransferMgr::returnSyncResult(int nBrowserID,
            amo::IPCResult& ret) {
        auto mgr = RendererProcessExchangerManager::getInstance();
        mgr->exchange(nBrowserID, ret);
    }
    
    void RendererTransferMgr::returnAsyncResult(int nBrowserID,
            Any& ret,
            int id,
            int frame_id) {
        std::shared_ptr<MessageLauncher> runner;
        using MGR = amo::BrowserManager < PID_RENDERER >;
        CefRefPtr<CefBrowser> pBrowser = MGR::GetBrowserByID(nBrowserID);
        runner.reset(new RenderMessageEmitter(pBrowser->GetFrame(frame_id)));
        runner->setValue(IPCArgsPosInfo::TransferName, "ipcMain");
        runner->setValue(IPCArgsPosInfo::AsyncCallback, id);
        runner->execute("asyncExecuteResult", ret);
    }
    
    void RendererTransferMgr::registerClass(int nBrowserID) {
        addTransfer(nBrowserID,
                    ClassTransfer::getUniqueTransfer<RendererThreadTransfer>());
        addTransfer(nBrowserID,
                    ClassTransfer::getUniqueTransfer<BigStringTransfer>());
    }
    
}

