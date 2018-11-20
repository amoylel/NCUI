// Created by amoylel on 20/07/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_FILEWATCH_9A8D1E8F_016D_4F94_8951_CE7100226DCE_HPP__
#define AMO_FILEWATCH_9A8D1E8F_016D_4F94_8951_CE7100226DCE_HPP__

#include <functional>
#include <memory>
#include <string>
#include <mutex>
#include <set>
#include <unordered_map>

#include <amo/filewatch_t.hpp>
#include <amo/timer.hpp>
#include <amo/path.hpp>
#include <amo/looper_executor.hpp>
#include <amo/md5.hpp>

namespace amo {
    class filewatch : public std::enable_shared_from_this<filewatch> {
    public:
        typedef std::function<void(std::shared_ptr<filewatch>, const std::string&, const amo::Event&)>
        event_callback_type;
        
        class event_info {
        public:
            event_info() {
                event_time = amo::timer::now();
                event_type = -1;
            }
            
            int64_t event_time;		// 事件触发时间
            int event_type;			// 事件类型
            std::string filename;	// 文件
        };
        
    public:
        filewatch(const std::string& dir_, bool master_ = false) : dir(dir_) {
            set_master(master_);
            delay = 0;
            
            nCount = 0;
            nCount2 = 0;
        }
        
        int nCount;
        int nCount2;
        bool is_master() const {
            return master;
        }
        
        void set_master(bool val) {
            master = val;
        }
        
        std::string get_root_dir() const {
            return dir;
        }
        void set_root_dir(std::string val) {
            dir = val;
        }
        
        std::string get_rename_old_name() const {
            return rename_old_name;
        }
        void set_rename_old_name(std::string val) {
            rename_old_name = val;
        }
        
        amo::path make_file_path(const std::string& filename) {
            amo::path p(dir);
            return p.append(filename).absolute().canonicalize(false);
        }
        
        int64_t get_time_delayed() const {
            return delay;
        }
        
        
        void set_time_delayed(int64_t val) {
            if (val < 0) {
                val = 0;
            }
            
            delay = val;
        }
        
        amo::filewatch::event_callback_type get_event_callback() const {
            return m_fn_callback;
        }
        
        void set_event_callback(amo::filewatch::event_callback_type val) {
            m_fn_callback = val;
        }
        
        
        void start_watch(const std::string reg = "") {
        
            if (!m_mutex) {
                m_mutex.reset(new std::recursive_mutex());
            }
            
            if (!m_executor) {
                m_executor.reset(new amo::looper_executor(10000));
            }
            
            if (reg.empty()) {
                watcher.reset(new amo::filewatch_t<std::string>(dir,
                              std::bind(&filewatch::on_file_change,
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2)));
            } else {
                watcher.reset(new amo::filewatch_t<std::string>(dir,
                              std::regex(reg),
                              std::bind(&filewatch::on_file_change,
                                        this,
                                        std::placeholders::_1,
                                        std::placeholders::_2)));
            }
            
        }
        
        void stop_watch() {
            watcher.reset();
        }
        std::string compute_md5(const std::string& filename) {
            auto src = this->make_file_path(filename);
            std::string srcMD5 = amo::md5::encode_file(src);
            return srcMD5;
        }
        
        
        std::string  get_file_md5(const std::string& filename) {
            auto iter = m_md5_map.find(filename);
            
            if (iter == m_md5_map.end()) {
                return amo::md5::invalid_md5();
            }
            
            return iter->second;
        }
        
        void set_file_md5(const std::string& filename,
                          const std::string& str_md5 = "") {
            std::unique_lock<std::recursive_mutex> lock1(*m_mutex);
            
            if (filename.empty()) {
                return;
            }
            
            if (str_md5.empty()) {
                m_md5_map.erase(filename);
            } else {
                m_md5_map[filename] = str_md5;
            }
        }
        
        
        
        // 判断文件是否发生更改
        bool is_file_changed(const std::string& filename) {
            amo::path p = make_file_path(filename);
            
            // 如果是增加文件夹，那么默认为发生改变
            if (p.exists() && p.is_directory()) {
                return true;
            }
            
            std::string str_md5 = compute_md5(filename);
            
            if (str_md5.empty() || str_md5 == amo::md5::invalid_md5()) {
            
                set_file_md5(filename);
                return true;
            }
            
            if (str_md5 == get_file_md5(filename)) {
            
            
                return false;
            }
            
            set_file_md5(filename, str_md5);
            
            return true;
        }
        
        void set_mutex(std::shared_ptr< std::recursive_mutex> mutex_) {
            m_mutex = mutex_;
        }
        
        
        void set_looper_executor(std::shared_ptr<amo::looper_executor> executor_) {
            m_executor = executor_;
        }
        
        void set_rules(const std::set<std::string>& rules, bool valid = true) {
            if (valid) {
                valid_rules = rules;
            } else {
                invalid_rules = rules;
            }
        }
        
        void add_ignore_event(const std::string& filename, const amo::Event change_type,
                              int64_t delay = 1000) {
            std::string event_name = make_event_name(filename, change_type);
            event_info info;
            info.event_time = amo:: timer::now() + delay;
            info.event_type = (int)change_type;
            info.filename = filename;
            m_ignore_events_map.insert({ event_name, info });
        }
        
        
        std::string make_event_name(const std::string& filename,
                                    const amo::Event change_type) const {
            std::stringstream stream;
            stream << (int)change_type;
            stream << ":";
            stream << filename;
            std::string event_name = stream.str();
            return event_name;
        }
        
    protected:
        void on_file_change(const std::string& filename, const amo::Event change_type) {
        
            // 匹配排除规则
            for (auto& p : invalid_rules) {
                if (std::regex_match(filename, std::regex(p))) {
                    return;
                }
            }
            
            
            
            
            bool valid_filename = valid_rules.empty() ? true : false;
            
            
            for (auto& p : valid_rules) {
                if (std::regex_match(filename, std::regex(p))) {
                    valid_filename = true;
                    break;
                }
            }
            
            if (!valid_filename) {
                return;
            }
            
            
            
            // 不响应文件夹属性被修改的消息
            if (make_file_path(filename).exists() &&
                    make_file_path(filename).is_directory() &&
                    change_type == amo::Event::modified) {
                return;
            }
            
            
            /*  amo::cdevel << amo::timer::now() << "\t" << make_file_path(
            filename).c_str() << "\t" <<
            (int)change_type << amo::endl;*/
            
            
            if (delay > 0) {
                std::unique_lock<std::recursive_mutex> lock1(*m_mutex);
                
                std::string event_name = make_event_name(filename, change_type);
                auto iter = m_events_map.find(event_name);
                
                if (iter != m_events_map.end()) {
                
                    iter->second.event_time = amo::timer::now() + delay;
                    
                    /* $cdevel("更新事件触发时间[{}]:{} ---> {}",
                    (int)change_type,
                    make_file_path(filename).to_string(),
                    amo::date_time(iter->second.event_time).format("yyyy-MM-dd hh:mm:ss.ms"));*/
                } else {
                    event_info info;
                    info.event_time = amo::timer::now() + delay;
                    info.event_type = (int)change_type;
                    info.filename = filename;
                    m_events_map[event_name] = info;
                    
                    /*  $cdevel("设置事件触发时间[{}]:{} ---> {}",
                    (int) change_type,
                    make_file_path(filename).to_string(),
                    amo::date_time(info.event_time).format("yyyy-MM-dd hh:mm:ss.ms"));*/
                }
                
                lock1.unlock();
                m_executor->execute(std::bind(&filewatch::on_file_change_with_delay, this,
                                              event_name, filename, change_type), delay + 10);
            } else {
                m_executor->execute(std::bind(&filewatch::on_file_change_at_now, this,
                                              filename, change_type));
            }
            
            
            
        }
        
        void on_file_change_at_now(const std::string& filename,
                                   const amo::Event change_type) {
                                   
            if (change_type == amo::Event::renamed_old) {
                set_rename_old_name(filename);
                return;
            }
            
            if (m_fn_callback) {
                m_fn_callback(shared_from_this(), filename, change_type);
            }
            
            if (change_type == amo::Event::renamed_new) {
                set_rename_old_name("");
            }
        }
        
        
        void on_file_change_with_delay(const std::string& event_name,
                                       const std::string& filename,
                                       const amo::Event change_type) {
            std::unique_lock<std::recursive_mutex> lock1(*m_mutex);
            
            
            
            
            auto iter = m_events_map.find(event_name);
            
            if (iter == m_events_map.end()) {
                /*  $cdevel("没有找到对应的事件[{}]:{}",
                (int)change_type,
                make_file_path(filename).to_string());*/
                return;
            }
            
            if (iter->second.event_time > amo::timer::now()) {
                // 时间被改了，没有到时间
                
                /* $cdevel("时间未到延时触发[{}]:{}，当前时间:{}，延时时间:{}",
                (int)change_type,
                make_file_path(filename).to_string(),
                amo::date_time().format("yyyy-MM-dd hh:mm:ss.ms"),
                amo::date_time(iter->second.event_time).format("yyyy-MM-dd hh:mm:ss.ms"));*/
                return;
            }
            
            /* $cdevel("时间已到，调用回调函数触发事件[{}]:{}",
            (int)change_type,
            make_file_path(filename).to_string());*/
            
            do {
            
                // 移除超时规则
                int64_t now = amo::timer::now();
                
                for (auto iter = m_ignore_events_map.begin(); iter != m_ignore_events_map.end();
                    ) {
                    if (iter->second.event_time < now) {
                        iter = m_ignore_events_map.erase(iter);
                    } else {
                        ++iter;
                    }
                }
                
                
                if (change_type == amo::Event::added || change_type == amo::Event::removed) {
                    auto iter = m_ignore_events_map.find(event_name);
                    
                    if (iter != m_ignore_events_map.end()) {
                        // 如果还在忽略列表中，那么直接返回
                        return;
                    }
                    
                }
                
                if (change_type == amo::Event::added) {
                    if (!is_file_changed(filename)) {
                        //$cdevel("文件内容未发生改变，无需触发事件");
                        break;
                    }
                }
                
                
                // 不响应文件夹的modify事件
                if (change_type == amo::Event::modified) {
                    amo::path p = make_file_path(filename);
                    
                    if (!p.exists()) {
                        break;
                    }
                    
                    if (p.is_directory()) {
                        break;
                    }
                    
                    if (!is_file_changed(filename)) {
                        //$cdevel("文件内容未发生改变，无需触发事件");
                        break;
                    }
                    
                }
                
                
                if (change_type == amo::Event::added) {
                    ++nCount;
                    //$cdevel("触发事件[{}]:{}", make_file_path(filename).to_string(), nCount);
                    
                }
                
                if (change_type == amo::Event::renamed_old || change_type == Event::removed) {
                    // 清理原有的MD5
                    set_file_md5(filename);
                    
                    for (auto iter = m_md5_map.begin(); iter != m_md5_map.end();) {
                        // 假设 filename是一个文件夹。
                        if (amo::string_utils::begin_with(iter->first, filename + "/")
                                || amo::string_utils::begin_with(iter->first, filename + "\\")) {
                            iter = m_md5_map.erase(iter);
                        } else {
                            ++iter;
                        }
                    }
                    
                }
                
                
                if (change_type == amo::Event::renamed_old) {
                    set_rename_old_name(filename);
                    return;
                }
                
                if (change_type == amo::Event::renamed_new) {
                    // 如果改名相同，那么直接返回
                    if (get_rename_old_name() == filename) {
                        set_rename_old_name("");
                        return;
                    }
                    
                    
                }
                
                if (m_fn_callback) {
                    m_fn_callback(shared_from_this(), filename, change_type);
                }
                
                if (change_type == amo::Event::renamed_new) {
                    set_rename_old_name("");
                }
            } while (false);
            
            
            m_events_map.erase(event_name);
        }
        
        
        
        
    private:
        std::shared_ptr<amo::filewatch_t<std::string> > watcher;
        
        bool master;		// 是否主master节点
        
        event_callback_type  m_fn_callback;// 事件回调函数
        
        std::string dir;	// 被监听的根目录
        
        std::string rename_old_name;
        
        std::shared_ptr< std::recursive_mutex> m_mutex;
        
        std::shared_ptr<amo::looper_executor> m_executor;
        
        int64_t delay;
        
        std::unordered_map<std::string, event_info> m_events_map;
        
        std::unordered_map<std::string, std::string> m_md5_map;
        
        std::set<std::string> valid_rules;
        
        std::set<std::string> invalid_rules;
        
        // 被忽略的事件
        std::unordered_map<std::string, event_info> m_ignore_events_map;
    };
    
}



#endif //AMO_FILEWATCH_9A8D1E8F_016D_4F94_8951_CE7100226DCE_HPP__