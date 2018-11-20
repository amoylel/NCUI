#include "stdafx.h"
#include "module/ConstantAttributeV8Handler.h"

namespace amo {

    ConstantAttributeV8Handler::ConstantAttributeV8Handler() {
        setHandlerName("ConstantAttributeV8Handler");
    }
    
    bool ConstantAttributeV8Handler::Execute(const CefString& name,
            CefRefPtr<CefV8Value> object,
            const CefV8ValueList& arguments,
            CefRefPtr<CefV8Value>& retval,
            CefString& exception) {
        std::string str = name.ToString();
        auto iter = m_oPropertyMap.find(str);
        
        if (iter == m_oPropertyMap.end()) {
            return false;
        }
        
        TypeConvertor convertor;
        retval = convertor.toV8Value(iter->second);
        return true;
    }
    
    
    
    WMAttriV8Handler::WMAttriV8Handler() {
        setHandlerName("ShowCmd");
        
    }
    
    void WMAttriV8Handler::onGetV8Object(CefRefPtr<CefV8Value> object) {
        object->SetValue("MSVIDEO",
                         V8_ACCESS_CONTROL_DEFAULT,
                         V8_PROPERTY_ATTRIBUTE_NONE);
    }
    
    
    
}

