// Created by 苏元海 on 06/01/2016.
// Copyright (c) 2016 amoylel. All rights reserved.

#ifndef IME_ICONV_HPP__
#define IME_ICONV_HPP__


#ifdef __linux__
#include <errno.h>
#include <iconv.h>

#include <stdexcept>
#include <string>
#include <vector>



namespace amo {

    class converter {
    public:
        converter(const std::string& in_encode,
                  const std::string& out_encode,
                  bool ignore_error = false,
                  size_t buf_size = 1024)
            : ignore_error_(ignore_error),
              buf_size_(buf_size) {
              
            if (buf_size == 0) {
                throw std::runtime_error("buffer size must be greater than zero");
            }
            
            iconv_t conv = ::iconv_open(out_encode.c_str(), in_encode.c_str());
            
            if (conv == (iconv_t) - 1) {
                if (errno == EINVAL)
                    throw std::runtime_error(
                        "not supported from " + in_encode + " to " + out_encode);
                else {
                    throw std::runtime_error("unknown error");
                }
            }
            
            iconv_ = conv;
            
            
        }
        
        ~converter() {
            iconv_close(iconv_);
        }
        
        void convert(const std::string& input, std::string& output) const {
            // copy the string to a buffer as iconv function requires a non-const char
            // pointer.
            std::vector<char> in_buf(input.begin(), input.end());
            char* src_ptr = &in_buf[0];
            size_t src_size = input.size();
            
            std::vector<char> buf(buf_size_);
            std::string dst;
            
            while (0 < src_size) {
                char* dst_ptr = &buf[0];
                size_t dst_size = buf.size();
                size_t res = ::iconv(iconv_, &src_ptr, &src_size, &dst_ptr, &dst_size);
                
                if (res == (size_t) - 1) {
                    if (errno == E2BIG)  {
                        // ignore this error
                    } else if (ignore_error_) {
                        // skip character
                        ++src_ptr;
                        --src_size;
                    } else {
                        check_convert_error();
                    }
                }
                
                dst.append(&buf[0], buf.size() - dst_size);
            }
            
            dst.swap(output);
        }
        
    private:
        void check_convert_error() const {
            switch (errno) {
            case EILSEQ:
                throw std::runtime_error("invalid multibyte chars");
                
            case EINVAL:
                throw std::runtime_error("invalid multibyte chars");
                
            default:
                throw std::runtime_error("unknown error");
            }
        }
        
        iconv_t iconv_;
        bool ignore_error_;
        const size_t buf_size_;
    };
    
}  // namespace iconvpp
#endif



#endif // IME_ICONV_HPP__
