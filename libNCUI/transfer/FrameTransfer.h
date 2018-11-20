// Created by amoylel on 07/03/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_FRAMETRANSFER_H__
#define AMO_FRAMETRANSFER_H__

#include <amo/singleton.hpp>
#include "handler/CefHeader.hpp"
#include "transfer/ClassTransfer.hpp"


namespace amo {

    /*!
     * @class	Frame
     *
     * @chapter cef
     *
     * @extend Object
     *
     * @brief	CEF Frame 封装.<br>
     * 			工作线程:**UI线程**.
     */
    
    class FrameTransfer
        : public ClassTransfer
        , public amo::singleton < FrameTransfer > {
    public:
        FrameTransfer();
        FrameTransfer(CefRefPtr<CefFrame> pFrame);
        
        
        Any current(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::injectScript(IPCMessage::SmartType msg);
         *
         * @brief	向页面注入JS代码.
         *
         * @param	#String JS代码.
         *
         * @return	#Boolean true成功/false失败.
         * @example
         *
         ```
        	include('Frame');
        	var frame = Frame.current;
        	frame.injectScript('@file:///example/js/testJs.js');
         ```
         */
        
        Any injectScript(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::injectCSS(IPCMessage::SmartType msg);
         *
         * @brief	向页面注入CSS代码.
         *
         * @param	#String CSS代码.
         *
         * @return	#Boolean true成功/false失败.
         *
         * @example
         *
         ```
        	 include('Frame');
        	 var frame = Frame.current;
        	 frame.injectCSS('@file:///example/css/zui-theme.css');
         ```
         */
        
        Any injectCSS(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::IsValid(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	当前页面是否有效.
         *
         *
         * @return	#Boolean true有效/false无效.
         * @example
         *
         ```
        	includes('Frame');
        	var frame = Frame.current;
        	console.log(frame.IsValid());
        
         ```
         */
        
        Any IsValid(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::Undo(IPCMessage::SmartType msg);
         *
         * @brief	撤消，编辑框内有效.
         *
         * @return	无.
         *
         */
        
        Any Undo(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::Redo(IPCMessage::SmartType msg);
         *
         * @brief	重做，编辑框内有效.
         *
         * @return	无.
         */
        
        Any Redo(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::Cut(IPCMessage::SmartType msg);
         *
         * @brief	剪切，编辑框内有效.
         *
         *
         * @return	无.
         */
        
        Any Cut(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::Copy(IPCMessage::SmartType msg);
         *
         * @brief	复制，编辑框内有效.
         *
         *
         * @return	无.
         */
        
        Any Copy(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::Paste(IPCMessage::SmartType msg);
         *
         * @brief	粘贴，编辑框内有效.
         *
         *
         * @return	无.
         */
        
        Any Paste(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::Delete(IPCMessage::SmartType msg);
         *
         * @brief	删除，编辑框内有效.
         *
         *
         * @return	无.
         */
        
        Any Delete(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::SelectAll(IPCMessage::SmartType msg);
         *
         * @brief	全选.
         *
         * @return	无.
         * @example
         *
         ```
         includes('Frame');
         Frame.current.SelectAll();
        
         ```
         */
        
        Any SelectAll(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::ViewSource(IPCMessage::SmartType msg);
         * @ignore
         * @brief	查看网页源代码.
         *
         * @return	无.
         */
        
        Any ViewSource(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::GetSource(IPCMessage::SmartType msg);
         *
         * @ignore
         *
         * @brief	获取网页源代码.
         *
         *
         * @return	#String.
         */
        
        Any GetSource(IPCMessage::SmartType msg);
        
        Any GetText(IPCMessage::SmartType msg);
        
        Any LoadRequest(IPCMessage::SmartType msg) ;
        
        /*!
         * @fn	Any FrameTransfer::LoadURL(IPCMessage::SmartType msg);
         *
         * @brief	加载URL.
         *
         * @param	#String URL.
         *
         * @return	无.
         * @example
         *
         ```
        	include('Frame');
        	Frame.current.LoadURL('http://www.baidu.com');
         ```
         */
        
        Any LoadURL(IPCMessage::SmartType msg) ;
        
        /**
         * @fn	Any FrameTransfer::LoadString(IPCMessage::SmartType msg);
         *
         * @brief	加载HTML代码.
         *
         * @param	#String HTML代码.
         * @param	#String="" URL,选填
         *
         * @return	#Undefined.
         * @example
         *
         ```
        	//@skip vConsole转换存在问题，程序中可以正常使用
        	include('Frame');
        	Frame.current.LoadString(`
        		<!DOCTYPE html>
        		<html lang="zh-CN">
        
        			<head>
        				<meta charset='utf-8'>
        				<script type="text/javascript">
        					includes('app', 'BrowserWindow');
        				</script>
        				<style>
        					html,
        					body {
        						width: 100%;
        						height: 100%;
        						overflow: hidden;
        					}
        					.container {
        						position: absolute;
        						top: 0;
        						left: 0;
        						right: 0;
        						bottom: 0;
        					}
        
        					.drag {
        						-webkit-app-region: drag;
        					}
        
        					.no-drag {
        						-webkit-app-region: no-drag;
        					}
        				</style>
        			</head>
        
        			<body>
        				<div class="container ">
        					<h1>我是一个普通浏览器窗口<a href="#" onclick="recoveryManifest();">点我恢复manifest.json</a></h1>
        					<h1>我是一个普通浏览器窗口<a href="#" onclick="app.restart();">点我重启APP</a></h1>
        				</div>
        			</body>
        			<script type="text/javascript">
        
        
        				function recoveryManifest() {
        					include('Path');
        					var path = new Path('manifest2.json');
        					if(!path.isExsit()) return;
        					path.copyTo('manifest.json');
        					path.remove();
        				}
        			</script>
        
        		</html>
        	`);
         ```
         */
        
        Any LoadString(IPCMessage::SmartType msg) ;
        
        /*!
         * @fn	Any FrameTransfer::IsMain(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	当前页面是否为主页面.
         *
         *
         * @return	#Boolean.
         * @example
         *
         ```
        	include('Frame');
        	console.assert(Frame.current.IsMain() == true);
         ```
         */
        
        Any IsMain(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::IsFocused(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	当前页面是否获得焦点.
         *
         * @return	#Boolean.
         *
         * @example
         *
         ```
        	 include('Frame');
        	 console.assert(Frame.current.IsFocused() == true);
         ```
         */
        
        Any IsFocused(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::GetName(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	获取当前页面的名称.
         *
         * @return	#String 页面名称.
         *
         * @example
         *
         ```
        	 include('Frame');
        	 console.assert(Frame.current.GetName() == '');
         ```
         */
        
        Any GetName(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::GetIdentifier(IPCMessage::SmartType msg);
         *
         * @tag sync
         *
         * @brief	获取当前页面的ID.
         *
         * @return	#Int identifier.
         *
         * @example
         *
         ```
        	 include('Frame');
        	 console.log(Frame.current.GetIdentifier());
         ```
         */
        
        Any GetIdentifier(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::GetParent(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	获取当前页面的父页面.
         *
         * @return	#Frame 如果不存在返回Undefined.
         *
         * @example
         *
         ```
        	include('Frame');
        	console.log(Frame.current.GetParent());
         ```
         */
        
        Any GetParent(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::GetURL(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	获取当前页面的URL.
         *
         * @return	#String url.
         *
         * @example
         *
         ```
        	 include('Frame');
        	 console.log(Frame.current.GetURL());
         ```
         */
        
        Any GetURL(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any FrameTransfer::GetBrowser(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	获取当前页面所属的Browser对象.
         *
         * @return	#Browser .
         * @example
         *
         ```
        	 include('Frame');
        	 console.log(Frame.current.GetBrowser().GetIdentifier());
         ```
         */
        
        Any GetBrowser(IPCMessage::SmartType msg);
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(FrameTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(current,
                                      TransferFuncConstProperty | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(injectScript, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(injectCSS, TransferExecNormal)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(IsValid, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(Undo, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(Redo, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(Cut, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(Copy, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(Paste, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(Delete, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(SelectAll, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(ViewSource, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetSource, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetText, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(LoadRequest, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(LoadURL, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(LoadString, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(IsMain, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(IsFocused, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetName, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetIdentifier, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetParent, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetURL, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetBrowser, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    private:
        CefRefPtr<CefFrame> m_pFrame;
    };
}

#endif // AMO_FRAMETRANSFER_H__
