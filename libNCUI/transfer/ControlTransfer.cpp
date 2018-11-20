#include "stdafx.h"
#include "ControlTransfer.h"


namespace amo {

    ControlTransfer::ControlTransfer()
        : ClassTransfer("CControlUI") {
        
    }
    
    ControlTransfer::ControlTransfer(CControlUI* pControl)
        : ClassTransfer("CControlUI")
        , m_pControl(pControl) {
        
    }
    
    Any ControlTransfer::getValue(IPCMessage::SmartType msg) {
        if (m_pControl) {
            amo::u8string strValue(m_pControl->GetText().GetData());
            return strValue.to_utf8();
        }
        
        return "";
    }
    
    Any ControlTransfer::setValue(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        amo::u8string strValue(args->getString(0), true);
        
        if (m_pControl) {
            m_pControl->SetText(strValue.to_unicode().c_str());
        }
        
        return Undefined();
    }
    
    Any ControlTransfer::onCreateClass(IPCMessage::SmartType msg) {
        return Undefined();
    }
    
}


