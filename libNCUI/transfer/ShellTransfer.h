// Created by amoylel on 07/03/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_SHELLTRANSFER_H__
#define AMO_SHELLTRANSFER_H__

#include <amo/singleton.hpp>

#include "transfer/ClassTransfer.hpp"


namespace amo {

    /**
     * @class	shell
     *
     * @extend Object
     *
     * @brief	运行一个外部程序（或者是打开一个已注册的文件、打开一个目录、打印一个文件等等）.<br>
     * 			工作线程:**UI线程**.
     *
     */
    
    class ShellTransfer
        : public ClassTransfer
        , public amo::singleton<ShellTransfer> {
    public:
        ShellTransfer();
        
        
        
        /**
         * @fn	Any ShellTransfer::exec(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	执行一下shell命令.
         *
         * @param	#String 要执行的shell命令 open print 等.
         * @param	#String 要调用的程序或打开的文件路径.
         * @param	#String 启动参数,默认为"".
         * @param	#String 工作目录,默认为当前路径.
         * @param	#Int=1 显示参数
         *
         *
         * @return	无.
         * @example
         *
        	 ```
        	 include('shell');
        	 shell.exec("open", "explorer", "/e, /select, skin\\close_p.png", "", shell.SW_SHOWNORMAL);
        	 ```
         */
        
        Any exec(IPCMessage::SmartType msg);
        
        
        
        /**
         * @fn	Any ShellTransfer::open(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	打开文件或程序.
         *
         * @param	#String	文件或程序路径.
         * @param	#String	启动参数,默认为"".
         * @param	#String	工作目录,默认为当前路径.
         *
         * @return	无.
         * @example
         *
         ```
				
        		 include('shell');
        		 shell.open("www.baidu.com");
        
         ```
         */
        
        Any open(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any ShellTransfer::print(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	 打印文件.
         *
         * @param	#String 文件路径.
         *
         * @return	无.
         * @example
         *
        
        	 ```
        		include('shell');
        		shell.print("skin\\close_p.png");
        	 ```
         */
        
        Any print(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any ShellTransfer::showItemInFolder(IPCMessage::SmartType msg);
         *
         * @tag static
         *
         * @brief	在文件夹中显示文件（定位文件位置）.
         *
         * @param	#String 文件路径.
         *
         * @return	无.
         * @example
         *
        		 ```
        			include('shell');
        			shell.showItemInFolder('skin\\close_p.png');
        		 ```
         */
        
        Any showItemInFolder(IPCMessage::SmartType msg);
        
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(ShellTransfer, ClassTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(exec, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(open, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(print, TransferFuncStatic)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(showItemInFolder, TransferFuncStatic)
        
        
        /*! @attr #Int=SW_HIDE SW_HIDE 打开时不显示界面. {@tag const static property}*/
        /*! @attr #Int=SW_MINIMIZE SW_MINIMIZE 以最小化的方式打开. {@tag const static property}*/
        /*! @attr #Int=SW_RESTORE SW_RESTORE 以还原显示的方式打开. {@tag const static property}*/
        /*! @attr #Int=SW_SHOW SW_SHOW 打开时显示界面. {@tag const static property}*/
        /*! @attr #Int=SW_SHOWMAXIMIZED SW_SHOWMAXIMIZED 激活窗口并将其最大化. {@tag const static property}*/
        /*! @attr #Int=SW_SHOWMINIMIZED SW_SHOWMINIMIZED 激活窗口并将其最小化. {@tag const static property}*/
        /*! @attr #Int=SW_SHOWMINNOACTIVE SW_SHOWMINNOACTIVE 窗口最小化，激活窗口仍然维持激活状态. {@tag const static property}*/
        /*! @attr #Int=SW_SHOWNA SW_SHOWNA 以窗口原来的状态显示窗口。激活窗口仍然维持激活状态. {@tag const static property}*/
        /*! @attr #Int=SW_SHOWNOACTIVATE SW_SHOWNOACTIVATE 以窗口最近一次的大小和状态显示窗口。激活窗口仍然维持激活状态. {@tag const static property}*/
        /*! @attr #Int=SW_SHOWNORMAL SW_SHOWNORMAL 激活并显示一个窗口。如果窗口被最小化或最大化，系统将其恢复到原来的尺寸和大小。. {@tag const static property}*/
        
        
        AMO_CEF_MESSAGE_TRANSFER_ATTR(SW_HIDE)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(SW_MINIMIZE)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(SW_RESTORE)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(SW_SHOW)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(SW_SHOWMAXIMIZED)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(SW_SHOWMINIMIZED)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(SW_SHOWMINNOACTIVE)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(SW_SHOWNA)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(SW_SHOWNOACTIVATE)
        AMO_CEF_MESSAGE_TRANSFER_ATTR(SW_SHOWNORMAL)
        
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    private:
        int StringToShowCmd(const amo::u8string& str);
    private:
    };
}


#endif // AMO_SHELLTRANSFER_H__
