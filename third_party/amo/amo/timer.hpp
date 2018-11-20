// Created by amoylel on 12/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_TIMER_ECC50CBC_9E1B_4C28_A4F7_EC47F72E9089_HPP__
#define AMO_TIMER_ECC50CBC_9E1B_4C28_A4F7_EC47F72E9089_HPP__



#include <amo/config.hpp>
#include <amo/chrono.hpp>
#include <amo/looper_executor.hpp>


#ifndef AMO_TIMER_INIT
#define AMO_TIMER_INIT(text)\
		amo::timer::start_timer_foo(#text);
#endif

#ifndef AMO_TIMER_RESTART
#define AMO_TIMER_RESTART()\
		amo::timer::get_static_timer().restart();
#endif

#ifndef AMO_TIMER_ELAPSED
#define AMO_TIMER_ELAPSED()\
		amo::timer::ouput_time_elapsed_info(func_orient);
#endif

#ifndef AMO_TIMER_ELAPSED_TEXT
#define AMO_TIMER_ELAPSED_TEXT(text)\
		amo::timer::ouput_time_elapsed_info(func_orient+#text);
#endif




#define func_orient_for_timer (std::string("[") + std::string(__FUNCTION__) + std::string(":") + std::to_string(__LINE__) + "] ")

#ifndef FUNC_CALL_TIME
#define FUNC_CALL_TIME() time_consuming(func_orient_for_timer);
#endif

//#define func_orient (std::string("[") + std::string(__FUNCTION__) + std::string(":") + amo::long_to_string(__LINE__) + "] ")
//static std::string long_to_string(long val) {
//	std::stringstream stream;
//	stream << val;
//	return stream.str();
//}
namespace amo {
    class timer {
    public:
    
        ////////////////////////////////////////////////////////////////////////
        // 计时器
        ////////////////////////////////////////////////////////////////////////
    public:
        static amo::timer & get_static_app_timer() {
            static amo::timer t;
            return t;
        }
        
        static amo::timer & get_static_runtime_timer() {
            static amo::timer t;
            return t;
        }
        
        static void start_timer_foo(const std::string & text) {
            std::stringstream stream;
            stream << text;
            stream << "\t app elapsed:" << get_static_app_timer().elapsed();
            stream << ":\t\n";
            get_static_runtime_timer().restart();
            $windows({ OutputDebugStringA(stream.str().c_str()); })
            
        }
        
        static void ouput_time_elapsed_info(const std::string & text) {
            std::stringstream stream;
            stream << text;
            stream << ":\t";
            stream << get_static_runtime_timer().elapsed();
            stream << "\t app elapsed :" << get_static_app_timer().elapsed();
            stream << "\n";
            get_static_runtime_timer().restart();
            $windows({ OutputDebugStringA(stream.str().c_str()); })
        }
        
    public:
        timer() {
            restart();
        }
        
        timer(int64_t ms)
            : start(amo::chrono::milliseconds(ms)) {
        }
        
        void restart() {                    // 执行之后: elapsed()==0
            start = amo::chrono::system_clock::now();
        }
        
        void restart(int64_t ms) {
            start = amo::chrono::time_point<amo::chrono::system_clock>
                    (amo::chrono::milliseconds(ms));
        }
        
        template<typename T>
        uint64_t elapsed()
        const {              // 以毫秒为单位，返回流逝的时间
            amo::chrono::time_point<amo::chrono::system_clock> now =
                amo::chrono::system_clock::now();
            return amo::chrono::duration_cast<T>(now.time_since_epoch()).count()
                   - amo::chrono::duration_cast<T>(start.time_since_epoch()).count();
                   
        }
        
        uint64_t elapsed() const {
            amo::chrono::time_point<amo::chrono::system_clock> now =
                amo::chrono::system_clock::now();
            return amo::chrono::duration_cast<amo::chrono::milliseconds>
                   (now.time_since_epoch()).count()
                   - amo::chrono::duration_cast<amo::chrono::milliseconds>
                   (start.time_since_epoch()).count();
        }
        
        template<typename T>
        static int64_t now() {
            amo::chrono::time_point<amo::chrono::system_clock> now =
                amo::chrono::system_clock::now();
            return amo::chrono::duration_cast<T>(now.time_since_epoch()).count();
        }
        
        static int64_t now() {
            amo::chrono::time_point<amo::chrono::system_clock> now =
                amo::chrono::system_clock::now();
            return amo::chrono::duration_cast<amo::chrono::milliseconds>
                   (now.time_since_epoch()).count();
        }
        
    private:
        amo::chrono::time_point<amo::chrono::system_clock>  start;
        
        
        
        ////////////////////////////////////////////////////////////////////////
        // 定时器
        //
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
            tid id = timer_can.size() ;
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
    
    class time_consuming {
    public:
        time_consuming(const std::string& msg) : m_msg(msg) {
        
        }
        
        ~time_consuming() {
        
            amo::cdevel << m_msg << ": " << t.elapsed() << amo::endl;
        }
        std::string m_msg;
        amo::timer t;
    };
    
    
}




#endif //AMO_TIMER_ECC50CBC_9E1B_4C28_A4F7_EC47F72E9089_HPP__

