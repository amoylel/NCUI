// Created by amoylel on 15/06/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_LOOPER_EXECUTOR_01A79A7C_FD15_4400_980C_4512FD69CB94_HPP__
#define AMO_LOOPER_EXECUTOR_01A79A7C_FD15_4400_980C_4512FD69CB94_HPP__






#include <strstream>
#include <queue>
#include <exception>
#include <string>
#include <iostream>

#include <amo/chrono.hpp>
#include <amo/thread.hpp>
#include <amo/functional.hpp>
#include <amo/memory.hpp>
#include <amo/logger.hpp>
#include <amo/atomic.hpp>
#include <amo/config.hpp>



namespace amo {

    class runnable {
    public:
        typedef  amo::chrono::time_point<amo::chrono::system_clock> time_point;
        typedef amo::function<void()> executor;
    public:
        runnable(executor run_, uint64_t runnable_index, uint64_t delay = 0) {
            time_point now = amo::chrono::system_clock::now();
            execute_time_point_ = now + amo::chrono::milliseconds(delay);
            insert_time_point_ = runnable_index;
            runnable_ = run_;
        }
        
        runnable(executor run_, uint64_t runnable_index,
                 time_point execute_time = amo::chrono::system_clock::now()) {
            execute_time_point_ = execute_time;
            insert_time_point_ = runnable_index;
            runnable_ = run_;
        }
        
        runnable(const runnable& rhs)
            : runnable_(rhs.runnable_),
              execute_time_point_(rhs.execute_time_point_),
              insert_time_point_(rhs.insert_time_point_) {
              
        }
        
        operator time_point() const {
            return execute_time_point_;
        }
        
        operator executor() const {
            return runnable_;
        }
        
        void operator()() {
            runnable_();
        }
        
        friend bool operator > (const runnable& lhs, const runnable& rhs) {
            if (lhs.execute_time_point_ == rhs.execute_time_point_) {
                return lhs.insert_time_point_ > rhs.insert_time_point_;
            }
            
            return lhs.execute_time_point_ > rhs.execute_time_point_;
        }
    private:
        time_point execute_time_point_;
        executor runnable_;
        uint64_t insert_time_point_;
    };
    
    
    class looper_executor {
    public:
        class thread_response_info {
        public:
            thread_response_info() {
                amo::chrono::time_point<amo::chrono::system_clock> now =
                    amo::chrono::system_clock::now();
                request_timestamp = amo::chrono::duration_cast<amo::chrono::milliseconds>
                                    (now.time_since_epoch()).count();
                response_timestamp = request_timestamp;
                delay = 0;
                exception_count = 0;
                test_id = 0;
            }
            
        public:
        
            std::string thread_name;	// 线程名
            int64_t thread_id;			// 线程ID
            int64_t test_id;			// 测试消息ID
            int64_t request_timestamp;	// 请求时间
            int64_t response_timestamp;	// 响应时间
            int64_t delay;				// 延时
            int64_t exception_count;	// 发生异常的次数
            
            
        };
        
        
        
    public:
        typedef std::priority_queue<runnable, std::vector<runnable>, std::greater<runnable> >
        handler;
        typedef amo::chrono::time_point<amo::chrono::system_clock> time_point;
        typedef amo::function<void()> executor;
        typedef std::function<bool(int64_t, const std::string&)>
        exception_callback_type;
        
        
        
    public:
        const static uint64_t empty_queue = 4070880000000;
    public:
        looper_executor(int buffer_size = 100, bool exit_at_exception = true)
            : m_buffer_size(buffer_size),
              m_exit_at_exception(exit_at_exception) {
            m_running = false;
            m_runnable_index = 0;
            m_id_number = -1;
            
            std::tm timeinfo = std::tm();
            timeinfo.tm_year = 199;												//!< year: 2099
            timeinfo.tm_mon = 0;												//!< month: january
            timeinfo.tm_mday = 1;												//!< day: 1st
            std::time_t tt = std::mktime(&timeinfo);
            m_empty_queue_time = amo::chrono::system_clock::from_time_t(tt);
            
            m_last_response_test_result.delay = 0xffffffff;
            start();
        }
        
        ~looper_executor() {
            stop();
        }
        
        void check_if_called_on_valid_thread() {
            if (!check_on_looper_thread()) {
                throw std::runtime_error("method is not called on valid thread");
            }
        }
        
        void check_if_called_on_invalid_thread() {
            if (check_on_looper_thread()) {
                throw std::runtime_error("method is not called on valid thread");
            }
        }
        
        bool check_on_looper_thread() {
            return (amo::this_thread::get_id() == m_thread_id);
        }
        
        void push(executor runnable_, uint64_t delay = 0) {
            return execute(runnable_, delay);
        }
        
        void push(executor runnable_, time_point execute_time) {
            return execute(runnable_, execute_time);
        }
        
        void execute(executor runnable_, uint64_t delay = 0) {
            time_point execute_time = amo::chrono::system_clock::now() +
                                      amo::chrono::milliseconds(delay);
                                      
            if (delay == empty_queue) {
                time_point execute_time = m_empty_queue_time;
            }
            
            return execute(runnable_, execute_time);
        }
        
        void execute(executor runnable_, time_point execute_time) {
            amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
            
            if (!m_running) {
                return;
            }
            
            if (check_on_looper_thread() &&
                    execute_time <= amo::chrono::system_clock::now()) {
                lock.unlock();
                return runnable_();
            }
            
            m_not_full.wait(lock, amo::bind(&looper_executor::is_not_full, this,
                                            m_buffer_size));
                                            
            if (execute_time == m_empty_queue_time) {
                m_is_empty.wait(lock, amo::bind(&looper_executor::is_empty, this));
                execute_time = amo::chrono::system_clock::now();
            }
            
            if (!m_running) {
                return;
            }
            
            m_handler.push(runnable(runnable_, m_runnable_index++, execute_time));
            m_not_empty.notify_all();
        }
        
        void clear() {
            amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
            
            while (!m_handler.empty()) {
                m_handler.pop();
            }
        }
        
        void response_test(int64_t test_id,
                           std::function < void(thread_response_info)> fn = {}) {
            thread_response_info info;
            info.test_id = test_id;
            info.thread_name = get_name();
            info.thread_id = get_id_number();
            execute(std::bind(&looper_executor::on_response_test, this, info, fn));
        }
        
        
        
        void run() {
            m_thread_id = amo::this_thread::get_id();
            std::stringstream stream;
            stream << m_thread_id;
            stream >> m_id_number;
            m_last_response_test_result.thread_id = m_id_number;
            
            try {
                while (m_running) {
                
                    amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
                    m_not_empty.wait(lock, amo::bind(&looper_executor::is_not_empty, this));
                    
                    if (!m_running) {
                        return;
                    }
                    
                    runnable runnable_ = m_handler.top();
                    
                    while (true) {
                        uint64_t delay = 0;
                        
                        runnable_ = m_handler.top();
                        uint64_t now = amo::chrono::duration_cast<amo::chrono::milliseconds>
                                       (amo::chrono::system_clock::now().time_since_epoch()).count();
                        time_point execute_time_point = runnable_;
                        uint64_t execute_time = amo::chrono::duration_cast<amo::chrono::milliseconds>
                                                (execute_time_point.time_since_epoch()).count();
                                                
                                                
                        if (now < execute_time) {
                            delay = execute_time - now;
                            amo::cv_status status = m_correct.wait_for(lock,
                                                    amo::chrono::milliseconds(1));
                                                    
                            continue;
                        }
                        
                        
                        m_handler.pop();
                        m_not_full.notify_all();
                        break;
                        
                    }
                    
                    lock.unlock();
                    
                    if (!m_exit_at_exception) {
                        try {
                            runnable_();
                        } catch (const std::exception& e) {
                            retport_exception(e.what());
                        }
                    } else {
                        runnable_();
                    }
                    
                    if (! m_mutex.try_lock()) {
                        continue;
                    }
                    
                    if (m_handler.empty()) {
                        m_is_empty.notify_all();
                    }
                    
                    m_mutex.unlock();
                    
                    
                }
            } catch (std::exception& e) {
                retport_exception(e.what());
                
                $cfatal("The thread has a serious error and will quit.");
				$cfatal("The thread has a serious error and will quit.");
				$cfatal("The thread has a serious error and will quit.");
            }
        }
        
        exception_callback_type get_exception_callback()  const {
            return m_fn_exception_cb;
        }
        
        void set_exception_callback(exception_callback_type  val) {
            m_fn_exception_cb = val;
            
        }
        void set_name(const std::string& thread_name) {
            m_thread_name = thread_name;
            m_last_response_test_result.thread_name = m_thread_name;
        }
        
        thread_response_info get_last_response_test_result() const {
            return m_last_response_test_result;
        }
        
        std::string get_name() const {
            return m_thread_name;
        }
        
        amo::thread::id get_id() const {
            return m_thread_id;
        }
        
        int get_id_number() const {
            return m_id_number;
        }
        size_t size() const {
            return m_handler.size();
        }
        
        int64_t get_exceptions() const {
            return m_exception_count;
        }
        
        void wait_for_empty() {
            if (is_empty()) {
                return;
            }
            
            amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
            m_is_empty.wait(lock, amo::bind(&looper_executor::is_empty, this));
            return;
        }
        
        void start() {
            amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
            
            if (m_running) {
                return;
            }
            
            m_running = true;
            clear();
            m_work_thread = amo::thread(amo::bind(&looper_executor::run, this));
        }
        
        void stop() {
            amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
            
            if (!m_running) {
                return;
            }
            
            m_running = false;
            m_not_empty.notify_all();
            m_not_full.notify_all();
            lock.unlock();
            
            // 向线程发送中断请求
#ifdef BOOST_NO_CXX11_HDR_THREAD
            m_work_thread.interrupt();
#endif // BOOST_NO_CXX11_HDR_THREAD
            
            //join函数，作用是等待直到线程执行结束；可不加，但不能保证退出Destroy函数前线程被终结
            if (m_work_thread.joinable()) {
                m_work_thread.join();
            }
        }
        
        void kill() {
        
            amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
            
            if (!m_handler.empty()) {
                // notify_all 可能会有死锁，为啥？
                $cdevel("maybe fatal error-->{}", func_orient);
                $cdevel("maybe fatal error-->{}", func_orient);
                $cdevel("maybe fatal error-->{}", func_orient);
                m_not_empty.notify_all();    // 会死锁
            }
            
            m_not_full.notify_all();
            lock.unlock();
            $windows({
                std::thread::native_handle_type handle = m_work_thread.native_handle();
                TerminateThread(handle, -1);
                m_running = false;
                
                if (m_work_thread.joinable()) {
                    m_work_thread.join();
                }
            })
        }
    private:
    
        void on_response_test(thread_response_info info,
                              std::function < void(thread_response_info)> fn_cb) {
            amo::chrono::time_point<amo::chrono::system_clock> now =
                amo::chrono::system_clock::now();
            info.response_timestamp = amo::chrono::duration_cast<amo::chrono::milliseconds>
                                      (now.time_since_epoch()).count();
            info.delay = info.response_timestamp - info.request_timestamp;
            info.exception_count = m_exception_count;
            
            
            if (fn_cb) {
                fn_cb(info);
            }
            
            m_last_response_test_result = info;
        }
        
        bool retport_exception(const std::string& error_msg) {
            amo::cfatal(func_orient + "[thread ID:{0}][thread name :{1}], execption:{2}",
                        m_thread_id,
                        m_thread_name,
                        error_msg);
            m_exception_count++;
            
            if (get_exception_callback()) {
                return m_fn_exception_cb(m_id_number, error_msg);
            }
            
            return true;
            
            
        }
        bool is_not_full(int64_t buffer_size = 100) {
            amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
            return (m_handler.size() < buffer_size) || !m_running ;
        }
        
        bool is_not_empty() {
            amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
            return (m_handler.size() > 0) || !m_running;
        }
        
        bool is_empty() {
            amo::unique_lock<amo::recursive_mutex> lock(m_mutex);
            return (m_handler.empty()) || !m_running;
        }
        
        
    private:
        amo::recursive_mutex  m_mutex;
        amo::condition_variable_any m_not_empty;
        amo::condition_variable_any m_is_empty;
        amo::condition_variable_any m_not_full;
        amo::condition_variable_any m_correct;
        handler m_handler;
        amo::atomic<bool> m_running;
        amo::thread m_work_thread;
        amo::thread::id m_thread_id;
        int64_t m_id_number;
        uint64_t m_runnable_index;												// 插入索引，如果两个任务的执行时间相同，会根据任务插入的先后顺序执行
        int m_buffer_size;
        std::string m_thread_name;
        time_point m_empty_queue_time;											//!< The empty queue time
        exception_callback_type  m_fn_exception_cb;				//!< 线程抛出异常时回调
        bool m_exit_at_exception;// 是否在发生异常时退出线程，默认为true
        int64_t m_exception_count;// 线程触发异常的次数
        
        thread_response_info m_last_response_test_result;
    };
    
    
    
    
}


#endif //AMO_LOOPER_EXECUTOR_01A79A7C_FD15_4400_980C_4512FD69CB94_HPP__