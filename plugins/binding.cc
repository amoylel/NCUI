#include <node.h>
#include <v8.h>
#include <uv.h>
#include <future>

#if defined _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
//#include <amo/loader.hpp>
//#include <amo/path.hpp>
//#include <functional>
//#include "ipc/Any.hpp"
#include "module/NodeClassV8Handler.h"
#include "handler/NodeProcessHandler.h"
#include "example/myobject.h"
#include <iostream>

using v8::Local;
using v8::Object;


#include "nan/nan.h"
using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;

//NAN_MODULE_INIT(InitAll) {





void terminateTest() {
    std::cout << "bye bye..." << std::endl;
}
static void OnFatalError(const char* location, const char* message) {
    /* if (location) {
    	 PrintErrorString("FATAL ERROR: %s %s\n", location, message);
    	 } else {
    	 PrintErrorString("FATAL ERROR: %s\n", message);
    	 }
    
    	 fflush(stderr);
    	 ABORT();*/
}

void OnMessage(v8::Local<v8::Message> message, v8::Local<v8::Value> error) {


    //FatalException(Isolate::GetCurrent(), error, message);
}


NAN_MODULE_INIT(InitAll) {

    //MessageBoxA(NULL, "1232", "23333", MB_OK);
    atexit(terminateTest);
    v8::Isolate* isolate = target->GetIsolate();
    //isolate->SetAbortOnUncaughtExceptionCallback(reportc);
    isolate->SetFatalErrorHandler(OnFatalError);
    isolate->AddMessageListener(OnMessage);
    //isolate->RemoveMessageListeners()
    /* v8::NeanderArray listeners(isolate->factory()->message_listeners());
     auto first = listeners.get(0);
     auto last = listeners.get
                 (listeners.length() - 1);*/
    //listeners.set(0, last);
    //listeners.set(listeners.length() - 1, first);
    //isolate->RemoveMessageListeners()
    //isolate->SetFailedAccessCheckCallback(FailedAccessCheckCallback2);
    
    MyObject::Init(target);
    
    using namespace v8;
    v8::TryCatch try_catch(target->GetIsolate());
    using namespace amo;
    NodeProcessHandler::Initialize();
    
    
    NodeClassV8Handler<NODE_BrowserWindow>::Init(target, "BrowserWindow");
    NodeClassV8Handler<NODE_App>::Init(target, "app");
    NodeClassV8Handler<NODE_Tray>::Init(target, "tray");
    NodeClassV8Handler<NODE_Menu>::Init(target, "Menu");
    NodeClassV8Handler<NODE_NativeWindow>::Init(target, "NativeWindow");
    NodeClassV8Handler<NODE_GlobalShortcut>::Init(target, "globalShortcut");
    NodeClassV8Handler<NODE_Shell>::Init(target, "shell");
    NodeClassV8Handler<NODE_Browser>::Init(target, "Browser");
    NodeClassV8Handler<NODE_BrowserHost>::Init(target, "BrowserHost");
    NodeClassV8Handler<NODE_Frame>::Init(target, "Frame");
    NodeClassV8Handler<NODE_URLRequest>::Init(target, "URLRequest");
    NodeClassV8Handler<NODE_Event>::Init(target, "Event");
    NodeClassV8Handler<NODE_Control>::Init(target, "Control");
    NodeClassV8Handler<NODE_IPC>::Init(target, "ipc");
    NodeClassV8Handler<NODE_Dialog>::Init(target, "dialog");
    NodeClassV8Handler<NODE_Downloader>::Init(target, "Downloader");
    NodeClassV8Handler<NODE_Net>::Init(target, "net");
    NodeClassV8Handler<NODE_System>::Init(target, "system");
    NodeClassV8Handler<NODE_Desktop>::Init(target, "desktop");
    NodeClassV8Handler<NODE_Keyboard>::Init(target, "keyboard");
    NodeClassV8Handler<NODE_Mouse>::Init(target, "mouse");
    NodeClassV8Handler<NODE_action>::Init(target, "action");
    
    
    
    
    //if (try_catch.HasCaught()) {
    //    node::FatalException(target->GetIsolate(), try_catch);
    //}
    //
    //
    ////Isolate* isolate = target->GetIsolate();
    //HandleScope handleScope(isolate);
    //Handle<Context> context = Context::New(isolate);
    //Context::Scope context_scope(context);
    //Handle<String> string = String::NewFromUtf8(isolate, "function Point() { this.x = 3; this.y = 4; }; var p = new Point(); ");
    //Handle<Script> script = Script::Compile(string);
    //script->Run();
    //Handle<String> str = String::NewFromUtf8(isolate, "p");
    //Handle<Value> data_p = context->Global()->Get(str);
    //Handle<Object> p = Handle<Object>::Cast(data_p);
    //Handle<Value> x = String::NewFromUtf8(isolate, "x");
    //Handle<Value> value = p->Get(x);
}

NODE_MODULE(ncui, InitAll)
//void InitAll2(v8::Local<v8::Object> exports) {
//	MyObject::Init(exports);
//}
//
//NODE_MODULE(addon, InitAll2)



