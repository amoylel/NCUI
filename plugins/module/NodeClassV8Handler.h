// Created by amoylel on 05/11/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_JSMESSAGEHANDLER_H__
#define AMO_JSMESSAGEHANDLER_H__

#include <memory>
#include <unordered_map>
#include <functional>
#include <vector>
#include <string>

#include <node.h>
#include <node_object_wrap.h>
#include <uv.h>

#include "ipc/Any.hpp"
#include "transfer/FunctionWrapper.hpp"
#include <v8.h>
#include "nan/nan.h"
#include "ipc/NodeMessageEmitter.hpp"
#include "NodeV8Handler.h"
#include "NodeTypeConvertor.h"
#include "handler/NodeProcessHandler.h"
#include "module/ConstructorMgr.h"

namespace amo {
    using v8::Context;
    using v8::Function;
    using v8::FunctionCallbackInfo;
    using v8::FunctionTemplate;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object;
    using v8::Persistent;
    using v8::String;
    using v8::Value;
    
    class loader;
    class NodeMessageEmitter;
    class FunctionWrapper;
    enum NodeClassType {
        NODE_BrowserWindow,
        
        NODE_App,
        
        NODE_Tray,
        
        NODE_Menu,
        
        NODE_NativeWindow,
        
        NODE_GlobalShortcut,
        
        NODE_Shell,
        
        NODE_Browser,
        
        NODE_BrowserHost,
        
        NODE_Frame,
        
        NODE_URLRequest,
        
        NODE_Event,
        
        NODE_Control,
        
        NODE_IPC,
        
        NODE_Dialog,
        
        NODE_Downloader,
        
        NODE_Net,
        
        NODE_System,
        
        NODE_Desktop,
        
        NODE_Keyboard,
        
        NODE_Mouse,
        
        NODE_action
    };
    
    
    // Transfer类Node封装
    template<NodeClassType>
    class NodeClassV8Handler : public NodeV8Handler {
    public:
        NodeClassV8Handler() {
            setName("");
        }
        
        NodeClassV8Handler(const std::string& strClassName) {
            setName(strClassName);
        }
        
        static std::string& getNodeClassName() {
            static std::string m_NodeClassName;
            return m_NodeClassName;
        }
        
        // 对象 属性
        static NAN_GETTER(ExecuteObjectGetter) {
            NodeClassV8Handler* pClassHandler =
                ObjectWrap::Unwrap<NodeClassV8Handler>(info.Holder());
                
            // 如果对象不存在，返回Undefined
            if (pClassHandler == NULL) {
                return;
            }
            
            NodeTypeConvertor convertor;
            std::string propertyName = convertor.toString(property);
            pClassHandler->GetProperty(propertyName, info);
            return;
        }
        
        
        // 类静态常量属性
        static void ExecuteClassAttrGetter(v8::Local<v8::Name> v8Name,
                                           const v8::PropertyCallbackInfo<v8::Value>& info) {
                                           
            auto& map = NodeProcessHandler::getClassMap();											//!< The map
            auto iter = map.find(getNodeClassName());
            
            if (iter == map.end()) {
                return;
            }
            
            // 先看是不是常量
            NodeTypeConvertor convertor;
            std::string attrName = convertor.toString(v8Name);
            
            for (auto& p : iter->second.getAttributes()) {
                amo::u8json json = p;
                std::string strName = json.getString("name");
                
                if (attrName == strName) {
                    Any val = Any::fromJson(json.get_child("value"));
                    info.GetReturnValue().Set(convertor.toV8Value(val));
                    return;
                }
            }
            
            //
            NodeClassV8Handler*  pClassHandler = getClassObject();
            
            if (pClassHandler == NULL) {
                return;
            }
            
            
            Nan::PropertyCallbackInfo<v8::Value> args(info, Nan::Undefined());
            pClassHandler->GetProperty(attrName, args);
            
        }
        
        
        
        static void ExecuteClassAttrSetter(v8::Local<v8::String> strName,
                                           v8::Local<v8::Value> value,
                                           const Nan::PropertyCallbackInfo<void>& info) {
                                           
        }
        
        // 初始化类
        static void Init(Local<Object> target, const std::string& strClassName) {
        
            getNodeClassName() = strClassName;
            // 准备构造函数模版
            v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
            tpl->SetClassName(Nan::New(strClassName).ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);
            
            v8::Local<v8::ObjectTemplate> itpl = tpl->InstanceTemplate();
            
            auto& map = NodeProcessHandler::getClassMap();											//!< The map
            auto iter = map.find(strClassName);
            
            if (iter == map.end()) {
                return;
            }
            
            std::vector<FunctionWrapper> functions = iter->second.toVector();
            getClassObject()->setID(iter->second.getObjectID());
            
            NodeTypeConvertor::addClassObject(getClassObject()->getID(),
                                              getClassObject());
                                              
                                              
            for (auto& p : functions) {
                if (p.functionType() == TransferFuncConstructor) {
                    // 构造函数
                    getClassObject()->setName(p.m_strName);
                } else if (p.functionType() == TransferFuncStatic) {
                    // 静态函数
                    Nan::SetMethod(tpl, p.m_strName.c_str(), ExecuteStaticFunc);
                } else if (p.functionType() == TransferFuncNormal) {
                    // 成员函数
                    Nan::SetPrototypeMethod(tpl, p.m_strName.c_str(),
                                            ExecuteObjectFunc);
                } else if (p.functionType() == TransferFuncClassProperty) {
                    /// 属性
                    tpl->SetNativeDataProperty(Nan::New(p.m_strName).ToLocalChecked(),
                                               NodeClassV8Handler::ExecuteClassAttrGetter);
                }
            }
            
            // 生成类常量属性
            for (auto& p : iter->second.getAttributes()) {
                amo::u8json json = p;
                std::string strName = json.getString("name");
                tpl->SetNativeDataProperty(Nan::New(strName).ToLocalChecked(),
                                           NodeClassV8Handler::ExecuteClassAttrGetter);
            }
            
            // 添加事件监听消息 EventEmitter
            Nan::SetMethod(tpl, "on", OnStaticEvent);
            Nan::SetPrototypeMethod(tpl, "on", OnEvent);
            
            
            // 对象的属性
            /* Nan::SetAccessor(itpl, Nan::New<v8::String>("prop2").ToLocalChecked(),
            				  NodeClassV8Handler::ExecuteObjectGetter);*/
            
            // 生成构造函数
            std::shared_ptr<Nan::Persistent<v8::Function> > constructor;
            constructor.reset(new Nan::Persistent<v8::Function>());
            constructor->Reset(tpl->GetFunction());
            //  注册类 构造函数
            auto manager = ConstructorMgr::getInstance();
            manager->addConstructor(strClassName, constructor);
            
            // 设置类到导出函数， 这个不会使用
            target->Set(Nan::New(strClassName).ToLocalChecked(),
                        tpl->GetFunction());
                        
            // 直接导出到全局变量
            v8::Local<v8::Object> globalObject
                = v8::Isolate::GetCurrent()->GetCurrentContext()->Global();
            Handle<Object> global = Handle<Object>::Cast(
                                        globalObject->GetPrototype());
            global->Set(Nan::New(strClassName).ToLocalChecked(),
                        tpl->GetFunction());
                        
                        
        }
        
        static std::shared_ptr<NodeMessageEmitter> getNativeRunner() {
            return NodeProcessHandler::getNativeRunner();
        }
        
        static NodeClassV8Handler* getClassObject() {
            static NodeClassV8Handler* object = new NodeClassV8Handler();
            return object;
        }
        
        static Nan::Persistent<v8::Function>& getConstructor()  {
            static Nan::Persistent<v8::Function> constructor;
            return constructor;
        }
        
        
        
        static void New(const Nan::FunctionCallbackInfo<Value>& args) {
            if (args.IsConstructCall()) {
                NodeClassV8Handler* object = NULL;
                
                do {
                    if (args.Length() != 1) {
                        break;
                    }
                    
                    std::string str = std::string(*v8::String::Utf8Value(args[0]));
                    
                    // 判断是否为主进程消息中的对象
                    if (str.find("transferObject") == -1) {
                        break;
                    }
                    
                    amo::u8json json(str);
                    int64_t nID = json.get<int64_t>("id");
                    NodeV8Handler* pHandler = NodeTypeConvertor::getClassObject(nID);
                    object = dynamic_cast<NodeClassV8Handler*>(pHandler);
                    
                    if (object == NULL) {
                        object = new NodeClassV8Handler(getClassObject()->getName());
                        object->setID(nID);
                        NodeTypeConvertor::addClassObject(nID, object); // 管理
                    }
                    
                    
                } while (false);
                
                if (object == NULL) {
                    object = new NodeClassV8Handler(getClassObject()->getName());
                    std::shared_ptr<NodeMessageEmitter> launcher = getNativeRunner();
                    launcher->setValue(IPCArgsPosInfo::TransferName, getClassObject()->getName());
                    launcher->setValue(IPCArgsPosInfo::TransferID, getClassObject()->getID());
                    
                    amo::Any out = launcher->syncExecute<const Nan::FunctionCallbackInfo<Value>&>
                                   (getClassObject()->getName(), args);
                    amo::u8json json = amo::stringToAny<amo::u8json>(out.value());
                    int64_t nID = json.get<int64_t>("id");
                    object->setID(nID);
                    NodeTypeConvertor::addClassObject(nID, object); // 管理
                }
                
                
                
                
                object->Wrap(args.This());
                args.GetReturnValue().Set(args.This());
                
                //object->m_oObject.reset(new Nan::Persistent<Value>(args.This()));
                
            }
        }
        
        static void OnStaticEvent(const  Nan::FunctionCallbackInfo<Value>& args) {
            NodeClassV8Handler* object = getClassObject();
            object->on(args);
        }
        static void OnEvent(const  Nan::FunctionCallbackInfo<Value>& args) {
            NodeClassV8Handler* object = ObjectWrap::Unwrap<NodeClassV8Handler>
                                         (args.Holder());
            object->on(args);
        }
        
        static void ExecuteStaticFunc(const  Nan::FunctionCallbackInfo<Value>& args) {
            NodeClassV8Handler* object = getClassObject();
            object->Execute(args);
        }
        
        static void ExecuteObjectFunc(const Nan::FunctionCallbackInfo<Value>& args) {
            NodeClassV8Handler* object = ObjectWrap::Unwrap<NodeClassV8Handler>
                                         (args.Holder());
            object->Execute(args);
        }
        
        static void EventCall(const  Nan::FunctionCallbackInfo<v8::Value>& args) {
            v8::Local<v8::Value> var = args.Callee().As<v8::Function>()->GetName();
            std::string strFunc = ObjectToString(var);	//获取函数名
            
            Isolate* isolate = args.GetIsolate();
            NodeClassV8Handler* object = ObjectWrap::Unwrap<NodeClassV8Handler>
                                         (args.Holder());
                                         
            if (strFunc == "on") {
                object->on(args);
            } else if (strFunc == "emit") {
                object->emit(args);
            }
        }
        
        
        // Object
        
        bool GetProperty(const std::string& name,
                         const Nan::PropertyCallbackInfo<v8::Value>& info) {
            auto& map = NodeProcessHandler::getClassMap();											//!< The map
            auto iter = map.find(getName());
            
            if (iter == map.end()) {
                return false;
            }
            
            std::vector<FunctionWrapper> vec = iter->second.toVector();
            
            std::shared_ptr<NodeMessageEmitter> launcher = getNativeRunner();
            launcher->setValue(IPCArgsPosInfo::TransferName, getName());
            launcher->setValue(IPCArgsPosInfo::TransferID, getID());
            
            for (auto& p : vec) {
                if (name == p.m_strName) {
                    if (p.execType() == TransferExecSync) {
                        amo::Any  any = launcher->syncExecute(name);
                        NodeTypeConvertor convertor(this->getName());
                        info.GetReturnValue().Set(convertor.toV8Value(any));
                        return true;
                    }
                }
            }
            
            return false;
        }
        
        
        bool Execute(const Nan::FunctionCallbackInfo<Value>& args) {
        
            v8::Local<v8::Value> var = args.Callee().As<v8::Function>()->GetName();
            std::string name = ObjectToString(var);	//获取函数名
            
            auto& map = NodeProcessHandler::getClassMap();											//!< The map
            auto iter = map.find(getName());
            
            if (iter == map.end()) {
                return false;
            }
            
            std::vector<FunctionWrapper> vec = iter->second.toVector();
            
            std::shared_ptr<NodeMessageEmitter> launcher = getNativeRunner();
            launcher->setValue(IPCArgsPosInfo::TransferName, getName());
            launcher->setValue(IPCArgsPosInfo::TransferID, getID());
            
            for (auto& p : vec) {
                if (name == p.m_strName) {
                
                    if (p.functionType() == TransferFuncConstructor) {
                        amo::Any  any = launcher->syncExecute<const Nan::FunctionCallbackInfo<Value>&>
                                        (name, args);
                        NodeTypeConvertor convertor(this->getName());
                        args.GetReturnValue().Set(convertor.toV8Value(any));
                        return true;
                    }
                    
                    if (p.execType() == TransferExecSync) {
                        amo::Any  any = launcher->syncExecute<const Nan::FunctionCallbackInfo<Value>&>
                                        (name, args);
                        NodeTypeConvertor convertor(this->getName());
                        args.GetReturnValue().Set(convertor.toV8Value(any));
                        return true;
                    } else if (p.execType() == TransferExecAsync) {
                        launcher->asyncExecute<const Nan::FunctionCallbackInfo<Value>&>(name, args);
                        return true;
                    } else {
                        launcher->execute<const Nan::FunctionCallbackInfo<Value>&>(name, args);
                        return true;
                    }
                }
            }
            
            return false;
        }
        // 接口
        
        void on(const  Nan::FunctionCallbackInfo<v8::Value>& args) {
            if (args.Length() != 2) {
                return;
            }
            
            std::string name =  std::string(*v8::String::Utf8Value(args[0]));
            v8::Local<v8::Function> cb = args[1].As<v8::Function>();
            
            auto iter = m_callbackFunctionMap.insert(
                            std::make_pair(name,
                                           std::make_shared<Nan::Callback>(cb)));
                                           
                                           
        }
        
        
        void emit(const  Nan::FunctionCallbackInfo<v8::Value>& args) {
            //if (args.Length() < 1) return;
            //std::string name = std::string(*v8::String::Utf8Value(args[0]));
            //auto iter = m_callbackFunctionMap.find(name);
            //if (iter == m_callbackFunctionMap.end()) return;
            //const unsigned argc = args.Length() - 1;
            //std::vector<Local<Value> > argv;
            //for (int i = 1; i < args.Length(); ++i)
            //{
            //	argv.push_back(args[i]);
            //}
            //
            ///*Local<Function> cb = Local<Function>::New(args.GetIsolate(), *(iter->second));
            //Nan::MakeCallback(Nan::GetCurrentContext()->Global(), cb, argc, argv.data());*/
            //m_callback->Call(argc, argv.data());
            ////iter->second->Call(isolate->GetCurrentContext()->Global(), argc, argv.data());
        }
        
        
        virtual amo::Any emit(IPCMessage::SmartType anyMessage) override {
            //http://blog.chinaunix.net/uid-52437-id-3959324.html
            //  https://stackoverflow.com/questions/16701275/correct-use-of-handlescope-in-asynchronous-addon
            Nan::HandleScope scope;   //非常重要，不加无法调用JS
            
            Nan::TryCatch try_catch;
            
            std::shared_ptr<AnyArgsList> args = anyMessage->getArgumentList();
            
            if (args->getArgsSize() < 1) {
                return Undefined();
            }
            
            std::string name = args->getString(0);
            
            auto iter = m_callbackFunctionMap.find(name);
            
            if (iter == m_callbackFunctionMap.end()) {
                return Undefined();
            }
            
            const unsigned argc = args->getArgsSize() - 1;
            
            std::vector<Local<Value> > argv;
            
            for (int i = 1; i < args->getArgsSize(); ++i) {
                Any&  any = args->getValue(i);
                
                NodeTypeConvertor convertor;
                argv.push_back(convertor.toV8Value(any));
                
            }
            
            v8::Local<v8::Value> retval = iter->second->Call(argc, argv.data());
            
            NodeTypeConvertor convertor;
            Any ret = convertor.toAny(retval);
            return ret;
            
        }
        
    public:
        std::unordered_map<std::string,
            std::shared_ptr<Nan::Callback> > m_callbackFunctionMap;					//!< 回调函数
            
        //std::shared_ptr<Nan::Persistent<Value> > m_oObject;
        
    };
    
    enum FuncType {
        StaticFunction,  // 静态函数
        StaticProproty, // 静态属性
        ConstructFunction, // 构造函数
        ObjectFunction,  // 成员函数
        ObjectProproty,  // 成员属性
    };
    template<FuncType>
    class Executor {
    public:
        void Execute(const  Nan::FunctionCallbackInfo<Value>& args);
    };
    
    template<> class Executor<StaticFunction> {
    
    };
    
}


#endif // AMO_MESSAGEHANDLER_H__
