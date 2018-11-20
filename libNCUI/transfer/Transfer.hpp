// Created by amoylel on 06/12/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_TRANSFER_HPP__
#define AMO_TRANSFER_HPP__
#include <string>
#include <map>

#include <amo/json.hpp>
#include <amo/stdint.hpp>
#include <amo/logger.hpp>

#include "ipc/Any.hpp"
#include "ipc/IPCMessage.hpp"
#include "transfer/FunctionWrapper.hpp"
#include "transfer/TransferEventInfo.hpp"


#ifndef AMO_CLASS_TRANSFER_DECLARE
#define AMO_CLASS_TRANSFER_DECLARE(LIB_API)\
extern "C"{\
	LIB_API bool registerTransfer(std::shared_ptr<amo::TransferRegister> info);\
	LIB_API void unregisterTransfer();\
	LIB_API void removeTransferByName(const std::string& strClass);\
	LIB_API void removeTransferByID(const int64_t& nID);\
}
#endif

#ifndef AMO_CLASS_REGISTER_BEGIN
#define AMO_CLASS_REGISTER_BEGIN()\
using namespace amo;\
ClassTransfer::getTransferMap() = info->transferMap;\
int nBrowserID = info->nBrowserID;\
auto fn = info->fnCallback;\
amo::log::register_logger(info->pLogger); \

#endif

#ifndef AMO_CLASS_REGISTER
#define AMO_CLASS_REGISTER(ClassName) \
{\
	auto pTransfer = ClassTransfer::getUniqueTransfer<ClassName>(); \
	pTransfer->setModuleName(info->moduleName);\
	if (fn) {\
		fn(nBrowserID, pTransfer);\
	}\
	\
}\

#endif

#ifndef AMO_CLASS_REGISTER_END
#define AMO_CLASS_REGISTER_END()\
return true;
#endif



#ifndef AMO_UNREGISTER_TRANSFER
#define AMO_UNREGISTER_TRANSFER(LIB_API)\
LIB_API void unregisterTransfer(){\
	using namespace amo;\
	ClassTransfer::getTransferMap().reset();\
	return;\
}\

#endif


#ifndef AMO_REMOVE_TRANSFER_BY_NAME
#define AMO_REMOVE_TRANSFER_BY_NAME(LIB_API)\
LIB_API void removeTransferByName(const std::string& strClassName){\
	amo::ClassTransfer::removeTransferByName(strClassName);\
	return;\
}\

#endif

#ifndef AMO_REMOVE_TRANSFER_BY_ID
#define AMO_REMOVE_TRANSFER_BY_ID(LIB_API)\
LIB_API void removeTransferByID(const int64_t& nID){\
	amo::ClassTransfer::removeTransfer(nID);\
	return;\
}\

#endif

#ifndef AMO_REGISTER_TRANSFER_BEGIN
#define AMO_REGISTER_TRANSFER_BEGIN(LIB_API)\
	AMO_UNREGISTER_TRANSFER(LIB_API)\
	AMO_REMOVE_TRANSFER_BY_NAME(LIB_API)\
	AMO_REMOVE_TRANSFER_BY_ID(LIB_API)\
	LIB_API bool registerTransfer(std::shared_ptr<amo::TransferRegister> info) {\
		AMO_CLASS_REGISTER_BEGIN()\

#endif

#ifndef	AMO_REGISTER_TRANSFER_END
#define AMO_REGISTER_TRANSFER_END()\
	AMO_CLASS_REGISTER_END()\
}\

#endif




// JS函数注册宏 开始
#define AMO_CEF_MESSAGE_TRANSFER_BEGIN(ClassName, BaseClassName)\
	virtual void registerFunction(){\
		typedef ClassName ClassType;\
		typedef BaseClassName BaseClassType;\

// JS函数注册 同名函数
#define AMO_CEF_MESSAGE_TRANSFER_FUNC(Func, nExecType)\
		registerTransfer(#Func,\
						std::bind(&ClassType::Func, \
								  this, \
								  std::placeholders::_1), \
						nExecType);

#define AMO_CEF_MESSAGE_TRANSFER_ATTR(Func)\
		registerAttribute(#Func, Func);

#define AMO_CEF_MESSAGE_TRANSFER_ATTR2(Func, VALUE)\
		registerAttribute(#Func, VALUE);

// JS函数注册宏 结束
#define AMO_CEF_MESSAGE_TRANSFER_END()\
		BaseClassType::registerFunction();\
	}

namespace amo {

    /*!
     * @class	Transfer
     *
     * @brief	JS调用C++时消息遍历基类.
     * 			所有的JS对C++的调用都由此类派生
     */
    class Transfer : public log_object,
        public std::enable_shared_from_this<Transfer> {
    public:
        /*!
         * @typedef	std::function<bool(const std::string&,
         * 			IPCMessage::SmartType, amo::IPCResult&)> BerforeResultFunc
         *
         * @brief	回传执行结果前的回调函数
         */
        typedef std::function<bool(const std::string&,
                                   IPCMessage::SmartType,
                                   amo::IPCResult&)> BerforeResultFunc;
                                   
                                   
                                   
                                   
                                   
    public:
        /*!
         * @fn	template<typename T> std::shared_ptr<T> Transfer::getDerivedClass()
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
         * @fn	Transfer::Transfer()
         *
         * @brief	Default constructor 主要用作类实例的创建类
         * 			（通过这个Transfer创建类对象）.
         */
        Transfer() {
            transferName("Transfer");
            m_oFuncMgr.name(transferName());
            setWorkOnRenderer(false);
            //m_bWorkOnRenderer = false;
            setObjectID(amo::uid::generate_uid());
            m_oFuncMgr.setObjectID(getObjectID());
            setFuncRegistered(false);
            setTransferObject(true);
            m_bReleased = false;
            
        }
        /*!
         * @fn	Transfer::Transfer(const std::string& strName)
         *
         * @brief	Constructor.
         *
         * @param	strName	 Transfer 名称.
         */
        Transfer(const std::string& strName) {
            transferName(strName);
            m_oFuncMgr.name(strName);
            m_bWorkOnRenderer = false;
            setObjectID(amo::uid::generate_uid());
            m_oFuncMgr.setObjectID(getObjectID());
            setFuncRegistered(false);
            setTransferObject(true);
            m_bReleased = false;
        }
        
        ~Transfer() {
            /* $cdevel("release transfer:transferName = \"{0}\"， objectName = \"{1}\"， objectID = {2}",
                     transferName(), getObjectName(), getObjectID());*/
        }
        
        virtual Any onRelase(IPCMessage::SmartType msg) {
        
            return Undefined();
        }
        
        /**
         * @fn	virtual std::string Transfer::getClass() const
         *
         * @brief	获取类名.
         *
         * @return	The class.
         */
        
        virtual std::string getClass() const {
            return "Transfer";
        }
        
        /**
         * @fn	virtual Transfer* Transfer::getInterface(const std::string& name)
         *
         * @brief	通过类开获取接口.
         *
         * @param	name	The name.
         *
         * @return	null if it fails, else the interface.
         */
        
        virtual Transfer* getInterface(const std::string& name) {
            if (name == Transfer::getClass()) {
                return this;
            }
            
            return NULL;
        }
        
        /*!
         * @fn	void Transfer::setObjectID(int64_t nID)
         *
         * @brief	设置Transfer ID.
         *
         * @param	nID	Transfer ID.
         */
        void setObjectID(int64_t nID) {
            m_nObjectID = nID;
        }
        
        /*!
         * @fn	int64_t Transfer::getObjectID() const
         *
         * @brief	获取Transfer ID.
         *
         * @return	返回Transfer ID.
         */
        int64_t getObjectID() const {
            return m_nObjectID;
        }
        
        /**
         * @fn	std::string Transfer::getObjectName() const
         *
         * @brief	获取当前Transfer的名称.
         *
         * @return	The object name.
         */
        
        std::string getObjectName() const {
            return m_strObjectName;
        }
        
        /**
         * @fn	void Transfer::setObjectName(std::string val)
         *
         * @brief	设置当前Transfer的名称.
         *
         * @param	val	The value.
         */
        
        void setObjectName(std::string val) {
        
            m_strObjectName = val;
            m_oFuncMgr.setObjectName(getObjectName());
        }
        
        /**
         * @fn	bool Transfer::isTransferOjbect() const
         *
         * @brief	判断当前Transfer是否为对象.
         *
         * @return	true if class ojbect, false if not.
         */
        
        bool isTransferOjbect() const {
            return m_bTransferObject;
        }
        
        /**
         * @fn	void Transfer::setClassObject(bool val)
         *
         * @brief	设置当前Trasnfer是否为对象.
         *
         * @param	val	true to value.
         */
        
        void setTransferObject(bool val) {
            m_bTransferObject = val;
        }
        
        /*!
         * @fn	void Transfer::setWorkOnRenderer(bool bWorkOnRenderer = true)
         *
         * @brief	设置当前Transfer是否工作在渲染进程(渲染线程)上.
         *
         * @param	bWorkOnRenderer	(Optional) true to work on renderer.
         */
        void setWorkOnRenderer(bool bWorkOnRenderer = true) {
            m_bWorkOnRenderer = bWorkOnRenderer;
        }
        /*!
         * @fn	bool Transfer::isWorkOnRenderer() const
         *
         * @brief	判断当前Transfer是否工作在渲染进程(渲染线程)上.
         *
         * @return	true if work on renderer, false if not.
         */
        bool isWorkOnRenderer() const {
            return m_bWorkOnRenderer;
        }
        
        /*!
         * @fn	const std::string& Transfer::transferName() const
         *
         * @brief	获取Transfer 名称 类和类的实例对象名称相同..
         *
         * @return	返回Transfer 名称.
         */
        const std::string& transferName() const {
            return m_strName;
        }
        /*!
         * @fn	void Transfer::transferName(const std::string& strName)
         *
         * @brief	设置Transfer 名称 类和类的实例对象名称相同.
         *
         * @param	strName	The Transfer name.
         */
        void transferName(const std::string& strName) {
            m_strName = strName;
        }
        
        /*!
         * @fn	virtual TransferType Transfer::transferType()
         *
         * @brief	Transfer 类型 包括类/对象.
         *
         * @return	返回 TransferType.
         */
        virtual TransferType transferType() {
            return TransferUnknown;
        }
        
        /**
         * @fn	TransferMultiType Transfer::multiType(const std::string& funcName)
         *
         * @brief	判断当前函数是否支持多线程执行.
         *
         * @param	funcName	Name of the function.
         *
         * @return	A TransferMultiType.
         */
        
        TransferMultiType multiType(const std::string& funcName) {
            return getFuncMgr().multiType(funcName);
        }
        
        /*!
         * @fn	TransferFuncType Transfer::functionType(const std::string& funcName)
         *
         * @brief	获取函数类型普通/静态/构造.
         *
         * @param	funcName	函数名.
         *
         * @return	函数类型 TransferFuncType.
         */
        TransferFuncType functionType(const std::string& funcName) {
            return getFuncMgr().functionType(funcName);
        }
        
        /*!
         * @fn	TransferExecType Transfer::execType(const std::string& funcName)
         *
         * @brief	获取函数的调用方式 普通/同步/异步.
         *
         * @param	funcName	函数名.
         *
         * @return	函数调用方式 TransferExecType.
         */
        TransferExecType execType(const std::string& funcName) {
            return getFuncMgr().execType(funcName);
        }
        
        
        /*!
         * @fn	virtual void Transfer::registerFunction()
         *
         * @brief	注册JS函数 设置注册状态为true.
         */
        virtual void registerFunction() {
            setFuncRegistered(true);
        }
        
        /*!
         * @fn	virtual bool Transfer::registerTransfer(const std::string& name,
         * 		FunctionWrapper::TransferFunc fn, int nType = 0)
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
                                      FunctionWrapper::TransferFunc fn,
                                      int nType = 0) {
            return getFuncMgr().toMap().insert(
                       std::make_pair(name,
                                      FunctionWrapper(name, fn, nType))).second;
        }
        
        /*!
         * @fn	void Transfer::addModule(const std::string& strName)
         *
         * @brief	添加需要调用的JS模块，模块存在于渲染进程中，
         * 			做为当前Transfer的子模块调用.
         *
         * @param	strName	The name.
         */
        void addModule(const std::string& strName) {
            getFuncMgr().addModule(strName);
        }
        
        
        void addDepends(const std::string& strName) {
            getFuncMgr().addDepends(strName);
        }
        
        
        void registerAttribute(const std::string& strName, Any val) {
            getFuncMgr().addAttribute(strName, val);
        }
        
        
        /*!
         * @fn	void Transfer::setBeforeResultCallback(BerforeResultFunc fn)
         *
         * @brief	设置JS调用C++的结果回调处理函数.
         *
         * @param	fn	回调函数.
         */
        void setBeforeResultCallback(BerforeResultFunc fn) {
            m_fnResultCallback = fn;
        }
        
        /*!
         * @fn	BerforeResultFunc Transfer::getBeforeResultCallback()
         *
         * @brief	获取调用结果回调处理函数，如果回调函数有效，将优先处理结果.
         *
         * @return	回调处理函数.
         */
        BerforeResultFunc getBeforeResultCallback() {
            return m_fnResultCallback;
        }
        
        /*!
         * @fn	amo::FunctionWrapperMgr& Transfer::getFuncMgr()
         *
         * @brief	获取可以用JS访问的函数列表.
         *
         * @return	函数列表.
         */
        amo::FunctionWrapperMgr& getFuncMgr() {
            return m_oFuncMgr;
        }
        
        virtual Any onDefaultMessageTransfer(IPCMessage::SmartType msg) {
            if (m_fnDefaultMsgFunc) {
                return m_fnDefaultMsgFunc(msg);
            }
            
            return Nothing();
        }
        
        
        /*!
         * @fn	virtual Any Transfer::onMessageTransfer(IPCMessage::SmartType message)
         *
         * @brief	执行JS函数对应的C++函数.
         *
         * @param	进程消息.
         *
         * @return	Any.
         */
        virtual Any onMessageTransfer(IPCMessage::SmartType msg) {
        
            // 函数调用方式
            std::string strExecName = msg->getMessageName();
            
            std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
            int nBrowserID = args->getInt(IPCArgsPosInfo::BrowserID);
            int nFrameID = args->getInt(IPCArgsPosInfo::FrameID);
            // 对应的C++函数名， 没有汉字
            std::string strFuncName = args->getString(IPCArgsPosInfo::FuncName);
            // 查找当前Transfer是否存在所给函数
            auto iter = getFuncMgr().toMap().find(strFuncName);
            
            amo::IPCResult result;
            result.setID(args->getInt(IPCArgsPosInfo::MessageID));
            
            //JS普通调用C++, 这种执行方式不向调用者返回结果
            if (strExecName == MSG_NATIVE_EXECUTE) {
                Any ret;
                
                if (iter == getFuncMgr().toMap().end()) {
                    ret = onDefaultMessageTransfer(msg);
                    
                    if (ret.is<Nothing>()) {
                        return Nothing();
                    }
                } else {
                    // 调用所注册的C++函数
                    ret = iter->second(msg);
                }
                
                
                
                result.setResult(ret);
                
                // 向调用者返回结果前先处理该结果
                if (m_fnResultCallback
                        && m_fnResultCallback(MSG_NATIVE_EXECUTE,
                                              msg,
                                              result)) {
                    //如果当前结果已被处理，那么不返回当前结果
                    return Nothing();
                }
                
                return ret;
            }
            //JS同步调用C++，此消息需要通过管道向页面返回结果
            else if (strExecName == MSG_NATIVE_SYNC_EXECUTE)	{
                //
                if (iter != getFuncMgr().toMap().end()) {
                    Any  ret = iter->second(msg);
                    result.setResult(ret);
                    
                    if (m_fnResultCallback
                            && m_fnResultCallback(MSG_NATIVE_SYNC_EXECUTE,
                                                  msg,
                                                  result)) {
                        //如果当前结果已被处理，那么不返回当前结果
                        return Nothing();
                    }
                    
                    return ret;
                } else {
                    Any ret = onDefaultMessageTransfer(msg);
                    
                    // 没有找到相关函数，但是还是需要返回一个值给调用者，以免死锁
                    if (ret.is<Nothing>()) {
                        ret = Undefined();
                    }
                    
                    result.setResult(ret);
                    
                    if (m_fnResultCallback
                            && m_fnResultCallback(MSG_NATIVE_SYNC_EXECUTE,
                                                  msg,
                                                  result)) {
                        return Nothing();
                    }
                    
                    return ret;
                }
            }
            //JS异步调用C++，此消息需要通过JS向页面返回结果
            else if (strExecName == MSG_NATIVE_ASYNC_EXECUTE) {
                if (iter != getFuncMgr().toMap().end()) {
                    Any  ret = iter->second(msg);
                    result.setResult(ret);
                    
                    if (m_fnResultCallback
                            && m_fnResultCallback(MSG_NATIVE_ASYNC_EXECUTE,
                                                  msg,
                                                  result)) {
                        return Nothing();
                    }
                    
                    return ret;
                } else {
                    Any ret = onDefaultMessageTransfer(msg);
                    
                    // 没有找到相关函数，但是还是需要返回一个值给调用者，以免死锁
                    if (ret.is<Nothing>()) {
                        ret = Undefined();
                    }
                    
                    result.setResult(ret);
                    
                    if (m_fnResultCallback
                            && m_fnResultCallback(MSG_NATIVE_ASYNC_EXECUTE,
                                                  msg,
                                                  result)) {
                        return Nothing();
                    }
                    
                    return ret;
                }
            }
            
            return Nothing();
        }
        
        /*!
         * @fn	bool Transfer::isFuncRegistered() const
         *
         * @brief	查询Transfer注册状态.
         *
         * @return	true/false 已注册/未注册.
         */
        bool isFuncRegistered() const {
            return m_bIsRegistered;
        }
        
        /*!
         * @fn	void Transfer::setFuncRegistered(bool val)
         *
         * @brief	设置Transfer注册状态.
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
        
        std::function<void(IPCMessage::SmartType) > getDefaultTriggerEventFunc() const {
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
            std::function<void(IPCMessage::SmartType) >
            val) {
            m_fnDefaultTriggerEventFunc = val;
        }
        
        
        /**
         * @fn	std::function<void(IPCMessage::SmartType) > getTriggerEventFunc() const
         *
         * @brief	获取事件触发函数.
         *
         * @return	The trigger event function.
         */
        
        std::function<void(IPCMessage::SmartType) > getTriggerEventFunc() const  {
            return m_fnTriggerEventFunc;
        }
        
        
        /**
         * @fn	void Transfer::setTriggerEventFunc(std::function<void(IPCMessage::SmartType) > val)
         *
         * @brief	设置Transfer中需要触发事件时的回调函数，
         * 			一般用于外部Transfer的事件触发.
         *
         * @param	val	The value.
         */
        
        virtual void setTriggerEventFunc(std::function<void(IPCMessage::SmartType) >
                                         val) {
            m_fnTriggerEventFunc = val;
        }
        
        Any triggerEvent(const TransferEventInfo& info) {
        
            IPCMessage::SmartType ipcMessage(new IPCMessage());
            ipcMessage->setMessageName(MSG_NATIVE_EXECUTE);
            std::shared_ptr<AnyArgsList>& ipcArgs = ipcMessage->getArgumentList();
            
            ipcArgs->setValue(IPCArgsPosInfo::TransferName, "ipcRenderer");
            ipcArgs->setValue(IPCArgsPosInfo::TransferID, 0);
            ipcArgs->setValue(IPCArgsPosInfo::EventObjectID, getObjectID());
            ipcArgs->setValue(IPCArgsPosInfo::BrowserID, info.browser);
            ipcArgs->setValue(IPCArgsPosInfo::FrameID, info.frame);
            
            if (info.toAll) {
                ipcArgs->setValue(IPCArgsPosInfo::FuncName, "emitEventAllFrame");
            } else {
                ipcArgs->setValue(IPCArgsPosInfo::FuncName, "triggerEvent");
            }
            
            if (info.name == "transfer.release") {
                ipcArgs->setValue(IPCArgsPosInfo::FuncName, "releaseAllTransfer");
            }
            
            ipcArgs->setValue(0, info.name);
            ipcArgs->setValue(1, info.toJson());
            //ipcArgs->setValue(1, info.toJson());
            ipcArgs->setValue(IPCArgsPosInfo::ArgsLength, 2);
            
            if (getTriggerEventFunc()) {
                getTriggerEventFunc()(ipcMessage);
            } else if (getDefaultTriggerEventFunc()) {
                getDefaultTriggerEventFunc()(ipcMessage);
            }
            
            return Undefined();
        }
        std::function < Any(IPCMessage::SmartType) > getDefaultMsgFunc() const {
            return m_fnDefaultMsgFunc;
        }
        void setDefaultMsgFunc(std::function < Any(IPCMessage::SmartType) > val) {
            m_fnDefaultMsgFunc = val;
        }
        
        std::string getModuleName() const {
            return m_strModuleName;
        }
        void setModuleName(const std::string& val) {
            m_strModuleName = val;
        }
    protected:
    
        /*! @brief	JS调用C++回调处理函数集合. */
        std::map < std::string, FunctionWrapper > m_mpJsFunction;
        
        /*! @brief	Transfer名称. */
        std::string m_strName;
        
        /*! @brief	结果返回前的回调函数，如果该函数返回true那么将不会再向调用都返回执行结果. */
        BerforeResultFunc m_fnResultCallback;
        
        /*! @brief	JS调用C++回调处理函数集合. */
        FunctionWrapperMgr m_oFuncMgr;
        
        /*! @brief	是否运行在Renderer线程上. */
        bool m_bWorkOnRenderer;
        
        /*! @brief	当前Transfer ID. */
        int64_t m_nObjectID;
        
        /** @brief	当前Transfer 名称. */
        std::string m_strObjectName;
        
        /*! @brief	判断当前类是否已经注册. */
        bool m_bIsRegistered;
        
        /** @brief	判断当前transfer是类还是对象. */
        bool m_bTransferObject;
        
        /** @brief	Transfer中产生的默认事件触发函数，m_fnTriggerEventFunc无效时会尝试使用该函数. */
        std::function<void(IPCMessage::SmartType)> m_fnDefaultTriggerEventFunc;
        
        /** @brief	Transfer中产生的事件触发函数. */
        std::function<void(IPCMessage::SmartType)> m_fnTriggerEventFunc;
        
        /** @brief	默认消息处理函数，如果没有找到消息处理函数将触发该函数. */
        std::function<Any(IPCMessage::SmartType)> m_fnDefaultMsgFunc;
        
        /** @brief	是否已经被释放掉，（在页面上释放掉）. */
        std::atomic_bool m_bReleased;
        
        /** @brief	所属模块名称. */
        std::string m_strModuleName;
        
        amo::u8json userData;
    };
    
}


#endif // AMO_TRANSFER_HPP__
