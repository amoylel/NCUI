// Created by amoylel on 05/21/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_EVENTFUNCTIONMANAGER_H__
#define AMO_EVENTFUNCTIONMANAGER_H__

#include "handler/CefHeader.hpp"

namespace amo {

    /*!
     * @class	CallbackFunctionWrapper
     *
     * @brief	A callback function wrapper.
     */
    
    class CallbackFunctionWrapper {
    public:
        typedef std::shared_ptr<CallbackFunctionWrapper> SmartType;
    public:
        CallbackFunctionWrapper(CefRefPtr<CefV8Context> pContext,
                                CefRefPtr<CefV8Value> pFunction,
                                bool bOnce = false) {
            m_pContext = pContext;
            m_pFunction = pFunction;
            m_bOnce = bOnce;
        }
    public:
        CefRefPtr<CefV8Context> m_pContext;
        CefRefPtr<CefV8Value> m_pFunction;
        bool m_bOnce;
    };
    
    class EventFunctionManager {
    
    };
    
}

#endif // AMO_EVENTFUNCTIONMANAGER_H__

