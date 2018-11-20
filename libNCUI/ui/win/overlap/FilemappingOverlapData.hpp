// Created by amoylel on 08/19/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_FILEMAPPINGOVERLAPDATA_722D3991_121D_4F65_A3C7_2DC1AE9FB29E_HPP__
#define AMO_FILEMAPPINGOVERLAPDATA_722D3991_121D_4F65_A3C7_2DC1AE9FB29E_HPP__


#include <vector>
#include <memory>
#include <amo/utility.hpp>
#include <amo/file_mapping.hpp>
#include "ui/win/overlap/OverlapData.hpp"


namespace amo {
    class FilemappingOverlapData : public OverlapData {
    public:
        FilemappingOverlapData(std::shared_ptr<amo::file_mapping> ptr) {
            m_file_mapping = ptr;
            m_buffer = NULL;
            m_size = 0;
        }
        
        virtual void fill(const char* buffer, int size) override {
            m_size = size;
            m_buffer = m_file_mapping->address();
            /*m_buffer = (char*)buffer;*/
            
        }
        virtual int size() override {
            return m_size;
        }
        
        
        virtual const char* data() const override {
            return m_buffer;
        }
        
        
        
        virtual void set_locked(bool locked) override {
            return m_file_mapping->set_locked(locked);
        }
        
    public:
        int m_size;
        char* m_buffer;
        std::shared_ptr<amo::file_mapping> m_file_mapping;
    };
}

#endif // AMO_FILEMAPPINGOVERLAPDATA_722D3991_121D_4F65_A3C7_2DC1AE9FB29E_HPP__

