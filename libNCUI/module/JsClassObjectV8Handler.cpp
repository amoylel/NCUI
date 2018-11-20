#include "stdafx.h"
#include "module/JsClassObjectV8Handler.h"

#include "module/basic/V8HandlerManager.h"

namespace amo {

    class JSExecV8Handler : public CefV8Handler {
    public:
        JSExecV8Handler()   {}
        ~JSExecV8Handler() {
        
        }
        static CefRefPtr<JSExecV8Handler> GetHandler() {
            static CefRefPtr<JSExecV8Handler> pHandler(new JSExecV8Handler());
            return pHandler;
        }
        virtual bool Execute(const CefString& name, CefRefPtr<CefV8Value> object,
                             const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval,
                             CefString& exception) override {
                             
                             
            JsClassV8Handler* pHandler = dynamic_cast<JsClassV8Handler*>
                                         (object->GetUserData().get());
                                         
            if (pHandler == NULL) {
                return false;
            }
            
            pHandler->Execute(name, object, arguments, retval, exception);
            return true;
            
            
        }
        IMPLEMENT_REFCOUNTING(JSExecV8Handler);
        
    };
    
    JsClassObjectV8Handler::JsClassObjectV8Handler() {
        setHandlerName("JsClassObjectV8Handler");
        
        //++ffff;
    }
    
    JsClassObjectV8Handler::~JsClassObjectV8Handler() {
    
    
    }
    
    CefRefPtr<CefV8Value> JsClassObjectV8Handler::getV8Object(
        CefRefPtr<CefV8Value> object /*= NULL*/) {
        
        // 不理会object是否为空，我们现在创建的是一个类
        CefRefPtr<CefV8Value> _object;
        
        
        
        // 创建JS类的构造函数
#if CHROME_VERSION_BUILD >= 2840
        _object = CefV8Value::CreateObject(m_pAccessor, NULL);
#else
        _object = CefV8Value::CreateObject(m_pAccessor);
#endif
        
        _object->SetUserData(this);
        
        onGetV8Object(_object);
        
        
        return _object;
    }
    
    void JsClassObjectV8Handler::onGetV8Object(CefRefPtr<CefV8Value> object) {
    
        std::string strObjectName = getFuncMgr().getObjectName();
        bool bAssitObject = (strObjectName.find("CLASS.") == 0);
        
        //CLASS.对象用来代替CLASS来的属性无法设置的功能，所有应该具有这些类的功能
        //CefRefPtr<CefV8Handler> pFuncHandler(new JSExecV8Handler(getID()));
        
        for (auto& p : m_oFuncMgr.toMap()) {
            // 类
            if (bAssitObject) {
                if (p.second.functionType() == TransferFuncStatic) {
                    CefRefPtr<CefV8Value> pFunc = CefV8Value::CreateFunction(p.second.m_strName,
                                                  JSExecV8Handler::GetHandler());
                    pFunc->SetUserData(this);
                    // 创建静态函数
                    object->SetValue(p.second.m_strName,
                                     pFunc,
                                     V8_PROPERTY_ATTRIBUTE_NONE);
                } else if (p.second.functionType() == TransferFuncConstProperty) {
                    object->SetValue(p.second.m_strName,
                                     V8_ACCESS_CONTROL_DEFAULT,
                                     V8_PROPERTY_ATTRIBUTE_NONE);
                                     
                } else if (p.second.functionType() == TransferFuncClassProperty) {
                    object->SetValue(p.second.m_strName,
                                     V8_ACCESS_CONTROL_DEFAULT,
                                     V8_PROPERTY_ATTRIBUTE_NONE);
                }
            } else {
                // 对象
                if (p.second.functionType() == TransferFuncNormal) {
                    CefRefPtr<CefV8Value> pFunc = CefV8Value::CreateFunction(p.second.m_strName,
                                                  JSExecV8Handler::GetHandler());
                    pFunc->SetUserData(this);
                    object->SetValue(p.second.m_strName,
                                     pFunc,
                                     V8_PROPERTY_ATTRIBUTE_NONE);
                                     
                                     
                } else if (p.second.functionType() == TransferFuncMemberProperty) {
                    object->SetValue(p.second.m_strName,
                                     V8_ACCESS_CONTROL_DEFAULT,
                                     V8_PROPERTY_ATTRIBUTE_NONE);
                }
            }
            
        }
        
        CefRefPtr<CefV8Context> pContext = CefV8Context::GetCurrentContext();
        int nBrowserID = pContext->GetBrowser()->GetIdentifier();
        auto manager = V8HandlerManager::getInstance();
        
        // 添加依赖模块
        for (auto& moduleName : m_oFuncMgr.getModules()) {
            CefRefPtr<JsV8Handler> pHandler;
            pHandler = manager->createV8Handler(nBrowserID, moduleName);
            
            if (pHandler.get() == NULL) {
                continue;
            }
            
            pHandler->setHandlerName(getHandlerName());
            pHandler->setID(getID());
            pHandler->getV8Object(object);
        }
        
        std::vector<Any>& attributes = m_oFuncMgr.getAttributes();
        
        for (auto& p : attributes) {
            amo::u8json json = p;
            std::string strName = json.getString("name");
            Any val = Any::fromJson(json.get_child("value"));
            TypeConvertor convertor;
            
            object->SetValue(strName,
                             convertor.toV8Value(val),
                             V8_PROPERTY_ATTRIBUTE_NONE);
        }
        
        std::string jsonStr = getFuncMgr().toSimplifiedJson().to_string();
        object->SetValue("transferJson",
                         CefV8Value::CreateString(jsonStr),
                         V8_PROPERTY_ATTRIBUTE_READONLY);
        // 在TypeConvertor::ParseJsonToObject 中已经将该对象加入了缓存，
        // 这里不需要重复添加 addClassObject(getID(), object);
    }
    
    
    
}

