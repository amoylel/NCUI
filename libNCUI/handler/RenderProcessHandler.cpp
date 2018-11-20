#include "stdAfx.h"

#include "handler/RenderProcessHandler.h"

#include <sstream>
#include <string>
#include <assert.h>



#include <amo/logger.hpp>

#include <amo/singleton.hpp>

#include "ipc/IPCMessage.hpp"
#include "handler/CefProcessExchanger.hpp"
#include "context/AppContext.h"
#include "handler/V8ExtentionHandler.h"

#include <include/cef_command_line.h>
#include "RenderProcessHandler.h"
#include "module/basic/V8HandlerManager.h"
#include "module/DragWindowV8Handler.h"
#include "module/dll/JsTypeMapHandler.h"
#include "module/EventEmitterV8Handler.h"
#include "module/IPCRendererV8Handler.h"
#include "transfer/TransferMgr.h"
#include "ipc/RenderMessageEmitter.hpp"
#include "module/basic/ClassMethodMgr.hpp"
#include "ui/win/BrowserWindowSkin.h"
#include "utility/utility.hpp"
#include "transfer/RendererTransferMgr.h"
#include "module/AppExHandler.h"
#include "ipc/BlobManager.hpp"



namespace amo {

    bool RenderProcessHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser>
            browser,
            CefProcessId source_process,
            CefRefPtr<CefProcessMessage> message) {
            
        assert(source_process == PID_BROWSER);
        std::string strMessageName = message->GetName();
        
        
        
        if (strMessageName == MSG_NATIVE_EXECUTE
                || strMessageName == MSG_NATIVE_SYNC_EXECUTE
                || strMessageName == MSG_NATIVE_ASYNC_EXECUTE) {
            int nBrowserID = browser->GetIdentifier();
            IPCMessage::SmartType  msg = amo::createAnyProcessMessage(message);
            
            
            if (RendererTransferMgr::getInstance()->onMessageTransfer(msg).isValid()) {
                return true;
            }
        }   else if (strMessageName == MSG_PROCESS_SYNC_EXECUTE) {
            RendererProcessExchangerManager::getInstance()->tryProcessMessage(
                browser->GetIdentifier());
        } else if (strMessageName == MSG_BROWSER_SETTINGS) {
            std::shared_ptr<BrowserWindowSettings> pSettings(new BrowserWindowSettings());
            
            pSettings->updateArgsSettings(message->GetArgumentList()->GetString(
                                              0).ToString());
                                              
            m_browserSettingsMap[browser->GetIdentifier()] = pSettings;
            return true;
        }
        
        
        bool handled = false;
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end() && !handled; ++it) {
            handled = (*it)->OnProcessMessageReceived(browser, source_process, message);
        }
        
        if (handled) {
            return true;
        }
        
        if (m_pMessageRouter) {
            handled = m_pMessageRouter->OnProcessMessageReceived(
                          browser, source_process, message);
        }
        
        return handled;
    }
    
    
    void RenderProcessHandler::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefDOMNode> node) {
            
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnFocusedNodeChanged(browser, frame, node);
        }
        
        auto iter = m_browserSettingsMap.find(browser->GetIdentifier());
        
        if (iter == m_browserSettingsMap.end()) {
            return;
        }
        
        // 只在离屏模式下计算元素位置
        if (!iter->second->offscreen) {
            return;
        }
        
        bool isEditable = IsEditableElement(node);
        int nID = frame->GetIdentifier();
        std::vector<int64_t> identifiers;
        frame->GetBrowser()->GetFrameIdentifiers(identifiers);
        
        std::shared_ptr<RenderMessageEmitter> runner(new RenderMessageEmitter(frame));
        runner->setValue(IPCArgsPosInfo::TransferName, "ipcMain");
        Any val = Undefined();
        
        if (isEditable) {
            IPCMessage::SmartType msg(new IPCMessage());
            auto args = msg->getArgumentList();
            args->setValue(IPCArgsPosInfo::FuncName, "runJSFunction");
            args->setValue(IPCArgsPosInfo::TransferName, "ipcRenderer");
            args->setValue(IPCArgsPosInfo::JsFuncName, "getActiveElementInfo");
            args->setValue(IPCArgsPosInfo::BrowserID, frame->GetBrowser()->GetIdentifier());
            args->setValue(IPCArgsPosInfo::FrameID, frame->GetIdentifier());
            args->setValue(IPCArgsPosInfo::ArgsLength, 0);
            args->setValue(IPCArgsPosInfo::MessageID, IPCMessage::getProcessMessageID());
            msg->setMessageName(MSG_NATIVE_EXECUTE);
            val = RendererTransferMgr::getInstance()->onMessageTransfer(msg);
        }
        
        if (val.is<amo::u8json>()) {
            runner->execute(MSG_FOCUSED_NODE_CHANGED, isEditable, val.As<amo::u8json>());
        } else {
            runner->execute(MSG_FOCUSED_NODE_CHANGED, isEditable);
        }
        
    }
    
    void RenderProcessHandler::OnUncaughtException(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context,
            CefRefPtr<CefV8Exception> exception,
            CefRefPtr<CefV8StackTrace> stackTrace) {
            
        CefString ss = exception->GetMessageW();
        
        $clog(amo::cdevel << func_orient << amo::string(
                  exception->GetMessageW().ToString(), true).str() << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnUncaughtException(browser, frame, context, exception, stackTrace);
        }
        
    }
    
    void RenderProcessHandler::OnContextReleased(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {
            
        $clog(amo::cdevel << func_orient << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnContextReleased(browser, frame, context);
        }
        
        if (m_pMessageRouter) {
            m_pMessageRouter->OnContextReleased(browser, frame, context);
        }
        
        V8ExtentionHandler* pHander = dynamic_cast<V8ExtentionHandler*>
                                      (m_pV8ExtensionHander.get());
                                      
        if (pHander != NULL) {
            pHander->OnContextReleased(frame->GetIdentifier());
        }
        
        int64_t nFrameID = frame->GetIdentifier();
        TypeConvertor::clearClassObject(nFrameID);
    }
    
    void RenderProcessHandler::OnContextCreated(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefV8Context> context) {
            
        $clog(amo::cdevel << func_orient << frame->GetIdentifier() << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnContextCreated(browser, frame, context);
        }
        
        if (m_pMessageRouter) {
            m_pMessageRouter->OnContextCreated(browser, frame, context);
        }
        
        
        CefRefPtr<CefV8Value> object = context->GetGlobal();
        int frame_id = static_cast<int>(frame->GetIdentifier());
        
        
        CefRefPtr<JsTypeMapHandler> handler = new JsTypeMapHandler();
        handler->getV8Object(object);
        
        int nBrowserID = browser->GetIdentifier();
        
        
#if CHROME_VERSION_BUILD <2704
        
        if (m_pV8ExtensionHander) {
            std::string url = frame->GetURL().ToString();
            
            if (!util::isDevUrl(url)) {
            
                IPCMessage::SmartType ipcMessage(new IPCMessage());
                ipcMessage->setMessageName(MSG_NATIVE_EXECUTE);
                std::shared_ptr<AnyArgsList>& args = ipcMessage->getArgumentList();
                args->setValue(IPCArgsPosInfo::TransferName, "ipcRenderer");
                args->setValue(IPCArgsPosInfo::JsFuncName,
                               "include(\"BrowserWindow\").currentWindow.dragable");
                args->setValue(IPCArgsPosInfo::FrameID, frame_id);
                args->setValue(IPCArgsPosInfo::BrowserID, nBrowserID);
                args->setValue(IPCArgsPosInfo::ArgsLength, 0);
                args->setValue(IPCArgsPosInfo::FuncName, "runJSFunction");
                m_pV8ExtensionHander->triggerEventOnRendererThread(ipcMessage);
            }
        }
        
#endif
        
        
    }
#if CEF_VERSION_REGION(0,3325)
    bool RenderProcessHandler::OnBeforeNavigation(CefRefPtr<CefBrowser> browser,
            CefRefPtr<CefFrame> frame,
            CefRefPtr<CefRequest> request,
            CefRenderProcessHandler::NavigationType navigation_type,
            bool is_redirect) {
            
        $clog(amo::cdevel << func_orient << amo::endl;);
        
        if (navigation_type == NAVIGATION_BACK_FORWARD) {
            auto iter = m_browserSettingsMap.find(browser->GetIdentifier());
            
            // 判断是否允许前进后退 屏蔽BackSpace回退页面
            if (iter != m_browserSettingsMap.end()
                    && !iter->second->back_forword) {
                return true;
            }
        } else    if (navigation_type == NAVIGATION_RELOAD) {
            auto iter = m_browserSettingsMap.find(browser->GetIdentifier());
            
            // 判断是否允许页面刷新
            if (iter != m_browserSettingsMap.end()
                    && !iter->second->reload) {
                return true;
            }
        }
        
        
        
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            if ((*it)->OnBeforeNavigation(browser, frame, request,
                                          navigation_type, is_redirect)) {
                return true;
            }
        }
        
        return false;
    }
#endif
    CefRefPtr<CefLoadHandler> RenderProcessHandler::GetLoadHandler() {
    
        $clog(amo::cdevel << func_orient << "GetLoadHandler。" << amo::endl;);
        CefRefPtr<CefLoadHandler> load_handler;
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end() && !load_handler.get(); ++it) {
            load_handler = (*it)->GetLoadHandler();
            
            if (load_handler) {
                break;
            }
        }
        
        return load_handler;
    }
    
    void RenderProcessHandler::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) {
    
        $clog(amo::cdevel << func_orient << "OnBrowserDestroyed" << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        m_browserSettingsMap.erase(browser->GetIdentifier());
        
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnBrowserDestroyed(browser);
        }
        
        RemoveBrowserByID(browser->GetIdentifier());
        RendererProcessExchangerManager::getInstance()->removeExchanger(
            browser->GetIdentifier());
            
        V8ExtentionHandler* pHander = dynamic_cast<V8ExtentionHandler*>
                                      (m_pV8ExtensionHander.get());
                                      
        if (pHander != NULL) {
            pHander->OnBrowserDestory(browser->GetIdentifier());
        }
        
        if (GetAnyBrowser() == NULL) {
            pHander->OnProcessDestory();
            ClassMethodMgr::getInstance()->clear();
            DllManager<PID_RENDERER>::getInstance()->clear();
        }
        
        
        
    }
    
    void RenderProcessHandler::createPipe(int nBrowserID,
                                          std::shared_ptr<ProcessExchanger> pExchanger) {
        //MessageBoxA(NULL, __FUNCTION__, __FUNCTION__, MB_OK);
        bool rec = pExchanger->getPipeServer()->connect();
        bool bOk = pExchanger->getPipeClient()->connect();
        /* CefPostTask(TID_RENDERER, NewCefRunnableMethod(this,
                     &RenderProcessHandler::afterCreatePipe,
                     browser, pExchanger));*/
        $clog(amo::cdevel << func_orient << ", ready" << amo::endl;);
        //MessageBoxA(NULL, "闪图", __FUNCTION__, MB_OK);
        
    }
    
    void RenderProcessHandler::afterCreatePipe(CefRefPtr<CefBrowser> browser,
            std::shared_ptr<ProcessExchanger> pExchanger, Any ret) {
        $clog(amo::cdevel << func_orient << ", start" << amo::endl;);
        int nBrowserID = browser->GetIdentifier();
        auto exchangerMgr = RendererProcessExchangerManager::getInstance();
        exchangerMgr->addExchanger(nBrowserID, pExchanger);
        
        if (!ret.isValid()) {
            ret = exchangerMgr->exchange <Any>(nBrowserID);
        }
        
        $clog(amo::cdevel << func_orient << "register:" << browser->GetIdentifier() <<
              amo::endl;);
              
        // 管道创建成功后再注册Browser
        RegisterBrowser(browser);
        
        // 注册Handler
        auto manager = V8HandlerManager::getInstance();
        manager->Register(nBrowserID, new  DragWindowV8Handler());
        manager->Register(nBrowserID, new EventEmitterV8Handler());
        manager->Register(nBrowserID, new AppExV8Handler());
        
        amo::u8json jsonArr(ret.value());
        std::vector<amo::u8json> arr = jsonArr.to_array();
        
        for (auto& json : arr) {
            FunctionWrapperMgr mgr = FunctionWrapperMgr::fromJson(json);
            
            std::string handlerName = mgr.name();
            CefRefPtr<JsV8Handler> pHandler;
            pHandler = manager->GetHandler(nBrowserID, handlerName);
            
            if (pHandler.get()) {
                pHandler->setFuncMgr(mgr);
            } else {
                ClassMethodMgr::getInstance()->addClass(handlerName, mgr);
            }
            
        }
        
        
        
        
        std::shared_ptr<IPCRendererV8Handler> pTransfer(new IPCRendererV8Handler());
        pTransfer->registerFunction();
        pTransfer->setBrowser(browser);
        pTransfer->setWorkOnRenderer(true);
        amo::RendererTransferMgr::getInstance()->addTransfer(browser->GetIdentifier(),
                pTransfer);
                
        // 这个IPCRendererV8Handler也应该加进去的。
        RendererTransferMgr::getInstance()->registerClass(nBrowserID);
        
        auto classManager = ClassMethodMgr::getInstance();
        
        // 添加渲染进程的transfer
        auto rendererTransferMap = RendererTransferMgr::getInstance()->getTransferMap(
                                       nBrowserID);
                                       
        for (auto& p : rendererTransferMap.transferMap()) {
            p.second->setWorkOnRenderer(true);
            p.second->getFuncMgr().setRendererClass(true);
            classManager->addClass(p.first, p.second->getFuncMgr());
        }
        
        
        
        CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(MSG_IPC_READY);
        browser->SendProcessMessage(PID_BROWSER, msg);
        //bool bOk  = (bool)exchangerMgr->Exchange <Any>(nBrowserID);
        
        $clog(amo::cdevel << func_orient << ", finish" << amo::endl;);
    }
    
    void RenderProcessHandler::RenderThreadActivityDetector() {
        //$clog(amo::cdevel << func_orient << "渲染进程活着" << amo::endl;);
        
        
        // 移除无效的数据
        BigStrManager<PID_RENDERER>::getInstance()->removeInvalidObject();
        BlobManager<PID_RENDERER>::getInstance()->removeInvalidObject();
        
#if CHROME_VERSION_BUILD >=2704
        CefPostDelayedTask(TID_RENDERER,
                           base::Bind(&RenderProcessHandler::RenderThreadActivityDetector, this),
                           10000);
#else
        CefPostDelayedTask(TID_RENDERER,
                           NewCefRunnableMethod(this,
                                                &RenderProcessHandler::RenderThreadActivityDetector),
                           10000);
#endif
                           
                           
    }
    
    
    void RenderProcessHandler::OnBrowserCreated(CefRefPtr<CefBrowser> browser) {
    
        AMO_TIMER_ELAPSED();
        $clog(amo::cdevel << func_orient << "create browser sucess。" << amo::endl;);
        amo::timer t;
        int nBrowserID = browser->GetIdentifier();
        
        auto manager = RendererProcessExchangerManager::getInstance();
        
        //manager->setDeadlockCallback()
        std::shared_ptr<ProcessExchanger> pExchanger;
        pExchanger = manager->findExchanger(nBrowserID);
        
        
        
        
        if (!pExchanger) {
        
            CefRefPtr<CefProcessMessage> msg;
            msg = CefProcessMessage::Create(MSG_CREATE_PIPE_CLIENT);
            CefRefPtr<CefListValue> args = msg->GetArgumentList();
            std::stringstream str;
            str << std::this_thread::get_id() << "_" << nBrowserID << std::endl;
            std::string strPipeServerName = RendererPipePrefix + str.str();
            std::string strPipeClientName = BrowserPipePrefix + str.str();
            args->SetString(0, str.str());
            
            std::shared_ptr<amo::pipe<amo::pipe_type::server> > m_pRenderPipeServer(
                new amo::pipe<amo::pipe_type::server>(strPipeServerName, DefaultPipeSize));
            std::shared_ptr<amo::pipe<amo::pipe_type::client> >  m_pBrowserPipeClient(
                new amo::pipe<amo::pipe_type::client>(strPipeClientName));
                
            //browser->SendProcessMessage(PID_BROWSER, msg);
            $clog(amo::cdevel << func_orient << "create pipe service:" <<
                  strPipeServerName << amo::endl;);
                  
            ////等待管道建立
            //bool rec = m_pRenderPipeServer->connect();
            //bool bOk = m_pBrowserPipeClient->connect();
            
            pExchanger.reset(new ProcessExchanger());
            pExchanger->setPipeServer(m_pRenderPipeServer);
            pExchanger->setPipeClient(m_pBrowserPipeClient);
            pExchanger->setProcessSyncMessageCallback(
                std::bind(&RenderProcessHandler::ProcessSyncMessage, this,
                          std::placeholders::_1, std::placeholders::_2));
            pExchanger->setBrowserID(browser->GetIdentifier());
            
            
            AMO_TIMER_ELAPSED();
            
            CefRefPtr<CefBrowser> pBrowser = GetAnyBrowser();
            std::shared_ptr<RenderMessageEmitter> runner;
            runner.reset(new RenderMessageEmitter(browser->GetMainFrame()));
            runner->setValue(IPCArgsPosInfo::TransferName, "ipcMain");
            
            if (!pBrowser) {
                //MessageBoxA(NULL, __FUNCTION__, __FUNCTION__, MB_OK);
                //MessageBoxA(NULL, str.str().c_str(), __FUNCTION__, MB_OK);
                int nBrowserID = browser->GetIdentifier();
                runner->execute("createPipeClient", str.str(), nBrowserID);
                
                // 如果是当前进程的第一个Browser,，那么直接创建管道
                createPipe(browser->GetIdentifier(), pExchanger);
                AMO_TIMER_ELAPSED();
                Any ret;
                afterCreatePipe(browser, pExchanger, ret);
                AMO_TIMER_ELAPSED();
            } else {
                /* std::shared_ptr<RenderMessageEmitter> runner;
                 runner.reset(new RenderMessageEmitter(pBrowser->GetMainFrame()));
                 runner->setValue(IPCArgsPosInfo::TransferName, "ipcMain");*/
                // 在单独的线程中连接管道
                // 不知什么原因，std::async不能创建线程
                /*    std::async(std::launch::async,
                			   std::bind(&RenderProcessHandler::createPipe,
                						 this,
                						 browser,
                						 pExchanger));*/
                int nBrowserID = browser->GetIdentifier();
                
                // 再死锁,就只能两个进程都放到单独的线程里进行管道连接
                //
                if (ClientHandler::SingleProcessMode()) {
                    auto emitter = RenderMessageEmitter(pBrowser->GetMainFrame());
                    emitter.createIPCMessage("createPipeClient", MSG_NATIVE_EXECUTE, str.str(),
                                             nBrowserID);
                                             
                    ClientHandler::createPipeClient(emitter.getIPCMessage());
                    createPipe(browser->GetIdentifier(), pExchanger);
                    AMO_TIMER_ELAPSED();
                    Any ret;
                    afterCreatePipe(browser, pExchanger, ret);
                } else {
                    //MessageBoxA(NULL, "8888", "", MB_OK);
                    //// 从进程模式下可以发送同步消息创建管道,这里可以永远调用不到
                    //std::thread th(std::bind(&RenderProcessHandler::createPipe,
                    //                         this,
                    //                         nBrowserID,
                    //                         pExchanger));
                    
                    //Any ret;
                    runner->execute("createPipeClient",
                                    str.str(),
                                    nBrowserID);
                                    
                    createPipe(browser->GetIdentifier(), pExchanger);
                    AMO_TIMER_ELAPSED();
                    Any ret;
                    afterCreatePipe(browser, pExchanger, ret);
                }
                
                
                
            }
        }
        
        
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnBrowserCreated(browser);
        }
        
        int64_t tt = t.elapsed();
        AMO_TIMER_ELAPSED();
    }
    
    
    
    void RenderProcessHandler::OnWebKitInitialized() {
    
    
        std::string s_extension(skinNCUI);
        
        $clog(amo::cdevel << func_orient <<
              "WebKitInitialized, Register NativeJS." << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnWebKitInitialized();
        }
        
        //创建Render进程MessageRouter
        CefMessageRouterConfig config;
        m_pMessageRouter = CefMessageRouterRendererSide::Create(config);
        
        CefRegisterExtension(L"v8/CEF-X", s_extension, m_pV8ExtensionHander);
    }
    
    void RenderProcessHandler::OnRenderThreadCreated(CefRefPtr<CefListValue>
            extra_info) {
        $clog(amo::cdevel << func_orient  << amo::endl;);
        DelegateSet::iterator it = m_Delegates.begin();
        
        for (; it != m_Delegates.end(); ++it) {
            (*it)->OnRenderThreadCreated(extra_info);
        }
        
        CefRefPtr<CefCommandLine> args = CefCommandLine::CreateCommandLine();
        args->SetProgram(CefCommandLine::GetGlobalCommandLine()->GetProgram());
        
        OnNodeCefCreated(*args.get());
        // 线程活动检测
        RenderThreadActivityDetector();
    }
    
    bool RenderProcessHandler::ProcessSyncMessage(int nID,
            IPCMessage::SmartType msg) {
        CefRefPtr<CefBrowser> pBrowser = GetBrowserByID(nID);
        $clog(amo::cdevel << func_orient << msg->toJson().to_string() << amo::endl;);
        
        if (!pBrowser) {
            return false;
        }
        
        CefRefPtr<CefProcessMessage> ipcMessage = amo::createCefProcessMessage(msg);
        bool bOk =  OnProcessMessageReceived(pBrowser, PID_BROWSER, ipcMessage);
        return bOk;
    }
    
    
    RenderProcessHandler::RenderProcessHandler() {
        m_pV8ExtensionHander = new V8ExtentionHandler();
        
        
        
    }
    
    RenderProcessHandler::~RenderProcessHandler() {
    
    }
    
    bool RenderProcessHandler::IsEditableElement(CefRefPtr<CefDOMNode> node) {
        if (!node.get() || !node->IsEditable()) {
            return false;
        }
        
        if (node->GetName() == "INPUT") {
            return true;
        }
        
        if (node->GetName() == "DIV") {
            CefString bStr = node->GetElementAttribute("contentEditable");
            
            if (bStr == "true" || bStr == "TRUE") {
                return true;
            }
            
            return false;
        }
        
        if (node->GetName() == "TEXTAREA"
                || node->GetElementAttribute("type") == "text"
                || node->GetElementAttribute("type") == "password") {
            return true;
        }
        
        return false;
    }
    
    
    void RenderProcessHandler::OnNodeCefCreated(CefCommandLine& args) {
        /*auto manager = ModuleManager::getInstance();
        manager->Register(new DragWindowV8Handler());
        manager->Register(m_pUIV8Handler);
        manager->Register(m_pRendererV8Handler);
        manager->Register(new AppV8Handler());
        manager->Register(new EventEmitterV8Handler());*/
    }
    
}

