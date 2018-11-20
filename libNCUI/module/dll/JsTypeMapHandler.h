// Created by amoylel on 03/17/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_JSTYPEMAPHANDLER_H__
#define AMO_JSTYPEMAPHANDLER_H__

#include "handler/CefHeader.hpp"
#include "module/JSV8Handler.h"
// tr
#define AMO_CEF_MAP_OBJECT_FUNCTION2CPP(Func)\
	{\
		this->addFunction(#Func, std::bind(&ClassType::Func,\
											this,\
											std::placeholders::_1,\
											std::placeholders::_2,\
											std::placeholders::_3,\
											std::placeholders::_4));\
		CefRefPtr<CefV8Value> _func = CefV8Value::CreateFunction(#Func, this);\
		_func->SetValue("name",\
						 CefV8Value::CreateString(#Func),\
						 V8_PROPERTY_ATTRIBUTE_NONE); \
		_object->SetValue(#Func, _func, V8_PROPERTY_ATTRIBUTE_NONE);\
	}

namespace amo {
    class JsTypeMapHandler : public JsV8Handler {
    public:
        JsTypeMapHandler();
        
        void BOOL(CefRefPtr<CefV8Value> object,
                  const CefV8ValueList& arguments,
                  CefRefPtr<CefV8Value>& retval,
                  CefString& except);
                  
        void INT(CefRefPtr<CefV8Value> object,
                 const CefV8ValueList& arguments,
                 CefRefPtr<CefV8Value>& retval,
                 CefString& except);
                 
        void INT8(CefRefPtr<CefV8Value> object,
                  const CefV8ValueList& arguments,
                  CefRefPtr<CefV8Value>& retval,
                  CefString& except);
                  
        void INT16(CefRefPtr<CefV8Value> object,
                   const CefV8ValueList& arguments,
                   CefRefPtr<CefV8Value>& retval,
                   CefString& except);
                   
        void INT32(CefRefPtr<CefV8Value> object,
                   const CefV8ValueList& arguments,
                   CefRefPtr<CefV8Value>& retval,
                   CefString& except);
                   
        void INT64(CefRefPtr<CefV8Value> object,
                   const CefV8ValueList& arguments,
                   CefRefPtr<CefV8Value>& retval,
                   CefString& except);
                   
        void UINT(CefRefPtr<CefV8Value> object,
                  const CefV8ValueList& arguments,
                  CefRefPtr<CefV8Value>& retval,
                  CefString& except);
                  
        void UINT8(CefRefPtr<CefV8Value> object,
                   const CefV8ValueList& arguments,
                   CefRefPtr<CefV8Value>& retval,
                   CefString& except);
                   
        void UINT16(CefRefPtr<CefV8Value> object,
                    const CefV8ValueList& arguments,
                    CefRefPtr<CefV8Value>& retval,
                    CefString& except);
                    
        void UINT32(CefRefPtr<CefV8Value> object,
                    const CefV8ValueList& arguments,
                    CefRefPtr<CefV8Value>& retval,
                    CefString& except);
                    
        void UINT64(CefRefPtr<CefV8Value> object,
                    const CefV8ValueList& arguments,
                    CefRefPtr<CefV8Value>& retval,
                    CefString& except);
                    
        void CHAR(CefRefPtr<CefV8Value> object,
                  const CefV8ValueList& arguments,
                  CefRefPtr<CefV8Value>& retval,
                  CefString& except);
                  
        void LONG(CefRefPtr<CefV8Value> object,
                  const CefV8ValueList& arguments,
                  CefRefPtr<CefV8Value>& retval,
                  CefString& except);
                  
        void ULONG(CefRefPtr<CefV8Value> object,
                   const CefV8ValueList& arguments,
                   CefRefPtr<CefV8Value>& retval,
                   CefString& except);
                   
        void FLOAT(CefRefPtr<CefV8Value> object,
                   const CefV8ValueList& arguments,
                   CefRefPtr<CefV8Value>& retval,
                   CefString& except);
        void DOUBLE(CefRefPtr<CefV8Value> object,
                    const CefV8ValueList& arguments,
                    CefRefPtr<CefV8Value>& retval,
                    CefString& except);
                    
        void LDOUBLE(CefRefPtr<CefV8Value> object,
                     const CefV8ValueList& arguments,
                     CefRefPtr<CefV8Value>& retval,
                     CefString& except);
        void STR(CefRefPtr<CefV8Value> object,
                 const CefV8ValueList& arguments,
                 CefRefPtr<CefV8Value>& retval,
                 CefString& except);
                 
        void NIL(CefRefPtr<CefV8Value> object,
                 const CefV8ValueList& arguments,
                 CefRefPtr<CefV8Value>& retval,
                 CefString& except);
                 
        void BYTE(CefRefPtr<CefV8Value> object,
                  const CefV8ValueList& arguments,
                  CefRefPtr<CefV8Value>& retval,
                  CefString& except);
        void BSTR(CefRefPtr<CefV8Value> object,
                  const CefV8ValueList& arguments,
                  CefRefPtr<CefV8Value>& retval,
                  CefString& except);
                  
        void COLORREF(CefRefPtr<CefV8Value> object,
                      const CefV8ValueList& arguments,
                      CefRefPtr<CefV8Value>& retval,
                      CefString& except);
        void DWORD(CefRefPtr<CefV8Value> object,
                   const CefV8ValueList& arguments,
                   CefRefPtr<CefV8Value>& retval,
                   CefString& except);
                   
        void HANDLE(CefRefPtr<CefV8Value> object,
                    const CefV8ValueList& arguments,
                    CefRefPtr<CefV8Value>& retval,
                    CefString& except);
        void HBITMAP(CefRefPtr<CefV8Value> object,
                     const CefV8ValueList& arguments,
                     CefRefPtr<CefV8Value>& retval,
                     CefString& except);
        void HBRUSH(CefRefPtr<CefV8Value> object,
                    const CefV8ValueList& arguments,
                    CefRefPtr<CefV8Value>& retval,
                    CefString& except);
        void HCURSOR(CefRefPtr<CefV8Value> object,
                     const CefV8ValueList& arguments,
                     CefRefPtr<CefV8Value>& retval,
                     CefString& except);
        void HDC(CefRefPtr<CefV8Value> object,
                 const CefV8ValueList& arguments,
                 CefRefPtr<CefV8Value>& retval,
                 CefString& except);
        void HFILE(CefRefPtr<CefV8Value> object,
                   const CefV8ValueList& arguments,
                   CefRefPtr<CefV8Value>& retval,
                   CefString& except);
        void HFONT(CefRefPtr<CefV8Value> object,
                   const CefV8ValueList& arguments,
                   CefRefPtr<CefV8Value>& retval,
                   CefString& except);
        void HHOOK(CefRefPtr<CefV8Value> object,
                   const CefV8ValueList& arguments,
                   CefRefPtr<CefV8Value>& retval,
                   CefString& except);
        void HKEY(CefRefPtr<CefV8Value> object,
                  const CefV8ValueList& arguments,
                  CefRefPtr<CefV8Value>& retval,
                  CefString& except);
        void HPEN(CefRefPtr<CefV8Value> object,
                  const CefV8ValueList& arguments,
                  CefRefPtr<CefV8Value>& retval,
                  CefString& except);
        void HWND(CefRefPtr<CefV8Value> object,
                  const CefV8ValueList& arguments,
                  CefRefPtr<CefV8Value>& retval,
                  CefString& except);
                  
                  
        void LONGLONG(CefRefPtr<CefV8Value> object,
                      const CefV8ValueList& arguments,
                      CefRefPtr<CefV8Value>& retval,
                      CefString& except);
        void LPARAM(CefRefPtr<CefV8Value> object,
                    const CefV8ValueList& arguments,
                    CefRefPtr<CefV8Value>& retval,
                    CefString& except);
        void LPBOOL(CefRefPtr<CefV8Value> object,
                    const CefV8ValueList& arguments,
                    CefRefPtr<CefV8Value>& retval,
                    CefString& except);
        void LPBYTE(CefRefPtr<CefV8Value> object,
                    const CefV8ValueList& arguments,
                    CefRefPtr<CefV8Value>& retval,
                    CefString& except);
        void LPCOLOREF(CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& except);
        void LPCSTR(CefRefPtr<CefV8Value> object,
                    const CefV8ValueList& arguments,
                    CefRefPtr<CefV8Value>& retval,
                    CefString& except);
        void LPCTSTR(CefRefPtr<CefV8Value> object,
                     const CefV8ValueList& arguments,
                     CefRefPtr<CefV8Value>& retval,
                     CefString& except);
        void LPVOID(CefRefPtr<CefV8Value> object,
                    const CefV8ValueList& arguments,
                    CefRefPtr<CefV8Value>& retval,
                    CefString& except);
        void LPDWORD(CefRefPtr<CefV8Value> object,
                     const CefV8ValueList& arguments,
                     CefRefPtr<CefV8Value>& retval,
                     CefString& except);
        void SHORT(CefRefPtr<CefV8Value> object,
                   const CefV8ValueList& arguments,
                   CefRefPtr<CefV8Value>& retval,
                   CefString& except);
        void VARIANT(CefRefPtr<CefV8Value> object,
                     const CefV8ValueList& arguments,
                     CefRefPtr<CefV8Value>& retval,
                     CefString& except);
        void WORD(CefRefPtr<CefV8Value> object,
                  const CefV8ValueList& arguments,
                  CefRefPtr<CefV8Value>& retval,
                  CefString& except);
        void WPARAM(CefRefPtr<CefV8Value> object,
                    const CefV8ValueList& arguments,
                    CefRefPtr<CefV8Value>& retval,
                    CefString& except);
        void INT_PTR(CefRefPtr<CefV8Value> object,
                     const CefV8ValueList& arguments,
                     CefRefPtr<CefV8Value>& retval,
                     CefString& except);
                     
        AMO_CEF_BEGIN_OBJECT_FACTORY(JsTypeMapHandler)
        
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(BOOL)
        
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(INT)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(INT8)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(INT16)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(INT32)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(INT64)
        
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(UINT)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(UINT8)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(UINT16)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(UINT32)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(UINT64)
        
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(CHAR)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(LONG)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(ULONG)
        
        
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(FLOAT)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(DOUBLE)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(LDOUBLE)
        
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(STR)
        
        
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(NIL)
        
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(BYTE)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(BSTR)
        
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(COLORREF)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(DWORD)
        
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(HANDLE)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(HBITMAP)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(HBRUSH)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(HCURSOR)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(HDC)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(HFILE)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(HFONT)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(HHOOK)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(HKEY)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(HPEN)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(HWND)
        
        
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(LONGLONG)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(LPARAM)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(LPBOOL)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(LPBYTE)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(LPCOLOREF)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(LPCSTR)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(LPCTSTR)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(LPVOID)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(LPDWORD)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(VARIANT)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(WORD)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(WPARAM)
        AMO_CEF_MAP_OBJECT_FUNCTION2CPP(INT_PTR)
        AMO_CEF_END_OBJECT_FACTORY()
        
        
        
    protected:
        CefRefPtr<CefV8Context> context;
    };
    
}


#endif // AMO_JSTYPEMAPHANDLER_H__
