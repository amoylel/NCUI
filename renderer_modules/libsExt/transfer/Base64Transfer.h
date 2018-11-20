// Created by amoylel on 12/01/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_BASE64TRANSFER_H__
#define AMO_BASE64TRANSFER_H__





#include <transfer/RunnableTransfer.hpp>
#include <amo/singleton.hpp>

namespace amo {

    /*!
     * @class	base64
     *
     * @chapter extend
     *
     * @extend Runnable
     *
     * @brief	Base64 编解码.<br>
     * 			工作线程**Renderer线程**
     *
     */
    
    class Base64Transfer
        : public RunnableTransfer
        , public amo::singleton<Base64Transfer> {
    public:
        Base64Transfer();
        
        virtual std::string getClass() const override;
        virtual Transfer* getInterface(const std::string& name) override;
        
        /*!
         * @fn	Any Base64Transfer::decode(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	Base64 解码.
         *
         * @param	#String 需要解码的字符串.
         *
         * @return	#String 解码后的字符串.
         * @example
         *
        	 ```
        		include('base64');
        		console.assert(base64.decode('TkNVSQ==') == 'NCUI');
        	 ```
         */
        
        Any decode(IPCMessage::SmartType msg);
        
        /*!
         * @fn	Any Base64Transfer::encode(IPCMessage::SmartType msg);
         *
         * @tag static sync
         *
         * @brief	Base64 编码.
         *
         * @param	#String 需要编码的字符串.
         *
         * @return	#String 编码后的字符串.
         * @example
         *
           ```
        		include('base64');
        		console.assert(base64.encode('NCUI') == 'TkNVSQ==');
            ```
         */
        
        Any encode(IPCMessage::SmartType msg);
        AMO_CEF_MESSAGE_TRANSFER_BEGIN(Base64Transfer, RunnableTransfer)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(decode, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_FUNC(encode, TransferFuncStatic | TransferExecSync)
        AMO_CEF_MESSAGE_TRANSFER_END()
        
    };
}


#endif // AMO_BASE64TRANSFER_H__
