// Created by amoylel on 06/26/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_GLOBALSHORTCUTTRANSFER_H__
#define AMO_GLOBALSHORTCUTTRANSFER_H__

#include <amo/singleton.hpp>

#include "transfer/ClassTransfer.hpp"

namespace amo {

    /*!
     * @class	globalShortcut
     *
     * @extend Object
     *
     * @brief	全局快捷键.<br>
     * 			工作线程 **UI线程**
     */
    
    class GlobalShortcutTransfer
        : public ClassTransfer
        , public amo::singleton<GlobalShortcutTransfer> {
    public:
    
        GlobalShortcutTransfer();
        
        /*!
         * @fn	Any GlobalShortcutTransfer::registerShortcut(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	通过组合键注册快捷键.
         *
         * @param	#String 字符串.
         *
         * @return	#Int 快捷键ID ， 当返回值为0时表示注册失败.
         * @example
         *
        		```
        			include('globalShortcut');
        			var hotkey = globalShortcut.registerShortcut('MOD_CONTROL+MOD_ALT+D');
        			console.log(hotkey);
        			console.assert(hotkey != 0);
        
        		```
         */
        
        /*!
        * @fn	Any GlobalShortcutTransfer::registerShortcut(IPCMessage::SmartType msg);
        *
        * @tag static sync
        *
        * @brief	注册快捷键.
        *
        * @param	#JsonObject 字符串.<br>
        * 						[**Boolean**] ctrl		键盘上的CTRL按键，默认为false, 对应字符串MOD_CONTROL.<br>
        * 						[**Boolean**] win		键盘上的WIN按键，默认为false, 对应字符串MOD_WIN.<br>
        * 						[**Boolean**] alt		键盘上的ALT按键，默认为false, 对应字符串MOD_ALT.<br>
        * 						[**Boolean**] shift		键盘上的SHIFT按键，默认为false, 对应字符串MOD_SHIFT.<br>
        * 						[**String**]  key		键盘上的字母或数字按键，一个字符.<br>
        * 						[**String**]  action	当前快捷键所触发的{@link 预定义事件=预置事件}.<br>
        *
        *
        * @return	#Int 快捷键ID， 当返回值为0时表示注册失败.
        *
        * @example
        *
        			```
        			include('globalShortcut');
        			var hotkey = globalShortcut.registerShortcut({ ctrl: true, alt: true, key: 'O', action: 'toggleVisible' });
        			console.log(hotkey);
        			// 按下Ctrl+Atl+O可以显示或隐藏当前窗口
        			console.assert(hotkey != 0);
        
        			```
        */
        
        Any registerShortcut(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any GlobalShortcutTransfer::isRegistered(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	通过ID判断快捷键是否注册.
         *
         * @param	#Int 快捷键ID.
         *
         * @return	无.
         *
         * @example
         *
        			 ```
        				include('globalShortcut');
        				var hotkey = globalShortcut.registerShortcut('MOD_CONTROL+MOD_ALT+M');
        				console.assert(globalShortcut.isRegistered(hotkey) == true);
        
        			 ```
         */
        
        /*!
        * @fn	Any GlobalShortcutTransfer::isRegistered(IPCMessage::SmartType msg);
        * @tag static
        * @brief	通过组合键判断快捷键是否注册.
        *
        * @param	#String 组合键.
        *
        * @return	无.
        * @example
        *
        			```
        				include('globalShortcut');
        				var hotkey = globalShortcut.registerShortcut('MOD_CONTROL+MOD_ALT+M');
        				console.assert(globalShortcut.isRegistered('MOD_CONTROL+MOD_ALT+M') == true);
        			```
        
        */
        
        Any isRegistered(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any GlobalShortcutTransfer::unregisterShortcut(IPCMessage::SmartType msg);
         * @tag static
         * @brief	通过ID注销快捷键.
         *
         * @param	#Int 快捷键ID.
         *
         * @return	无.
         */
        
        /*!
        * @fn	Any GlobalShortcutTransfer::unregisterShortcut(IPCMessage::SmartType msg);
        *
        * @tag static
        *
        * @brief	通过组合键注销快捷键.
        *
        * @param	#String 组合键所组成的字符串.
        *
        * @return	无.
        * @example
        *
        		```
        			include('globalShortcut');
        			var hotkey = globalShortcut.registerShortcut('MOD_CONTROL+MOD_ALT+N');
        			console.assert(globalShortcut.isRegistered('MOD_CONTROL+MOD_ALT+N') == true);
        
        			globalShortcut.unregisterShortcut('MOD_CONTROL+MOD_ALT+N');
        			console.assert(globalShortcut.isRegistered('MOD_CONTROL+MOD_ALT+N') == false);
        
        		```
        */
        
        Any unregisterShortcut(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any GlobalShortcutTransfer::unregisterAll(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	注销所有快捷键.
         *
         * @return	无.
         *
         * @example
         *
        		 ```
        			 include('globalShortcut');
        			 globalShortcut.unregisterAll();
        		 ```
         */
        
        Any unregisterAll(IPCMessage::SmartType msg);
        
        
        /*!
        * @event	Any GlobalShortcutTransfer::hotkeydown(IPCMessage::SmartType msg);
        *
        * @tag static
        *
        * @brief	快捷键事件监听，当按下注册的快捷键时，该事件会被触发.
        * 			但是如果注册快捷键时指定了action，且该action能被程序处理，那么该快捷键的事件不会被触发.
        *
        * @param	#Int 快捷键ID
        *
        * @param	#JsonObject 快捷键相关信息
        * 				[**Boolean**] ctrl		键盘上的CTRL按键， 对应字符串MOD_CONTROL.<br>
        * 				[**Boolean**] win		键盘上的WIN按键， 对应字符串MOD_WIN.<br>
        * 				[**Boolean**] alt		键盘上的ALT按键， 对应字符串MOD_ALT.<br>
        * 				[**Boolean**] shift		键盘上的SHIFT按键, 对应字符串MOD_SHIFT.<br>
        * 				[**String**]  key		键盘上的字母或数字按键，一个字符.<br>
        * 				[**String**]  action	当前快捷键所触发的{@link 预定义事件=预置事件}.<br>
        *
        * @example
        *
        	```
        	include('globalShortcut');
        	globalShortcut.on('hotkeydown', function(hotkey){
        		console.log(hotkey);
        	});
        	```
        */
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(GlobalShortcutTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(registerShortcut, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isRegistered, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(unregisterShortcut, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(unregisterAll, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_END()
    private:
    };
}


#endif // AMO_GLOBALSHORTCUTTRANSFER_H__