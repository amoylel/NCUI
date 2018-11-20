// Created by amoylel on 10/09/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_UTILITYV8HANDLER_H__
#define AMO_UTILITYV8HANDLER_H__
#include <memory>

#include "handler/CefHeader.hpp"

namespace amo {
    class UtilityV8Handler : public CefV8Handler {
    
    public:
        virtual bool Execute(const CefString& name,
                             CefRefPtr<CefV8Value> object,
                             const CefV8ValueList& arguments,
                             CefRefPtr<CefV8Value>& retval,
                             CefString& exception) override;
        IMPLEMENT_REFCOUNTING(UtilityV8Handler);
        
    };
}


#endif // AMO_UTILITYV8HANDLER_H__

