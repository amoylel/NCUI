#include "ipc/NodeMessageEmitter.hpp"
#include "ipc/NodeProcessExchanger.hpp"
#include "handler/NodeProcessHandler.h"
#include "nan\nan.h"



namespace amo {

    NodeMessageEmitter::NodeMessageEmitter(int nPipeID, int nFrameID)
        : MessageLauncher(nPipeID, nFrameID) {
        
    }
    
    NodeMessageEmitter::~NodeMessageEmitter() {
    
    }
    
    std::string NodeMessageEmitter::ObjectToString(v8::Local<v8::Value> value) {
        v8::String::Utf8Value utf8_value(value);
        return std::string(*utf8_value);
    }
    
    void NodeMessageEmitter::onMakeProcessMessage(IPCMessage::SmartType msg, void* param) {
        const Nan::FunctionCallbackInfo<v8::Value>& args = *reinterpret_cast<const Nan::FunctionCallbackInfo<v8::Value>*>(param);
        
        std::shared_ptr<amo::AnyArgsList> list = msg->getArgumentList();
        
        for (int i = 0; i < args.Length(); ++i) {
            amo::Any val = amo::Nil();
            
            if (args[i]->IsBoolean()) {
                val = args[i]->BooleanValue();
                list->setValue(i, val);
            }
            
            if (args[i]->IsInt32()) {
                val = args[i]->Int32Value();
                list->setValue(i, val);
            } else if (args[i]->IsString()) {
                val = ObjectToString(args[i]);
                list->setValue(i, val);
            } else if (args[i]->IsObject()) {
                v8::Isolate* isolate = args.GetIsolate();
                v8::Handle<v8::String> src = v8::String::NewFromUtf8(isolate, "(function(){return function(obj){return JSON.stringify(obj);};})();");
                
                v8::Handle<v8::Script> script = v8::Script::Compile(src);
                v8::Handle<v8::Value> value = script->Run();
                
                
                v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(value);
                v8::Local<v8::Value> out;
                v8::Local<v8::Value> argv[1] = {
                    args[i]
                };
                
                v8::Local<v8::Value> val2 = callback->Call(isolate->GetCurrentContext()->Global(), 1, argv);
                val = ObjectToString(val2);
                list->setValue(i, val);
            } else {
                list->setValue(i, val);
            }
        }
    }
    
    bool NodeMessageEmitter::exchange(int nPipeID, IPCMessage::SmartType msg) {
        return NodeProcessExchangerManager::get_instance()->exchange(nPipeID, msg);
    }
    
    
    amo::Any NodeMessageEmitter::waitResult(int nPipeID, int nMessageID) {
        amo::Any ret = NodeProcessExchangerManager::get_instance()->waitResult<amo::Any>(nPipeID, nMessageID);
        return ret;
    }
    
    bool NodeMessageEmitter::sendMessage(IPCMessage::SmartType anyMessage) {
        return NodeProcessHandler::SendMessageToUI(anyMessage);
    }
    
}