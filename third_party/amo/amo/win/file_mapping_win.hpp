// Created by amoylel on 08/29/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_FILE_MAPPING_HPP__
#define AMO_FILE_MAPPING_HPP__



#include <amo/config.hpp>
#include <amo/string.hpp>
#include <amo/utility.hpp>
#include <amo/mutex.hpp>

namespace amo {
    class file_mapping {
    public:
        const static int LenSize = 4;
        const static int HeaderSize = 1000;
    public:
        static int GetReservedSize() {
            return LenSize + HeaderSize;
        }
    public:
        file_mapping(const std::string& name,
                     bool open_only = false,
                     int map_size = 1024)
            : m_mutex(name + "_mutex") {
            m_nMapSize = map_size + GetReservedSize();		//共享内存大小
            m_strName = name;			//共享内存名称
            
            m_hMapFile = NULL;
            lpMapAddr = NULL;
            lpHeaderAddr = NULL;
            
            init_file_mapping(open_only);
        }
        
        
        ~file_mapping() {
            if (lpMapAddr != NULL) {
                UnmapViewOfFile(lpMapAddr);
                lpMapAddr = NULL;
            }
            
            if (m_hMapFile != NULL) {
                CloseHandle(m_hMapFile);
                m_hMapFile = NULL;
            }
            
        }
        
        void set_locked(bool locked) {
            return m_mutex.set_locked(locked);
        }
        
        //------------------------------------------------------------------------------
        // 方法:    read
        // 功能:
        // 访问:    public
        // 返回:    int
        // 参数:    char * buffer
        // 参数:    int begin_pos
        // 参数:    int size
        //
        // 说明:
        //
        //------------------------------------------------------------------------------
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
        
        /*!
         * @fn	char* file_mapping::address(int begin_pos = 0)
         *
         * @brief	Address the given begin position.
         *
         * @param	begin_pos	(Optional) the begin position.
         *
         * @return	null if it fails, else a pointer to a char.
         */
        
        char* address(int begin_pos = 0) {
            if (!is_opened()) {
                return 0;
            }
            
            if (begin_pos  > m_nMapSize) {
                return NULL;
            }
            
            return lpMapAddr + begin_pos;
        }
        
        /*!
         * @fn	bool file_mapping::write(char* buffer, int begin_pos, int size)
         *
         * @brief	Writes.
         *
         * @param [in,out]	buffer	If non-null, the buffer.
         * @param	begin_pos	  	The begin position.
         * @param	size		  	The size.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool write(char* buffer, int begin_pos, int size) {
            if (!is_opened()) {
                return false;
            }
            
            if (begin_pos + size > m_nMapSize) {
                return false;
            }
            
            memcpy(lpMapAddr + begin_pos, buffer, size);
            return true;
        }
        
        /*!
         * @fn	bool file_mapping::is_opened() const
         *
         * @brief	Query if this object is opened.
         *
         * @return	true if opened, false if not.
         */
        
        bool is_opened() const {
            return m_hMapFile && lpMapAddr != NULL && m_nMapSize > 0;
        }
        
        /*!
         * @fn	bool file_mapping::open(bool open_only = false)
         *
         * @brief	Opens the given open only.
         *
         * @param	open_only	(Optional) true to open only.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
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
        
        // 获取整个内存大小
        int capcity() const {
            // 数据空间+ 保留空间
            return m_nMapSize + GetReservedSize();
        }
        
        /*!
         * @fn	std::string file_mapping::read_header()
         *
         * @brief	获取头部数据.
         *
         * @return	A std::string.
         */
        
        std::string read_header() {
            char str[HeaderSize] = { 0 };
            memcpy(str, lpHeaderAddr, HeaderSize);
            return str;
        }
        
        /*!
         * @fn	bool file_mapping::write_header(const std::string& data)
         *
         * @brief	写入头部数据.
         *
         * @param	data	The data.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool write_header(const std::string& data) {
            if (data.empty()) {
                return false;
            }
            
            if (data.size() > HeaderSize - 1) {
                return false;
            }
            
            memset(lpHeaderAddr, 0, HeaderSize);
            strcpy(lpHeaderAddr, data.c_str());
            return true;
        }
        
    protected:
    
    
        //------------------------------------------------------------------------------
        // 方法:    init_file_mapping
        // 功能:
        // 访问:    protected
        // 返回:    void
        //
        // 说明:
        //
        //------------------------------------------------------------------------------
        void init_file_mapping(bool open_only) {
            //打开共享的文件对象。
            m_hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, m_strName.c_str());
            
            if (m_hMapFile) {
                //显示共享的文件数据。
                lpMapAddr = (char*)MapViewOfFile(m_hMapFile, FILE_MAP_ALL_ACCESS,
                                                 0, 0, 0);
                                                 
                // 获取共享内存大小
                m_nMapSize = amo::bytes_to_int<int>((unsigned char*)lpMapAddr);
                lpHeaderAddr = lpMapAddr + LenSize;
                // 获取真正的数据的起始地址
                lpMapAddr = lpMapAddr + GetReservedSize();
                
                
                
            } else if (!open_only) {
                //创建共享文件。
                m_hMapFile = CreateFileMappingA((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0,
                                                m_nMapSize, m_strName.c_str());
                                                
                //拷贝数据到共享文件里。
                lpMapAddr = (char*)MapViewOfFile(m_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
                memset(lpMapAddr, 0, m_nMapSize);
                std::vector<unsigned char> vec;
                vec  = amo::int_to_bytes(m_nMapSize);
                // 写入共享内存大小
                write((char*)vec.data(), 0, LenSize);
                lpHeaderAddr = lpMapAddr + LenSize;
                lpMapAddr = lpMapAddr + GetReservedSize();
            }
            
            m_nMapSize -= GetReservedSize();
        }
    protected:
        std::string m_strName;
        
        HANDLE m_hMapFile;
        
        char* lpMapAddr;
        
        char* lpHeaderAddr;
        
        int m_nMapSize;
        
        amo::process_mutex m_mutex;
    };
}


#endif // AMO_FILE_MAPPING_HPP__