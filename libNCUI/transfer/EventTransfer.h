// Created by amoylel on 08/08/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_EVENTTRANSFER_H__
#define AMO_EVENTTRANSFER_H__

#include <memory>

#include <amo/singleton.hpp>
#include "ipc/Any.hpp"
#include "transfer/ClassTransfer.hpp"
namespace amo {

    // 只能同步执行，要不然不能有效阻止冒泡（多进程条件下）
    class EventTransfer
        : public ClassTransfer
        , public amo::singleton<EventTransfer> {
        
    public:
        EventTransfer();
        
        
    public:
        std::shared_ptr<ClassTransfer> getTarget() const;
        void setTarget(std::shared_ptr<ClassTransfer> val);
        
        void setRetval(const Any& val);
        
        IPCMessage::SmartType getMsg() const;
        void setMsg(IPCMessage::SmartType val);
        
        /*!
         * @fn	Any EventTransfer::getReturnValue(IPCMessage::SmartType msg);
         *
         * @brief	事件返回值.
         *
         * @param	msg	The message.
         *
         * @return	The return value.
         */
        
        Any getReturnValue(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any EventTransfer::setReturnValue(IPCMessage::SmartType msg);
         *
         * @brief	Sets return value.
         *
         * @param	msg	The message.
         *
         * @return	Any.
         */
        
        Any setReturnValue(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any EventTransfer::target(IPCMessage::SmartType msg);
         *
         * @brief	触发事件的对象.
         *
         * @param	msg	The message.
         *
         * @return	Any.
         */
        
        Any target(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any EventTransfer::data(IPCMessage::SmartType msg);
         *
         * @brief	数据.
         *
         * @param	msg	The message.
         *
         * @return	Any.
         */
        
        Any data(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any EventTransfer::stopPropagation(IPCMessage::SmartType msg);
         *
         * @brief	阻止事件向下传递.
         *
         * @param	msg	The message.
         *
         * @return	Any.
         */
        
        Any stopPropagation(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any EventTransfer::isStopPropagation(IPCMessage::SmartType msg);
         *
         * @brief	是否停止触发事件.
         *
         * @param	msg	The message.
         *
         * @return	Any.
         */
        
        Any isStopPropagation(IPCMessage::SmartType msg);
        
        virtual Any onCreateClass(IPCMessage::SmartType msg) override;
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(EventTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getReturnValue, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setReturnValue, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(target,
                                      TransferFuncMemberProperty | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(data,
                                      TransferFuncMemberProperty | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(stopPropagation,  TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isStopPropagation, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    private:
        bool m_bStopPropagation; // 是否停止冒泡
        IPCMessage::SmartType m_pMsg; // 数据
        std::shared_ptr<ClassTransfer> m_pTarget; // target
        Any retval; // 返回值
    };
}

#endif // AMO_EVENTTRANSFER_H__
