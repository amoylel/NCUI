// Created by amoylel on 06/19/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_RCEDITTRANSFER_H__
#define AMO_RCEDITTRANSFER_H__

#include "transfer/ClassTransfer.hpp"


#include <memory>
#include <amo/json.hpp>
#include <vector>


namespace amo {

    class ResourceUpdater;
    
    /*!
     * @class	rcedit
     *
     * @chapter extend
     *
     * @brief	资源编辑器，用来生成exe程序.
     */
    
    class RceditTransfer
        : public ClassTransfer
        , public amo::singleton<RceditTransfer> {
    public:
        RceditTransfer();
        
        ~RceditTransfer();
        
        
        /*!
         * @fn	amo::Any RceditTransfer::loadDiskSettings(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	从磁盘中加载配置文件，配置文件包含四个数据段，其中三个段内容与manifest.json相同，只增加了一个文件段描述EXE文件的基本信息，段名"fileSettings",可用值如下:<br>
         *			&nbsp; **CompanyName**: 公司名称，默认值"NCUI"<br>
         *			&nbsp; **FileDescription**: 文件描述，默认值"NCUI演示程序"<br>
         *			&nbsp; **FileVersion**:文件版本，默认值"1.0.0.0"<br>
         *			&nbsp; **InternalName**:内部名称，默认值"NCUIDemo.exe"<br>
         *			&nbsp; **LegalCopyright**:法律著作权，默认值"Copyright (C) 2017"<br>
         *			&nbsp; **OriginalFileName**:原始文件名称，默认值"NCUIDemo.exe"<br>
         *			&nbsp; **ProductName**:产品名称，默认值"NCUI演示程序"<br>
         *			&nbsp; **ProductVersion**:产品版本，默认值"1.0.0.0"<br>
         *			&nbsp; **Icon**:图标路径，默认使用当前程序的图标<br>
         *
         * @param	#String 配置文件路径，文件内容必须为JSON格式.
         *
         * @return	#Boolean true 成功/ false 失败.
         */
        
        amo::Any loadDiskSettings(IPCMessage::SmartType msg);
        
        /*!
         * @fn	amo::Any RceditTransfer::getDefaultFileSettings(IPCMessage::SmartType msg);
         * @tag static sync
         * @brief	获取默认的文件参数.
         *
         *
         * @return	#JsonObject.
         */
        
        amo::Any getDefaultFileSettings(IPCMessage::SmartType msg);
        
        /*!
         * @fn	amo::Any RceditTransfer::getDefaultAppSettings(IPCMessage::SmartType msg);
         * @tag static sync
         * @brief	获取默认的APP启动参数.
         *
         * @return	#JsonObject.
         */
        
        amo::Any getDefaultAppSettings(IPCMessage::SmartType msg);
        
        /*!
         * @fn	amo::Any RceditTransfer::getDefaultBrowserSettings(IPCMessage::SmartType msg);
         * @tag static sync
         * @brief	获取默认的浏览器窗口启动参数.
         *
         * @return	#JsonObject.
         */
        
        amo::Any getDefaultBrowserSettings(IPCMessage::SmartType msg);
        
        /*!
         * @fn	amo::Any RceditTransfer::getDefaultSplashSettings(IPCMessage::SmartType msg);
         * @tag static sync
         * @brief	获取默认的启动画面.
         *
         * @return	#JsonObject.
         */
        
        amo::Any getDefaultSplashSettings(IPCMessage::SmartType msg);
        
        /**
         * @fn	amo::Any RceditTransfer::getDefaultSettings(IPCMessage::SmartType msg);
         * @tag static sync
         * @brief	获取默认的程序配置.
         *
         * @return	#JsonObject.
         */
        
        amo::Any getDefaultSettings(IPCMessage::SmartType msg);
        
        /*!
         * @fn	amo::Any RceditTransfer::commit(IPCMessage::SmartType msg);
         * @tag static sync
         * @brief	生成EXE文件，这个函数将把配置好的参数写入EXE文件中.
         * 			注意:**输入的参数都是JSON格式的字符串，不是JSON对象**
         *
         * @param	#String EXE文件信息，JSON字符串，可用见{@link loadDiskSettings=rcedit.loadDiskSettings}.
         * @param	#String 启动参数,JSON字符串，可用值见{@link 启动参数}.
         * @param	#String 浏览器窗口参数,JSON字符串，可用值见{@link 浏览器窗口参数}.
         * @param	#String 启动画面参数,JSON字符串，可用值见{@link 启动画面}.
         *
         * @return	#Boolean true/false.
         * @example
         *
         ```
        	include('rcedit');
        	var fileSettings = rcedit.getDefaultFileSettings();
        	var appSettings = rcedit.getDefaultAppSettings();
        	var browserWindowSettings = rcedit.getDefaultBrowserSettings();
        	var splashWindowSettings = rcedit.getDefaultSplashSettings();
        	console.log(fileSettings);
        	console.log(appSettings);
        	console.log(browserWindowSettings);
        	console.log(splashWindowSettings);
        	rcedit.commit(fileSettings, appSettings, browserWindowSettings, splashWindowSettings);
        
         ```
         */
        
        /*!
        * @fn	amo::Any RceditTransfer::commit(IPCMessage::SmartType msg);
        * @tag static sync
        * @brief	生成EXE文件，这个函数将把配置好的参数写入EXE文件中.
        *
        * @return	#Boolean .
        * @example
        *
        ```
        	include('rcedit');
        	rcedit.commit();
        ```
        */
        
        amo::Any commit(IPCMessage::SmartType msg);
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(RceditTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(loadDiskSettings,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getDefaultFileSettings,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getDefaultAppSettings,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getDefaultBrowserSettings,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getDefaultSplashSettings,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getDefaultSettings,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(commit, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    protected:
        amo::u8json m_oConfig;
        std::shared_ptr<ResourceUpdater> m_pUpdater;
        amo::u8json m_oSettings;
    };
    
}


#endif // AMO_RCEDITTRANSFER_H__


