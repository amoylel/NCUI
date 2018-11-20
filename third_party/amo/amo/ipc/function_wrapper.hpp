// Created by amoylel on 14/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_FUNCTION_WRAPPER_1812FFA3_064A_4FCC_9C9C_7077D82E6725_HPP__
#define AMO_FUNCTION_WRAPPER_1812FFA3_064A_4FCC_9C9C_7077D82E6725_HPP__







#include <functional>
#include <memory>
#include <vector>
#include <map>
#include <string>

#include <amo/json.hpp>

#include <amo/ipc/any.hpp>



namespace amo {
    // transfer类型
    enum transfer_type {
        transfer_unknown = -1,
        transfer_class,						//类
        transfer_object						//对象
    };
    
    // 多线程支持
    enum transfer_multi_type {
        transfer_multi_unkown = -1, //未知
        transfer_multi_support = 0 << 16,  // 允许在其他线程上执行（默认）
        transfer_multi_disabled = 1 << 16, // 禁止多线程执行
    };
    // JS类中的函数类型，
    enum transfer_func_type {
        transfer_func_unknown = -1,					// 未知
        transfer_func_normal = 0 << 8,				// 普通函数
        transfer_func_static = 1 << 8,				// 静态函数
        transfer_func_constructor = 2 << 8,			// 构造函数
        transfer_func_const_property = 3 << 8,			// 类静态常量属性，不能被赋值
        transfer_func_class_property = 4 << 8,			// 类属性，可以通过.CLASS访问赋值
        transfer_func_member_property = 5 << 8,		// 对象属性，可以赋值
    };
    
    // JS在调用C++时的同步类型
    enum transfer_exec_type {
        transfer_exec_unknown = -1,	//未知
        transfer_exec_normal = 0,		//普通
        transfer_exec_sync = 1,		//同步
        transfer_exec_async = 2,		//异步
    };
    
    class function_wrapper {
    public:
        typedef std::function<any(ipc_msg::SmartType)> TransferFunc;
    public:
        static function_wrapper fromJson(amo::json& json) {
            function_wrapper wrapper;
            wrapper.m_strName = json.get<std::string>("function");
            wrapper.m_nExecType = json.get<int>("exec");
            return wrapper;
        }
        
        static std::vector<function_wrapper> fromJsonArray(
            std::vector<amo::json>& json) {
            
            std::vector<function_wrapper> vec;
            
            for (auto& p : json) {
                vec.push_back(fromJson(p));
            }
            
            return vec;
        }
    public:
        function_wrapper() {
            m_nExecType = 0;
        }
        
        function_wrapper(const std::string& strName, int nType = 0) {
            m_strName = strName;
            m_nExecType = nType;
        }
        
        virtual ~function_wrapper() {
        
        }
        amo::json toJson() {
            amo::json json;
            json.put("function", m_strName);
            json.put("exec", m_nExecType);
            return json;
        }
        
        /**
         * @fn	TransferMultiThread function_wrapper::multiType()
         *
         * @brief	多线程类型.
         *
         * @return	A TransferMultiThread.
         */
        
        transfer_multi_type multiType() {
            int type = m_nExecType & 0xff0000;
            
            if (type < 0) {
                return transfer_multi_unkown;
            }
            
            return (transfer_multi_type)type;
        }
        
        transfer_func_type functionType() {
            int type = m_nExecType & 0xff00;
            
            if (type < 0) {
                return transfer_func_unknown;
            }
            
            return (transfer_func_type)type;
        }
        
        transfer_exec_type execType() {
            int type = m_nExecType & 0xff;
            
            if (type < 0) {
                return transfer_exec_unknown;
            }
            
            return (transfer_exec_type)type;
        }
        
        function_wrapper(const std::string strName,
                         TransferFunc fn,
                         int nType = 0) {
                         
            m_strName = strName;
            m_fn = fn;
            m_nExecType = nType;
        }
        
        any operator()(ipc_msg::SmartType args) {
            if (!m_fn) {
                return undefined();
            }
            
            return m_fn(args);
        }
        
        
        TransferFunc m_fn;
        int m_nExecType;	 //0 exec 1 sync 2 async
        std::string m_strName;
    };
    
    
    class function_wrapperMgr {
    public:
        function_wrapperMgr() {
            m_nObjectID = 0;
            m_bRendererClass = false;
            m_bBuiltIn = true;
        }
        
        function_wrapperMgr(const std::string& str) {
            m_strName = str;
        }
        
        virtual ~function_wrapperMgr() {
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
        std::vector<function_wrapper> toVector() {
            std::vector<function_wrapper> vec;
            
            for (auto& p : m_mpJsFunc) {
                vec.push_back(p.second);
            }
            
            return vec;
        }
        
        std::string toString() {
            return toJson().to_string();
        }
        
        amo::json toJson() {
            std::vector<function_wrapper> vec = toVector();
            amo::json json;
            json.put("name", this->name());
            json.put("objectName", this->getObjectName());
            json.put("id", this->getObjectID());
            json.put("builtin", this->isBuiltIn());
            json.put("rendererClass", this->isRendererClass());
            json.put("transferClass", true); //表示这是一个类
            amo::json functions;
            
            functions.set_array();
            
            for (size_t i = 0; i < vec.size(); ++i) {
                functions.push_back(vec[i].toJson());
            }
            
            json.put_child("functions", functions);
            amo::json modules;
            modules.set_array();
            
            for (auto& p : m_vecModules) {
                modules.push_back(p);
            }
            
            json.put_child("modules", modules);
            
            amo::json depends;
            depends.set_array();
            
            for (auto& p : m_vecDepends) {
                depends.push_back(p);
            }
            
            json.put_child("depends", depends);
            
            
            any attributes = m_vecAttributes;
            
            json.put_child("attributes", attributes.toJson());
            
            return json;
        }
        
        amo::json toSimplifiedJson() {
            amo::json json;
            json.put("name", this->name());
            json.put("objectName", this->getObjectName());
            json.put("id", this->getObjectID());
            json.put("builtin", this->isBuiltIn());
            json.put("rendererClass", this->isRendererClass());
            json.put("transferObject", true); //表示这是一个类实例
            return json;
        }
        
        static function_wrapperMgr fromJson(amo::json& json) {
            function_wrapperMgr mgr;
            mgr.m_strName = json.get<std::string>("name");
            mgr.m_nObjectID = json.get<int64_t>("id");
            mgr.m_strObjectName = json.get<std::string>("objectName");
            mgr.m_bRendererClass = json.get<bool>("rendererClass");
            std::vector<amo::json> functions;
            functions = json.get_child("functions").to_array();
            std::vector<function_wrapper> wrappers;
            wrappers = function_wrapper::fromJsonArray(functions);
            
            for (auto& p : wrappers) {
                mgr.toMap().insert(std::make_pair(p.m_strName, p));
            }
            
            std::vector<amo::json> modules;
            modules = json.get_child("modules").to_array();
            
            for (auto& p : modules) {
                mgr.addModule(p.get<std::string>());
            }
            
            std::vector<amo::json> depends;
            depends = json.get_child("depends").to_array();
            
            for (auto& p : depends) {
                mgr.addDepends(p.get<std::string>());
            }
            
            any attributes = any::fromJson(json.get_child("attributes"));
            mgr.getAttributes() = attributes.As<std::vector<any> >();
            return mgr;
        }
        
        std::unordered_map < std::string, function_wrapper >& toMap() {
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
        
        
        void addAttribute(const std::string& strName, any val) {
            amo::json json;
            json.put("name", strName);
            json.put("value", val.toJson());
            m_vecAttributes.push_back(json);
        }
        
        std::vector<any>& getAttributes() {
            return m_vecAttributes;
        }
        
        transfer_multi_type multiType(const std::string& funcName) {
            auto iter = toMap().find(funcName);
            
            if (iter == toMap().end()) {
                return transfer_multi_unkown;
            }
            
            return iter->second.multiType();
            
        }
        
        transfer_func_type functionType(const std::string& funcName) {
            auto iter = toMap().find(funcName);
            
            if (iter == toMap().end()) {
                return transfer_func_unknown;
            }
            
            return iter->second.functionType();
        }
        
        transfer_exec_type execType(const std::string& funcName) {
            auto iter = toMap().find(funcName);
            
            if (iter == toMap().end()) {
                return transfer_exec_unknown;
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
         * @fn	bool function_wrapperMgr::isBuiltIn() const
         *
         * @brief	Query if this object is built in.
         *
         * @return	true if built in, false if not.
         */
        bool isBuiltIn() const {
            return m_bBuiltIn;
        }
        /*!
         * @fn	void function_wrapperMgr::setBuiltIn(bool val)
         *
         * @brief	Sets built in.
         *
         * @param	val	true to value.
         */
        void setBuiltIn(bool val) {
            m_bBuiltIn = val;
        }
        
    private:
        int64_t m_nObjectID;
        std::vector<std::string> m_vecModules;
        std::vector<std::string> m_vecDepends;
        std::vector<any> m_vecAttributes;
        
        
        std::unordered_map < std::string, function_wrapper >
        m_mpJsFunc;								//JS调用C++回调处理函数集合
        std::string m_strName;
        std::string m_strObjectName;
        bool m_bRendererClass;
        /*! @brief	是否为内置模块，默认为true. */
        bool m_bBuiltIn;
    };
}

#endif //AMO_FUNCTION_WRAPPER_1812FFA3_064A_4FCC_9C9C_7077D82E6725_HPP__






