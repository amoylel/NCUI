// Created by amoylel on 15/06/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_LOOPER_EXECUTOR_POOL_12E4FFCE_E9DC_4413_9438_1C64366E2251_HPP__
#define AMO_LOOPER_EXECUTOR_POOL_12E4FFCE_E9DC_4413_9438_1C64366E2251_HPP__

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <amo/logger.hpp>
#include <amo/looper_executor.hpp>

namespace amo {

    class looper_executor_pool {
    public:
    
        looper_executor_pool(size_t threads = 2)
            : m_is_running(true) {
            init(threads);
        }
        
        void set_name(const std::string& name) {
            m_thread_pool_name = name;
            
            for (size_t i = 0; i < m_executors.size(); ++i) {
                auto& p = m_executors[i];
                p->set_name(m_thread_pool_name + std::to_string(i));
            }
        }
        std::string get_name() const {
            return m_thread_pool_name;
        }
        
        bool is_running() {
            return m_is_running;
        }
        
        void response_test(int64_t test_id,
                           std::function < void(amo::looper_executor::thread_response_info)> fn = {}) {
                           
            for (auto& p : m_executors) {
                p->response_test(test_id, fn);
            }
        }
        
        void execute(amo::looper_executor::executor runnable_, int64_t thread_id = 0) {
            return push(runnable_, thread_id);
        }
        
        void push(amo::looper_executor::executor runnable_, int64_t thread_id = 0) {
            std::shared_ptr<looper_executor> executor;
            
            ++m_total_packages;
            
            for (auto& p : m_executors) {
            
                if (p->get_id_number() == thread_id) {
                    p->execute(runnable_);
                    return;
                }
                
                if (!executor) {
                    executor = p;
                    continue;
                }
                
                if (p->size() < executor->size()) {
                    executor = p;
                    continue;
                }
                
            }
            
            if (executor) {
                executor->execute(runnable_);
            }
            
        }
        
        // the destructor joins all threads
        ~looper_executor_pool() {
            stop(false);
        }
        
        void stop(bool wait_for_empty_ = true) {
            if (!is_running()) {
                return;
            }
            
            m_is_running = false;
            
            for (auto &executor : m_executors) {
                if (wait_for_empty_) {
                    executor->wait_for_empty();
                }
                
                executor->stop();
            }
        }
        
        void wait_for_empty(int delay = 100) {
            if (!is_running()) {
                return;
            }
            
            for (auto &executor : m_executors) {
                executor->wait_for_empty();
            }
            
            // std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
        
        
        std::vector<int> get_ids() {
        
            std::vector<int> vec;
            
            for (auto& p : m_executors) {
                vec.push_back(p->get_id_number());
            }
            
            return vec;
            
        }
        
        void check_if_called_on_valid_thread() {
            if (!check_on_looper_executor_pool()) {
                throw std::runtime_error("method is not called on valid thread");
            }
        }
        
        bool check_on_looper_executor_pool() {
            for (auto& p : m_executors) {
                if (amo::this_thread::get_id() == p->get_id()) {
                    return true;
                }
            }
            
            return false;
            
        }
        
        // 获取线程池中未处理的消息总数
        size_t size(int id = -1) const {
        
            size_t retval = 0;
            
            for (auto& p : m_executors) {
                if (p->get_id_number() == id) {
                    return p->size();
                }
                
                retval += p->size();
                
            }
            
            return retval;
        }
        
        int64_t total() const {
            return m_total_packages;
        }
        
        bool on_thread_exception(int64_t thread_id, const std::string& error_msg) {
            $cerr("thread [{}] execption:{}", thread_id, error_msg);
            return false;
        }
        
        std::vector< std::shared_ptr<amo::looper_executor> > looper_executors() {
            return m_executors;
        }
        
    protected:
        void init(size_t threads) {
            for (size_t i = 0; i < threads; ++i) {
                std::shared_ptr<amo::looper_executor> executor;
                executor.reset(new amo::looper_executor(10000, false));
                executor->set_exception_callback(std::bind(
                                                     &looper_executor_pool::on_thread_exception,
                                                     this,
                                                     std::placeholders::_1,
                                                     std::placeholders::_2));
                m_executors.push_back(executor);
                
            }
            
        }
        
    private:
        std::vector< std::shared_ptr<amo::looper_executor> > m_executors;
        
        /** @brief	是否在在运行. */
        std::atomic_bool m_is_running;
        
        std::atomic<int64_t> m_total_packages;
        
        /** @brief	最后一次调用的线程. */
        std::shared_ptr<amo::looper_executor> m_last_executor;
        
        std::string m_thread_pool_name;
    };
    
}

#endif //AMO_LOOPER_EXECUTOR_POOL_12E4FFCE_E9DC_4413_9438_1C64366E2251_HPP__