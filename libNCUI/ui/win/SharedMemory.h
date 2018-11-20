// Created by amoylel on 08/29/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_SHAREDMEMORY_H__
#define AMO_SHAREDMEMORY_H__





#include <amo/singleton.hpp>
#include <amo/file_mapping.hpp>

namespace amo {


    class SharedMemory : public amo::file_mapping
        , public amo::singleton < SharedMemory > {
    public:
        SharedMemory(const std::string& name);
        
        ~SharedMemory();
        
        /*!
         * @fn	char SharedMemory::getInstanceCount();
         *
         * @brief	获取实例数.
         *
         * @return	The instance count.
         */
        
        char getInstanceCount();
        
    protected:
    
        /*!
         * @fn	void SharedMemory::incInstanceCount();
         *
         * @brief	实例自加1.
         */
        
        void incInstanceCount();
        
        /*!
         * @fn	void SharedMemory::decInstanceCount();
         *
         * @brief	实例自减.
         */
        
        void decInstanceCount();
    private:
    
    };
    
}
#endif // AMO_SHAREDMEMORY_H__
