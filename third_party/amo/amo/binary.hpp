// Created by amoylel on 10/14/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_BINARY_3934DD69_41D2_47ED_938E_5357EDA6167F_HPP__
#define AMO_BINARY_3934DD69_41D2_47ED_938E_5357EDA6167F_HPP__

#include <string>
#include <vector>
#include <deque>
#include <stdint.h>
#include <amo/bit_converter.hpp>
#include <algorithm>


namespace amo {
    class binary {
    public:
        binary() {
        
        }
        
        binary(const binary& rhs) {
            m_buffer = rhs.m_buffer;
        }
        
        
        binary(const std::deque<int8_t>& que) {
            m_buffer = que;
        }
        
        template<typename T>
        binary(const T& val) {
            if (std::is_integral<T>::value) {
                int8_t mask = 0x01;
                T num = val;
                
                for (int i = 0; i < sizeof(T) * 8; ++i) {
                    if ((num & mask) != 0) {
                        m_buffer.push_back(1);
                    } else {
                        m_buffer.push_back(0);
                    }
                    
                    num >>= 1;
                }
                
                std::reverse(m_buffer.begin(), m_buffer.end());
            }
        }
        
        binary(const std::string& str) {
            for (size_t i = 0; i < str.size(); ++i) {
                char c = str[i];
                
                if (c != '0' && c != '1') {
                    continue;
                }
                
                m_buffer.push_back(c - '0');
            }
        }
        
        binary(const char* str) {
            for (size_t i = 0; i < strlen(str); ++i) {
                char c = str[i];
                
                if (c != '0' && c != '1') {
                    continue;
                }
                
                m_buffer.push_back(c - '0');
            }
        }
        std::string to_string(bool trim_left = false) const {
            std::string retval;
            std::deque<int8_t> vec = m_buffer;
            
            while (trim_left && !vec.empty() && vec.front() == 0) {
                vec.pop_front();
            }
            
            
            for (auto& p : vec) {
                retval += (p + 0x30);
            }
            
            return retval;
        }
        
        binary sub(const int& high, const int& offset) {
            if (offset <= 0) {
                return binary();
            }
            
            size_t start = m_buffer.size() - high - 1;
            
            size_t diff = offset;
            
            size_t len = (std::min)(start + diff, m_buffer.size());
            
            std::deque<int8_t> que;
            
            for (size_t i = start; i < len; ++i) {
                que.push_back(m_buffer[i]);
            }
            
            return binary(que);
            
        }
        
        // 向上补齐
        binary& ceil() {
        
            if (!is_align()) {
                // 补小位
                int offset = next_ceil_offset();
                
                while (offset > 0) {
                    m_buffer.push_front(0);
                    --offset;
                }
                
            } else  if (m_buffer.size() < 64) {
                int offset = m_buffer.size();
                
                while (offset > 0) {
                    m_buffer.push_front(m_buffer[0]);
                    --offset;
                }
            }
            
            return *this;
        }
        
        // 向下补齐
        binary& floor() {
            if (!is_align()) {
                // 补小位
                int offset = next_floor_offset();
                
                while (offset > 0) {
                    m_buffer.pop_front();
                    --offset;
                }
                
            } else  if (m_buffer.size() > 8) {
                int offset = m_buffer.size() / 2 ;
                
                while (offset > 0) {
                    m_buffer.pop_front();
                    --offset;
                }
            }
            
            return *this;
        }
        
        template<typename R>
        operator R() const {
            R retval = 0;
            
            if (m_buffer.empty()) {
                return retval;
            }
            
            if (!is_align()) {
                return binary(m_buffer).ceil().operator R();
            }
            
            int start_pos = m_buffer.size() - sizeof(R) * 8;
            
            if (start_pos < 0) {
                for (int i = 0; i < abs(start_pos); ++i) {
                    if (m_buffer[0] != 0) {
                        retval |= (1 << (sizeof(R) * 8 - i - 1));
                    }
                }
                
                start_pos = 0;
            }
            
            
            
            for (size_t i = start_pos; i < m_buffer.size(); ++i) {
                if (m_buffer[i] != 0) {
                    retval |= (R)((R)1 << (m_buffer.size() - i - 1));
                }
            }
            
            return retval;
        }
        
        bool test(const int& index) const {
            int offset = m_buffer.size() - index - 1;
            
            if (offset < 0) {
                return false;
            }
            
            if (offset > (int)m_buffer.size() - 1) {
                return false;
            }
            
            return m_buffer.at(offset) != 0;
        }
        
        bool is_align() const {
            if (m_buffer.size() > 64) {
                return false;
            }
            
            switch (m_buffer.size()) {
            case 0:
            case 8:
            case 16:
            case 32:
            case 64:
                return true;
                
            default:
            
                return false;
            }
        }
        
    protected:
        int next_ceil_offset() const {
            if (m_buffer.size() < 8) {
                return 8 - m_buffer.size();
            }
            
            if (m_buffer.size() < 16) {
                return 16 - m_buffer.size();
            }
            
            if (m_buffer.size() < 32) {
                return 32 - m_buffer.size();
            }
            
            if (m_buffer.size() < 64) {
                return 64 - m_buffer.size();
            }
            
            return 0;
        }
        
        int next_floor_offset() const {
            if (m_buffer.size() > 64) {
                return  m_buffer.size() - 64;
            }
            
            if (m_buffer.size() > 32) {
                return  m_buffer.size() - 32;
            }
            
            if (m_buffer.size() > 16) {
                return   m_buffer.size() - 16;
            }
            
            if (m_buffer.size() > 8) {
                return  m_buffer.size() - 8;
            }
            
            return 0;
        }
    protected:
        std::deque<int8_t> m_buffer;
    };
}

#endif // AMO_BINARY_3934DD69_41D2_47ED_938E_5357EDA6167F_HPP__

