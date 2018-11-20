// Created by amoylel on 31/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_PROCESS_LINUX_3D45A7C0_9FAC_48C8_94D4_E6A4F4EC00A2_HPP__
#define AMO_PROCESS_LINUX_3D45A7C0_9FAC_48C8_94D4_E6A4F4EC00A2_HPP__

#include <vector>
#include <string>
#include <sstream>
#include <cstdint>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <amo/timer.hpp>
#include <amo/config.hpp>
#include <amo/logger.hpp>
#include <amo/string/string_utils.hpp>

namespace amo {
    class process {
    public:
    
    
        static int64_t get_current_process_id() {
            return ::getpid();
        }
        
        static int64_t get_current_parent_id() {
            return ::getppid();
        }
        
        static int64_t get_parent_process_pid(int64_t pid, bool root = false) {
            char dir[1024] = { 0 };
            char path[1024] = { 0 };
            char buf[1024] = { 0 };
            int rpid = 0;
            int fpid = 0;
            char fpth[1024] = { 0 };
            struct stat st;
            ssize_t ret = 0;
            
            ::sprintf(dir, "/proc/%d/", pid);
            
            ::sprintf(path, "%sstat", dir);
            
            if (stat(path, &st) != 0) {
                return -2;
            }
            
            memset(buf, 0, strlen(buf));
            
            FILE * fp = fopen(path, "r");
            
            ret += fread(buf + ret, 1, 300 - ret, fp);
            
            fclose(fp);
            
            sscanf(buf, "%*d %*c%s %*c %d %*s", fpth, &fpid);
            
            fpth[strlen(fpth) - 1] = '\0';
            
            if (strcmp(fpth, "bash") != 0 &&
                    strcmp(fpth, "sudo") != 0) { //bash 终端  sudo 终端
                if (fpid == 1) {
                    return pid;
                } else if (fpid == 2) {
                    return -1; //内核线程
                }
                
                if (root) {
                    rpid = get_parent_process_pid(fpid, root);
                    
                    if (rpid == 0) {
                        rpid = pid;
                    }
                } else {
                    return fpid;
                }
                
            }
            
            return rpid;
        }
        
        static int64_t  find_a_pid_by_process_name(const std::string&
                procName) {
            FILE *fpipe = NULL;
            char command[BUFSIZ] = { 0 };
            unsigned int procId = 0;
            ::sprintf(command, "ps ax | grep -v grep | grep %s", procName.c_str());
            
            
            if ((fpipe = (FILE*)popen(command, "r")) != 0) {
                char results[1000] = { 0 };
                
                while (fgets(results, sizeof results, fpipe)) {
                
                    std::string str = results;
                    
                    std::string appName = procName;
                    
                    amo::string_utils::trim_left(appName, " \n\t./\\");
                    amo::string_utils::trim_right(str, " \t\n");
                    
                    std::cout << str << std::endl;
                    
                    if (amo::string_utils::is_utf8_system()) {
                        amo::u8path p(str);
                        p.normalize().remove_args();
                        str = p.generic_string();
                    } else {
                        amo::path p(str);
                        p.normalize().remove_args();
                        str = p.generic_string();
                    }
                    
                    // 获取到的字符串比进程名还短，为无效数据
                    if (str.size() < appName.size()) {
                        continue;
                    }
                    
                    if (!amo::string_utils::end_with(str, appName)) {
                        continue;
                    }
                    
                    str = str.substr(0, str.size() - appName.size());
                    
                    if (!str.empty()) {
                        char p = str[str.size() - 1];
                        
                        if (p != '/' && p != '\\') {
                            continue;
                        }
                    }
                    
                    sscanf(results, "%u", &procId);
                    break;
                }
                
                pclose(fpipe);
            }
            
            return procId;
        }
        
        static std::vector<int64_t> find_pid_by_process_name(const std::string&
                procName) {
            std::vector<int64_t> vec;
            
            FILE *fpipe = NULL;
            char command[BUFSIZ] = { 0 };
            
            ::sprintf(command, "ps ax | grep -v grep | grep %s", procName.c_str());
            
            
            if ((fpipe = (FILE*)popen(command, "r")) != 0) {
                char results[1000] = { 0 };
                
                while (fgets(results, sizeof results, fpipe)) {
                    unsigned int procId = 0;
                    std::string str = results;
                    std::string appName = procName;
                    
                    amo::string_utils::trim_left(appName, " \t./\\");
                    amo::string_utils::trim_right(str, " \t");
                    
                    // 获取到的字符串比进程名还短，为无效数据
                    if (str.size() < appName.size()) {
                        continue;
                    }
                    
                    if (!amo::string_utils::end_with(str, appName)) {
                        continue;
                    }
                    
                    str = str.substr(0, str.size() - appName.size());
                    
                    if (!str.empty()) {
                        char p = str[str.size() - 1];
                        
                        if (p != ' ' && p != '/' && p != '\\') {
                            continue;
                        }
                    }
                    
                    sscanf(results, "%u", &procId);
                    
                    vec.push_back(procId);
                    
                }
                
                pclose(fpipe);
            }
            
            return vec;
            
            
        }
        
        static bool kill_current_process() {
            return kill_process_by_pid(get_current_process_id());
        }
        
        static  int64_t kill_a_process_by_name(const std::string& procName,
                                               bool killSubProcess = true) {
            unsigned procId = find_a_pid_by_process_name(procName);
            
            if (procId != 0) {
                char command[1000] = { 0 };
                ::sprintf(command, "kill -9 %u", procId);
                
                std::cout << command << std::endl;
                
                if (system(command) != 0) {
                    perror("Kill failed.");
                    return -1;
                }
                
            }
            
            //amo::path::set_work_path_to_app_path()
            return procId;
        }
        
        static bool  kill_process_by_name(const std::string& procName,
                                          bool killSubProcess = true) {
                                          
            bool success = true;
            int retry_count = 0;
            
            while (true) {
                int64_t retval = kill_a_process_by_name(procName, killSubProcess);
                
                if (retval < 0) {
                    success = false;
                    retry_count++;
                }
                
                
                if (retry_count > 3) {
                    return false;
                }
                
                
                if (retval == 0) {
                    break;
                }
            }
            
            return success;
            
        }
        
        static  bool kill_process_by_pid(int pid, bool killSubProcess = true) {
            std::string command = "kill -9 " + std::to_string(pid);
            system(command.c_str());
            return true;
        }
        
        static  std::string find_process_name_by_pid(int64_t pid) {
            char *task_name;
            char proc_pid_path[1024];
            char buf[1024];
            
            ::sprintf(proc_pid_path, "/proc/%d/status", pid);
            FILE* fp = fopen(proc_pid_path, "r");
            
            if (NULL != fp) {
                if (fgets(buf, 1024 - 1, fp) == NULL) {
                    fclose(fp);
                }
                
                fclose(fp);
                sscanf(buf, "%*s %s", task_name);
            }
            
            return task_name;
        }
        
        
        
    };
}



#endif //AMO_PROCESS_LINUX_3D45A7C0_9FAC_48C8_94D4_E6A4F4EC00A2_HPP__