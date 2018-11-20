#include "stdafx.h"
#include "module/AppExHandler.h"

namespace amo {
    AppExV8Handler::AppExV8Handler() {
        setHandlerName("AppEx");
        
    }
    
    AppExV8Handler::~AppExV8Handler() {
    }
    
    CefRefPtr<amo::JsV8Handler> AppExV8Handler::createV8Handler() {
        return new AppExV8Handler();
    }
    
    void AppExV8Handler::addNonGlobalModules(CefRefPtr<CefV8Value> object,
            const CefV8ValueList& args,
            CefRefPtr<CefV8Value>& retval,
            CefString& except) {
            
        amo::u8json arr = AppSettings::getInstance()->nonGlobalModules;
        
        if (!arr.is_array()) {
            arr = amo::u8json();
            arr.set_array();
        }
        
        //std::set<std::string> oSet;
        for (size_t i = 0; i < args.size(); ++i) {
            CefRefPtr<CefV8Value> val = args.at(i);
            
            if (!val || !val->IsString() || val->GetStringValue().empty()) {
                except = L"the parameter must be a string and cannot be empty.";
                return ;
            }
            
            CefString module = val->GetStringValue();
            arr.push_back(module.ToString());
            
        }
        
        AppSettings::getInstance()->nonGlobalModules = arr;
    }
    
    void AppExV8Handler::addNonGlobalModule(CefRefPtr<CefV8Value> object,
                                            const CefV8ValueList& args, CefRefPtr<CefV8Value>& retval, CefString& except) {
        return addNonGlobalModules(object, args, retval, except);
    }
    
}

