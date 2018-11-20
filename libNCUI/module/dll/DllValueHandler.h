// Created by amoylel on 03/22/2017.
// Copyright (c) 2017 amoylel All rights reserved. 

#ifndef AMO_DLLVALUEHANDLER_H__
#define AMO_DLLVALUEHANDLER_H__

#include "handler/CefHeader.hpp"
#include "module/JSV8Handler.h"
#include "module/dll/DllValueAccessor.h"

namespace amo {

    template<typename T>
    class DllValueHandler : public DllValueAccessor<T> {
    public:
        typedef T value_type;
    public:
        AMO_CEF_BEGIN_OBJECT_FACTORY(DllValueHandler<T>)
        AMO_CEF_MAP_OBJECT_FUNCTION(typeName)
        AMO_CEF_MAP_OBJECT_FUNCTION(val)
        AMO_CEF_MAP_OBJECT_STRING(id)
        AMO_CEF_END_OBJECT_FACTORY()
    };
}

#endif // AMO_DLLVALUEHANDLER_H__
