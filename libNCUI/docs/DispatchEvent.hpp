// Created by amoylel on 11/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef LIBNCUI_DISPATCHEVENT_3C949AC8_6F42_4A67_A7C6_EF033BFA40BD_HPP__
#define LIBNCUI_DISPATCHEVENT_3C949AC8_6F42_4A67_A7C6_EF033BFA40BD_HPP__


namespace amo {

    /**
     * @class	事件驱动编程
     * 			
     * @id basicDispatchEvent
     * 	   
     * @chapter basic
     * 			
     * @brief	你可以使用事件驱动编程防止NCUI污染你的页面.
     */
    
    class DispatchEvent {
    public:
    
        /**
         * @section 监听事件
         *
         * @brief	使用document.addEventListener监听一个自定义事件.
         * 			NCUI会将ipc.dispatchEvent发送的消息重定向到document上
         * @example
         *
         ```
        	document.addEventListener('NCUI.TEST', function(event){
        		console.log(arguments);
        	});
        	// 下面的代码可以写到Node.js里面去，这里只做演示
        	include('ipc');
        	ipc.dispatchEvent('NCUI.TEST', {name:3, value:'2323'});
        
         ```
         */
        
        void foo1() {
        
        }
        
        /**
         * @section 发送事件
         *
         * @brief	NCUI允许你通过document.dispatchEvent向Node.js或其他页面发送自定消息.<br>
         * 			1. "ipc.exec", 通过{@link ipc.exec}向Node.js或其他页面发送消息，使用ipc.on或ipc.unique可以监听该消息<br>
         * 			2. "ipc.dispatchEvent", 通过{@link ipc.dispatchEvent}他页面的document对象发送消息，使用document.addEventListener可以监听该消息，Node.js代码无法接收该消息<br>
         * 			说明:**必须在CustomEvent的detail对象中增加eventName字段表明需要触发的事件名称**
         * @example
         *
         ```
        	// 你可以将ipc.unique写到Node.js代码中,或编辑manifest.json，开启Node.js支持，并以调试模块运行NCUI-DEV.exe查看效果
        	include('ipc');
        	ipc.unique('NCUI.TEST2', function(){
        		console.log('NCUI.TEST2');
        		console.log(arguments);
        	});
        
        	document.addEventListener('NCUI.TEST3', function(event){
        		console.log('NCUI.TEST3');
        		console.log(arguments);
        	});
        
        	document.dispatchEvent(new CustomEvent('ipc.exec', {
        			detail: {
        				eventName: 'NCUI.TEST2',
        				value: 3232
        			}
        		}));
        
        	document.dispatchEvent(new CustomEvent('ipc.dispatchEvent', {
        			detail: {
        				eventName: 'NCUI.TEST3',
        				value: 3232,
        				path: 'D:/'
        			}
        		}));
        
         ```
         */
        
        void foo2() {
        
        }
    };
}



#endif //LIBNCUI_DISPATCHEVENT_3C949AC8_6F42_4A67_A7C6_EF033BFA40BD_HPP__