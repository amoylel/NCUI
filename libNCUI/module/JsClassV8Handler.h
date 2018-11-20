// Created by amoylel on 04/11/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_JSCLASSV8HANDLER_H__
#define AMO_JSCLASSV8HANDLER_H__

#include "module/JSV8Handler.h"

namespace amo {

    /*!
     * @class	JsClassObjectV8Accessor
     *
     * @brief	用来解决对象中属性问题，暂时没有用.
     */
    
    class JsClassObjectV8Accessor : public CefV8Accessor {
    public:
        ~JsClassObjectV8Accessor() {
            int i = 32;
        }
        /*!
         * @fn	virtual bool JsClassObjectV8Accessor::Get(
         * 		const CefString& name,
         * 		const CefRefPtr<CefV8Value> object,
         * 		CefRefPtr<CefV8Value>& retval,
         * 		CefString& exception) override
         *
         * @brief	Gets.
         *
         * @param	name			 	The name.
         * @param	object			 	The object.
         * @param [in,out]	retval   	The retval.
         * @param [in,out]	exception	The exception.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool Get(const CefString& name,
                         const CefRefPtr<CefV8Value> object,
                         CefRefPtr<CefV8Value>& retval,
                         CefString& exception) override {
            if (name == "Name") {
                int i = 32;//for break;
            }
            
            if (m_fnGetSet) {
                CefV8ValueList list;
                return m_fnGetSet(name, object, list, retval, exception);
            } else {
                return false;
            }
        }
        
        /*!
         * @fn	bool JsClassObjectV8Accessor::Set(
         * 		const CefString& name,
         * 		const CefRefPtr<CefV8Value> object,
         * 		const CefRefPtr<CefV8Value> value,
         * 		CefString& exception)
         *
         * @brief	Sets.
         *
         * @param	name			 	The name.
         * @param	object			 	The object.
         * @param	value			 	The value.
         * @param [in,out]	exception	The exception.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        bool Set(const CefString& name,
                 const CefRefPtr<CefV8Value> object,
                 const CefRefPtr<CefV8Value> value,
                 CefString& exception) override {
            if (name == "Name") {
                int i = 32;//for break;
            }
            
            if (m_fnGetSet) {
                CefV8ValueList list;
                list.push_back(value);
                CefRefPtr<CefV8Value> retval = CefV8Value::CreateUndefined();
                return m_fnGetSet(name, object, list, retval, exception);
            } else {
                return false;
            }
            
        }
        
        /*!
         * @fn	void JsClassObjectV8Accessor::setGetSetCallback(
         * 		std::function < bool(const CefString&,
         * 		 CefRefPtr<CefV8Value>,
         * 		 const CefV8ValueList&,
         * 		 CefRefPtr<CefV8Value>&,
         * 		 CefString&) > fn)
         *
         * @brief	Callback, called when the set get set.
         *
         * @param	fn	The function.
         */
        
        void setGetSetCallback(
            std::function < bool(const CefString&,
                                 CefRefPtr<CefV8Value>,
                                 const CefV8ValueList&,
                                 CefRefPtr<CefV8Value>&,
                                 CefString&) > fn) {
            m_fnGetSet = fn;
        }
        
        
        
        IMPLEMENT_REFCOUNTING(JsClassObjectV8Accessor);
        
    protected:
        std::function < bool(const CefString&,
                             CefRefPtr<CefV8Value>,
                             const CefV8ValueList&,
                             CefRefPtr<CefV8Value>&,
                             CefString&) > m_fnGetSet;
                             
                             
    };
    
    // class 调用UI线程中的Transfer
    class JsClassV8Handler : public JsV8Handler {
    public:
    
        JsClassV8Handler();
        
        virtual ~JsClassV8Handler();
        
        /*!
         * @fn	virtual CefRefPtr<CefV8Value> JsClassV8Handler::getV8Object(
         * 		CefRefPtr<CefV8Value> object = NULL);
         *
         * @brief	获取类的JS对象.
         * 			不在使用宏创建对象，这里需要构建的是一个函数（构造函数）
         * 			可以使用new Object();
         *
         * @param	object	(Optional) the object.
         * 					如果object不存在，那么创建一个新的对象，
         * 					如果object存在，那么直接在这个对象上添加属性
         *
         * @return	The v8 object.
         */
        virtual CefRefPtr<CefV8Value> getV8Object(
            CefRefPtr<CefV8Value> object = NULL) override;
            
        /*!
         * @fn	virtual void JsClassV8Handler::onGetV8Object(
         * 		CefRefPtr<CefV8Value> object) override;
         *
         * @brief	函数创建后的处理函数，这里可以将一些静态函数之类的东西附加到类中.
         *
         * @param	object	The object.
         */
        virtual void onGetV8Object(CefRefPtr<CefV8Value> object) override;
        
        /*!
         * @fn	virtual bool JsClassV8Handler::Execute(const CefString& name,
         * 		 CefRefPtr<CefV8Value> object,
         * 		 const CefV8ValueList& arguments,
         * 		  CefRefPtr<CefV8Value>& retval,
         * 		  CefString& exception) override;
         *
         * @brief	重载Execute函数.
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
                             CefString& exception) override;
                             
        /*!
         * @fn	bool JsClassV8Handler::isRendererClass() const;
         *
         * @brief	判断当前调用的C++函数是否是在渲染进程上执行.
         *
         * @return	true if renderer class, false if not.
         */
        bool isRendererClass() const;
        
        /*!
         * @fn	void JsClassV8Handler::setRendererClass(bool val);
         *
         * @brief	设置当前类的C++函数在渲染进程上执行.
         *
         * @param	val	true to value.
         */
        void setRendererClass(bool val);
        /*!
         * @fn	void JsClassV8Handler::nativeObject(CefRefPtr<CefV8Value> object,
         * 		 const CefV8ValueList& arguments,
         * 		  CefRefPtr<CefV8Value>& retval,
         * 		   CefString& exception);
         *
         * @brief	js调用 时可以判断我是一个本地Transfer对象.
         *
         * @param	name	The name.
         *
         * @param	object			 	The object.
         * @param	arguments		 	The arguments.
         * @param 	retval   	The retval.
         * @param 	exception	The exception.
         */
        void nativeObject(CefRefPtr<CefV8Value> object,
                          const CefV8ValueList& arguments,
                          CefRefPtr<CefV8Value>& retval,
                          CefString& exception);
        /*!
         * @fn	void JsClassV8Handler::nativeToJson(CefRefPtr<CefV8Value> object,
         * 		 const CefV8ValueList& arguments,
         * 		  CefRefPtr<CefV8Value>& retval,
         * 		   CefString& exception);
         *
         * @brief	将本地对象转换为 JSON字符串.
         *
         * @param	name	The name.
         *
         * @param	object			 	The object.
         * @param	arguments		 	The arguments.
         * @param 	retval   	The retval.
         * @param 	exception	The exception.
         */
        void nativeToJson(CefRefPtr<CefV8Value> object,
                          const CefV8ValueList& arguments,
                          CefRefPtr<CefV8Value>& retval,
                          CefString& exception);
                          
        IMPLEMENT_REFCOUNTING(JsClassV8Handler);
    protected:
    
        bool m_bRendererClass;
        CefRefPtr<JsClassObjectV8Accessor> m_pAccessor;
        
    };
}

#endif // AMO_JSCLASSV8HANDLER_H__
