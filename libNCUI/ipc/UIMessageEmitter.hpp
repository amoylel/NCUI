// Created by amoylel on 06/30/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_UIMESSAGEEMITTER_HPP__
#define AMO_UIMESSAGEEMITTER_HPP__


#include <assert.h>
#include "handler/CefHeader.hpp"
#include "ipc/MessageLauncher.hpp"
#include <functional>

namespace amo {

    /*!
     * @class	UIMessageEmitter
     *
     * @brief	A message emitter.
     */
    
    class UIMessageEmitter : public MessageLauncher {
    public:
        UIMessageEmitter(CefRefPtr<CefFrame> frame)
            : m_pFrame(frame) {
            // frame 可以不存在，那么全向NodeJS发送消息
            if (!m_pFrame) {
                return;
            }
            
            m_nPipeID = m_pFrame->GetBrowser()->GetIdentifier();
            m_nFrameID = m_pFrame->GetIdentifier();
        }
        
        UIMessageEmitter(CefRefPtr<CefBrowser> pBrowser, CefRefPtr<CefFrame> frame)
            : m_pFrame(frame) {
            if (!m_pFrame || !pBrowser) {
                return;
            }
            
            m_nPipeID = pBrowser->GetIdentifier();
            m_nFrameID = frame->GetIdentifier();
        }
        
        UIMessageEmitter() {
        
        }
        
        /*!
         * @fn	static std::function<bool(IPCMessage::SmartType)>& getNodeMessageSender()
         *
         * @brief	Gets node message sender.
         *
         * @return	The node message sender.
         */
        
        static std::function<bool(IPCMessage::SmartType)>& getNodeMessageSender() {
            static std::function<bool(IPCMessage::SmartType)> m_fnSendMessage;
            return m_fnSendMessage;
        }
        
        /*!
         * @fn	virtual bool UIMessageEmitter::Exchange(int nPipeID, IPCMessage::SmartType msg) override
         *
         * @brief	Exchanges.
         *
         * @param	nPipeID	Identifier for the pipe.
         * @param	msg	   	The message.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool exchange(int nPipeID, IPCMessage::SmartType msg) override {
            auto manager = BrowserProcessExchangerManager::getInstance();
            return manager->exchange(nPipeID, msg);
        }
        
        /*!
         * @fn	virtual Any UIMessageEmitter::waitResult(int nPipeID, int nMessageID)
         *
         * @brief	Wait result.
         *
         * @param	nPipeID   	Identifier for the pipe.
         * @param	nMessageID	Identifier for the message.
         *
         * @return	Any.
         */
        
        virtual Any waitResult(int nPipeID, int nMessageID) {
            auto manager = BrowserProcessExchangerManager::getInstance();
            return manager->waitResult<Any>(nPipeID, nMessageID);
        }
        
        /*!
         * @fn	template<typename ... Args> Any UIMessageEmitter::Transfer(std::string name, Args... args)
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
         * @fn	virtual bool UIMessageEmitter::SendMessage(IPCMessage::SmartType msg) override
         *
         * @brief	Sends a message.
         *
         * @param	msg	The message.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool sendMessage(IPCMessage::SmartType msg) override {
            if (!m_pFrame) {
                typedef std::function<bool(IPCMessage::SmartType)> NodeSender;
                NodeSender& fn = getNodeMessageSender();
                
                if (fn) {
                    return fn(msg);
                }
                
                return false;
            }
            
            CefRefPtr<CefProcessMessage> ipcMessage
                = CefProcessMessage::Create(msg->getName());
                
            ipcMessage->GetArgumentList()->SetString(0,
                    msg->toJson().to_string());
                    
            return m_pFrame->GetBrowser()->SendProcessMessage(PID_RENDERER,
                    ipcMessage);
        }
        
        /*!
         * @fn	CefRefPtr<CefFrame> UIMessageEmitter::getFrame() const
         *
         * @brief	Gets the frame.
         *
         * @return	The frame.
         */
        
        CefRefPtr<CefFrame> getFrame() const {
            return m_pFrame;
        }
        
        /*!
         * @fn	void UIMessageEmitter::setFrame(CefRefPtr<CefFrame> val)
         *
         * @brief	Sets a frame.
         *
         * @param	val	The value.
         */
        
        void setFrame(CefRefPtr<CefFrame> val) {
            m_pFrame = val;
            
            if (!m_pFrame) {
                m_nFrameID = -1;
                m_nPipeID = -9999;
            } else {
                m_nFrameID = m_pFrame->GetIdentifier();
                m_nPipeID = m_pFrame->GetBrowser()->GetIdentifier();
            }
            
            std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
            args->setValue(IPCArgsPosInfo::BrowserID, m_nPipeID);
            args->setValue(IPCArgsPosInfo::FrameID, m_nFrameID);
        }
    protected:
        /*! @brief	页面Frame指针. */
        CefRefPtr<CefFrame> m_pFrame;
        
    };
    
    
}


#endif // AMO_UIMESSAGEEMITTER_HPP__

