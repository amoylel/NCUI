// Created by amoylel on 04/10/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_JSOBJECTV8HANDLER_H__
#define AMO_JSOBJECTV8HANDLER_H__


#include "module/JsClassV8Handler.h"


namespace amo {

    /*!
     * @class	JsClassObjectV8Handler
     *
     * @brief	Transfer 对应的JS Object 类实例对象.
     */
    class JsClassObjectV8Handler : public JsClassV8Handler {
    public:
        JsClassObjectV8Handler();
        ~JsClassObjectV8Handler();
        
        /*!
         * @fn	virtual CefRefPtr<CefV8Value> JsClassObjectV8Handler::getV8Object(
         * 		CefRefPtr<CefV8Value> object = NULL) override;
         *
         * @brief	Gets v 8 object.
         *
         * @param	object	(Optional) the object.
         *
         * @return	The v 8 object.
         */
        
        virtual CefRefPtr<CefV8Value> getV8Object(
            CefRefPtr<CefV8Value> object = NULL) override;
            
        /*!
         * @fn	virtual void JsClassObjectV8Handler::onGetV8Object(
         * 		CefRefPtr<CefV8Value> object);
         *
         * @brief	添加成员函数.
         *
         * @param	object	The object.
         */
        virtual void onGetV8Object(CefRefPtr<CefV8Value> object);
        
        
        
        
        
    };
}

#endif // AMO_JSOBJECTV8HANDLER_H__
