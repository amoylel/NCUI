
#include "stdafx.h"
#include "module/basic/V8HandlerManager.h"
#include <utility>



namespace amo {


    bool BuiltinModule::Register(CefRefPtr<JsV8Handler> module) {
        m_oBuiltin.insert(module);
        return true;
    }
    
    CefRefPtr<JsV8Handler> BuiltinModule::GetHandler(CefString id) {
    
        for (auto iter = m_oBuiltin.begin(); iter != m_oBuiltin.end(); ++iter) {
            CefRefPtr<JsV8Handler> ptr = *iter;
            CefString name = ptr->getHandlerName();
            amo::u8string s1(name.ToString(), true);
            amo::u8string s2(id.ToString(), true);
            
            if (s1 == s2) {
                return ptr;
            }
            
            //if (ptr->name() == id) return ptr;
        }
        
        return NULL;
    }
    
    void BuiltinModule::Reset() {
        m_oBuiltin.clear();
    }
    
    
    CefRefPtr<JsV8Handler> BuiltinModule::createV8Handler(const std::string handlerName) {
        for (auto& p : m_oBuiltin) {
            if (handlerName == p->getHandlerName()) {
                return p->createV8Handler();
            }
        }
        
        return NULL;
    }
    
    bool V8HandlerManager::Register(int nBrowserID,
                                    CefRefPtr<JsV8Handler> module) {
        return getBrowserMoudle(nBrowserID).Register(module);
    }
    
    CefRefPtr<JsV8Handler> V8HandlerManager::GetHandler(int nBrowserID,
            CefString nModuleID) {
        return getBrowserMoudle(nBrowserID).GetHandler(nModuleID);
    }
    
    CefRefPtr<JsV8Handler> V8HandlerManager::createV8Handler(int nBrowserID,
            const std::string& handlerName) {
        return getBrowserMoudle(nBrowserID).createV8Handler(handlerName);
    }
    
    void V8HandlerManager::Reset(int nBrowserID) {
        m_oModuleMap.erase(nBrowserID);
    }
    
    void V8HandlerManager::Reset() {
        m_oModuleMap.clear();
    }
    
    BuiltinModule& V8HandlerManager::getBrowserMoudle(int nBrowserID) {
        auto iter = m_oModuleMap.find(nBrowserID);
        
        if (iter != m_oModuleMap.end()) {
            return iter->second;
        }
        
        return m_oModuleMap.insert(std::make_pair(nBrowserID, BuiltinModule())).first->second;
    }
    
} // amo

