#include "stdafx.h"
#include "module/dll/DllV8Handler.h"

#include "ipc/Any.hpp"
#include "ipc/IPCMessage.hpp"
#include "module/dll/DllManager.h"
#include "module/dll/DllRunner.h"
#include "module/basic/TypeConvertor.h"


namespace amo {



    DllV8Handler::DllV8Handler(const CefString& dllName)
        : m_dllName(dllName) {
        setHandlerName("dll");
    }
    
    IPCMessage::SmartType DllV8Handler::V8ToAnyMessage(
        const CefString& name,
        const CefV8ValueList& arguments) {
        
        CefRefPtr<CefV8Context> pContext = CefV8Context::GetCurrentContext();
        CefRefPtr<CefFrame> pFrame = pContext->GetFrame();
        IPCMessage::SmartType msg(new IPCMessage());
        msg->setMessageName(MSG_NATIVE_SYNC_EXECUTE);
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        auto manager = amo::DllManager<PID_RENDERER>::getInstance();
        std::string funcName = name.ToString();
        DllFunctionWrapper& wrapper = manager->getDllFunctionWrapper(m_dllName);
        FunctionArgs&  funcArgs = wrapper.getArgsList(funcName);
        
        for (size_t i = 0; i < funcArgs.m_vecArgsList.size(); ++i) {
            auto& p = funcArgs.m_vecArgsList[i];
            std::stringstream stream;
            stream << "(function(){ return " << p << ";})()";
            std::string js = stream.str();
            
            CefRefPtr<CefV8Value> retal;
            CefRefPtr<CefV8Exception> exp;
            
#if CHROME_VERSION_BUILD >= 2840
            bool bOk = pFrame->GetV8Context()->Eval(js,
                                                    CefString(),
                                                    0,
                                                    retal,
                                                    exp);
#else
            bool bOk = pFrame->GetV8Context()->Eval(js, retal, exp);
#endif
                                                    
                                                    
                                                    
                                                    
                                                    
            if (retal && retal->IsFunction()) {
                CefV8ValueList list;
                list.push_back(arguments[i]);
                CefRefPtr<CefV8Value>  ret;
                ret = retal->ExecuteFunctionWithContext(pFrame->GetV8Context(),
                                                        NULL,
                                                        list);
                                                        
                CefRefPtr<CefV8Value> funcID = ret->GetValue("id");
                
                if (funcID && funcID->IsString()) {
                    std::string ss = funcID->GetStringValue();
                    args->setValue(i, ss);
                }
            }
        }
        
        args->setValue(IPCArgsPosInfo::ArgsLength, (int)arguments.size());
        args->setValue(IPCArgsPosInfo::DllName, m_dllName.ToString());	// dll name
        args->setValue(IPCArgsPosInfo::DllFuncName, funcName); // function name
        args->setValue(IPCArgsPosInfo::DllRetalType,
                       funcArgs.m_strRetal);// 返回值类型
        args->setValue(IPCArgsPosInfo::FrameID, (int)pFrame->GetIdentifier());
        return msg;
    }
    
    bool DllV8Handler::Execute(const CefString& name,
                               CefRefPtr<CefV8Value> object,
                               const CefV8ValueList& arguments,
                               CefRefPtr<CefV8Value>& retval,
                               CefString& exception) {
        std::vector<FunctionWrapper> vec;
        //std::vector<FunctionWrapper>& vec = getFunctions();
        
        for (auto& p : vec) {
            if (name == p.m_strName) {
#ifdef	AMO_RUN_DLL
                IPCMessage::SmartType msg = V8ToAnyMessage(name, arguments);
                DllRunner runner(msg);
                Any any = runner.run();
                amo::TypeConvertor convertor;
                retval = convertor.toV8Value(any);
#endif
                return true;
            }
        }
        
        return JsClassObjectV8Handler::Execute(name,
                                               object,
                                               arguments,
                                               retval,
                                               exception);
    }
    
    void DllV8Handler::import(CefRefPtr<CefV8Value> object,
                              const CefV8ValueList& arguments,
                              CefRefPtr<CefV8Value>& retval,
                              CefString& except) {
        auto manager = amo::DllManager<PID_RENDERER>::getInstance();
        
        if (arguments.size() < 1) {
            return;
        }
        
        CefRefPtr<CefV8Value> val = arguments.at(0);
        
        if (val->IsString()) {
            manager->addArgsList(m_dllName, val->GetStringValue());
        } else if (val->IsArray()) {
            for (int i = 0; i < val->GetArrayLength(); ++i) {
                manager->addArgsList(m_dllName,
                                     val->GetValue(i)->GetStringValue());
            }
        }
        
    }
    
}

