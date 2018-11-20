// Created by amoylel on 08/20/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_MOUSETRANSFER_H__
#define AMO_MOUSETRANSFER_H__

#include <amo/singleton.hpp>

#include "transfer/ClassTransfer.hpp"

namespace amo {

    /*!
     * @class	mouse
     *
     * @extend Object
     *
     * @brief	鼠标控制类,所有函数均为静态函数.<br>
     * 			工作线程:**UI线程**.
     */
    
    class MouseTransfer
        : public ClassTransfer
        , public amo::singleton < MouseTransfer > {
    public:
        MouseTransfer();
        
        /*!
        * @fn	Any MouseTransfer::click(IPCMessage::SmartType msg);
        *
        * @brief	单击鼠标左键.
        *
        * @param	#Int=0 x坐标
        * @param	#Int=0 y坐标
        *
        * @return	无.
        *
        * @see leftClick=mouse.leftClick
        *
        * @example
        *
        		```
        			includes('BrowserWindow','mouse');
        			var win = BrowserWindow.current;
        			// 获取最大化按钮的位置
        			var winRect = win.getWindowRect();
        			var x = winRect.r - 50;
        			var y = winRect.y + 10;
        			// 点击最大化按钮
        			mouse.click(x,y);
        		```
        */
        
        Any click(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::leftClick(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	单击左键.
         *
         * @param	#Int=0 x坐标.
         * @param	#Int=0 y坐标.
         *
         * @return	无.
         *
         * @see click=mouse.click
         */
        
        Any leftClick(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::leftDown(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	按下左键.
         *
         * @param	#Int=0 x坐标.
         * @param	#Int=0 y坐标.
         *
         * @return	无.
         * @example
         *
        			 ```
        			 includes('BrowserWindow','mouse');
        			 var win = BrowserWindow.current;
        
        
        			 var winRect = win.getWindowRect();
        			 var x = winRect.r - winRect.width / 2;
        			 var y = winRect.y + 20;
        
        			 // 移动鼠标试试
        			 mouse.moveTo(x, y);
        			 mouse.leftDown(x,y);
        			 ```
         */
        
        Any leftDown(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::leftUp(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	弹起左键.
         *
         * @param	#Int=0 x坐标.
         * @param	#Int=0 y坐标.
         *
         * @return	无.
         */
        
        Any leftUp(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::leftDoubleClick(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	双击左键.
         *
         * @param	#Int=0 x坐标.
         * @param	#Int=0 y坐标.
         *
         * @return	无.
         *
         * @example
         *
        		 ```
        		 includes('BrowserWindow','mouse');
        		 var win = BrowserWindow.current;
        
        
        		 var winRect = win.getWindowRect();
        		 var x = winRect.r - winRect.width / 2;
        		 var y = winRect.y + 20;
        
        		 // 双击标题栏最大化窗口
        		 mouse.moveTo(x, y)
        		 mouse.leftDoubleClick(x, y);
        		 ```
         */
        
        Any leftDoubleClick(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::rightClick(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	单击右键.
         *
         * @param	#Int=0 x坐标.
         * @param	#Int=0 y坐标.
         *
         * @return	无.
         * @example
         *
        		 ```
        		 includes('BrowserWindow','mouse');
        		 var win = BrowserWindow.current;
        
        		 // 获取最大化按钮的位置
        		 var winRect = win.getWindowRect();
        		 var x = winRect.x + 150;
        		 var y = winRect.y + 20;
        
        		 // 右键
        		 mouse.rightClick(x,y);
        		 ```
         */
        
        Any rightClick(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::rightDown(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	按下右键.
         *
         * @param	#Int=0 x坐标.
         * @param	#Int=0 y坐标.
         *
         * @return	无.
         */
        
        Any rightDown(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::rightUp(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	弹起右键.
         *
         * @param	#Int=0 x坐标.
         * @param	#Int=0 y坐标.
         *
         * @return	无.
         */
        
        Any rightUp(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::middleClick(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	单击中键.
         *
         * @param	#Int=0 x坐标.
         * @param	#Int=0 y坐标.
         *
         * @return	无.
         * @example
         *
        		 ```
        		 includes('BrowserWindow','mouse');
        		 var win = BrowserWindow.current;
        
        
        		 var winRect = win.getWindowRect();
        		 var x = winRect.r - 500;
        		 var y = winRect.y + winRect.height / 2;
        
        		 //
        		 mouse.moveTo(x, y);
        		 mouse.middleClick(x, y);
        		 ```
         */
        
        Any middleClick(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::moveTo(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	移动鼠标到指定位置.
         *
         * @param	#Int=0 x坐标.
         * @param	#Int=0 y坐标.
         *
         * @return	无.
         * @example
         *
        		 ```
        		 includes('BrowserWindow','mouse');
        		 var win = BrowserWindow.current;
        
        
        		 var winRect = win.getWindowRect();
        		 var x = winRect.r - 50;
        		 var y = winRect.y + 20;
        
        		 //
        		 mouse.moveTo(x, y);
        		 ```
         */
        
        Any moveTo(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::moveR(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	移动鼠标到当前位置的相对位置.
         *
         * @param	#Int=0 x坐标.
         * @param	#Int=0 y坐标.
         *
         * @return	无.
         *
         * @example
         *
        			 ```
        			 includes('BrowserWindow','mouse');
        			 var win = BrowserWindow.current;
        
        
        			 var winRect = win.getWindowRect();
        			 var x = 100;
        			 var y = 0;
        
        			 //
        			 mouse.moveR(x, y);
        			 ```
         */
        
        Any moveR(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::mouseWheel(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	鼠标滚轮.
         *
         * @param	#Int=0 偏移量 >0 时向上滚动，<0时向下滚动.
         *
         * @return	无.
         *
         * @example
         *
        		 ```
        		 include('mouse');
        		 mouse.mouseWheel(100);
        		 ```
         */
        
        Any mouseWheel(IPCMessage::SmartType msg);
        
        /*!
        * @fn	Any MouseTransfer::saveMousePos(IPCMessage::SmartType msg);
        *
        * @tag static
        *
        * @brief	保存当前鼠标位置.
        *
        * @return	无.
        * @example
        *
        			```
        			include('mouse');
        			mouse.saveMousePos();
        			```
        */
        
        /*!
         * @fn	Any MouseTransfer::saveMousePos(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	保存指定鼠标位置.
         *
         * @param	#Int=0 x坐标.
         * @param	#Int=0 y坐标.
         *
         * @return	无.
         * @example
         *
        		 ```
        		 include('mouse');
        		 mouse.saveMousePos(100, 100);
        		 ```
         */
        
        Any saveMousePos(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::restoreMousePos(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	移动鼠标到上次保存的鼠标位置.
         *
         * @return	无.
         * @example
         *
        			 ```
        			 include('mouse');
        			 mouse.restoreMousePos();
        			 ```
         */
        
        Any restoreMousePos(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::lockMouse(IPCMessage::SmartType msg);
         *
         * @brief	锁定鼠标，禁止所有鼠标功能（未实现）.
         *
         * @return	无.
         */
        
        Any lockMouse(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::unlockMouse(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	解锁鼠标（未实现）.
         * s
         * @return	无.
         */
        
        Any unlockMouse(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::waitClick(IPCMessage::SmartType msg);
         *
         * @brief	等待一下鼠标事件（未实现）.
         *
         * @return	无.
         */
        
        Any waitClick(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::getLastClick(IPCMessage::SmartType msg);
         *
         * @brief	获取上一次按过的鼠标按键（未实现）.
         *
         *
         * @return	The last click.
         */
        
        Any getLastClick(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::getCursorPos(IPCMessage::SmartType msg);
         *
         * @brief	获取当前鼠标在屏幕上的位置.
         *
         *
         * @return	#JsonObject.
         * 			|#Int x 鼠标x轴坐标.
         * 			|#Int y 鼠标y轴坐标.
         * @example
         *
        		 ```
        		 include('mouse');
        		 var pos = mouse.getCursorPos();
        		 console.log(pos);
        		 ```
         */
        
        Any getCursorPos(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::setCursorPos(IPCMessage::SmartType msg);
         *
         * @brief	设置鼠标位置.
         *
         * @param	#Int=0 x x轴坐标.
         * @param	#Int=0 y y轴坐标
         *
         * @return	无.
         *
         * @see moveTo=mouse.moveTo
         *
         * @example
         *
        			 ```
        			 include('mouse');
        			 mouse.setCursorPos(100,100);
        			 ```
         */
        
        Any setCursorPos(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::screenToClient(IPCMessage::SmartType msg);
         *
         * @brief	将一个屏幕坐标转换为当前窗口坐标.
         *
         * @param	#Int=0 x坐标.
         * @param	#Int=0 y坐标.
         *
         * @return	#JsonObject.
         * 			|#Int x 转换后的x坐标
         * 			|#Int y 转换后的y坐标
         * @example
         *
        			 ```
        			 include('mouse');
        			 var pos = mouse.screenToClient(100,100);
        			 console.log(pos);
        			 ```
        
         */
        
        /*!
        * @fn	Any MouseTransfer::screenToClient(IPCMessage::SmartType msg);
        *
        * @brief	将一个屏幕坐标转换为指定窗口坐标.
        *
        * @param	#BrowserWindow 窗口对象
        * @param	#Int=0 x坐标.
        * @param	#Int=0 y坐标.
        *
        * @return	#JsonObject.
        * 			|#Int x 转换后的x坐标
        * 			|#Int y 转换后的y坐标
        *
        * @example
        *
        			```
        			includes('BrowserWindow', 'mouse');
        			var win = BrowserWindow.current;
        			var pos = mouse.screenToClient(win, 100, 100);
        			console.log(pos);
        			```
        */
        
        
        Any screenToClient(IPCMessage::SmartType msg);
        
        
        /*!
        * @fn	Any MouseTransfer::clientToScreen(IPCMessage::SmartType msg);
        *
        * @brief	将当前窗口坐标转换为屏幕坐标.
        *
        * @param	#Int=0 x坐标.
        * @param	#Int=0 y坐标.
        *
        * @return	#JsonObject.
        * 			|#Int x 转换后的x坐标
        * 			|#Int y 转换后的y坐标
        * @example
        *
        			```
        			include('mouse');
        			var pos = mouse.clientToScreen(100,100);
        			console.log(pos);
        			```
        
        */
        
        /*!
        * @fn	Any MouseTransfer::clientToScreen(IPCMessage::SmartType msg);
        *
        * @brief	将一个指定窗口坐标转换为屏幕坐标.
        *
        * @param	#BrowserWindow 窗口对象
        * @param	#Int=0 x坐标.
        * @param	#Int=0 y坐标.
        *
        * @return	#JsonObject.
        * 			|#Int x 转换后的x坐标
        * 			|#Int y 转换后的y坐标
        *
        * @example
        *
        		```
        		includes('BrowserWindow', 'mouse');
        		var win = BrowserWindow.current;
        		var pos = mouse.clientToScreen(win, 100, 100);
        		console.log(pos);
        		```
        */
        
        
        Any clientToScreen(IPCMessage::SmartType msg);
        
        
        
        /*!
         * @fn	Any MouseTransfer::hideCursor(IPCMessage::SmartType msg);
         *
         * @brief	隐藏鼠标，只对当前程序窗口有效.
         *
         * @return	无.
         *
         * @example
         *
        		 ```
        		 includes('mouse');
        		 mouse.hideCursor();
        		 setTimeout(function(){
        			mouse.showCursor();
        		 }, 5000);
        		 ```
         */
        
        Any hideCursor(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any MouseTransfer::showCursor(IPCMessage::SmartType msg);
         *
         * @brief	显示鼠标.
         *
         * @return	无.
         *
         * @example
         *
        			 ```
        			 includes('mouse');
        			 mouse.showCursor();
        			 ```
         */
        
        Any showCursor(IPCMessage::SmartType msg);
        
        
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(MouseTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(leftClick, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(leftDown, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(leftUp, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(leftDoubleClick, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(rightClick, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(rightDown, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(rightUp, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(middleClick, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(moveTo, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(moveR, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(mouseWheel, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(saveMousePos, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(restoreMousePos, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(lockMouse, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(unlockMouse, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(waitClick, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getLastClick,
                                      TransferFuncStatic | TransferExecSync)
                                      
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getCursorPos,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setCursorPos, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(screenToClient,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(clientToScreen,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(click, TransferFuncStatic)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(hideCursor, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(showCursor, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    private:
        Any SendMouseEvent(IPCMessage::SmartType msg, uint32_t dwFlags,
                           int mouseData = 0);
                           
        POINT pt;
    };
}

#endif // AMO_MOUSETRANSFER_H__

