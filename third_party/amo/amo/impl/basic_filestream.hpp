// Created by amoylel on 11/10/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_BASIC_FILESTREAM_E7EC3059_5395_4BA2_AFEC_F79B03462483_HPP__
#define AMO_BASIC_FILESTREAM_E7EC3059_5395_4BA2_AFEC_F79B03462483_HPP__



#include <memory>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iosfwd>
#include <utility>

#include <amo/type_traits.hpp>
#include <amo/string.hpp>
#include <amo/path.hpp>
#include <amo/config.hpp>

namespace amo {
    template<typename CodingType>
    class basic_filestream {
    public:
        typedef amo::basic_path<CodingType> path_type;
    public:
        /*ios::in	为输入(读)而打开文件
        ios::out	为输出(写)而打开文件
        ios::ate	初始位置:文件尾
        ios::app	所有输出附加在文件末尾
        ios::trunc	如果文件已存在则先删除该文件
        ios::binary	二进制方式*/
        /* filestream(const std::wstring& name,
        std::ios::openmode mode = std::ios::in | std::ios::out)
        : m_path(name)
        , m_openmode(mode) {
        open();
        
        }
        */
        basic_filestream(const path_type& name,
                         std::ios::openmode mode = std::ios::in | std::ios::out)
            : m_path(name)
            , m_openmode(mode) {
            open();
            
        }
        
        ~basic_filestream() {
            close();
        }
        
        
        bool is_open() {
            if (!ifs) {
                return false;
            }
            
            return ifs->is_open();
        }
        
        void close() {
            if (ifs) {
                ifs->close();
            }
            
            ifs.reset();
        }
        
        bool eof() const {
            if (!ifs) {
                return true;
            }
            
            return ifs->eof();
            
        }
        
        // 写入文件
        void write(const std::string& str) {
            if (!ifs) {
                return;
            }
            
            ifs->write(str.c_str(), str.size());
        }
        
        void write(const char* str, int nSize) {
            if (!ifs) {
                return;
            }
            
            ifs->write(str, nSize);
        }
        
        void write(const std::vector<int8_t>& vec) {
            if (!ifs) {
                return;
            }
            
            ifs->write((const char*)vec.data(), vec.size());
        }
        
        
        size_t size() const {
            return boost::filesystem::file_size(m_path.filesystem_path());
        }
        
        
        // 读取整个文件
        std::string read_all() {
            std::shared_ptr<std::fstream> ifs;
            $windows({ ifs.reset(new std::fstream(m_path.generic_wstring())); })
            $welse({ ifs.reset(new std::fstream(m_path.generic_string())); })
            size_t size = 	boost::filesystem::file_size(m_path.filesystem_path());
            std::string retval(size, '\0');
            
            read_some((char*)retval.data(), size);
            return retval;
            //return  std::move(std::string((char*)ifs->rdbuf(), size));
            
            /* std::stringstream buffer;
             buffer << std::string(ifs->rdbuf(), size);
            
             return std::move(buffer.str());*/
        }
        
        
        int read_some(char* buffer, int nCount) {
            if (!ifs) {
                return 0;
            }
            
            if (ifs->eof()) {
                return 0;
            }
            
            ifs->read(buffer, nCount);
            int nBytes = (int)ifs->gcount();
            return nBytes;
        }
        
        std::vector<int8_t> read_all_bytes() {
            std::vector<int8_t> vec;
            read_all_bytes(vec);
            return std::move(vec);
        }
        
        /**
        * @fn	bool filestream::read_all_bytes(std::vector<int8_t>& vec)
        *
        * @brief	Reads all bytes ，还是会被0截断.
        *
        * @param [in,out]	vec	The vector.
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        bool read_all_bytes(std::vector<int8_t>& vec) {
            std::shared_ptr<std::fstream> ifs;
            $windows({ ifs.reset(new std::fstream(m_path.generic_wstring())); })
            $welse({ ifs.reset(new std::fstream(m_path.generic_string())); })
            size_t size = boost::filesystem::file_size(m_path.filesystem_path());
            vec.resize(size);
            return  read_some((char*)vec.data(), size) == size;
            
            /*return  std::move(std::string(ifs->rdbuf(), size));
            
            if (!ifs->is_open()) {
                return false;
            }
            
            std::filebuf* pbuf = ifs->rdbuf();
            uint32_t size = (uint32_t)pbuf->pubseekoff(0, ifs->end, ifs->in);
            vec.resize(size);
            
            pbuf->pubseekoff(0, ifs->beg);
            pbuf->sgetn((char*)vec.data(), size);
            ifs->close();*/
            return true;
        }
        
        // 按行读取
        std::vector<std::string> read_all_lines(int max_len = 4096) {
            std::vector<std::string> vec;
            std::string str;
            
            while (read_line(str, max_len)) {
                vec.push_back(str);
            }
            
            return std::move(vec);
        }
        
        // 读取一行数据
        bool read_line(std::string& val, int max_len = 4096) {
        
            std::vector<char> vec(max_len, 0);
            
            if (ifs->getline(vec.data(), max_len)) {
                val = std::string(vec.data());
                return true;
            }
            
            return false;
        }
        
        // 不能判断是空行还是失败
        std::string read_line(int max_len = 4096) {
            std::string val;
            read_line(val, max_len);
            return val;
        }
        
        inline bool open() {
            if (!ifs) {
                $windows({ ifs.reset(new std::fstream(m_path.generic_wstring(), m_openmode)); })
                $welse({ ifs.reset(new std::fstream(m_path.generic_string(), m_openmode)); })
            }
            
            return ifs->is_open();
        }
        
        
        /**
        * @fn	const amo::astring& get_path() const
        *
        * @brief	Gets the path.
        *
        * @return	The path.
        */
        
        const path_type get_path() const {
            return m_path;
        }
        
        //////////////////////////////////////////////////////////////////////////
        
    private:
        path_type m_path;
        //std::wstring m_path;
        std::ios::openmode m_openmode;
        std::shared_ptr<std::fstream> ifs;
    };
}






#endif //AMO_BASIC_FILESTREAM_E7EC3059_5395_4BA2_AFEC_F79B03462483_HPP__