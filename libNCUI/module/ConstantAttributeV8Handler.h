
// Created by amoylel on 08/11/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_CONSTANTATTRIBUTEV8HANDLER_H__
#define AMO_CONSTANTATTRIBUTEV8HANDLER_H__

#include "module/JsClassObjectV8Handler.h"



namespace amo {

    /*!
     * @class	ConstantAttributeV8Handler
     *
     * @brief	A constant attribute v 8 handler.
     */
    
    class ConstantAttributeV8Handler : public JsClassObjectV8Handler {
    public:
        ConstantAttributeV8Handler();
        
        
        virtual bool Execute(const CefString& name,
                             CefRefPtr<CefV8Value> object,
                             const CefV8ValueList& arguments,
                             CefRefPtr<CefV8Value>& retval,
                             CefString& exception) override;
                             
                             
    private:
        std::unordered_map<std::string, Any> m_oPropertyMap;
    };
    
    class WMAttriV8Handler : public ConstantAttributeV8Handler {
    public:
        WMAttriV8Handler();
        virtual void onGetV8Object(CefRefPtr<CefV8Value> object) override;
        
        
        
    };
}

#endif // AMO_CONSTANTATTRIBUTEV8HANDLER_H__

