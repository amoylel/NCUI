// Created by amoylel on 06/26/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_TRAYTRANSFER_H__
#define AMO_TRAYTRANSFER_H__


#include <amo/singleton.hpp>
#include "transfer/ClassTransfer.hpp"


namespace amo {
    class Tray;
    
    /**
     * @class	tray
     *
     * @extend Object
     *
     * @brief	托盘操作类.<br>
     * 			工作线程:**UI线程**.
     *
     */
    
    class TrayTransfer
        : public ClassTransfer
        , public amo::singleton<TrayTransfer> {
    public:
    
        TrayTransfer();
        std::shared_ptr<amo::Tray> getTray()  ;
        void setTray(std::shared_ptr<amo::Tray> val);
        
        void onTrayEvent(const std::string& event);
        /*!
         * @fn	Any TrayTransfer::show(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	显示托盘.
         *
         * @return	无.
         *
         * @example
         *
        		 ```
        			include('tray');
        			tray.show();
        		 ```
         */
        Any show(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any TrayTransfer::hide(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	隐藏托盘.
         *
         * @return	无.
         * @example
         *
        			 ```
        				include('tray');
        				tray.hide();
        			 ```
         */
        Any hide(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any TrayTransfer::setIcon(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	设置托盘图标.
         *
         * @param	#String 图标本地路径.
         *
         * @return	无.
         *
         * @example
         *
        		 ```
        			include('tray');
        			tray.setIcon('skin/logo1.ico');
        		 ```
         */
        Any setIcon(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any TrayTransfer::setTooltip(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	设置托盘提示文本.
         *
         * @param	#String 需要显示的提示文本.
         *
         * @return	无.
         *
         * @example
         *
        	 ```
        		include('tray');
        		tray.setTooltip(new Date().toString());
        	 ```
         */
        Any setTooltip(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any TrayTransfer::blink(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	闪烁托盘.
         *
         * @param	#Boolean=false true闪烁/false停止闪烁.
         *
         * @return	无.
         *
         * @see isBlink=tray.isBlink
         * @example
         *
        	 ```
        		include('tray');
        		tray.blink(true);
        	 ```
         */
        Any blink(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any TrayTransfer::isBlink(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	判断托盘是否闪烁.
         *
         * @return	#Boolean.
         *
         * @see blink=tray.blink
         *
         * @example
         *
        		 ```
        			include('tray');
        			tray.blink(true);
        			console.assert(tray.isBlink() == true);
        			tray.blink(false);
        			console.assert(tray.isBlink() == false);
        		 ```
         */
        Any isBlink(IPCMessage::SmartType msg);
        
        
        /*!
        * @event	Any TrayTransfer::buttondown(IPCMessage::SmartType msg);
        *
        * @tag static
        *
        * @brief	鼠标左键按下时触发.
        *
        * @example
        *
        	```
        	include('tray');
        	tray.on('buttondown', function(){
        		console.log('buttondown');
        	});
        	```
        */
        
        /*!
        * @event	Any TrayTransfer::buttonup(IPCMessage::SmartType msg);
        *
        * @tag static
        *
        * @brief	鼠标左键弹起时触发.
        *
        * @example
        *
        	```
        		include('tray');
        		tray.on('buttonup', function(){
        			console.log('buttonup');
        		});
        	```
        */
        
        /*!
        * @event	Any TrayTransfer::rbuttondown(IPCMessage::SmartType msg);
        *
        * @tag static
        *
        * @brief	鼠标右键按下时触发.
        *
        * @example
        *
        	```
        	include('tray');
        	tray.on('rbuttondown', function(){
        		console.log('rbuttondown');
        	});
        	```
        */
        
        /*!
        * @event	Any TrayTransfer::rbuttonup(IPCMessage::SmartType msg);
        *
        * @tag static
        *
        * @brief	鼠标右键弹起时触发.
        *
        * @example
        *
        		```
        		include('tray');
        		tray.on('rbuttonup', function(){
        			console.log('rbuttonup');
        		});
        		```
        */
        
        /*!
        * @event	Any TrayTransfer::click(IPCMessage::SmartType msg);
        *
        * @tag static
        *
        * @brief	鼠标左键单击时触发.
        *
        * @example
        *
        		```
        		include('tray');
        		tray.on('click', function(){
        			console.log('click');
        		});
        		```
        */
        
        /*!
        * @event	Any TrayTransfer::dbclick(IPCMessage::SmartType msg);
        *
        * @tag static
        *
        * @brief	鼠标左键双击时触发.
        *
        * @example
        *
        	```
        	include('tray');
        	tray.on('dbclick', function(){
        		console.log('dbclick');
        	});
        	```
        */
        
        /*!
        * @event	Any TrayTransfer::mousehover(IPCMessage::SmartType msg);
        *
        * @tag static
        *
        * @brief	鼠标悬停时触发.
        *
        * @example
        *
        	```
        	include('tray');
        	tray.on('mousehover', function(){
        		console.log('mousehover');
        	});
        	```
        */
        
        /*!
        * @event	Any TrayTransfer::mouseleave(IPCMessage::SmartType msg);
        *
        * @tag static
        *
        * @brief	鼠标离开时触发.
        *
        * @example
        *
        			```
        			include('tray');
        			tray.on('mouseleave', function(){
        				console.log('mouseleave');
        			});
        			```
        */
        
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(TrayTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(show, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(hide, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setIcon, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(setTooltip, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(blink, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(isBlink, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    private:
        std::shared_ptr<Tray> m_pTray;
        
    };
}


#endif // AMO_TRAYTRANSFER_H__