#include "stdafx.h"
#include "module/dll/DllRunner.h"
#include "module/dll/TypeMapManager.h"



namespace amo {
#ifdef AMO_RUN_DLL

    DllRunner::DllRunner(IPCMessage::SmartType msg)
        : m_args(msg->getArgumentList())
        , m_nErrorCode(DllNoError) {
        
    }
    
    Any DllRunner::run() {
        std::string retType(m_args->getString(IPCArgsPosInfo::DllRetalType));
        JS_TYPE nType = TypeMapManager::getInstance()->stringToJsType(retType);
        Any any = Undefined();
        
        switch (nType) {
        case JS_INVALID_TYPE:
        
            break;
            
        case JS_NIL:
            any = runDllFunc<amo::nil>(AnyArgsWrapper(m_args));
            break;
            
        case JS_INT:
            any = runDllFunc<int>(AnyArgsWrapper(m_args));
            break;
            
        case JS_INT8:
            break;
            
        case JS_INT16:
            break;
            
        case JS_INT32:
            break;
            
        case JS_INT64:
            break;
            
        case JS_UINT:
            break;
            
        case JS_UINT8:
            break;
            
        case JS_UINT16:
            break;
            
        case JS_UINT32:
            break;
            
        case JS_UINT64:
            any = (int)runDllFunc<DWORD>(AnyArgsWrapper(m_args));
            break;
            
        case JS_INT_PTR:
            any = (int)runDllFunc<int*>(AnyArgsWrapper(m_args));
            break;
            
        case JS_INT8_PTR:
            break;
            
        case JS_INT16_PTR:
            break;
            
        case JS_INT32_PTR:
            break;
            
        case JS_INT64_PTR:
            break;
            
            
        case JS_CONST_INT_PTR:
            break;
            
        case JS_CONST_INT8_PTR:
            break;
            
        case JS_CONST_INT16_PTR:
            break;
            
        case JS_CONST_INT32_PTR:
            break;
            
        case JS_CONST_INT64_PTR:
            break;
            
        default:
            break;
            
        }
        
        if (m_nErrorCode != DllNoError) {
            any = Undefined();
        }
        
        return any;
    }
    
    
#endif
    
}