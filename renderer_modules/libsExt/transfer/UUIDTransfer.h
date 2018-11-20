// Created by amoylel on 13/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef LIBSEXT_UUIDTRANSFER_03786AD7_00D1_4B00_8C9C_582243FBDDD6_H__
#define LIBSEXT_UUIDTRANSFER_03786AD7_00D1_4B00_8C9C_582243FBDDD6_H__







#include <transfer/RunnableTransfer.hpp>


namespace amo {

    /*!
     * @class	UUID
     *
     * @chapter extend
     *
     * @extend Runnable
     *
     * @brief	UUID 生成类,如果使用频繁，请直接调用静态函数返回UUID字符串，纯UUID对象也没什么意义.
     * 			<br>工作线程**Renderer线程**
     */
    
    class UUIDTransfer
        : public RunnableTransfer
        , public amo::singleton<UUIDTransfer> {
    public:
    
        /**
         * @fn	UUIDTransfer::UUID();
         *
         * @tag constructor
         *
         * @brief	创建一个UUID 对象.
         * @return  #UUID 实例
         *
         * @example
         *
         ```
        	include('UUID');
        	var uuid = new UUID();
         ```
         */
        
        UUIDTransfer();
        
        UUIDTransfer(const std::string & str);
        
        ~UUIDTransfer();
        
        virtual Any onCreateClass(IPCMessage::SmartType msg) override;
        virtual std::string getClass() const override;
        virtual Transfer* getInterface(const std::string& name) override;
        
        /**
         * @fn	Any UUIDTransfer::fromString(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	用一个字符串构建UUID.
         *
         * @param	#String 字符串.
         *
         * @return	#UUID 返回一个UUID对象.
         *
         * @example
         *
         ```
        	include('UUID');
        	var uuid = UUID.fromString('f1adcc5b-6943-4929-8a2f-2b64691c3867');
        	console.log(uuid);
        	console.log(uuid.toString());
         ```
         */
        
        Any fromString(IPCMessage::SmartType msg);
        
        /**
        * @fn	Any UUIDTransfer::getString(IPCMessage::SmartType msg);
        *
        * @tag static sync
        *
        * @brief	直接获取一个UUID的字符串，不创建新的对象.
        *
        * @return	#String UUID字符串.
        *
        * @example
        *
        ```
        	include('UUID');
        	console.log(UUID.getString());
        ```
        */
        
        Any getString(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any UUIDTransfer::nil(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	获取一个空UUID对象.
         *
         * @return	#UUID .
         * @example
         *
         ```
        	include('UUID');
        	console.log(UUID.nil().toString());
         ```
         */
        
        Any nil(IPCMessage::SmartType msg);
        
        /**
         * @fn	Any UUIDTransfer::nilString(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	获取一个空UUID的字符串.
         *
         *
         * @return	#String 一个表示空UUID的字符串.
         * @example
         *
         ```
        	include('UUID');
        	console.log(UUID.nilString());
         ```
         */
        
        Any nilString(IPCMessage::SmartType msg);
        
        
        
        /**
         * @fn	Any UUIDTransfer::toString(IPCMessage::SmartType msg);
         *
         * @tag sync
         *
         * @brief	将当前UUID转换为字符串.
         *
         *
         * @return	#String UUID字符串.
         *
         * @example
         *
         ```
        	include('UUID');
        	var uuid = new UUID();
        	console.log(uuid.toString());
         ```
         */
        
        Any toString(IPCMessage::SmartType msg);
        
        
    public:
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(UUIDTransfer, RunnableTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(fromString, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(getString, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(nil, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(nilString, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(toString, TransferFuncNormal | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    private:
        amo::uuid m_uuid;
        
    };
}


#endif //LIBSEXT_UUIDTRANSFER_03786AD7_00D1_4B00_8C9C_582243FBDDD6_H__