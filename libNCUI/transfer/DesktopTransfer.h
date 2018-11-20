// Created by amoylel on 08/20/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_DESKTOPTRANSFER_H__
#define AMO_DESKTOPTRANSFER_H__



#include <amo/singleton.hpp>

#include "transfer/ClassTransfer.hpp"

namespace amo {

    /*!
     * @class	desktop
     *
     * @extend Object
     *
     * @brief	用于获取桌面的基本信息.<br>
     * 			工作线程:**UI线程**.
     */
    
    class DesktopTransfer
        : public ClassTransfer
        , public amo::singleton < DesktopTransfer > {
    public:
        DesktopTransfer();
        
        
        
        /*!
         * @fn	Any DesktopTransfer::getScreenInfo(IPCMessage::SmartType msg);
         *
         * @tag sync static
         *
         * @brief	获取屏幕信息,包括屏幕大小和工作区域大小.
         *
         * @return	#JsonObject.
         *
         * 			|#Int width  桌面宽度
         * 			|#Int height 桌面高度
         * 			|#JsonObject rcWork 桌面工作区域信息
         * 				||#Int left 工作区域左上角坐标X
         * 				||#Int top 工作区域左上角坐标Y
         * 				||#Int right 工作区域右下角坐标X
         * 				||#Int bottom 工作区域右下角坐标Y
         * 				||#Int width 工作区域宽度
         * 				||#Int height 工作区域高度
         *
         * @example
         *
        			 ```
        				include('desktop');
        				var desktopInfo = desktop.getScreenInfo();
        				console.log(desktopInfo);
        			 ```
         */
        
        Any getScreenInfo(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any DesktopTransfer::screenshot(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	截屏.
         *
         * @return	#String base64编码的图片数据（JPG格式）.
         * @example
         *
         ```
        	include('desktop');
        	var img = desktop.screenshot();
        	console.log(img.length);
        	$('#ncuiImg2').attr("src", img);
        
         ```
         */
        
        /*!
        * @fn	Any DesktopTransfer::screenshot(IPCMessage::SmartType msg);
        *
        * @tag static sync
        *
        * @brief	截屏.
        *
        * @param	#Array 指定截屏区域Int类型 [left,top, width, height].
        *
        *
        * @return	#String base64编码的图片数据（JPG格式）.
        * @html <div id="example" class="example code" contenteditable="true"><img id="ncuiImg2" src=""></img> </div>
        * @example
        *
        ```
        	includes('desktop', 'BrowserWindow');
        	var win = BrowserWindow.current;
        	var winInfo = win.getWindowRect();
        	console.log(winInfo);
        	var img = desktop.screenshot([winInfo.x, winInfo.y, winInfo.width, winInfo.height]);
        	$('#ncuiImg2').attr("src", img);
        
        
         ```
        */
        
        Any screenshot(IPCMessage::SmartType msg);
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(DesktopTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getScreenInfo,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(screenshot,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    private:
    };
}

#endif // AMO_DESKTOPTRANSFER_H__

