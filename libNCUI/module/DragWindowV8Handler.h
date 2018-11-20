// Created by amoylel on 04/26/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_DRAGWINDOWV8HANDLER_H__
#define AMO_DRAGWINDOWV8HANDLER_H__

#include "module/JsClassObjectV8Handler.h"

namespace amo {

    /*!
     * @class	DragWindowV8Handler
     *
     * @brief	窗口拖拽.
     */
    
    class DragWindowV8Handler : public JsClassObjectV8Handler {
    public:
        DragWindowV8Handler();
        ~DragWindowV8Handler();
        
        /*!
         * @fn	void DragWindowV8Handler::onmouseover(
         * 		CefRefPtr<CefV8Value> target,
         * 		const CefV8ValueList& args,
         * 		CefRefPtr<CefV8Value>& retval,
         * 		CefString& except);
         *
         * @brief	Onmouseovers.
         *
         * @param	target		  	Target for the.
         * @param	args		  	The arguments.
         * @param [in,out]	retval	The retval.
         * @param [in,out]	except	The except.
         */
        
        void onmouseover(CefRefPtr<CefV8Value> target,
                         const CefV8ValueList& args,
                         CefRefPtr<CefV8Value>& retval,
                         CefString& except);
                         
        /*!
         * @fn	void DragWindowV8Handler::dragable(
         * 		CefRefPtr<CefV8Value> object,
         * 		const CefV8ValueList& args,
         * 		CefRefPtr<CefV8Value>& retval,
         * 		CefString& except);
         *
         * @brief	Dragables.
         *
         * @param	object		  	The object.
         * @param	args		  	The arguments.
         * @param [in,out]	retval	The retval.
         * @param [in,out]	except	The except.
         */
        
        void dragable(CefRefPtr<CefV8Value> object,
                      const CefV8ValueList& args,
                      CefRefPtr<CefV8Value>& retval,
                      CefString& except);
                      
        /*!
         * @fn	void DragWindowV8Handler::sendDragableToBrowserProcess(bool bEnabled);
         *
         * @brief	向UI发送当前点是否可以移动窗口.
         *
         * @param	bEnabled	true to enable, false to disable.
         */
        
        void sendDragableToBrowserProcess(bool bEnabled);
        
        /*!
         * @fn	void DragWindowV8Handler::mouseover(
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
        
        void mouseover(CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& args,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& except);

		std::string getWebkitAppRegion(CefRefPtr<CefV8Value> object);
                       
        virtual CefRefPtr<JsV8Handler> createV8Handler();
        
        AMO_CEF_BEGIN_OBJECT_FACTORY(DragWindowV8Handler)
        AMO_CEF_MAP_OBJECT_FUNCTION(dragable);
        AMO_CEF_MAP_OBJECT_FUNCTION(mouseover);
        AMO_CEF_END_OBJECT_FACTORY()
        
    protected:
        bool m_bInit;
        std::string dragClassName;
        std::string noDragClassName;
        std::string strRegex;
    };
    
}


#endif // AMO_DRAGWINDOWV8HANDLER_H__



