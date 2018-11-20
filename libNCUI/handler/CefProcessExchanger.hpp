// Created by amoylel on 06/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_CEFPROCESSEXCHANGER_HPP__
#define AMO_CEFPROCESSEXCHANGER_HPP__


#include "handler/CefHeader.hpp"
#include "ipc/ProcessExchanger.hpp"
#include "ipc/Any.hpp"

// 通过管道消息 创建进程消息
namespace amo {

    /*!
     * @fn	static CefRefPtr<CefProcessMessage> createCefProcessMessage(
     * 		IPCMessage::SmartType msg)
     *
     * @brief	IPCMessage 转 CefProcessMessage
     *
     * @param	msg	The message.
     *
     * @return	The new cef process message.
     */
    
    static CefRefPtr<CefProcessMessage> createCefProcessMessage(
        IPCMessage::SmartType msg) {
        std::string name = msg->getMessageName();
        CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(name);
        message->GetArgumentList()->SetString(0, anyToString(*msg));
        return message;
    }
    
    /*!
     * @fn	static ::SmartType createAnyProcessMessage(
     * 		CefRefPtr<CefProcessMessage> message)
     *
     * @brief	CefProcessMessage 转 IPCMessage.
     *
     * @param	message	The message.
     *
     * @return	The new any process message.
     */
    
    static IPCMessage::SmartType createAnyProcessMessage(
        CefRefPtr<CefProcessMessage> message) {
        std::string name = message->GetName();
        IPCMessage::SmartType anyMessaage(new amo::IPCMessage());
        std::string str = message->GetArgumentList()->GetString(0).ToString();
        *anyMessaage = amo::stringToAny<amo::IPCMessage>(str);
        return anyMessaage;
    }
}




#endif // AMO_CEFPROCESSEXCHANGER_HPP__
