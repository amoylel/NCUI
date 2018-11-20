// Created by amoylel on 31/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_DAEMON_7352DE05_B96B_423D_A86F_0F11EF7C12CB_HPP__
#define AMO_DAEMON_7352DE05_B96B_423D_A86F_0F11EF7C12CB_HPP__



#include <exception>
#include <thread>
#include <sstream>
#include <chrono>

#include <amo/config.hpp>
#include <amo/path.hpp>
#include <amo/logger.hpp>
#include <amo/utility.hpp>
#include <amo/process.hpp>

#include <amo/timer.hpp>
#include <amo/app.hpp>
#include <amo/date_time.hpp>
#include <amo/json_helper.hpp>

#include <amo/managed_shared_memory.hpp>
#include <amo/file_mapping.hpp>
#include <amo/md5.hpp>


namespace amo {

    class daemon : log_object {
    public:
        class processinfo : public amo::u8json_object {
        public:
            processinfo() {
                timestamp = amo::timer::now();
                delay = 60000;
                killMode = 2;
                interval = 20000;
                
                
            }
            
            bool need_restart_process() const {
                int64_t now = amo::timer::now();
                /*
                  $cdevel("last report time:{} --> current time:{}  diff:{}",
                          amo::date_time(timestamp).to_string(),
                          amo::date_time(now).to_string(),
                          now - timestamp);
                  $cdevel("last report timestamp:{} --> current timestamp:{}  diff:{}",
                          (timestamp),
                          (now),
                          now - timestamp);*/
                return now - timestamp > delay;
            }
            
            bool unresponsive(const int64_t& lasttimestamp) const {
                /*    $cdevel("last detect time:{}  --> last report time:{}",
                            amo::date_time(lasttimestamp).to_string(),
                            amo::date_time(timestamp).to_string());
                
                    $cdevel("last detect timestamp:{}  --> last report timestamp:{}",
                            (lasttimestamp),
                            (timestamp));*/
                
                return lasttimestamp >= timestamp;
            }
            
            AMO_ENTITY_ARGS_GET_BEGIN(processinfo)
            AMO_ENTITY_ARGS_GET(appName)
            AMO_ENTITY_ARGS_GET(fullAppName)
            AMO_ENTITY_ARGS_VEC_GET(pid)
            AMO_ENTITY_ARGS_GET(timestamp)
            AMO_ENTITY_ARGS_GET(delay)
            AMO_ENTITY_ARGS_GET(killMode)
            AMO_ENTITY_ARGS_GET(interval)
            AMO_ENTITY_ARGS_GET_END()
            
            AMO_ENTITY_ARGS_SET_BEGIN(processinfo)
            AMO_ENTITY_ARGS_SET(appName)
            AMO_ENTITY_ARGS_SET(fullAppName)
            AMO_ENTITY_ARGS_VEC_SET(pid)
            AMO_ENTITY_ARGS_SET(timestamp)
            AMO_ENTITY_ARGS_SET(delay)
            AMO_ENTITY_ARGS_SET(killMode)
            AMO_ENTITY_ARGS_SET(interval)
            AMO_ENTITY_ARGS_SET_END()
            
        public:
            /** @var #std::string fullname	程序完整路径. */
            std::string fullAppName;
            /** @var #std::string name	程序名（短路径）. */
            std::string appName;
            
            /** @var #Array	 pid 所有相关的进程pid. */
            std::vector<int64_t> pid;
            
            /** @var #int64_t=now()	上次响应的时间,默认为程序启动时间. */
            int64_t timestamp;
            /** @var #int64_t=60000	delay 杀死进程的延时时间. */
            int64_t delay;
            /** @var #int32_t=2 kill_mode	终止模式   1 pid only 2 name only 3 所有的方式都试一下 , 默认为2. */
            int32_t killMode;
            /** @var #int32_t=20000 interval	检测间隔时间， 默认为10秒检测一次. */
            int32_t interval;
        };
        
        
        
        
    public:
    
        daemon(const amo::u8json& settings) {
        
            m_app_info = processinfo::fromJson(settings);
            init();
            assert(m_app_info);
        }
        
        daemon(std::shared_ptr<processinfo> settings) : m_app_info(settings) {
            init();
            assert(m_app_info);
        }
        
        ~daemon() {
            segment_writter.reset();
            segment_reader.reset();
        }
        
        void init() {
            m_is_running = false;
            m_last_response_state = true;
            auto_update_my_state(true);
            m_last_timestamp = amo::date_time::now();
            
            m_my_app_info.reset(new amo::daemon::processinfo());
            m_my_app_info->appName =
                amo::u8path::appName().remove_front_backslash().to_string();
            m_my_app_info->fullAppName = amo::u8path::fullAppName().to_linux_string();
            m_my_app_info->pid.push_back(amo::process::get_current_process_id());
            
            // 使用对方提供的参数
            m_my_app_info->interval = m_app_info->interval;
            m_my_app_info->delay = m_app_info->delay;
            
            assert(m_my_app_info);
            
            std::string reader = amo::md5::encode(m_app_info->appName);
            std::string writter = amo::md5::encode(m_my_app_info->appName);
            
            segment_writter.reset(new amo::file_mapping(amo::string(writter, false)));
            segment_reader.reset(new amo::file_mapping(amo::string(reader, false)));
            
            assert(segment_reader);
            assert(segment_writter);
            
        }
        
        // 判断是否更新自身状态
        bool auto_update_my_state() const {
            return m_auto_update_my_state;
        }
        
        
        // 设置是否自动更新自身状态
        void auto_update_my_state(bool val) {
            m_auto_update_my_state = val;
        }
        
        // 开启服务
        bool start_service() {
        
            if (m_is_running) {
                return true;
            }
            
            if (!m_executor) {
                m_executor.reset(new amo::looper_executor());
            }
            
            m_executor->execute(std::bind(&daemon::run_service, this));
            return true;
        }
        
        // 运行服务，会阻塞当前线程
        bool run_service() {
            if (m_is_running) {
                return true;
            }
            
            m_is_running = true;
            m_last_timestamp = amo::date_time::now();
            m_last_response_state = true;
            return run();
        }
        
        
        // 停止服务, 不可在m_executor上执行
        bool stop_service() {
            if (!m_is_running) {
                return true;
            }
            
            
            m_is_running = false;
            
            if (m_executor) {
                m_executor->wait_for_empty();
            }
            
            return true;
            
        }
        
        // 更新当前程序的状态
        virtual bool update_my_state() {
            if (!segment_writter) {
                return false;
            }
            
            m_my_app_info->timestamp = amo::date_time::now();
            /*      $cdevel("current timestamp :\n{}", amo::date_time::now());
                  $cdevel("update my timestamp :\n{}", m_my_app_info->timestamp);
                  $cdevel("update my state :\n{}", m_my_app_info->toString());*/
            std::string state = m_my_app_info->toString();
            std::vector<char> vec(1000, 0);
            memcpy(vec.data(), state.c_str(), state.size());
            return  segment_writter->write(vec.data(), 0, vec.size());
            
        }
        
        
        // 读取被守护程序的状态
        virtual bool read_app_state() {
            if (!segment_reader) {
                return false;
            }
            
            std::vector<char> vec(1000, 0);
            segment_reader->read(vec.data(), 0, vec.size());
            
            std::string str(vec.data());
            
            if (str.empty()) {
                $cerr("read error: empty state");
                return false;
            }
            
            amo::u8json json(str);
            //$cdevel("read app state :\n{}", json.to_string());
            
            if (!json.is_valid()) {
                return false;
            }
            
            m_app_info->joinJson(json);
            
            return true;
            
        }
        
        virtual bool clear_app_state() {
            m_app_info->pid.clear();
            std::string state = m_app_info->toString();
            std::vector<char> vec(1000, 0);
            memcpy(vec.data(), state.c_str(), state.size());
            return  segment_reader->write(vec.data(), 0, vec.size());
        }
        
        int64_t get_last_test_timestamp() const {
            return m_last_timestamp;
        }
        void set_last_test_timestamp(int64_t val) {
            m_last_timestamp = val;
        }
        
        std::shared_ptr<processinfo> get_processinfo() {
            return m_app_info;
        }
        
    protected:
        // 杀掉被守护的进程
        virtual bool kill_app() {
        
            if ((m_app_info->killMode & 0x1) != 0) {
                kill_app_by_pid();
            }
            
            
            if ((m_app_info->killMode & 0x2) != 0) {
                kill_app_by_name();
            }
            
            // 判断有没有杀干净
            //amo::process::find_pid_by_process_name(m_app_info->appName);
            return true;
        }
        
        // 通过进程名杀进程
        virtual bool kill_app_by_name() {
            return amo::process::kill_process_by_name(m_app_info->appName, true);
        }
        
        // 通过进程ID杀进程
        virtual bool kill_app_by_pid() {
            if (!m_app_info->pid.empty()) {
                bool bOk = true;
                
                for (auto& p : m_app_info->pid) {
                    if (!amo::process::kill_process_by_pid(p)) {
                        bOk = false;
                    }
                }
                
                return bOk;
            } else {
                return false;
            }
        }
        
        // 挂起线程，直到下一次检测程序状态
        void sleep_for_next_time() {
            int64_t timestamp = amo::timer::now() + m_app_info->interval;
            
            while (amo::timer::now() < timestamp) {
                if (!m_is_running) {
                    break;
                }
                
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                
            }
            
        }
        
        bool run() {
            try {
            
                // 该程序只允许运行一个实例，如果有多个实例运行将会在初始化日志的时候报异常退出
                
                bool response = true;
                
                update_my_state();
                
                while (true) {
                    sleep_for_next_time();
                    
                    if (!m_is_running) {
                        return true;
                    }
                    
                    if (auto_update_my_state()) {
                        update_my_state();
                    }
                    
                    // 读取对方状态
                    read_app_state();
                    
                    response = !m_app_info->unresponsive(m_last_timestamp);
                    
                    m_last_timestamp = m_app_info->timestamp;
                    
                    if (response && !m_last_response_state) {
                        $cdevel("[{}] restored response", m_app_info->appName);
                    }
                    
                    if (!response) {
                        $cwarn("[{}] no response", m_app_info->appName);
                    }
                    
                    m_last_response_state = response;
                    
                    if (m_app_info->need_restart_process()) {
                        $cdevel("restart app:{}",
                                amo::u8string(m_app_info->toString(), true).to_locale());
                                
                        kill_app();
                        clear_app_state();
                        
                        amo::u8path p(m_app_info->fullAppName);
                        
                        if (p.empty()) {
                            p =  m_app_info->appName;
                            p.add_front_backslash();
                        }
                        
                        if (p.empty()) {
                            $cerr("restart error, empty path: {}", p.c_str());
                            //$cdevel("启动参数:{}", m_app_info->toString());
                            continue;
                        }
                        
                        if (!p.exists()) {
                            $cerr("restart error, path is not exists:{}", amo::u8string(p.c_str(),
                                    true).to_locale());
                            continue;
                        }
                        
                        //$cdevel("启动路径: {}", p.c_str());
                        //$cdevel("开始:{}", amo::date_time().to_string());
                        bool nRetval = amo::app(amo::u8string(p.c_str(), true)).newInstance(3);
                        //$cdevel("完成:{} 返回值:{}", amo::date_time().to_string(), nRetval);
                        
                        
                        $cerr("start [{}] result:{}", m_app_info->appName, nRetval);
                        
                        //重置状态
                        m_last_timestamp = amo::timer::now();
                        m_app_info->timestamp = amo::timer::now();
                        
                    }
                    
                }
                
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                return false;
            }
            
            return true;
        }
    protected:
    
        bool m_last_response_state;		// 上一次测试时是否响应
        std::shared_ptr<processinfo> m_app_info;
        std::shared_ptr<processinfo> m_my_app_info;
        std::shared_ptr<amo::file_mapping> segment_writter;
        std::shared_ptr<amo::file_mapping> segment_reader;
        int64_t m_last_timestamp;  // 上一次测试响应状态的时间戳
        std::atomic<bool> m_is_running;
        std::shared_ptr<amo::looper_executor> m_executor;
        bool m_auto_update_my_state; // 是否自动更新自身状态，默认为true
    };
    
}


#endif //AMO_DAEMON_7352DE05_B96B_423D_A86F_0F11EF7C12CB_HPP__