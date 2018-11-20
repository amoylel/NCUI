// Created by amoylel on 05/18/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_FUNCTIONWRAPPER_HPP__
#define AMO_FUNCTIONWRAPPER_HPP__

#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <string>

#include <amo/json.hpp>

#include "ipc/Any.hpp"

#ifndef AMO_CEF_IMPL_NO_REFCOUNTING
#define AMO_CEF_IMPL_NO_REFCOUNTING(_TYPE) \
    void    AddRef() const { } \
    bool   Release() const { return false; } \
    bool HasOneRef() const { return true; }

#endif


namespace amo {
    // Transfer类型
    enum TransferType {
        TransferUnknown = -1,
        TransferClass,						//类
        TransferObject						//对象
    };
    
    // 多线程支持
    enum TransferMultiType {
        TransferMultiUnkown = -1, //未知
        TransferMultiSupport = 0 << 16,  // 允许在其他线程上执行（默认）
        TransferMultiDisabled = 1 << 16, // 禁止多线程执行
    };
    // JS类中的函数类型，
    enum TransferFuncType {
        TransferFuncUnknown = -1,					// 未知
        TransferFuncNormal = 0 << 8,				// 普通函数
        TransferFuncStatic = 1 << 8,				// 静态函数
        TransferFuncConstructor = 2 << 8,			// 构造函数
        TransferFuncConstProperty = 3 << 8,			// 类静态常量属性，不能被赋值
        TransferFuncClassProperty = 4 << 8,			// 类属性，可以通过.CLASS访问赋值
        TransferFuncMemberProperty = 5 << 8,		// 对象属性，可以赋值
    };
    
    // JS在调用C++时的同步类型
    enum TransferExecType {
        TransferExecUnknown = -1,	//未知
        TransferExecNormal = 0,		//普通
        TransferExecSync = 1,		//同步
        TransferExecAsync = 2,		//异步
    };
    
    class FunctionWrapper {
    public:
        typedef std::function<Any(IPCMessage::SmartType)> TransferFunc;
    public:
        static FunctionWrapper fromJson(amo::u8json& json) {
            FunctionWrapper wrapper;
            wrapper.m_strName = json.get<std::string>("function");
            wrapper.m_nExecType = json.get<int>("exec");
            return wrapper;
        }
        
        static std::vector<FunctionWrapper> fromJsonArray(
            std::vector<amo::u8json>& json) {
            
            std::vector<FunctionWrapper> vec;
            
            for (auto& p : json) {
                vec.push_back(fromJson(p));
            }
            
            return vec;
        }
    public:
        FunctionWrapper() {
            m_nExecType = 0;
        }
        
        FunctionWrapper(const std::string& strName, int nType = 0) {
            m_strName = strName;
            m_nExecType = nType;
        }
        
        virtual ~FunctionWrapper() {
        
        }
        amo::u8json toJson() {
            amo::u8json json;
            json.put("function", m_strName);
            json.put("exec", m_nExecType);
            return json;
        }
        
        /**
         * @fn	TransferMultiThread FunctionWrapper::multiType()
         *
         * @brief	多线程类型.
         *
         * @return	A TransferMultiThread.
         */
        
        TransferMultiType multiType() {
            int type = m_nExecType & 0xff0000;
            
            if (type < 0) {
                return TransferMultiUnkown;
            }
            
            return (TransferMultiType)type;
        }
        
        TransferFuncType functionType() {
            int type = m_nExecType & 0xff00;
            
            if (type < 0) {
                return TransferFuncUnknown;
            }
            
            return (TransferFuncType)type;
        }
        
        TransferExecType execType() {
            int type = m_nExecType & 0xff;
            
            if (type < 0) {
                return TransferExecUnknown;
            }
            
            return (TransferExecType)type;
        }
        
        FunctionWrapper(const std::string strName,
                        TransferFunc fn,
                        int nType = 0) {
                        
            m_strName = strName;
            m_fn = fn;
            m_nExecType = nType;
        }
        
        Any operator()(IPCMessage::SmartType args) {
            if (!m_fn) {
                return Undefined();
            }
            
            return m_fn(args);
        }
        
        
        TransferFunc m_fn;
        int m_nExecType;	 //0 exec 1 sync 2 async
        std::string m_strName;
    };
    
    
    class FunctionWrapperMgr {
    public:
        FunctionWrapperMgr() {
            m_nObjectID = 0;
            m_bRendererClass = false;
            m_bBuiltIn = true;
            setNonGlobalModule(false);
        }
        
        FunctionWrapperMgr(const std::string& str) {
            m_strName = str;
        }
        
        virtual ~FunctionWrapperMgr() {
            m_mpJsFunc.clear();
            m_vecModules.clear();
            m_vecAttributes.clear();
            m_vecDepends.clear();
            
        }
        const std::string& name() const {
            return m_strName;
        }
        
        void name(const std::string& str) {
            m_strName = str;
        }
        
        std::string getObjectName() const {
            return m_strObjectName;
        }
        void setObjectName(std::string val) {
            m_strObjectName = val;
        }
        std::vector<FunctionWrapper> toVector() {
            std::vector<FunctionWrapper> vec;
            
            for (auto& p : m_mpJsFunc) {
                vec.push_back(p.second);
            }
            
            return vec;
        }
        
        std::string toString() {
            return toJson().to_string();
        }
        
        amo::u8json toJson() {
            std::vector<FunctionWrapper> vec = toVector();
            amo::u8json json;
            json.put("name", this->name());
            json.put("objectName", this->getObjectName());
            json.put("id", this->getObjectID());
            json.put("builtin", this->isBuiltIn());
            json.put("rendererClass", this->isRendererClass());
            json.put("transferClass", true); //表示这是一个类
            amo::u8json functions;
            
            functions.set_array();
            
            for (size_t i = 0; i < vec.size(); ++i) {
                functions.push_back(vec[i].toJson());
            }
            
            json.put_child("functions", functions);
            amo::u8json modules;
            modules.set_array();
            
            for (auto& p : m_vecModules) {
                modules.push_back(p);
            }
            
            json.put_child("modules", modules);
            
            amo::u8json depends;
            depends.set_array();
            
            for (auto& p : m_vecDepends) {
                depends.push_back(p);
            }
            
            json.put_child("depends", depends);
            
            
            Any attributes = m_vecAttributes;
            
            json.put_child("attributes", attributes.toJson());
            
            return json;
        }
        
        amo::u8json toSimplifiedJson() {
            amo::u8json json;
            json.put("name", this->name());
            json.put("objectName", this->getObjectName());
            json.put("id", this->getObjectID());
            json.put("builtin", this->isBuiltIn());
            json.put("rendererClass", this->isRendererClass());
            json.put("transferObject", true); //表示这是一个类实例
            return json;
        }
        
        static FunctionWrapperMgr fromJson(amo::u8json& json) {
            FunctionWrapperMgr mgr;
            mgr.m_strName = json.get<std::string>("name");
            mgr.m_nObjectID = json.get<int64_t>("id");
            mgr.m_strObjectName = json.get<std::string>("objectName");
            mgr.m_bRendererClass = json.get<bool>("rendererClass");
            std::vector<amo::u8json> functions;
            functions = json.get_child("functions").to_array();
            std::vector<FunctionWrapper> wrappers;
            wrappers = FunctionWrapper::fromJsonArray(functions);
            
            for (auto& p : wrappers) {
                mgr.toMap().insert(std::make_pair(p.m_strName, p));
            }
            
            std::vector<amo::u8json> modules;
            modules = json.get_child("modules").to_array();
            
            for (auto& p : modules) {
                mgr.addModule(p.get<std::string>());
            }
            
            std::vector<amo::u8json> depends;
            depends = json.get_child("depends").to_array();
            
            for (auto& p : depends) {
                mgr.addDepends(p.get<std::string>());
            }
            
            Any attributes = Any::fromJson(json.get_child("attributes"));
            mgr.getAttributes() = attributes.As<std::vector<Any> >();
            return mgr;
        }
        
        std::unordered_map < std::string, FunctionWrapper >& toMap() {
            return m_mpJsFunc;
        }
        
        // 添加需要调用的JS模块
        void addModule(const std::string& strName) {
            m_vecModules.push_back(strName);
        }
        
        std::vector<std::string>& getModules() {
            return m_vecModules;
        }
        
        void addDepends(const std::string& strName) {
            m_vecDepends.push_back(strName);
        }
        
        std::vector<std::string>& getDepends()  {
            return m_vecDepends;
        }
        
        
        void addAttribute(const std::string& strName, Any val) {
            amo::u8json json;
            json.put("name", strName);
            json.put("value", val.toJson());
            m_vecAttributes.push_back(json);
        }
        
        std::vector<Any>& getAttributes() {
            return m_vecAttributes;
        }
        
        TransferMultiType multiType(const std::string& funcName) {
            auto iter = toMap().find(funcName);
            
            if (iter == toMap().end()) {
                return TransferMultiUnkown;
            }
            
            return iter->second.multiType();
            
        }
        
        TransferFuncType functionType(const std::string& funcName) {
            auto iter = toMap().find(funcName);
            
            if (iter == toMap().end()) {
                return TransferFuncUnknown;
            }
            
            return iter->second.functionType();
        }
        
        TransferExecType execType(const std::string& funcName) {
            auto iter = toMap().find(funcName);
            
            if (iter == toMap().end()) {
                return TransferExecUnknown;
            }
            
            return iter->second.execType();
        }
        
        int64_t getObjectID() const {
            return m_nObjectID;
        }
        void setObjectID(int64_t val) {
            m_nObjectID = val;
        }
        
        
        /*!
        * @fn	bool JsClassV8Handler::isRendererClass() const;
        *
        * @brief	判断当前调用的C++函数是否是在渲染进程上执行.
        *
        * @return	true if renderer class, false if not.
        */
        bool isRendererClass() const {
            return m_bRendererClass;
        }
        
        /*!
        * @fn	void JsClassV8Handler::setRendererClass(bool val);
        *
        * @brief	设置当前类的C++函数在渲染进程上执行.
        *
        * @param	val	true to value.
        */
        void setRendererClass(bool val) {
            m_bRendererClass = val;
        }
        /*!
         * @fn	bool FunctionWrapperMgr::isBuiltIn() const
         *
         * @brief	Query if this object is built in.
         *
         * @return	true if built in, false if not.
         */
        bool isBuiltIn() const {
            return m_bBuiltIn;
        }
        /*!
         * @fn	void FunctionWrapperMgr::setBuiltIn(bool val)
         *
         * @brief	Sets built in.
         *
         * @param	val	true to value.
         */
        void setBuiltIn(bool val) {
            m_bBuiltIn = val;
        }
        
        /*!
         * @fn	bool FunctionWrapperMgr::isNonGlobalModule() const
         *
         * @brief	Query if this object is non global module.
         *
         * @return	true if non global module, false if not.
         */
        
        bool isNonGlobalModule() const {
            return nonGlobalModule;
        }
        
        /*!
         * @fn	void FunctionWrapperMgr::setNonGlobalModule(bool val)
         *
         * @brief	Sets non global module.
         *
         * @param	val	true to value.
         */
        
        void setNonGlobalModule(bool val) {
            nonGlobalModule = val;
        }
    private:
        int64_t m_nObjectID;
        std::vector<std::string> m_vecModules;
        std::vector<std::string> m_vecDepends;
        std::vector<Any> m_vecAttributes;
        
        
        std::unordered_map < std::string, FunctionWrapper >
        m_mpJsFunc;								//JS调用C++回调处理函数集合
        std::string m_strName;
        std::string m_strObjectName;
        bool m_bRendererClass;
        /*! @brief	是否为内置模块，默认为true. */
        bool m_bBuiltIn;
        bool nonGlobalModule;  // 不导出为全局变量
    };
}

#endif // AMO_FUNCTIONWRAPPER_HPP__





