#include "stdafx.h"
#include "RegistryTransfer.h"

namespace amo {

    RegistryTransfer::RegistryTransfer()
        : RunnableTransfer("Registry") {
        
    }
    
    RegistryTransfer::RegistryTransfer(int hKey)
        : RunnableTransfer("Registry") {
        m_registry.reset(new amo::registry((HKEY)hKey));
    }
    
    Any RegistryTransfer::onCreateClass(IPCMessage::SmartType msg) {
        int hKey = msg->getArgumentList()->getInt(0);
        
        if (hKey < (int)HKEY_CLASSES_ROOT) {
            return Undefined();
        }
        
        auto pTransfer = ClassTransfer::createTransfer<RegistryTransfer>(hKey);
        return pTransfer->getFuncMgr().toSimplifiedJson();
    }
    
    std::string RegistryTransfer::getClass() const {
        return "Registry";
    }
    
    amo::Transfer* RegistryTransfer::getInterface(const std::string& name) {
        if (name == RegistryTransfer::getClass()) {
            return this;
        }
        
        return RunnableTransfer::getInterface(name);
    }
    
    Any RegistryTransfer::create(IPCMessage::SmartType msg) {
        auto args = msg->getArgumentList();
        amo::astring subKey = amo::string(args->getString(0), true);
        return m_registry->create(subKey);
    }
    
    Any RegistryTransfer::open(IPCMessage::SmartType msg) {
        auto args = msg->getArgumentList();
        amo::astring subKey = amo::string(args->getString(0), true);
        return m_registry->open(subKey, KEY_ALL_ACCESS);
        
    }
    
    Any RegistryTransfer::read(IPCMessage::SmartType msg) {
        auto args = msg->getArgumentList();
        amo::astring subKey = amo::string(args->getString(0), true);
        amo::astring valueKey = amo::string(args->getString(1), true);
        amo::astring retval =  m_registry->read<std::string>(subKey, valueKey);
        return amo::string(retval, false).to_utf8();
    }
    
    Any RegistryTransfer::write(IPCMessage::SmartType msg) {
        auto args = msg->getArgumentList();
        amo::astring subKey = amo::string(args->getString(0), true);
        amo::astring valueKey = amo::string(args->getString(1), true);
        amo::astring value = amo::string(args->getString(2), true);
        bool autoCreate = args->getBool(3);
        return m_registry->write(subKey, valueKey, value, autoCreate);
    }
    
    Any RegistryTransfer::remove(IPCMessage::SmartType msg) {
        auto args = msg->getArgumentList();
        amo::astring subKey = amo::string(args->getString(0), true);
        amo::astring valueKey = amo::string(args->getString(1), true);
        
        if (valueKey.empty()) {
            return m_registry->remove(subKey);
        } else {
            return m_registry->remove(subKey, valueKey);
        }
    }
    
}

