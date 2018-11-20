// Created by amoylel on 10/27/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_UITHREADTRANSFER_H__
#define AMO_UITHREADTRANSFER_H__

#include "transfer/ThreadTransfer.hpp"
namespace amo {

    /*!
     * @class	Task
     *
     * @copy  ThreadBase
     *
     * @brief	线程类，该类构建的实例会在UI进程上执行,可以使一个由C++编写的扩展在线程上运行.<br>
     * 			工作线程:**UI线程**.<br>
     * 			说明:**虽然没有限制，但还是不建议将内置接口放到线程上去执行**
     *
     */
    
    class UIThreadTransfer : public ThreadTransfer<ThreadUI>
        , public amo::singleton<UIThreadTransfer> {
    public:
        UIThreadTransfer();
        
        virtual  std::shared_ptr< TransferMgr> getTransferMgr() override;
        virtual Any onCreateClass(IPCMessage::SmartType msg) override;
        virtual std::string getClass() const override;
        
        virtual Transfer* getInterface(const std::string& name) override;
        
        
        /*!
        * @section	Task演示
        *
        * @brief	Task演示
        * @example
        *
        ```
        	includes('Task', 'TaskTest');
        	var taskTest = new TaskTest();
        
        	taskTest.unique('TaskTest.add', function(retval){
        		console.log(arguments);
        		console.log(retval.data);
        	});
        
        	taskTest.unique('TaskTest.sub', function(retval){
        		console.log(arguments);
        		console.log(retval.data);
        	});
        
        	// 使用Task类提供的线程
        	Task.Exec(taskTest.add, 1,1);
        
        
        	console.assert(Task.Sync(taskTest.add, 1,2) == 3);
        
        	var thread = new Task();
        	thread.exec(taskTest.add, 1,3);
        
        
        	console.assert(thread.sync(taskTest.add, 1,4) == 5);
        
        	 // 附加到线程
        	taskTest.attach(thread);
        	// 输出undefined
        	console.log(taskTest.add(1,5));
        	taskTest.sub(3,1);
        	taskTest.detach();
        	console.assert(taskTest.add(1,6) == 7);
        
        	taskTest.attach(thread);
        	// 在扩展中挂起线程
        	taskTest.sleepForWeakup();
        	// add 不能得到结果
        	taskTest.add(1,8);
        	setTimeout(function(){
        		// 唤醒线程后，继续执行add(1,8);
        		taskTest.weakup();
        
        		// 挂起线程
        
        		taskTest.suspend();
        		taskTest.add(1,9);
        		setTimeout(function(){
        			// 线程已经被杀死，add(1,9)不会被执行;
        			taskTest.weakup();
        		}, 5000);
        
        		taskTest.detach();
        		thread.kill(); // 杀死线程
        
        		taskTest.add(1,10);
        		console.assert(taskTest.add(1,10) == 11);
        
        	}, 5000);
        
        
        
        ```
        */
        
        
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(UIThreadTransfer, ThreadTransfer<ThreadUI>)
        
        
        AMO_CEF_MESSAGE_TRANSFER_END()
        
        
        
    };
}


#endif AMO_UITHREADTRANSFER_H__
