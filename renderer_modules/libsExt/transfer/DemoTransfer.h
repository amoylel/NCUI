// Created by amoylel on 11/24/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_DEMOTRANSFER_H__
#define AMO_DEMOTRANSFER_H__




#include <transfer/RunnableTransfer.hpp>
#include <amo/singleton.hpp>


// 最简单的扩展，需要提供以下接口
//namespace amo {
//	class DemoTransfer
//		: public RunnableTransfer
//		, public amo::singleton<DemoTransfer> {
//	public:
//		DemoTransfer();
//		Any onCreateClass(IPCMessage::SmartType msg) override;
//		virtual std::string getClass() const override;
//		virtual Transfer* getInterface(const std::string& name) override;
//
//		Any foo(IPCMessage::SmartType msg);
//
//		AMO_CEF_MESSAGE_TRANSFER_BEGIN(DemoTransfer, RunnableTransfer)
//			AMO_CEF_MESSAGE_TRANSFER_FUNC(foo, TransferFuncNormal | TransferExecNormal)
//			AMO_CEF_MESSAGE_TRANSFER_END()
//
//	};
//}

//
//// Transfer类型
//enum TransferType {
//    TransferUnknown = -1,
//    TransferClass,						//类
//    TransferObject						//对象
//};
//
//// 多线程支持
//enum TransferMultiType {
//    TransferMultiUnkown = -1, //未知
//    TransferMultiSupport = 0 << 16,  // 允许在其他线程上执行（默认）
//    TransferMultiDisabled = 1 << 16, // 禁止多线程执行
//};
//// JS类中的函数类型，
//enum TransferFuncType {
//    TransferFuncUnknown = -1,					// 未知
//    TransferFuncNormal = 0 << 8,				// 普通函数
//    TransferFuncStatic = 1 << 8,				// 静态函数
//    TransferFuncConstructor = 2 << 8,			// 构造函数
//    TransferFuncConstProperty = 3 << 8,			// 类静态常量属性，不能被赋值
//    TransferFuncClassProperty = 4 << 8,			// 类属性，可以通过.CLASS访问赋值
//    TransferFuncMemberProperty = 5 << 8,		// 对象属性，可以赋值
//};
//
//// JS在调用C++时的同步类型
//enum TransferExecType {
//    TransferExecUnknown = -1,	//未知
//    TransferExecNormal = 0,		//普通
//    TransferExecSync = 1,		//同步
//    TransferExecAsync = 2,		//异步
//};



#define AMO_CONFIG_GETTER_SETTER4(item)\
	Any item (IPCMessage::SmartType msg) {\
		std::shared_ptr<AnyArgsList> args = msg->getArgumentList();\
		Any& val = args->getValue(0);\
		if (val.is<Nil>() || val.is<Undefined>()) {\
			return GetConfigValue < decltype(this-> m_##item ) > (this-> m_##item );\
		} else {\
			return SetConfigValue < decltype(this-> m_##item ) > (this-> m_##item , val);\
		}\
	}

namespace amo {
    class DemoTransfer
        : public RunnableTransfer
        , public amo::singleton<DemoTransfer> {
    public:
    
        DemoTransfer();
        Any onCreateClass(IPCMessage::SmartType msg) override;
        virtual std::string getClass() const override;
        virtual Transfer* getInterface(const std::string& name) override;
        
        Any add(IPCMessage::SmartType msg);
        Any sub(IPCMessage::SmartType msg);
        Any mul(IPCMessage::SmartType msg);
        Any div(IPCMessage::SmartType msg);
        Any sleepForWeakup(IPCMessage::SmartType msg);
        
        
        AMO_CONFIG_GETTER_SETTER(staticTestID)
        AMO_CONFIG_GETTER_SETTER(constStaticTestID)
        AMO_CONFIG_GETTER_SETTER(memberTestID)
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(DemoTransfer, RunnableTransfer)
        // 成员函数
        AMO_CEF_MESSAGE_TRANSFER_FUNC(add, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(sub, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(mul, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(div, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(sleepForWeakup, TransferFuncNormal | TransferExecNormal)
        
        AMO_CEF_MESSAGE_TRANSFER_FUNC(staticTestID,
                                      TransferFuncClassProperty | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(constStaticTestID,
                                      TransferFuncConstProperty | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(memberTestID,
                                      TransferFuncMemberProperty | TransferExecSync)
                                      
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    private:
        int m_staticTestID;
        int m_constStaticTestID;
        int m_memberTestID;
    };
}


#endif // AMO_DEMOTRANSFER_H__