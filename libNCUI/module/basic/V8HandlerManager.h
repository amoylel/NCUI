// Created by amoylel on 09/15/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_V8HANDLERMANAGER_H__
#define AMO_V8HANDLERMANAGER_H__



#include <amo/singleton.hpp>
#include "handler/CefHeader.hpp"
#include "module/JSV8Handler.h"
#include <set>
namespace amo {

    /*!
     * @class	BuiltinModule
     *
     * @brief	内置模块 Handler.
     */
    
    class BuiltinModule {
    public:
        bool Register(CefRefPtr<JsV8Handler> module);
        CefRefPtr<JsV8Handler> GetHandler(CefString id);
        void Reset();
        CefRefPtr<JsV8Handler> createV8Handler(const std::string handlerName);
    private:
        std::set<CefRefPtr<JsV8Handler> > m_oBuiltin;
    };
    
    /*!
     * @class	V8HandlerManager
     *
     * @brief	模块管理器，各个Browser使用不同的全局Handler.
     */
    
    class V8HandlerManager : public amo::singleton<V8HandlerManager> {
    public:
        bool Register(int nBrowserID, CefRefPtr<JsV8Handler> module);
        CefRefPtr<JsV8Handler> GetHandler(int nBrowserID, CefString nModuleID);
        CefRefPtr<JsV8Handler> createV8Handler(int nBrowserID,
                                               const std::string& handlerName);
        void Reset(int nBrowserID);
        void Reset();
        BuiltinModule& getBrowserMoudle(int nBrowserID);
    private:
        std::unordered_map<int, BuiltinModule> m_oModuleMap;
    };
    
} // amo

#endif // AMO_V8HANDLERMANAGER_H__

