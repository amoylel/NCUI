// Created by amoylel on 08/19/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_DEFAULTOVERLAPDATA_87F1E551_FF24_4FFD_9D85_EF572238DC73_HPP__
#define AMO_DEFAULTOVERLAPDATA_87F1E551_FF24_4FFD_9D85_EF572238DC73_HPP__

#include <vector>
#include <memory>
#include <gdiplus.h>
#include <amo/utility.hpp>
#include "ui/win/overlap/OverlapData.hpp"
#include <wtypes.h>
#include <Gdiplus.h>
#include <gdiplus.h>

namespace amo {
    class DefalutOverlapData : public OverlapData {
    public:
        DefalutOverlapData() {
        
        }
        
        virtual void fill(const char* buffer, int size) override {
            m_buffer.resize(size);
            memcpy(m_buffer.data(), buffer, size);
            /* char a = m_buffer[0];
             memcpy(m_buffer.data(), m_buffer.data() + 1, size - 1);
             m_buffer[size - 1] = a;*/
            /*for (size_t i = 0; i < m_buffer.size(); ++i) {
                std::swap(m_buffer[i], m_buffer[i + 3]);
                i += 4;
            }*/
            
        }
        
        virtual void resize(int size, char val) override {
            m_buffer.resize(size);
            memset(m_buffer.data(), val, m_buffer.size());
        }
        
        virtual int size() override {
            return m_buffer.size();
        }
        
        
        virtual const char* data() const override {
            return (const char*)m_buffer.data();
        }
        
    public:
    
        std::vector<uint8_t> m_buffer;
    };
    
}

#endif // AMO_DEFAULTOVERLAPDATA_87F1E551_FF24_4FFD_9D85_EF572238DC73_HPP__

