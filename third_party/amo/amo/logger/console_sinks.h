// Created by amoylel on 30/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_CONSOLE_SINKS_D2328EDB_0E61_46AA_95AF_A14D6917552A_H__
#define AMO_CONSOLE_SINKS_D2328EDB_0E61_46AA_95AF_A14D6917552A_H__




#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>

#include <cstdio>
#include <memory>
#include <mutex>

namespace spdlog {
    namespace sinks {
    
        class log_filter {
        
        public:
            log_filter() {
            
            
            }
            ~log_filter() {
            
            }
            // 添加允许的规则
            void add_allowed_rule(const std::string& str) {
            
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                allowed_rules.insert(str);
            }
            
            // 添加不允许写的规则
            void add_disallowed_rule(const std::string& str) {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                
                disallowed_rules.insert(str);
            }
            
            void remove_allowed_rule(const std::string& str) {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                
                if (str.empty()) {
                    allowed_rules.clear();
                } else {
                    allowed_rules.erase(str);
                }
            }
            void remove_disallowed_rule(const std::string& str) {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                
                if (str.empty()) {
                    disallowed_rules.clear();
                } else {
                    disallowed_rules.erase(str);
                }
            }
            
            bool need_match()const {
                return allowed_rules.size() > 0 || disallowed_rules.size() > 0;
            }
            
            // 判断日志是否满足过滤规则
            bool match(const std::string str) {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                
                try {
                    // 如果满足排除规则，那么直接退出
                    for (auto& p : disallowed_rules) {
                        if (std::regex_search(str, std::regex(p))) {
                            return false;
                        }
                    }
                    
                    bool valid_log = allowed_rules.empty() ? true : false;
                    
                    
                    for (auto& p : allowed_rules) {
                        if (std::regex_search(str, std::regex(p))) {
                            valid_log = true;
                            break;
                        }
                    }
                    
                    return valid_log;
                    
                } catch (const std::exception& e) {
                    std::cout << e.what() << std::endl;
                    return false;
                }
                
            }
            
        public:
        
            std::recursive_mutex m_mutex;
            std::set<std::string> allowed_rules;
            std::set<std::string> disallowed_rules;
        };
        
        
        
        template <class Mutex>
        class console_sink : public base_sink<Mutex> {
            using MyType = console_sink<Mutex>;
        public:
            console_sink() {}
            static std::shared_ptr<MyType> instance() {
                static std::shared_ptr<MyType> instance = std::make_shared<MyType>();
                return instance;
            }
            
            void _sink_it(const details::log_msg& msg) override {
                if (m_filter.need_match() && !m_filter.match(msg.formatted.data())) {
                    return;
                }
                
                std::string str(msg.formatted.data(), msg.formatted.size());
                std::cout << str;
                //fwrite(msg.formatted.data(), sizeof(char), msg.formatted.size(), stdout);
                flush();
            }
            
            void flush() override {
                std::cout.flush();
                //fflush(stdout);
            }
            
            
            void add_allowed_rule(const std::string& str) {
                m_filter.add_allowed_rule(str);
            }
            
            // 添加不允许写的规则
            void add_disallowed_rule(const std::string& str) {
                m_filter.add_disallowed_rule(str);
            }
            
            void remove_allowed_rule(const std::string& str) {
                m_filter.remove_allowed_rule(str);
            }
            void remove_disallowed_rule(const std::string& str) {
                m_filter.remove_disallowed_rule(str);
                
            }
            
            log_filter m_filter;
        };
        
        typedef console_sink<details::null_mutex> console_sink_st;
        typedef console_sink<std::mutex> console_sink_mt;
        
    }
}




#endif //AMO_CONSOLE_SINKS_D2328EDB_0E61_46AA_95AF_A14D6917552A_H__