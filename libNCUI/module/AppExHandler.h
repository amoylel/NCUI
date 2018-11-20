// Created by amoylel on 10/09/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_APPEXHANDLER_70854450_CF4C_4BE7_B9C2_D51F73E3ED11_H__
#define AMO_APPEXHANDLER_70854450_CF4C_4BE7_B9C2_D51F73E3ED11_H__


#include "module/JsClassObjectV8Handler.h"

namespace amo {

    /*!
    * @class	AppExV8Handler
    *
    * @brief	窗口拖拽.
    */
    
    class AppExV8Handler : public JsClassObjectV8Handler {
    public:
        AppExV8Handler();
        ~AppExV8Handler();
        
        
        virtual CefRefPtr<JsV8Handler> createV8Handler();
        
        
        /*!
        * @fn	void AppExV8Handler::addNonGlobalModules(
        * 		CefRefPtr<CefV8Value> object,
        * 		const CefV8ValueList& args,
        * 		CefRefPtr<CefV8Value>& retval,
        * 		CefString& except);
        *
        * @brief	Mouseovers.
        *
        * @param	object		  	The object.
        * @param	args		  	The arguments.
        * @param [in,out]	retval	The retval.
        * @param [in,out]	except	The except.
        */
        
        void addNonGlobalModules(CefRefPtr<CefV8Value> object,
                                 const CefV8ValueList& args,
                                 CefRefPtr<CefV8Value>& retval,
                                 CefString& except);
        void addNonGlobalModule(CefRefPtr<CefV8Value> object,
                                const CefV8ValueList& args,
                                CefRefPtr<CefV8Value>& retval,
                                CefString& except);
        AMO_CEF_BEGIN_OBJECT_FACTORY(AppExV8Handler)
        AMO_CEF_MAP_OBJECT_FUNCTION(addNonGlobalModule)
        AMO_CEF_MAP_OBJECT_FUNCTION(addNonGlobalModules)
        AMO_CEF_END_OBJECT_FACTORY()
        
    protected:
        bool m_bInit;
        std::string dragClassName;
        std::string noDragClassName;
        std::string strRegex;
    };
    
}

#endif // AMO_APPEXHANDLER_70854450_CF4C_4BE7_B9C2_D51F73E3ED11_H__

