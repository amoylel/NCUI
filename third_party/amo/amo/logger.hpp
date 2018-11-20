#ifndef AMO_LOGGER_HPP__
#define AMO_LOGGER_HPP__

#ifndef FMT_HEADER_ONLY
#define FMT_HEADER_ONLY
#endif

#ifndef SPDLOG_FMT_EXTERNAL
#define SPDLOG_FMT_EXTERNAL
#endif


#include <amo/config.hpp>
#include <amo/format.hpp>
#include <amo/logger/raw_daily_file_sink.h>

#ifdef OS_WIN
#include <spdlog/sinks/msvc_sink.h>
#endif // OS_WIN

#include <mutex>

#include <sstream>
#include <vector>
#include <regex>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/dist_sink.h>
#include <amo/logger/console_sinks.h>


#define file_orient (std::string("[") + amo::short_file_name(__FILE__) + std::string(":") + amo::long_to_string(__LINE__) + "] ")

#define func_orient (std::string("[") + std::string(__FUNCTION__) + std::string(":") + amo::long_to_string(__LINE__) + "] ")

#define log_separator ("--------------------------------------------------------------------------------\n")

#define log_asterisk_separator ("********************************************************************************\n")


//##连接起来后要得到一个标识符，也就是_、字母、数字
#define LOG_CONN(a, b) (a+b)

#ifndef AMO_LOG_TRACE_OFF
#define $cdevel(format, ...) amo::cdevel((func_orient + format).c_str(), ##__VA_ARGS__)
#else
#define $cdevel(format, ...)
#endif

#ifndef AMO_LOG_DEBUG_OFF
#define $cdebug(format, ...) amo::cdebug((func_orient  + format).c_str(), ##__VA_ARGS__)
#else
#define $cdebug(format, ...)
#endif

#ifndef AMO_LOG_INFO_OFF
#define $cinfo(format, ...) amo::cinfo(format, ##__VA_ARGS__)
#else
#define $cinfo(format, ...)
#endif


#ifndef AMO_LOG_WARN_OFF
#define $cwarn(format, ...) amo::cwarn((func_orient + format).c_str(), ##__VA_ARGS__)
#else
#define $cwarn(format, ...)
#endif

#ifndef AMO_LOG_ERR_OFF
#define $cerr(format, ...) amo::cerr((func_orient  + format).c_str(), ##__VA_ARGS__)
#else
#define $cerr(format, ...)
#endif

#ifndef AMO_LOG_CRITICAL_OFF
#define $cfatal(format, ...) amo::cfatal((func_orient  + format).c_str(), ##__VA_ARGS__)
#else
#define $cfatal(format, ...)
#endif

#ifndef AMO_LOG_MACRO_OFF
#define $clog(...) __VA_ARGS__
#else
#define $clog(...)
#endif


#ifndef $devel
#define $devel(format, ...) writeLog(amo::log::level::trace, (std::string( format ) ).c_str(), ##__VA_ARGS__);
#endif

#ifndef $debug
#define $debug(format, ...) writeLog(amo::log::level::debug, (std::string( format ) ).c_str(), ##__VA_ARGS__);
#endif

#ifndef $info
#define $info(format, ...) writeLog(amo::log::level::info, (std::string( format ) ).c_str(), ##__VA_ARGS__);
#endif

#ifndef $warn
#define $warn(format, ...) writeLog(amo::log::level::warn, (std::string( format ) ).c_str(), ##__VA_ARGS__);
#endif

#ifndef $err
#define $err(format, ...) writeLog(amo::log::level::err, (std::string( format ) ).c_str(), ##__VA_ARGS__);
#endif

#ifndef $fatal
#define $fatal(format, ...) writeLog(amo::log::level::critical, (std::string() + format).c_str(), ##__VA_ARGS__);
#endif

#ifndef $file_orient
#define $file_orient $cdevel(file_orient)
#endif

#ifndef $func_orient
#define $func_orient $cdevel(func_orient)
#endif

namespace amo {

    enum flag {
        endl = 0,		//日志写入标志
        separator = 1,	//分隔符
        blank_line = 2,	//空行
    };
    
    static std::string short_file_name(std::string file_name) {
        file_name = file_name.substr(file_name.find_last_of('\\') + 1);
        file_name = file_name.substr(file_name.find_last_of('/') + 1);
        return file_name;
    }
    
    static std::string bracket(std::string str) {
        std::string rec = "(";
        rec += str;
        rec += ") ";
        return rec;
    }
    
    static std::string long_to_string(long val) {
        std::stringstream stream;
        stream << val;
        return stream.str();
    }
    
    namespace log {
    
    
    
        using namespace spdlog;
        
        static std::shared_ptr<spdlog::logger> logger() {
        
            return spdlog::get("default");
        }
        
        static std::shared_ptr<spdlog::sinks::dist_sink_mt> sink() {
            static std::shared_ptr<spdlog::sinks::dist_sink_mt> ptr;
            
            if (!ptr) {
                ptr = std::make_shared<spdlog::sinks::dist_sink_mt>();
            }
            
            return ptr;
        }
        
        static void add_sink(std::shared_ptr<spdlog::sinks::sink> sk) {
            sink()->add_sink(sk);
        }
        
        static void remove_sink(std::shared_ptr<spdlog::sinks::sink> sk) {
            sink()->remove_sink(sk);
        }
        
        static void set_level(amo::log::level::level_enum l) {
            auto pLog = logger();
            
            if (!pLog) {
                return;
            }
            
            logger()->set_level(l);
        }
        
        static bool should_log(amo::log::level::level_enum l) {
            auto pLog = logger();
            
            if (!pLog) {
                return false;
            }
            
            return logger()->should_log(l);
        }
        
        static amo::log::level::level_enum get_level() {
            auto pLog = logger();
            
            if (!pLog) {
                return level::off;
            }
            
            return logger()->level();
        }
        
        static const std::string& name() {
            auto pLog = logger();
            
            if (!pLog) {
                return "";
            }
            
            return logger()->name();
        }
        
        static void set_pattern(const std::string& str) {
            auto pLog = logger();
            
            if (!pLog) {
                return ;
            }
            
            logger()->set_pattern(str);
        }
        
        static void write(const std::string& str) {
            auto pLog = logger();
            
            if (!pLog) {
                return;
            }
            
            logger()->log(spdlog::level::level_enum::critical, str);
        }
        
        static bool initialize(bool stdlog = false, bool msvclog = false) {
            try {
                if (logger()) {
                    return true;
                }
                
                auto ptr = spdlog::create("default", sink());
                
                if (!ptr) {
                    return false;
                }
                
                if (stdlog) {
                    auto sink1 =
                        std::make_shared<spdlog::sinks::console_sink_mt>();			// 标准输出
                    add_sink(sink1);
                }
                
                $windows(
                
                if (msvclog) {
                auto sink2 = std::make_shared<spdlog::sinks::msvc_sink_mt>();			// MSVC 输出
                    add_sink(sink2);
                }
                
                )
                
                
                set_level(amo::log::level::trace);
                set_pattern("[%Y-%m-%d %H:%M:%S.%e][%l] %v");
                
                return true;
            } catch (const spdlog::spdlog_ex& ex) {
                std::cerr << "Log init failed: " << ex.what() << std::endl;
                return false;
            }
        }
        
        static bool register_logger(std::shared_ptr<spdlog::logger> ptr) {
            if (!ptr) {
                return false;
            }
            
            auto p = spdlog::get(ptr->name());
            
            if (p == ptr) {
                return true;
            }
            
            spdlog::register_logger(ptr);
            return true;
        }
        
        static void finalize() {
            spdlog::drop_all();
        }
        
        
        
        class unit {
        public:
            unit(spdlog::level::level_enum l,
                 std::function<bool(spdlog::level::level_enum, const std::string&)> fn_cb
                 = std::function<bool(spdlog::level::level_enum, const std::string&)>())
                : m_level(l)
                , m_fn_before_write(fn_cb)
                , m_enabled(true) {
                
            }
            
            template <typename... Args>
            inline void operator()(const std::string& fmt, const Args&... args) {
            
                if (!enable()) {
                    return;
                }
                
                if (!m_logger) {
                    m_logger = logger();
                }
                
                if (!m_logger) {
                    return;
                }
                
                
                
                m_logger->log(m_level, fmt.c_str(), args...);
            }
            
            unit& operator << (amo::flag val) {
                if (!enable()) {
                    return*this;
                }
                
                switch (val) {
                case endl:
                    write_buffer();
                    break;
                    
                case separator:
                    write_buffer();
                    break;
                    
                case blank_line:
                    write_buffer();
                    break;
                    
                default:
                    break;
                }
                
                return *this;
            }
            
            template<typename T>
            unit& operator << (const T& val) {
                buffer << val;
                return *this;
            }
            
            
            
            void write_buffer() {
                try {
                    if (!enable()) {
                        return;
                    }
                    
                    std::string str = buffer.str();
                    buffer.clear();
                    
                    /* if (str.empty()) {
                         return;
                     }*/
                    
                    // 使用其他
                    if (m_fn_before_write && m_fn_before_write(m_level, str)) {
                        return;
                    }
                    
                    if (!m_logger) {
                        m_logger = logger();
                    }
                    
                    if (!m_logger) {
                        return;
                    }
                    
                    
                    m_logger->log(m_level, str);
                } catch (const std::exception& e) {
                    std::cout << e.what() << std::endl;
                }
                
            }
            
            void enable(bool b) {
                m_enabled = b;
            }
            
            bool enable() const {
                return m_enabled;
            }
            
            void add_filter(const std::string& filter) {
            
            }
        private:
            std::string reg_filter;  // 日志输出过虑器
            bool m_enabled;
            spdlog::level::level_enum m_level;
            amo::memory_writer buffer;
            std::shared_ptr<spdlog::logger> m_logger;
            std::function<bool(spdlog::level::level_enum, const std::string&)>
            m_fn_before_write;
        };
    }
    
    class log_utils {
    public:
        static amo::log::unit& get_default_devel_unit() {
            static amo::log::unit unt_(amo::log::level::trace);
            return unt_;
        }
        
        static amo::log::unit& get_default_debug_unit() {
            static amo::log::unit unt_(amo::log::level::debug);
            return unt_;
        }
        
        static amo::log::unit& get_default_info_unit() {
            static amo::log::unit unt_(amo::log::level::info);
            return unt_;
        }
        
        static amo::log::unit& get_default_warn_unit() {
            static amo::log::unit unt_(amo::log::level::warn);
            return unt_;
        }
        
        static amo::log::unit& get_default_rerror_unit() {
            static amo::log::unit unt_(amo::log::level::err);
            return unt_;
        }
        
        static amo::log::unit& get_default_fatal_unit() {
            static amo::log::unit unt_(amo::log::level::critical);
            return unt_;
        }
    };
    
    static amo::log::unit&	cdevel	= log_utils::get_default_devel_unit();
    static amo::log::unit&	cdebug	= log_utils::get_default_debug_unit();
    static amo::log::unit&	cinfo	= log_utils::get_default_info_unit();
    static amo::log::unit&	cwarn	= log_utils::get_default_warn_unit();
    static amo::log::unit&	cerr	= log_utils::get_default_rerror_unit();
    static amo::log::unit&	cfatal	= log_utils::get_default_fatal_unit();
    
    
    class log_object {
    public:
        log_object() {
        
        }
        
        std::shared_ptr<spdlog::logger> getLogger() const {
            return m_logger;
        }
        
        void setLogger(std::shared_ptr<spdlog::logger> val) {
            m_logger = val;
        }
        
        template <typename... Args>
        inline void writeLog(log::level::level_enum lvl, const char* fmt,
                             const Args&... args) {
            if (getLogger()) {
                getLogger()->log(lvl, fmt, args...);
            } else {
                switch (lvl) {
                case log::level::trace:
                    $cdevel(fmt, args...);
                    break;
                    
                case log::level::debug:
                    $cdebug(fmt, args...);
                    break;
                    
                case log::level::info:
                    $cinfo(fmt, args...);
                    break;
                    
                case log::level::warn:
                    $cwarn(fmt, args...);
                    break;
                    
                case log::level::err:
                    $cerr(fmt, args...);
                    break;
                    
                case log::level::critical:
                    $cfatal(fmt, args...);
                    break;
                    
                default:
                    break;
                }
            }
        }
    private:
        std::shared_ptr<spdlog::logger> m_logger;
    };
    
}

#endif // AMO_LOGGER_HPP__