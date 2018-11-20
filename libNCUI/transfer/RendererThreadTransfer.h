// Created by amoylel on 10/27/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_RENDERERTHREADTRANSFER_H__
#define AMO_RENDERERTHREADTRANSFER_H__

#include "transfer/ThreadTransfer.hpp"


namespace amo {
    class TransferMgr;
    /*!
    * @class Thread
    *
    * @copy  ThreadBase
    *
    * @extend Object
    *
    *
    * @brief 线程类，该类构建的实例会在渲染进程上执行,可以使一个由C++编写的扩展在线程上运行.<br>
    * 		 工作线程:**Renderer线程**.
    * 		 说明:**虽然没有限制，但还是不建议将内置接口放到线程上去执行**
    */
    class RendererThreadTransfer : public ThreadTransfer<ThreadRenderer>
        , public amo::singleton<RendererThreadTransfer> {
    public:
        RendererThreadTransfer();
        ~RendererThreadTransfer();
        Any onCreateClass(IPCMessage::SmartType msg);
        
        virtual std::shared_ptr< TransferMgr> getTransferMgr() override;
        
        virtual std::string getClass() const override {
            return "Thread";
        }
        
        virtual Transfer* getInterface(const std::string& name) override {
            if (name == RendererThreadTransfer::getClass()) {
                return this;
            }
            
            return ThreadBaseTransfer::getInterface(name);
        }
        
        /*!
        * @section	Thread演示
        *
        * @brief	Thread演示
        * @example
        *
        ```
        includes('Thread', 'Demo');
        var demoTest = new Demo();
        
        demoTest.unique('Demo.add', function(retval){
        	console.log(arguments);
        	console.log(retval.data);
        });
        
        demoTest.unique('Demo.sub', function(retval){
        	console.log(arguments);
        	console.log(retval.data);
        });
        
        // 使用Thread类提供的线程
        Thread.Exec(demoTest.add, 1,1);
        
        
        console.assert(Thread.Sync(demoTest.add, 1,2) == 3);
        
        var thread = new Thread();
        thread.exec(demoTest.add, 1,3);
        
        
        console.assert(thread.sync(demoTest.add, 1,4) == 5);
        
        // 附加到线程
        demoTest.attach(thread);
        // 输出undefined
        console.log(demoTest.add(1,5));
        demoTest.sub(3,1);
        demoTest.detach();
        console.assert(demoTest.add(1,6) == 7);
        
        demoTest.attach(thread);
        // 在扩展中挂起线程
        demoTest.sleepForWeakup();
        // add 不能得到结果
        demoTest.add(1,8);
        setTimeout(function(){
        	// 唤醒线程后，继续执行add(1,8);
        	demoTest.weakup();
        
        	// 挂起线程
        
        	demoTest.suspend();
        	demoTest.add(1,9);
        	setTimeout(function(){
        		// 线程已经被杀死，add(1,9)不会被执行;
        		demoTest.weakup();
        	}, 5000);
        
        	demoTest.detach();
        	thread.kill(); // 杀死线程
        
        	demoTest.add(1,10);
        	console.assert(demoTest.add(1,10) == 11);
        
        }, 5000);
        
        
        
        ```
        */
    };
    
}

#endif AMO_RENDERERTHREADTRANSFER_H__