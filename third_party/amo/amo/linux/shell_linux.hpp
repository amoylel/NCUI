// Created by amoylel on 04/09/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_SHELL_LINUX_501E26BA_9CAA_4AE9_B5D3_BFEBA4E6B3DB_HPP__
#define AMO_SHELL_LINUX_501E26BA_9CAA_4AE9_B5D3_BFEBA4E6B3DB_HPP__

#include <string>

namespace amo {
    class shell {
    
    public:
        shell(const std::string& strExe):
            fileName(strExe) {
            nShowCmd = 0;
        }
        
        int exec() {
            return 0;
        }
        
        
        int open() {
            std::string command = fileName + " " + params;
            return (int)::system(command.c_str());
        }
        int print() {
            return 0;
        }
        int showItemInFolder() {
            return 0;
        }
        
        void addArgs(const std::string args) {
            params += " ";
            params += args;
        }
        
        void show(bool bShow) {
            if (bShow) {
                nShowCmd = 1;
            } else {
                nShowCmd = 0;
            }
        }
    private:
        int nShowCmd;
        std::string fileName;
        std::string params;
        std::string workDir;
        
    };
}
#endif //AMO_SHELL_LINUX_501E26BA_9CAA_4AE9_B5D3_BFEBA4E6B3DB_HPP__