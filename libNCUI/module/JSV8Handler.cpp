#include "stdafx.h"
#include "JSV8Handler.h"

namespace amo {



    JsV8Handler::JsV8Handler() {
        setHandlerName("JsV8Handler");
        setID(0);
    }
    
    JsV8Handler::~JsV8Handler() {
        return;
    }
    
    CefRefPtr<amo::JsV8Handler> JsV8Handler::createV8Handler() {
        return NULL;
    }
    
    void JsV8Handler::setHandlerName(const std::string& strName) {
        m_strHandlerName = strName;
    }
    
    const std::string& JsV8Handler::getHandlerName() const {
        return m_strHandlerName;
    }
    
    //void JsV8Handler::setFunctions(const std::vector<FunctionWrapper>& vec) {
    //    m_vecFunctions = vec;
    //}
    //
    //std::vector<amo::FunctionWrapper>& JsV8Handler::getFunctions() {
    //    return m_vecFunctions;
    //}
    
    /*   void JsV8Handler::setModules(const std::vector<std::string>& vec) {
           m_vecModules = vec;
       }
    
       std::vector<std::string>& JsV8Handler::getModules() {
           return m_vecModules;
       }*/
    
    CefRefPtr<CefV8Value> JsV8Handler::getV8Object(
        CefRefPtr<CefV8Value> object /*= NULL*/) {
        
        onGetV8Object(object);
        return object;
    }
    
    void JsV8Handler::onGetV8Object(CefRefPtr<CefV8Value> object) {
    
        return;
    }
    
    bool JsV8Handler::Execute(const CefString& name,
                              CefRefPtr<CefV8Value> object,
                              const CefV8ValueList& arguments,
                              CefRefPtr<CefV8Value>& retval,
                              CefString& exception) {
        std::string key = name.ToString(); // utf8
        
        if (m_oFuncMap.find(key) == m_oFuncMap.end()) {
            exception = ("invalid function.");
            // 如果当前handler没有注册该函数，返回FALSE
            return false;
        }
        
        m_oFuncMap[key](object, arguments, retval, exception);
        return true;
    }
    
    void JsV8Handler::addFunction(const std::string& key, FuncType fn) {
        m_oFuncMap[key] = fn;
    }
    
    std::vector<std::string> JsV8Handler::getAllKeys() {
        std::vector<std::string> vec;
        
        for (auto& p : m_oFuncMap) {
            vec.push_back(p.first);
        }
        
        /*    for (auto& p : getFunctions()) {
                vec.push_back(p.m_strName);
            }*/
        
        return vec;
    }
    
    const int64_t& JsV8Handler::getID() const {
        return m_nID;
    }
    
    void JsV8Handler::setID(const int64_t& val) {
        m_nID = val;
    }
    
    FunctionWrapperMgr& JsV8Handler::getFuncMgr() {
        return m_oFuncMgr;
    }
    
    void JsV8Handler::setFuncMgr(FunctionWrapperMgr& val) {
        m_oFuncMgr = val;
    }
    
}

