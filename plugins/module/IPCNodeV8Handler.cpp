

#include <amo/json.hpp>
#include "IPCNodeV8Handler.h"
#include "NodeTypeConvertor.h"


namespace amo {

    IPCNodeV8Handler::IPCNodeV8Handler()
        : ClassTransfer("ipcRenderer") {
        
    }
    
    IPCNodeV8Handler::~IPCNodeV8Handler() {
    
    }
    
    
    
    amo::Any IPCNodeV8Handler::asyncExecuteResult(IPCMessage::SmartType msg) {
    
        /*amo::TypeConvertor convertor;
        CefRefPtr<CefV8Value> retval = convertor.ParseResult(msg->getArgumentList()->GetValue(0));
        
        int id = msg->getArgumentList()->GetInt(IPCArgsPosInfo::AsyncCallback);
        AsyncFunctionWrapper& item = AsyncFunctionManager<PID_RENDERER>::get_instance()->Get(id);
        if (item.m_pFrame == NULL || item.m_pFunction == NULL) return amo::NonReturn();
        CefV8ValueList args;
        args.push_back(retval);
        
        CefRefPtr<CefV8Context> context = item.m_pFrame->GetV8Context();
        item.m_pFunction->ExecuteFunctionWithContext(context, item.m_pFunction, args);
        AsyncFunctionManager<PID_RENDERER>::get_instance()->UnRegisterCallbackFunction(id);
        return amo::NonReturn();*/
        
        return Undefined();
    }
    
    int IPCNodeV8Handler::getArgsSize(std::shared_ptr<AnyArgsList> args) const {
        size_t nSize = 0;
        
        for (auto& p : args->getAllValues()) {
            if (p.first < IPCArgsPosInfo::FuncName) {
                ++nSize;
            }
        }
        
        return nSize;
    }
    
    
    
    amo::Any IPCNodeV8Handler::runJSFunction(IPCMessage::SmartType anyMessage) {
    
        /*std::shared_ptr<amo::AnyArgsList> args = anyMessage->getArgumentList();
        int nFrameId = args->GetInt(IPCArgsPosInfo::FrameID);
        std::string jsFunctionName = args->GetString(IPCArgsPosInfo::JsFunctionName);
        std::stringstream stream;
        stream << "(function(){return " << jsFunctionName << ";})()";
        
        std::string js = stream.str();
        
        CefRefPtr<CefV8Value> retal;
        CefRefPtr<CefV8Exception> exp;
        CefRefPtr<CefFrame> pFrame = m_pBrowser->GetFrame(nFrameId);
        if (!pFrame) return amo::Undefined();
        bool bOk = pFrame->GetV8Context()->Eval(js, retal, exp);
        
        if (bOk && retal && retal->IsFunction()){
        	CefV8ValueList list;
        	for (int i = 0; i < getArgsSize(args); ++i) {
        		list.push_back(JsConvertor::toV8Value(args, i));
        	}
        	CefRefPtr<CefV8Value> v8Value = retal->ExecuteFunctionWithContext(
        		pFrame->GetV8Context(), NULL, list);
        	return JsConvertor::toAny(v8Value, pFrame);
        }*/
        
        
        return amo::Undefined();
    }
    
    amo::Any IPCNodeV8Handler::runJsFragment(IPCMessage::SmartType anyMessage) {
        /*std::shared_ptr<amo::AnyArgsList> args = anyMessage->getArgumentList();
        int nFrameId = args->GetInt(IPCArgsPosInfo::FrameID);
        CefRefPtr<CefFrame> pFrame = m_pBrowser->GetFrame(nFrameId);
        if (!pFrame) return amo::Undefined();
        
        CefRefPtr<CefV8Value> retal;
        CefRefPtr<CefV8Exception> exp;
        
        std::string js = amo::string(args->GetString(0), true).to_utf8();
        
        bool bOk = pFrame->GetV8Context()->Eval(js, retal, exp);
        if (!bOk || !retal)  return amo::Undefined();
        return  JsConvertor::toAny(retal, pFrame);*/
        
        return amo::Undefined();
    }
    
    amo::Any IPCNodeV8Handler::triggerEvent(IPCMessage::SmartType msg) {
        std::shared_ptr<amo::AnyArgsList> args = msg->getArgumentList();
        int32_t nBrowserID = args->getInt(IPCArgsPosInfo::BrowserID);
        int nFrameId = args->getInt(IPCArgsPosInfo::FrameID);
        
        int64_t nObjectID = args->getInt64(IPCArgsPosInfo::EventObjectID);
        NodeV8Handler*  pValue = NodeTypeConvertor::getClassObject(nObjectID);
        
        if (pValue == NULL) {
            return Undefined();
        }
        
        return pValue->emit(msg);
        
        
    }
    
    amo::Any IPCNodeV8Handler::emitEventAllFrame(IPCMessage::SmartType msg) {
        return triggerEvent(msg);
    }
    
}

