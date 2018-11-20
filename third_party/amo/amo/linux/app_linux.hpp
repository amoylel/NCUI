// Created by amoylel on 31/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_APP_LINUX_A9603A53_F74D_49BB_B40F_70B902ABFCC0_HPP__
#define AMO_APP_LINUX_A9603A53_F74D_49BB_B40F_70B902ABFCC0_HPP__

#include <string>

#include <sstream>
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <amo/path.hpp>
#include <amo/linux/shell_linux.hpp>

namespace amo {
    class app {
    public:
    
        static bool make_daemon(void) {
            int pid = 0;
            
            if (pid = fork()) {
                exit(0);    //是父进程，结束父进程
            } else if (pid < 0) {
                //exit(1);    //fork失败，退出
                return false;
            }
            
            //是第一子进程，后台继续执行
            setsid();//第一子进程成为新的会话组长和进程组长
            
            //并与控制终端分离
            if (pid = fork()) {
                exit(0);    //是第一子进程，结束第一子进程
            } else if (pid < 0) {
                //exit(1);    //fork失败，退出
                return false;
            }
            
            //是第二子进程，继续
            //第二子进程不再是会话组长
            for (int i = 0; i < NOFILE; ++i) { //关闭打开的文件描述符
                ::close(i);
            }
            
            //chdir("/tmp");//改变工作目录到/tmp
            umask(0);//重设文件创建掩模
            return true;
        }
        
    public:
        app(const std::string& appPath = "") {
            m_appPath = appPath;
            
            if (m_appPath.empty()) {
                m_appPath = amo::u8path::getFullExeName();
                m_pid = (int64_t)getpid();
                
            }
        }
        
        std::string dataDir() {
            return "";
        }
        
        std::string tempDir() {
            return "";
        }
        
        void setAutoRun(bool bAutoRun = true) {
        
            return;
        }
        
        
        bool isAutoRun() {
            return false;
        }
        
        bool restart(int delay = 3) {
            // 只允许调用一次
            static std::atomic<bool> is_called(false);
            
            if (is_called) {
                return false;
            }
            
            is_called = true;
            newInstance(delay);
            close();
            return true;
        }
        
        // 新开实例
        bool newInstance(int delay = 3) {
            std::shared_ptr<amo::shell> shell(new amo::shell(""));
            shell->addArgs("sleep");
            shell->addArgs(std::to_string(delay));
            shell->addArgs("&&");
            shell->addArgs(m_appPath);
            shell->addArgs("&");
            shell->show(false);
            return  shell->open() == 0;
        }
        
        // 关闭程序
        void close() {
            if (m_fnClose) {
                m_fnClose();
            } else {
                //::ExitProcess(0);
            }
        }
        
        void registerCloseCallback(std::function<void()> fn) {
            m_fnClose = fn;
        }
        
        int64_t pid() const {
            return m_pid;
        }
        
        
    private:
        std::string m_appPath;
        
        std::function<void()> m_fnClose;
        
        int64_t m_pid;
    };
}



#endif //AMO_APP_LINUX_A9603A53_F74D_49BB_B40F_70B902ABFCC0_HPP__