// Created by amoylel on 08/05/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_UTILITY_HPP2__
#define AMO_UTILITY_HPP2__

#include <string>
#include <sstream>
#include <amo/string.hpp>
#include <ipc/Any.hpp>
#include <amo/json.hpp>

namespace amo {

    class util {
    public:
        static std::string decodeUrl(const std::string &value) noexcept {
            std::string result;
            result.reserve(value.size() / 3 + (value.size() % 3)); // Minimum size of result
            
            for (std::size_t i = 0; i < value.size(); ++i) {
                auto &chr = value[i];
                
                if (chr == '%' && i + 2 < value.size()) {
                    auto hex = value.substr(i + 1, 2);
                    auto decoded_chr = static_cast<char>(std::strtol(hex.c_str(), nullptr, 16));
                    result += decoded_chr;
                    i += 2;
                } else if (chr == '+') {
                    result += ' ';
                } else {
                    result += chr;
                }
            }
            
            return result;
        }
        
        static amo::u8string getUrlFromUtf8(const std::string& url) {
            return amo::u8string(decodeUrl(url), true);
        }
        
        static bool isDevUrl(const std::string& url) {
            return url == "chrome-devtools://devtools/inspector.html";
        }
        
        
        static  void addAnyToJsonArray(amo::u8json& json, const Any& val)   {
            switch (val.type()) {
            case  AnyValueType<Undefined>::value:
            case  AnyValueType<Nil>::value:
                json.push_back((amo::u8json)val.toJson());
                break;
                
            case  AnyValueType<bool>::value:
                json.push_back(val.As<bool>());
                break;
                
            case  AnyValueType<int>::value:
                json.push_back(val.As<int>());
                break;
                
            case  AnyValueType<int64_t>::value:
                json.push_back(val.As<int64_t>());
                break;
                
            case  AnyValueType<double>::value:
                json.push_back(val.As<double>());
                break;
                
            case  AnyValueType<std::string>::value:
                json.push_back(val.As<std::string>());
                break;
                
                
            case  AnyValueType<amo::u8json>::value:
                json.push_back(val.As<amo::u8json>());
                break;
                
            case AnyValueType<std::vector<Any>>::value: {
                amo::u8json arr;
                arr.set_array();
                std::vector<Any> vec = val;
                
                for (size_t i = 0; i < vec.size(); ++i) {
                    addAnyToJsonArray(arr, vec[i]);
                }
                
                json.push_back(arr);
            }
            
            default:
                break;
            }
            
            return;
        }
        
        static  void addAnyToJson(amo::u8json& json, const std::string& key,
                                  const Any& val)   {
            switch (val.type()) {
            case  AnyValueType<Undefined>::value: {
                amo::u8json child = val.toJson();
                child.put("a02c2b36-3d70-44f6-9456-85dab0e0ddb5", 0);
                json.put(key, child);
                break;
            }
            
            case  AnyValueType<Nil>::value: {
                amo::u8json child = val.toJson();
                child.put("a02c2b36-3d70-44f6-9456-85dab0e0ddb5", 1);
                json.put(key, child);
                break;
            }
            
            
            case  AnyValueType<bool>::value:
                json.put(key, val.As<bool>());
                break;
                
            case  AnyValueType<int>::value:
                json.put(key, val.As<int>());
                break;
                
            case  AnyValueType<int64_t>::value:
                json.put(key, val.As<int64_t>());
                break;
                
            case  AnyValueType<uint64_t>::value:
                json.put(key, val.As<int64_t>());
                break;
                
            case  AnyValueType<double>::value:
                json.put(key, val.As<double>());
                break;
                
            case  AnyValueType<std::string>::value:
                json.put(key, val.As<std::string>());
                break;
                
            case  AnyValueType<amo::u8json>::value:
                json.put(key, val.As<amo::u8json>());
                break;
                
                
                
            case  AnyValueType<std::vector<Any> >::value: {
                amo::u8json arr;
                arr.set_array();
                std::vector<Any> vec = val;
                
                for (size_t i = 0; i < vec.size(); ++i) {
                    addAnyToJsonArray(arr, vec[i]);
                }
                
                json.put(key, arr);
            }
            
            break;
            
            default:
                break;
            }
            
        }
        
        static Any jsonToAny(const amo::u8json& json)   {
            if (json.is_bool()) {
                return json.get<bool>();
            } else if (json.is_int()) {
                return json.get<int>();
            }  else if (json.is_int64()) {
                return json.get<int64_t>();
            } else if (json.is_double()) {
                return json.get<double>();
            } else if (json.is_string()) {
                return json.get<std::string>();
            } else if (json.is_object()) {
                return json;
            } else {
                return Undefined();
            }
        }
        static  Any jsonToAny(const amo::u8json& json, const std::string& key)  {
            amo::u8json val = json.getJson(key);
            return jsonToAny(val);
            
        }
    };
    
}

#endif // AMO_UTILITY_HPP2__

