// Created by amoylel on 13/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef LIBNCUI_IPCMESSAGEEMITTER_4F56D4BA_5285_441E_AA3B_D0D84CA2D726_HPP__
#define LIBNCUI_IPCMESSAGEEMITTER_4F56D4BA_5285_441E_AA3B_D0D84CA2D726_HPP__

#include "ipc/MessageLauncher.hpp"
#include <functional>
#include <memory>

namespace amo {

    /*!
    * @class	ProcessMessageEmitter
    *
    * @brief	A message emitter.
    */
    
    class IPCEmitter : public MessageLauncher {
    public:
        typedef std::function<bool(IPCMessage::SmartType)> ProcessSender;
        typedef  std::shared_ptr<amo::ProcessExchangerManager<amo::ProcessExchanger> >
        SmartProcessExchangerMgr;
        
    public:
    
        IPCEmitter(int32_t nPipeID = -9999, int64_t nFrameID = -1)
            : MessageLauncher(nPipeID, nFrameID) {
            
        }
        
        
        /*!
        * @fn	static std::function<bool(IPCMessage::SmartType)>& getNodeMessageSender()
        *
        * @brief	Gets node message sender.
        *
        * @return	The node message sender.
        */
        
        std::function<bool(IPCMessage::SmartType)>& getProcessMessageSender() {
        
            return m_fnSendMessage;
        }
        
        void setProcessMessageSender(std::function<bool(IPCMessage::SmartType)> fn) {
            m_fnSendMessage = fn;
        }
        
        /*!
        * @fn	virtual bool ProcessMessageEmitter::Exchange(int nPipeID, IPCMessage::SmartType msg) override
        *
        * @brief	Exchanges.
        *
        * @param	nPipeID	Identifier for the pipe.
        * @param	msg	   	The message.
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        virtual bool exchange(int nPipeID, IPCMessage::SmartType msg) override {
        
            return getProcessExchangeManager()->exchange(nPipeID, msg);
        }
        
        /*!
        * @fn	virtual Any ProcessMessageEmitter::waitResult(int nPipeID, int nMessageID)
        *
        * @brief	Wait result.
        *
        * @param	nPipeID   	Identifier for the pipe.
        * @param	nMessageID	Identifier for the message.
        *
        * @return	Any.
        */
        
        virtual Any waitResult(int nPipeID, int nMessageID) {
            return getProcessExchangeManager()->waitResult<Any>(nPipeID, nMessageID);
        }
        
        /*!
        * @fn	template<typename ... Args> Any ProcessMessageEmitter::Transfer(std::string name, Args... args)
        *
        * @brief	Transfers.
        *
        * @tparam	Args	Type of the arguments.
        * @param	name	The name.
        * @param	args	Variable arguments providing the arguments.
        *
        * @return	Any.
        */
        
        template<typename ... Args>
        Any transfer(std::string name, Args... args) {
            msg->setMessageName(MSG_NATIVE_ASYNC_EXECUTE);
            makeProcessMessage(msg, args...);
            afterMakeProcessMessage(msg, name);
            int nBrowserID = m_pFrame->GetBrowser()->GetIdentifier();
            return BrowserTransferMgr::getInstance()->onMessageTransfer(msg);
        }
        
        /*!
        * @fn	virtual bool ProcessMessageEmitter::SendMessage(IPCMessage::SmartType msg) override
        *
        * @brief	Sends a message.
        *
        * @param	msg	The message.
        *
        * @return	true if it succeeds, false if it fails.
        */
        
        virtual bool sendMessage(IPCMessage::SmartType msg) override {
        
        
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
    
        std::function<bool(IPCMessage::SmartType)> m_fnSendMessage;
        SmartProcessExchangerMgr  m_pProcessExchangeManager;
        
    };
    
    
}





#endif //LIBNCUI_IPCMESSAGEEMITTER_4F56D4BA_5285_441E_AA3B_D0D84CA2D726_HPP__

