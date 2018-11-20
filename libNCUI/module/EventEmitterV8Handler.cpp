#include "stdafx.h"
#include "module/EventEmitterV8Handler.h"
#include "module/basic/EventFunctionManager.h"


namespace amo {

    EventEmitterV8Handler::EventEmitterV8Handler() {
        setHandlerName("EventEmitter");
    }
    
    void EventEmitterV8Handler::eventNames(CefRefPtr<CefV8Value> object,
                                           const CefV8ValueList& arguments,
                                           CefRefPtr<CefV8Value>& retval,
                                           CefString& except) {
                                           
    }
    
    void EventEmitterV8Handler::getMaxListeners(CefRefPtr<CefV8Value> object,
            const CefV8ValueList& arguments,
            CefRefPtr<CefV8Value>& retval,
            CefString& except) {
            
    }
    
    void EventEmitterV8Handler::listenerCount(CefRefPtr<CefV8Value> object,
            const CefV8ValueList& arguments,
            CefRefPtr<CefV8Value>& retval,
            CefString& except) {
        std::string nKey = arguments.at(0)->GetStringValue();
        auto iter = m_oEventCallbackFunc.find(nKey);
        
        if (iter == m_oEventCallbackFunc.end()) {
            retval = CefV8Value::CreateInt(0);
        } else {
            retval = CefV8Value::CreateInt(iter->second.size());
        }
        
        
    }
    
    void EventEmitterV8Handler::listeners(CefRefPtr<CefV8Value> object,
                                          const CefV8ValueList& arguments,
                                          CefRefPtr<CefV8Value>& retval,
                                          CefString& except) {
                                          
    }
    
    
    
    CallbackFunctionWrapper::SmartType EventEmitterV8Handler::addListener(CefRefPtr<CefV8Value> object,
            const CefV8ValueList& arguments,
            CefRefPtr<CefV8Value>& retval,
            CefString& except) {
        if (arguments.size() < 2
                || !arguments.at(0)->IsString()
                || !arguments.at(1)->IsFunction()) {
            return CallbackFunctionWrapper::SmartType();
        }
        
        CefRefPtr<CefV8Context> pContext = CefV8Context::GetCurrentContext();
        CefRefPtr<CefBrowser> browser = pContext->GetBrowser();
        int nBrowserID = browser->GetIdentifier();
        int nFrameID = pContext->GetFrame()->GetIdentifier();
        
        
        std::string strEvent = arguments.at(0)->GetStringValue();
        CefRefPtr<CefV8Value> cb = arguments.at(1);
        auto iter = m_oEventCallbackFunc.find(strEvent);
        
        if (iter == m_oEventCallbackFunc.end()) {
            m_oEventCallbackFunc[strEvent] = std::vector<CallbackFunctionWrapper::SmartType >();
        }
        
        CallbackFunctionWrapper::SmartType fn(new CallbackFunctionWrapper(pContext, cb));
        m_oEventCallbackFunc[strEvent].push_back(
            fn
        );
        
        return fn;
    }
    
    void EventEmitterV8Handler::on(CefRefPtr<CefV8Value> object,
                                   const CefV8ValueList& arguments,
                                   CefRefPtr<CefV8Value>& retval,
                                   CefString& except) {
        addListener(object, arguments, retval, except);
    }
    
    void EventEmitterV8Handler::emit(CefRefPtr<CefV8Value> object,
                                     const CefV8ValueList& arguments,
                                     CefRefPtr<CefV8Value>& retval,
                                     CefString& except) {
        CefRefPtr<CefV8Context> pContext = CefV8Context::GetCurrentContext();
        CefRefPtr<CefBrowser> browser = pContext->GetBrowser();
        int nBrowserID = browser->GetIdentifier();
        int nFrameID = pContext->GetFrame()->GetIdentifier();
        
        CefRefPtr<CefFrame> pFrame = pContext->GetFrame();
        int frame_id = static_cast<int>(pFrame->GetIdentifier());
        retval = CefV8Value::CreateUndefined();
        
        std::string nKey = arguments.at(0)->GetStringValue();
        
        auto iter = m_oEventCallbackFunc.find(nKey);
        
        if (iter == m_oEventCallbackFunc.end()) {
            return;
        }
        
        std::vector<CallbackFunctionWrapper::SmartType >&  vec = iter->second;
        
        if (vec.size() < 1) {
            return;
        }
        
        CefV8ValueList args;
        
        for (size_t i = 1; i < arguments.size(); ++i) {
            args.push_back(arguments.at(i));
        }
        
        for (auto iter = vec.begin(); iter != vec.end();) {
        
            CallbackFunctionWrapper::SmartType wrapper = *iter;
            
            CefRefPtr<CefV8Value> pFunction = wrapper->m_pFunction;
            retval = pFunction->ExecuteFunctionWithContext(
                         wrapper->m_pContext,
                         NULL,
                         args);
                         
            if (wrapper->m_bOnce) {
                iter = vec.erase(iter);
            } else {
                ++iter;
            }
        }
        
        return;
    }
    
    void EventEmitterV8Handler::once(CefRefPtr<CefV8Value> object,
                                     const CefV8ValueList& arguments,
                                     CefRefPtr<CefV8Value>& retval,
                                     CefString& except) {
        CallbackFunctionWrapper::SmartType fn = addListener(object, arguments, retval, except);
        
        if (fn) {
            fn->m_bOnce = true;
        }
        
        return;
        
    }
    
    void EventEmitterV8Handler::unique(CefRefPtr<CefV8Value> object,
                                       const CefV8ValueList& arguments,
                                       CefRefPtr<CefV8Value>& retval,
                                       CefString& except) {
        std::string nKey = arguments.at(0)->GetStringValue();
        
        auto iter = m_oEventCallbackFunc.find(nKey);
        
        if (iter != m_oEventCallbackFunc.end()) {
            iter->second.clear();
        }
        
        
        on(object, arguments, retval, except);
        return;
    }
    
    void EventEmitterV8Handler::prependListener(CefRefPtr<CefV8Value> object,
            const CefV8ValueList& arguments,
            CefRefPtr<CefV8Value>& retval,
            CefString& except) {
            
    }
    
    void EventEmitterV8Handler::prependOnceListener(CefRefPtr<CefV8Value> object,
            const CefV8ValueList& arguments,
            CefRefPtr<CefV8Value>& retval,
            CefString& except) {
            
    }
    
    void EventEmitterV8Handler::removeAllListeners(CefRefPtr<CefV8Value> object,
            const CefV8ValueList& arguments,
            CefRefPtr<CefV8Value>& retval,
            CefString& except) {
            
    }
    
    void EventEmitterV8Handler::removeListener(CefRefPtr<CefV8Value> object,
            const CefV8ValueList& arguments,
            CefRefPtr<CefV8Value>& retval,
            CefString& except) {
            
    }
    
    void EventEmitterV8Handler::setMaxListeners(CefRefPtr<CefV8Value> object,
            const CefV8ValueList& arguments,
            CefRefPtr<CefV8Value>& retval,
            CefString& except) {
            
    }
    
    CefRefPtr<JsV8Handler> EventEmitterV8Handler::createV8Handler() {
        return new EventEmitterV8Handler();
    }
    
}

