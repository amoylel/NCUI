// Created by amoylel on 04/09/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef _SHELL_WIN_687E77E6_CA2A_4086_98DE_23F7837626A9_HPP__
#define _SHELL_WIN_687E77E6_CA2A_4086_98DE_23F7837626A9_HPP__



#include <string>
#include <WinSock2.h>
#include <windows.h>
#include <shellapi.h>


#include <amo/string.hpp>
namespace amo {
    class shell {
    
    public:
        shell(const amo::u8string& strExe):
            fileName(strExe) {
            nShowCmd = SW_SHOWNORMAL;
        }
        
        int exec() {
            return 0;
        }
        
        
        int open() {
            return (int)::ShellExecuteW(NULL,
                                        L"open",
                                        fileName.to_wide().c_str(),
                                        params.to_wide().c_str(),
                                        workDir.to_wide().c_str(),
                                        nShowCmd);
        }
        int print() {
            return 0;
        }
        int showItemInFolder() {
            return 0;
        }
        
        void addArgs(const amo::u8string& args) {
            params += amo::u8string(" ", true);
            params += args;
        }
        
        void show(bool bShow) {
            if (bShow) {
                nShowCmd = SW_SHOWNORMAL;
            } else {
                nShowCmd = SW_HIDE;
            }
        }
    private:
        int nShowCmd;
        amo::u8string fileName;
        amo::u8string params;
        amo::u8string workDir;
        
    };
}

#endif //_SHELL_WIN_687E77E6_CA2A_4086_98DE_23F7837626A9_HPP__