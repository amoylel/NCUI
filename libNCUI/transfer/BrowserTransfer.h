// Created by amoylel on 06/14/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_BROWSERTRANSFER_H__
#define AMO_BROWSERTRANSFER_H__


#include <amo/singleton.hpp>

#include "handler/CefHeader.hpp"
#include "transfer/ClassTransfer.hpp"


namespace amo {

    /*!
     * @class	Browser
     *
     * @chapter cef
     *
     * @brief	CEF提供的浏览器类封装.<br>
     * 			工作线程:**UI线程**.
     */
    
    class BrowserTransfer
        : public ClassTransfer
        , public amo::singleton<BrowserTransfer> {
    public:
        BrowserTransfer();
        BrowserTransfer(CefRefPtr<CefBrowser> pBrowser);
        
        /*!
         * @fn	Any BrowserTransfer::GetHost(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	获取浏览器的Host对象.
         *
         * @return	#BrowserHost.
         * @example
         *
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 var host = browser.GetHost();
        	 var browser2 = host.GetBrowser();
        	 console.assert(browser.IsSame(browser2) == true);
         ```
         */
        
        Any GetHost(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::CanGoBack(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	判断当前浏览器是否可以后退.
         *
         *
         * @return	#Boolean true可以后退/false不能回退.
         * @example
         *
         ```
        	includes('BrowserWindow', 'Browser');
        	var browser = BrowserWindow.current.getBrowser();
        	console.log(browser.CanGoBack());
         ```
         */
        
        Any CanGoBack(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::GoBack(IPCMessage::SmartType msg);
         *
         * @brief	后退. BrowserWindowSettings.back_forword如果为false，或者{@link CanGoBack=Browser.CanGoBack}==false该操作不会成功.
         *
         * @return	无.
         *
         * @see GetHost=Browser.GetHost
         *
         * @example
         *
         ```
        	includes('BrowserWindow', 'Browser');
        	var browser = BrowserWindow.current.getBrowser();
        	browser.GoBack();
         ```
         */
        
        Any GoBack(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::CanGoForward(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	判断当前浏览器是否可以前进.
         *
         *
         * @return	#Boolean true可以前进false不能前进.
         * @example
         *
         ```
        	includes('BrowserWindow', 'Browser');
        	var browser = BrowserWindow.current.getBrowser();
        	console.log(browser.CanGoForward());
         ```
         */
        
        Any CanGoForward(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::GoForward(IPCMessage::SmartType msg);
         *
         * @brief	前进. BrowserWindowSettings.back_forword如果为false，或者{@link CanGoForward=Browser.CanGoForward}==false该操作不会成功.
         *
         * @return	无.
         * @see CanGoForward=Browser.CanGoForward
         *
         * @example
         *
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 browser.GoForward();
         ```
         */
        
        Any GoForward(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::IsLoading(IPCMessage::SmartType msg);
         *
         * @brief	是否正在加载页面.
         *
         * @return	#Boolean true是/false否.
         * @example
         *
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 console.log(browser.IsLoading());
        	 console.assert(browser.IsLoading() != true);
         ```
         */
        
        Any IsLoading(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::Reload(IPCMessage::SmartType msg);
         *
         * @brief	重新加载页面.
         *
         * @return	无.
         *
         * @example
         *
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 browser.Reload();
         ```
         */
        
        Any Reload(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::ReloadIgnoreCache(IPCMessage::SmartType msg);
         *
         * @brief	重新加载页面，无视任何缓存数据.
         *
         * @return	无.
         *
         * @example
         *
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 browser.ReloadIgnoreCache();
         ```
         */
        
        Any ReloadIgnoreCache(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::StopLoad(IPCMessage::SmartType msg);
         *
         * @brief	停止加载页面.
         *
         * @return	无.
         *
         * @example
         *
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 browser.StopLoad();
         ```
         */
        
        Any StopLoad(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::GetIdentifier(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	获取浏览器ID.
         *
         * @return	#Int 浏览器ID.
         *
         * @example
         *
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 console.log(browser.GetIdentifier());
         ```
         */
        
        Any GetIdentifier(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::IsSame(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	与另一个Borwser对象比较是否相等.
         *
         * @param	#Object Browser 对象.
         *
         * @return	#Boolean true相同/false失败.
         *
         * @example
         *
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 var win = new BrowserWindow({url:'http://www.baidu.com'});
        	 var browser2 = win.getBrowser();
        	 console.assert(browser.IsSame(browser) == false);
        	 setTimeout(function(){
        		win.close();
        	 }, 3000);
         ```
         */
        
        Any IsSame(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::IsPopup(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	是否为一个弹出窗口.
         *
         * @return	#Boolean true弹出窗口/false非弹出窗口.
         *
         * @example
         *
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 console.log(browser.IsPopup());
         ```
         */
        
        Any IsPopup(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::HasDocument(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	是否在浏览器中加载了文档.
         *
         *
         * @return	#Boolean true有document/false 无document.
         *
         * @example
         *
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 console.log(browser.HasDocument());
         ```
         */
        
        Any HasDocument(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::GetMainFrame(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	返回浏览器窗口的主（顶层）框架Frame.
         *
         *
         * @return	#Frame Frame 对象.
         *
         * @example
         *
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 var frame = browser.GetMainFrame();
        	 console.log(frame.GetIdentifier());
         ```
         */
        
        Any GetMainFrame(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::GetFocusedFrame(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	返回浏览器窗口的焦点框架Frame.
         *
         *
         * @return	#Frame Frame 对象.
         *
         * @example
         *
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 var frame = browser.GetFocusedFrame();
        	 console.log(frame.GetIdentifier());
         ```
         */
        
        Any GetFocusedFrame(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::GetFrame(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	通过名称或ID获取Frame.
         *
         * @param	#Int/String Frame ID 或Frame 名称.
         *
         * @return	#Frame Frame 对象.
         * @example
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 var frame = browser.GetFocusedFrame();
        	 var id = frame.GetIdentifier();
        	 var frame2 = browser.GetFrame(id);
        	 console.assert(frame.GetIdentifier() == frame2.GetIdentifier());
         ```
         */
        
        Any GetFrame(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::GetFrameCount(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	获取Frame个数.
         *
         * @return	#Int Frame个数.
         *
         * @example
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 var count = browser.GetFrameCount();
        	 console.log(count);
        	 console.assert(count == 1);
         ```
         */
        
        Any GetFrameCount(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::GetFrameIdentifiers(IPCMessage::SmartType msg);
         * @tag sync
         * @brief	获取所有Frame的ID，返回一个数组[].
         *
         * @return	#Array .
         *
         * @example
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 var arr = browser.GetFrameIdentifiers();
        	 console.log(arr);
        	 console.assert(arr.length == 1);
         ```
         */
        
        Any GetFrameIdentifiers(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::GetFrameNames(IPCMessage::SmartType msg);
         *
         * @brief	获取所有Frame的名称，返回一个数组[].
         *
         *
         * @return	#Array.
         *
         * @example
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 var arr = browser.GetFrameNames();
        	 console.log(arr);
        	 console.assert(arr.length == 1);
         ```
         */
        
        Any GetFrameNames(IPCMessage::SmartType msg);
        
        Any SendProcessMessage(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any BrowserTransfer::currentFrame(IPCMessage::SmartType msg);
         *
         * @brief	获取当前Frame.
         *
         *
         * @return	#Frame Frame 对象.
         *
         * @example
         ```
        	 includes('BrowserWindow', 'Browser');
        	 var browser = BrowserWindow.current.getBrowser();
        	 var frame = browser.currentFrame();
        	 console.log(frame.GetIdentifier());
         ```
         */
        
        Any currentFrame(IPCMessage::SmartType msg);
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(BrowserTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetHost, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(CanGoBack, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GoBack, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(CanGoForward, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GoForward, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(IsLoading, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(Reload, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(ReloadIgnoreCache, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(StopLoad, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetIdentifier, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(IsSame, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(IsPopup, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(HasDocument, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetMainFrame, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetFocusedFrame, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetFrame, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetFrameCount, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetFrameIdentifiers, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(GetFrameNames, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(SendProcessMessage, TransferExecSync)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(currentFrame, TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_END()
    private:
        CefRefPtr<CefBrowser> m_pBrowser;
    };
}

#endif // AMO_BROWSERTRANSFER_H__