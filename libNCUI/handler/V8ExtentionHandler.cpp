#include "stdafx.h"

#include "handler/V8ExtentionHandler.h"

#include <thread>
#include <sstream>

#include "handler/CefHeader.hpp"
#include "module/JSV8Handler.h"
#include "transfer/ClassTransfer.hpp"

#include <amo/logger.hpp>
#include <amo/string.hpp>
#include <amo/directory.hpp>
#include "module/basic/V8HandlerManager.h"
#include "module/dll/DllManager.h"
#include "module/JsClassV8Handler.h"
#include "module/JsClassObjectV8Handler.h"
#include "module/dll/DllV8Handler.h"
#include "transfer/TransferMgr.h"
#include "module/basic/ClassMethodMgr.hpp"
#include "ipc/RenderMessageEmitter.hpp"
#include "handler/UtilityV8Handler.h"
#include "transfer/RendererTransferMgr.h"
#include "../settings/AppSettings.h"


namespace amo {

    bool V8ExtentionHandler::Execute(const CefString& name,
                                     CefRefPtr<CefV8Value> object,
                                     const CefV8ValueList& arguments,
                                     CefRefPtr<CefV8Value>& retval,
                                     CefString& exception) {
                                     
        if (name != "include"
                && name != "includes"
                && name != "renderer_modules"
                && name != "browser_modules") {
            // 不支持其他Native Function
            
            return m_pUtilityV8Handler->Execute(name, object, arguments, retval, exception);
        }
        
        
        
        // include 只能加载一个模块
        if ((name == "include")
                && (arguments.size() != 1
                    || !arguments.at(0)->IsString()
                    || arguments.at(0)->GetStringValue().empty())) {
            exception =
                L"include parameters can only be composed of strings and cannot be empty.";
            return true;
        }
        
        CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
        
        if (!context->IsValid()) {
            return false;
        }
        
        CefRefPtr<CefBrowser> pBrowser = context->GetBrowser();
        
        if (!pBrowser) {
            return false;
        }
        
        CefRefPtr<CefFrame> pFrame = context->GetFrame();
        
        if (!pFrame) {
            return false;
        }
        
        int nBrowserID = context->GetBrowser()->GetIdentifier();
        int64_t nFrameID = context->GetFrame()->GetIdentifier();
        
        if (m_oRegisteredSet.find(nBrowserID) == m_oRegisteredSet.end()) {
            m_oRegisteredSet.insert(nBrowserID);
            amo::u8directory dir(amo::u8path::getFullPathInExeDir("renderer_modules"));
            dir.transfer([&](amo::u8path & p) {
                if (p.is_directory()) {
                    return ;
                }
                
                if (p.find_extension() != ".dll") {
                    return ;
                }
                
                
                amo::u8string module(p.strip_path().remove_extension(), true);
                loadExternalTransfer(module, pBrowser);
            }, false);
            
        }
        
        
        
        CefV8ValueList list;
        
        for (size_t i = 0; i < arguments.size(); ++i) {
            CefRefPtr<CefV8Value> args = arguments.at(i);
            
            if (args->IsObject()) {
                // includes 有多个参数将会以object的方法我传递参数
                std::vector<CefString> keys;
                args->GetKeys(keys);
                
                for (auto& p : keys) {
                    list.push_back(args->GetValue(p));
                }
            } else if (args->IsString()) {
                list.push_back(args);
            } else {
                exception = L"the parameter must be a string.";
                return false;
            }
        }
        
        // 导入模块
        for (size_t i = 0; i < list.size(); ++i) {
            CefRefPtr<CefV8Value> args = list.at(i);
            
            if (!args || !args->IsString() || args->GetStringValue().empty()) {
                exception = L"the parameter must be a string and cannot be empty.";
                return true;
            }
            
            CefString module = args->GetStringValue();
            
            
            
            
            // 判断模块名是否合法
            if (module.size() > 255) {
                exception = L"module name is too long(>255).";
                return true;
            }
            
            
            
            
            // 先检查是否已经加载
            CefRefPtr<CefV8Value> pGlobal = context->GetGlobal();
            
            if (!pGlobal) {
                return false;
            }
            
            
            // 先看当前模块是否已经加载到全局变量
            CefRefPtr<CefV8Value> 	pNCUI = pGlobal->GetValue("NCUI");
            
            if (!pNCUI || pNCUI->IsUndefined() || pNCUI->IsNull()) {
#if CHROME_VERSION_BUILD >= 2840
                pNCUI = CefV8Value::CreateObject(NULL, NULL);
#else
                pNCUI = CefV8Value::CreateObject(NULL);
#endif
                
                pGlobal->SetValue("NCUI", pNCUI, V8_PROPERTY_ATTRIBUTE_NONE);
            }
            
            CefRefPtr<CefV8Value> pCache = CefV8Value::CreateUndefined();
            
            do {
                // 先看当前模块是否已经加载到全局变量
                pCache = pGlobal->GetValue(module);
                
                if (!pCache || pCache->IsUndefined() || pCache->IsNull()) {
                    pCache = pNCUI->GetValue(name);
                }
                
                if (pCache && !pCache->IsUndefined() && !pCache->IsNull()) {
                    break;
                }
                
                auto cache = std::make_pair(module.ToString(), nFrameID);
                
                // 防止 Depends 重置包含死循环
                if (m_oModuleSet.find(cache) != m_oModuleSet.end()) {
                    break;
                }
                
                m_oModuleSet.insert(cache);
                
                
                // 看内置Handler里面有没有 builtin
                auto pHandlerManager = V8HandlerManager::getInstance();
                CefRefPtr<JsV8Handler> pBuiltInHandler;
                pBuiltInHandler = pHandlerManager->GetHandler(nBrowserID, module);
                
                if (pBuiltInHandler) {
                    includeDepends(pBuiltInHandler->getFuncMgr().getDepends());
                    pCache = pBuiltInHandler->getV8Object();
                    break;
                }
                
                if (name != "browser_modules") {
                    pCache = includeFromRendererThread(module.ToString());
                    
                    if (pCache) {
                        break;
                    }
                }
                
                
                if (name != "renderer_modules") {
                    pCache = includeFromBrowserThread(module.ToString());
                    
                    if (!pCache) {
                        break;
                    }
                }
                
                
                
            } while (false);
            
            
            if (!pCache) {
                exception = module.ToWString() + L": failure of import module";
                return true;
            }
            
            auto classMethodMgr = ClassMethodMgr::getInstance();
            bool nonGlobalModule = true;
            
            // 将判断类管理器中是否存在的给类信息，如不存在说明是一个外部类
            if (classMethodMgr->hasClass(module)) {
                auto cc = classMethodMgr->getClass(module);
                nonGlobalModule = cc.isNonGlobalModule();
            }
            
            {
                bool bOK = pNCUI->SetValue(module,
                                           pCache,
                                           V8_PROPERTY_ATTRIBUTE_NONE);
                                           
                if (!bOK) {
                    exception = module.ToWString() + L": failure of import module";
                    return false;
                }
                
            }
            
            auto nonGlobalModules = getNonGlobalModules();
            
            // transfer 里面标志 为全局变量，且用户没有设置为非全局变量时才能导出为全局变量
            if (!nonGlobalModule && (nonGlobalModules.find(module) == nonGlobalModules.end())) {
                // 将模块设置为全局变量
                bool bOK = pGlobal->SetValue(module,
                                             pCache,
                                             V8_PROPERTY_ATTRIBUTE_NONE);
                                             
                if (!bOK) {
                    exception = module.ToWString() + L": failure of import module";
                    return false;
                }
            }
            
            
            
            
            retval = pCache;
            continue;
            
        }
        
        return true;
    }
    
    bool V8ExtentionHandler::includeDepends(const std::vector<std::string>& vec) {
    
        if (vec.empty()) {
            return false;
        }
        
        std::stringstream stream;
        stream << "includes(";
        
        for (size_t i = 0; i < vec.size(); ++i) {
            stream << "'" << vec[i] << "'";
            
            if (i < vec.size() - 1) {
                stream << ",";
            }
        }
        
        stream << ");";
        
        std::string js = stream.str();
        
        CefRefPtr<CefV8Value> retal;
        CefRefPtr<CefV8Exception> exp;
#if CHROME_VERSION_BUILD >= 2840
        bool bOk = CefV8Context::GetCurrentContext()->Eval(js,
                   CefString(),
                   0,
                   retal,
                   exp);
#else
        bool bOk = CefV8Context::GetCurrentContext()->Eval(js, retal, exp);
#endif
                   
                   
        return bOk;
        
    }
    
    void V8ExtentionHandler::OnContextReleased(int64 nFrameID) {
        std::vector<std::set<std::pair<std::string, int64_t> >::iterator> vec;
        
        for (auto iter = m_oModuleSet.begin(); iter != m_oModuleSet.end(); ++iter) {
            if (iter->second == nFrameID) {
                vec.push_back(iter);
            }
        }
        
        for (auto& p : vec) {
        
            m_oModuleSet.erase(p);
        }
        
        
    }
    
    void V8ExtentionHandler::OnBrowserDestory(int nBrowserID) {
        RendererTransferMgr::getInstance()->removeTransfer(nBrowserID);
        
        if (RendererTransferMgr::getInstance()->isEmpty()) {
        
            std::vector<std::pair<std::string, std::string> > vec;
            
            for (auto&p : m_oClassTransferMap) {
                vec.push_back({p.first, p.second->getModuleName() });
                
            }
            
            m_oClassTransferMap.clear();
            
            for (auto& p : vec) {
                std::string& moduleName = p.second;
                std::shared_ptr<amo::loader> loader;
                
                if (!moduleName.empty()) {
                    loader = DllManager<PID_RENDERER>::getInstance()->load(amo::u8string(moduleName, true));
                }
                
                if (loader) {
                    loader->exec<amo::nil, const std::string&>("removeTransferByName", p.first);
                } else {
                    ClassTransfer::removeTransferByName(p.first);
                }
            }
            
            //for (auto& p : m_oClassTransferMap) {
            //
            //    auto pppp =    p.second->getObjectName();
            //    std::cout << pppp << std::endl;
            //
            //    p.second->removeTransferByName(p.first);
            //    //ClassTransfer::removeTransferByName(p.first);
            //}
            //
            //m_oClassTransferMap.clear();
        }
    }
    
    /*void V8ExtentionHandler::OnBrowserDestory(int nBrowserID) {
        RendererTransferMgr::getInstance()->removeTransfer(nBrowserID);
    
        if (RendererTransferMgr::getInstance()->isEmpty()) {
            std::vector<std::string> vec;
    
            for (auto&p : m_oClassTransferMap) {
                vec.push_back(p.first);
    
            }
    
            m_oClassTransferMap.clear();
    
            for (auto& p : vec) {
                ClassTransfer::removeTransferByName(p);
            }
    
    
        }
    }*/
    
    void V8ExtentionHandler::OnProcessDestory() {
    
        std::vector<std::pair<std::string, std::string> > vec;
        
        for (auto&p : m_oClassTransferMap) {
            vec.push_back({ p.first, p.second->getModuleName() });
            
        }
        
        m_oClassTransferMap.clear();
        
        for (auto& p : vec) {
            std::string& moduleName = p.second;
            std::shared_ptr<amo::loader> loader;
            
            if (!moduleName.empty()) {
                loader = DllManager<PID_RENDERER>::getInstance()->load(amo::u8string(moduleName, true));
            }
            
            if (loader) {
                loader->exec<amo::nil, const std::string&>("removeTransferByName", p.first);
            } else {
                ClassTransfer::removeTransferByName(p.first);
            }
        }
        
        /*for (auto& p : m_oClassTransferMap) {
        	ClassTransfer::removeTransferByName(p.first);
        }
        
        m_oClassTransferMap.clear();*/
    }
    
    void V8ExtentionHandler::triggerEventOnRendererThread(IPCMessage::SmartType
            msg) {
        CefPostTask(TID_RENDERER,
                    base::Bind(&V8ExtentionHandler::triggerEventOnRendererThreadImpl,
                               this,
                               msg));
                               
    }
    
    void V8ExtentionHandler::triggerEventOnRendererThreadImpl(
        IPCMessage::SmartType msg) {
        std::unordered_map<int, CefRefPtr<CefBrowser> > mp
            = BrowserManager<PID_RENDERER>::GetAllBrowser();
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        
        if (args->getInt(IPCArgsPosInfo::BrowserID) > 0) {
            // 如果指定了浏览器ID，那么直接执行
            RendererTransferMgr::getInstance()->onMessageTransfer(msg);
        } else {
            // 如果没有指定浏览器ID，则向所有浏览器发送
            for (auto& p : mp) {
            
                int nBrowserID = p.second->GetIdentifier();
                int64_t nFrameID = p.second->GetMainFrame()->GetIdentifier();
                args->setValue(IPCArgsPosInfo::BrowserID, nBrowserID);
                args->setValue(IPCArgsPosInfo::FrameID, nFrameID);
                RendererTransferMgr::getInstance()->onMessageTransfer(msg);
            }
        }
        
        
        
        
    }
    
    std::set<std::string> V8ExtentionHandler::getNonGlobalModules() {
        std::set<std::string> retval;
        auto settings = AppSettings::getInstance();
        auto str = 	settings->toJson().to_string();
        amo::u8json arr = settings->nonGlobalModules;
        
        if (!arr.is_array()) {
            return retval;
        }
        
        std::vector<amo::u8json> vec = arr.to_array();
        
        for (auto& p : vec) {
            if (!p.is_string()) {
                continue;
            }
            
            retval.insert(p.get<std::string>());
        }
        
        return retval;
    }
    
    V8ExtentionHandler::V8ExtentionHandler() {
        m_pUtilityV8Handler = new UtilityV8Handler();
        
    }
    
    V8ExtentionHandler::~V8ExtentionHandler() {
        m_oClassTransferMap.clear();
    }
    
    void  V8ExtentionHandler::registerExternalTransfer(int nBrowserID,
            std::shared_ptr<ClassTransfer> pTransfer) {
            
        // 可能找到错误的DLL
        if (pTransfer == NULL) {
            return  ;
        }
        
        
        m_oClassTransferMap.insert(std::make_pair(pTransfer->transferName(),
                                   pTransfer));
        // 设置事件回调函数
        pTransfer->setTriggerEventFunc(
            std::bind(&V8ExtentionHandler::triggerEventOnRendererThread,
                      this,
                      std::placeholders::_1));
                      
        //TODO:有大问题，这里只能加到一个BrowserID里面去，实际上需要添加到所有的里面去还是不需要？？？
        // 注册外部模块到程序中
        RendererTransferMgr::getInstance()->addTransfer(nBrowserID, pTransfer);
        
        
        std::string strClass = pTransfer->transferName();
        auto mananger = RendererTransferMgr::getInstance();
        TransferMap& transferMap = mananger->getTransferMap(nBrowserID);
        
        
        
        FunctionWrapperMgr& mgr = pTransfer->getFuncMgr();
        mgr.setRendererClass(true);
        mgr.setBuiltIn(false);
        FunctionWrapperMgr mgr2 = mgr;
        
        for (auto& p : mgr2.toMap()) {
            p.second.m_fn = FunctionWrapper::TransferFunc();
        }
        
        auto classMethodMgr = ClassMethodMgr::getInstance();
        classMethodMgr->addClass(strClass, mgr2);
    }
    
    bool V8ExtentionHandler::loadExternalTransfer(const std::string& u8ClassName,
            CefRefPtr<CefBrowser> browser) {
            
        AMO_TIMER_ELAPSED();
        // 从磁盘中加载与所给模块同名dll
        std::shared_ptr<amo::loader> pLoader;
        pLoader = DllManager<PID_RENDERER>::getInstance()->load(amo::u8string(u8ClassName, true));
        
        
        if (!pLoader) {
            return NULL;
        }
        
        // 外部模块必须提供registerTransfer函数
        int nBrowserID = browser->GetIdentifier();
        std::shared_ptr< TransferRegister> info(new TransferRegister());
        info->nBrowserID = nBrowserID;
        info->fnCallback = std::bind(&V8ExtentionHandler::registerExternalTransfer,
                                     this,
                                     std::placeholders::_1,
                                     std::placeholders::_2);
        info->moduleName = u8ClassName;
        
        auto options = pLoader->exec<bool, std::shared_ptr<TransferRegister>> (
                           "registerTransfer",
                           info);
                           
        // 判断外部模块是否注册成功
        if (!options || !*options) {
            return false;
        }
        
        AMO_TIMER_ELAPSED();
        return true;
    }
    
    CefRefPtr<JsV8Handler> V8ExtentionHandler::fromClass(const std::string&
            strClass,
            CefRefPtr<CefBrowser> browser) {
            
        // 当前为一个class
        
        auto classMethodMgr = ClassMethodMgr::getInstance();
        
        // 将判断类管理器中是否存在的给类信息，如不存在说明是一个外部类
        if (!classMethodMgr->hasClass(strClass)) {
        
            if (!loadExternalTransfer(strClass, browser)) {
                return NULL;
            }
            
            int nBrowserID = browser->GetIdentifier();
            auto mananger = RendererTransferMgr::getInstance();
            TransferMap& transferMap = mananger->getTransferMap(nBrowserID);
            Transfer* pTransfer = transferMap.findTransfer(strClass);
            
            // 可能找到错误的DLL
            if (pTransfer == NULL) {
                return NULL;
            }
            
            FunctionWrapperMgr& mgr = classMethodMgr->getClass(strClass);
            
            CefRefPtr<JsClassV8Handler> pHandler = new JsClassV8Handler();
            pHandler->setRendererClass(true);
            pHandler->setHandlerName(strClass);
            pHandler->setID(mgr.getObjectID());
            pHandler->setFuncMgr(mgr);
            /*  m_oClassHandler.insert(std::make_pair(std::pair<std::string, int>(
            		  strClass, browser->GetIdentifier()), pHandler));*/
            return pHandler;
        } else {
        
            int nBrowserID = browser->GetIdentifier();
            auto manager = RendererTransferMgr::getInstance();
            
            FunctionWrapperMgr& mgr = classMethodMgr->getClass(strClass);
            
            // 判断是否为外部模块
            if (!mgr.isBuiltIn()) {
                auto iter = m_oClassTransferMap.find(strClass);
                
                if (iter == m_oClassTransferMap.end()) {
                    // 重新加载
                    
                    return NULL;
                }
                
                
                manager->addTransfer(nBrowserID, iter->second);
            }
            
            
            TransferMap& transferMap = manager->getTransferMap(nBrowserID);
            Transfer* pTransfer = transferMap.findTransfer(strClass);
            
            
            if (pTransfer != NULL && pTransfer->isWorkOnRenderer()) {
                // 设置事件回调函数
                pTransfer->setTriggerEventFunc(
                    std::bind(&V8ExtentionHandler::triggerEventOnRendererThread,
                              this,
                              std::placeholders::_1));
                              
            }
            
            CefRefPtr<JsClassV8Handler> pHandler = new JsClassV8Handler();
            pHandler->setHandlerName(strClass);
            
            pHandler->setRendererClass(mgr.isRendererClass());
            pHandler->setID(mgr.getObjectID());
            pHandler->setFuncMgr(mgr);
            
            return pHandler;
        }
        
        
    }
    
    
    
    CefRefPtr<CefV8Value> V8ExtentionHandler::includeFromRendererThread(
        const std::string& module) {
        
        CefRefPtr<CefV8Value> pCache = NULL;
        CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
        int nBrowserID = context->GetBrowser()->GetIdentifier();
        auto pHandlerManager = V8HandlerManager::getInstance();
        
        // 类 class
        CefRefPtr<JsV8Handler> pClassHandler;
        pClassHandler = fromClass(module, context->GetBrowser());
        
        if (pClassHandler) {
            includeDepends(pClassHandler->getFuncMgr().getDepends());
            // 从这里面取出来的Handler可以是一个类，也可以是一个全局对象，他们是可以缓存的
            pHandlerManager->Register(nBrowserID, pClassHandler);
            pCache = pClassHandler->getV8Object();
            return pCache;
        } else {
        
        
            // 从磁盘中查找， dll
            
            amo::u8string dllName(module, true);
            std::vector<amo::u8string> vec;
            // 从dll中导出c函数为JS函数
            std::shared_ptr<amo::loader> pLoader;
            pLoader = DllManager<PID_RENDERER>::getInstance()->load(dllName);
            
            if (!pLoader) {
                return NULL;
            }
            
            vec = DllManager<PID_RENDERER>::getInstance()->exports(dllName);
            
            std::vector<FunctionWrapper> functions;
            
            for (auto& p : vec) {
                FunctionWrapper fw;
                fw.m_strName = p.to_utf8();
                fw.m_nExecType = 0;
                functions.push_back(fw);
            }
            
            CefRefPtr<DllV8Handler> retvalHandler;
            retvalHandler = new DllV8Handler(module);
            includeDepends(retvalHandler->getFuncMgr().getDepends());
            //TODO: DLL 功能已放弃
            //retvalHandler->setFunctions(functions);
            pCache = retvalHandler->getV8Object();
        }
        
        return pCache;
    }
    
    
    CefRefPtr<CefV8Value>  V8ExtentionHandler::includeFromBrowserThread(
        const std::string& module) {
        
        CefRefPtr<CefV8Value> pCache = NULL;
        CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();
        int nBrowserID = context->GetBrowser()->GetIdentifier();
        auto pHandlerManager = V8HandlerManager::getInstance();
        
        
        CefRefPtr<CefFrame> pFrame = context->GetFrame();
        
        /*  CefRefPtr<JsV8Handler> pClassHandler;
          pClassHandler = fromClass(module, context->GetBrowser());*/
        
        std::shared_ptr<RenderMessageEmitter> runner(new RenderMessageEmitter(pFrame));
        runner->setValue(IPCArgsPosInfo::TransferName, "ipcMain");
        Any ret = runner->syncExecute("onInclude", module);
        
        if (!ret.isValid()) {
            return NULL;
        }
        
        amo::u8json json = (amo::u8json)ret;
        
        if (ret.type() != AnyValueType<amo::u8json>::value) {
            return NULL;
        }
        
        //如果返回的是一个数组，那么说明是Dll
        if (json.is_array()) {
            amo::u8string dllName(module, true);
            std::vector<amo::u8json> vec = json.to_array();
            
            
            
            std::vector<FunctionWrapper> functions;
            
            for (auto& p : vec) {
                FunctionWrapper fw;
                fw.m_strName = amo::u8string(p.to_string(), true).to_utf8();
                fw.m_nExecType = 0;
                functions.push_back(fw);
            }
            
            CefRefPtr<DllV8Handler> retvalHandler;
            retvalHandler = new DllV8Handler(module);
            //TODO: DLL 功能已放弃
            includeDepends(retvalHandler->getFuncMgr().getDepends());
            //retvalHandler->setFunctions(functions);
            pCache = retvalHandler->getV8Object();
        } else {
            // 解析类函数
            FunctionWrapperMgr funcWrapperMgr = FunctionWrapperMgr::fromJson(json);
            // 加入类管理
            ClassMethodMgr::getInstance()->addClass(module, funcWrapperMgr);
            // 从class中创建Handler
            CefRefPtr<JsV8Handler> pClassHandler;
            pClassHandler = fromClass(module, context->GetBrowser());
            
            if (pClassHandler == NULL) {
                return NULL;
            }
            
            includeDepends(pClassHandler->getFuncMgr().getDepends());
            pCache = pClassHandler->getV8Object();
        }
        
        return pCache;
    }
    
}