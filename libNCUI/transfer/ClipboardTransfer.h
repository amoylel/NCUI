// Created by amoylel on 14/11/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef LIBNCUI_CLIPBOARDTRANSFER_7CB74BAD_E5C1_4CE6_B3D2_E9CA9C435548_H__
#define LIBNCUI_CLIPBOARDTRANSFER_7CB74BAD_E5C1_4CE6_B3D2_E9CA9C435548_H__

#include <amo/singleton.hpp>

#include "transfer/ClassTransfer.hpp"

namespace amo {

    /*!
    * @class	clipboard
    *
    * @brief	剪贴板.
    */
    class ClipboardTransfer
        : public ClassTransfer
        , public amo::singleton<ClipboardTransfer> {
    public:
        ClipboardTransfer();
        
        /**
         * @fn	Any ClipboardTransfer::startWatch(IPCMessage::SmartType msg);
         * @tag static sync
         *
         * @brief	监听剪贴板事件.
         *
         * @return	#Boolean.
         * @example
         *
         ```
        	include('clipboard');
        	console.assert(clipboard.startWatch() == true);
         ```
         */
        
        Any startWatch(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any ClipboardTransfer::stopWatch(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	停止监听剪贴板事件.
         *
         * @return	#Boolean.
         * @example
         *
         ```
        	include('clipboard');
        	console.assert(clipboard.stopWatch() == true);
         ```
         */
        
        Any stopWatch(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any ClipboardTransfer::isWatching(IPCMessage::SmartType msg);
         *
         * @tag sync static
         *
         * @brief	是否正在监听剪贴板事件.
         *
         * @return	#Boolean.
         * @example
         *
         ```
        	include('clipboard');
        	clipboard.startWatch();
        	console.assert(clipboard.isWatching() == true);
        	clipboard.stopWatch();
        	console.assert(clipboard.isWatching() == false);
        	clipboard.startWatch();
         ```
         */
        
        Any isWatching(IPCMessage::SmartType msg);
        /**
        * @fn	Any ClipboardTransfer::writeText(IPCMessage::SmartType msg);
        * @tag static sync
        * @brief	向剪贴板中写入字符串.
        *
        * @param	#String 要写入的字符串.
        *
        * @return	#Undefined.
        *
        * @example
        ```
        	include('clipboard');
        	console.assert(clipboard.writeText('NCUI Clipboard Text 中文') == true);
        ```
        *
        */
        
        Any writeText(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any ClipboardTransfer::writeImage(IPCMessage::SmartType msg);
         * @tag static sync
         *
         * @brief	写入图片到剪贴板.
         *
         * @param	#String 图片数据 base64编码的图片.
         *
         * @return	#Boolean.
         */
        
        /**
        * @fn	Any ClipboardTransfer::writeImage(IPCMessage::SmartType msg);
        * @tag static sync
        *
        * @brief	写入图片到剪贴板.
        *
        * @param	#String 图片路径（本地文件路径）,(也可以是一个BigStr封装的对象，如果图片太大，最好使用BigStr封装，进程间通信的管道只有2M，超过这个大小的数据不能被正确送达).
        *
        * @return	#Boolean.
        * @html <div id="example" class="example code" contenteditable="true"><img id="ncuiImg_writeImage" src="@file:///%appDir%images/splash.jpg"></img> </div>
        * @example
        *
        ```
        	include('clipboard');
        	clipboard.writeImage('');
        	//将图片转化成base64
        	function convertImgToBase64(url, callback, outputFormat) {
        		var canvas = document.createElement('CANVAS'),
        	　  ctx = canvas.getContext('2d'),
        	  　img = new Image;　　
        		img.crossOrigin = 'Anonymous';　　
        		img.onload = function() {　　
        			canvas.height = img.height;　　
        			canvas.width = img.width;　　
        			ctx.drawImage(img, 0, 0);　　
        			var dataURL = canvas.toDataURL(outputFormat || 'image/jpeg');　　
        			callback.call(this, dataURL);　　
        			canvas = null;
        		};
        		img.src = url;
        	}
        
        	var url = "@file:///%appDir%images/splash.jpg"; //这是站内的一张图片资源，采用的相对路径
        	convertImgToBase64(url, function(base64Img) {
        		//转化后的base64
        		clipboard.writeImage(base64Img);
        
        		include('BigStr');
        		// 也可以使用BigStr传递数据
        		clipboard.writeImage(new BigStr(base64Img));
        	});
        
        
        
        ```
        */
        Any writeImage(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any ClipboardTransfer::writeImageFromFile(IPCMessage::SmartType msg);
         * @tag static sync
         * @brief	将一个本地图片文件内容写入到剪贴板.
         *
         * @param	#String 文件路径.
         *
         * @return	#Boolean.
         * @example
         *
         ```
        	include('clipboard');
        	console.assert(clipboard.writeImageFromFile('%appDir%images/splash.jpg') == true);
         ```
         */
        
        Any writeImageFromFile(IPCMessage::SmartType msg);
        
        /**
        * @fn	Any ClipboardTransfer::writeFiles(IPCMessage::SmartType msg);
        * @tag static sync
        *
        * @brief	写入文件到剪贴板.
        *
        * @param	#String 文件路径.
        *
        * @return	#Boolean.
        *
        * @example
        *
        ```
        	include('clipboard');
        	console.assert(clipboard.writeFiles('manifest.json') == true);
        ```
        */
        
        
        /**
         * @fn	Any ClipboardTransfer::writeFiles(IPCMessage::SmartType msg);
         * @tag static sync
         *
         * @brief	写入文件到剪贴板.
         *
         * @param	#Array 文件路径.
         *
         * @return	#Boolean.
         * @example
         *
         ```
        	include('clipboard');
        	console.assert(clipboard.writeFiles(['skin', 'manifest.json']) == true);
         ```
         */
        
        Any writeFiles(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any ClipboardTransfer::readText(IPCMessage::SmartType msg);
         * @tag static sync
         *
         * @brief	读取剪贴板中的字符串.
         *
         * @return	#String.
         *
         * @example
         ```
        	include('clipboard');
        	console.assert(clipboard.writeText('NCUI Clipboard Text 中文') == true);
        	var str = clipboard.readText();
        	console.log(str);
        	console.assert(str == 'NCUI Clipboard Text 中文');
         ```
         *
         */
        
        Any readText(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any ClipboardTransfer::readImage(IPCMessage::SmartType msg);
         * @tag static sync
         * @brief	读取图片,读取到的图片以Base64编码返回
         *
         * @param	#String="jpeg" 图片格式.
         *
         * @return	#String 图片数据.
         * @html <div id="example" class="example code" contenteditable="true"><img id="ncuiImg" src="http:://127.0.0.1:8030/@file:///%skinDir%logo.ico"></img> </div>
         * @example
         *
         ```
        	include('clipboard');
        	clipboard.writeImageFromFile('%appDir%images/splash.jpg');
        	var str = clipboard.readImage('.jpeg');
        	$('#ncuiImg').attr("src", str);
         ```
         */
        
        Any readImage(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any ClipboardTransfer::readFiles(IPCMessage::SmartType msg);
         * @tag static sync
         * @brief	读取文件.
         *
         * @param	#Boolean=false 是否包含目录.
         *
         * @return	#Array 返回一个包含文件路径的字符串.
         * @example
         *
         ```
        	include('clipboard');
        	var arr = clipboard.readFiles();
        	console.log(arr);
        	console.log(arr.length);
         ```
         */
        
        Any readFiles(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any ClipboardTransfer::saveText(IPCMessage::SmartType msg);
         *
         * @brief	保存剪贴板中的文本数据到磁盘.
         *
         * @param	#String 文件路径.
         *
         * @return	#Boolean.
         * @example
         *
         ```
        	include('clipboard');
        	clipboard.writeText('1111111111111111111111中文');
        	console.assert(clipboard.saveText('test3.json') == true);
        	include('Path');
        	console.assert(Path.Remove('test3.json') == true);
         ```
         */
        
        Any saveText(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any ClipboardTransfer::saveImage(IPCMessage::SmartType msg);
         *
         * @brief	保存剪贴板中的文本数据到磁盘.
         *
         * @param	#String 文件路径.
         *
         * @return	#Boolean.
         * @html <div id="example" class="example code" contenteditable="true"><img id="ncuiImg4" src=""></img> </div>
         * @example
         *
         ```
        	includes( 'clipboard');
        	console.assert(clipboard.screenshot() == true);
        	console.assert(clipboard.saveImage('test2/screen.jpg') == true);
        	console.assert(clipboard.saveImage('test2/screen.bmp') == true);
        	console.assert(clipboard.saveImage('test2/screen.png') == true);
        	$('#ncuiImg4').attr('src', '');
        	$('#ncuiImg4').attr('src', '@file:///%appDir%test2/screen.jpg');
         ```
         */
        
        Any saveImage(IPCMessage::SmartType msg);
        
        
        /*!
        * @fn	Any ClipboardTransfer::saveFiles(IPCMessage::SmartType msg);
        *
        * @brief	保存剪贴板中的文件数据到磁盘（单个文件）.
        *
        * @param	#String 文件路径.
        *
        * @return	#Boolean.
        * @example
        *
        ```
        	include('clipboard');
        	clipboard.empty();
        	console.assert(clipboard.saveFiles('%appDir%test2.json') == false);
        	clipboard.writeFiles('manifest.json');
        	console.assert(clipboard.saveFiles('%appDir%test2.json') == true);
        	include('Path');
        	console.assert(Path.Remove('test2.json') == true);
        ```
        */
        
        /*!
         * @fn	Any ClipboardTransfer::saveFiles(IPCMessage::SmartType msg);
         *
         * @brief	保存剪贴板中的文本数据到磁盘.
         *
         * @param	#JsonArray 文件路径.
         *
         * @return	#Boolean.
         * @example
         *
         ```
        	include('clipboard');
        	clipboard.empty();
        	console.assert(clipboard.saveFiles('%appDir%test2.json') == false);
        	clipboard.writeFiles(['manifest.json', 'libcef.dll', 'C:/windows/notepad.exe']);
        	console.assert(clipboard.saveFiles([
        		{src: 'manifest.json', dst:'%appDir%test2/test2.json'},
        		{src: 'libcef.dll', dst:'%appDir%test2/libcef.dll'},
        		{src: 'C:/windows/notepad.exe', dst:'%appDir%test2/nodepad.exe'}
        	]) == true);
        	include('Path');
        	Path.Remove('test2.json');
        	Path.RemoveAll('test2');
         ```
         */
        
        Any saveFiles(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any ClipboardTransfer::hasFormat(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	查看剪贴板中是否存在指定格式的数据.
         *
         * @param	#Int 需要判断的格式.
         *
         * @return	#Boolean.
         * @example
         *
         ```
        	include('clipboard');
        	clipboard.hasFormat(clipboard.CF_TEXT);
         ```
         */
        
        Any hasFormat(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any ClipboardTransfer::empty(IPCMessage::SmartType msg)
         * @tag static sync
         * @brief	清空剪贴板.
         *
         *
         * @return	#Boolean.
         *
         * @example
         ```
        	 include('clipboard');
        	 clipboard.empty();
        	 console.assert(clipboard.isEmpty() == true);
        	 clipboard.writeText('1232');
        	 console.assert(clipboard.isEmpty() == false);
         ```
         */
        
        Any empty(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any ClipboardTransfer::isEmpty(IPCMessage::SmartType msg)
         * @tag static sync
         * @brief	判断剪贴板是否为空.
         *
         *
         * @return	#Boolean.
         * @example
         ```
        	 include('clipboard');
        	 clipboard.empty();
        	 console.assert(clipboard.isEmpty() == true);
        	 clipboard.writeText('1232');
        	 console.assert(clipboard.isEmpty() == false);
         ```
         */
        
        Any isEmpty(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any ClipboardTransfer::getFormats(IPCMessage::SmartType msg);
         * @tag static sync
         *
         * @brief	获取剪贴板中所有的数据格式.
         *
         * @return	#Array.
         *
         * @example
         ```
        	 include('clipboard');
        	 clipboard.empty();
        	 var arr = clipboard.getFormats();
        	 var len = arr.length;
        	 console.log(arr);
        	 console.log(len);
        	 console.assert(len == 0);
        	 clipboard.writeText('1232');
        	 arr = clipboard.getFormats();
        	 len = arr.length;
        	 console.log(arr);
        	 console.log(len);
         ```
         */
        
        Any getFormats(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any ClipboardTransfer::effect(IPCMessage::SmartType msg);
         * @tag static sync
         *
         * @brief	判断剪贴板中的数据是复制的还是粘贴的(只对资源管理器中的文件操作有效).
         *
         * @return	#Int 未知(0) 复制(1) 剪切(2) 连接(4).
         *
         * @example
         *
         ```
        	 include('clipboard');
        	 clipboard.empty();
        	 console.assert(clipboard.effect() == 0);
        	 clipboard.writeFiles('manifest.json');
        	 console.assert(clipboard.effect() == 1);
         ```
        
         */
        
        Any effect(IPCMessage::SmartType msg);
        
        
        /*!
        * @fn	Any ClipboardTransfer::screenshot(IPCMessage::SmartType msg);
        *
        * @tag static sync
        *
        * @brief	截屏,将数据保存到剪贴板中（JPG格式）.
        *
        * @return	#Boolean
        * @example
        *
        ```
        	include('clipboard');
        	console.assert(clipboard.screenshot() == true);
        	var img = clipboard.readImage();
        	$('#ncuiImg3').attr("src", img);
        
        ```
        */
        
        /*!
        * @fn	Any ClipboardTransfer::screenshot(IPCMessage::SmartType msg);
        *
        * @tag static sync
        *
        * @brief	截屏,将数据保存到剪贴板中（JPG格式）.
        *
        * @param	#Array 指定截屏区域Int类型 [left,top, width, height].
        *
        *
        * @return	#Boolean.
        * @html <div id="example" class="example code" contenteditable="true"><img id="ncuiImg3" src=""></img> </div>
        * @example
        *
        ```
        	includes('clipboard', 'BrowserWindow');
        	var win = BrowserWindow.current;
        	var winInfo = win.getWindowRect();
        	console.log(winInfo);
        	console.assert(clipboard.screenshot([winInfo.x, winInfo.y, winInfo.width, winInfo.height]) == true);
        	var img = clipboard.readImage();
        	$('#ncuiImg3').attr("src", img);
        
        
        ```
        */
        Any screenshot(IPCMessage::SmartType msg);
        
        
        Any test(IPCMessage::SmartType msg);
        
        /**
        * @event	Any ClipboardTransfer::copy(IPCMessage::SmartType msg);
        *
        * @brief	监听到复制事件时触发 .
        *
        * @example
        *
        ```
        	include('clipboard');
        	clipboard.unique('copy', function(){
        		console.log('trigger copy event');
        	});
        ```
        */
        
        /**
        * @event	Any ClipboardTransfer::cut(IPCMessage::SmartType msg);
        *
        * @brief	监听到剪切事件时触发（只在文件剪切时触发） .
        * @example
        *
        ```
        	include('clipboard');
        	clipboard.unique('cut', function(){
        		console.log('trigger cut event');
        	});
        ```
        */
        
        /**
        * @event	Any ClipboardTransfer::empty(IPCMessage::SmartType msg);
        *
        * @brief	清空剪贴板时触发 .
        * @example
        *
        ```
        	include('clipboard');
        	clipboard.unique('empty', function(){
        		console.log('trigger empty event');
        	});
        ```
        */
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(ClipboardTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(startWatch, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(stopWatch, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isWatching, TransferFuncStatic | TransferExecSync)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(writeText, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(writeImage, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(writeImageFromFile,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(writeFiles, TransferFuncStatic | TransferExecSync)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(readText, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(readImage, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(readFiles, TransferFuncStatic | TransferExecSync)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(saveText, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(saveImage, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(saveFiles, TransferFuncStatic | TransferExecSync)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(hasFormat, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(empty, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isEmpty, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getFormats, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(effect, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(screenshot, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(test, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(CF_TEXT, (int)CF_TEXT)
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(CF_UNICODETEXT, (int)CF_UNICODETEXT)
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(CF_BITMAP, (int)CF_BITMAP)
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(CF_HDROP, (int)CF_HDROP)
        
        AMO_CEF_MESSAGE_TRANSFER_END()
    private:
    };
}



#endif //LIBNCUI_CLIPBOARDTRANSFER_7CB74BAD_E5C1_4CE6_B3D2_E9CA9C435548_H__
