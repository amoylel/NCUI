// Created by amoylel on 10/04/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_BASIC_JSON_0E4D4BB8_2C4C_4DAC_9764_11606B181646_HPP__
#define AMO_BASIC_JSON_0E4D4BB8_2C4C_4DAC_9764_11606B181646_HPP__



#ifndef RAPIDJSON_HAS_STDSTRING
#define RAPIDJSON_HAS_STDSTRING 1
#endif

#include <vector>
#include <string>
#include <iosfwd>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>




#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>

#include <nlohmann/json.hpp>

#include <amo/uuid.hpp>
#include <amo/stdint.hpp>
#include <amo/base64.hpp>
#include <amo/uuid.hpp>
#include <amo/date_time.hpp>

#include <amo/string/coding_type.hpp>

#include <amo/string/string_utils.hpp>


typedef nlohmann::json njson;

namespace amo {

    //class MyOStreamWrapper {
    //public:
    //    typedef char Ch;
    //    MyOStreamWrapper(std::ostream& os) : os_(os) {
    //    }
    //    Ch Peek() const {
    //        assert(false);
    //        return '\0';
    //    }
    //    Ch Take() {
    //        assert(false);
    //        return '\0';
    //    }
    //    size_t Tell() const {  }
    //    Ch* PutBegin() {
    //        assert(false);
    //        return 0;
    //    }
    //    void Put(Ch c) {
    //        os_.put(c);    // 1
    //    }
    //    void Flush() {
    //        os_.flush();    // 2
    //    }
    //    size_t PutEnd(Ch*) {
    //        assert(false);
    //        return 0;
    //    }
    //private:
    //    MyOStreamWrapper(const MyOStreamWrapper&);
    //    MyOStreamWrapper& operator=(const MyOStreamWrapper&);
    //    std::ostream& os_;
    //};
    
    template<typename CodingType>
    class basic_json {
    public:
        typedef std::string string_type;
        typedef CodingType coding_type;
    public:
        static   StringCodingType get_coding_type() {
            return CodingType::coding_type;
        }
        
        static bool is_utf8() {
            return CodingType::coding_type == StringCodingType::CODING_TYPE_UTF8;
        }
    public:
        typedef rapidjson::Value::MemberIterator  iterator;
        typedef rapidjson::Value::MemberIterator assoc_iterator;
        typedef const rapidjson::Value::ConstMemberIterator const_iterator;
        typedef rapidjson::Document::AllocatorType allocator_type;
        typedef rapidjson::Value::ValueIterator	value_iterator;
        typedef rapidjson::Value::ConstValueIterator	const_value_iterator;
    public:
        static basic_json from_file(const std::string& file) {
            try {
                basic_json json;
                json.parse(file);
                return json;
                
            } catch (const std::exception& e) {
                std::cerr << e.what() << std::endl;
                basic_json retval = basic_json();
                retval.set_valid(false);
                retval.set_error_msg(e.what());
                return retval;
            }
            
        }
        
        
    public:
    
        basic_json(bool is_boject = true) {
            m_valid = true;
            ignore_case = false;
            
            if (is_boject) {
                doc.SetObject();
            } else {
                set_array();
            }
        }
        
        
        basic_json(const std::string& str_json) {
            m_valid = true;
            ignore_case = false;
            
            if (str_json.size() > 1000) {
                std::stringstream stream;
                stream << str_json;
                parse(stream);
            } else {
                doc.Parse<0>(str_json.c_str());
                checkParseError();
            }
            
        }
        
        basic_json(const char* str_json) {
            m_valid = true;
            ignore_case = false;
            
            if (str_json == NULL) {
                return;
            }
            
            if (strlen(str_json) > 1000) {
                std::stringstream stream;
                stream << str_json;
                parse(stream);
            } else {
                doc.Parse<0>(str_json);
                checkParseError();
            }
        }
        
        basic_json(
            nlohmann::json::initializer_list_t init,
            bool type_deduction = true,
            nlohmann::json::value_t manual_type = nlohmann::json::value_t::array) {
            
            
            
            std::string str_json = nlohmann::json(init, type_deduction,
                                                  manual_type).dump();
            m_valid = true;
            ignore_case = false;
            
            if (str_json.size() > 1000) {
                std::stringstream stream;
                stream << str_json;
                parse(stream);
            } else {
                doc.Parse<0>(str_json.c_str());
                
                checkParseError();
            }
        }
        
        
        basic_json(const basic_json& rhs) {
        
            doc.CopyFrom(rhs.doc, get_allocator());
            m_valid = rhs.m_valid;
            m_error_msg = rhs.m_error_msg;
            ignore_case = rhs.ignore_case;
            checkParseError();
        }
        
        basic_json(std::istream& stream) {
            try {
                ignore_case = false;
                rapidjson::IStreamWrapper isw(stream);
                doc.ParseStream(isw);
                m_valid = true;
                checkParseError();
            } catch (const std::exception& e) {
                m_error_msg = e.what();
                m_valid = false;
                return;
            }
        }
        
        basic_json(const rapidjson::Document& document) {
        
            doc.CopyFrom(document, get_allocator());
            m_valid = true;
            ignore_case = false;
            checkParseError();
        };
        
        basic_json(const rapidjson::Value& val) {
        
            doc.CopyFrom(val, get_allocator());
            m_valid = true;
            ignore_case = false;
            checkParseError();
        }
        
        basic_json& operator =(const basic_json & rhs) {
        
            doc.CopyFrom(rhs.doc, get_allocator());
            m_valid = rhs.m_valid;
            ignore_case = rhs.ignore_case;
            m_error_msg = rhs.m_error_msg;
            return *this;
        }
        
        
        // 此函数有BUG
        // 好像改好了
        basic_json& join(const basic_json& val) {
            basic_json other = val;
            
            for (iterator iter = other.begin(); iter != other.end(); ++iter) {
                std::string name = iter->name.GetString();
                iterator it = find_member(name);
                
                if (it != this->end()) {
                    it->value.CopyFrom(iter->value, get_allocator());
                } else {
                
                    rapidjson::Value node;
                    node.CopyFrom(iter->value, get_allocator());
                    rapidjson::Value key;
                    key.CopyFrom(iter->name, get_allocator());
                    
                    doc.AddMember(key, node, get_allocator());
                    
                    //doc.AddMember(iter->name, iter->value, get_allocator());
                }
            }
            
            // 简单处理，重新复制一下可用
            /* basic_json json2 = *this;
            *this = json2;*/
            
            return *this;
        }
        
        
        bool parse(const std::string & fileName) {
            try {
#ifdef OS_WIN
                std::ifstream ifs(amo::string_utils::string_to_wide(fileName,
                                  get_coding_type()));
                rapidjson::IStreamWrapper isw(ifs);
                doc.ParseStream(isw);
#else
                std::ifstream ifs(amo::string_utils::string_to_ansi(fileName,
                                  get_coding_type()));
                
                rapidjson::IStreamWrapper isw(ifs);
                doc.ParseStream(isw);
#endif
                
                
                checkParseError();
            } catch (const std::exception& e) {
                m_valid = false;
                m_error_msg = e.what();
                return false;
            }
            
            return true;
            
        }
        
        
        bool parse(std::istream& stream) {
            try {
                ignore_case = false;
                rapidjson::IStreamWrapper isw(stream);
                doc.ParseStream(isw);
                m_valid = true;
                checkParseError();
            } catch (const std::exception& e) {
                m_valid = false;
                m_error_msg = e.what();
                return false;
            }
            
            return true;
        }
        
        
        
        basic_json to_utf8() const {
            return basic_json(amo::string_utils::string_to_utf8(this->to_string(),
                              get_coding_type()));
        }
        
        basic_json to_ansi() const {
            return basic_json(amo::string_utils::string_to_ansi(this->to_string(),
                              get_coding_type()));
        }
        
        /*static basic_json form_utf8(const basic_json& other) {
            return basic_json(amo::string_utils::utf8_to_ansi(other.to_string()));
        }*/
        
        void put_child(const std::string& key, const basic_json& val) {
            iterator iter = find_member(key);
            
            if (iter != end()) {
                iter->value.CopyFrom(val.doc, get_allocator());
            } else {
                rapidjson::Value name;
                name.SetString(key.c_str(), get_allocator());
                doc.AddMember(name, rapidjson::Value().CopyFrom(val.doc, get_allocator()),
                              get_allocator());
            }
            
        }
        
        basic_json get_child(const std::string& key) const {
            const_iterator iter = find_member(key);
            
            if (iter == end()) {
                return basic_json();
            }
            
            /*    rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            iter->value.Accept(writer);
            */
            //rapidjson::Value val(iter->value);
            
            //std::string json_str = buffer.GetString();
            return basic_json(iter->value);
        }
        
        allocator_type& get_allocator() {
            return doc.GetAllocator();
        }
        
        rapidjson::Document& get_document() {
            return doc;
        }
        
        iterator find_member(const std::string& key) {
            if (is_object()) {
                if (!ignore_case) {
                    return doc.FindMember(key.c_str());
                }
                
                std::vector<std::string> vec = keys();
                
                for (auto& p : vec) {
                    if (isSameKey(p, key)) {
                        return doc.FindMember(p.c_str());
                    }
                }
            }
            
            return end();
        }
        
        
        const_iterator find_member(const std::string& key) const {
        
            if (is_object()) {
                if (!ignore_case) {
                    return doc.FindMember(key.c_str());
                }
                
                
                std::vector<std::string> vec = keys();
                
                for (auto& p : vec) {
                    if (isSameKey(p, key)) {
                        return doc.FindMember(p.c_str());
                    }
                }
                
            }
            
            return end();
        }
        
        
        bool contains_key(const std::string& key) const {
            // key 只能对Object使用
            if (doc.IsObject()) {
                if (!ignore_case) {
                    return doc.FindMember(key.c_str()) != end();
                }
                
                std::vector<std::string> vec = keys();
                
                for (auto& p : vec) {
                    if (isSameKey(p, key)) {
                        return true;
                    }
                }
            }
            
            return false;
        }
        
        bool isSameKey(const std::string& key1, const std::string& key2) const {
            if (!ignore_case) {
                return key1 == key2;
            } else {
                return amo::string_utils::to_upper(key1) == amo::string_utils::to_upper(key2);
            }
        }
        
        std::vector<std::string> keys() const {
            std::vector<std::string> vec;
            
            for (auto iter = begin(); iter != end(); ++iter) {
                vec.push_back(iter->name.GetString());
            }
            
            return vec;
        }
        
        std::vector<std::pair<std::string, basic_json> > values() {
            std::vector<std::pair<std::string, basic_json> > vec;
            
            for (auto iter = begin(); iter != end(); ++iter) {
                vec.push_back(std::pair<std::string, basic_json>(
                                  iter->name.GetString(), basic_json(iter->value)));
                                  
            }
            
            return vec;
        }
        
        template<typename T>
        void put(const T& val) {
            put_impl(val);
        }
        
        void put_impl(const bool& val) {
            doc.SetBool(val);
            
        }
        
        void put_impl(const int& val) {
            doc.SetInt(val);
        }
        
        void put_impl(const uint32_t& val) {
            doc.SetUint(val);
        }
        
        void put_impl(const uint64_t& val) {
            doc.SetUint64(val);
        }
        
        void put_impl(const int64_t& val) {
            doc.SetInt64(val);
        }
        
        void put_impl(const double& val) {
            doc.SetDouble(val);
        }
        
        void put_impl(const float& val) {
            doc.SetDouble(val);
        }
        
        
        void put_impl(const std::string& val) {
            doc.SetString(val.c_str(), val.size(), get_allocator());
        }
        
        void put_impl(const amo::uuid& val) {
            std::string str = val.to_string();
            doc.SetString(str.c_str(), str.size(), get_allocator());
        }
        
        void put_impl(const amo::date_time& val) {
            std::string str = val.to_string();
            doc.SetString(str.c_str(), str.size(), get_allocator());
        }
        
        void put_impl(const char* val) {
            doc.SetString(val, strlen(val), get_allocator());
        }
        
        template<typename T>
        void put(const std::string& key, const T& val) {
            rapidjson::Value node;
            node = val;
            
            rapidjson::Value name;
            name.SetString(key.c_str(), get_allocator());
            
            iterator iter = find_member(key);
            
            if (iter != end()) {
                iter->value.CopyFrom(node, get_allocator());
            } else {
                doc.AddMember(name, node, get_allocator());
            }
            
        }
        
        void put(const std::string& key, const basic_json& val) {
            iterator iter = find_member(key);
            
            if (iter != end()) {
                iter->value.CopyFrom(val.doc, get_allocator());
            } else {
                rapidjson::Value name;
                name.SetString(key.c_str(), get_allocator());
                doc.AddMember(name, rapidjson::Value().CopyFrom(val.doc, get_allocator()),
                              get_allocator());
            }
            
        }
        
        void put(const std::string key, const std::string& val) {
        
            rapidjson::Value node;
            node.SetString(val.c_str(), val.size(), get_allocator());
            rapidjson::Value name;
            name.SetString(key.c_str(), get_allocator());
            
            iterator iter = find_member(key);
            
            if (iter != end()) {
                iter->value.CopyFrom(node, get_allocator());
            } else {
                doc.AddMember(name, node, get_allocator());
            }
            
            
        }
        
        void put(const std::string key, const char* val, size_t len = 0) {
            if (val == NULL) {
                return;
            }
            
            if (len == 0) {
                len = strlen(val);
            }
            
            rapidjson::Value node;
            node.SetString(val, len, get_allocator());
            rapidjson::Value name;
            name.SetString(key.c_str(), get_allocator());
            
            iterator iter = find_member(key);
            
            if (iter != end()) {
                iter->value.CopyFrom(node, get_allocator());
            } else {
                doc.AddMember(name, node, get_allocator());
            }
        }
        
        void put(const std::string& key, const amo::uuid& val) {
            put(key, val.to_string());
        }
        
        void put(const std::string& key, const amo::date_time& val) {
            put(key, val.to_string());
        }
        
        /*void put(const std::string& key, const long& val) {
        put<int32_t>(key, val);
        }
        
        void put(const std::string& key, const unsigned long& val) {
        put<uint64_t>(key, val);
        }*/
        
        /*void put(const std::string& key, const int64_t& val) {
        put<int32_t>(key, val);
        }
        
        void put(const std::string& key, const uint64_t& val) {
        put<uint64_t>(key, val);
        }*/
        
        void put(rapidjson::Value key, rapidjson::Value val) {
            iterator iter = find_member(key.GetString());
            
            if (iter != end()) {
                iter->value.CopyFrom(val, get_allocator());
            } else {
                doc.AddMember(key, val, get_allocator());
            }
            
        }
        
        template<typename T>
        T get() const {
            if (!is_value()) {
                return T();
            }
            
            return get_impl(T());
        }
        
        bool get_impl(bool)  const {
            if (doc.IsBool()) {
                return doc.GetBool();
            }
            
            return false;
        }
        
        int8_t get_impl(int8_t)  const {
            if (doc.IsNumber()) {
                if (doc.IsDouble()) {
                    return (int8_t)doc.GetDouble();
                } else if (doc.IsFloat()) {
                    return (int8_t)doc.GetFloat();
                } else if (doc.IsInt()) {
                    return (int8_t)doc.GetInt();
                } else if (doc.IsInt64()) {
                    return (int8_t)doc.GetInt64();
                } else if (doc.IsUint()) {
                    return (int8_t)doc.GetUint();
                } else if (doc.IsUint64()) {
                    return (int8_t)doc.GetUint64();
                }
            }
            
            return int8_t();
        }
        
        uint8_t get_impl(uint8_t)  const {
            if (doc.IsNumber()) {
                if (doc.IsDouble()) {
                    return (uint8_t)doc.GetDouble();
                } else if (doc.IsFloat()) {
                    return (uint8_t)doc.GetFloat();
                } else if (doc.IsInt()) {
                    return (uint8_t)doc.GetInt();
                } else if (doc.IsInt64()) {
                    return (uint8_t)doc.GetInt64();
                } else if (doc.IsUint()) {
                    return (uint8_t)doc.GetUint();
                } else if (doc.IsUint64()) {
                    return (uint8_t)doc.GetUint64();
                }
            }
            
            return uint8_t();
        }
        
        int16_t get_impl(int16_t)  const {
            if (doc.IsNumber()) {
                if (doc.IsDouble()) {
                    return (int16_t)doc.GetDouble();
                } else if (doc.IsFloat()) {
                    return (int16_t)doc.GetFloat();
                } else if (doc.IsInt()) {
                    return (int16_t)doc.GetInt();
                } else if (doc.IsInt64()) {
                    return (int16_t)doc.GetInt64();
                } else if (doc.IsUint()) {
                    return (int16_t)doc.GetUint();
                } else if (doc.IsUint64()) {
                    return (int16_t)doc.GetUint64();
                }
            }
            
            return int16_t();
        }
        uint16_t get_impl(uint16_t)  const {
            if (doc.IsNumber()) {
                if (doc.IsDouble()) {
                    return (uint16_t)doc.GetDouble();
                } else if (doc.IsFloat()) {
                    return (uint16_t)doc.GetFloat();
                } else if (doc.IsInt()) {
                    return (uint16_t)doc.GetInt();
                } else if (doc.IsInt64()) {
                    return (uint16_t)doc.GetInt64();
                } else if (doc.IsUint()) {
                    return (uint16_t)doc.GetUint();
                } else if (doc.IsUint64()) {
                    return (uint16_t)doc.GetUint64();
                }
            }
            
            return uint16_t();
        }
        
        int get_impl(int)  const {
            if (doc.IsNumber()) {
                if (doc.IsDouble()) {
                    return (int)doc.GetDouble();
                } else if (doc.IsFloat()) {
                    return (int)doc.GetFloat();
                } else if (doc.IsInt()) {
                    return (int)doc.GetInt();
                } else if (doc.IsInt64()) {
                    return (int)doc.GetInt64();
                } else if (doc.IsUint()) {
                    return (int)doc.GetUint();
                } else if (doc.IsUint64()) {
                    return (int)doc.GetUint64();
                }
            }
            
            return int();
        }
        
        uint32_t get_impl(uint32_t) const {
            if (doc.IsNumber()) {
                if (doc.IsDouble()) {
                    return (uint32_t)doc.GetDouble();
                } else if (doc.IsFloat()) {
                    return (uint32_t)doc.GetFloat();
                } else if (doc.IsInt()) {
                    return (uint32_t)doc.GetInt();
                } else if (doc.IsInt64()) {
                    return (uint32_t)doc.GetInt64();
                } else if (doc.IsUint()) {
                    return (uint32_t)doc.GetUint();
                } else if (doc.IsUint64()) {
                    return (uint32_t)doc.GetUint64();
                }
            }
            
            return int();
        }
        
        uint64_t get_impl(uint64_t) const {
            if (doc.IsNumber()) {
                return (uint64_t)doc.GetUint64();
            }
            
            return uint64_t();
        }
        
        int64_t get_impl(int64_t) const {
            if (doc.IsNumber()) {
                return (int64_t)doc.GetInt64();
            }
            
            return int();
        }
        
        double get_impl(double) const {
            if (doc.IsNumber()) {
                return doc.GetDouble();
            }
            
            return double();
        }
        
        float get_impl(float) const {
            if (doc.IsNumber()) {
                return (float)doc.GetDouble();
            }
            
            return float();
        }
        
        std::string get_impl(std::string) const {
            if (doc.IsString()) {
                return doc.GetString();
            }
            
            return std::string();
        }
        
        amo::date_time get_impl(amo::date_time) const {
        
            if (doc.IsString()) {
                std::string str = doc.GetString();
                return date_time::from_string(str);
            }
            
            if (doc.IsInt64()) {
                int64_t timestamp = doc.GetInt64();
                return date_time(timestamp);
            }
            
            
            return date_time(0);
        }
        
        amo::uuid get_impl(amo::uuid)  const {
            if (!doc.IsString()) {
                return amo::uuid::nil();
            }
            
            return amo::uuid::from_string(doc.GetString());
        }
        
        int get_type() const {
            return (int)doc.GetType();
        }
        
        bool is_null()   const {
            return doc.IsNull();
        }
        bool is_false()  const {
            return doc.IsFalse();
        }
        bool is_true()   const {
            return doc.IsTrue();
        }
        bool is_bool()   const {
            return doc.IsBool();
        }
        bool is_object() const {
            return doc.IsObject();
        }
        bool is_array()  const {
            return doc.IsArray();
        }
        bool is_number() const {
            return doc.IsNumber();
        }
        bool is_int()    const {
            return doc.IsInt();
        }
        bool is_uint()   const {
            return doc.IsUint();
        }
        bool is_int64()  const {
            return doc.IsInt64();
        }
        bool is_uint64() const {
            return doc.IsUint64();
        }
        bool is_double() const {
            return doc.IsDouble();
        }
        bool is_string() const {
            return doc.IsString();
        }
        
        
        bool is_null(const std::string& key)   const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsNull()) {
                return false;
            }
            
            return true;
        }
        bool is_false(const std::string& key)  const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsFalse()) {
                return false;
            }
            
            return true;
        }
        bool is_true(const std::string& key)   const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsTrue()) {
                return false;
            }
            
            return true;
        }
        bool is_bool(const std::string& key)   const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsBool()) {
                return false;
            }
            
            return true;
        }
        bool is_object(const std::string& key) const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsObject()) {
                return false;
            }
            
            return true;
        }
        bool is_array(const std::string& key)  const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsArray()) {
                return false;
            }
            
            return true;
        }
        bool is_number(const std::string& key) const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsNumber()) {
                return false;
            }
            
            return true;
        }
        bool is_int(const std::string& key)    const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsInt()) {
                return false;
            }
            
            return true;
        }
        bool is_uint(const std::string& key)   const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsUint()) {
                return false;
            }
            
            return true;
        }
        bool is_int64(const std::string& key)  const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsInt64()) {
                return false;
            }
            
            return true;
        }
        bool is_uint64(const std::string& key) const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsUint64()) {
                return false;
            }
            
            return true;
        }
        bool is_double(const std::string& key) const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsDouble()) {
                return false;
            }
            
            return true;
        }
        
        
        bool is_string(const std::string& key) const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsString()) {
                return false;
            }
            
            return true;
        }
        
        bool is_value() const {
            return !is_array() && !is_object() && !is_null();
            
        }
        
        template<typename T>
        T get(const std::string& key, T default_val = T()) const {
            return getImpl(key, default_val);
        }
        
        
        bool getBool(const std::string& key, bool default_val = false) const {
            return get<bool>(key, default_val);
        }
        
        std::string getString(const std::string& key,
                              const std::string& default_val = "") const {
            return get<std::string>(key, default_val);
        }
        
        basic_json getJson(const std::string& key,
                           const basic_json& default_val = basic_json()) const {
            return get<basic_json>(key, default_val);
        }
        
        
        char getChar(const std::string& key, const char& default_val = '0') const {
            return get<char>(key, default_val);
        }
        
        
        int8_t getInt8(const std::string& key, int8_t default_val = 0) const {
            return get<int8_t>(key, default_val);
        }
        
        uint8_t getUint8(const std::string& key, uint8_t default_val = 0)  const {
            return get<uint8_t>(key, default_val);
        }
        
        int16_t getInt16(const std::string& key, int16_t default_val = 0) const {
            return get<int16_t>(key, default_val);
        }
        
        uint16_t getUint16(const std::string& key, uint16_t default_val = 0)  const {
            return get<uint16_t>(key, default_val);
        }
        
        int32_t getInt(const std::string& key, int32_t default_val = 0) const {
            return get<int32_t>(key, default_val);
        }
        
        uint32_t getUint(const std::string& key, uint32_t default_val = 0)  const {
            return get<uint32_t>(key, default_val);
        }
        
        
        int64_t getInt64(const std::string& key, int64_t default_val = 0)  const {
            return get<int64_t>(key, default_val);
        }
        
        uint64_t getUint64(const std::string& key, uint64_t default_val = 0) const {
            return get<uint64_t>(key, default_val);
        }
        
        double getDouble(const std::string& key, double default_val = 0.0) const {
            return get<double>(key, default_val);
        }
        
        
        char getImpl(const std::string& key, char default_val) const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsInt()) {
                return default_val;
            }
            
            return (char)iter->value.GetInt();
        }
        
        bool getImpl(const std::string& key, bool default_val)  const {
            const_iterator iter = find_member(key);
            
            if (iter == end()) {
                return default_val;
            }
            
            if (iter->value.IsString()) {
                std::string val = iter->value.GetString();
                
                if (val == "true" || val == "TRUE" || val == "1") {
                    return true;
                }
                
                return false;
            } else if (iter->value.IsBool()) {
                return iter->value.GetBool();
                
            } else if (iter->value.IsInt()) {
                return (iter->value.GetInt() != 0);
            }
            
            return default_val;
        }
        
        const char* getImpl(const std::string& key, const char* default_val) const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsString()) {
                return default_val;
            }
            
            return iter->value.GetString();
        }
        
        std::string getImpl(const std::string& key, std::string default_val)  const {
        
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsString()) {
                return default_val;
            }
            
            size_t len = iter->value.GetStringLength();
            size_t len2 = strlen(iter->value.GetString());
            return std::string(iter->value.GetString(), len);
        }
        
        date_time getImpl(const std::string& key, date_time default_val)  const {
            const_iterator iter = find_member(key);
            
            if (iter == end()) {
                return default_val;
            }
            
            if (iter->value.IsString()) {
                std::string str = iter->value.GetString();
                return date_time::from_string(str);
                
            }
            
            if (iter->value.IsInt64()) {
                int64_t timestamp = iter->value.GetInt64();
                return date_time(timestamp);
            }
            
            return default_val;
        }
        
        
        int8_t getImpl(const std::string& key, int8_t default_val)  const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsInt()) {
                return default_val;
            }
            
            return (int8_t)iter->value.GetInt();
        }
        
        uint8_t getImpl(const std::string& key, uint8_t default_val)  const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsUint()) {
                return default_val;
            }
            
            return (uint8_t)iter->value.GetUint();
        }
        
        
        int16_t getImpl(const std::string& key, int16_t default_val)  const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsInt()) {
                return default_val;
            }
            
            return (int16_t)iter->value.GetInt();
        }
        
        uint16_t getImpl(const std::string& key, uint16_t default_val) const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsUint()) {
                return default_val;
            }
            
            return (uint16_t)iter->value.GetUint();
        }
        
        int32_t getImpl(const std::string& key, int32_t default_val) const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsInt()) {
                return default_val;
            }
            
            return iter->value.GetInt();
        }
        
        uint32_t getImpl(const std::string& key, uint32_t default_val)  const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsUint()) {
                return default_val;
            }
            
            return iter->value.GetUint();
        }
        
        
        int64_t getImpl(const std::string& key, int64_t default_val) const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsInt64()) {
                return default_val;
            }
            
            return iter->value.GetInt64();
        }
        
        uint64_t getImpl(const std::string& key, uint64_t default_val) const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsUint64()) {
                return default_val;
            }
            
            return iter->value.GetInt64();
        }
        
        float getImpl(const std::string& key, float default_val)  const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsNumber()) {
                return default_val;
            }
            
            return (float)iter->value.GetDouble();
        }
        
        
        double getImpl(const std::string& key, double default_val)  const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsDouble()) {
                return default_val;
            }
            
            return iter->value.GetDouble();
        }
        
        
        /*long getImpl(const std::string& key, long default_val) const {
        const_iterator iter = find_member(key);
        
        if (iter == end() || !iter->value.IsNumber()) {
        return default_val;
        }
        
        return iter->value.GetInt64();
        }
        
        unsigned long getImpl(const std::string& key, unsigned long default_val) const {
        const_iterator iter = find_member(key);
        
        if (iter == end() || !iter->value.IsNumber()) {
        return default_val;
        }
        
        return iter->value.GetUint64();
        }
        
        
        */
        
        
        
        
        amo::uuid getImpl(const std::string& key, amo::uuid default_val)  const {
            const_iterator iter = find_member(key);
            
            if (iter == end() || !iter->value.IsString()) {
                return default_val;
            }
            
            return amo::uuid::from_string(iter->value.GetString());
        }
        
        basic_json getImpl(const std::string& key, basic_json default_val) const {
            const_iterator iter = find_member(key);
            
            if (iter == end()) {
                return default_val;
            }
            
            /* rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            iter->value.Accept(writer);
            std::string json_str = buffer.GetString();*/
            return basic_json(iter->value);
        }
        
        iterator begin() {
            return doc.MemberBegin();
        }
        
        const_iterator begin() const {
            return doc.MemberBegin();
        }
        
        iterator end() {
            return doc.MemberEnd();
        }
        
        const_iterator end() const {
            return doc.MemberEnd();
        }
        
        /**
        * @fn	std::vector<basic_json> to_array()
        *
        * @brief	将JSON 转换为JSON数组.
        *
        * @return	This object as a std::vector&lt;basic_json&gt;
        */
        
        std::vector<basic_json> to_array() const {
            std::vector<basic_json> vec;
            
            if (!is_array()) {
                return vec;
            }
            
            const_value_iterator iter = doc.Begin();
            
            for (const_value_iterator iter = doc.Begin(); iter != doc.End(); ++iter) {
                std::stringstream ss;
                rapidjson::OStreamWrapper osw(ss);
                rapidjson::Writer< rapidjson::OStreamWrapper> writer(osw);
                iter->Accept(writer);
                //std::string ss.str();
                /*  OutputDebugStringA(ss.str().c_str());
                OutputDebugStringA("\n");*/
                vec.push_back((ss));
            }
            
            return vec;
        }
        
        /**
        * @fn	int to_array(std::function<void(basic_json&)> fn)
        *
        * @brief	将JSON 转换为JSON数组.
        *
        * @param [in,out]	fn	The function.
        *
        * @return	fn as an int.
        */
        
        int to_array(std::function<void(basic_json&)> fn) {
        
            int nCount = 0;
            
            if (!is_array()) {
                return nCount;
            }
            
            value_iterator iter = doc.Begin();
            
            for (value_iterator iter = doc.Begin(); iter != doc.End(); ++iter) {
                std::stringstream ss;
                rapidjson::OStreamWrapper osw(ss);
                rapidjson::Writer< rapidjson::OStreamWrapper> writer(osw);
                iter->Accept(writer);
                //std::string ss.str();
                /*     OutputDebugStringA(ss.str().c_str());
                OutputDebugStringA("\n");*/
                ++nCount;
                basic_json val(ss);
                
                /*  if (nCount > 10000) {
                OutputDebugStringA(std::to_string(nCount).c_str());
                OutputDebugStringA("\n");
                }*/
                
                if (fn) {
                    fn(val);
                }
                
            }
            
            return nCount;
        }
        
        assoc_iterator not_found() {
            return doc.MemberEnd();
        }
        
        assoc_iterator find(std::string key) {
            return find_member(key);
        }
        
        std::string to_string() const {
            std::stringstream ss;
            rapidjson::OStreamWrapper osw(ss);
            rapidjson::Writer< rapidjson::OStreamWrapper> writer(osw);
            doc.Accept(writer);
            return ss.str();
        }
        
        void to_file(const std::string& filename) const {
        
#ifdef OS_WIN
        
            std::fstream ofs(amo::string_utils::string_to_wide(filename,
                             get_coding_type()), std::ios::out | std::ios::binary);
            rapidjson::OStreamWrapper osw(ofs);
            rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
            doc.Accept(writer);
            
#else
            std::fstream ofs(amo::string_utils::string_to_ansi(filename,
                             get_coding_type()), std::ios::out | std::ios::binary);
            rapidjson::OStreamWrapper osw(ofs);
            rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
            doc.Accept(writer);
#endif
            
            /*std::fstream ofs(filename, std::ios::out | std::ios::binary);
            rapidjson::OStreamWrapper osw(ofs);
            rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
            doc.Accept(writer);*/
            return;
        }
        
        void set_array() {
            doc.SetArray();
        }
        
        
        template<typename T>
        void push_back(const T& val) {
            rapidjson::Value v = (val);
            doc.PushBack(v, get_allocator());
        }
        
        
        
        void push_back(const bool& val) {
            rapidjson::Value v;
            v.SetBool(val);
            doc.PushBack(v, get_allocator());
        }
        
        void push_back(const char& val) {
            rapidjson::Value v;
            v.SetInt(val);
            doc.PushBack(v, get_allocator());
        }
        
        void push_back(const int8_t& val) {
            rapidjson::Value v;
            v.SetInt(val);
            doc.PushBack(v, get_allocator());
        }
        
        void push_back(const uint8_t& val) {
            rapidjson::Value v;
            v.SetUint(val);
            doc.PushBack(v, get_allocator());
        }
        
        void push_back(const int16_t& val) {
            rapidjson::Value v;
            v.SetInt(val);
            doc.PushBack(v, get_allocator());
        }
        
        void push_back(const uint16_t& val) {
            rapidjson::Value v;
            v.SetUint(val);
            doc.PushBack(v, get_allocator());
        }
        
        void push_back(const int& val) {
            rapidjson::Value v;
            v.SetInt(val);
            doc.PushBack(v, get_allocator());
        }
        
        void push_back(const int64_t& val) {
            rapidjson::Value v;
            v.SetInt64(val);
            doc.PushBack(v, get_allocator());
        }
        
        void push_back(const uint32_t& val) {
            rapidjson::Value v;
            v.SetUint(val);
            doc.PushBack(v, get_allocator());
        }
        
        void push_back(const uint64_t& val) {
            rapidjson::Value v;
            v.SetUint64(val);
            doc.PushBack(v, get_allocator());
        }
        
        
        void push_back(const float& val) {
            rapidjson::Value v;
            v.SetFloat(val);
            doc.PushBack(v, get_allocator());
        }
        
        void push_back(const double& val) {
            rapidjson::Value v;
            v.SetDouble(val);
            doc.PushBack(v, get_allocator());
        }
        
        void push_back(const char* val) {
            rapidjson::Value v;
            v.SetString(val, get_allocator());
            doc.PushBack(v, get_allocator());
        }
        
        void push_back(const std::string& val) {
            rapidjson::Value v;
            v.SetString(val.c_str(), get_allocator());
            doc.PushBack(v, get_allocator());
        }
        
        void push_back(const date_time& val) {
            std::string str = val.to_string();
            rapidjson::Value v;
            v.SetString(str.c_str(), get_allocator());
            doc.PushBack(v, get_allocator());
        }
        
        
        
        void push_back(const basic_json& val) {
            //basic_json js(val.to_string());
            rapidjson::Value v;
            v.CopyFrom(val.doc, get_allocator());
            doc.PushBack(v, get_allocator());
        }
        
        template<typename T>
        void push_back(const std::string& key, const std::vector<T>& val) {
            rapidjson::Value arr(rapidjson::kArrayType);
            
            for (int i = 0; i < val.size(); ++i) {
                rapidjson::Value v(val[i]);
                arr.PushBack(v, get_allocator());
            }
            
            rapidjson::Value name;
            name.SetString(key.c_str(), get_allocator());
            doc.AddMember(name, arr, get_allocator());
        }
        
        
        void push_back(const std::string& key, const std::vector<basic_json>& val) {
        
            rapidjson::Value arr(rapidjson::kArrayType);
            
            for (size_t i = 0; i < val.size(); ++i) {
                basic_json js(val[i].to_string());
                
                rapidjson::Value v;
                v.CopyFrom(js.get_document(), get_allocator());
                arr.PushBack(v, get_allocator());
                
            }
            
            rapidjson::Value name;
            name.SetString(key.c_str(), get_allocator());
            doc.AddMember(name, arr, get_allocator());
        }
        
        
        void push_back(const std::string& key, const std::vector<std::string>& val) {
        
            rapidjson::Value arr(rapidjson::kArrayType);
            
            for (size_t i = 0; i < val.size(); ++i) {
                rapidjson::Value v;
                v.SetString(val[i].c_str(), get_allocator());
                arr.PushBack(v, get_allocator());
                
            }
            
            rapidjson::Value name;
            name.SetString(key.c_str(), get_allocator());
            doc.AddMember(name, arr, get_allocator());
        }
        
        bool is_valid() const {
            return m_valid;
        }
        
        /*template<typename T> operator T() const;*/
        
        /*  operator basic_json<string_coding_ansi>() const {
              return  basic_json<string_coding_ansi>(amo::string_utils::string_to_ansi(this->to_string(), get_coding_type()));
          }
        
          operator basic_json<string_coding_utf8>() const {
              return  basic_json<string_coding_utf8>(amo::string_utils::string_to_utf8(this->to_string(), get_coding_type()));
          }*/
        
        operator bool() const {
            return is_valid();
        }
        
        
        
        
        void set_valid(bool valid = true) {
            m_valid = valid;
        }
        void clear() {
        
            if (doc.IsArray()) {
                doc.Clear();
            } else if (doc.IsObject()) {
                doc.RemoveAllMembers();
            }
        }
        
        void removeKey(const std::string& key) {
            if (is_object()) {
                doc.RemoveMember(key);
            }
        }
        bool empty() {
            if (doc.IsArray()) {
                return doc.Empty();
            } else if (doc.IsObject()) {
                return doc.ObjectEmpty();
            } else {
                return true;
            }
        }
        
        size_t size()const {
            if (doc.IsArray()) {
                return doc.Size();
            } else if (doc.IsObject()) {
                return doc.MemberCount();
            }
            
            return 0;
            
        }
        
        
        void erase(const std::string& key) {
            const_iterator iter = find_member(key);
            
            if (iter == end()) {
                return;
            }
            
            doc.RemoveMember(key.c_str());
        }
        
        void add_child(const std::string& key, basic_json& val) {
            return put_child(key, val);
        }
        void ignoreCase(bool ignore) {
            ignore_case = ignore;
        }
        
        bool ignoreCase() const {
            return ignore_case;
        }
        
        void set_error_msg(const std::string& msg) {
            m_error_msg = msg;
        }
        
        const std::string& get_error_msg() const {
            return m_error_msg;
        }
        
        std::string make_indent_str(int indent) const {
            std::vector<char> vec(indent, ' ');
            vec.push_back('\0');
            return vec.data();
        }
        
        std::string format(int base_indent = 4) const {
            return format(base_indent, base_indent, true);
        }
        
        std::string format(int base_indent, int indent, bool autoEnter) const {
            std::string retval;
            basic_json m_str = *this;
            
            // 简单格式化
            if (m_str.is_object()) {
                std::vector<std::string> keys;
                keys = m_str.keys();
                retval += "{\n";
                
                for (size_t i = 0; i < keys.size(); ++i) {
                    auto& p = keys[i];
                    retval += make_indent_str(indent) + "\"" + p + "\":" + m_str.getJson(
                                  p).format(base_indent,
                                            indent + base_indent, false);
                                            
                    if (i < keys.size() - 1) {
                        retval += ",";
                    }
                    
                    retval += "\n";
                }
                
                
                retval += make_indent_str(indent - base_indent) + "}";
            } else if (m_str.is_array()) {
                std::vector<basic_json> vec;
                vec = m_str.to_array();
                bool bHasEnter = false;
                
                retval += "[";
                
                for (size_t i = 0; i < vec.size(); ++i) {
                    auto & p = vec[i];
                    
                    if (p.is_string()) {
                        bHasEnter = true;
                        retval += "\n" + make_indent_str(indent) + p.format(base_indent,
                                  indent + base_indent, false);
                    } else {
                        retval += (p).format(base_indent, indent + base_indent, false);
                    }
                    
                    if (i < vec.size() - 1) {
                        retval += ", ";
                    }
                    
                }
                
                if (bHasEnter) {
                    retval += "\n" + make_indent_str(indent - base_indent) + "]";
                } else {
                    retval += "]";
                }
                
            } else {
                retval = m_str.to_string();
            }
            
            if (autoEnter) {
                retval += "\n";
            }
            
            return retval;
        }
        
    private:
        void checkParseError() {
            if (doc.HasParseError()) {
                m_error_msg = " parse error ("
                              + std::to_string(doc.GetParseError()) + ")"
                              + " position:[" + std::to_string(doc.GetErrorOffset()) + "]";
                this->set_valid(false);
            }
        }
        
    private:
        bool m_valid;
        bool ignore_case;
        rapidjson::Document doc;
        std::string m_error_msg;
    };
    
    
}


#endif // AMO_BASIC_JSON_0E4D4BB8_2C4C_4DAC_9764_11606B181646_HPP__

