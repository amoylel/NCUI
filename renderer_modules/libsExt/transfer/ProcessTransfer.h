// Created by amoylel on 12/27/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_PROCESSTRANSFER_H__
#define AMO_PROCESSTRANSFER_H__

#include <amo/singleton.hpp>

#include "transfer/RunnableTransfer.hpp"

namespace amo {
    class process;
    
    /*!
     * @class	Process
     *
     * @extend Runnable
     *
     * @chapter extend
     *
     * @brief	子进程类，本模块的大部分函数都比较慢，尽可能的放到线程里面去执行<br>
     * 			工作线程**Renderer线程**
     */
    
    class ProcessTransfer
        : public RunnableTransfer
        , public amo::singleton<ProcessTransfer> {
    public:
        ProcessTransfer();
        
        /*!
         * @fn	ProcessTransfer::Process(const std::string& strAppPath);
         * @tag constructor sync
         * @brief	创建一个进程类.
         *
         * @param	#String 进程路径 .
         * @return  #Process 进程对象.
         * @example
         *
         ```
        	include ('Process');
        	var process = new Process('NCUI.exe');
        	process.start();
         ```
         */
        
        ProcessTransfer(const std::string& strAppPath);
        
        Any onCreateClass(IPCMessage::SmartType msg) override;
        virtual std::string getClass() const override;
        virtual Transfer* getInterface(const std::string& name) override;
        
        /*!
         * @fn	Any ProcessTransfer::findPIDByName(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	通过程序名称查找进程.
         *
         * @param	#String 程序名.
         *
         * @return	#Array 与程序名匹配的进程ID列表.
         * @example
         *
         ```
        	include('Process');
        	console.log(Process.findPIDByName('NCUI.exe'));
        
        	// 该函数通常需要1秒以上的执行时间，实际上应该把这个函数放到线程上支执行
        	include('Thread');
        	Thread.Exec(Process.findPIDByName, 'NCUI.exe');
        	Process.unique('Process.findPIDByName', function(){
        		console.log(arguments);
        	});
         ```
         */
        
        Any findPIDByName(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any ProcessTransfer::terminateProcessByName(IPCMessage::SmartType msg);
         *
         * @brief	通过程序名称结束进程.
         *
         * @param	#String 程序名.
         *
         * @return	#Boolean true成功/false失败.
         * @example 终止程序
         *
         ```
        	//@skip 注意，运行该示例将结束当前程序
        	include('Process');
        
        	Process.terminateProcessByName('NCUI.exe');
         ```
         */
        
        Any terminateProcessByName(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any ProcessTransfer::killProcessByName(IPCMessage::SmartType msg);
         *
         * @brief	通过程序名杀死进程.
         *
         * @param	#String 程序名.
         * @param	#Boolean=true 是否杀死子进程.
         *
         * @return	#Boolean true成功/false失败.
         *
         * @example 终止程序
         *
         ```
        	 //@skip 注意，运行该示例将结束当前程序
        	 include('Process');
        
        	 Process.killProcessByName('NCUI.exe');
         ```
         */
        
        Any killProcessByName(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any ProcessTransfer::start(IPCMessage::SmartType msg);
         *
         * @brief	运行子程序.
         *
         * @param	#String 启动参数.
         *
         * @return	#Boolean true成功/false失败.
         *
         * @example 终止程序
         *
         ```
        
        	 include('Process');
        	 var process = new Process('C:/Windows/System32/notepad.exe');
        
        	 process.start('manifest.json');
         ```
         */
        
        Any start(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any ProcessTransfer::getResult(IPCMessage::SmartType msg);
         *
         * @brief	获取子进程执行结果，该函数应该放到线程里面去执行，不然会将渲染进程阻塞.
         *
         * @return	无.
         * @example
         *
         ```
        
        	 includes('Process', 'Thread');
        	 var process = new Process('NCUI.exe');
        
        	 process.start('');
        	 Thread.Exec(process.getResult);
        	 process.unique('process.result', function(){
        		console.log(arguments);
        	 });
        	 process.unique('Process.getResult', function(){
        		console.log(arguments);
        	 });
         ```
         */
        
        Any getResult(IPCMessage::SmartType msg);
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(ProcessTransfer, RunnableTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(findPIDByName,
                                      TransferFuncStatic | TransferExecAsync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(terminateProcessByName,
                                      TransferFuncStatic | TransferFuncNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(killProcessByName,
                                      TransferFuncStatic | TransferFuncNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(start, TransferFuncNormal)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getResult, TransferFuncNormal)
        
        
        
        AMO_CEF_MESSAGE_TRANSFER_END()
        
        
    private:
        std::shared_ptr<amo::process> m_pPorcess;
    };
}
#endif // AMO_PROCESSTRANSFER_H__
