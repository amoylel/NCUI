// Created by amoylel on 10/04/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_BASIC_JSON_HELPER_0DBC1095_3071_47C5_8AEB_F0FDDF691105_HPP__
#define AMO_BASIC_JSON_HELPER_0DBC1095_3071_47C5_8AEB_F0FDDF691105_HPP__


#include <string>
#include <vector>
#include <memory>
#include <amo/json.hpp>
#include <amo/string.hpp>



#ifndef  JSON_HELPER_CONTAINER_DECLARE
#define  JSON_HELPER_CONTAINER_DECLARE(ContainerType)\
void json_from_type(const ContainerType  & val, basic_json<CodingType>& json, const std::string& key) { \
    return json_from_container_type< ContainerType >(val, json, key);\
}\
\
\
static ContainerType type_from_json(basic_json<CodingType>& json, const std::string& key, const ContainerType& default_val) { \
    return container_type_from_json< ContainerType   >( json, key, default_val);\
}\

#endif





namespace amo {

    template<typename CodingType>
    class basic_json_object : public basic_json<CodingType> {
    public:
        typedef CodingType coding_type;
    public:
    
        template<typename T>
        static T type_from_json(const basic_json<CodingType>& json,
                                const std::string& key,
                                const T& default_val) {
                                
            return json.get(key, default_val);
        }
        
        template<typename T>
        static T smart_type_from_json(const basic_json<CodingType>& json,
                                      const std::string& key) {
                                      
            basic_json<CodingType> child = json.get_child(key);
            
            if (!child.is_valid() || !child.is_object()) {
                return T();
            }
            
            return typename T::element_type::fromJson(child);
            
        }
        
        template<typename ContainerType>
        static ContainerType container_type_from_json(const basic_json<CodingType>&
                json,
                const std::string& key, const ContainerType& default_val) {
            ContainerType vec = default_val;
            
            basic_json<CodingType> child = json.get_child(key);
            
            if (!child.is_array()) {
                return vec;
            }
            
            vec.clear();
            std::vector<basic_json<CodingType> > arr = child.to_array();
            
            for (auto& p : arr) {
                vec.push_back(p.get_impl(typename ContainerType::value_type()));
            }
            
            return vec;
            
        }
        
        template<typename T>
        static void json_from_type(const T& val, basic_json<CodingType>& json,
                                   const std::string& key) {
            json.put(key, val);
            return;
        }
        
        
        
        template<typename T>
        static void json_from_smart_type(const T& val, basic_json<CodingType>& json,
                                         const std::string& key) {
            if (!val) {
                return;
            }
            
            json.put(key, val->toJson());
            return;
        }
        
        template<typename ContainerType>
        static void json_from_container_type(const ContainerType& val,
                                             basic_json<CodingType>& json,
                                             const std::string& key) {
                                             
            basic_json<CodingType> arr;
            arr.set_array();
            
            for (auto& p : val) {
                arr.push_back(p);
            }
            
            json.put(key, arr);
            return;
        }
        
        JSON_HELPER_CONTAINER_DECLARE(std::vector<std::string>)
        JSON_HELPER_CONTAINER_DECLARE(std::vector<int>)
        JSON_HELPER_CONTAINER_DECLARE(std::vector<double>)
        JSON_HELPER_CONTAINER_DECLARE(std::vector<float>)
        JSON_HELPER_CONTAINER_DECLARE(std::vector<int64_t>)
    public:
        static std::string format(std::unordered_map<std::string, std::string>&
                                  key_val_map,
                                  std::vector<std::string >& key_val_vec,
                                  basic_json<CodingType>& json,
                                  std::function<std::string(const std::string&, const std::string&)> callback = {},
                                  bool h = true,
                                  int align = 0,  /*0 all 1 header only 2 body only*/
                                  int mode = 0,
                                  const std::string& split_str = "    ") {
                                  
            amo::basic_string<coding_type> format_str;
            
            if (h) {
                amo::basic_string<coding_type> line1;
                amo::basic_string<coding_type> line2;
                
                for (size_t i = 0; i < key_val_vec.size(); i += 2) {
                    line1 += key_val_vec[i] + split_str;
                    line2 += key_val_vec[i + 1] + split_str;
                }
                
                if (mode == 0) {
                    format_str = line1 + split_str + "\n" + line2 + "\n";
                } else if (mode == 1) {
                    format_str = line1;
                } else if (mode == 2) {
                    format_str = line2 + "\n";
                }
                
            } else {
                for (size_t i = 0; i < key_val_vec.size(); i += 2) {
                    format_str += key_val_vec[i] + ":" + split_str;
                    format_str += key_val_vec[i + 1] + "\n";
                }
                
                format_str += "\n";
                
                // 重新计算字符串长度
                std::vector<std::string> map_keys;
                std::vector<std::string> map_vals;
                
                for (auto& p : key_val_map) {
                    map_keys.push_back(p.first);
                    map_vals.push_back(p.second);
                }
                
                amo::string_utils::make_same_length(map_vals, align);
                
                for (size_t i = 0; i < map_keys.size(); ++i) {
                    key_val_map[map_keys[i]] = map_vals[i];
                }
                
                
            }
            
            
            
            auto keys = json.keys();
            
            
            basic_json<CodingType> keyval;
            
            for (auto& p : keys) {
                auto iter = key_val_map.find(p + "Name");
                
                if (iter == key_val_map.end()) {
                    continue;
                }
                
                std::string val = "";
                
                if (json.is_int64(p)) {
                    val = std::to_string(json.getInt64(p));
                }
                
                if (json.is_double(p)) {
                    val = std::to_string(json.getDouble(p));
                } else if (json.is_string(p)) {
                    val = json.getString(p);
                }
                
                if (callback) {
                    val = callback(p, val);
                }
                
                std::vector<std::string> strs;
                strs.push_back(iter->second);
                strs.push_back(val);
                
                if (h) {
                    amo::string_utils::make_same_length(strs, align);
                }
                
                
                if (mode == 0) {
                    keyval.put(p + "Name", strs[0]);
                    keyval.put(p, strs[1]);
                } else if (mode == 1) {
                    keyval.put(p + "Name", strs[0]);
                } else if (mode == 2) {
                    keyval.put(p, strs[1]);
                }
                
            }
            
            /*   for (auto& p : key_val_map) {
            std::cout << p.second << std::endl;
            std::cout << amo::string(p.second, false).to_utf8() << std::endl;
            }*/
            
            /*  std::cout << "............." << std::endl;
            
            std::cout << keyval.to_utf8().to_string() << std::endl;
            std::cout << keyval.to_string() << std::endl;
            std::cout << format_str.str() << std::endl;
            std::cout << format_str.format(keyval).c_str() << std::endl;*/
            
            return format_str.format(keyval).to_locale();
        }
    public:
    
        /**
        * @fn	virtual void basic_json_object::onToJson(basic_json<CodingType>& json) const
        *
        * @brief	当前对象被转换为时被调用该函数，你可以重载该函数实现宏无法实现的功能，如自定义类，指针之类的数据.
        *
        * @param	The JSON.
        */
        
        virtual void onToJson(basic_json<CodingType>& json) const {
            return;
        }
        
        /**
        * @fn	virtual void basic_json_object::onFromJson(basic_json<CodingType>& json)
        *
        * @brief	当JSON被转换为对象时被调用该函数，你可以重载该函数实现宏无法实现的功能，如自定义类，指针之类的数据.
        *
        * @param 	The JSON.
        */
        
        virtual void onFromJson(basic_json<CodingType>& json) {
            return;
        }
        
        virtual basic_json<CodingType> toJson() const {
            return basic_json<CodingType>();
        }
        
        virtual std::string toString() const {
            return toJson().to_string();
        }
        
        virtual std::string formatJson(bool h = true, int align = 0, int mode = 0,
                                       const std::string& split_str = "    ") {
            return "";
        }
        
        virtual	std::string onFormatJson(const std::string& key,
                                         const std::string& val) {
            return val;
        }
        
    };
}

// 注册宏 开始
#define AMO_ENTITY_ARGS_SET_BEGIN(ClassName)\
	virtual void joinJson(const basic_json<typename ClassName :: coding_type>& other)  {\
		typedef ClassName :: coding_type CodingType ;\
		typedef ClassName ClassType;\
		basic_json<CodingType> json = this->toJson();\
		json.join(other);\
		*this = *ClassType::fromJson(json);\
		return;\
	}\
	virtual amo::basic_json<typename ClassName :: coding_type> toJson() const{\
		amo::basic_json<typename ClassName :: coding_type> json = *this;\

#ifndef AMO_ENTITY_ARGS_SET2
#define  AMO_ENTITY_ARGS_SET2(val, type) (json_from_type< type >(this->val, json, #val));
#endif

#ifndef AMO_ENTITY_ARGS_SET
#define  AMO_ENTITY_ARGS_SET(val) AMO_ENTITY_ARGS_SET2(val,decltype(this->val))
#endif

#ifndef AMO_ENTITY_ARGS_SMART_SET
#define  AMO_ENTITY_ARGS_SMART_SET(val) (json_from_smart_type<decltype(this->val)>(this->val, json, #val));
#endif

#ifndef AMO_ENTITY_ARGS_VEC_SET
#define  AMO_ENTITY_ARGS_VEC_SET(val) (json_from_container_type<decltype(this->val)>(this->val, json, #val));
#endif

// 注册宏 结束
#define AMO_ENTITY_ARGS_SET_END()\
		this->onToJson(json); \
		return json;\
	}\
	virtual std::string toString() const {\
		return toJson().to_string();\
	}


#define AMO_ENTITY_ARGS_SET_EXTEND(SuperClass)\
	{\
		typedef SuperClass SuperType;\
		typedef typename SuperClass :: coding_type CodingType;\
		amo::basic_json<CodingType> other = SuperType ::toJson();\
		json.join(other);\
}


#define AMO_ENTITY_ARGS_GET_BEGIN(ClassName)\
	static std::shared_ptr<ClassName> fromJson(const basic_json<typename ClassName :: coding_type>& json, std::shared_ptr<ClassName> ptr = {}, bool callOnFromJson = true) {\
		typedef typename ClassName :: coding_type CodingType;\
		typedef ClassName ClassType;\
		if(!ptr) ptr.reset(new ClassType());\
		amo::basic_json<CodingType> val = json;\
		ptr->join(val);\

//#ifndef AMO_ENTITY_ARGS_GET2
//#define  AMO_ENTITY_ARGS_GET2(key, type) (ptr->key = type_from_json<decltype(ptr->key)>(json, #key, ptr->key));
//#endif


#ifndef AMO_ENTITY_ARGS_GET2
#define  AMO_ENTITY_ARGS_GET2(key, type) (ptr->key = type_from_json< type >(json, #key, ptr->key));
#endif


#ifndef AMO_ENTITY_ARGS_GET
#define  AMO_ENTITY_ARGS_GET(key) AMO_ENTITY_ARGS_GET2(key, decltype(ptr->key))
#endif


#ifndef AMO_ENTITY_ARGS_SMART_GET
#define  AMO_ENTITY_ARGS_SMART_GET(key) (ptr->key = smart_type_from_json<decltype(ptr->key)>(json, #key));
#endif

#ifndef AMO_ENTITY_ARGS_VEC_GET
#define  AMO_ENTITY_ARGS_VEC_GET(key) (ptr->key = container_type_from_json<decltype(ptr->key)>(json, #key, ptr->key));
#endif

#define AMO_ENTITY_ARGS_GET_END()\
		if(callOnFromJson){\
			ptr->onFromJson(val);\
		}\
		return ptr;\
	}


#define AMO_ENTITY_ARGS_GET_EXTEND(SuperClass)\
	{\
		typedef SuperClass SuperType;\
		SuperType ::fromJson(json, ptr, false);\
	}



#ifndef AMO_ENTITY_FORMAT_BEGIN
#define AMO_ENTITY_FORMAT_BEGIN(ClassName)\
	virtual std::string formatJson(bool h = true, int align = 0, int mode = 0, const std::string& split_str = "    ") override {\
		typedef ClassName ClassType;\
		typedef typename ClassName :: coding_type CodingType;\
		std::unordered_map<std::string, std::string> key_val_map;\
		std::vector<std::string > key_val_vec;\
		std::function<std::string(const std::string&, const std::string&)> callback;\
		callback = std::bind(&ClassType::onFormatJson, this, std::placeholders::_1, std::placeholders::_2);\

#endif

#ifndef AMO_ENTITY_FORMAT_SET
#define AMO_ENTITY_FORMAT_SET(key, desc) \
		{\
			std::string map_key = #key;\
			map_key += "Name";\
			amo::basic_string<CodingType> map_val( desc, true );\
			std::string format_key_name = "{";\
			format_key_name += #key;\
			format_key_name += "Name";\
			format_key_name += "}";\
			std::string format_key_val = "{";\
			format_key_val += #key;\
			format_key_val += "}";\
			key_val_map[map_key] = map_val;\
			key_val_vec.push_back(format_key_name);\
			key_val_vec.push_back(format_key_val);\
		}
#endif

#ifndef AMO_ENTITY_FORMAT_CALLBACK_SET
#define AMO_ENTITY_FORMAT_CALLBACK_SET(func)\
		callback = std::bind(&ClassType::func, this, std::placeholders::_1, std::placeholders::_2);
#endif



#ifndef AMO_ENTITY_FORMAT_END
#define AMO_ENTITY_FORMAT_END()\
		amo::basic_json<CodingType> json = toJson();\
		return basic_json_object::format(key_val_map, key_val_vec, json, callback, h, align, mode, split_str);\
	}

#endif


#endif // AMO_BASIC_JSON_HELPER_0DBC1095_3071_47C5_8AEB_F0FDDF691105_HPP__

