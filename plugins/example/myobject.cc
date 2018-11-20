#include "myobject.h"
#include <unordered_map>
#include <iostream>

Nan::Persistent<v8::Function> MyObject::constructor;

std::unordered_map<int64_t, v8::Local<v8::Object> > myMap;

MyObject::MyObject(double value) : value_(value) {
}

MyObject::~MyObject() {
}

void MyObject::Foo(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    v8::Local<v8::Object> obj = info.Holder();
    
    for (auto& p : myMap) {
        if (p.second == obj) {
            std::cout << 3 << std::endl;
        }
    }
    
    //MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.Holder());
    info.GetReturnValue().Set(Nan::New(39));
    
}

void MyObject::Init(v8::Local<v8::Object> target) {
    Nan::HandleScope scope;
    
    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("MyObject").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    
    //myMap[123] = tpl->GetFunction();
    
    // static function
    Nan::SetMethod(tpl, "Foo", Foo);
    
    // Prototype
    Nan::SetPrototypeMethod(tpl, "value", GetValue);
    Nan::SetPrototypeMethod(tpl, "plusOne", PlusOne);
    Nan::SetPrototypeMethod(tpl, "multiply", Multiply);
    
    constructor.Reset(tpl->GetFunction());
    target->Set(Nan::New("MyObject").ToLocalChecked(), tpl->GetFunction());
    /*
    using namespace Nan;
    
    v8::Local<v8::FunctionTemplate> tpl =
        Nan::New<v8::FunctionTemplate>(New);
    
    tpl->SetClassName(Nan::New<v8::String>("SetterGetter").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    SetPrototypeMethod(tpl, "log", PlusOne);
    v8::Local<v8::ObjectTemplate> itpl = tpl->InstanceTemplate();
    SetAccessor(
        itpl
        , Nan::New("prop1").ToLocalChecked()
        , SetterGetter::GetProp1);
    SetAccessor(
        itpl
        , Nan::New<v8::String>("prop2").ToLocalChecked()
        , SetterGetter::GetProp2
        , SetterGetter::SetProp2
    );
    
    v8::Local<v8::Function> createnew =
        Nan::New<v8::FunctionTemplate>(CreateNew)->GetFunction();
    Set(target, Nan::New<v8::String>("create").ToLocalChecked(), createnew);*/
    
}

void MyObject::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    if (info.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        double value = info[0]->IsUndefined() ? 0 : info[0]->NumberValue();
        MyObject* obj = new MyObject(value);
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { info[0] };
        v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
        info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
}

void MyObject::GetValue(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.Holder());
    info.GetReturnValue().Set(Nan::New(obj->value_));
}

void MyObject::PlusOne(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.Holder());
    obj->value_ += 1;
    info.GetReturnValue().Set(Nan::New(obj->value_));
}

void MyObject::Multiply(const Nan::FunctionCallbackInfo<v8::Value>& info) {
    MyObject* obj = ObjectWrap::Unwrap<MyObject>(info.Holder());
    double multiple = info[0]->IsUndefined() ? 1 : info[0]->NumberValue();
    
    v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
    
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = { Nan::New(obj->value_ * multiple) };
    
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
}
