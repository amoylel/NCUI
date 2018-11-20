// Created by amoylel on 14/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_TRANSFER_590C7217_1E3A_4E4F_A11C_BBBE47CD5E75_HPP__
#define AMO_TRANSFER_590C7217_1E3A_4E4F_A11C_BBBE47CD5E75_HPP__






#include <string>
#include <map>

#include <amo/json.hpp>
#include <amo/stdint.hpp>
#include <amo/logger.hpp>

#include <amo/ipc/any.hpp>
#include <amo/ipc/ipc_args_pos_info.hpp>
#include <amo/ipc/function_wrapper.hpp>
#include <amo/ipc/transfer_event_info.hpp>
#include <functional>




#ifndef IPC_CLASS_REGISTER_BEGIN
#define IPC_CLASS_REGISTER_BEGIN()\
using namespace amo;\
class_transfer::getTransferMap() = info->transferMap;\
int nBrowserID = info->nBrowserID;\
auto fn = info->fnCallback;\
amo::log::register_logger(info->pLogger); \

#endif

#ifndef IPC_CLASS_REGISTER
#define IPC_CLASS_REGISTER(ClassName) \
{\
	auto pTransfer = class_transfer::getUniqueTransfer<ClassName>(); \
	if (fn) {\
		fn(nBrowserID, pTransfer);\
	}\
	\
}\

#endif

#ifndef IPC_CLASS_REGISTER_END
#define IPC_CLASS_REGISTER_END()\
return true;
#endif



// JS函数注册宏 开始
#define IPC_MESSAGE_TRANSFER_BEGIN(ClassName, BaseClassName)\
	virtual void registerFunction(){\
		typedef ClassName ClassType;\
		typedef BaseClassName BaseClassType;\

// JS函数注册 同名函数
#define IPC_MESSAGE_TRANSFER_FUNC(Func, nExecType)\
		registerTransfer(#Func,\
						std::bind(&ClassType::Func, \
								  this, \
								  std::placeholders::_1), \
						nExecType);

#define IPC_MESSAGE_TRANSFER_ATTR(Func)\
		registerAttribute(#Func, Func);

#define IPC_MESSAGE_TRANSFER_ATTR2(Func, VALUE)\
		registerAttribute(#Func, VALUE);

// JS函数注册宏 结束
#define IPC_MESSAGE_TRANSFER_END()\
		BaseClassType::registerFunction();\
	}

namespace amo {

    /*!
     * @class	transfer
     *
     * @brief	JS调用C++时消息遍历基类.
     * 			所有的JS对C++的调用都由此类派生
     */
    class transfer : public log_object,
        public std::enable_shared_from_this<transfer> {
    public:
        /*!
         * @typedef	std::function<bool(const std::string&,
         * 			ipc_msg::SmartType, amo::ipc_result&)> BerforeResultFunc
         *
         * @brief	回传执行结果前的回调函数
         */
        typedef std::function<bool(const std::string&,
                                   ipc_msg::SmartType,
                                   amo::ipc_result&)> BerforeResultFunc;
                                   
                                   
                                   
                                   
                                   
    public:
        /*!
         * @fn	template<typename T> std::shared_ptr<T> transfer::getDerivedClass()
         *
         * @brief	获取当前对象的智能指针.
         *
         * @tparam	T	派生类类型.
         *
         * @return	派生类的智能指针.
         */
        template<typename T>
        std::shared_ptr<T> getDerivedClass() {
            return std::dynamic_pointer_cast<T>(shared_from_this());
        }
    public:
    
        /*!
         * @fn	transfer::transfer()
         *
         * @brief	Default constructor 主要用作类实例的创建类
         * 			（通过这个transfer创建类对象）.
         */
        transfer() {
            transferName("transfer");
            m_oFuncMgr.name(transferName());
            setWorkOnRenderer(false);
            //m_bWorkOnRenderer = false;
            setObjectID(amo::uid::generate_uid());
            m_oFuncMgr.setObjectID(getObjectID());
            setFuncRegistered(false);
            setClassObject(false);
            m_bReleased = false;
        }
        /*!
         * @fn	transfer::transfer(const std::string& strName)
         *
         * @brief	Constructor.
         *
         * @param	strName	 transfer 名称.
         */
        transfer(const std::string& strName) {
            transferName(strName);
            m_oFuncMgr.name(strName);
            m_bWorkOnRenderer = false;
            setObjectID(amo::uid::generate_uid());
            m_oFuncMgr.setObjectID(getObjectID());
            setFuncRegistered(false);
            setClassObject(false);
            m_bReleased = false;
        }
        
        ~transfer() {
        
        }
        virtual any onRelase(ipc_msg::SmartType msg) {
        
            return undefined();
        }
        
        /**
         * @fn	virtual std::string transfer::getClass() const
         *
         * @brief	获取类名.
         *
         * @return	The class.
         */
        
        virtual std::string getClass() const {
            return "transfer";
        }
        
        /**
         * @fn	virtual transfer* transfer::getInterface(const std::string& name)
         *
         * @brief	通过类开获取接口.
         *
         * @param	name	The name.
         *
         * @return	null if it fails, else the interface.
         */
        
        virtual transfer* getInterface(const std::string& name) {
            if (name == transfer::getClass()) {
                return this;
            }
            
            return NULL;
        }
        
        /*!
         * @fn	void transfer::setObjectID(int64_t nID)
         *
         * @brief	设置transfer ID.
         *
         * @param	nID	transfer ID.
         */
        void setObjectID(int64_t nID) {
            m_nObjectID = nID;
        }
        
        /*!
         * @fn	int64_t transfer::getObjectID() const
         *
         * @brief	获取transfer ID.
         *
         * @return	返回transfer ID.
         */
        int64_t getObjectID() const {
            return m_nObjectID;
        }
        
        /**
         * @fn	std::string transfer::getObjectName() const
         *
         * @brief	获取当前transfer的名称.
         *
         * @return	The object name.
         */
        
        std::string getObjectName() const {
            return m_strObjectName;
        }
        
        /**
         * @fn	void transfer::setObjectName(std::string val)
         *
         * @brief	设置当前transfer的名称.
         *
         * @param	val	The value.
         */
        
        void setObjectName(std::string val) {
        
            m_strObjectName = val;
            m_oFuncMgr.setObjectName(getObjectName());
        }
        
        /**
         * @fn	bool transfer::isClassOjbect() const
         *
         * @brief	判断当前transfer是否为对象.
         *
         * @return	true if class ojbect, false if not.
         */
        
        bool isClassOjbect() const {
            return m_bClassObject;
        }
        
        /**
         * @fn	void transfer::setClassObject(bool val)
         *
         * @brief	设置当前Trasnfer是否为对象.
         *
         * @param	val	true to value.
         */
        
        void setClassObject(bool val) {
            m_bClassObject = val;
        }
        
        /*!
         * @fn	void transfer::setWorkOnRenderer(bool bWorkOnRenderer = true)
         *
         * @brief	设置当前transfer是否工作在渲染进程(渲染线程)上.
         *
         * @param	bWorkOnRenderer	(Optional) true to work on renderer.
         */
        void setWorkOnRenderer(bool bWorkOnRenderer = true) {
            m_bWorkOnRenderer = bWorkOnRenderer;
        }
        /*!
         * @fn	bool transfer::isWorkOnRenderer() const
         *
         * @brief	判断当前transfer是否工作在渲染进程(渲染线程)上.
         *
         * @return	true if work on renderer, false if not.
         */
        bool isWorkOnRenderer() const {
            return m_bWorkOnRenderer;
        }
        
        /*!
         * @fn	const std::string& transfer::transferName() const
         *
         * @brief	获取transfer 名称 类和类的实例对象名称相同..
         *
         * @return	返回transfer 名称.
         */
        const std::string& transferName() const {
            return m_strName;
        }
        /*!
         * @fn	void transfer::transferName(const std::string& strName)
         *
         * @brief	设置transfer 名称 类和类的实例对象名称相同.
         *
         * @param	strName	The transfer name.
         */
        void transferName(const std::string& strName) {
            m_strName = strName;
        }
        
        /*!
         * @fn	virtual transfer_type transfer::transferType()
         *
         * @brief	transfer 类型 包括类/对象.
         *
         * @return	返回 transfer_type.
         */
        virtual transfer_type transferType() {
            return transfer_unknown;
        }
        
        /**
         * @fn	transfer_multi_type transfer::multiType(const std::string& funcName)
         *
         * @brief	判断当前函数是否支持多线程执行.
         *
         * @param	funcName	Name of the function.
         *
         * @return	A transfer_multi_type.
         */
        
        transfer_multi_type multiType(const std::string& funcName) {
            return getFuncMgr().multiType(funcName);
        }
        
        /*!
         * @fn	transfer_func_type transfer::functionType(const std::string& funcName)
         *
         * @brief	获取函数类型普通/静态/构造.
         *
         * @param	funcName	函数名.
         *
         * @return	函数类型 transfer_func_type.
         */
        transfer_func_type functionType(const std::string& funcName) {
            return getFuncMgr().functionType(funcName);
        }
        
        /*!
         * @fn	transfer_exec_type transfer::execType(const std::string& funcName)
         *
         * @brief	获取函数的调用方式 普通/同步/异步.
         *
         * @param	funcName	函数名.
         *
         * @return	函数调用方式 transfer_exec_type.
         */
        transfer_exec_type execType(const std::string& funcName) {
            return getFuncMgr().execType(funcName);
        }
        
        
        /*!
         * @fn	virtual void transfer::registerFunction()
         *
         * @brief	注册JS函数 设置注册状态为true.
         */
        virtual void registerFunction() {
            setFuncRegistered(true);
        }
        
        /*!
         * @fn	virtual bool transfer::registerTransfer(const std::string& name,
         * 		function_wrapper::TransferFunc fn, int nType = 0)
         *
         * @brief	Registers the transfer.
         *
         * @param	name 	JS函数名.
         * @param	fn   	JS函数所对应的C++函数.
         * @param	nType	函数类型及执行方式.
         *
         * @return	true if it succeeds, false if it fails.
         */
        virtual bool registerTransfer(const std::string& name,
                                      function_wrapper::TransferFunc fn,
                                      int nType = 0) {
            return getFuncMgr().toMap().insert(
                       std::make_pair(name,
                                      function_wrapper(name, fn, nType))).second;
        }
        
        /*!
         * @fn	void transfer::addModule(const std::string& strName)
         *
         * @brief	添加需要调用的JS模块，模块存在于渲染进程中，
         * 			做为当前transfer的子模块调用.
         *
         * @param	strName	The name.
         */
        void addModule(const std::string& strName) {
            getFuncMgr().addModule(strName);
        }
        
        
        void addDepends(const std::string& strName) {
            getFuncMgr().addDepends(strName);
        }
        
        
        void registerAttribute(const std::string& strName, any val) {
            getFuncMgr().addAttribute(strName, val);
        }
        
        
        /*!
         * @fn	void transfer::setBeforeResultCallback(BerforeResultFunc fn)
         *
         * @brief	设置JS调用C++的结果回调处理函数.
         *
         * @param	fn	回调函数.
         */
        void setBeforeResultCallback(BerforeResultFunc fn) {
            m_fnResultCallback = fn;
        }
        
        /*!
         * @fn	BerforeResultFunc transfer::getBeforeResultCallback()
         *
         * @brief	获取调用结果回调处理函数，如果回调函数有效，将优先处理结果.
         *
         * @return	回调处理函数.
         */
        BerforeResultFunc getBeforeResultCallback() {
            return m_fnResultCallback;
        }
        
        /*!
         * @fn	amo::function_wrapperMgr& transfer::getFuncMgr()
         *
         * @brief	获取可以用JS访问的函数列表.
         *
         * @return	函数列表.
         */
        amo::function_wrapperMgr& getFuncMgr() {
            return m_oFuncMgr;
        }
        
        virtual any onDefaultMessageTransfer(ipc_msg::SmartType msg) {
            if (m_fnDefaultMsgFunc) {
                return m_fnDefaultMsgFunc(msg);
            }
            
            return nothing();
        }
        
        /*!
         * @fn	virtual any transfer::onMessageTransfer(ipc_msg::SmartType message)
         *
         * @brief	执行JS函数对应的C++函数.
         *
         * @param	进程消息.
         *
         * @return	any.
         */
        virtual any onMessageTransfer(ipc_msg::SmartType msg) {
        
            // 函数调用方式
            std::string strExecName = msg->getMessageName();
            
            std::shared_ptr<any_args_list> args = msg->getArgumentList();
            int nBrowserID = args->getInt(ipc_args_pos_info::BrowserID);
            int nFrameID = args->getInt(ipc_args_pos_info::FrameID);
            // 对应的C++函数名， 没有汉字
            std::string strFuncName = args->getString(ipc_args_pos_info::FuncName);
            // 查找当前transfer是否存在所给函数
            auto iter = getFuncMgr().toMap().find(strFuncName);
            
            amo::ipc_result result;
            result.setID(args->getInt(ipc_args_pos_info::MessageID));
            
            //JS普通调用C++, 这种执行方式不向调用者返回结果
            if (strExecName == IPC_NATIVE_EXECUTE) {
                any ret;
                
                if (iter == getFuncMgr().toMap().end()) {
                    ret = onDefaultMessageTransfer(msg);
                    
                    if (ret.is<nothing>()) {
                        return nothing();
                    }
                } else {
                    // 调用所注册的C++函数
                    ret = iter->second(msg);
                }
                
                result.setResult(ret);
                
                // 向调用者返回结果前先处理该结果
                if (m_fnResultCallback
                        && m_fnResultCallback(IPC_NATIVE_EXECUTE,
                                              msg,
                                              result)) {
                    //如果当前结果已被处理，那么不返回当前结果
                    return nothing();
                }
                
                return ret;
            }
            //JS同步调用C++，此消息需要通过管道向页面返回结果
            else if (strExecName == IPC_NATIVE_SYNC_EXECUTE)	{
                //
                if (iter != getFuncMgr().toMap().end()) {
                    any  ret = iter->second(msg);
                    result.setResult(ret);
                    
                    if (m_fnResultCallback
                            && m_fnResultCallback(IPC_NATIVE_SYNC_EXECUTE,
                                                  msg,
                                                  result)) {
                        //如果当前结果已被处理，那么不返回当前结果
                        return nothing();
                    }
                    
                    return ret;
                } else {
                    any ret = onDefaultMessageTransfer(msg);
                    
                    // 没有找到相关函数，但是还是需要返回一个值给调用者，以免死锁
                    if (ret.is<nothing>()) {
                        ret = undefined();
                    }
                    
                    result.setResult(ret);
                    
                    if (m_fnResultCallback
                            && m_fnResultCallback(IPC_NATIVE_SYNC_EXECUTE,
                                                  msg,
                                                  result)) {
                        return nothing();
                    }
                    
                    return ret;
                }
            }
            //JS异步调用C++，此消息需要通过JS向页面返回结果
            else if (strExecName == IPC_NATIVE_ASYNC_EXECUTE) {
                if (iter != getFuncMgr().toMap().end()) {
                    any  ret = iter->second(msg);
                    result.setResult(ret);
                    
                    if (m_fnResultCallback
                            && m_fnResultCallback(IPC_NATIVE_ASYNC_EXECUTE,
                                                  msg,
                                                  result)) {
                        return nothing();
                    }
                    
                    return ret;
                } else {
                
                    any ret = onDefaultMessageTransfer(msg);
                    
                    // 没有找到相关函数，但是还是需要返回一个值给调用者，以免死锁
                    if (ret.is<nothing>()) {
                        ret = undefined();
                    }
                    
                    result.setResult(ret);
                    
                    if (m_fnResultCallback
                            && m_fnResultCallback(IPC_NATIVE_ASYNC_EXECUTE,
                                                  msg,
                                                  result)) {
                        return nothing();
                    }
                    
                    return ret;
                }
            }
            
            return nothing();
        }
        
        /*!
         * @fn	bool transfer::isFuncRegistered() const
         *
         * @brief	查询transfer注册状态.
         *
         * @return	true/false 已注册/未注册.
         */
        bool isFuncRegistered() const {
            return m_bIsRegistered;
        }
        
        /*!
         * @fn	void transfer::setFuncRegistered(bool val)
         *
         * @brief	设置transfer注册状态.
         *
         * @param	true/false 注册/未注册.
         */
        void setFuncRegistered(bool val) {
            m_bIsRegistered = val;
        }
        
        /**
        * @fn	std::function<void(IPCMessage::SmartType) > getDefaultTriggerEventFunc() const
        *
        * @brief	获取事件触发函数.
        *
        * @return	The trigger event function.
        */
        
        std::function<void(ipc_msg::SmartType) > getDefaultTriggerEventFunc() const {
            return m_fnDefaultTriggerEventFunc;
        }
        
        /**
        * @fn	virtual void Transfer::setDefaultTriggerEventFunc( std::function<void(IPCMessage::SmartType) > val)
        *
        * @brief	设置Transfer中需要触发事件时的回调函数， 一般用于外部Transfer的事件触发.
        *
        * @param	val	The value.
        */
        
        virtual void setDefaultTriggerEventFunc(
            std::function<void(ipc_msg::SmartType) >
            val) {
            m_fnDefaultTriggerEventFunc = val;
        }
        
        /**
         * @fn	std::function<void(ipc_msg::SmartType) > getTriggerEventFunc() const
         *
         * @brief	获取事件触发函数.
         *
         * @return	The trigger event function.
         */
        
        std::function<void(ipc_msg::SmartType) > getTriggerEventFunc() const  {
            return m_fnTriggerEventFunc;
        }
        
        /**
         * @fn	virtual void transfer::setTriggerEventFunc(std::function<void(ipc_msg::SmartType) > val)
         *
         * @brief	设置transfer中需要触发事件时的回调函数， 一般用于外部transfer的事件触发.
         *
         * @param	val	The value.
         */
        
        virtual void setTriggerEventFunc(std::function<void(ipc_msg::SmartType) > val) {
            m_fnTriggerEventFunc = val;
        }
        
        any triggerEvent(const transfer_event_info& info) {
        
            ipc_msg::SmartType ipcMessage(new ipc_msg());
            ipcMessage->setMessageName(IPC_NATIVE_EXECUTE);
            std::shared_ptr<any_args_list>& ipcArgs = ipcMessage->getArgumentList();
            
            ipcArgs->setValue(ipc_args_pos_info::TransferName, "ipcRenderer");
            ipcArgs->setValue(ipc_args_pos_info::TransferID, 0);
            ipcArgs->setValue(ipc_args_pos_info::EventObjectID, getObjectID());
            ipcArgs->setValue(ipc_args_pos_info::BrowserID, info.browser);
            ipcArgs->setValue(ipc_args_pos_info::FrameID, info.frame);
            
            if (info.toAll) {
                ipcArgs->setValue(ipc_args_pos_info::FuncName, "emitEventAllFrame");
            } else {
                ipcArgs->setValue(ipc_args_pos_info::FuncName, "triggerEvent");
            }
            
            if (info.name == "transfer.release") {
                ipcArgs->setValue(ipc_args_pos_info::FuncName, "releaseAllTransfer");
            }
            
            ipcArgs->setValue(0, info.name);
            ipcArgs->setValue(1, info.toJson());
            //ipcArgs->setValue(1, info.toJson());
            ipcArgs->setValue(ipc_args_pos_info::ArgsLength, 2);
            
            if (getTriggerEventFunc()) {
                getTriggerEventFunc()(ipcMessage);
            } else if (getDefaultTriggerEventFunc()) {
                getDefaultTriggerEventFunc()(ipcMessage);
            }
            
            return undefined();
        }
        std::function < any(ipc_msg::SmartType) > getDefaultMsgFunc() const {
            return m_fnDefaultMsgFunc;
        }
        void setDefaultMsgFunc(std::function < any(ipc_msg::SmartType) > val) {
            m_fnDefaultMsgFunc = val;
        }
    protected:
    
        /*! @brief	JS调用C++回调处理函数集合. */
        std::map < std::string, function_wrapper > m_mpJsFunction;
        
        /*! @brief	transfer名称. */
        std::string m_strName;
        
        /*! @brief	结果返回前的回调函数，如果该函数返回true那么将不会再向调用都返回执行结果. */
        BerforeResultFunc m_fnResultCallback;
        
        /*! @brief	JS调用C++回调处理函数集合. */
        function_wrapperMgr m_oFuncMgr;
        
        /*! @brief	是否运行在Renderer线程上. */
        bool m_bWorkOnRenderer;
        
        /*! @brief	当前transfer ID. */
        int64_t m_nObjectID;
        
        /** @brief	当前transfer 名称. */
        std::string m_strObjectName;
        
        /*! @brief	判断当前类是否已经注册. */
        bool m_bIsRegistered;
        
        /** @brief	判断当前transfer是类还是对象. */
        bool m_bClassObject;
        
        /** @brief	Transfer中产生的默认事件触发函数，m_fnTriggerEventFunc无效时会尝试使用该函数. */
        std::function<void(ipc_msg::SmartType)> m_fnDefaultTriggerEventFunc;
        
        /** @brief	transfer中产生的事件触发函数. */
        std::function<void(ipc_msg::SmartType)> m_fnTriggerEventFunc;
        
        /** @brief	默认消息处理函数，如果没有找到消息处理函数将触发该函数. */
        std::function<any(ipc_msg::SmartType)> m_fnDefaultMsgFunc;
        /** @brief	是否已经被释放掉，（在页面上释放掉）. */
        std::atomic_bool m_bReleased;
    };
    
}


#endif //AMO_TRANSFER_590C7217_1E3A_4E4F_A11C_BBBE47CD5E75_HPP__
