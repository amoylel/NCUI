#include "stdafx.h"
#include "module/dll/JsTypeMapHandler.h"

#include <windows.h>

#include <amo/string.hpp>
#include <amo/logger.hpp>

#include "module/dll/TypeMapManager.h"
#include "module/dll/DllValueHandler.h"



namespace amo {


    template < typename T >
    void createHandler(CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& except) {
        CefRefPtr<DllValueHandler<T> > handler = new DllValueHandler<T>();
        handler->val(object, arguments, retval, except);
        retval = handler->getV8Object();
    }
    
    JsTypeMapHandler::JsTypeMapHandler() {
        setHandlerName("window");
    }
    
    void JsTypeMapHandler::BOOL(CefRefPtr<CefV8Value> object,
                                const CefV8ValueList& arguments,
                                CefRefPtr<CefV8Value>& retval,
                                CefString& except) {
        createHandler<bool>(object, arguments, retval, except);
    }
    
    void JsTypeMapHandler::INT(CefRefPtr<CefV8Value> object,
                               const CefV8ValueList& arguments,
                               CefRefPtr<CefV8Value>& retval,
                               CefString& except) {
        createHandler<int32_t>(object, arguments, retval, except);
    }
    
    void JsTypeMapHandler::INT8(CefRefPtr<CefV8Value> object,
                                const CefV8ValueList& arguments,
                                CefRefPtr<CefV8Value>& retval,
                                CefString& except) {
        createHandler<int8_t>(object, arguments, retval, except);
        
    }
    
    void JsTypeMapHandler::INT16(CefRefPtr<CefV8Value> object,
                                 const CefV8ValueList& arguments,
                                 CefRefPtr<CefV8Value>& retval,
                                 CefString& except) {
        createHandler<int16_t>(object, arguments, retval, except);
        
    }
    
    void JsTypeMapHandler::INT32(CefRefPtr<CefV8Value> object,
                                 const CefV8ValueList& arguments,
                                 CefRefPtr<CefV8Value>& retval,
                                 CefString& except) {
        createHandler<int32_t>(object, arguments, retval, except);
        
    }
    
    void JsTypeMapHandler::INT64(CefRefPtr<CefV8Value> object,
                                 const CefV8ValueList& arguments,
                                 CefRefPtr<CefV8Value>& retval,
                                 CefString& except) {
        createHandler<int64_t>(object, arguments, retval, except);
        
    }
    
    void JsTypeMapHandler::UINT(CefRefPtr<CefV8Value> object,
                                const CefV8ValueList& arguments,
                                CefRefPtr<CefV8Value>& retval,
                                CefString& except) {
        createHandler<uint32_t>(object, arguments, retval, except);
        
    }
    
    void JsTypeMapHandler::UINT8(CefRefPtr<CefV8Value> object,
                                 const CefV8ValueList& arguments,
                                 CefRefPtr<CefV8Value>& retval,
                                 CefString& except) {
        createHandler<uint8_t>(object, arguments, retval, except);
        
    }
    
    void JsTypeMapHandler::UINT16(CefRefPtr<CefV8Value> object,
                                  const CefV8ValueList& arguments,
                                  CefRefPtr<CefV8Value>& retval,
                                  CefString& except) {
        createHandler<uint16_t>(object, arguments, retval, except);
        
    }
    
    void JsTypeMapHandler::UINT32(CefRefPtr<CefV8Value> object,
                                  const CefV8ValueList& arguments,
                                  CefRefPtr<CefV8Value>& retval,
                                  CefString& except) {
        createHandler<uint32_t>(object, arguments, retval, except);
        
    }
    
    void JsTypeMapHandler::UINT64(CefRefPtr<CefV8Value> object,
                                  const CefV8ValueList& arguments,
                                  CefRefPtr<CefV8Value>& retval,
                                  CefString& except) {
        createHandler<uint64_t>(object, arguments, retval, except);
        
    }
    
    void JsTypeMapHandler::CHAR(CefRefPtr<CefV8Value> object,
                                const CefV8ValueList& arguments,
                                CefRefPtr<CefV8Value>& retval,
                                CefString& except) {
        createHandler<char>(object, arguments, retval, except);
    }
    
    void JsTypeMapHandler::LONG(CefRefPtr<CefV8Value> object,
                                const CefV8ValueList& arguments,
                                CefRefPtr<CefV8Value>& retval,
                                CefString& except) {
        createHandler<long>(object, arguments, retval, except);
    }
    
    void JsTypeMapHandler::ULONG(CefRefPtr<CefV8Value> object,
                                 const CefV8ValueList& arguments,
                                 CefRefPtr<CefV8Value>& retval,
                                 CefString& except) {
        createHandler<unsigned long>(object, arguments, retval, except);
    }
    
    void JsTypeMapHandler::FLOAT(CefRefPtr<CefV8Value> object,
                                 const CefV8ValueList& arguments,
                                 CefRefPtr<CefV8Value>& retval,
                                 CefString& except) {
        createHandler<float>(object, arguments, retval, except);
    }
    
    void JsTypeMapHandler::DOUBLE(CefRefPtr<CefV8Value> object,
                                  const CefV8ValueList& arguments,
                                  CefRefPtr<CefV8Value>& retval,
                                  CefString& except) {
        createHandler<double>(object, arguments, retval, except);
        
    }
    
    void JsTypeMapHandler::LDOUBLE(CefRefPtr<CefV8Value> object,
                                   const CefV8ValueList& arguments,
                                   CefRefPtr<CefV8Value>& retval,
                                   CefString& except) {
        createHandler<long double>(object, arguments, retval, except);
        
    }
    
    void JsTypeMapHandler::STR(CefRefPtr<CefV8Value> object,
                               const CefV8ValueList& arguments,
                               CefRefPtr<CefV8Value>& retval,
                               CefString& except) {
        createHandler<std::string>(object, arguments, retval, except);
        
    }
    
    void JsTypeMapHandler::NIL(CefRefPtr<CefV8Value> object,
                               const CefV8ValueList& arguments,
                               CefRefPtr<CefV8Value>& retval,
                               CefString& except) {
        createHandler<amo::nil>(object, arguments, retval, except);
    }
    
    
    
    void JsTypeMapHandler::BYTE(CefRefPtr<CefV8Value> object,
                                const CefV8ValueList& arguments,
                                CefRefPtr<CefV8Value>& retval,
                                CefString& except) {
        createHandler<::BYTE>(object, arguments, retval, except);
    }
    
    void JsTypeMapHandler::BSTR(CefRefPtr<CefV8Value> object,
                                const CefV8ValueList& arguments,
                                CefRefPtr<CefV8Value>& retval,
                                CefString& except) {
                                
    }
    
    
    void JsTypeMapHandler::COLORREF(CefRefPtr<CefV8Value> object,
                                    const CefV8ValueList& arguments,
                                    CefRefPtr<CefV8Value>& retval,
                                    CefString& except) {
                                    
    }
    
    void JsTypeMapHandler::DWORD(CefRefPtr<CefV8Value> object,
                                 const CefV8ValueList& arguments,
                                 CefRefPtr<CefV8Value>& retval,
                                 CefString& except) {
        createHandler<::DWORD>(object, arguments, retval, except);
    }
    
    
    void JsTypeMapHandler::HANDLE(CefRefPtr<CefV8Value> object,
                                  const CefV8ValueList& arguments,
                                  CefRefPtr<CefV8Value>& retval,
                                  CefString& except) {
                                  
    }
    
    void JsTypeMapHandler::HBITMAP(CefRefPtr<CefV8Value> object,
                                   const CefV8ValueList& arguments,
                                   CefRefPtr<CefV8Value>& retval,
                                   CefString& except) {
                                   
    }
    
    void JsTypeMapHandler::HBRUSH(CefRefPtr<CefV8Value> object,
                                  const CefV8ValueList& arguments,
                                  CefRefPtr<CefV8Value>& retval,
                                  CefString& except) {
                                  
    }
    
    void JsTypeMapHandler::HCURSOR(CefRefPtr<CefV8Value> object,
                                   const CefV8ValueList& arguments,
                                   CefRefPtr<CefV8Value>& retval,
                                   CefString& except) {
                                   
    }
    
    void JsTypeMapHandler::HDC(CefRefPtr<CefV8Value> object,
                               const CefV8ValueList& arguments,
                               CefRefPtr<CefV8Value>& retval,
                               CefString& except) {
                               
    }
    
    void JsTypeMapHandler::HFILE(CefRefPtr<CefV8Value> object,
                                 const CefV8ValueList& arguments,
                                 CefRefPtr<CefV8Value>& retval,
                                 CefString& except) {
                                 
    }
    
    void JsTypeMapHandler::HFONT(CefRefPtr<CefV8Value> object,
                                 const CefV8ValueList& arguments,
                                 CefRefPtr<CefV8Value>& retval,
                                 CefString& except) {
                                 
    }
    
    void JsTypeMapHandler::HHOOK(CefRefPtr<CefV8Value> object,
                                 const CefV8ValueList& arguments,
                                 CefRefPtr<CefV8Value>& retval,
                                 CefString& except) {
                                 
    }
    
    void JsTypeMapHandler::HKEY(CefRefPtr<CefV8Value> object,
                                const CefV8ValueList& arguments,
                                CefRefPtr<CefV8Value>& retval,
                                CefString& except) {
                                
    }
    
    void JsTypeMapHandler::HPEN(CefRefPtr<CefV8Value> object,
                                const CefV8ValueList& arguments,
                                CefRefPtr<CefV8Value>& retval,
                                CefString& except) {
                                
    }
    
    void JsTypeMapHandler::HWND(CefRefPtr<CefV8Value> object,
                                const CefV8ValueList& arguments,
                                CefRefPtr<CefV8Value>& retval,
                                CefString& except) {
                                
    }
    
    
    
    
    
    
    
    void JsTypeMapHandler::LONGLONG(CefRefPtr<CefV8Value> object,
                                    const CefV8ValueList& arguments,
                                    CefRefPtr<CefV8Value>& retval,
                                    CefString& except) {
        createHandler<::LONGLONG>(object, arguments, retval, except);
    }
    
    void JsTypeMapHandler::LPARAM(CefRefPtr<CefV8Value> object,
                                  const CefV8ValueList& arguments,
                                  CefRefPtr<CefV8Value>& retval,
                                  CefString& except) {
                                  
    }
    
    void JsTypeMapHandler::LPBOOL(CefRefPtr<CefV8Value> object,
                                  const CefV8ValueList& arguments,
                                  CefRefPtr<CefV8Value>& retval,
                                  CefString& except) {
                                  
    }
    
    void JsTypeMapHandler::LPBYTE(CefRefPtr<CefV8Value> object,
                                  const CefV8ValueList& arguments,
                                  CefRefPtr<CefV8Value>& retval,
                                  CefString& except) {
                                  
    }
    
    void JsTypeMapHandler::LPCOLOREF(CefRefPtr<CefV8Value> object,
                                     const CefV8ValueList& arguments,
                                     CefRefPtr<CefV8Value>& retval,
                                     CefString& except) {
                                     
    }
    
    void JsTypeMapHandler::LPCSTR(CefRefPtr<CefV8Value> object,
                                  const CefV8ValueList& arguments,
                                  CefRefPtr<CefV8Value>& retval,
                                  CefString& except) {
                                  
    }
    
    void JsTypeMapHandler::LPCTSTR(CefRefPtr<CefV8Value> object,
                                   const CefV8ValueList& arguments,
                                   CefRefPtr<CefV8Value>& retval,
                                   CefString& except) {
                                   
    }
    
    void JsTypeMapHandler::LPVOID(CefRefPtr<CefV8Value> object,
                                  const CefV8ValueList& arguments,
                                  CefRefPtr<CefV8Value>& retval,
                                  CefString& except) {
                                  
    }
    
    void JsTypeMapHandler::LPDWORD(CefRefPtr<CefV8Value> object,
                                   const CefV8ValueList& arguments,
                                   CefRefPtr<CefV8Value>& retval,
                                   CefString& except) {
                                   
    }
    
    void JsTypeMapHandler::SHORT(CefRefPtr<CefV8Value> object,
                                 const CefV8ValueList& arguments,
                                 CefRefPtr<CefV8Value>& retval,
                                 CefString& except) {
        createHandler<::SHORT>(object, arguments, retval, except);
    }
    
    void JsTypeMapHandler::VARIANT(CefRefPtr<CefV8Value> object,
                                   const CefV8ValueList& arguments,
                                   CefRefPtr<CefV8Value>& retval,
                                   CefString& except) {
                                   
    }
    
    void JsTypeMapHandler::WORD(CefRefPtr<CefV8Value> object,
                                const CefV8ValueList& arguments,
                                CefRefPtr<CefV8Value>& retval,
                                CefString& except) {
        createHandler<::WORD>(object, arguments, retval, except);
    }
    
    void JsTypeMapHandler::WPARAM(CefRefPtr<CefV8Value> object,
                                  const CefV8ValueList& arguments,
                                  CefRefPtr<CefV8Value>& retval,
                                  CefString& except) {
                                  
    }
    
    void JsTypeMapHandler::INT_PTR(CefRefPtr<CefV8Value> object,
                                   const CefV8ValueList& arguments,
                                   CefRefPtr<CefV8Value>& retval,
                                   CefString& except) {
        createHandler<int*>(object, arguments, retval, except);
    }
    
    
}

