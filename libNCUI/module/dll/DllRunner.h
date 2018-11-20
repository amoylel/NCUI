// Created by amoylel on 03/16/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_DLLRUNNER_H__
#define AMO_DLLRUNNER_H__

#include <string>
#include <memory>

#include <amo/loader.hpp>
#include <amo/string.hpp>

#include "handler/CefHeader.hpp"

#include "ipc/Any.hpp"
#include "ipc/IPCMessage.hpp"
#include "module/dll/DllManager.h"
#include "module/dll/TypeMapManager.h"


#include <tuple>

namespace amo {

#ifdef AMO_RUN_DLL

    // Cef Dll调用类
    class DllRunner {
    public:
    
        class AnyArgsWrapper {
        public:
            AnyArgsWrapper(std::shared_ptr<AnyArgsList> args) : m_args(args) {
                nCurrentPos = args->getInt(IPCArgsPosInfo::ArgsLength);
            }
            
            const int& size() const {
                return nCurrentPos;
            }
            void pop_back() {
                --nCurrentPos;
            }
            
            TypeItem& back() {
                auto manager = TypeMapManager::getInstance();
                return manager->get(m_args->getString(nCurrentPos - 1));
            }
            
        public:
            int nCurrentPos;
            std::shared_ptr<AnyArgsList> m_args;
        };
        
        enum DllErrorType {
            DllNoError = 0,
            DllNameError = 1,
            DllFuncError,
            DllArgsError,
            DllExecError,
            DllUnknownError,
        };
        
    public:
    
        DllRunner(IPCMessage::SmartType msg);
        
        template<typename R, typename ... Args>
        R runDllFuncImpl(Args... args) {
            try {
                auto manager = DllManager<PID_RENDERER>::getInstance();
                amo::u8string dllName(m_args->getString(IPCArgsPosInfo::DllName), true);
                amo::u8string funcName(m_args->getString(IPCArgsPosInfo::DllFuncName), true);
                std::shared_ptr<amo::loader> pLoader = manager->get(dllName);
                
                if (!pLoader) {
                    m_nErrorCode = DllNameError;
                    return R();
                }
                
                auto ret = pLoader->exec<R>(funcName.to_ansi(), args...);
                
                if (!ret) {
                    m_nErrorCode = DllArgsError;
                    return R();
                }
                
                return *ret;
            } catch (std::exception& e) {
                $clog(amo::cdevel << __FUNCTION__ << e.what() << amo::endl;);
                m_nErrorCode = DllExecError;
                return R();
            }
            
            m_nErrorCode = DllUnknownError;
            return R();
        }
        
        template<typename R, typename ... Args>
        R runDllFunc(AnyArgsWrapper valueWrapper, Args ... args) {
            try {
                if (valueWrapper.size() > 0) {
                    TypeItem&  val = valueWrapper.back();
                    valueWrapper.pop_back();
                    
                    switch (val.nType) {
                    case JS_INT: {
                        using type = typename IntToType<JS_INT32>::type;
                        return runDllFunc<R>(valueWrapper,
                                             boost::any_cast<type>(val.data),
                                             args...);
                    }
                    
                    case JS_DOUBLE: {
                        using type = typename IntToType<JS_DOUBLE>::type;
                        return runDllFunc<R>(valueWrapper,
                                             boost::any_cast<type>(val.data),
                                             args...);
                    }
                    
                    case JS_STRING: {
                        using type = typename IntToType<JS_STRING>::type;
                        return runDllFunc<R>(valueWrapper,
                                             boost::any_cast<type>(val.data),
                                             args...);
                    }
                    
                    case JS_BOOL: {
                        using type = typename IntToType<JS_BOOL>::type;
                        return runDllFunc<R>(valueWrapper,
                                             boost::any_cast<type>(val.data),
                                             args...);
                    }
                    
                    default:
                        m_nErrorCode = DllArgsError;
                        return R();
                    }
                } else {
                    return runDllFuncImpl<R>(args...);
                }
                
            } catch (std::exception& e) {
                m_nErrorCode = DllUnknownError;
                return R();
            }
            
            
            
            m_nErrorCode = DllUnknownError;
            return R();
        }
        
        // 展开终止函数，否则编译时会无限制的展开， 参数个数不能太多
        /*template<typename R, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
        R runDllFunc(AnyArgsWrapper valueWrapper, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6){
        	if (valueWrapper.size() == 0) return runDllFuncImpl<R>(p1, p2, p3, p4, p5, p6);
        	return R();
        }*/
        // 调试时使用，减少编译时间
        template<typename R, typename P1, typename P2, typename P3>
        R runDllFunc(AnyArgsWrapper valueWrapper, P1 p1, P2 p2, P3 p3) {
            if (valueWrapper.size() == 0) {
                return runDllFuncImpl<R>(p1, p2, p3);
            }
            
            return R();
        }
        
        Any run();
        
    protected:
    private:
        std::shared_ptr<AnyArgsList> m_args;
        DllErrorType m_nErrorCode;
    };
#endif
}


#endif // AMO_DLLRUNNER_H__
