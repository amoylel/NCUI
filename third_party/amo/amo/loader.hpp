// Created by amoylel on 14/06/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_LOADER_490968A9_27ED_43AB_BAB1_22AEF233F020_HPP__
#define AMO_LOADER_490968A9_27ED_43AB_BAB1_22AEF233F020_HPP__








#include <amo/config.hpp>

#ifdef OS_WIN
#include <windows.h>
#include <imagehlp.h>
#pragma comment(lib, "Dbghelp.lib")
#endif

#ifdef OS_LINUX
#include <dlfcn.h>
#endif

#include <string>
#include <utility>

#include <cstdlib>
#include <exception>
#include <string>
#include <amo/string.hpp>
#include <amo/template_macro.hpp>
#include <amo/optional.hpp>
#include <amo/nil.hpp>

#include <amo/memory.hpp>
#include <amo/stdint.hpp>
#include <amo/logger.hpp>
#include <boost/dll.hpp>
//#include <boost/dll/smart_library.hpp>
//#include <boost/dll/import_mangled.hpp>
//#include <boost/dll/import_class.hpp>

//#ifndef LOADER_CLASS_GEN0
//#ifdef OS_WIN
//
//
//
//
//#define LOADER_CLASS_GEN0(val)\
//template<TEMPLATE_PARAM_TYPE##val>\
//amo::optional<R> exec##val(const std::string& funcName COMMA##val FUNC_PARAM##val){\
//	if (is_nil<R>::value){\
//		if(m_nCallType == StdCall){\
//			get<void __stdcall(FUNC_PARAM_TYPE##val)>(funcName)(FUNC_PARAM_VAL##val);\
//		}\
//		else{\
//			get<void __cdecl(FUNC_PARAM_TYPE##val)>(funcName)(FUNC_PARAM_VAL##val);\
//			}\
//		return R();\
//	}\
//	else{\
//		if(m_nCallType == StdCall){\
//			return get<R __stdcall(FUNC_PARAM_TYPE##val)>(funcName)(FUNC_PARAM_VAL##val);\
//		}\
//		else{\
//			return get<R __cdecl(FUNC_PARAM_TYPE##val)>(funcName)(FUNC_PARAM_VAL##val);\
//		}\
//	}\
//}\
//
//#else
//
//#endif // OS_WIN
//
//#define LOADER_CLASS_GEN0(val)\
//template<TEMPLATE_PARAM_TYPE##val>\
//amo::optional<R> exec##val(const std::string& funcName COMMA##val FUNC_PARAM##val){\
//	if (is_nil<R>::value){\
//		get<void __cdecl(FUNC_PARAM_TYPE##val)>(funcName)(FUNC_PARAM_VAL##val);\
//		return R();\
//	}\
//	else{\
//		 return get<R __cdecl(FUNC_PARAM_TYPE##val)>(funcName)(FUNC_PARAM_VAL##val);\
//	}\
//}\
//
//#endif // !LOADER_CLASS_GEN0

namespace amo {

    enum CallType {
        StdCall,	//__stdcall
        Cdecl    //__cdecl
    };
    
    template<typename T> struct is_nil {
        const static bool value = false;
    };
    template<> struct is_nil <amo::nil> {
        const static bool value = true;
    };
    
    
    class loader : public boost::dll::shared_library {
    public:
        loader() {
        
        }
        
        loader(const std::string& lib_name_) {
            lib_name = amo::string_utils::ansi_to_wide(lib_name_);
        }
        
        loader(const amo::string& lib_name_)  {
            lib_name = lib_name_.to_wide();
        }
        
        loader(const amo::u8string& lib_name_) {
            lib_name = lib_name_.to_wide();
        }
        
        ~loader() {
        
        }
        
        /*  bool load(const std::string& lib_name_) {
        	  lib_name = amo::string_utils::ansi_to_wide(lib_name_);
        
        	  return load();
          }*/
        
        bool load(const std::wstring& lib_name_) {
            lib_name = lib_name_;
            return load();
        }
        
        bool load() {
            boost::system::error_code ec;
            boost::dll::shared_library::load(lib_name, ec);
            
            if (ec) {
                set_ast_error(ec.message());
                return false;
            }
            
            if (lib_name.find(L"C:\\windows") != -1) {
                m_nCallType = StdCall;
            } else {
                m_nCallType = Cdecl;
            }
            
            return true;
            
        }
        
        std::string get_last_error() const {
            return m_last_error;
        }
        void set_ast_error(std::string val) {
            m_last_error = val;
        }
        
        const std::string get_lib_name() const {
            return amo::string_utils::wide_to_ansi(lib_name);
        }
        
        const std::string get_lib_name_u8() const {
            return amo::string_utils::wide_to_utf8(lib_name);
        }
        
        const std::wstring get_lib_name_wide() const {
            return lib_name;
        }
    public:
    
        bool exports(std::vector<amo::string>& vec) {
            boost::dll::library_info inf(lib_name);
            std::vector<std::string> retval = inf.symbols();
            
            for (auto& p : retval) {
                vec.push_back(amo::string(p, false));
            }
            
            return true;
        }
        
        bool exports(std::vector<amo::u8string>& vec) {
            boost::dll::library_info inf(lib_name);
            std::vector<std::string> retval = inf.symbols();
            
            for (auto& p : retval) {
                vec.push_back(amo::u8string(p, false));
            }
            
            return true;
        }
        
        template<typename R, typename ... Args>
        amo::optional<R> exec(const std::string& funcName, Args... args) {
            try {
                std::string strType = typeid(R).name();
                
                if (!has(funcName)) {
                    return  amo::optional<R>();
                }
                
                if (is_nil<R>::value) {
                    auto fn = boost::dll::import<void(Args ...)>(*this, funcName);
                    fn(args...);
                    
                    return R();
                } else {
                    return boost::dll::import<R(Args ...)>(*this, funcName)(args...);
                }
            } catch (std::exception& e) {
                set_ast_error(e.what());
                return R();
            }
            
            return R();
            
        }
        
        
        
        template<typename R, typename ... Args>
        R getClass(const std::string& funcName, Args... args) {
            amo::optional<R> retval = exec<R, Args ...>(funcName, args...);
            
            if (!retval) {
                set_ast_error("getClass error:  invalid return value");
                return NULL;
            }
            
            return *retval;
            
        }
        
        //LOADER_CLASS_GEN0(0)
        
        
        template<typename R>
        amo::optional<R> exec0(const std::string& funcName) {
            try {
                std::string strType = typeid(R).name();
                
                if (!has(funcName)) {
                    return  amo::optional<R>();
                }
                
                if (is_nil<R>::value) {
                    auto fn = boost::dll::import<void()>(*this, funcName);
                    fn();
                    
                    return R();
                } else {
                    return boost::dll::import<R()>(*this, funcName)();
                }
            } catch (std::exception& e) {
                set_ast_error(e.what());
                amo::cerr << func_orient << e.what() << amo::endl;
                return R();
            }
            
            return R();
            
        }
        
        
    private:
    
        std::wstring lib_name;
        CallType m_nCallType;
        std::string m_last_error;
        
    };
}




#endif //AMO_LOADER_490968A9_27ED_43AB_BAB1_22AEF233F020_HPP__
