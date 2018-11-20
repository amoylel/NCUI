// Created by amoylel on 08/19/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_DOWNLOADERTRANSFER_H__
#define AMO_DOWNLOADERTRANSFER_H__


#include <memory>

#include <amo/singleton.hpp>

#include "handler/CefHeader.hpp"
#include "transfer/ClassTransfer.hpp"

namespace amo {

    class UIMessageEmitter;
    class DownloaderSettings;
    
    /*!
     * @class	Downloader
     * @extend  Object
     *
     * @brief	文件下载类，使用浏览器的下载功能下载文件.<br>
     * 			工作线程:**UI线程**.
     */
    
    class DownloaderTransfer
        : public ClassTransfer
        , public amo::singleton < DownloaderTransfer > {
    public:
    
        /*!
         * @enum	DLCommand
         *
         * @brief	Values that represent dl commands.
         */
        
        enum DLCommand {
            ///< An enum constant representing the dl normal option
            DL_NORMAL,
            DL_CANCEL,
            DL_RESUME,
            DL_PAUSE,
        };
    public:
    
        DownloaderTransfer();
        
        /*!
         * @fn	DownloaderTransfer::Downloader(std::shared_ptr<DownloaderSettings> pSettings);
         *
         * @tag constructor
         *
         * @brief	创建一下下载类，用于下载文件.
         *
         * @param	#JsonObject 下载参数.<br>
         * 						[**String**]	url 下载文件的url,如果所给的URL与之前所创建的下载对象url相同反返回之前的对象，下面的参数将被忽略.<br>
         * 						[**String**]	file 文件保存路径, 默认会根据url自动命名.<br>
         * 						[**Boolean**]	autoDownload 是否自动开始下载,默认为true.<br>
         * 						[**Boolean**]	forceReplace 是否强制替换已经存在的文件,默认为true.
         *
         * @return #Object 返回一个Downloader实例.
         * @example
         *
        		 ```
        		 // 需要先运行该示例才能运行后面的示例
        		 include('Downloader');
        		 window.downloader = new Downloader({
        			url: 'https://dldir1.qq.com/qqfile/qq/TIM1.2.0/21645/TIM1.2.0.exe'
        		 });
        		 // 监听下载开始事件
        		 window.downloader.on('start', function(item){
        
        			console.log('下载开始');
        			console.log(item);
        		 });
        
        		 window.downloader.on('resume', function(item){
        			 console.log('下载恢复');
        		 });
        
        		 window.downloader.on('pause', function(item){
        			 console.log('下载暂停');
        		 });
        
        		 window.downloader.on('cancel', function(item){
        			console.log('下载取消');
        		 });
        
        		 window.downloader.on('canceled', function(item){
        			 console.log('下载取消完成');
        		 });
        
        		 window.downloader.on('update', function(item){
        			console.log('下载进度:' + item.percentComplete);
        		 });
        
        		 window.downloader.on('complete', function(item){
        			 console.log('下载完毕');
        		 });
        		 ```
         *
         */
        
        DownloaderTransfer(std::shared_ptr<DownloaderSettings> pSettings);
        
        
        virtual Any onCreateClass(IPCMessage::SmartType msg) override;
        
        /*!
         * @fn	Any DownloaderTransfer::start(IPCMessage::SmartType msg);
         *
         * @brief	开始下载.
         *
         * @return	无.
         *
         * @example
         *
        			 ```
        				window.downloader.start();
        			 ```
         */
        
        Any start(IPCMessage::SmartType msg);
        
        /*!
        * @fn	Any DownloaderTransfer::pause(IPCMessage::SmartType msg);
        *
        * @brief	暂停下载.
        *
        *
        * @return	无.
        *
        * @example
        *
        		```
        			window.downloader.pause();
        		```
        */
        
        Any pause(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any DownloaderTransfer::resume(IPCMessage::SmartType msg);
         *
         * @brief	恢复当前下载.
         *
         * @return	无.
         *
         * @example
         *
         ```
        	window.downloader.resume();
         ```
         */
        
        Any resume(IPCMessage::SmartType msg);
        
        
        
        /*!
         * @fn	Any DownloaderTransfer::cancel(IPCMessage::SmartType msg);
         *
         * @brief	取消下载.
         *
         * @return	无.
         *
         * @example
         *
         ```
        	window.downloader.cancel();
         ```
         */
        
        Any cancel(IPCMessage::SmartType msg);
        
        
        /*!
        * @event	Any DownloaderTransfer::start(IPCMessage::SmartType msg);
        *
        * @brief	开始下载时触发.
        * @param	#JsonObject <br>
        * 						[**Boolean**] isValid 当前下载是否有效.<br>
        * 						[**Boolean**] isInProgress 是否正在下载（包括暂停）.<br>
        * 						[**Boolean**] isComplete 当前下载是否已经完成.<br>
        * 						[**Boolean**] isCanceled 当前下载是否已经被取消.<br>
        * 						<br>
        * 						[**Int**] currentSpeed 当前下载速度.<br>
        * 						[**Int**] percentComplete 下载完成百分比，如果返回-1，表示未获取到所下载文件的大小.<br>
        * 						[**Int**] totalBytes 总字节数.<br>
        * 						[**Int**] receivedBytes 已接收的字节数.<br>
        * 						<br>
        * 						[**Double**] startTime 下载开始的时间.<br>
        * 						[**Double**] endTime 下载结束的时间.<br>
        *						<br>
        * 						[**String**] fullPath 下载文件所保存的完整路径.<br>
        * 						[**String**] id 当前下载的唯一标识符.<br>
        * 						[**String**] url URL.<br>
        * 						[**String**] originalUrl 重定向之前的原始URL（如果被重定向的话）.<br>
        * 						[**String**] suggestedFileName 浏览器建议的文件名.<br>
        * 						[**String**] contentDisposition  .<br>
        * 						[**String**] mimeType MIME类型.<br>
        *
        *
        *
        * @return	无.
        */
        
        /*!
        * @event	Any DownloaderTransfer::resume(IPCMessage::SmartType msg);
        *
        * @brief	下载恢复时触发.
        *
        * @param #JsonObject 与start事件中的参数相同.
        *
        *
        * @return	无.
        */
        
        /*!
        * @event	Any DownloaderTransfer::pause(IPCMessage::SmartType msg);
        *
        * @brief	暂停下载时触发.
        *
        * @param #JsonObject 与start事件中的参数相同.
        *
        * @return	无.
        */
        
        /*!
        * @event	Any DownloaderTransfer::cancel(IPCMessage::SmartType msg);
        *
        * @brief	取消下载时触发.
        *
        * @param #JsonObject 与start事件中的参数相同.
        *
        * @return	无.
        */
        
        /*!
        * @event	Any DownloaderTransfer::canceled(IPCMessage::SmartType msg);
        *
        * @brief	取消下载后时触发.
        *
        * @param #JsonObject 与start事件中的参数相同.
        *
        * @return	无.
        */
        
        /*!
        * @event	Any DownloaderTransfer::update(IPCMessage::SmartType msg);
        *
        * @brief	下载数据更新时触发.
        *
        * @param #JsonObject 与start事件中的参数相同.
        *
        * @return	无.
        */
        
        
        /*!
        * @event	Any DownloaderTransfer::complete(IPCMessage::SmartType msg);
        *
        * @brief	下载完成时触发.
        *
        * @param #JsonObject 与start事件中的参数相同.
        *
        * @return	无.
        */
        
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(DownloaderTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(start, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(resume, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(pause, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(cancel, TransferExecNormal)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    public:
    
        /*!
         * @fn	void DownloaderTransfer::startDownload();
         * @ignore
         * @brief	Starts a download.
         */
        
        void startDownload();
        
        /*!
         * @fn	CefRefPtr<CefFrame> DownloaderTransfer::getFrame() const;
         * @ignore
         * @brief	Gets the frame.
         *
         * @return	The frame.
         */
        
        CefRefPtr<CefFrame> getFrame() const;
        
        /*!
         * @fn	void DownloaderTransfer::setFrame(CefRefPtr<CefFrame> val);
         * @ignore
         * @brief	Sets a frame.
         *
         * @param	val	The value.
         */
        
        void setFrame(CefRefPtr<CefFrame> val);
        
        /*!
         * @fn	CefRefPtr<CefBrowser> DownloaderTransfer::getBrowser() const;
         * @ignore
         * @brief	Gets the browser.
         *
         * @return	The browser.
         */
        
        CefRefPtr<CefBrowser> getBrowser() const;
        
        /*!
         * @fn	void DownloaderTransfer::setBrowser(CefRefPtr<CefBrowser> val);
         * @ignore
         * @brief	Sets a browser.
         *
         * @param	val	The value.
         */
        
        void setBrowser(CefRefPtr<CefBrowser> val);
        
        /*!
         * @fn	void DownloaderTransfer::removeDownloader(const std::string& url);
         * @ignore
         * @brief	Removes the downloader described by URL.
         *
         * @param	url	URL of the document.
         */
        
        void removeDownloader(const std::string& url);
        
        /*!
         * @fn	std::shared_ptr<UIMessageEmitter> DownloaderTransfer::getMessageEmitter();
         * @ignore
         * @brief	Gets message emitter.
         *
         * @return	The message emitter.
         */
        
        std::shared_ptr<UIMessageEmitter>  getMessageEmitter();
        
        /*!
         * @fn	bool DownloaderTransfer::OnBeforeDownload(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefDownloadItem> download_item,
         * 		const CefString& suggested_name,
         * 		CefRefPtr<CefBeforeDownloadCallback>& callback);
         * @ignore
         * @brief	Executes the before download action.
         *
         * @param	browser				The browser.
         * @param	download_item   	The download item.
         * @param	suggested_name  	Name of the suggested.
         * @param [in,out]	callback	The callback.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool OnBeforeDownload(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefDownloadItem> download_item,
                              const CefString& suggested_name,
                              CefRefPtr<CefBeforeDownloadCallback>& callback);
                              
        /*!
         * @fn	bool DownloaderTransfer::OnDownloadUpdated(
         * 		CefRefPtr<CefBrowser> browser,
         * 		CefRefPtr<CefDownloadItem> download_item,
         * 		CefRefPtr<CefDownloadItemCallback>& callback);
         * @ignore
         * @brief	Executes the download updated action.
         *
         * @param	browser				The browser.
         * @param	download_item   	The download item.
         * @param [in,out]	callback	The callback.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefDownloadItem> download_item,
                               CefRefPtr<CefDownloadItemCallback>& callback);
                               
        /*!
         * @fn	amo::u8json DownloaderTransfer::downloadItemToJson(CefRefPtr<CefDownloadItem>& item);
         * @ignore
         * @brief	Downloads the item to JSON described by item.
         *
         * @param [in,out]	item	The item.
         *
         * @return	An amo::u8json.
         */
        
        amo::u8json downloadItemToJson(CefRefPtr<CefDownloadItem>& item);
    private:
    
        /*! @brief	The frame. */
        CefRefPtr<CefFrame> m_pFrame;
        /*! @brief	The browser. */
        CefRefPtr<CefBrowser> m_pBrowser;
        /*! @brief	The download callback. */
        CefRefPtr<CefBeforeDownloadCallback> m_pDownloadCallback;
        /*! @brief	The download item callback. */
        CefRefPtr<CefDownloadItemCallback> m_pDownloadItemCallback;
        
        /*! @brief	The downloader settings. */
        std::shared_ptr<DownloaderSettings> m_pDownloaderSettings;
        
        /*! @brief	The downloader map. */
        std::unordered_map<std::string, std::shared_ptr<DownloaderTransfer> > m_oDownloaderMap;
        
        /*! @brief	The command. */
        DLCommand m_nCommand;
    };
}


#endif // AMO_DOWNLOADERTRANSFER_H__

