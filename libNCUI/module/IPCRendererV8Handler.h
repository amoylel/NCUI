// Created by amoylel on 05/20/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_IPCRENDERERV8HANDLER_H__
#define AMO_IPCRENDERERV8HANDLER_H__

#include <string>

#include "handler/CefHeader.hpp"
#include "module/JsClassObjectV8Handler.h"
#include "transfer/ClassTransfer.hpp"

namespace amo {

    // JS 异步调用C++ 结果回调函数处理类
    class IPCRendererV8Handler : public JsClassObjectV8Handler
        , public ClassTransfer {
        
    public:
        IPCRendererV8Handler();
        
        virtual ~IPCRendererV8Handler();
        
        /*!
         * @fn	Any IPCRendererV8Handler::asyncExecuteResult(IPCMessage::SmartType msg);
         *
         * @brief	异步调用C++结果回调处理函数.
         *
         * @param	msg	The message.
         *
         * @return	Any.
         */
        Any asyncExecuteResult(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any IPCRendererV8Handler::runJSFunction(IPCMessage::SmartType msg);
         *
         * @brief	调用JS函数.
         *
         * @param	msg	The message.
         *
         * @return	Any.
         */
        Any runJSFunction(IPCMessage::SmartType msg);
        
        
        /*!
         * @fn	Any IPCRendererV8Handler::runJsFragment(IPCMessage::SmartType msg);
         *
         * @brief	运行JS片段.
         *
         * @param	msg	The message.
         *
         * @return	Any.
         */
        Any runJsFragment(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any IPCRendererV8Handler::triggerEvent(IPCMessage::SmartType msg);
         *
         * @brief	触发事件.
         *
         * @param	msg	The message.
         *
         * @return	Any.
         */
        Any triggerEvent(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any IPCRendererV8Handler::triggerEventAllFrame(IPCMessage::SmartType msg);
         *
         * @brief	触发所有当前Browser中所有Frame注册的事件
         * 			指定Browser， 不指定Frame.
         *
         * @param	msg	The message.
         *
         * @return	Any.
         */
        Any triggerEventAllFrame(IPCMessage::SmartType msg);
        
        
        Any emitCustomEventAllFrame(IPCMessage::SmartType msg);
        Any emitEventAllFrame(IPCMessage::SmartType msg);
        
        Any releaseAllTransfer(IPCMessage::SmartType msg);
        
        
        /*!
         * @fn	CefRefPtr<CefBrowser> IPCRendererV8Handler::getBrowser() const
         *
         * @brief	获取Browser.
         *
         * @return	The browser.
         */
        CefRefPtr<CefBrowser> getBrowser() const;
        
        /*!
         * @fn	void IPCRendererV8Handler::setBrowser(CefRefPtr<CefBrowser> pBrowser);
         *
         * @brief	设置当前对象所属Browser..
         *
         * @param	pBrowser	The browser.
         */
        void setBrowser(CefRefPtr<CefBrowser> pBrowser);
        
        // for transfer
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(IPCRendererV8Handler, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(asyncExecuteResult, 0)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(runJSFunction, 0)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(runJsFragment, 0)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(triggerEvent, 0)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(triggerEventAllFrame, 0)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(emitEventAllFrame, 0)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(emitCustomEventAllFrame, 0)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(releaseAllTransfer, 0)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
        // for handler
        AMO_CEF_BEGIN_OBJECT_FACTORY(IPCRendererV8Handler)
        AMO_CEF_END_OBJECT_FACTORY()
        
    protected:
        /*! @brief	The browser 没什么用，Browser现在可以从IPCMessage里面获取. */
        CefRefPtr<CefBrowser> m_pBrowser;
        
    };
}

#endif // AMO_IPCRENDERERV8HANDLER_H__
