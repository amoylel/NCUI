#include "module/ConstructorMgr.h"
#include "nan/nan.h"


namespace amo {

    void ConstructorMgr::addConstructor(const std::string& name,
                                        std::shared_ptr<Nan::Persistent<v8::Function> > function) {
        m_oConstructorMap[name] = function;
    }
    
    bool ConstructorMgr::hasConstruct(const std::string& name) {
        return m_oConstructorMap.find(name) != m_oConstructorMap.end();
    }
    
    std::shared_ptr<Nan::Persistent<v8::Function> >
    ConstructorMgr::getConstructor(const std::string& name) {
        return m_oConstructorMap[name];
    }
    
}

