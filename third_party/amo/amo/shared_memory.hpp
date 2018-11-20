// Created by amoylel on 31/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_SHARED_MEMORY_96059AEC_A87C_463A_9202_F25775AA6CDC_HPP__
#define AMO_SHARED_MEMORY_96059AEC_A87C_463A_9202_F25775AA6CDC_HPP__

#include <string>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/detail/managed_memory_impl.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <cstdlib> //std::system



namespace amo {

    class shared_memory  {
    public:
        //Define an STL compatible allocator of ints that allocates from the managed_shared_memory.
        //This allocator will allow placing containers in the segment
        typedef boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager>
        BufferAllocator;
        
        //Alias a vector that uses the previous STL-like allocator so that allocates
        //its values from the segment
        typedef std::vector<char, BufferAllocator> BufferType;
    public:
        const static int LenSize = 4;
        const static int HeaderSize = 1024;
        const static int BoostReservedSize = 1024;
    public:
        static int GetReservedSize() {
            return LenSize + HeaderSize + BoostReservedSize;
        }
        
        static bool remove(const std::string& object_name) {
            return boost::interprocess::shared_memory_object::remove(object_name.c_str());
        }
    public:
    
        shared_memory(const std::string& name,
                      bool open_only = false,
                      int map_size = 1024) {
            m_nMapSize = map_size + GetReservedSize();		//共享内存大小
            m_strName = name;			//共享内存名称
            
            init_shared_memory(open_only);
        }
        
        
        ~shared_memory() {
        
            remove_shared_memory();
        }
        
        bool remove_shared_memory() {
            // 关闭共享内存
            
            int instance_count = dec_instance_count();
            
            m_segment.reset();
            
            if (instance_count <= 0) {
                return boost::interprocess::shared_memory_object::remove(m_strName.c_str());
            }
            
            return true;
        }
        
        void set_locked(bool locked) {
            return;
        }
        
        int read(char* buffer, int begin_pos, int size) {
            if (!is_opened()) {
                return 0;
            }
            
            BufferType *myvector = m_segment->find<BufferType>("Buffer").first;
            auto size2 = myvector->size();
            auto* data = myvector->data();
            
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
        
        bool write(const char* buffer, int begin_pos, int size) {
            if (!is_opened()) {
                return false;
            }
            
            if (begin_pos + size > m_nMapSize) {
                return false;
            }
            
            memcpy(lpMapAddr + begin_pos, buffer, size);
            return true;
        }
        
        bool is_opened() const {
            return m_segment  &&  lpMapAddr != NULL && m_nMapSize > 0;
        }
        
        bool open(bool open_only = false) {
            if (is_opened()) {
                return true;
            }
            
            init_shared_memory(open_only);
            return is_opened();
        }
        
        int size() const {
            return m_nMapSize;
        }
        
        int capcity() const {
            return m_nMapSize + GetReservedSize();
        }
        
        int instance_count() const {
            if (!m_segment) {
                return 0;
            }
            
            int* instanceCount = m_segment->find<int>("Instances").first;
            
            if (instanceCount == NULL) {
                return 0;
            }
            
            if (*instanceCount <= 0) {
                return 0;
            }
            
            return *instanceCount;
        }
        
        int dec_instance_count()   {
            if (!m_segment) {
                return 0;
            }
            
            int* instanceCount = m_segment->find<int>("Instances").first;
            
            if (instanceCount == NULL) {
                return 0;
            }
            
            --(*instanceCount);
            
            
            if (*instanceCount <= 0) {
                return 0;
            }
            
            return *instanceCount;
        }
        
        int inc_instance_count() {
            if (!m_segment) {
                return 0;
            }
            
            int* instanceCount = m_segment->find<int>("Instances").first;
            
            if (instanceCount == NULL) {
                return 0;
            }
            
            ++(*instanceCount);
            
            
            if (*instanceCount <= 0) {
                return 0;
            }
            
            return *instanceCount;
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
        void init_shared_memory(bool openOnly) {
        
            try {
                if (openOnly) {
                    open_shared_memory();
                }
                
                else {
                    if (open_shared_memory()) {
                        return;
                    }
                    
                    create_shared_memory();
                }
                
            } catch (const std::exception& e) {
                m_error_str = e.what();
                m_segment.reset();
            }
            
            
        }
        
        bool open_shared_memory() {
        
            try {
            
                m_segment.reset(new boost::interprocess::managed_shared_memory(
                                    boost::interprocess::open_only,
                                    m_strName.c_str()));
                                    
                // 获取共享内存大小
                m_nMapSize = m_segment->get_size();
                
                
                BufferType *myvector = m_segment->find<BufferType>("Buffer").first;
                inc_instance_count();
                
                assert(myvector->size() == m_nMapSize - BoostReservedSize);
                
                lpHeaderAddr = myvector->data() + LenSize;
                lpMapAddr = myvector->data() + LenSize + HeaderSize;
                m_nMapSize = myvector->size() - LenSize - HeaderSize;
                
                return true;
            } catch (const std::exception& e) {
                m_error_str = e.what();
                m_segment.reset();
                return false;
            }
            
        }
        
        bool create_shared_memory() {
            try {
            
                // 先尝试关闭共享内存
                remove_shared_memory();
                
                m_segment.reset(new boost::interprocess::managed_shared_memory(
                                    boost::interprocess::open_or_create,
                                    m_strName.c_str(),
                                    m_nMapSize));
                                    
                                    
                const BufferAllocator alloc_inst(m_segment->get_segment_manager());
                BufferType *myvector = m_segment->construct<BufferType>("Buffer")(alloc_inst);
                
                if (myvector->size() != m_nMapSize - BoostReservedSize) {
                    myvector->resize(m_nMapSize - BoostReservedSize);
                }
                
                
                lpHeaderAddr = myvector->data() + LenSize;
                lpMapAddr = myvector->data() + LenSize + HeaderSize;
                m_nMapSize = myvector->size() - LenSize - HeaderSize;
                
                // 设置实例数
                m_segment->construct<int>("Instances")(1);
                
                return true;
                
            } catch (const std::exception& e) {
                m_error_str = e.what();
                m_segment.reset();
                return false;
            }
        }
    private:
        std::string m_strName;
        int m_nMapSize;
        std::string m_error_str;
        char* lpMapAddr;
        
        char* lpHeaderAddr;
        std::shared_ptr<boost::interprocess::managed_shared_memory> m_segment;
    };
}



#endif //AMO_SHARED_MEMORY_96059AEC_A87C_463A_9202_F25775AA6CDC_HPP__