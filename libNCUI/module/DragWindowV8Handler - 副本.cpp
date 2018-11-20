#include "stdafx.h"
#include "module/DragWindowV8Handler.h"

#include <regex>


#include "ipc/IPCMessage.hpp"
#include "ipc/RenderMessageEmitter.hpp"


namespace amo {

    void  DragWindowV8Handler::sendDragableToBrowserProcess(bool bEnabled) {
        CefRefPtr<CefV8Context> pContext = CefV8Context::GetCurrentContext();
        amo::RenderMessageEmitter runner(pContext->GetFrame());
        runner.setValue(IPCArgsPosInfo::TransferName, getHandlerName());
        runner.setValue(IPCArgsPosInfo::TransferID, getID());
        
        if (bEnabled) {
            runner.execute("enableDrag");
        } else {
            runner.execute("disableDrag");
        }
    }
    
    
    
    void DragWindowV8Handler::onmouseover(CefRefPtr<CefV8Value> target,
                                          const CefV8ValueList& args,
                                          CefRefPtr<CefV8Value>& retval,
                                          CefString& except) {
        // 递归
        if (target->IsUndefined() || target->IsNull() || !target->IsObject()) {
            return sendDragableToBrowserProcess(false);
        }
        
        // 优先使用 webkit-app-region
        std::string str = getWebkitAppRegion(target);
        
        if (!str.empty()) {
            if (str == "no-drag") {
                return sendDragableToBrowserProcess(false);
            } else if (str == "drag") {
                return sendDragableToBrowserProcess(true);
            }
        }
        
        
        //CefRefPtr<CefV8Value> style = target->GetValue("style");
        //
        //if (style && style->IsObject()) {
        //    std::vector<CefString> vecs;
        //    style->GetKeys(vecs);
        //
        //    for (auto& p : vecs) {
        //        OutputDebugStringA(p.ToString().c_str());
        //        OutputDebugStringA("\n");
        //
        //        CefRefPtr<CefV8Value> appRegion = style->GetValue(p);
        //
        //        if (appRegion && appRegion->IsString()) {
        //            OutputDebugStringA(appRegion->GetStringValue().ToString().c_str());
        //            OutputDebugStringA("\n");
        //        }
        //    }
        //
        //
        //
        //    //CefRefPtr<CefV8Value> appRegion = style->GetValue("webkitAppRegion");
        //    //
        //    //if (appRegion && appRegion->IsString()) {
        //    //    std::string str = appRegion->GetStringValue();
        //    //
        //    //    // 搞不定 取不到值
        //    //    if (!str.empty()) {
        //    //        if (str == "no-drag") {
        //    //            return sendDragableToBrowserProcess(false);
        //    //        } else if (str == "drag") {
        //    //            return sendDragableToBrowserProcess(true);
        //    //        }
        //    //    }
        //    //
        //    //}
        //
        //}
        
        //// 再判断 className
        //CefRefPtr<CefV8Value> className = target->GetValue("className");
        //std::string strClassList = className->GetStringValue().ToString();
        //
        //$clog(amo::cdevel << strClassList << amo::endl;);
        //std::regex reg(strRegex);
        //std::smatch m;
        //std::vector<std::string> class_list;
        //
        //while (std::regex_search(strClassList, m, reg)) {
        //    for (auto x = m.begin(); x != m.end(); x++) {
        //        $clog(amo::cdevel << x->str() << amo::endl;);
        //
        //        if (x->str() == noDragClassName) {
        //            return sendDragableToBrowserProcess(false);
        //        } else if (x->str() == dragClassName) {
        //            return sendDragableToBrowserProcess(true);
        //        }
        //    }
        //
        //    strClassList = m.suffix().str();
        //}
        
        // 旧的 class
        //{
        //    CefRefPtr<CefV8Value> className = target->GetValue("className");
        //    amo::string strClassList(className->GetStringValue().ToString(), true);
        //    //$clog(amo::cdevel << strClassList.to_ansi() << amo::endl;);
        //    std::vector<amo::string> vec = strClassList.split(" ");
        //
        //    for (auto& str : vec) {
        //        if (str == noDragClassName) {
        //            return sendDragableToBrowserProcess(false);
        //        } else if (str == dragClassName) {
        //            return sendDragableToBrowserProcess(true);
        //        }
        //    }
        //}
        
        
        CefRefPtr<CefV8Value> parent = target->GetValue("parentNode");
        
        onmouseover(parent, args, retval, except);
    }
    
    
    void DragWindowV8Handler::mouseover(CefRefPtr<CefV8Value> object,
                                        const CefV8ValueList& args,
                                        CefRefPtr<CefV8Value>& retval,
                                        CefString& except) {
        int nSize = args.size();
        
        if (nSize < 1) {
            return;
        }
        
        
        CefRefPtr<CefV8Value> target = args.at(0)->GetValue("target");
        onmouseover(target, args, retval, except);
    }
    
    
    std::string DragWindowV8Handler::getWebkitAppRegion(CefRefPtr<CefV8Value>
            object) {
        CefRefPtr<CefV8Context> pContext = CefV8Context::GetCurrentContext();
        CefRefPtr<CefV8Value> stringToObject;
        
        CefRefPtr<CefV8Value> pGlobal = pContext->GetGlobal();
        std::vector<CefString> keys;
        object->GetKeys(keys);
        
        
        
        /*for (auto& p : keys) {
            OutputDebugStringA(p.ToString().c_str());
            OutputDebugStringA("\n");
        }*/
        
        if (object->IsSame(pGlobal)) {
            return "no-drag";
        }
        
        
        stringToObject = pGlobal->GetValue("getCssText");
        
        
        if (!stringToObject) {
            return "no-drag";
        }
        
        CefV8ValueList list;
        list.push_back(object);
        CefRefPtr<CefV8Value> retval;
        retval = stringToObject->ExecuteFunctionWithContext(
                     pContext,
                     stringToObject,
                     list);
                     
        if (!retval && !retval->IsString()) {
            return "";
        }
        
        std::string cssText = retval->GetStringValue();
        
        if (cssText.find("-webkit-app-region: drag;") != -1) {
            return "drag";
        } else if (cssText.find("-webkit-app-region: no-drag;") != -1) {
            return "no-drag";
        }
        
        return "";
    }
    
    CefRefPtr<JsV8Handler> DragWindowV8Handler::createV8Handler() {
        return new DragWindowV8Handler();
    }
    
    void DragWindowV8Handler::dragable(CefRefPtr<CefV8Value> object,
                                       const CefV8ValueList& args,
                                       CefRefPtr<CefV8Value>& retval,
                                       CefString& except) {
                                       
        if (args.size() > 0 && args.at(0)->IsString()) {
        
            std::string dragName = args.at(0)->GetStringValue();
            
            if (!dragName.empty()) {
                dragClassName = dragName;
            }
        }
        
        if (args.size() > 1) {
            std::string noDragName = args.at(1)->GetStringValue();
            
            if (!noDragName.empty()) {
                noDragClassName = noDragName;
            }
        }
        
        
        
        strRegex =  dragClassName + "\\b|" + noDragClassName + "\\b";
        
        // 可以更新DragClassName
        if (m_bInit) {
            return;
        }
        
        CefRefPtr<CefBrowser> browser;
        browser = CefV8Context::GetCurrentContext()->GetBrowser();
        
        if (!browser) {
            return;
        }
        
        CefRefPtr<CefV8Context> pContext = CefV8Context::GetCurrentContext();
        CefRefPtr<CefV8Value> pGlobal = pContext->GetGlobal();
        CefRefPtr<CefV8Value> document = pGlobal->GetValue("document");
        
        if (!document) {
            return;
        }
        
        
        CefRefPtr<CefV8Value> listener = document->GetValue("addEventListener");
        
        // 监听鼠标消息
        if (listener->IsFunction()) {
            CefV8ValueList list;
            list.push_back(CefV8Value::CreateString("mouseover"));
            list.push_back(CefV8Value::CreateFunction("mouseover", this));
            listener->ExecuteFunction(document, list);
            m_bInit = true;
        }
        
        
    }
    
    DragWindowV8Handler::DragWindowV8Handler() {
        setHandlerName("DragWindow");
        m_bInit = false;
        dragClassName = "drag";
        noDragClassName = "no-drag";
    }
    
    DragWindowV8Handler::~DragWindowV8Handler() {
    }
    
}