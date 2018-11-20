// Created by amoylel on 10/06/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_SPLASHTRANSFER_H__
#define AMO_SPLASHTRANSFER_H__

#include <amo/singleton.hpp>

#include "transfer/ClassTransfer.hpp"

namespace amo {
    class SplashWindow;
    class SplashWindowSettings;
    
    /*!
     * @class	splash
     *
     * @extend Object
     *
     * @brief	启动画面.<br>
     * 			工作线程:**UI线程**.
     */
    
    class SplashTransfer
        : public ClassTransfer
        , public amo::singleton<SplashTransfer> {
    public:
        SplashTransfer();
        ~SplashTransfer();
        
        void closeSplash(int nDelay = 0);
        
        void fadeout();
        /*!
         * @fn	Any SplashTransfer::show(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	显示Splash.
         *
         * @param	#JsonObject 可支持的参数见{@link 启动画面=启动画面}.
         *
         * @return	无.
         *
         * @see hide=splash.hide
         * @example 简单示例
         *
         ```
        	 // 窗口会居中显示
        	 include('splash');
        	 splash.show({
        		image: 'images/splash.jpg',
        		duration: 5000
        	 });
         ```
         * @example 指定位置
         *
         ```
        	 include('splash');
        	 splash.show({
        		 image: 'images/splash.jpg',
        		 duration: 5000,
        		 x:100,
        		 y:100,
        		 width:1280,
        		 height:720,
        		 adjustPos:true,
        		 center:false
        	});
         ```
         * @example 指定大小
         *
         ```
        	 include('splash');
        	 splash.show({
        		 image: 'images/splash.jpg',
        		 duration: 5000,
        		 x:100,
        		 y:100,
        		 width:640,
        		 height:360,
        		 adjustPos:true,
        		 center:false
        	 });
         ```
         * @example 覆盖父窗口
         *
         ```
        	 includes('splash', 'BrowserWindow');
        	 var win = BrowserWindow.current;
        	 // 获取当前窗体在屏幕中的位置
        	 var winInfo = win.getPosition();
        	 winInfo.image = 'images/splash.jpg';
        	 winInfo.center = false;
        	 winInfo.adjustPos = true;
        	 winInfo.duration = 5000;
        	 splash.show(winInfo);
         ```
         */
        
        Any show(IPCMessage::SmartType msg);
        
        
        /*!
        * @fn	Any SplashTransfer::hide(IPCMessage::SmartType msg);
        *
        * @tag static
        *
        * @brief	n毫秒后隐藏Splash，n由{@link show=splash.show}中设置的fadeout决定.
        *
        * @return	无.
        *
        * @example
        
        		```
        				include('splash');
        				splash.hide();
        		```
        */
        
        /*!
         * @fn	Any SplashTransfer::hide(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	n毫秒后隐藏Splash.
         *
         * @param	#Int 指定延时时间n (毫秒).
         *
         * @return	无.
         * @example
        
        		```
        				include('splash');
        				splash.hide(3000);
        		```
         */
        
        Any hide(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any SplashTransfer::isVisible(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	判断Splash是否可见.
         *
         * @return	#Boolean.
         * @example
        
        		```
        				include('splash');
        				console.assert(splash.isVisible() == false);
        		```
         */
        
        Any isVisible(IPCMessage::SmartType msg);
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(SplashTransfer, ClassTransfer)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(show, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(hide, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isVisible, TransferFuncStatic | TransferExecSync)
        
        AMO_CEF_MESSAGE_TRANSFER_END()
        
        
        void create(std::shared_ptr<SplashWindowSettings> pSettings);
    protected:
        /*! @brief	The splash window. */
        SplashWindow* m_pSplashWindow;
        
        /*! @brief	Splash关闭定时器. */
        UINT_PTR m_splashTimer;
        
        /*! @brief	关闭窗口的动画时间. */
        int m_nDelay;
        
        /*! @brief	当前淡化窗口剩余次数. */
        int m_nfadeTimes;
    };
}

#endif // AMO_SPLASHTRANSFER_H__

