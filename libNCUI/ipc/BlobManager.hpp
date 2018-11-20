// Created by amoylel on 11/17/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_BLOBMANAGER_9C392578_192C_4690_8ED7_013527092A89_HPP__
#define AMO_BLOBMANAGER_9C392578_192C_4690_8ED7_013527092A89_HPP__

#include <unordered_map>
#include <memory>
#include <mutex>

#include <amo/singleton.hpp>
#include <amo/blob.hpp>
namespace amo {

    template<typename ValueType>
    class BlobManagerBase  {
    public:
        BlobManagerBase() {
        
        }
        
        std::shared_ptr<ValueType> create(const std::string& name, int32_t size,
                                          int64_t delay = 10000) {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            std::shared_ptr<ValueType> ptr(new ValueType(name, size, delay));
            m_blobs[ptr->get_name()] = ptr;
            return ptr;
        }
        
        std::shared_ptr<ValueType> create(int32_t size, int64_t delay = 10000) {
            return create("", size, delay);
        }
        
        std::shared_ptr<ValueType> find(const std::string& name) {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            auto iter = m_blobs.find(name);
            
            if (iter != m_blobs.end()) {
                return iter->second;
            }
            
            return{};
            
        }
        
        void removeInvalidObject() {
            std::unique_lock<std::recursive_mutex> lock(m_mutex);
            
            for (auto iter = m_blobs.begin(); iter != m_blobs.end();) {
                if (iter->second->timeout()) {
                    iter = m_blobs.erase(iter);
                } else  {
                    ++iter;
                }
            }
        }
        
    private:
        std::recursive_mutex m_mutex;
        std::unordered_map<std::string, std::shared_ptr<ValueType> > m_blobs;
    };
    
    
    template<cef_process_id_t>
    class BlobManager : public BlobManagerBase<amo::blob> {
    
    };
    
    template<> class BlobManager<PID_RENDERER>
        : public BlobManagerBase<amo::blob>
        , public singleton<BlobManager<PID_RENDERER> > {
    public:
        BlobManager() {
        
        }
    };
    
    template<> class BlobManager<PID_BROWSER>
        : public BlobManagerBase<amo::blob>
        , public singleton<BlobManager<PID_BROWSER> > {
    public:
        BlobManager() {
        
        
        }
    };
    
    
    
    template<cef_process_id_t>
    class BigStrManager : public BlobManagerBase<amo::bigstr> {
    
    };
    
    template<> class BigStrManager<PID_RENDERER>
        : public BlobManagerBase<amo::bigstr>
        , public singleton<BigStrManager<PID_RENDERER> > {
    public:
        BigStrManager() {
        
        }
    };
    
    template<> class BigStrManager<PID_BROWSER>
        : public BlobManagerBase<amo::bigstr>
        , public singleton<BigStrManager<PID_BROWSER> > {
    public:
        BigStrManager() {
        
        
        }
    };
}


#endif // AMO_BLOBMANAGER_9C392578_192C_4690_8ED7_013527092A89_HPP__
