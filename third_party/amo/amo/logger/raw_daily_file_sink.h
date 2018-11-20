
// Created by amoylel on 12/15/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_RAW_DAILY_FILE_SINK_H__
#define AMO_RAW_DAILY_FILE_SINK_H__



#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>
#include <spdlog/details/file_helper.h>
#include <spdlog/fmt/fmt.h>

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <mutex>
#include <string>
#include <cerrno>

#include <amo/logger/raw_file_helper.h>
#include <spdlog/sinks/file_sinks.h>

namespace spdlog {
    namespace sinks {
    
        /*
        * Default generator of daily log file names.
        */
        struct default_month_daily_file_name_calculator {
            //Create filename for the form basename.YYYY-MM-DD_hh-mm.extension
            static filename_t calc_filename(const filename_t& basename,
                                            const filename_t& extension) {
                std::tm tm = spdlog::details::os::localtime();
                std::conditional<std::is_same<filename_t::value_type, char>::value, fmt::MemoryWriter, fmt::WMemoryWriter>::type
                w;
                w.write(SPDLOG_FILENAME_T("{}_{:04d}-{:02d}-{:02d}_{:02d}-{:02d}.{}"), basename,
                        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, extension);
                return w.str();
            }
        };
        
        /*
        * Generator of daily log file names in format basename.YYYY-MM-DD.extension
        */
        struct month_dateonly_daily_file_name_calculator {
            //Create filename for the form basename.YYYY-MM-DD.extension
            static filename_t calc_filename(const filename_t& basename,
                                            const filename_t& extension) {
                std::tm tm = spdlog::details::os::localtime();
                
                std::conditional<std::is_same<filename_t::value_type, char>::value, fmt::MemoryWriter, fmt::WMemoryWriter>::type
                w;
                w.write(SPDLOG_FILENAME_T("{:04d}{:02d}/{}_{:04d}-{:02d}-{:02d}.{}"),
                        tm.tm_year + 1900, tm.tm_mon + 1, basename, tm.tm_year + 1900, tm.tm_mon + 1,
                        tm.tm_mday, extension);
                        
                return w.str();
            }
        };
        
        struct month_datetime_daily_file_name_calculator {
            //Create filename for the form basename.YYYY-MM-DD.extension
            static filename_t calc_filename(const filename_t& basename,
                                            const filename_t& extension) {
                std::tm tm = spdlog::details::os::localtime();
                std::conditional<std::is_same<filename_t::value_type, char>::value, fmt::MemoryWriter, fmt::WMemoryWriter>::type
                w;
                w.write(SPDLOG_FILENAME_T("{:04d}{:02d}/{}_{:04d}-{:02d}-{:02d}_{:02d}-{:02d}-{:02d}.{}"),
                        tm.tm_year + 1900, tm.tm_mon + 1, basename, tm.tm_year + 1900, tm.tm_mon + 1,
                        tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, extension);
                return w.str();
            }
        };
        
        /*
        * Rotating file sink based on date. rotates at midnight
        */
        template<class Mutex, class FileNameCalc = default_daily_file_name_calculator>
        class raw_daily_file_sink : public base_sink < Mutex > {
        public:
            //create daily file sink which rotates on given time
            raw_daily_file_sink(const filename_t& log_dir,
                                const filename_t& base_filename,
                                const filename_t& extension,
                                int rotation_hour,
                                int rotation_minute,
                                bool force_flush = false,
                                bool write_formatted = false)
                : _log_dir(log_dir)
                ,  _base_filename(base_filename)
                , _extension(extension)
                , _rotation_h(rotation_hour)
                , _rotation_m(rotation_minute)
                , _file_helper(force_flush, write_formatted) {
                if (rotation_hour < 0 || rotation_hour > 23 || rotation_minute < 0 ||
                        rotation_minute > 59) {
                    throw spdlog_ex("raw_daily_file_sink: Invalid rotation time in ctor");
                }
                
                _rotation_tp = _next_rotation_tp();
                _file_helper.log_dir(_log_dir);
                _file_helper.open(FileNameCalc::calc_filename(_base_filename, _extension));
            }
            
            void flush() override {
                _file_helper.flush();
            }
            
        protected:
            void _sink_it(const details::log_msg& msg) override {
                if (std::chrono::system_clock::now() >= _rotation_tp) {
                    _file_helper.open(FileNameCalc::calc_filename(_base_filename, _extension));
                    _rotation_tp = _next_rotation_tp();
                }
                
                _file_helper.write(msg);
            }
            
        private:
            std::chrono::system_clock::time_point _next_rotation_tp() {
                auto now = std::chrono::system_clock::now();
                time_t tnow = std::chrono::system_clock::to_time_t(now);
                tm date = spdlog::details::os::localtime(tnow);
                date.tm_hour = _rotation_h;
                date.tm_min = _rotation_m;
                date.tm_sec = 0;
                auto rotation_time = std::chrono::system_clock::from_time_t(std::mktime(&date));
                
                if (rotation_time > now) {
                    return rotation_time;
                } else {
                    return std::chrono::system_clock::time_point(rotation_time
                            + std::chrono::hours(24));
                }
            }
            filename_t _log_dir;
            filename_t _base_filename;
            filename_t _extension;
            int _rotation_h;
            int _rotation_m;
            std::chrono::system_clock::time_point _rotation_tp;
            details::raw_file_helper _file_helper;
        };
        
        typedef raw_daily_file_sink<std::mutex, month_dateonly_daily_file_name_calculator>
        raw_daily_file_sink_mt;
        typedef raw_daily_file_sink<details::null_mutex, month_dateonly_daily_file_name_calculator>
        raw_daily_file_sink_st;
        
        
        typedef raw_daily_file_sink<std::mutex, month_datetime_daily_file_name_calculator>
        raw_daily_datetime_file_sink_mt;
        typedef raw_daily_file_sink<details::null_mutex, month_datetime_daily_file_name_calculator>
        raw_daily_datetime_file_sink_st;
        
    }
}


#endif // AMO_RAW_DAILY_FILE_SINK_H__
