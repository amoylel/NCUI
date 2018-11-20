// Created by amoylel on 05/15/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_SCRIPTRUNNER_HPP__
#define AMO_SCRIPTRUNNER_HPP__


#include <functional>
#include "ipc/Any.hpp"
#include "ipc/ProcessExchanger.hpp"
#include "ipc/IPCMessage.hpp"


namespace amo {

    /*!
     * @class	MessageLauncher
     *
     * @brief	A message launcher.
     */
    
    class MessageLauncher {
    public:
    
    
    public:
        MessageLauncher(int nPipeID = -9999, int64_t nFrameID = -1) {
            m_nPipeID = nPipeID;
            m_nFrameID = nFrameID;
            msg.reset(new amo::IPCMessage());
        }
        
        ~MessageLauncher() { }
        
    public:
    
        /*!
         * @fn	virtual bool MessageLauncher::Exchange(int nPipeID, IPCMessage::SmartType msg) = 0;
         *
         * @brief	Exchanges.
         *
         * @param	nPipeID	Identifier for the pipe.
         * @param	msg	   	The message.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool exchange(int nPipeID, IPCMessage::SmartType msg) = 0;
        
        /*!
         * @fn	virtual Any MessageLauncher::WaitResult(int nPipeID, int nMessageID) = 0;
         *
         * @brief	Wait result.
         *
         * @param	nPipeID   	Identifier for the pipe.
         * @param	nMessageID	Identifier for the message.
         *
         * @return	Any.
         */
        
        virtual Any waitResult(int nPipeID, int nMessageID) = 0;
        
        /*!
         * @fn	virtual bool MessageLauncher::SendMessage(IPCMessage::SmartType msg) = 0;
         *
         * @brief	Sends a message.
         *
         * @param	msg	The message.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool sendMessage(IPCMessage::SmartType msg) = 0;
        
        /*!
         * @fn	virtual void MessageLauncher::onMakeProcessMessage(IPCMessage::SmartType msg, void* args)
         *
         * @brief	Executes the make process message action.
         *
         * @param	msg				The message.
         * @param [in,out]	args	If non-null, the arguments.
         */
        
        virtual void onMakeProcessMessage(IPCMessage::SmartType msg, void* args) { }
        
        /*!
         * @fn	virtual void MessageLauncher::AfterMakeProcessMessage(IPCMessage::SmartType msg, const std::string& name)
         *
         * @brief	After make process message.
         *
         * @param	msg 	The message.
         * @param	name	The name.
         */
        
        virtual void afterMakeProcessMessage(IPCMessage::SmartType msg,
                                             const std::string& name) {
            msg->getArgumentList()->setValue(IPCArgsPosInfo::BrowserID, m_nPipeID);
            msg->getArgumentList()->setValue(IPCArgsPosInfo::FrameID, m_nFrameID);
            msg->getArgumentList()->setValue(IPCArgsPosInfo::FuncName, name);
            msg->getArgumentList()->setValue(IPCArgsPosInfo::MessageID, msg->getID());
        }
        
        /*!
         * @fn	template<typename ... Args> void MessageLauncher::createIPCMessage(std::string name, const std::string& execType, Args... args)
         *
         * @brief	Creates ipc message.
         *
         * @tparam	Args	Type of the arguments.
         * @param	name		The name.
         * @param	execType	Type of the execute.
         * @param	args		Variable arguments providing the arguments.
         */
        
        template<typename ... Args>
        void createIPCMessage(std::string name,
                              const std::string& execType,
                              Args... args) {
            msg->setMessageName(execType);
            makeProcessMessage(msg, args...);
            afterMakeProcessMessage(msg, name);
        }
        
        /*!
         * @fn	void MessageLauncher::createIPCMessage(std::string name, const std::string& execType, IPCMessage::SmartType ipcMessage)
         *
         * @brief	Creates ipc message.
         *
         * @param	name	  	The name.
         * @param	execType  	Type of the execute.
         * @param	ipcMessage	Message describing the ipc.
         */
        
        void createIPCMessage(std::string name,
                              const std::string& execType,
                              IPCMessage::SmartType ipcMessage) {
            msg = ipcMessage->clone();
            //*msg = IPCMessage::fromJson(ipcMessage->toJson());
            //*msg = *ipcMessage;
            msg->setMessageName(execType);
            msg->getArgumentList()->setValue(IPCArgsPosInfo::FuncName, name);
        }
        
        /*!
         * @fn	template<typename ... Args> void MessageLauncher::asyncExecute(std::string name, Args... args)
         *
         * @brief	Asynchronous execute.
         *
         * @tparam	Args	Type of the arguments.
         * @param	name	The name.
         * @param	args	Variable arguments providing the arguments.
         */
        
        template<typename ... Args>
        void asyncExecute(std::string name, Args... args) {
            createIPCMessage<Args ...>(name, MSG_NATIVE_ASYNC_EXECUTE, args...);
            sendMessage(msg);
        }
        
        /*!
         * @fn	void MessageLauncher::asyncExecute(IPCMessage::SmartType msg)
         *
         * @brief	Asynchronous execute.
         *
         * @param	msg	The message.
         */
        
        void asyncExecute(IPCMessage::SmartType msg) {
            sendMessage(msg);
        }
        
        /*!
         * @fn	void MessageLauncher::asyncExecute()
         *
         * @brief	Asynchronous execute.
         */
        
        void asyncExecute() {
            asyncExecute(msg);
        }
        
        /*!
         * @fn	template<typename ... Args> Any MessageLauncher::syncExecute(std::string name, Args... args)
         *
         * @brief	Synchronises the execute.
         *
         * @tparam	Args	Type of the arguments.
         * @param	name	The name.
         * @param	args	Variable arguments providing the arguments.
         *
         * @return	Any.
         */
        
        template<typename ... Args>
        Any syncExecute(std::string name, Args... args) {
        
            createIPCMessage<Args ...>(name, MSG_NATIVE_SYNC_EXECUTE, args...);
            
            bool  bOk =  exchange(m_nPipeID, msg);	// 往管道中写入数据
            
            if (!bOk) {
                return Undefined();
            }
            
            IPCMessage::SmartType notifyMsg(new
                                            amo::IPCMessage());	// 通知对方处理数据
            notifyMsg->setMessageName(MSG_PROCESS_SYNC_EXECUTE);
            sendMessage(notifyMsg);
            
            return waitResult(m_nPipeID, msg->getID());
        }
        
        /*!
         * @fn	Any MessageLauncher::syncExecute(IPCMessage::SmartType msg)
         *
         * @brief	Synchronises the execute described by msg.
         *
         * @param	msg	The message.
         *
         * @return	Any.
         */
        
        Any syncExecute(IPCMessage::SmartType msg) {
            bool bOk = exchange(m_nPipeID, msg);	// 往管道中写入数据
            
            if (!bOk) {
                return Undefined();
            }
            
            IPCMessage::SmartType notifyMsg(new
                                            amo::IPCMessage());	// 通知对方处理数据
            notifyMsg->setMessageName(MSG_PROCESS_SYNC_EXECUTE);
            notifyMsg->setID(msg->getID());
            notifyMsg->getArgumentList()->setValue(IPCArgsPosInfo::MessageID, msg->getID());
            sendMessage(notifyMsg);
            return waitResult(m_nPipeID, msg->getID());
        }
        
        /*!
         * @fn	Any MessageLauncher::syncExecute()
         *
         * @brief	Synchronises the execute.
         *
         * @return	Any.
         */
        
        Any syncExecute() {
            return syncExecute(msg);
        }
        
        /*!
         * @fn	template<typename ... Args> void MessageLauncher::Execute(std::string name, Args... args)
         *
         * @brief	Executes.
         *
         * @tparam	Args	Type of the arguments.
         * @param	name	The name.
         * @param	args	Variable arguments providing the arguments.
         */
        
        template<typename ... Args>
        void execute(std::string name, Args... args) {
            createIPCMessage<Args ...>(name, MSG_NATIVE_EXECUTE, args...);
            sendMessage(msg);
        }
        
        /*!
         * @fn	void MessageLauncher::Execute(IPCMessage::SmartType msg)
         *
         * @brief	Executes the given message.
         *
         * @param	msg	The message.
         */
        
        void execute(IPCMessage::SmartType msg) {
            sendMessage(msg);
        }
        
        /*!
         * @fn	void MessageLauncher::Execute()
         *
         * @brief	Executes this object.
         */
        
        void execute() {
            return execute(msg);
        }
        
        /*!
         * @fn	void MessageLauncher::MakeProcessMessage(IPCMessage::SmartType msg)
         *
         * @brief	Makes process message.
         *
         * @param	msg	The message.
         */
        
        void makeProcessMessage(IPCMessage::SmartType msg) {
            int nSize = getArgsSize(msg->getArgumentList());
            msg->getArgumentList()->setValue(IPCArgsPosInfo::ArgsLength, nSize);
        }
        
        /*!
         * @fn	template<typename T, typename ...Args> void MessageLauncher::MakeProcessMessage( IPCMessage::SmartType msg, const T& t, Args ... args)
         *
         * @brief	Makes process message.
         *
         * @tparam	T	   	Generic type parameter.
         * @tparam	...Args	Type of the ... arguments.
         * @param	msg 	The message.
         * @param	t   	The T to process.
         * @param	args	Variable arguments providing the arguments.
         */
        
        template<typename T, typename ...Args> void makeProcessMessage(
            IPCMessage::SmartType msg, const T& t, Args ... args) {
            /* typedef std::conditional<AnyValueType<T>::value
             == AnyValueType<amo::Unknown>::value, InvalidAny, ValidAny>::type Type;*/
            makeProcessMessageImpl(msg, typename GetAnyType<T>::Type(), t, args...);
        }
        
        /*!
         * @fn	template<typename T, typename ...Args> void MessageLauncher::MakeProcessMessageImpl( IPCMessage::SmartType msg, ValidAny any, const T& t, Args ... args)
         *
         * @brief	Makes process message implementation.
         *
         * @tparam	T	   	Generic type parameter.
         * @tparam	...Args	Type of the ... arguments.
         * @param	msg 	The message.
         * @param	any 	any.
         * @param	t   	The T to process.
         * @param	args	Variable arguments providing the arguments.
         */
        
        template<typename T, typename ...Args> void makeProcessMessageImpl(
            IPCMessage::SmartType msg, ValidAny any,  const T& t, Args ... args) {
            int nIndex = getArgsSize(msg->getArgumentList());
            msg->getArgumentList()->setValue(nIndex, t);
            return makeProcessMessage(msg, args...);
        }
        
        /*!
         * @fn	template<typename T, typename ...Args> void MessageLauncher::MakeProcessMessageImpl( IPCMessage::SmartType msg, InvalidAny any, const T& t, Args ... args)
         *
         * @brief	Makes process message implementation.
         *
         * @tparam	T	   	Generic type parameter.
         * @tparam	...Args	Type of the ... arguments.
         * @param	msg 	The message.
         * @param	any 	any.
         * @param	t   	The T to process.
         * @param	args	Variable arguments providing the arguments.
         */
        
        template<typename T, typename ...Args> void makeProcessMessageImpl(
            IPCMessage::SmartType msg, InvalidAny any, const T& t, Args ... args) {
            onMakeProcessMessage(msg, (void*)&t);
        }
        
        /*!
         * @fn	int MessageLauncher::getArgsSize(std::shared_ptr<AnyArgsList> args) const
         *
         * @brief	Gets arguments size.
         *
         * @param	args	The arguments.
         *
         * @return	The arguments size.
         */
        
        int getArgsSize(std::shared_ptr<AnyArgsList> args) const {
            size_t nSize = 0;
            
            for (auto& p : args->getAllValues()) {
                if (p.first < IPCArgsPosInfo::FuncName) {
                    ++nSize;
                }
            }
            
            return nSize;
        }
        
        /*!
         * @fn	void MessageLauncher::setValue(const int& nIndex, const Any& val)
         *
         * @brief	Sets a value.
         *
         * @param	nIndex	The index.
         * @param	val   	The value.
         */
        
        void setValue(const int& nIndex, const Any& val) {
            msg->getArgumentList()->setValue(nIndex, val);
        }
        
        /*!
         * @fn	void MessageLauncher::ClearValue(const int& nIndex)
         *
         * @brief	Clears the value described by nIndex.
         *
         * @param	nIndex	The index.
         */
        
        void clearValue(const int& nIndex) {
            msg->getArgumentList()->clearValue(nIndex);
        }
        
        /*!
         * @fn	void MessageLauncher::ClearValue(const int& nBegin, const int& nEnd)
         *
         * @brief	Clears the value.
         *
         * @param	nBegin	The begin.
         * @param	nEnd  	The end.
         */
        
        void clearValue(const int& nBegin, const int& nEnd) {
            msg->getArgumentList()->clearValue(nBegin, nEnd);
        }
        
        /*!
         * @fn	void MessageLauncher::setIPCMessage(std::shared_ptr<IPCMessage> ipcMessage)
         *
         * @brief	Sets ipc message.
         *
         * @param	ipcMessage	Message describing the ipc.
         */
        
        void setIPCMessage(std::shared_ptr<IPCMessage> ipcMessage) {
            msg = ipcMessage;
        }
        
        IPCMessage::SmartType& getIPCMessage() {
            return msg;
        }
        
    protected:
    
        /*! @brief	Identifier for the pipe. */
        int m_nPipeID;
        
        /*! @brief	Identifier for the frame. */
        int64_t m_nFrameID;
        
        /*! @brief	The message. */
        IPCMessage::SmartType msg;
    };
}


#endif // AMO_SCRIPTRUNNER_HPP__