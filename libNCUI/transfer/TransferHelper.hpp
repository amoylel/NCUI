// Created by amoylel on 11/24/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_TRANSFERHELPER_HPP__
#define AMO_TRANSFERHELPER_HPP__

#include <ipc/Any.hpp>


namespace amo {
    template<typename T>
    struct is_smart_type {
        const static bool value = false;
    };
    
    template<> struct is_smart_type<std::shared_ptr<int> > {
        const static bool value = true;
    };
    
    template<> struct is_smart_type<std::shared_ptr<amo::date_time> > {
        const static bool value = true;
    };
    
    
    
    template<typename T>
    Any SetConfigValueImpl(T& ptr, Any& val, ValidAny) {
        ptr = val.As<T>();
        return Undefined();
    }
    
    template<typename T>
    Any SetConfigValueImpl(T& ptr, Any& val, InvalidAny) {
        if (!ptr) {
            ptr.reset(new typename T::element_type());
        }
        
        *ptr = val;
        return Undefined();
    }
    
    template<typename T>
    Any SetConfigValue(T& ptr, Any& val) {
        return SetConfigValueImpl(ptr, val, typename GetAnyType<T>::Type());
    }
    
    template<typename T>
    Any GetConfigValueImpl(const T& ptr, ValidAny) {
        return ptr;
    }
    
    template<typename T>
    Any GetConfigValueImpl(const T& ptr, InvalidAny) {
        if (is_smart_type<T>::value) {
            if (!ptr) {
                return Nil();
            } else {
                return *ptr;
            }
        }
        
        return Nil();
    }
    
    template<typename T>
    Any  GetConfigValue(T& ptr) {
        return GetConfigValueImpl(ptr, typename GetAnyType<T>::Type());
    }
}


#ifndef AMO_CONFIG_GETTER_SETTER
#define AMO_CONFIG_GETTER_SETTER(item)\
	Any item (IPCMessage::SmartType msg) {\
		std::shared_ptr<AnyArgsList> args = msg->getArgumentList();\
		Any& val = args->getValue(0);\
		if (val.is<Nil>() || val.is<Undefined>()) {\
			return GetConfigValue < decltype(this-> m_##item ) > (this-> m_##item );\
		} else {\
			return SetConfigValue < decltype(this-> m_##item ) > (this-> m_##item , val);\
		}\
	}
#endif

#ifndef AMO_CONFIG_GETTER_SETTER2
#define AMO_CONFIG_GETTER_SETTER2(type,item)\
	Any item (IPCMessage::SmartType msg) {\
		std::shared_ptr<AnyArgsList> args = msg->getArgumentList();\
		Any& val = args->getValue(0);\
		if (val.is<Nil>() || val.is<Undefined>()) {\
			return GetConfigValue < type> (this-> m_##item );\
		} else {\
			return SetConfigValue < type > (this-> m_##item , val);\
		}\
	}
#endif

#ifndef AMO_CONFIG_GETTER_SETTER3
#define AMO_CONFIG_GETTER_SETTER3( type,item, ptr)\
	Any item (IPCMessage::SmartType msg) {\
		std::shared_ptr<AnyArgsList> args = msg->getArgumentList();\
		Any& val = args->getValue(0);\
		if (val.is<Nil>() || val.is<Undefined>()) {\
			if (!ptr) return Nil();\
			return GetConfigValue < type> (ptr -> item );\
		} else {\
			if (!ptr) return Undefined();\
			return SetConfigValue < type > (ptr -> item , val);\
		}\
	}
#endif

#endif // AMO_TRANSFERHELPER_HPP__