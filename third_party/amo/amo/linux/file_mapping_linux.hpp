// Created by amoylel on 29/09/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_FILE_MAPPING_LINUX_6F6FFB4F_E0AC_46C3_A725_5652F8977E15_HPP__
#define AMO_FILE_MAPPING_LINUX_6F6FFB4F_E0AC_46C3_A725_5652F8977E15_HPP__






#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstddef>
#include <cstdlib>

#include <amo/config.hpp>
#include <amo/path.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>

namespace amo {


    class file_mapping {
    public:
    
    public:
        const static int LenSize = 0;
        const static int HeaderSize = 1000;
    public:
        static int GetReservedSize() {
            return LenSize + HeaderSize ;
        }
        
        static bool remove(const std::string& object_name) {
            return boost::interprocess::file_mapping::remove(object_name.c_str());
        }
    public:
    
        file_mapping(const std::string& name,
                     bool open_only = false,
                     int map_size = 1024) {
            lpMapAddr = lpHeaderAddr = NULL;
            
            amo::path p(name);
            
            m_nMapSize = map_size + GetReservedSize();		//共享内存大小
            m_strName = name;			//共享内存名称
            
            if (!p.parent().is_root()) {
                p.parent().create_directory();
            }
            
            
            if (!p.exists()) {
                //Create a file
                std::filebuf fbuf;
                fbuf.open(name, std::ios_base::in | std::ios_base::out
                          | std::ios_base::trunc | std::ios_base::binary);
                //Set the size
                fbuf.pubseekoff(m_nMapSize - 1, std::ios_base::beg);
                fbuf.sputc(0);
                
            }
            
            init_file_mapping(open_only);
        }
        
        
        ~file_mapping() {
        
            remove_shared_memory();
        }
        
        bool remove_shared_memory() {
            // 关闭共享内存
            //return true;
            return boost::interprocess::file_mapping::remove(m_strName.c_str());
            
        }
        
        void set_locked(bool locked) {
            return;
        }
        
        
        bool is_opened() const {
            return m_file  && lpMapAddr != NULL &&  m_nMapSize > 0;
        }
        
        bool open(bool open_only = false) {
            if (is_opened()) {
                return true;
            }
            
            init_file_mapping(open_only);
            return is_opened();
        }
        
        int size() const {
            return m_nMapSize;
        }
        
        int capcity() const {
            return m_nMapSize + GetReservedSize();
        }
        
        int read(char* buffer, int begin_pos, int size) {
            if (!is_opened()) {
                return 0;
            }
            
            if (begin_pos + size > m_nMapSize) {
                size = m_nMapSize - begin_pos;
            }
            
            memcpy(buffer, lpMapAddr + begin_pos, size);
            return size;
        }
        
        char* address(int begin_pos = 0) {
            if (!is_opened()) {
                return 0;
            }
            
            if (begin_pos > m_nMapSize) {
                return NULL;
            }
            
            return lpMapAddr + begin_pos;
        }
        
        bool write(char* buffer, int begin_pos, int size) {
            if (!is_opened()) {
                return false;
            }
            
            if (begin_pos + size > m_nMapSize) {
                return false;
            }
            
            
            
            std::memcpy(lpMapAddr + begin_pos, buffer, size);
            /*  m_region->flush(0, 1000, false);
              m_region->flush(0, 0, false);*/
            m_region->flush(0, 0, false);
            //open_file_mapping();
            //m_region.reset();
            return true;
        }
        
    protected:
    
        void init_file_mapping(bool openOnly) {
        
            try {
                if (openOnly) {
                    open_file_mapping();
                }
                
                else {
                    if (open_file_mapping()) {
                        return;
                    }
                    
                    create_file_mapping();
                }
                
            } catch (const std::exception& e) {
                m_error_str = e.what();
                m_file.reset();
            }
            
            
        }
        
        bool open_file_mapping() {
        
            try {
            
                m_file.reset(new boost::interprocess::file_mapping(
                                 m_strName.c_str(),
                                 boost::interprocess::read_write));
                                 
                //m_file->get_mapping_handle()
                m_region.reset(new boost::interprocess::mapped_region(*m_file,
                               boost::interprocess::read_write));
                               
                               
                // 获取共享内存大小
                lpHeaderAddr = (char*)m_region->get_address();
                lpMapAddr = lpHeaderAddr + GetReservedSize();
                m_nMapSize = m_region->get_size() - GetReservedSize();
                
                return true;
            } catch (const std::exception& e) {
                m_error_str = e.what();
                m_file.reset();
                return false;
            }
            
        }
        
        bool create_file_mapping() {
            try {
            
                m_file.reset(new boost::interprocess::file_mapping(
                                 m_strName.c_str(),
                                 boost::interprocess::read_write));
                m_region.reset(new boost::interprocess::mapped_region(*m_file,
                               boost::interprocess::read_write));
                               
                memset((char*)m_region->get_address(), 0, m_region->get_size());
                
                // 获取共享内存大小
                lpHeaderAddr = (char*)m_region->get_address();
                lpMapAddr = lpHeaderAddr + GetReservedSize();
                m_nMapSize = m_region->get_size() - GetReservedSize();
                
                
                return true;
                
            } catch (const std::exception& e) {
                m_error_str = e.what();
                m_file.reset();
                return false;
            }
        }
    private:
    
    
    
        char* lpMapAddr;
        char* lpHeaderAddr;
        
        std::string m_strName;
        int m_nMapSize;
        std::string m_error_str;
        
        std::shared_ptr<boost::interprocess::file_mapping> m_file;
        std::shared_ptr<boost::interprocess::mapped_region> m_region;
    };
}



#endif //AMO_FILE_MAPPING_LINUX_6F6FFB4F_E0AC_46C3_A725_5652F8977E15_HPP__


