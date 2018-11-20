// Created by amoylel on 12/04/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_IPC_EMITTER_3A29699B_4EEA_486A_B621_FEE7FCABDBF9_HPP__
#define AMO_IPC_EMITTER_3A29699B_4EEA_486A_B621_FEE7FCABDBF9_HPP__





#include <amo/ipc/message_launcher.hpp>
#include <functional>
#include <memory>

namespace amo {

    /*!
    * @class	ProcessMessageEmitter
    *
    * @brief	A message emitter.
    */
    
    class ipc_emitter : public message_launcher {
    public:
        typedef std::function<bool(ipc_msg::SmartType)> ProcessSender;
        typedef  std::shared_ptr<amo::process_exchanger_manager<amo::process_exchanger> >
        SmartProcessExchangerMgr;
        
    public:
    
        ipc_emitter(int32_t nPipeID = -9999, int64_t nFrameID = -1)
            : message_launcher(nPipeID, nFrameID) {
            
        }
        
        
        /*!
        * @fn	static std::function<bool(ipc_msg::SmartType)>& getNodeMessageSender()
        *
        * @brief	Gets node message sender.
        *
        * @return	The node message sender.
        */
        
        std::function<bool(ipc_msg::SmartType)>& getProcessMessageSender() {
        
            return m_fnSendMessage;
        }
        
        void setProcessMessageSender(std::function<bool(ipc_msg::SmartType)> fn) {
            m_fnSendMessage = fn;
        }
        
        /*!
        * @fn	virtual bool ProcessMessageEmitter::Exchange(int nPipeID, ipc_msg::SmartType msg) override
        *
        * @brief	Exchanges.
        *
        * @param	nPipeID	Identifier for the pipe.
        * @param	msg	   	The message.
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        virtual bool exchange(int nPipeID, ipc_msg::SmartType msg) override {
        
            return getProcessExchangeManager()->exchange(nPipeID, msg);
        }
        
        /*!
        * @fn	virtual any ProcessMessageEmitter::waitResult(int nPipeID, int nMessageID)
        *
        * @brief	Wait result.
        *
        * @param	nPipeID   	Identifier for the pipe.
        * @param	nMessageID	Identifier for the message.
        *
        * @return	any.
        */
        
        virtual any waitResult(int nPipeID, int nMessageID) {
            return getProcessExchangeManager()->waitResult<any>(nPipeID, nMessageID);
        }
        
        /*!
        * @fn	template<typename ... Args> any ProcessMessageEmitter::transfer(std::string name, Args... args)
        *
        * @brief	Transfers.
        *
        * @tparam	Args	Type of the arguments.
        * @param	name	The name.
        * @param	args	Variable arguments providing the arguments.
        *
        * @return	any.
        */
        
        template<typename ... Args>
        any transfer(std::string name, Args... args) {
            msg->setMessageName(IPC_NATIVE_ASYNC_EXECUTE);
            makeProcessMessage(msg, args...);
            afterMakeProcessMessage(msg, name);
            int nBrowserID = m_pFrame->GetBrowser()->GetIdentifier();
            return BrowserTransferMgr::getInstance()->onMessageTransfer(msg);
        }
        
        /*!
        * @fn	virtual bool ProcessMessageEmitter::SendMessage(ipc_msg::SmartType msg) override
        *
        * @brief	Sends a message.
        *
        * @param	msg	The message.
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        virtual bool sendMessage(ipc_msg::SmartType msg) override {
        
        
            ProcessSender& fn = getProcessMessageSender();
            
            if (fn) {
                return fn(msg);
            }
            
            return false;
            
        }
        SmartProcessExchangerMgr  getProcessExchangeManager() const {
            return m_pProcessExchangeManager;
        }
        void setProcessExchangeManager(SmartProcessExchangerMgr val) {
            m_pProcessExchangeManager = val;
        }
        
        
    protected:
    
        std::function<bool(ipc_msg::SmartType)> m_fnSendMessage;
        SmartProcessExchangerMgr  m_pProcessExchangeManager;
        
    };
    
    
}


#endif //AMO_IPC_EMITTER_3A29699B_4EEA_486A_B621_FEE7FCABDBF9_HPP__

