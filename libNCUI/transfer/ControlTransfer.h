// Created by amoylel on 08/08/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_CONTROLTRANSFER_H__
#define AMO_CONTROLTRANSFER_H__

#include <amo/singleton.hpp>

#include "transfer/ClassTransfer.hpp"
#include "transfer/TransferMappingMgr.hpp"

namespace DuiLib {
    class CControlUI;
}
namespace amo {



    class ControlTransfer
        : public ClassTransfer
        , public amo::singleton<ControlTransfer> {
    public:
        ControlTransfer();
        ControlTransfer(CControlUI* pControl);
        
        Any getValue(IPCMessage::SmartType msg);
        Any setValue(IPCMessage::SmartType msg);
        
        virtual Any onCreateClass(IPCMessage::SmartType msg) override;
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(ControlTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getValue, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setValue, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    protected:
        CControlUI* m_pControl;
        
    };
    
    template< >
    class TransferMappingMgr<ControlTransfer>
        : public TransferMappingMgrBase<ControlTransfer,
          std::shared_ptr<ControlTransfer>,
          CControlUI* > {
    public:
        typedef std::shared_ptr<ControlTransfer> transfer_type;
        typedef CControlUI*  cefrefptr_type;
    public:
    
    };
}

#endif // AMO_CONTROLTRANSFER_H__

