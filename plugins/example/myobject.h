#ifndef MYOBJECT_H
#define MYOBJECT_H

#include "nan/nan.h"

class MyObject : public Nan::ObjectWrap {
public:
    // 初始化函数
    static NAN_MODULE_INIT(Init);
    
    // 构造函数
    static NAN_METHOD(New);
    
    static v8::Local<v8::Value> NewInstance() {
    
    }
    
    
    
    
    
private:
    explicit MyObject(double value = 0);
    ~MyObject();
    
    static void Foo(const Nan::FunctionCallbackInfo<v8::Value>& info);
    
    static void GetValue(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void PlusOne(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static void Multiply(const Nan::FunctionCallbackInfo<v8::Value>& info);
    static Nan::Persistent<v8::Function> constructor;
    double value_;
};

#endif
