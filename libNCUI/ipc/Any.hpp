// Created by amoylel on 05/18/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_ANY_HPP__
#define AMO_ANY_HPP__

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <type_traits>
#include <amo/json.hpp>
#include <amo/nil.hpp>
#include <amo/stdint.hpp>
#include <amo/date_time.hpp>
#include "ipc/IPCMessage.hpp"
#include <amo/blob.hpp>
#include <amo/bigstr.hpp>

/*!
 * @namespace	amo
 *
 * @brief	.
 */
namespace amo {




    /*!
     * @typedef	amo::nil Nil
     *
     * @brief	函数无返回值时，线程/进程之间同步时 返回Nil表示void.
     * 			对应JS中的null
     */
    typedef amo::nil Nil;
    
    /*!
     * @class	Undefined
     *
     * @brief	调用C++时返回Undefined对应JS中的undefined.
     */
    class Undefined {};
    
    /*!
     * @class	NonReturn
     *
     * @brief	JS调用C++不需要向JS返回结果，如异步回调函数.
     */
    class NonReturn {};
    
    /*!
     * @class	Nothing
     *
     * @brief	JS调用C++不需要向JS返回结果，跟NonReturn差不多，分不清了.
     */
    class Nothing {};
    
    /**
     * @class	Deadlock
     *
     * @brief	进程死锁时的返回类型.
     */
    
    class Deadlock {};
    /*!
     * @class	Unknown
     *
     * @brief	未知结果，正常不会出现.
     */
    class Unknown {};
    
    // 前置声明
    class Any;
    class IPCMessage;
    class IPCResult;
    
    /*!
     * @fn	template<typename T> static std::string valueToString(const T& val)
     *
     * @brief	将类型转换成字符串.
     *
     * @tparam	T	类型.
     * @param	val	类型实例.
     *
     * @return	std::string.
     */
    template<typename T> static std::string valueToString(const T& val) {
        amo::u8json json;
        json.put(val);
        return json.to_string();
    }
    /*!
     * @fn	template<typename T> static std::string AnyToString(const T& val);
     *
     * @brief	将任意类型转换为String.
     * 			将函数不能被调用，只能使用特化版本
     *
     * @tparam	T	类型.
     * @param	val	类型实例.
     *
     * @return	std::string.
     */
    template<typename T> static std::string anyToString(const T& val);
    
    template<> static std::string anyToString(const bool& val) {
        return valueToString<bool>(val);
    }
    template<> static std::string anyToString(const char& val) {
        return valueToString<char>(val);
    }
    template<> static std::string anyToString(const int& val) {
        return valueToString<int>(val);
    }
    template<> static std::string anyToString(const int64_t& val) {
        return valueToString<int64_t>(val);
    }
    template<> static std::string anyToString(const uint64_t& val) {
        return valueToString<uint64_t>(val);
    }
    template<> static std::string anyToString(const double& val) {
        return valueToString<double>(val);
    }
    template<> static std::string anyToString(const std::string& val) {
        return val;
    }
    template<> static std::string anyToString<Nil>(const Nil& val) {
        return valueToString<std::string>("nil");
    }
    template<> static std::string anyToString<Undefined>(const Undefined& val) {
        return valueToString<std::string>("undefined");
    }
    template<> static std::string anyToString<NonReturn>(const NonReturn& val) {
        return valueToString<std::string>("nonreturn");
    }
    template<> static std::string anyToString<Deadlock>(const Deadlock& val) {
        return valueToString<std::string>("deadlock");
    }
    template<> static std::string anyToString<amo::blob>(const amo::blob& val) {
        return val.get_name();
    }
    template<> static std::string anyToString<amo::bigstr>(const amo::bigstr& val) {
        return val.get_name();
    }
    template<> static std::string anyToString<amo::u8json>(const amo::u8json& val) {
        return val.to_string();
    }
    template<> static std::string anyToString<Nothing>(const Nothing& val) {
        return std::string();
    }
    
    template<> static std::string anyToString<date_time>(const date_time& val) {
        return val.to_string();
    }
    /*!
     * @fn	template<typename T> static T stringToValue(const std::string& val)
     *
     * @brief	将字符串转换为所给类型.
     *
     * @tparam	T	类型.
     * @param	val	输入字符串.
     *
     * @return	目标类型值.
     */
    template<typename T> static T stringToValue(const std::string& val) {
        amo::u8json json(val);
        return json.get<T>();
    }
    /*!
     * @fn	template<typename T> static T stringToAny(const std::string& val);
     *
     * @brief	将字符串转换为任意类型.
     *
     * @tparam	T	目标类型.
     * @param	val	输入字符串.
     *
     * @return	目标类型值.
     */
    template<typename T> static T stringToAny(const std::string& val);
    template<> static bool stringToAny(const std::string& val) {
        return stringToValue<bool>(val);
    }
    template<> static char stringToAny(const std::string& val) {
        return stringToValue<char>(val);
    }
    template<> static int stringToAny(const std::string& val) {
        return stringToValue<int>(val);
    }
    template<> static int64_t stringToAny(const std::string& val) {
        return stringToValue<int64_t>(val);
    }
    template<> static uint64_t stringToAny(const std::string& val) {
        return stringToValue<uint64_t>(val);
    }
    template<> static double stringToAny(const std::string& val) {
        return stringToValue<double>(val);
    }
    template<> static std::string stringToAny(const std::string& val) {
        return val;
    }
    template<> static amo::u8json stringToAny(const std::string& val) {
        return amo::u8json(val);
    }
    
    template<> static Nil stringToAny(const std::string& val) {
        return Nil();
    }
    template<> static Undefined stringToAny(const std::string& val) {
        return Undefined();
    }
    template<> static Deadlock stringToAny(const std::string& val) {
        return Deadlock();
    }
    template<> static amo::blob stringToAny(const std::string& val) {
        return amo::blob(val);
    }
    template<> static amo::bigstr stringToAny(const std::string& val) {
        return amo::bigstr(val);
    }
    template<> static NonReturn stringToAny(const std::string& val) {
        return NonReturn();
    }
    template<> static Nothing stringToAny(const std::string& val) {
        return Nothing();
    }
    template<> static date_time stringToAny(const std::string& val) {
        return date_time::from_string(val);
    }
    /*!
     * @struct	AnyValueType
     *
     * @brief	Any类型判断类.
     *
     * @tparam	T	Generic type parameter.
     */
    template<typename T> struct AnyValueType {
        static const char value = -1;
    };
    
    template<> struct AnyValueType <Unknown> {
        static const char value = -1;
    };
    
    ///
    template<> struct AnyValueType <Nothing> {
        static const char value = 0;
    };
    template<> struct AnyValueType <bool> {
        static const char value = 1;
    };
    template<> struct AnyValueType <char> {
        static const char value = 2;
    };
    template<> struct AnyValueType <int> {
        static const char value = 3;
    };
    template<> struct AnyValueType <int64_t> {
        static const char value = 4;
    };
    template<> struct AnyValueType <double> {
        static const char value = 5;
    };
    template<> struct AnyValueType <std::string> {
        static const char value = 6;
    };
    template<> struct AnyValueType <const char*> {
        static const char value = 7;
    };
    
    template<> struct AnyValueType <uint64_t> {
        static const char value = 8;
    };
    
    // 辅助类
    template<> struct AnyValueType <Nil> {
        static const char value = 21;
    };
    template<> struct AnyValueType <Undefined> {
        static const char value = 22;
    };
    template<> struct AnyValueType <NonReturn> {
        static const char value = 23;
    };
    template<> struct AnyValueType <Deadlock> {
        static const char value = 24;
    };
    template<> struct AnyValueType < Any > {
        static const char value = 30;
    };
    template<> struct AnyValueType < IPCMessage > {
        static const char value = 31;
    };
    template<> struct AnyValueType < IPCResult > {
        static const char value = 32;
    };
    
    // 自定义类型
    template<> struct AnyValueType <std::map<std::string, std::string> > {
        static const char value = 40;
    };
    template<> struct AnyValueType <amo::u8json> {
        static const char value = 41;
    };
    /*  template<> struct AnyValueType <amo::u8json> {
          static const char value = 44;
      };*/
    
    template<> struct AnyValueType<std::vector<Any> > {
        static const char value = 42;
    };
    template<> struct AnyValueType <amo::date_time> {
        static const char value = 43;
    };
    
    template<> struct AnyValueType <amo::blob> {
        static const char value = 44;
    };
    template<> struct AnyValueType <amo::bigstr> {
        static const char value = 45;
    };
    /*!
     * @class	AnyData
     *
     * @brief	any 数据.
     */
    class AnyData {
    public:
        /*! @brief	实际类型. */
        char type;
        /*! @brief	序列化后的字符串. */
        std::string value;
    };
    /*!
     * @class	Any
     *
     * @brief	Any类，用于进程间数据通信统一类型.
     */
    class Any  {
    public:
        Any() {
            m_pData.reset(new AnyData());
            m_pData->type = 0;
        }
        
        Any(char type_)  {
            m_pData.reset(new AnyData());
            m_pData->type = type_;
        }
        
        Any(const Any& rhs) {
            m_pData.reset(new AnyData());
            m_pData->value = rhs.value();
            m_pData->type = rhs.type();
        }
        
        Any(char type_, const std::string& val_)  {
            m_pData.reset(new AnyData());
            m_pData->type = type_;
            m_pData->value = val_;
        }
        
        /*!
         * @fn	template<typename T> Any::Any(const T& val_)
         *
         * @brief	使用任意类型初始化.
         *
         * @tparam	T	Generic type parameter.
         * @param	val_	The value.
         */
        template<typename T> Any(const T& val_) {
            m_pData.reset(new AnyData());
            m_pData->type = AnyValueType<T>::value;
            m_pData->value = anyToString<T>(val_);
        }
        
        /*!
         * @fn	Any::Any(const char* val_)
         *
         * @brief	使用字符串初始化.
         *
         * @param	val_	The value.
         */
        Any(const char* val_) {
            m_pData.reset(new AnyData());
            m_pData->type = AnyValueType<std::string>::value;
            m_pData->value = anyToString<std::string>(val_);
        }
        
        ~Any() { }
        
        Any& operator=(const Any& rhs) {
            m_pData->type = rhs.type();
            m_pData->value = rhs.value();
            return *this;
        }
        
        bool isValid() const {
            return type() != AnyValueType<Nothing>::value;
        }
        
        // 类型转换
        template<typename R> operator R() const {
            return stringToAny<R>(m_pData->value);
        }
        template<typename R> R As() const {
            return stringToAny<R>(m_pData->value);
        }
        
        // 类型判断
        template<typename T> bool is() const {
            return (type() == AnyValueType<T>::value);
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
        
        static Any fromJson(amo::u8json& json) {
            char type = json.get<char>("type");
            std::string val = json.get<std::string>("value");
            return Any(type, val);
        }
        
        amo::u8json toJson() const {
            amo::u8json json;
            json.put("type", m_pData->type);
            json.put("value", m_pData->value);
            return json;
        }
        
        Any clone() {
            return Any(type(), value());
        }
        
        /*! @brief	实际类型数据. */
        std::shared_ptr<AnyData> m_pData;
    };
    
    /*!
     * @class	AnyArgsList
     *
     * @brief	进程消息中的消息参数列表.
     */
    class AnyArgsList {
    public:
        AnyArgsList() {
            m_Nil = Nil();
        }
        /*!
         * @fn	void AnyArgsList::setValue(const int& index, const Any& val)
         *
         * @brief	所给位置设置值.
         *
         * @param	index	位置.
         * @param	val  	值.
         */
        void setValue(const int& index, const Any& val) {
            m_oMap[index] = val;
        }
        /*!
         * @fn	void AnyArgsList::ClearValue(const int& index)
         *
         * @brief	删除所给位置的参数值.
         *
         * @param	index	Zero-based index of the.
         */
        void clearValue(const int& index) {
            m_oMap.erase(index);
        }
        /*!
         * @fn	void AnyArgsList::ClearValue(const int& nBegin, const int& nEnd)
         *
         * @brief	删除所给范围值.
         *
         * @param	nBegin	The begin.
         * @param	nEnd  	The end.
         */
        void clearValue(const int& nBegin, const int& nEnd) {
            for (int i = nBegin; i < nEnd; ++i) {
                if (i >= IPCArgsPosInfo::FuncName) {
                    return;
                }
                
                m_oMap.erase(i);
            }
            
            // TODO: 没有处理ARGSLENGTH
        }
        /*!
         * @fn	size_t AnyArgsList::GetSize() const
         *
         * @brief	获取当前列表的所有参数个数.
         *
         * @return	The size.
         */
        size_t getSize() const {
            return m_oMap.size();
        }
        
        std::map<int, Any>& getAllValues() {
            return m_oMap;
        }
        /*!
         * @fn	int AnyArgsList::getArgsSize() const
         *
         * @brief	获取当前列表中函数参数个数.
         *
         * @return	The arguments size.
         */
        int getArgsSize() const {
            int nSize = 0;
            
            for (auto& p : m_oMap) {
                if (p.first < IPCArgsPosInfo::FuncName) {
                    ++nSize;
                }
            }
            
            return nSize;
        }
        
        /*!
         * @fn	Any& AnyArgsList::GetValue(const int& index)
         *
         * @brief	获取所给位置值.
         *
         * @param	index	索引.
         *
         * @return	The value.
         */
        Any& getValue(const int& index) {
            auto iter = m_oMap.find(index);
            
            if (iter == m_oMap.end()) {
                return m_Nil;
            }
            
            return iter->second;
        }
        
        /*!
         * @fn	amo::u8json AnyArgsList::toJson() const
         *
         * @brief	返回当前对象的JSON对象，将参数列表转换为JSON数组.
         *
         * @return	This object as an amo::u8json.
         */
        amo::u8json toJson() const {
            amo::u8json json;
            json.set_array();
            
            for (auto& p : m_oMap) {
                amo::u8json args;
                args.put("index", p.first);
                args.put("type", p.second.type());
                args.put("value", p.second.value());
                json.push_back(args);
            }
            
            return json;
        }
        
        /*!
         * @fn	static std::shared_ptr<AnyArgsList> AnyArgsList::fromJson(amo::u8json& json)
         *
         * @brief	将JSON转换为AnyArgsList.
         *
         * @param [in]	json	The JSON.
         *
         * @return	std::shared_ptr<AnyArgsList>.
         */
        static std::shared_ptr<AnyArgsList> fromJson(amo::u8json& json) {
            std::shared_ptr<AnyArgsList> list(new AnyArgsList());
            
            if (!json.is_array()) {
                return list;
            }
            
            std::vector<amo::u8json> vec = json.to_array();
            
            for (auto& p : vec) {
                Any val;
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
        uint64_t getUint64(const int& nIndex) {
            return getValue(nIndex);
        }
        double getDouble(const int& nIndex) {
            return getValue(nIndex);
        }
        std::string getString(const int& nIndex) {
            Any& val =  getValue(nIndex);
            
            if (val.type() == m_Nil.type()) {
                return "";
            }
            
            return val;
        }
        /*!
         * @fn	bool AnyArgsList::IsValid(const int& nIndex)
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
        amo::u8json getJson(const int& nIndex) {
            return getValue(nIndex);
        }
        
        std::shared_ptr<AnyArgsList> clone() {
            std::shared_ptr<AnyArgsList> args(new AnyArgsList());
            
            for (auto& p : m_oMap) {
                args->setValue(p.first, p.second.clone());
            }
            
            return args;
            
        }
        
    private:
        std::map<int, Any> m_oMap;
        Any m_Nil;
    };
    
    /*!
     * @class	IPCMessage
     *
     * @brief	进程消息封装.
     */
    class IPCMessage {
    public:
        typedef std::shared_ptr<IPCMessage> SmartType;
    public:
        /*!
         * @fn	static int IPCMessage::GetProcessMessageID()
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
        IPCMessage() {
            m_nID = getProcessMessageID();
            
            m_pMessageList.reset(new AnyArgsList());
        }
        
        
        ~IPCMessage() {
        }
        
        /*!
         * @fn	int IPCMessage::getID() const
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
         * @fn	static IPCMessage IPCMessage::fromJson(amo::u8json& json)
         *
         * @brief	将JSON转换为IPCMessage.
         *
         * @param [in]	json	The JSON.
         *
         * @return	An IPCMessage.
         */
        static IPCMessage fromJson(amo::u8json& json) {
            IPCMessage message;
            message.m_strMessageName = json.get<std::string>("name");
            message.m_nID = json.get<int>("id");
            amo::u8json args = json.get<amo::u8json>("args");
            message.m_pMessageList = AnyArgsList::fromJson(args);
            return message;
        }
        
        static IPCMessage::SmartType Empty() {
            return std::shared_ptr<IPCMessage>(new IPCMessage());
        }
        /*!
         * @fn	const std::string& IPCMessage::getMessageName() const
         *
         * @brief	获取消息名称.
         *
         * @return	消息名称.
         */
        const std::string& getMessageName() const {
            return m_strMessageName;
        }
        
        /*!
         * @fn	const std::string& IPCMessage::GetName() const
         *
         * @brief	获取消息名称.
         *
         * @return	消息名称.
         */
        const std::string& getName() const {
            return m_strMessageName;
        }
        
        /*!
         * @fn	void IPCMessage::setMessageName(const std::string& name)
         *
         * @brief	设置消息名称.
         *
         * @param	name	消息名称.
         */
        void setMessageName(const std::string& name) {
            m_strMessageName = name;
        }
        
        /*!
         * @fn	std::shared_ptr<AnyArgsList>& IPCMessage::GetArgumentList()
         *
         * @brief	获取参数列表.
         *
         * @return	std::shared_ptr<AnyArgsList>.
         */
        std::shared_ptr<AnyArgsList>& getArgumentList() {
            return m_pMessageList;
        }
        
        /*!
         * @fn	void IPCMessage::SetArgumentList(std::shared_ptr<AnyArgsList> list)
         *
         * @brief	设置消息列表.
         *
         * @param	list	The list.
         */
        void setArgumentList(std::shared_ptr<AnyArgsList> list) {
            m_pMessageList = list;
        }
        
        amo::u8json toJson() const {
            amo::u8json json;
            json.put("name", m_strMessageName);
            json.put("id", m_nID);
            json.put_child("args", m_pMessageList->toJson());
            return json;
        }
        
        SmartType clone() {
            SmartType msg(new IPCMessage());
            msg->setMessageName(m_strMessageName);
            msg->setArgumentList(m_pMessageList->clone());
            msg->setID(getID()); // 考虑这个要不要复制
            return msg;
        }
    private:
        /*! @brief	消息名. */
        std::string m_strMessageName;
        /*! @brief	参数列表. */
        std::shared_ptr<AnyArgsList> m_pMessageList;
        /*! @brief	消息ID. */
        int m_nID;
    };
    
    /*!
     * @class	IPCResult
     *
     * @brief	执行结果.
     */
    class IPCResult {
    public:
        IPCResult() {}
        ~IPCResult() {}
        
        /*!
         * @fn	static IPCResult IPCResult::fromJson(amo::u8json& json)
         *
         * @brief	将JSON转换为IPCResult.
         *
         * @param [in]	json	The JSON.
         *
         * @return	An IPCResult.
         */
        static IPCResult fromJson(amo::u8json& json) {
            IPCResult result;
            result.m_nID = json.get<int>("id");
            amo::u8json args = json.get<amo::u8json>("args");
            char type = args.get<char>("type");
            std::string value = args.get<std::string>("value");
            result.m_val = Any(type, value);;
            return result;
        }
        
        /*!
         * @fn	const int& IPCResult::getID() const
         *
         * @brief	获取消息ID，与IPCMessage中的消息ID相同.
         *
         * @return	The identifier.
         */
        const int& getID() const {
            return m_nID;
        }
        
        /*!
         * @fn	void IPCResult::setID(const int& id)
         *
         * @brief	设置消息ID.
         *
         * @param	id	The identifier.
         */
        void setID(const int& id) {
            m_nID = id;
        }
        
        /*!
         * @fn	Any& IPCResult::GetResult()
         *
         * @brief	获取真正的返回值.
         *
         * @return	The result.
         */
        Any& getResult() {
            return m_val;
        }
        
        /*!
         * @fn	void IPCResult::SetResult(const Any& val)
         *
         * @brief	设置返回值.
         *
         * @param	val	The value.
         */
        void setResult(const Any& val) {
            m_val = val;
        }
        
        /*!
         * @fn	amo::u8json IPCResult::toJson() const
         *
         * @brief	将IPCResult转换为JSON.
         *
         * @return	This object as an amo::u8json.
         */
        amo::u8json toJson() const {
            amo::u8json json;
            json.put("id", m_nID);
            
            amo::u8json args;
            args.put("type", m_val.type());
            args.put("value", m_val.value());
            json.put_child("args", args);
            
            return json;
        }
    private:
        /*! @brief	消息ID（与执行的IPCMessage中的消息ID相同）. */
        int m_nID;
        /*! @brief	实际返回值. */
        Any m_val;
    };
    
    template<> static std::string anyToString<Any>(const Any& val) {
        return val.value();
    }
    
    template<> static IPCMessage stringToAny(const std::string& val) {
        return IPCMessage::fromJson(amo::u8json(val));
    }
    template<> static IPCResult stringToAny(const std::string& val) {
        return IPCResult::fromJson(amo::u8json(val));
    }
    
    template<> static std::string anyToString<IPCMessage>(const IPCMessage& val) {
        return val.toJson().to_string();
    }
    template<> static std::string anyToString(const IPCResult& val) {
        return val.toJson().to_string();
    }
    
    template<> static std::vector<Any> stringToAny(const std::string& val) {
        std::vector<Any> vec;
        amo::u8json json(val);
        
        if (!json.is_array()) {
            return vec;
        }
        
        std::vector<amo::u8json> jsonArr = json.to_array();
        
        for (auto& p : jsonArr) {
            vec.push_back(Any::fromJson(p));
        }
        
        return vec;
    }
    
    template<> static std::string anyToString(const std::vector<Any>& val) {
        amo::u8json jsonArr;
        jsonArr.set_array();
        
        for (auto p : val) {
            jsonArr.push_back(p.toJson());
        }
        
        return jsonArr.to_string();
    }
    
    
    
    /*!
    * @class	ValidAny
    *
    * @brief	A valid any.
    */
    
    class ValidAny {};
    
    /*!
    * @class	InvalidAny
    *
    * @brief	An invalid any.
    */
    
    class InvalidAny {};
    
    template<typename T>
    class GetAnyType {
    public:
        typedef typename std::conditional<AnyValueType<T>::value
        == AnyValueType<Unknown>::value, InvalidAny, ValidAny>::type Type;
    };
}

#endif // AMO_ANY_HPP__

