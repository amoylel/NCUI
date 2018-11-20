// Created by amoylel on 11/17/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_BLOB_99BF98E9_40DA_4030_AA45_248E4C15B9CF_HPP__
#define AMO_BLOB_99BF98E9_40DA_4030_AA45_248E4C15B9CF_HPP__

#include <string>
#include <memory>
#include <utility>

#include <amo/file_mapping.hpp>
#include <amo/uuid.hpp>
#include <amo/utility.hpp>
#include <amo/json.hpp>



namespace amo {


    class file_mapping_data {
    public:
        enum blob_state {
            BLOB_UNKNOWN = -1,		// 未知状态
            BLOB_NEW,			// 新建对象
            BLOB_READY,			// 已写入数据
            BLOB_DELETE,		// 已被删除
        };
    public:
        file_mapping_data(const std::string& name, int32_t size,
                          int64_t delay_destroy = 10000) {
            m_name = amo::uuid().to_string();
            
            if (!name.empty()) {
                m_name = name;
            }
            
            m_open_only = false;
            m_state = BLOB_NEW;
            m_create_timestamp = amo::utils::now();
            m_delay_destroy = delay_destroy;
            m_mine = "application/octet-stream";
            m_file.reset(new amo::file_mapping(m_name, m_open_only, size));
            write_header();
            
        }
        file_mapping_data(const std::string& name) {
            m_name = name;
            m_open_only = true;
            m_state = BLOB_UNKNOWN;
            m_create_timestamp = 0;
            m_delay_destroy = 10000;
            m_file.reset(new amo::file_mapping(m_name, m_open_only));
            read_header();
        }
        
        const std::string& get_name() const {
            return m_name;
        }
        
        const int64_t create_timestamp() const {
            return m_create_timestamp;
        }
        
        const int64_t delay_destroy() const {
            return m_delay_destroy;
        }
        
        const std::string mime() const {
            return m_mine;
        }
        const int state() const {
            return m_state ;
        }
        void create_timestamp(const int64_t& val)   {
            m_create_timestamp = val;
        }
        
        void  delay_destroy(const int64_t& val)   {
            m_delay_destroy = val;
        }
        
        void mime(const std::string& val)   {
            m_mine = val;
        }
        void state(int val) {
            m_state = val;
        }
        
        
        void update_header() {
            if (m_open_only) {
                read_header();
            } else {
                write_header();
            }
        }
        
        void read_header() {
            amo::json json = m_file->read_header();
            m_create_timestamp = json.getInt64("create_timestamp");
            m_delay_destroy = json.getInt64("delay_destroy");
            m_data_size = json.getInt64("data_size");
            m_mine = json.getString("mime");
            m_state = json.getInt("state");
        }
        
        void write_header() {
            amo::json json;
            json.put("create_timestamp", m_create_timestamp);
            json.put("delay_destroy", m_delay_destroy);
            json.put("data_size", m_data_size);
            json.put("mime", m_mine);
            json.put("state", m_state);
            m_file->write_header(json.to_string());
            
        }
        
        std::string read() {
            read_header();
            std::string retval(m_data_size, 0);
            m_file->read((char*)retval.c_str(), 0, m_data_size);
            return std::move(retval);
        }
        
        void write(const std::string& data) {
            m_file->write((char*)data.c_str(), 0, data.size());
            m_data_size = data.size();
            m_state = BLOB_READY;
            write_header();
        }
        
        
        bool timeout() {
            int64_t timestamp = amo::utils::now();
            
            if (timestamp > m_create_timestamp + m_delay_destroy) {
                return true;
            }
            
            return false;
        }
        
    protected:
        std::shared_ptr<file_mapping> m_file;	// 数据保存位置
        std::string m_name;						// 名称
        int64_t m_create_timestamp;				// 数据创建时间
        int64_t m_delay_destroy;			// 数据销毁时间
        int64_t m_data_size;					// 数据长度
        std::string m_mine;					// 数据类型
        int m_state;
        bool m_open_only;						//
    };
    
    class blob : public amo::file_mapping_data {
    public:
        blob(const std::string& name, int32_t size,
             int64_t delay_destroy = 10000) : file_mapping_data(name, size,
                         delay_destroy) {
                         
                         
        }
        
        blob(const std::string& name) : file_mapping_data(name) {
        
        }
        
    };
}

#endif // AMO_BLOB_99BF98E9_40DA_4030_AA45_248E4C15B9CF_HPP__

