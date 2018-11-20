// Created by amoylel on 18/07/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef ADOC_FILESYNC_FE9F9E29_5110_4796_8DF1_EABDE7F4EC94_HPP__
#define ADOC_FILESYNC_FE9F9E29_5110_4796_8DF1_EABDE7F4EC94_HPP__

#include <memory>
#include <string>
#include <set>
#include <unordered_map>

#include <amo/looper_executor.hpp>
#include <amo/logger.hpp>
#include <amo/filewatch.hpp>
#include <amo/json_helper.hpp>


namespace amo {

    class filesync {
    public:
        class settings : public json_object {
        public:
            settings() {
                is_utf8 = false;
                mode = 0;
            }
            AMO_ENTITY_ARGS_GET_BEGIN(settings)
            AMO_ENTITY_ARGS_VEC_GET(dirs)
            AMO_ENTITY_ARGS_VEC_GET(exclude_rules)
            AMO_ENTITY_ARGS_VEC_GET(rules)
            AMO_ENTITY_ARGS_GET(is_utf8)
            AMO_ENTITY_ARGS_GET(master)
            AMO_ENTITY_ARGS_GET(mode)
            AMO_ENTITY_ARGS_GET_END()
            
            AMO_ENTITY_ARGS_SET_BEGIN(settings)
            AMO_ENTITY_ARGS_VEC_SET(dirs)
            AMO_ENTITY_ARGS_VEC_SET(exclude_rules)
            AMO_ENTITY_ARGS_VEC_SET(rules)
            AMO_ENTITY_ARGS_SET(master)
            AMO_ENTITY_ARGS_SET(is_utf8)
            AMO_ENTITY_ARGS_SET(mode)
            AMO_ENTITY_ARGS_SET_END()
        public:
            std::vector<std::string> dirs;				// 需要同步的文件夹列表
            std::vector< std::string> exclude_rules;	// 被排除的规则
            std::vector< std::string> rules;			// 匹配的规则
            std::string master;							// 主文件夹
            bool is_utf8;								// 配置文件是否为utf8编码
            int mode;									// 同步模式 0 同步文件夹 1 暴力合并文件夹
        };
    public:
        filesync() {
            init();
        }
        
        filesync(std::shared_ptr<amo::filesync::settings> settings_) {
            m_settings = settings_;
            init();
        }
        
        filesync(const amo::json& json) {
            m_settings = amo::filesync::settings::fromJson(json);
            init();
        }
        
        
        void init() {
            m_mutex.reset(new std::recursive_mutex());
            m_executor.reset(new amo::looper_executor(10000));
            
            if (!m_settings) {
                return;
            }
            
            for (auto& p : m_settings->dirs) {
                add_dir(p);
            }
            
            if (!m_settings->master.empty()) {
                set_master_watcher(m_settings->master);
            }
            
            for (auto& p : m_settings->exclude_rules) {
                add_rules(p, false);
            }
            
            
            
            for (auto& p : m_settings->rules) {
                add_rules(p, true);
            }
            
        }
        
        
        std::string to_watcher_key(const std::string& str) {
            amo::path p(str);
            p.absolute();
            p.canonicalize(false);
            return p.to_string();
        }
        
        void add_rules(const std::string& rules, bool valid = true) {
            if (valid) {
                valid_rules.insert(rules);
            } else {
                invalid_rules.insert(rules);
            }
        }
        
        bool add_dir(const std::string& str, bool master_ = false) {
        
            // 如果master不存在，那么设置一个
            if (master_name.empty()) {
                master_name = str;
            }
            
            if (master_) {
                master_name = str;
            }
            
            std::string dir = to_watcher_key(str);
            amo::path p(dir);
            
            // 目录之间不能互为子目录
            
            if (!p.create_directory() || !p.is_directory()) {
                return false;
            }
            
            
            auto iter = m_watchers.find(dir);
            
            if (iter == m_watchers.end()) {
            
                std::shared_ptr<amo::filewatch> watcher;
                watcher.reset(new amo::filewatch(dir, master_));
                watcher->set_time_delayed(200);
                watcher->set_event_callback(std::bind(&filesync::on_file_change,
                                                      this,
                                                      std::placeholders::_1,
                                                      std::placeholders::_2,
                                                      std::placeholders::_3));
                m_watchers[dir] = watcher;
                return true;
            } else {
                return false;
            }
        }
        
        void remove_dir(const std::string& str) {
            m_watchers.erase(to_watcher_key(str));
        }
        
        // 合并差异
        void merge() {
            auto watcher = get_master_watcher();
            
            if (!watcher) {
                return;
            }
            
            amo::path src(watcher->get_root_dir());
            
            for (auto& p : m_watchers) {
                if (p.second == watcher) {
                    continue;
                }
                
                amo::path dst(p.second->get_root_dir());
                dst.remove_all();
                src.copy_to(dst);
            }
        }
        
        
        
        // 设置主节点
        void set_master_watcher(const std::string& name) {
        
        
            auto iter = m_watchers.find(to_watcher_key(name));
            
            if (iter == m_watchers.end()) {
                return;
            }
            
            for (auto& p : m_watchers) {
                p.second->set_master(false);
            }
            
            iter->second->set_master(true);
            
            master_name = name;
        }
        
        // 获取主节点
        std::shared_ptr<filewatch> get_master_watcher() {
        
            if (m_watchers.empty()) return{};
            
            for (auto& p : m_watchers) {
                if (p.second->is_master()) {
                    return p.second;
                }
            }
            
            if (master_name.empty()) {
                m_watchers.begin()->second;
            }
            
            std::string watchkey = to_watcher_key(master_name);
            
            for (auto& p : m_watchers) {
                if (p.first == watchkey) {
                    return p.second;
                }
            }
            
            return m_watchers.begin()->second;
        }
        
        
        // 开始同步
        bool start_sync() {
            if (m_watchers.size() < 2) {
                amo::cdevel << "at least two folders to synchronize" << amo::endl;
                return false;
            }
            
            for (auto& p : m_watchers) {
                p.second->set_mutex(m_mutex);
                p.second->set_looper_executor(m_executor);
                p.second->set_rules(valid_rules, true);
                p.second->set_rules(invalid_rules, false);
                p.second->start_watch();
            }
            
            
            
            return true;
            
        }
        
        void stop_sync() {
            for (auto& p : m_watchers) {
                p.second->stop_watch();
            }
        }
        
        void create_file(std::shared_ptr<amo::filewatch> watcher,
                         const std::string& filename) {
            auto src = watcher->make_file_path(filename);
            
            
            
            if (src.is_file() && src.exists()) {
            
                std::string srcMD5 = watcher->get_file_md5(filename);
                
                for (auto& p : m_watchers) {
                    if (p.second == watcher) {
                        continue;
                    }
                    
                    auto dst = p.second->make_file_path(filename);
                    
                    
                    
                    if (dst.exists()) {
                    
                        std::string dstMD5 = p.second->get_file_md5(filename);
                        
                        if (srcMD5 == dstMD5) {
                            continue;
                        }
                    }
                    
                    src.copy_to(dst);
                    p.second->set_file_md5(filename, srcMD5);
                    p.second->add_ignore_event(filename, amo::Event::added);
                    $cdevel("[new sync][{}]-->[{}]", src.to_string(), dst.to_string());
                    
                    
                }
            } else if (src.is_directory() && src.exists()) {
                for (auto& p : m_watchers) {
                    if (p.second == watcher) {
                        continue;
                    }
                    
                    auto dst = p.second->make_file_path(filename);
                    dst.create_directory();
                }
            }
        }
        
        void copy_file(std::shared_ptr<amo::filewatch> watcher,
                       const std::string& filename) {
                       
                       
                       
            auto src = watcher->make_file_path(filename);
            std::string srcMD5 = watcher->get_file_md5(filename);
            
            
            for (auto& p : m_watchers) {
                if (p.second == watcher) {
                    continue;
                }
                
                auto dst = p.second->make_file_path(filename);
                
                if (dst.exists()) {
                    std::string dstMD5 = p.second->get_file_md5(filename);
                    
                    if (srcMD5 == dstMD5) {
                        continue;
                    }
                }
                
                src.copy_to(dst);
                $cdevel("[modify sync][{}]-->[{}]", src.to_string(), dst.to_string());
                p.second->set_file_md5(filename, srcMD5);
            }
            
        }
        
        void remove_file(std::shared_ptr<amo::filewatch> watcher,
                         const std::string& filename) {
            auto src = watcher->make_file_path(filename);
            
            for (auto& p : m_watchers) {
                if (p.second == watcher) {
                    continue;
                }
                
                auto dst = p.second->make_file_path(filename);
                
                if (!dst.exists()) {
                    continue;
                }
                
                if (dst.is_file()) {
                    bool bOk =  dst.remove();
                    
                    if (!bOk) {
                        $cerr("remove folder failed:{}", dst.to_string());
                    }
                    
                } else if (dst.is_directory()) {
                    bool bOk =   dst.remove_all();
                    
                    if (!bOk) {
                        $cerr("remove folder failed:{}", dst.to_string());
                    }
                }
                
                $cdevel("[remove sync][{}]",  dst.to_string());
                p.second->add_ignore_event(filename, amo::Event::removed);
                
            }
            
        }
        
        void rename_file(std::shared_ptr<amo::filewatch> watcher,
                         const std::string& filename) {
                         
            auto src = watcher->make_file_path(filename);
            std::string oldname = watcher->get_rename_old_name();
            
            if (oldname.empty()) {
                return;
                
            }
            
            for (auto& p : m_watchers) {
                if (p.second == watcher) {
                    continue;
                }
                
                
                auto dst = p.second->make_file_path(oldname);
                
                auto dst_newname = p.second->make_file_path(filename);
                
                
                if (dst.exists()) {
                    dst.canonicalize();
                    dst_newname.canonicalize();
                    dst.rename(dst_newname);
                }
                
            }
            
        }
        
        
        void on_file_change(std::shared_ptr<amo::filewatch> watcher,
                            const std::string& filename, const amo::Event change_type) {
            //amo::cdevel << watcher->make_file_path(filename).to_string() << " : ";
            std::string fullname = watcher->make_file_path(filename).to_string();
            
            switch (change_type) {
            case amo::Event::added:
                //$cdevel("new:{}", fullname);
                create_file(watcher, filename);
                //amo::cdevel << "The file was added to the directory." << amo::endl;
                break;
                
            case amo::Event::removed:
                $cdevel("[remove sync][{}]", fullname);
                remove_file(watcher, filename);
                //amo::cdevel << "The file was removed from the directory." << amo::endl;
                break;
                
            case amo::Event::modified:
                //$cdevel("modify:{}", fullname);
                copy_file(watcher, filename);
                /* amo::cdevel <<
                             "The file was modified. This can be a change in the time stamp or attributes."
                             << amo::endl;*/
                break;
                
            case amo::Event::renamed_old:
                //remove_file(watcher, filename);
                break;
                
            case amo::Event::renamed_new:
                $cdevel("[rename sync][{}]-->[{}]",
                        watcher->make_file_path(watcher->get_rename_old_name()).to_string(), fullname);
                rename_file(watcher, filename);
                break;
            };
            
        }
        
    protected:
        std::shared_ptr<amo::filesync::settings> m_settings;
        
        std::string master_name;
        std::shared_ptr< std::recursive_mutex> m_mutex;
        
        std::shared_ptr<amo::looper_executor> m_executor;
        std::unordered_map<std::string, std::shared_ptr<amo::filewatch> > m_watchers;
        
        std::set<std::string> valid_rules;
        std::set<std::string> invalid_rules;
    };
}



#endif //ADOC_FILESYNC_FE9F9E29_5110_4796_8DF1_EABDE7F4EC94_HPP__