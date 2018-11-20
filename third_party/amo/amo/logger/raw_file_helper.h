//
// Copyright(c) 2015 Gabi Melman.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
//

// Created by amoylel on 12/15/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_RAW_FILE_HELPER_H__
#define AMO_RAW_FILE_HELPER_H__




#pragma once

// Helper class for file sink
// When failing to open a file, retry several times(5) with small delay between the tries(10 ms)
// Can be set to auto flush on every line
// Throw spdlog_ex exception on errors

#include <spdlog/details/os.h>
#include <spdlog/details/log_msg.h>

#include <chrono>
#include <cstdio>
#include <string>
#include <thread>
#include <cerrno>
#include <amo/path.hpp>

namespace spdlog {
    namespace details {
    
        class raw_file_helper {
        
        public:
            const int open_tries = 5;
            const int open_interval = 10;
            
            explicit raw_file_helper(bool force_flush, bool write_formatted_data) :
                _fd(nullptr),
                _force_flush(force_flush),
                _write_formatted_data(write_formatted_data)
            {}
            
            raw_file_helper(const raw_file_helper&) = delete;
            raw_file_helper& operator=(const raw_file_helper&) = delete;
            
            ~raw_file_helper() {
                close();
            }
            
            void log_dir(const filename_t& fdir) {
                _dirname = fdir;
            }
            
            
            
            void open(const filename_t& fname, bool truncate = false) {
            
                close();
                auto *mode = truncate ? SPDLOG_FILENAME_T("wb") : SPDLOG_FILENAME_T("ab");
                
                _filename = fname;
                
                filename_t file = _dirname;
                
                
                
                if (!file.empty() &&  *file.rbegin() != '\\' && *file.rbegin() != '/') {
                    file += "/";
                }
                
                for (size_t i = 0; i < file.size(); ++i) {
                    if (file[i] == '\\') {
                        file[i] = '/';
                    }
                }
                
                
                file += _filename;
                
                int index = file.find_last_of('/');
                
                if (index != -1) {
                    std::string dir = file.substr(0, index);
                    amo::path p(dir);
                    p.create_directory();
                    
                    if (!p.create_directory()) {
                        throw spdlog_ex("Failed create folder " + dir + " for writing", errno);
                    }
                    
                }
                
                for (int tries = 0; tries < open_tries; ++tries) {
                    if (!os::fopen_s(&_fd, file, mode)) {
                        return;
                    }
                    
                    std::this_thread::sleep_for(std::chrono::milliseconds(open_interval));
                }
                
                throw spdlog_ex("Failed opening file " + os::filename_to_str(
                                    file) + " for writing", errno);
            }
            
            void reopen(bool truncate) {
                if (_filename.empty()) {
                    throw spdlog_ex("Failed re opening file - was not opened before");
                }
                
                open(_filename, truncate);
                
            }
            
            void flush() {
                std::fflush(_fd);
            }
            
            void close() {
                if (_fd) {
                    std::fclose(_fd);
                    _fd = nullptr;
                }
            }
            
            void write(const log_msg& msg) {
            
                if (_write_formatted_data) {
                    size_t msg_size = msg.formatted.size();
                    auto data = msg.formatted.data();
                    
                    
                    if (std::fwrite(data, 1, msg_size, _fd) != msg_size) {
                        throw spdlog_ex("Failed writing to file " + os::filename_to_str(_filename),
                                        errno);
                    }
                } else {
                    size_t msg_size = msg.raw.size();
                    auto data = msg.raw.data();
                    
                    if (std::fwrite(data, 1, msg_size, _fd) != msg_size) {
                        throw spdlog_ex("Failed writing to file " + os::filename_to_str(_filename),
                                        errno);
                    }
                }
                
                
                if (_force_flush) {
                    std::fflush(_fd);
                }
            }
            
            size_t size() {
                if (!_fd) {
                    throw spdlog_ex("Cannot use size() on closed file " + os::filename_to_str(
                                        _filename));
                }
                
                return os::filesize(_fd);
            }
            
            const filename_t& filename() const {
                return _filename;
            }
            
            static bool file_exists(const filename_t& name) {
            
                return os::file_exists(name);
            }
            
            bool write_formatted_data() const {
                return _write_formatted_data;
            }
            
            void write_formatted_data(bool val) {
                _write_formatted_data = val;
            }
        private:
            FILE* _fd;
            filename_t _dirname;
            filename_t _filename;
            bool _force_flush;
            bool _write_formatted_data;
        };
    }
}


#endif // AMO_RAW_FILE_HELPER_H__