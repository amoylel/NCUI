// Created by amoylel on 14/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_ANY_4752CEEE_AFC1_4408_8855_BAEDA0FD3669_HPP__
#define AMO_ANY_4752CEEE_AFC1_4408_8855_BAEDA0FD3669_HPP__





#include <string>
#include <map>
#include <memory>
#include <vector>
#include <type_traits>
#include <amo/json.hpp>
#include <amo/nil.hpp>
#include <amo/stdint.hpp>
#include <amo/date_time.hpp>
#include <amo/ipc/ipc_args_pos_info.hpp>
#include <amo/blob.hpp>
#include <amo/bigstr.hpp>

/*!
 * @namespace	amo
 *
 * @brief	.
 */
namespace amo {

    /*!
     * @typedef	amo::nil nil
     *
     * @brief	函数无返回值时，线程/进程之间同步时 返回nil表示void.
     * 			对应JS中的null
     */
    typedef amo::nil nil;
    
    /*!
     * @class	undefined
     *
     * @brief	调用C++时返回undefined对应JS中的undefined.
     */
    class undefined {};
    
    /*!
     * @class	nonreturn
     *
     * @brief	JS调用C++不需要向JS返回结果，如异步回调函数.
     */
    class nonreturn {};
    
    /*!
     * @class	nothing
     *
     * @brief	JS调用C++不需要向JS返回结果，跟nonreturn差不多，分不清了.
     */
    class nothing {};
    
    /**
     * @class	deadlock
     *
     * @brief	进程死锁时的返回类型.
     */
    
    class deadlock {};
    
    /**
     * @class	breakout
     *
     * @brief	跳出循环.
     */
    
    class breakout {};
    
    /**
     * @class	cutout
     *
     * @brief	终止执行.
     */
    
    class cutout {};
    /*!
     * @class	unknown
     *
     * @brief	未知结果，正常不会出现.
     */
    class unknown {};
    
    // 前置声明
    class any;
    class ipc_msg;
    class ipc_result;
    
    /*!
     * @fn	template<typename T> static std::string value_to_string(const T& val)
     *
     * @brief	将类型转换成字符串.
     *
     * @tparam	T	类型.
     * @param	val	类型实例.
     *
     * @return	std::string.
     */
    template<typename T> static std::string value_to_string(const T& val) {
        amo::json json;
        json.put(val);
        return json.to_string();
    }
    /*!
     * @fn	template<typename T> static std::string any_to_string(const T& val);
     *
     * @brief	将任意类型转换为String.
     * 			将函数不能被调用，只能使用特化版本
     *
     * @tparam	T	类型.
     * @param	val	类型实例.
     *
     * @return	std::string.
     */
    template<typename T> static std::string any_to_string(const T& val);
    
    template<>  std::string any_to_string(const bool& val) {
        return value_to_string<bool>(val);
    }
    template<>  std::string any_to_string(const char& val) {
        return value_to_string<char>(val);
    }
    template<>  std::string any_to_string(const int& val) {
        return value_to_string<int>(val);
    }
    template<>  std::string any_to_string(const int64_t& val) {
        return value_to_string<int64_t>(val);
    }
    template<>  std::string any_to_string(const double& val) {
        return value_to_string<double>(val);
    }
    template<>  std::string any_to_string(const std::string& val) {
        return val;
    }
    template<>  std::string any_to_string<nil>(const nil& val) {
        return value_to_string<std::string>("nil");
    }
    template<>  std::string any_to_string<undefined>(const undefined& val) {
        return value_to_string<std::string>("undefined");
    }
    template<>  std::string any_to_string<nonreturn>(const nonreturn& val) {
        return value_to_string<std::string>("nonreturn");
    }
    template<>  std::string any_to_string<deadlock>(const deadlock& val) {
        return value_to_string<std::string>("deadlock");
    }
    template<>  std::string any_to_string<amo::blob>(const amo::blob& val) {
        return val.get_name();
    }
    template<>  std::string any_to_string<amo::bigstr>(const amo::bigstr& val) {
        return val.get_name();
    }
    template<>  std::string any_to_string<breakout>(const breakout& val) {
        return value_to_string<std::string>("breakout");
    }
    template<>  std::string any_to_string<cutout>(const cutout& val) {
        return value_to_string<std::string>("cutout");
    }
    template<>  std::string any_to_string<amo::json>(const amo::json& val) {
        return val.to_string();
    }
    template<>  std::string any_to_string<nothing>(const nothing& val) {
        return std::string();
    }
    
    template<>  std::string any_to_string<date_time>(const date_time& val) {
        return val.to_string();
    }
    /*!
     * @fn	template<typename T> static T string_to_value(const std::string& val)
     *
     * @brief	将字符串转换为所给类型.
     *
     * @tparam	T	类型.
     * @param	val	输入字符串.
     *
     * @return	目标类型值.
     */
    template<typename T> static T string_to_value(const std::string& val) {
        amo::json json(val);
        return json.get<T>();
    }
    /*!
     * @fn	template<typename T> static T string_to_any(const std::string& val);
     *
     * @brief	将字符串转换为任意类型.
     *
     * @tparam	T	目标类型.
     * @param	val	输入字符串.
     *
     * @return	目标类型值.
     */
    template<typename T> static T string_to_any(const std::string& val);
    template<>  bool string_to_any(const std::string& val) {
        return string_to_value<bool>(val);
    }
    template<>  char string_to_any(const std::string& val) {
        return string_to_value<char>(val);
    }
    template<>  int string_to_any(const std::string& val) {
        return string_to_value<int>(val);
    }
    template<>  int64_t string_to_any(const std::string& val) {
        return string_to_value<int64_t>(val);
    }
    template<>  double string_to_any(const std::string& val) {
        return string_to_value<double>(val);
    }
    template<>  std::string string_to_any(const std::string& val) {
        return val;
    }
    template<>  amo::json string_to_any(const std::string& val) {
        return amo::json(val);
    }
    template<>  nil string_to_any(const std::string& val) {
        return nil();
    }
    template<>  undefined string_to_any(const std::string& val) {
        return undefined();
    }
    template<>  deadlock string_to_any(const std::string& val) {
        return deadlock();
    }
    template<>  amo::blob string_to_any(const std::string& val) {
        return amo::blob(val);
    }
    template<>  amo::bigstr string_to_any(const std::string& val) {
        return amo::bigstr(val);
    }
    template<>  breakout string_to_any(const std::string& val) {
        return breakout();
    }
    template<>  cutout string_to_any(const std::string& val) {
        return cutout();
    }
    template<>  nonreturn string_to_any(const std::string& val) {
        return nonreturn();
    }
    template<>  nothing string_to_any(const std::string& val) {
        return nothing();
    }
    template<>  date_time string_to_any(const std::string& val) {
        return date_time::from_string(val);
    }
    /*!
     * @struct	any_value_type
     *
     * @brief	any类型判断类.
     *
     * @tparam	T	Generic type parameter.
     */
    template<typename T> struct any_value_type {
        static const char value = -1;
    };
    
    template<> struct any_value_type <unknown> {
        static const char value = -1;
    };
    
    ///
    template<> struct any_value_type <nothing> {
        static const char value = 0;
    };
    template<> struct any_value_type <bool> {
        static const char value = 1;
    };
    template<> struct any_value_type <char> {
        static const char value = 2;
    };
    template<> struct any_value_type <int> {
        static const char value = 3;
    };
    template<> struct any_value_type <int64_t> {
        static const char value = 4;
    };
    template<> struct any_value_type <double> {
        static const char value = 5;
    };
    template<> struct any_value_type <std::string> {
        static const char value = 6;
    };
    template<> struct any_value_type <const char*> {
        static const char value = 7;
    };
    
    // 辅助类
    template<> struct any_value_type <nil> {
        static const char value = 21;
    };
    template<> struct any_value_type <undefined> {
        static const char value = 22;
    };
    template<> struct any_value_type <nonreturn> {
        static const char value = 23;
    };
    template<> struct any_value_type <deadlock> {
        static const char value = 24;
    };
    template<> struct any_value_type <breakout> {
        static const char value = 25;
    };
    template<> struct any_value_type <cutout> {
        static const char value = 26;
    };
    
    template<> struct any_value_type < any > {
        static const char value = 30;
    };
    template<> struct any_value_type < ipc_msg > {
        static const char value = 31;
    };
    template<> struct any_value_type < ipc_result > {
        static const char value = 32;
    };
    
    // 自定义类型
    template<> struct any_value_type <std::map<std::string, std::string> > {
        static const char value = 40;
    };
    template<> struct any_value_type <amo::json> {
        static const char value = 41;
    };
    
    template<> struct any_value_type<std::vector<any> > {
        static const char value = 42;
    };
    template<> struct any_value_type <amo::date_time> {
        static const char value = 43;
    };
    template<> struct any_value_type <amo::blob> {
        static const char value = 44;
    };
    template<> struct any_value_type <amo::bigstr> {
        static const char value = 45;
    };
    /*!
     * @class	any_data
     *
     * @brief	any 数据.
     */
    class any_data {
    public:
        /*! @brief	实际类型. */
        char type;
        /*! @brief	序列化后的字符串. */
        std::string value;
    };
    /*!
     * @class	any
     *
     * @brief	any类，用于进程间数据通信统一类型.
     */
    class any  {
    public:
        any() {
            m_pData.reset(new any_data());
            m_pData->type = 0;
        }
        
        any(char type_)  {
            m_pData.reset(new any_data());
            m_pData->type = type_;
        }
        
        any(const any& rhs) {
            m_pData.reset(new any_data());
            m_pData->value = rhs.value();
            m_pData->type = rhs.type();
        }
        
        any(char type_, const std::string& val_)  {
            m_pData.reset(new any_data());
            m_pData->type = type_;
            m_pData->value = val_;
        }
        
        /*!
         * @fn	template<typename T> any::any(const T& val_)
         *
         * @brief	使用任意类型初始化.
         *
         * @tparam	T	Generic type parameter.
         * @param	val_	The value.
         */
        template<typename T> any(const T& val_) {
            m_pData.reset(new any_data());
            m_pData->type = any_value_type<T>::value;
            m_pData->value = any_to_string<T>(val_);
        }
        
        /*!
         * @fn	any::any(const char* val_)
         *
         * @brief	使用字符串初始化.
         *
         * @param	val_	The value.
         */
        any(const char* val_) {
            m_pData.reset(new any_data());
            m_pData->type = any_value_type<std::string>::value;
            m_pData->value = any_to_string<std::string>(val_);
        }
        
        ~any() { }
        
        any& operator=(const any& rhs) {
            m_pData->type = rhs.type();
            m_pData->value = rhs.value();
            return *this;
        }
        
        bool isValid() const {
            return type() != any_value_type<nothing>::value;
        }
        
        // 类型转换
        template<typename R> operator R() const {
            return string_to_any<R>(m_pData->value);
        }
        
        
        template<typename R> R As() const {
            return string_to_any<R>(m_pData->value);
        }
        
        // 类型判断
        template<typename T> bool is() const {
            return (type() == any_value_type<T>::value);
        }
        
        // 类型
        
        char& type() {
            return m_pData->type;
        }
        
        const char& type() const {
            return m_pData->type;
        }
        
        void type(const char& t) {
            m_pData->type = t;
        }
        
        // 数据
        
        std::string& value() {
            return m_pData->value;
        }
        
        const std::string& value() const {
            return m_pData->value;
        }
        
        void value(const std::string& val) {
            m_pData->value = val;
        }
        
        static any fromJson(amo::json& json) {
            char type = json.get<char>("type");
            std::string val = json.get<std::string>("value");
            return any(type, val);
        }
        
        amo::json toJson() const {
            amo::json json;
            json.put("type", m_pData->type);
            json.put("value", m_pData->value);
            return json;
        }
        
        any clone() {
            return any(type(), value());
        }
        
        /*! @brief	实际类型数据. */
        std::shared_ptr<any_data> m_pData;
    };
    
    /*!
     * @class	any_args_list
     *
     * @brief	进程消息中的消息参数列表.
     */
    class any_args_list {
    public:
        any_args_list() {
            m_Nil = nil();
        }
        /*!
         * @fn	void any_args_list::setValue(const int& index, const any& val)
         *
         * @brief	所给位置设置值.
         *
         * @param	index	位置.
         * @param	val  	值.
         */
        void setValue(const int& index, const any& val) {
            m_oMap[index] = val;
        }
        /*!
         * @fn	void any_args_list::ClearValue(const int& index)
         *
         * @brief	删除所给位置的参数值.
         *
         * @param	index	Zero-based index of the.
         */
        void clearValue(const int& index) {
            m_oMap.erase(index);
        }
        /*!
         * @fn	void any_args_list::ClearValue(const int& nBegin, const int& nEnd)
         *
         * @brief	删除所给范围值.
         *
         * @param	nBegin	The begin.
         * @param	nEnd  	The end.
         */
        void clearValue(const int& nBegin, const int& nEnd) {
            for (int i = nBegin; i < nEnd; ++i) {
                if (i >= ipc_args_pos_info::FuncName) {
                    return;
                }
                
                m_oMap.erase(i);
            }
            
            // TODO: 没有处理ARGSLENGTH
        }
        /*!
         * @fn	size_t any_args_list::GetSize() const
         *
         * @brief	获取当前列表的所有参数个数.
         *
         * @return	The size.
         */
        size_t getSize() const {
            return m_oMap.size();
        }
        
        std::map<int, any>& getAllValues() {
            return m_oMap;
        }
        /*!
         * @fn	int any_args_list::getArgsSize() const
         *
         * @brief	获取当前列表中函数参数个数.
         *
         * @return	The arguments size.
         */
        int getArgsSize() const {
            int nSize = 0;
            
            for (auto& p : m_oMap) {
                if (p.first < ipc_args_pos_info::FuncName) {
                    ++nSize;
                }
            }
            
            return nSize;
        }
        
        /*!
         * @fn	any& any_args_list::GetValue(const int& index)
         *
         * @brief	获取所给位置值.
         *
         * @param	index	索引.
         *
         * @return	The value.
         */
        any& getValue(const int& index) {
            auto iter = m_oMap.find(index);
            
            if (iter == m_oMap.end()) {
                return m_Nil;
            }
            
            return iter->second;
        }
        
        /*!
         * @fn	amo::json any_args_list::toJson() const
         *
         * @brief	返回当前对象的JSON对象，将参数列表转换为JSON数组.
         *
         * @return	This object as an amo::json.
         */
        amo::json toJson() const {
            amo::json json;
            json.set_array();
            
            for (auto& p : m_oMap) {
                amo::json args;
                args.put("index", p.first);
                args.put("type", p.second.type());
                args.put("value", p.second.value());
                json.push_back(args);
            }
            
            return json;
        }
        
        /*!
         * @fn	static std::shared_ptr<any_args_list> any_args_list::fromJson(amo::json& json)
         *
         * @brief	将JSON转换为any_args_list.
         *
         * @param [in]	json	The JSON.
         *
         * @return	std::shared_ptr<any_args_list>.
         */
        static std::shared_ptr<any_args_list> fromJson(amo::json& json) {
            std::shared_ptr<any_args_list> list(new any_args_list());
            
            if (!json.is_array()) {
                return list;
            }
            
            std::vector<amo::json> vec = json.to_array();
            
            for (auto& p : vec) {
                any val;
                val.type() = p.get<int>("type");
                val.value() = p.get<std::string>("value");
                list->setValue(p.get<int>("index"), val);
            }
            
            return list;
        }
        
        bool getBool(const int& nIndex) {
            return getValue(nIndex);
        }
        int getInt(const int& nIndex) {
            return getValue(nIndex);
        }
        int64_t getInt64(const int& nIndex) {
            return getValue(nIndex);
        }
        double getDouble(const int& nIndex) {
            return getValue(nIndex);
        }
        std::string getString(const int& nIndex) {
            any& val =  getValue(nIndex);
            
            if (val.type() == m_Nil.type()) {
                return "";
            }
            
            return val;
        }
        /*!
         * @fn	bool any_args_list::IsValid(const int& nIndex)
         *
         * @brief	所给索引参数是否存在.
         *
         * @param	nIndex	The index.
         *
         * @return	true if valid, false if not.
         */
        bool isValid(const int& nIndex) {
            auto iter = m_oMap.find(nIndex);
            
            if (iter == m_oMap.end()) {
                return false;
            }
            
            return true;
        }
        amo::json getJson(const int& nIndex) {
            return getValue(nIndex);
        }
        
        std::shared_ptr<any_args_list> clone() {
            std::shared_ptr<any_args_list> args(new any_args_list());
            
            for (auto& p : m_oMap) {
                args->setValue(p.first, p.second.clone());
            }
            
            return args;
            
        }
        
    private:
        std::map<int, any> m_oMap;
        any m_Nil;
    };
    
    /*!
     * @class	ipc_msg
     *
     * @brief	进程消息封装.
     */
    class ipc_msg {
    public:
        typedef std::shared_ptr<ipc_msg> SmartType;
    public:
        /*!
         * @fn	static int ipc_msg::GetProcessMessageID()
         *
         * @brief	生成消息ID.
         *
         * @return	生成的ID.
         */
        static int getProcessMessageID() {
            static int id = 0;
            static std::mutex mutex;
            return ++id;
        }
        
    public:
        ipc_msg() {
            m_nID = getProcessMessageID();
            
            m_pMessageList.reset(new any_args_list());
        }
        
        
        ~ipc_msg() {
        }
        
        /*!
         * @fn	int ipc_msg::getID() const
         *
         * @brief	获取消息ID.
         *
         * @return	消息ID.
         */
        int getID() const {
            return m_nID;
        }
        
        void setID(const int64_t& nID) {
            m_nID = nID;
        }
        
        /*!
         * @fn	static ipc_msg ipc_msg::fromJson(amo::json& json)
         *
         * @brief	将JSON转换为ipc_msg.
         *
         * @param [in]	json	The JSON.
         *
         * @return	An ipc_msg.
         */
        static ipc_msg fromJson(amo::json& json) {
            ipc_msg message;
            message.m_strMessageName = json.get<std::string>("name");
            message.m_nID = json.get<int>("id");
            amo::json args = json.get<amo::json>("args");
            message.m_pMessageList = any_args_list::fromJson(args);
            return message;
        }
        static ipc_msg::SmartType fromJson2(amo::json& json) {
            ipc_msg::SmartType message(new ipc_msg());
            message->m_strMessageName = json.get<std::string>("name");
            message->m_nID = json.get<int>("id");
            amo::json args = json.get<amo::json>("args");
            message->m_pMessageList = any_args_list::fromJson(args);
            return message;
        }
        static ipc_msg::SmartType Empty() {
            return std::shared_ptr<ipc_msg>(new ipc_msg());
        }
        /*!
         * @fn	const std::string& ipc_msg::getMessageName() const
         *
         * @brief	获取消息名称.
         *
         * @return	消息名称.
         */
        const std::string& getMessageName() const {
            return m_strMessageName;
        }
        
        /*!
         * @fn	const std::string& ipc_msg::GetName() const
         *
         * @brief	获取消息名称.
         *
         * @return	消息名称.
         */
        const std::string& getName() const {
            return m_strMessageName;
        }
        
        /*!
         * @fn	void ipc_msg::setMessageName(const std::string& name)
         *
         * @brief	设置消息名称.
         *
         * @param	name	消息名称.
         */
        void setMessageName(const std::string& name) {
            m_strMessageName = name;
        }
        
        /*!
         * @fn	std::shared_ptr<any_args_list>& ipc_msg::GetArgumentList()
         *
         * @brief	获取参数列表.
         *
         * @return	std::shared_ptr<any_args_list>.
         */
        std::shared_ptr<any_args_list>& getArgumentList() {
            return m_pMessageList;
        }
        
        /*!
         * @fn	void ipc_msg::SetArgumentList(std::shared_ptr<any_args_list> list)
         *
         * @brief	设置消息列表.
         *
         * @param	list	The list.
         */
        void setArgumentList(std::shared_ptr<any_args_list> list) {
            m_pMessageList = list;
        }
        
        amo::json toJson() const {
            amo::json json;
            json.put("name", m_strMessageName);
            json.put("id", m_nID);
            json.put_child("args", m_pMessageList->toJson());
            return json;
        }
        
        SmartType clone() {
            SmartType msg(new ipc_msg());
            msg->setMessageName(m_strMessageName);
            msg->setArgumentList(m_pMessageList->clone());
            msg->setID(getID()); // 考虑这个要不要复制
            return msg;
        }
    private:
        /*! @brief	消息名. */
        std::string m_strMessageName;
        /*! @brief	参数列表. */
        std::shared_ptr<any_args_list> m_pMessageList;
        /*! @brief	消息ID. */
        int m_nID;
    };
    
    /*!
     * @class	ipc_result
     *
     * @brief	执行结果.
     */
    class ipc_result {
    public:
        ipc_result() {}
        ~ipc_result() {}
        
        /*!
         * @fn	static ipc_result ipc_result::fromJson(amo::json& json)
         *
         * @brief	将JSON转换为ipc_result.
         *
         * @param [in]	json	The JSON.
         *
         * @return	An ipc_result.
         */
        static ipc_result fromJson(amo::json& json) {
            ipc_result result;
            result.m_nID = json.get<int>("id");
            amo::json args = json.get<amo::json>("args");
            char type = args.get<char>("type");
            std::string value = args.get<std::string>("value");
            result.m_val = any(type, value);;
            return result;
        }
        
        /*!
         * @fn	const int& ipc_result::getID() const
         *
         * @brief	获取消息ID，与ipc_msg中的消息ID相同.
         *
         * @return	The identifier.
         */
        const int& getID() const {
            return m_nID;
        }
        
        /*!
         * @fn	void ipc_result::setID(const int& id)
         *
         * @brief	设置消息ID.
         *
         * @param	id	The identifier.
         */
        void setID(const int& id) {
            m_nID = id;
        }
        
        /*!
         * @fn	any& ipc_result::GetResult()
         *
         * @brief	获取真正的返回值.
         *
         * @return	The result.
         */
        any& getResult() {
            return m_val;
        }
        
        /*!
         * @fn	void ipc_result::SetResult(const any& val)
         *
         * @brief	设置返回值.
         *
         * @param	val	The value.
         */
        void setResult(const any& val) {
            m_val = val;
        }
        
        /*!
         * @fn	amo::json ipc_result::toJson() const
         *
         * @brief	将ipc_result转换为JSON.
         *
         * @return	This object as an amo::json.
         */
        amo::json toJson() const {
            amo::json json;
            json.put("id", m_nID);
            
            amo::json args;
            args.put("type", m_val.type());
            args.put("value", m_val.value());
            json.put_child("args", args);
            
            return json;
        }
    private:
        /*! @brief	消息ID（与执行的ipc_msg中的消息ID相同）. */
        int m_nID;
        /*! @brief	实际返回值. */
        any m_val;
    };
    
    template<>  std::string any_to_string<any>(const any& val) {
        return val.value();
    }
    
    template<>  ipc_msg string_to_any(const std::string& val) {
        amo::json json(val);
        return ipc_msg::fromJson(json);
    }
    template<>  ipc_result string_to_any(const std::string& val) {
        amo::json json(val);
        return ipc_result::fromJson(json);
    }
    
    template<>  std::string any_to_string<ipc_msg>(const ipc_msg& val) {
        return val.toJson().to_string();
    }
    template<>  std::string any_to_string(const ipc_result& val) {
        return val.toJson().to_string();
    }
    
    template<>  std::vector<any> string_to_any(const std::string& val) {
        std::vector<any> vec;
        amo::json json(val);
        
        if (!json.is_array()) {
            return vec;
        }
        
        std::vector<amo::json> jsonArr = json.to_array();
        
        for (auto& p : jsonArr) {
            vec.push_back(any::fromJson(p));
        }
        
        return vec;
    }
    
    template<>  std::string any_to_string(const std::vector<any>& val) {
        amo::json jsonArr;
        jsonArr.set_array();
        
        for (auto p : val) {
            jsonArr.push_back(p.toJson());
        }
        
        return jsonArr.to_string();
    }
    
    
    
    /*!
    * @class	valid_any
    *
    * @brief	A valid any.
    */
    
    class valid_any {};
    
    /*!
    * @class	invalid_any
    *
    * @brief	An invalid any.
    */
    
    class invalid_any {};
    
    template<typename T>
    class get_any_type {
    public:
        typedef typename std::conditional<any_value_type<T>::value
        == any_value_type<unknown>::value, invalid_any, valid_any>::type Type;
    };
}


#endif //AMO_ANY_4752CEEE_AFC1_4408_8855_BAEDA0FD3669_HPP__
