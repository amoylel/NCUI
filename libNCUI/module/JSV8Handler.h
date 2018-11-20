// Created by amoylel on 03/12/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_JSV8HANDLER_H__
#define AMO_JSV8HANDLER_H__

#include <functional>
#include <unordered_map>

#include "handler/CefHeader.hpp"

#include "transfer/FunctionWrapper.hpp"


#ifndef AMO_CEF_IMPL_NO_REFCOUNTING
#define AMO_CEF_IMPL_NO_REFCOUNTING(_TYPE) \
    void    AddRef() const { } \
    bool   Release() const { return false; } \
    bool HasOneRef() const { return true; }

#endif


static CefRefPtr<CefV8Value> DefCreateV8Object(CefRefPtr<CefV8Value> _object) {
#if CHROME_VERSION_BUILD >= 2840

    if (_object == NULL) {
        _object = CefV8Value::CreateObject(NULL, NULL);
    }
    
#else
    
    if (_object == NULL) {
        _object = CefV8Value::CreateObject(NULL);
    }
    
#endif
    return _object;
}

// CefV8Value object对象生成
// 生成JS对象 相当于var a = {};
#define AMO_CEF_BEGIN_OBJECT_FACTORY(ClassName)\
	public:\
		IMPLEMENT_REFCOUNTING(ClassName);\
    public: \
    virtual CefRefPtr<CefV8Value> getV8Object(CefRefPtr<CefV8Value> object = NULL) \
	{ \
		typedef ClassName ClassType;\
		CefRefPtr<CefV8Value> _object = object;\
		_object = DefCreateV8Object(_object);

// 结束对象生成，这里调用onGetV8Object将生成的Object交给其他地方处理
#define AMO_CEF_END_OBJECT_FACTORY() \
		onGetV8Object(_object);\
        return _object; \
	}
// 向Object中添加函数
#define AMO_CEF_MAP_OBJECT_FUNCTION(Func)\
	 this->addFunction(#Func, std::bind(&ClassType::Func,\
										this,\
										std::placeholders::_1,\
										std::placeholders::_2,\
										std::placeholders::_3,\
										std::placeholders::_4));\
	_object->SetValue(#Func, \
					  CefV8Value::CreateFunction(#Func, this), \
					  V8_PROPERTY_ATTRIBUTE_NONE);

// 向Object添加String
#define AMO_CEF_MAP_OBJECT_STRING(Func)\
	_object->SetValue(#Func,\
					 CefV8Value::CreateString(Func()), \
					 V8_PROPERTY_ATTRIBUTE_NONE);

#define AMO_CEF_MAP_OBJECT_PROPERTY(Func)\
	_object->SetValue(#Func,\
					 V8_ACCESS_CONTROL_DEFAULT, \
					 V8_PROPERTY_ATTRIBUTE_NONE);

namespace amo {



    /*!
     * @class	JsV8Handler
     *
     * @brief	JS 调用C++ 基类.
     * 			将Transfer 转换为 V8 Object
     */
    class JsV8Handler : public CefV8Handler {
    public:
    
        /*!
         * @typedef	std::function < void(CefRefPtr<CefV8Value>, const CefV8ValueList&, CefRefPtr<CefV8Value>&, CefString&) > FuncType
         *
         * @brief	JS函数调用对应C++的函数类型.
         */
        
        typedef std::function < void(CefRefPtr<CefV8Value>,
                                     const CefV8ValueList&,
                                     CefRefPtr<CefV8Value>&,
                                     CefString&) > FuncType;
                                     
        /*!
         * @typedef	std::function<void(CefRefPtr<CefListValue>&, const CefV8ValueList&, int)> OnCopyParamsType
         *
         * @brief	自定义处理参数复制回调函数.
         */
        
        typedef std::function<void(CefRefPtr<CefListValue>&,
                                   const CefV8ValueList&,
                                   int)> OnCopyParamsType;
                                   
                                   
    public:
    
        JsV8Handler();
        
        virtual ~JsV8Handler();
        
        /*!
         * @fn	virtual CefRefPtr<JsV8Handler> JsV8Handler::createV8Handler()
         *
         * @brief	由派生类继承创建新的Handler.
         *
         * @return	The new v 8 handler.
         */
        virtual CefRefPtr<JsV8Handler> createV8Handler();
        
        /*!
         * @fn	void JsV8Handler::setHandlerName(const std::string& strName)
         *
         * @brief	设置Handler类名.
         *
         * @param	strName	The name.
         */
        void setHandlerName(const std::string& strName);
        
        /*!
         * @fn	const std::string& JsV8Handler::getHandlerName() const
         *
         * @brief	获取Handler类名.
         *
         * @return	The handler name.
         */
        const std::string& getHandlerName() const;
        
        ///*!
        // * @fn	virtual void JsV8Handler::setFunctions(
        // * 		const std::vector<FunctionWrapper>& vec)
        // *
        // * @brief	设置JS函数名称集合，通常为主进程中的C++函数.
        // *
        // * @param	vec	The vector.
        // */
        //virtual void setFunctions(const std::vector<FunctionWrapper>& vec);
        //
        ///*!
        // * @fn	virtual std::vector<amo::FunctionWrapper>& JsV8Handler::getFunctions()
        // *
        // * @brief	获取所有的函数.
        // *
        // * @return	The functions.
        // */
        //virtual std::vector<amo::FunctionWrapper>& getFunctions();
        
        //// 设置当前Handler所用的内置模块集合，用到哪些模块
        //
        //virtual void setModules(const std::vector<std::string>& vec);
        //
        ///*!
        // * @fn	virtual std::vector<std::string>& JsV8Handler::getModules()
        // *
        // * @brief	获取当前Handler所依赖的内置模块.
        // *
        // * @return	The modules.
        // */
        //virtual std::vector<std::string>& getModules();
        
        /*!
         * @fn	virtual CefRefPtr<CefV8Value> JsV8Handler::getV8Object(
         * 		 CefRefPtr<CefV8Value> object = NULL)
         *
         * @brief	模块JS代码 获取当前Handler的Js对象 通常用宏生成，
         * 			如果object== NULL 将创建新的对象，否则使用已有的对象.
         *
         * @param	object	(Optional) the object.
         *
         * @return	The v 8 object.
         */
        virtual CefRefPtr<CefV8Value> getV8Object(
            CefRefPtr<CefV8Value> object = NULL);
            
        /*!
         * @fn	virtual void JsV8Handler::onGetV8Object(CefRefPtr<CefV8Value> object)
         *
         * @brief	通过重载该函数来扩展object.
         *
         * @param	object	The object.
         */
        virtual void onGetV8Object(CefRefPtr<CefV8Value> object);
        
        /*!
         * @fn	virtual bool JsV8Handler::Execute(const CefString& name,
         * 		CefRefPtr<CefV8Value> object,
         * 		const CefV8ValueList& arguments,
         * 		CefRefPtr<CefV8Value>& retval,
         * 		 CefString& exception) OVERRIDE
         *
         * @brief	JS调用C++时所执行的函数.
         *
         * @param	name			 	The name.
         * @param	object			 	The object.
         * @param	arguments		 	The arguments.
         * @param 	retval   	The retval.
         * @param 	exception	The exception.
         *
         * @return	true if it succeeds, false if it fails.
         */
        virtual bool Execute(const CefString& name,
                             CefRefPtr<CefV8Value> object,
                             const CefV8ValueList& arguments,
                             CefRefPtr<CefV8Value>& retval,
                             CefString& exception) OVERRIDE;
                             
        /*!
         * @fn	void JsV8Handler::addFunction(const std::string& key, FuncType fn)
         *
         * @brief	注册JS函数.
         *
         * @param	key	The key.
         * @param	fn 	The function.
         */
        void addFunction(const std::string& key, FuncType fn);
        
        /*!
         * @fn	std::vector<std::string> JsV8Handler::getAllKeys()
         *
         * @brief	获取所有的JS函数名称.
         *
         * @return	all keys.
         */
        std::vector<std::string> getAllKeys();
        
        /*!
         * @fn	const int64_t& JsV8Handler::getID() const
         *
         * @brief	Gets the identifier.
         *
         * @return	The identifier.
         */
        const int64_t& getID() const;
        
        /*!
         * @fn	void JsV8Handler::setID(const int64_t& val)
         *
         * @brief	Sets an identifier.
         *
         * @param	val	The value.
         */
        void setID(const int64_t& val);
        
        
        FunctionWrapperMgr& getFuncMgr() ;
        
        void setFuncMgr(FunctionWrapperMgr& val);
        
    protected:
    
        /*! @brief 对应C++中的Transfer objectID. */
        int64_t m_nID;
        /*! @brief	需要执行的函数, 从UI线程传递过来的函数名. */
        std::vector<amo::FunctionWrapper> m_vecFunctions;
        /*! @brief	需要用到的模块. */
        std::vector<std::string> m_vecModules;
        /*! @brief	模块名. */
        std::string m_strHandlerName;
        
        FunctionWrapperMgr m_oFuncMgr;
        
        /*! @brief	模块中所注册的函数. */
        std::unordered_map < std::string, FuncType> m_oFuncMap;
        
    };
    
    
    
}



#endif // AMO_JSV8HANDLER_H__
