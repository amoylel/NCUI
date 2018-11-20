// Created by amoylel on 08/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_IPCTRANSFER_H__
#define AMO_IPCTRANSFER_H__
#include <amo/singleton.hpp>

#include "transfer/ClassTransfer.hpp"




namespace amo {

    /*!
     * @class	ipc
     *
     * @extend Object
     *
     * @brief	进程间通信类，你可以通过该类发其他页面发送消息.<br>
     * 			但要注意的是不能在消息处理函数中调用任何导致UI阻塞的函数如(弹出窗口 alert dialog)<br>
     * 			工作线程:**UI线程**.
     * @example
     *
    			 ```
    				include('ipc');
    				ipc.unique('ipc.exec', function(){
    					console.log(arguments);
    					return 'hello';
    				});
    				ipc.unique('ipc.sync', function(){
    					console.log(arguments);
    					return 'ipc.sync';
    				});
    				ipc.unique('ipc.async', function(){
    					console.log(arguments)
    					return 'ipc.async';
    				});
    				ipc.unique('ipc.test', function(){
    					console.log(arguments);
    					return 666;
    				});
    
    			 ```
     */
    
    class IPCTransfer
        : public ClassTransfer
        , public amo::singleton<IPCTransfer> {
    public:
        IPCTransfer();
        
        /*!
         * @fn	Any IPCTransfer::exec(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	发送一条消息.
         *
         * @param	#Args... 要发送的内容.
         *
         * @return	无.
         *
         * @example
         *
        	 ```
        		include('ipc');
        		ipc.exec('ipc.exec', 12, '88', 12.33);
        	 ```
         */
        
        Any exec(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any IPCTransfer::sync(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	发送一条同步消息.
         *
         * @param	#Args... 要发送的内容.
         *
         * @return	#Any 消息返回的结果,只会返回第一个监听函数的返回值.
         *
         * @example
         *
        		 ```
        			include('ipc');
        			var cc = ipc.sync('ipc.sync', 12);
        			console.log(cc);
        			console.assert(ipc.sync('ipc.sync', 12) == 'ipc.sync');
        			console.assert(ipc.sync('ipc.async', 'test') == 'ipc.async');
        
        		 ```
         */
        
        Any sync(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any IPCTransfer::async(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	发送一条异步消息，可以通过回调函数获取结果.
         *
         * @param	#Args... 要发送的内容.
         *
         * @param	#Function 要接收消息返回值的回调函数,任意位置都可以，但只能有一个, 只会返回第一个监听函数的返回值.
         *
         * @return	无.
         *
         * @example
         *
        			 ```
        				include('ipc');
        				ipc.async('ipc.sync', function(val){console.assert(val == 'ipc.sync');});
        				ipc.async('ipc.async', function(val){console.assert(val == 'ipc.async');}, '2382934', 888);
        			 ```
         */
        
        Any async(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any IPCTransfer::dispatchEvent(IPCMessage::SmartType msg);
         *
         * @brief	向所有页面的document发送自定义事件.
         *
         * @param	#String 事件名.
         * @param	#JsonObject 事件内容，CustomEvent的detail字段
         *
         * @return	无.
         */
        
        Any dispatchEvent(IPCMessage::SmartType msg);
        
        Any joinChannel(IPCMessage::SmartType msg);
        Any getChannel(IPCMessage::SmartType msg);
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(IPCTransfer, ClassTransfer)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(exec, TransferFuncStatic | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(sync, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(async, TransferFuncStatic | TransferExecAsync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(dispatchEvent, TransferFuncStatic | TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    };
}

#endif // AMO_IPCTRANSFER_H__

