// Created by amoylel on 08/14/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_DIALOGTRANSFER_H__
#define AMO_DIALOGTRANSFER_H__


#include <amo/singleton.hpp>

#include "transfer/ClassTransfer.hpp"


namespace amo {

    /**
     * @class	dialog
     *
     * @extend Object
     *
     * @brief	弹出框，文件选择框.<br>
     * 			工作线程:**UI线程**.
     *
     */
    
    class DialogTransfer
        : public ClassTransfer
        , public amo::singleton < DialogTransfer > {
        
    public:
        DialogTransfer();
        
        /*!
         * @fn	Any DialogTransfer::runFileDialog(IPCMessage::SmartType msg);
         *
         * @tag async static
         *
         * @brief	打开文件选择对话框.
         *
         * @param	#JsonObject 文件打开参数.<br>
         * 						[**String**]  title 标题.<br>
         * 						[**String**]	model 对话框类型，可用的{@link 对话框模式=dialog.属性列表}<br>
         * 						[**String**]	defaultFilePath 默认开始目录<br>
         * 						[**Array**]	acceptFilters, 可接受的文件类型<br>
         * 						[**String**]	selectedAcceptFilter 过滤器
         *
         * @param	#Function  选择完成后的回调函数, 用于传递用户的选择结果，结果为一个Array.
         *
         * @return	无.
         *
         * @example
         *
        		 ```
        		 include('dialog');
        		 // 单文件选择
        		 dialog.runFileDialog({
        			 title: 'NCUI文件选择对话框',
        			 model: dialog.FILE_DIALOG_OPEN,
        			 defaultFilePath: '',
        			 acceptFilters: ['image/*'],
        			 selectedAcceptFilter: 0
        		 }, function(arr) {
        			 console.log(arr);
        		 });
        		 ```
         * @example
         *
        		 ```
        		 include('dialog');
        		 // 从文件选择
        		 dialog.runFileDialog({
        			 title: 'NCUI文件选择对话框',
        			 model: dialog.FILE_DIALOG_OPEN_MULTIPLE,
        			 defaultFilePath: '',
        			 acceptFilters: ['image/*'],
        			 selectedAcceptFilter: 0
        		 }, function(arr) {
        			console.log(arr);
        		 });
        		 ```
        * @example
        *
        		 ```
        		 include('dialog');
        		 // 从文件选择
        		 dialog.runFileDialog({
        		 title: 'NCUI文件选择对话框',
        		 model: dialog.FILE_DIALOG_OPEN_MULTIPLE,
        		 defaultFilePath: '',
        		 acceptFilters: ['image/*'],
        		 selectedAcceptFilter: 0
        		 }, function(arr) {
        		 console.log(arr);
        		 });
         */
        
        Any runFileDialog(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any DialogTransfer::alert(IPCMessage::SmartType msg);
         *
         * @tag sync static
         *
         * @brief	弹出提示框，相关于js的alert函数.
         *
         * @param	#String 需要显示的文本.
         *
         * @return	无.
         *
         * @example
         *
        		 ```
        		 include('dialog');
        		 dialog.alert('我是一个弹出框');
        		 ```
         */
        
        Any alert(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any DialogTransfer::confirm(IPCMessage::SmartType msg);
         *
         * @tag sync static
         *
         * @brief	选择对话框，相当于js的confirm函数.
         *
         * @param	#String 需要显示的文本.
         *
         * @return	#Boolean.
         *
         * @example
         *
        			 ```
        				include('dialog');
        				var retval = dialog.confirm('是否需要退出程序');
        				console.log(retval);
        			 ```
         */
        
        Any confirm(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any DialogTransfer::prompt(IPCMessage::SmartType msg);
         * @tag sync static
         * @brief	获取用户输入值，相当于js的prompt函数.
         *
         * @param	#String 提示文本.
         * @param	#String 默认输入文本，可选.
         *
         * @return	#JsonObject.
         * 			|#Boolean code 用户的选择结果,true /false.
         * 			|#String text 用户的输入文本.
         * @example
         *
        			 ```
        			 include('dialog');
        			 var retval = dialog.prompt('请输入你的姓名', '张三');
        			 console.log(retval);
        			 ```
         */
        
        Any prompt(IPCMessage::SmartType msg);
        
        /*! @attr #Int=FILE_DIALOG_OPEN FILE_DIALOG_OPEN 允许选择一个存在的文件. {@tag const static property}*/
        /*! @attr #Int=FILE_DIALOG_OPEN_MULTIPLE FILE_DIALOG_OPEN_MULTIPLE 可以选择多个存在的文件. {@tag const static property}*/
        /*! @attr #Int=FILE_DIALOG_OPEN_FOLDER FILE_DIALOG_OPEN_FOLDER 选择文件夹. {@tag const static property}*/
        /*! @attr #Int=FILE_DIALOG_SAVE FILE_DIALOG_SAVE 允许选择一个不存在的文件，如果所选择的文件存在，那么会提示是否覆盖该文件，一般用来做文件保存选择. {@tag const static property}*/
        /*! @attr #Int=FILE_DIALOG_TYPE_MASK FILE_DIALOG_TYPE_MASK General mask defining the bits used for the type values. {@tag const static property}*/
        /*! @attr #Int=FILE_DIALOG_OVERWRITEPROMPT_FLAG FILE_DIALOG_OVERWRITEPROMPT_FLAG Prompt to overwrite if the user selects an existing file with the Save dialog. {@tag const static property}*/
        /*! @attr #Int=FILE_DIALOG_HIDEREADONLY_FLAG FILE_DIALOG_HIDEREADONLY_FLAG 不显示只读文件. {@tag const static property}*/
        
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(DialogTransfer, ClassTransfer)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(runFileDialog,
                                      TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(alert, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(confirm, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(prompt, TransferFuncStatic | TransferExecSync)
        
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(FILE_DIALOG_OPEN, (int)FILE_DIALOG_OPEN)
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(FILE_DIALOG_OPEN_MULTIPLE,
                                       (int)FILE_DIALOG_OPEN_MULTIPLE)
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(FILE_DIALOG_OPEN_FOLDER,
                                       (int)FILE_DIALOG_OPEN_FOLDER)
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(FILE_DIALOG_SAVE, (int)FILE_DIALOG_SAVE)
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(FILE_DIALOG_TYPE_MASK,
                                       (int)FILE_DIALOG_TYPE_MASK)
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(FILE_DIALOG_OVERWRITEPROMPT_FLAG,
                                       (int)FILE_DIALOG_OVERWRITEPROMPT_FLAG)
        AMO_CEF_MESSAGE_TRANSFER_ATTR2(FILE_DIALOG_HIDEREADONLY_FLAG,
                                       (int)FILE_DIALOG_HIDEREADONLY_FLAG)
                                       
        AMO_CEF_MESSAGE_TRANSFER_END()
        
        
    };
}
#endif // AMO_DIALOGTRANSFER_H__
