#include "stdafx.h"

#include "ui/win/SharedMemory.h"

namespace amo {



    SharedMemory::~SharedMemory() {
        decInstanceCount();
    }
    
    SharedMemory::SharedMemory(const std::string& name)
        : amo::file_mapping(amo::string(name, false)) {
        incInstanceCount();
    }
    
    char SharedMemory::getInstanceCount() {
        char count = 0;
        read(&count, 0, 1);
        return count;
    }
    
    void SharedMemory::incInstanceCount() {
        char count = getInstanceCount();
        ++count;
        write(&count, 0, 1);
        count = getInstanceCount();
    }
    
    void SharedMemory::decInstanceCount() {
        char count = getInstanceCount();
        --count;
        write(&count, 0, 1);
    }
}