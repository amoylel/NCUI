// Created by amoylel on 26/07/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_TASK_TIMER_D0141725_7811_4E7E_AF91_54C8CE85B931_HPP__
#define AMO_TASK_TIMER_D0141725_7811_4E7E_AF91_54C8CE85B931_HPP__

#include <amo/looper_executor.hpp>
namespace amo {

    class task_timer {
    public:
    
        ////////////////////////////////////////////////////////////////////////
        // 计时器
        ////////////////////////////////////////////////////////////////////////
    public:
        typedef int64_t tid;
        
        class timer_info {
        public:
            //TODO:加锁
        public:
            typedef std::shared_ptr<timer_info> smart_type;
            enum timer_status { TIMER_FAKE, TIMER_OK, TIMER_CANCELED };
            
            tid id;
            
            timer_status status;
            size_t interval_ms;
            std::function<bool(tid)>  call_back;
            
            std::shared_ptr<amo::looper_executor> m_executor;
            timer_info() : status(TIMER_FAKE), interval_ms(0) {}
            
            void cancel() {
            
                if (m_executor) {
                    // 不能在工作线程中取消
                    m_executor->check_if_called_on_invalid_thread();
                    m_executor->kill();
                    this->status = timer_info::TIMER_CANCELED;
                }
                
                m_executor.reset();
            }
            
            void start() {
                cancel();
                m_executor.reset(new amo::looper_executor());
                m_executor->execute(std::bind(&timer_info::on_timer, this), interval_ms);
            }
            
        private:
            bool on_timer() {
                if (call_back  && m_executor && call_back(id)) {
                    m_executor->execute(std::bind(&timer_info::on_timer, this), interval_ms);
                    return true;
                } else {
                    this->status = timer_info::TIMER_CANCELED;
                }
                
                return false;
            }
        };
        
        
    public:
        void update_timer_info(tid id,
                               size_t interval,
                               std::function<bool(tid)>& call_back,
                               bool start = false) {
            if (id > timer_can.size()) {
                return;
            }
            
            timer_info::smart_type& ti = timer_can[id - 1];
            ti->id = id;
            ti->status = timer_info::TIMER_OK;
            ti->interval_ms = interval;
            ti->call_back = call_back;
            
            
            if (start) {
                ti->start();
            } else {
                ti->cancel();
            }
        }
        
        
        
        tid set_timer(size_t interval,
                      std::function<bool(tid)> fn_cb,
                      bool start = true) {
            std::shared_ptr<timer_info> ti(new timer_info());
            timer_can.push_back(ti);
            tid id = timer_can.size();
            ti->id = id;
            update_timer_info(id, interval, fn_cb, start);
            return id;
        }
        
        bool start_timer(tid id) {
            if (id > timer_can.size()) {
                return false;
            }
            
            timer_info::smart_type& ti = timer_can[id - 1];
            
            if (timer_info::TIMER_FAKE == ti->status) {
                return false;
            }
            
            if (timer_info::TIMER_OK == ti->status) {
                return true;
            }
            
            ti->status = timer_info::TIMER_OK;
            ti->start();
            
            return true;
        }
        
        void stop_timer(tid id = -1) {
            for (auto& p : timer_can) {
                if (id == -1 || p->id == id) {
                    p->cancel();
                }
            }
        }
        
        
    private:
    
        std::vector<timer_info::smart_type > timer_can;
    };
}



#endif //AMO_TASK_TIMER_D0141725_7811_4E7E_AF91_54C8CE85B931_HPP__
