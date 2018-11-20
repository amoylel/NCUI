#include "stdafx.h"

#include "transfer/ShellTransfer.h"

#include <shellapi.h>
#include <amo/path.hpp>

namespace amo {

    ShellTransfer::ShellTransfer()
        : ClassTransfer("shell") {
    }
    
    Any ShellTransfer::exec(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        amo::u8string strOperation(args->getString(0), true);
        amo::u8string strFileName(args->getString(1), true);
        amo::u8string strParam(args->getString(2), true);
        amo::u8string stsrDir(args->getString(3), true);
        int nShowCmd = SW_SHOWNORMAL;
        
        if (args->isValid(4)) {
            nShowCmd = args->getInt(4);
        }
        
        return (int)::ShellExecuteW(NULL,
                                    strOperation.to_unicode().c_str(),
                                    strFileName.to_unicode().c_str(),
                                    strParam.to_unicode().c_str(),
                                    stsrDir.to_unicode().c_str(),
                                    nShowCmd);
    }
    
    
    Any ShellTransfer::open(IPCMessage::SmartType msg) {
        std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
        amo::u8string strFileName(args->getString(0), true);
        amo::u8string strParam(args->getString(1), true);
        amo::u8string stsrDir(args->getString(3), true);
        return (int)::ShellExecuteW(NULL,
                                    L"open",
                                    strFileName.to_unicode().c_str(),
                                    strParam.to_unicode().c_str(),
                                    stsrDir.to_unicode().c_str(),
                                    SW_SHOWNORMAL);
    }
    
    Any ShellTransfer::print(IPCMessage::SmartType msg) {
        amo::u8string str(msg->getArgumentList()->getString(0), true);
        return (int)::ShellExecuteW(NULL,
                                    L"print",
                                    str.to_unicode().c_str(),
                                    NULL,
                                    NULL,
                                    SW_SHOWNORMAL);
    }
    
    Any ShellTransfer::showItemInFolder(IPCMessage::SmartType msg) {
        amo::u8string str(msg->getArgumentList()->getString(0), true);
        amo::u8string strParam(amo::u8string("/e, /select, ", true));
        strParam += str;
        return (int)::ShellExecuteW(NULL,
                                    L"open",
                                    L"explorer.exe",
                                    strParam.to_wide().c_str(),
                                    NULL,
                                    SW_SHOWNORMAL);
    }
    
    int ShellTransfer::StringToShowCmd(const amo::u8string& str) {
        if (str == "SW_HIDE") {
            //隐藏窗口，活动状态给令一个窗口
            return SW_HIDE;
        } else if (str == "SW_MINIMIZE") {
            //最小化窗口，活动状态给令一个窗口
            return SW_MINIMIZE;
        } else if (str == "SW_RESTORE") {
            //用原来的大小和位置显示一个窗口，同时令其进入活动状态
            return SW_RESTORE;
        } else if (str == "SW_SHOW") {
            //用当前的大小和位置显示一个窗口，同时令其进入活动状态
            return SW_SHOW;
        } else if (str == "SW_SHOWMAXIMIZED") {
            //最大化窗口，并将其激活
            return SW_SHOWMAXIMIZED;
        } else if (str == "SW_SHOWMINIMIZED") {
            //最小化窗口，并将其激活
            return SW_SHOWMINIMIZED;
        } else if (str == "SW_SHOWMINNOACTIVE") {
            //最小化一个窗口，同时不改变活动窗口
            return SW_SHOWMINNOACTIVE;
        } else if (str == "SW_SHOWNA") {
            //用当前的大小和位置显示一个窗口，不改变活动窗口
            return SW_SHOWNA;
        } else if (str == "SW_SHOWNOACTIVATE") {
            //用最近的大小和位置显示一个窗口，同时不改变活动窗口
            return SW_SHOWNOACTIVATE;
        } else if (str == "SW_SHOWNORMAL") {
            //与RESTORE相同
            return SW_SHOWNORMAL;
        } else {
            return SW_SHOWNORMAL;
        }
        
    }
    
}