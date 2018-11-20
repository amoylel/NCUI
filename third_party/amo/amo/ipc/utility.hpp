// Created by amoylel on 14/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_UTILITY_8E0408A3_7EDF_4FA7_8BF6_08AB08D44EBE_HPP__
#define AMO_UTILITY_8E0408A3_7EDF_4FA7_8BF6_08AB08D44EBE_HPP__




#include <string>
#include <sstream>
#include <amo/string.hpp>
#include <amo/ipc/any.hpp>
#include <amo/json.hpp>

namespace amo {
    namespace ipc {
    
        class util {
        public:
        
        
            void add_any_to_json_array(amo::json& json, const any& val) const  {
                switch (val.type()) {
                case  any_value_type<undefined>::value:
                case  any_value_type<nil>::value:
                    json.push_back(val.toJson());
                    break;
                    
                case  any_value_type<bool>::value:
                    json.push_back(val.As<bool>());
                    break;
                    
                case  any_value_type<int>::value:
                    json.push_back(val.As<int>());
                    break;
                    
                case  any_value_type<double>::value:
                    json.push_back(val.As<double>());
                    break;
                    
                case  any_value_type<std::string>::value:
                    json.push_back(val.As<std::string>());
                    break;
                    
                case  any_value_type<amo::json>::value:
                    json.push_back(val.As<amo::json>());
                    break;
                    
                default:
                    break;
                }
                
                return;
            }
            
            void add_any_to_json(amo::json& json, const std::string& key,
                                 const any& val) const  {
                switch (val.type()) {
                case  any_value_type<undefined>::value:
                case  any_value_type<nil>::value:
                    json.put(key, val.toJson());
                    break;
                    
                case  any_value_type<bool>::value:
                    json.put(key, val.As<bool>());
                    break;
                    
                case  any_value_type<int>::value:
                    json.put(key, val.As<int>());
                    break;
                    
                case  any_value_type<double>::value:
                    json.put(key, val.As<double>());
                    break;
                    
                case  any_value_type<std::string>::value:
                    json.put(key, val.As<std::string>());
                    break;
                    
                case  any_value_type<amo::json>::value:
                    json.put(key, val.As<amo::json>());
                    break;
                    
                case  any_value_type<std::vector<any> >::value: {
                    amo::json arr;
                    arr.set_array();
                    std::vector<any> vec = val;
                    
                    for (size_t i = 0; i < vec.size(); ++i) {
                        add_any_to_json_array(arr, vec[i]);
                    }
                    
                    json.put(key, arr);
                }
                
                break;
                
                default:
                    break;
                }
                
            }
            
            any json_to_any(const amo::json& json)  const {
                if (json.is_bool()) {
                    return json.get<bool>();
                } else if (json.is_int()) {
                    return json.get<int>();
                }   else if (json.is_int64()) {
                    return json.get<int64_t>();
                }  else if (json.is_double()) {
                    return json.get<double>();
                } else if (json.is_string()) {
                    return json.get<std::string>();
                } else if (json.is_object()) {
                    return json;
                } else {
                    return undefined();
                }
            }
            
            any json_to_any(const amo::json& json, const std::string& key) const {
                amo::json val = json.getJson(key);
                return json_to_any(val);
                
            }
        };
    }
}

#endif //AMO_UTILITY_8E0408A3_7EDF_4FA7_8BF6_08AB08D44EBE_HPP__

