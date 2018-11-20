// Created by amoylel on 07/07/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_APPTRANSFER_H__
#define AMO_APPTRANSFER_H__


#include <amo/singleton.hpp>

#include "transfer/ClassTransfer.hpp"


namespace amo {

    /*!
     * @class	app
     *
     * @extend  Object
     *
     * @brief	控制程序程序的生命周期，并提供一些与程序相关的功能.<br>
     * 			工作线程:**UI线程**.
     */
    
    class AppTransfer
        : public ClassTransfer
        , public amo::singleton<AppTransfer> {
        
    public:
        AppTransfer();
        
        /*!
         * @ignore
         *
         * @fn	void AppTransfer::initUrlMapping(amo::u8json& json);
         *
         * @brief	将配置文件中的设置添加到MAP中.
         *
         * @param [in,out]	json	The JSON.
         */
        
        void initUrlMapping(amo::u8json& json);
        
        /*!
         * @fn	static Any AppTransfer::getConfig(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	获取应用程序的所有配置参数,可获取的内容见{@link 启动参数=启动参数}.
         *
         * @return	#JsonObject 所有配置参数.
         */
        
        
        /*!
        * @fn	static Any AppTransfer::getConfig(IPCMessage::SmartType msg);
        * @tag static sync
        * @brief	获取应用程序的指定配置参数.
        *
        * @param	#String 需要获取的字段名称.
        *
        * @return	返回字段名所对应的配置参数。返回类型视配置参数类型而定，为JS所支持的基本数据类型.
        *
        * @see setConfig=app.setConfig
        * @example
        			```
        				//获取应用程序的所有配置参数
        				var config = app.getConfig();
        				console.log(config);
        
        				//获取应用程序的指定配置参数
        				var single_process = app.getConfig('single_process');
        				console.log('单进程模式:' + single_process);
        			```
        */
        
        
        Any getConfig(IPCMessage::SmartType msg);
        
        /*!
         * @fn	static Any AppTransfer::setConfig(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	设置应用程序配置参数.该函数并不要求一定要输入所支持的参数，
         * 			你也可以设置自定义参数，并在getConfig里面获取所设置的值与{@link app.getConfig}相对应.
         *
         * @param	#JsonObject 需要设置的参数，该值为一个JSON对象.
         *
         * @return	无.
         *
         * @see getConfig=app.getConfig
         *
         * @example
        			```
        			app.setConfig({
        				debugMode: true,
        				custom: 'custom'
        			});
        
        			```
         */
        
        Any setConfig(IPCMessage::SmartType msg);
        
        /*!
         * @fn	static Any AppTransfer::toAbsolutePath(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	将一个路径转为绝对路径如:%web%.
         *
         * @param	#String 需要转换的路径.
         *
         * @return	#String 转换后的绝对路径.
         *
         * @example
         			```
        				// 获取web资源文件目录
        				var webDir = app.toAbsolutePath('%webDir%');
        				console.log(webDir);
        				console.log(app.toAbsolutePath('%webDir%docs/index.html'));
        
        				// 获取图片文件所有目录
        				var picturesDir = app.toAbsolutePath('%picturesDir%');
        				console.log(picturesDir);
        
        			```
         */
        
        Any toAbsolutePath(IPCMessage::SmartType msg);
        
        /*!
         * @fn	static Any AppTransfer::quit(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	尝试退出程序，可以用户被取消（如window.unload函数）.
         *
         *
         * @return	无.
         * @example
          		   ```
        		   //@skip
         		   app.quit();
          		   ```
         */
        
        Any quit(IPCMessage::SmartType msg);
        
        /*!
         * @fn	static Any AppTransfer::exit(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	强制退出程序.
         *
         * @return	无.
         *
         * @example
        		   ```
        		   //@skip
        		   // 直接退出程序，不会有任何提示
        		   app.exit();
        		   ```
         */
        
        Any exit(IPCMessage::SmartType msg);
        
        
        /*!
        * @fn	static Any AppTransfer::addUrlMapping(IPCMessage::SmartType msg);
        *
        * @tag static sync
        *
        * @brief   添加URL映射路径,简单来说就是将URL替换为被映射的URL或文件路径.
        *
        * @param   #String 需要添加映射的URL.
        *
        * @param   #String 被映射的本地文件路径，该路径可以是一个普通的文件或文件夹，
        * 					也可以是ZIP压缩包或DLL动态库.
        *
        * @return	#Boolean 成为返回true, 失败返回false.
        * @example 映射到另一个URL
        			```
        				// 增加映射
        				app.addUrlMapping('http://www.baidu.com', 'http://www.qq.com');
        				window.open('http://www.baidu.com');
        
        				app.addUrlMapping('http://www.baidu.com/more', 'http://www.qq.com');
        				window.open('http://www.baidu.com/more');
        
        			```
        * @example 映射到文件
        			```
        
        			// 你也可以指定一个绝对路径
        			//	app.addUrlMapping('http://www.baidu.com/', 'E:/webDir/index.hmtl');
        			//	app.addUrlMapping('http://www.baidu.com/more', 'E:/webDir/more/more.html');
        
        			app.addUrlMapping('http://www.baidu.com', '%webDir%example/BrowserWindow.html');
        			window.open('http://www.baidu.com');
        
        			app.addUrlMapping('http://www.baidu.com/more', '%webDir%/example/FramelessWindow.html');
        			window.open('http://www.baidu.com/more');
        
        ```
        * @example 映射到ZIP文件
        ```
        		// zip文件同样可以是一个绝对路径
        		// app.addUrlMapping('http://www.baidu.com', 'zip:///D:/web.zip');
        
        		app.addUrlMapping('http://127.0.0.1:8088', 'zip:///%webDir%web.zip');
        		window.open('http://127.0.0.1:8088/example/BrowserWindow.html');
        
        		app.addUrlMapping('http://127.0.0.1:8088/test',  'zip:///%webDir%web.zip/example/FramelessWindow.html');
        		window.open('http://127.0.0.1:8088/test');
        
        ```
        * @example 映射到DLL文件
        ```
        
        		// demo中的dll所提供的根目录为当前程序所在目录下的web目录
        		app.addUrlMapping('http://www.baidu.com', 'dll:///libResource.dll');
        		window.open('http://www.baidu.com/example/BrowserWindow.html');
        
        		app.addUrlMapping('http://www.baidu.com/more', 'dll:///libResource.dll/example');
        		window.open('http://www.baidu.com/more/BrowserWindow.html');
        ```
        * @example 映射到RES文件
        ```
        
        	// RES本质上与ZIP是一下，只是将ZIP文件提前写入EXE的资源文件里.
        	// 你可以参考程序生成里面的内容，将ZIP文件写到EXE里面去
        	// 当前文档程序没有ID为131的资源，所以下面的方法不能正确的打开页面
        	app.addUrlMapping('http://www.baidu.com', 'res:///131.res');
        	window.open('http://www.baidu.com/example/BrowserWindow.html');
        
        ```
        */
        
        Any addUrlMapping(IPCMessage::SmartType msg);	// UI
        
        /*!
        * @fn	static Any AppTransfer::removeUrlMapping(IPCMessage::SmartType msg);
        *
        * @tag static sync
        *
        * @brief	移除URL映射路径.
        *
        * @param	#String 需要移除的URL.
        *
        * @return	无.
        * @example
        			```
        				// 删除映射
        				app.removeUrlMapping('http://www.baidu.com');
        				window.open('http://www.baidu.com');
        			```
        */
        
        Any removeUrlMapping(IPCMessage::SmartType msg); //UI
        
        /*!
         * @fn	static Any AppTransfer::urlToNativePath(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	将一个URL转换为本地路径，
         * 			这几个函数可能会被IO线程和UI线程同时调用，所以要加锁.
         *
         * @param	#String 需要转换的URL.
         * @param	#Boolean=false 是否要求所映射的文件存在 .
         *
         * @return	#String 转换后的路径，如果当前所给的URL没有与之映射的本地路径，
         * 			将返回该URL.
         * @example
         *
        	```
        		app.addUrlMapping('http://test/test.html', '%webDir%/index.html');
        		console.log(app.urlToNativePath('http://test/test.html', true));
        		console.assert(app.urlToNativePath('http://test/test.html', true) != '');
        		console.assert(app.urlToNativePath('http://test/test2.html', true) == '');
        		console.assert(app.urlToNativePath('http://test/test2.html', false)  == '');
        		console.assert(app.urlToNativePath('http://www.github.com', true)  == '');
        		console.assert(app.urlToNativePath('http://www.github.com', false)  == '');
        
        	```
         */
        
        Any urlToNativePath(IPCMessage::SmartType msg); //IO
        
        
        
        /*!
         * @ignore
         *
         * @fn	static Any AppTransfer::setDragClassName(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	设置可拖动窗口控件Class名.
         *
         * @param	msg	The message.
         *
         * @return	Any.
         */
        
        Any setDragClassName(IPCMessage::SmartType msg);
        
        /*!
         * @ignore
         *
         * @fn	static Any AppTransfer::setNoDragClassName(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	设置不可手动窗口控件Class名.
         *
         * @param	msg	The message.
         *
         * @return	Any.
         */
        
        Any setNoDragClassName(IPCMessage::SmartType msg);
        
        /*!
         * @ignore
         *
         * @fn	static Any AppTransfer::setGlobal(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	设置全局数据.
         *
         * @param	msg	The message.
         *
         * @return	Any.
         */
        
        Any setGlobal(IPCMessage::SmartType msg);
        
        /*!
         * @ignore
         *
         * @fn	static Any AppTransfer::getGlobal(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	获取全局数据.
         *
         * @param	msg	The message.
         *
         * @return	The global.
         */
        
        Any getGlobal(IPCMessage::SmartType msg);
        
        /*!
         * @fn	static Any AppTransfer::getAutoRun(IPCMessage::SmartType msg);
         *
         * @tag static sync deprecated
         *
         * @brief	获取是否开机启动，使用{@link isAutoRun=app.isAutoRun}代替.
         *
         * @return	#Boolean .
         *
         * @see setAutoRun=app.setAutoRun
         */
        
        Any getAutoRun(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any AppTransfer::isAutoRun(IPCMessage::SmartType msg);
           *
         * @tag static sync
         *
         * @brief	获取是否开机启动.
         *
         * @return	#Boolean .
         *
         * @see setAutoRun=app.setAutoRun
         * @example
         *
         ```
        	include('app');
        	app.setAutoRun(false);
        	console.assert(app.isAutoRun() == false);
         ```
         */
        
        Any isAutoRun(IPCMessage::SmartType msg);
        
        
        /*!
         * @fn	static Any AppTransfer::setAutoRun(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	设置是否开机启动.
         *
         * @param	#Boolean=false true开机启动，fase取消开机启动.
         *
         * @return	无.
         *
         * @see isAutoRun=app.isAutoRun
         *
         * @example
        			```
        			// 设置开机启动
        			app.setAutoRun(true);
        			console.assert(app.isAutoRun() == true);
        
        			// 取消开机启动
        			app.setAutoRun(false);
        			console.assert(app.isAutoRun() == false);
        			```
         */
        
        Any setAutoRun(IPCMessage::SmartType msg);
        
        /**
         * @fn	static Any AppTransfer::elapsed(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	当前程序从启动到现在的运行时间(毫秒).
         *
         * @return	#Int.
         * @example
        			```
        			var elapsed = app.elapsed();
        			console.log('程序已运行:' + elapsed + ' ms');
        			```
         */
        
        Any elapsed(IPCMessage::SmartType msg);
        
        /*!
         * @fn	static Any AppTransfer::restart(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	重启当前程序.
         *
         * @param	#Int=0 重启延时(秒)，当前程序会立即关闭，新的程序会在延时时间到达后启动.
         *
         * @return	#Boolean 生成返回true, 失败返回false.
         *
         * @example
        			```
        			//@skip  3秒后重启运行当前程序
        			app.restart(3);
        			```
         */
        
        Any restart(IPCMessage::SmartType msg);
        
        
        /**
         * @fn	static Any AppTransfer::destroy(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	销毁当前程序，(从磁盘上删除当前可执行程序).
         *
         *
         * @return	无.
         * @example
        		```
        			//@skip 不要试！！！
        			app.destroy();
        		```
         */
        
        Any destroy(IPCMessage::SmartType msg);
        
        /**
        * @fn	static Any AppTransfer::setZipPassword(IPCMessage::SmartType msg);
        *
        * @tag static sync
        *
        * @brief	设置ZIP文件密码，用于打开加密的ZIP文件.
        * @param	#String= 需要设置密码的ZIP文件路径.
        * @param	#String='' 密码.
        * @return	无.
        * @example
        ```
        	//
        	app.setZipPassword('%webDir%web1.zip', '123456');
        ```
        */
        
        Any setZipPassword(IPCMessage::SmartType msg);
        
        /**
        * @fn	static Any AppTransfer::setResPassword(IPCMessage::SmartType msg);
        *
        * @tag static sync
        *
        * @brief	RES密码.
        * @param	#String= 需要设置密码的RES文件ID.
        * @param	#String='' 密码.
        *
        * @return	无.
        * @example
        	```
        		app.setResPassword();
        	```
        */
        
        Any setResPassword(IPCMessage::SmartType msg);
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(AppTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getConfig, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setConfig, TransferFuncStatic)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setGlobal, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getGlobal, TransferFuncStatic | TransferExecSync)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setAutoRun, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getAutoRun, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isAutoRun, TransferFuncStatic | TransferExecSync)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(toAbsolutePath,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(urlToNativePath,
                                      TransferFuncStatic | TransferExecSync)
                                      
        AMO_CEF_MESSAGE_TRANSFER_FUNC(addUrlMapping,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(removeUrlMapping,
                                      TransferFuncStatic | TransferExecSync)
                                      
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setDragClassName,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setNoDragClassName,
                                      TransferFuncStatic | TransferExecSync)
                                      
        AMO_CEF_MESSAGE_TRANSFER_FUNC(quit, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(exit, TransferFuncStatic)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(restart, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(destroy, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setZipPassword,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setResPassword,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(elapsed, TransferFuncStatic | TransferExecSync)
        
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    private:
        /*! @brief	URL转本地路径锁. */
        std::recursive_mutex m_mutex;
        /*! @brief	URL到本地路径映射. */
        std::list<std::pair<std::string, std::string > > m_oUrlToNativeMap;
        /*! @brief	全局数据. */
        amo::u8json m_global;
    };
}



#endif // AMO_APPTRANSFER_H__