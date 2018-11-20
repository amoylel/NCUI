// Created by amoylel on 06/13/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_CONSTANTS_HPP__
#define AMO_CONSTANTS_HPP__

#include <string>


#define WM_CUSTOM_MESSAGE_BEGIN (WM_USER + 1000)
// 进入全屏
#define WM_ENTER_FULLSCREEN (WM_CUSTOM_MESSAGE_BEGIN + 1)
// 离开全屏
#define WM_LEAVE_FULLSCREEN (WM_CUSTOM_MESSAGE_BEGIN + 2)
// 窗口创建成功
#define WM_WINDOW_CREATED (WM_CUSTOM_MESSAGE_BEGIN + 3)
// 管理创建成功
#define WM_IPC_READY (WM_CUSTOM_MESSAGE_BEGIN + 4)

namespace amo {

    /*! @brief	通知主进程管道已经准备好，可以进行管道通信了. */
    static const std::string MSG_IPC_READY = "IPCReady";
    /*! @brief	创建渲染进程客户端Pipe由渲染进程发送，主进程接收处理. */
    static const std::string MSG_CREATE_PIPE_CLIENT	 = "CreatePipeClient";
    /*! @brief	日志消息. */
    static const std::string MSG_LOG_MESSAGE		 = "LogMessage";
    /*! @brief	设置是否允许前进后退. */
    static const std::string MSG_ENABLE_BACK_FORWORD = "EnableBackForword";
    /*! @brief	直接调用C++或JS. */
    static const std::string MSG_NATIVE_EXECUTE		 = "NativeExecute";
    /*! @brief	同步调用C++或JS. */
    static const std::string MSG_NATIVE_SYNC_EXECUTE = "NativesyncExecute";
    /*! @brief	异步调用C++或JS. */
    static const std::string MSG_NATIVE_ASYNC_EXECUTE = "NativeasyncExecute";
    /*! @brief	通知对方处理同步数据. */
    static const std::string MSG_PROCESS_SYNC_EXECUTE = "ProcesssyncExecute";
    /*! @brief	焦点发生改变时触发. */
    static const std::string MSG_FOCUSED_NODE_CHANGED = "focusedNodeChanged";
    
    static const std::string MSG_BROWSER_SETTINGS = "browserSettings";
    
    /*! @brief	默认管道大小. */
    static const int DefaultPipeSize = 2000000;
    
    /*! @brief	渲染进程管道前缀. */
    static const std::string RendererPipePrefix = "render_pipe_name_";
    /*! @brief	主进程管道前缀. */
    static const std::string BrowserPipePrefix = "browser_pipe_name_";
    
    /*! @brief	管道消息头部. */
    static const char PipeMessageHeader = 'a';
    /*! @brief	管道消息尾部. */
    static const char PipeMessageTail = 'z';
    
    // 消息各参数所在位置
    class IPCArgsPosInfo {
    public:
        /*! @brief	消息参数的起始位置. */
        const static int MessageArgs = 0;
        /*! @brief	消息名称所在位置. */
        const static int FuncName = 10;
        
        /*! @brief	自定义参数的起始位置. */
        const static int CustomArgs = 11;
        
        /*! @brief	自定义事件名称. */
        const static int CustomEventName = 18;
        
        /*! @brief	Transfer类名位置. */
        const static int TransferName = 11;
        /*! @brief	TransferID位置. */
        const static int TransferID = 12;
        
        /*! @brief	需要触发事件的EventEmittter ID. */
        const static int EventObjectID = 14;
        /*! @brief	JavaScript 函数名 相当于自定义参数里面的一个. */
        const static int JsFuncName = 13;
        
        
        /*! @brief	动态库名. */
        const static int DllName = 11;
        /*! @brief	动态库函数名. */
        const static int DllFuncName = 12;
        /*! @brief	动态库函数返回值类型. */
        const static int DllRetalType = 13;
        
        /** @brief	要在线程中执行的Transfer Name. */
        const static int ThreadTransferName = 15;
        
        /** @brief	要在线程中执行的Transfer ID. */
        const static int ThreadTransferID = 16;
        
        /** @brief	要在线程中执行的函数名. */
        const static int ThreadTransferFuncName = 17;
        
        /** @brief	跨进程调用标志1表示只在当前进程中执行，0表示跨进程调用. */
        const static int CrossProcessFlag = 22;
        
        /*! @brief	异步调用时回调函数位置. */
        const static int AsyncCallback = 23;
        /*! @brief	消息列表中保存BrowserID的位置. */
        const static int BrowserID = 24;
        /*! @brief	消息列表中保存FrameID的位置. */
        const static int FrameID = 25;
        /*! @brief	消息列表中保存参数列表长度的位置. */
        const static int ArgsLength = 26;
        /*! @brief	消息ID. */
        const static int MessageID = 27;
        /*! @brief	参数列表最大长度[0,16]. */
        const static int MaxLength = 28;
    };
    
    
}
#endif // AMO_CONSTANTS_HPP__