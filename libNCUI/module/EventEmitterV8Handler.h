// Created by amoylel on 03/27/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_EVENTEMITTERV8HANDLER_H__
#define AMO_EVENTEMITTERV8HANDLER_H__

#include "module/JsClassObjectV8Handler.h"
#include "module/basic/EventFunctionManager.h"
namespace amo {

    class EventEmitterV8Handler : public JsClassObjectV8Handler {
    public:
        EventEmitterV8Handler();
        
        void eventNames(CefRefPtr<CefV8Value> object,
                        const CefV8ValueList& arguments,
                        CefRefPtr<CefV8Value>& retval,
                        CefString& except);
        void getMaxListeners(CefRefPtr<CefV8Value> object,
                             const CefV8ValueList& arguments,
                             CefRefPtr<CefV8Value>& retval,
                             CefString& except);
        void listenerCount(CefRefPtr<CefV8Value> object,
                           const CefV8ValueList& arguments,
                           CefRefPtr<CefV8Value>& retval,
                           CefString& except);
        void listeners(CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& except);
                       
        CallbackFunctionWrapper::SmartType addListener(CefRefPtr<CefV8Value> object,
                const CefV8ValueList& arguments,
                CefRefPtr<CefV8Value>& retval,
                CefString& except);
                
        void on(CefRefPtr<CefV8Value> object,
                const CefV8ValueList& arguments,
                CefRefPtr<CefV8Value>& retval,
                CefString& except);
        void emit(CefRefPtr<CefV8Value> object,
                  const CefV8ValueList& arguments,
                  CefRefPtr<CefV8Value>& retval,
                  CefString& except);
        void once(CefRefPtr<CefV8Value> object,
                  const CefV8ValueList& arguments,
                  CefRefPtr<CefV8Value>& retval,
                  CefString& except);
        void unique(CefRefPtr<CefV8Value> object,
                    const CefV8ValueList& arguments,
                    CefRefPtr<CefV8Value>& retval,
                    CefString& except);
        void prependListener(CefRefPtr<CefV8Value> object,
                             const CefV8ValueList& arguments,
                             CefRefPtr<CefV8Value>& retval,
                             CefString& except);
        void prependOnceListener(CefRefPtr<CefV8Value> object,
                                 const CefV8ValueList& arguments,
                                 CefRefPtr<CefV8Value>& retval,
                                 CefString& except);
        void removeAllListeners(CefRefPtr<CefV8Value> object,
                                const CefV8ValueList& arguments,
                                CefRefPtr<CefV8Value>& retval,
                                CefString& except);
        void removeListener(CefRefPtr<CefV8Value> object,
                            const CefV8ValueList& arguments,
                            CefRefPtr<CefV8Value>& retval,
                            CefString& except);
        void setMaxListeners(CefRefPtr<CefV8Value> object,
                             const CefV8ValueList& arguments,
                             CefRefPtr<CefV8Value>& retval,
                             CefString& except);
                             
        AMO_CEF_BEGIN_OBJECT_FACTORY(EventEmitterV8Handler)
        
        AMO_CEF_MAP_OBJECT_FUNCTION(emit)
        AMO_CEF_MAP_OBJECT_FUNCTION(eventNames)
        AMO_CEF_MAP_OBJECT_FUNCTION(getMaxListeners)
        AMO_CEF_MAP_OBJECT_FUNCTION(listenerCount)
        AMO_CEF_MAP_OBJECT_FUNCTION(listeners)
        AMO_CEF_MAP_OBJECT_FUNCTION(on)
        AMO_CEF_MAP_OBJECT_FUNCTION(once)
        AMO_CEF_MAP_OBJECT_FUNCTION(unique)
        AMO_CEF_MAP_OBJECT_FUNCTION(prependListener)
        AMO_CEF_MAP_OBJECT_FUNCTION(prependOnceListener)
        AMO_CEF_MAP_OBJECT_FUNCTION(removeAllListeners)
        AMO_CEF_MAP_OBJECT_FUNCTION(removeListener)
        AMO_CEF_MAP_OBJECT_FUNCTION(setMaxListeners)
        AMO_CEF_END_OBJECT_FACTORY()
    public:
        virtual CefRefPtr<JsV8Handler> createV8Handler();
    protected:
        std::map<std::string, std::vector<CallbackFunctionWrapper::SmartType > >  m_oEventCallbackFunc;			//!< 事件监听回调函数
    };
}
#endif AMO_EVENTEMITTERV8HANDLER_H__