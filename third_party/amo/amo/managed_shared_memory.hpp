// Created by amoylel on 31/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_MANAGED_SHARED_MEMORY_5D48B397_5980_4FD2_90E7_754B6AE14271_HPP__
#define AMO_MANAGED_SHARED_MEMORY_5D48B397_5980_4FD2_90E7_754B6AE14271_HPP__






#include <string>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/detail/managed_memory_impl.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <cstdlib> //std::system
#include <utility>



namespace amo {

    class managed_shared_memory {
    public:
        //Define an STL compatible allocator of ints that allocates from the managed_shared_memory.
        //This allocator will allow placing containers in the segment
        typedef boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager>
        BufferAllocator;
        
        //Alias a vector that uses the previous STL-like allocator so that allocates
        //its values from the segment
        typedef std::vector<char, BufferAllocator> BufferType;
    public:
        const static int LenSize = 0;
        const static int HeaderSize = 0;
        const static int BoostReservedSize = 0;
    public:
        static int GetReservedSize() {
            return LenSize + HeaderSize + BoostReservedSize;
        }
        
        static bool remove(const std::string& object_name) {
            return boost::interprocess::shared_memory_object::remove(object_name.c_str());
        }
    public:
    
        managed_shared_memory(const std::string& name,
                              bool open_only = false,
                              int map_size = 1024) {
            m_nMapSize = map_size + GetReservedSize();		//共享内存大小
            m_strName = name;			//共享内存名称
            
            init_shared_memory(open_only);
        }
        
        
        ~managed_shared_memory() {
        
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
        
        bool contais_key(const std::string & name) {
            if (!m_segment) {
                return false;
            }
            
            std::string* instanceCount = m_segment->find<std::string>(name.c_str()).first;
            
            if (instanceCount == NULL) {
                return false;
            }
            
            return true;
            
        }
        
        void destroy(const std::string& name) {
        
            if (!m_segment) {
                return;
            }
            
            if (!contais_key(name)) {
                return;
            }
            
            try {
            
                m_segment->destroy<std::string>(name.c_str());
            } catch (const std::exception&) {
            
            }
            
        }
        
        void set_locked(bool locked) {
            return;
        }
        
        
        bool is_opened() const {
            return m_segment  &&   m_nMapSize > 0;
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
        
        int dec_instance_count() {
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
        
        std::string read(const std::string& name) {
            if (!m_segment) {
                return "";
            }
            
            std::pair<std::string*, boost::interprocess::managed_shared_memory::size_type>
            res;
            res = m_segment->find<std::string>(name.c_str());
            
            if (res.first == NULL) {
                return "";
            }
            
            return *res.first;
        }
        
        bool write(const std::string& name, const std::string& val) {
            try {
                if (!m_segment) {
                    return false;
                }
                
                if ((int)val.size() > m_nMapSize) {
                    return false;
                }
                
                std::pair<std::string*, boost::interprocess::managed_shared_memory::size_type>
                res;
                res = m_segment->find<std::string>(name.c_str());
                
                if (res.first == NULL) {
                    m_segment->construct<std::string>(name.c_str())(val);
                } else {
                    *res.first = val;
                }
                
                return true;
            } catch (const std::exception&) {
                return false;
            }
            
            
        }
    protected:
    
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
                inc_instance_count();
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
        
        std::shared_ptr<boost::interprocess::managed_shared_memory> m_segment;
    };
}



#endif //AMO_MANAGED_SHARED_MEMORY_5D48B397_5980_4FD2_90E7_754B6AE14271_HPP__