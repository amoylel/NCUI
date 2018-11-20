// Created by amoylel on 07/17/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_ACTIONTRANSFER_32C4DC11_6CBC_4D06_86C2_CBB3DC621A17_H__
#define AMO_ACTIONTRANSFER_32C4DC11_6CBC_4D06_86C2_CBB3DC621A17_H__

#include <amo/singleton.hpp>

#include "transfer/ClassTransfer.hpp"

namespace amo {

    /*!
     * @class	action
     *
     * @brief	内置事件.
     */
    
    class ActionTransfer
        : public ClassTransfer
        , public amo::singleton<ActionTransfer> {
        
    public:
        ActionTransfer();
        
        /*!
         * @fn	Any ActionTransfer::execute(IPCMessage::SmartType msg);
         * @tag static
         * @brief	执行一个内置动作，可以支持事件参考{@link 预置事件}.
         *
         * @param	#String 动作.
         *
         * @return	无.
         * @example
         *
         ```
        	include('action');
        	// 最小化当前窗口
        	action.execute('minimize');
        	// 最大化当前窗口
        	action.execute('maximize');
         ```
         */
        
        Any execute(IPCMessage::SmartType msg);
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(ActionTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(execute, TransferFuncStatic)
        
        AMO_CEF_MESSAGE_TRANSFER_END()
    };
}

#endif // AMO_ACTIONTRANSFER_32C4DC11_6CBC_4D06_86C2_CBB3DC621A17_H__

