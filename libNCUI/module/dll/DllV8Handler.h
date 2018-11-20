// Created by amoylel on 03/27/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_DLLRETVALV8HANDLER_H__
#define AMO_DLLRETVALV8HANDLER_H__

#include <string>
#include <unordered_map>
#include <functional>

#include "handler/CefHeader.hpp"

#include "module/JsClassObjectV8Handler.h"

namespace amo {


    class DllV8Handler : public JsClassObjectV8Handler {
    
    public:
        DllV8Handler(const CefString& dllName);
        
        IPCMessage::SmartType V8ToAnyMessage(const CefString& name,
                                             const CefV8ValueList& arguments);
                                             
        virtual bool Execute(const CefString& name,
                             CefRefPtr<CefV8Value> object,
                             const CefV8ValueList& arguments,
                             CefRefPtr<CefV8Value>& retval,
                             CefString& exception);
                             
        void import(CefRefPtr<CefV8Value> object,
                    const CefV8ValueList& arguments,
                    CefRefPtr<CefV8Value>& retval,
                    CefString& except);
                    
        AMO_CEF_BEGIN_OBJECT_FACTORY(DllV8Handler)
        AMO_CEF_MAP_OBJECT_FUNCTION(import)
        AMO_CEF_END_OBJECT_FACTORY()
        
        
        
    private:
        CefString m_dllName;																		//!< DLL 名
    };
}

#endif // AMO_DLLRETVALV8HANDLER_H__