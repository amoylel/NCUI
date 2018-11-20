// Created by amoylel on 14/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_MESSAGE_LAUNCHER_D04CFECA_E60E_445E_BBC1_64A8CCD26812_HPP__
#define AMO_MESSAGE_LAUNCHER_D04CFECA_E60E_445E_BBC1_64A8CCD26812_HPP__







#include <functional>
#include <amo/ipc/any.hpp>
#include <amo/ipc/process_exchanger.hpp>


namespace amo {

    /*!
     * @class	message_launcher
     *
     * @brief	A message launcher.
     */
    
    class message_launcher {
    public:
    
    
    public:
        message_launcher(int nPipeID = -9999, int64_t nFrameID = -1) {
            m_nPipeID = nPipeID;
            m_nFrameID = nFrameID;
            msg.reset(new amo::ipc_msg());
        }
        
        ~message_launcher() { }
        
    public:
    
        /*!
         * @fn	virtual bool message_launcher::Exchange(int nPipeID, ipc_msg::SmartType msg) = 0;
         *
         * @brief	Exchanges.
         *
         * @param	nPipeID	Identifier for the pipe.
         * @param	msg	   	The message.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool exchange(int nPipeID, ipc_msg::SmartType msg) = 0;
        
        /*!
         * @fn	virtual any message_launcher::WaitResult(int nPipeID, int nMessageID) = 0;
         *
         * @brief	Wait result.
         *
         * @param	nPipeID   	Identifier for the pipe.
         * @param	nMessageID	Identifier for the message.
         *
         * @return	any.
         */
        
        virtual any waitResult(int nPipeID, int nMessageID) = 0;
        
        /*!
         * @fn	virtual bool message_launcher::SendMessage(ipc_msg::SmartType msg) = 0;
         *
         * @brief	Sends a message.
         *
         * @param	msg	The message.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool sendMessage(ipc_msg::SmartType msg) = 0;
        
        /*!
         * @fn	virtual void message_launcher::onMakeProcessMessage(ipc_msg::SmartType msg, void* args)
         *
         * @brief	Executes the make process message action.
         *
         * @param	msg				The message.
         * @param [in,out]	args	If non-null, the arguments.
         */
        
        virtual void onMakeProcessMessage(ipc_msg::SmartType msg, void* args) { }
        
        /*!
         * @fn	virtual void message_launcher::AfterMakeProcessMessage(ipc_msg::SmartType msg, const std::string& name)
         *
         * @brief	After make process message.
         *
         * @param	msg 	The message.
         * @param	name	The name.
         */
        
        virtual void afterMakeProcessMessage(ipc_msg::SmartType msg,
                                             const std::string& name) {
            msg->getArgumentList()->setValue(ipc_args_pos_info::BrowserID, m_nPipeID);
            msg->getArgumentList()->setValue(ipc_args_pos_info::FrameID, m_nFrameID);
            msg->getArgumentList()->setValue(ipc_args_pos_info::FuncName, name);
            msg->getArgumentList()->setValue(ipc_args_pos_info::MessageID, msg->getID());
        }
        
        /*!
         * @fn	template<typename ... Args> void message_launcher::createipc_msg(std::string name, const std::string& execType, Args... args)
         *
         * @brief	Creates ipc message.
         *
         * @tparam	Args	Type of the arguments.
         * @param	name		The name.
         * @param	execType	Type of the execute.
         * @param	args		Variable arguments providing the arguments.
         */
        
        template<typename ... Args>
        void createipc_msg(std::string name,
                           const std::string& execType,
                           Args... args) {
            msg->setMessageName(execType);
            makeProcessMessage(msg, args...);
            afterMakeProcessMessage(msg, name);
        }
        
        /*!
         * @fn	void message_launcher::createipc_msg(std::string name, const std::string& execType, ipc_msg::SmartType ipcMessage)
         *
         * @brief	Creates ipc message.
         *
         * @param	name	  	The name.
         * @param	execType  	Type of the execute.
         * @param	ipcMessage	Message describing the ipc.
         */
        
        void createipc_msg(std::string name,
                           const std::string& execType,
                           ipc_msg::SmartType ipcMessage) {
            msg = ipcMessage->clone();
            //*msg = ipc_msg::fromJson(ipcMessage->toJson());
            //*msg = *ipcMessage;
            msg->setMessageName(execType);
            msg->getArgumentList()->setValue(ipc_args_pos_info::FuncName, name);
        }
        
        /*!
         * @fn	template<typename ... Args> void message_launcher::asyncExecute(std::string name, Args... args)
         *
         * @brief	Asynchronous execute.
         *
         * @tparam	Args	Type of the arguments.
         * @param	name	The name.
         * @param	args	Variable arguments providing the arguments.
         */
        
        template<typename ... Args>
        void asyncExecute(std::string name, Args... args) {
            createipc_msg<Args ...>(name, IPC_NATIVE_ASYNC_EXECUTE, args...);
            sendMessage(msg);
        }
        
        /*!
         * @fn	void message_launcher::asyncExecute(ipc_msg::SmartType msg)
         *
         * @brief	Asynchronous execute.
         *
         * @param	msg	The message.
         */
        
        void asyncExecute(ipc_msg::SmartType msg) {
            sendMessage(msg);
        }
        
        /*!
         * @fn	void message_launcher::asyncExecute()
         *
         * @brief	Asynchronous execute.
         */
        
        void asyncExecute() {
            asyncExecute(msg);
        }
        
        /*!
         * @fn	template<typename ... Args> any message_launcher::syncExecute(std::string name, Args... args)
         *
         * @brief	Synchronises the execute.
         *
         * @tparam	Args	Type of the arguments.
         * @param	name	The name.
         * @param	args	Variable arguments providing the arguments.
         *
         * @return	any.
         */
        
        template<typename ... Args>
        any syncExecute(std::string name, Args... args) {
        
            createipc_msg<Args ...>(name, IPC_NATIVE_SYNC_EXECUTE, args...);
            
            bool  bOk =  exchange(m_nPipeID, msg);	// 往管道中写入数据
            
            if (!bOk) {
                return undefined();
            }
            
            ipc_msg::SmartType notifyMsg(new
                                         amo::ipc_msg());	// 通知对方处理数据
            notifyMsg->setMessageName(IPC_PROCESS_SYNC_EXECUTE);
            sendMessage(notifyMsg);
            
            return waitResult(m_nPipeID, msg->getID());
        }
        
        /*!
         * @fn	any message_launcher::syncExecute(ipc_msg::SmartType msg)
         *
         * @brief	Synchronises the execute described by msg.
         *
         * @param	msg	The message.
         *
         * @return	any.
         */
        
        any syncExecute(ipc_msg::SmartType msg) {
            bool bOk = exchange(m_nPipeID, msg);	// 往管道中写入数据
            
            if (!bOk) {
                return undefined();
            }
            
            ipc_msg::SmartType notifyMsg(new
                                         amo::ipc_msg());	// 通知对方处理数据
            notifyMsg->setMessageName(IPC_PROCESS_SYNC_EXECUTE);
            notifyMsg->setID(msg->getID());
            notifyMsg->getArgumentList()->setValue(ipc_args_pos_info::MessageID,
                                                   msg->getID());
            sendMessage(notifyMsg);
            return waitResult(m_nPipeID, msg->getID());
        }
        
        /*!
         * @fn	any message_launcher::syncExecute()
         *
         * @brief	Synchronises the execute.
         *
         * @return	any.
         */
        
        any syncExecute() {
            return syncExecute(msg);
        }
        
        /*!
         * @fn	template<typename ... Args> void message_launcher::Execute(std::string name, Args... args)
         *
         * @brief	Executes.
         *
         * @tparam	Args	Type of the arguments.
         * @param	name	The name.
         * @param	args	Variable arguments providing the arguments.
         */
        
        template<typename ... Args>
        void execute(std::string name, Args... args) {
            createipc_msg<Args ...>(name, IPC_NATIVE_EXECUTE, args...);
            sendMessage(msg);
        }
        
        /*!
         * @fn	void message_launcher::Execute(ipc_msg::SmartType msg)
         *
         * @brief	Executes the given message.
         *
         * @param	msg	The message.
         */
        
        void execute(ipc_msg::SmartType msg) {
            sendMessage(msg);
        }
        
        /*!
         * @fn	void message_launcher::Execute()
         *
         * @brief	Executes this object.
         */
        
        void execute() {
            return execute(msg);
        }
        
        /*!
         * @fn	void message_launcher::MakeProcessMessage(ipc_msg::SmartType msg)
         *
         * @brief	Makes process message.
         *
         * @param	msg	The message.
         */
        
        void makeProcessMessage(ipc_msg::SmartType msg) {
            int nSize = getArgsSize(msg->getArgumentList());
            msg->getArgumentList()->setValue(ipc_args_pos_info::ArgsLength, nSize);
        }
        
        /*!
         * @fn	template<typename T, typename ...Args> void message_launcher::MakeProcessMessage( ipc_msg::SmartType msg, const T& t, Args ... args)
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
            ipc_msg::SmartType msg, const T& t, Args ... args) {
            /* typedef std::conditional<any_value_type<T>::value
             == any_value_type<amo::unknown>::value, invalid_any, valid_any>::type Type;*/
            makeProcessMessageImpl(msg, typename get_any_type<T>::Type(), t, args...);
        }
        
        /*!
         * @fn	template<typename T, typename ...Args> void message_launcher::MakeProcessMessageImpl( ipc_msg::SmartType msg, valid_any any, const T& t, Args ... args)
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
            ipc_msg::SmartType msg, valid_any any,  const T& t, Args ... args) {
            int nIndex = getArgsSize(msg->getArgumentList());
            msg->getArgumentList()->setValue(nIndex, t);
            return makeProcessMessage(msg, args...);
        }
        
        /*!
         * @fn	template<typename T, typename ...Args> void message_launcher::MakeProcessMessageImpl( ipc_msg::SmartType msg, invalid_any any, const T& t, Args ... args)
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
            ipc_msg::SmartType msg, invalid_any any, const T& t, Args ... args) {
            onMakeProcessMessage(msg, (void*)&t);
        }
        
        /*!
         * @fn	int message_launcher::getArgsSize(std::shared_ptr<any_args_list> args) const
         *
         * @brief	Gets arguments size.
         *
         * @param	args	The arguments.
         *
         * @return	The arguments size.
         */
        
        int getArgsSize(std::shared_ptr<any_args_list> args) const {
            size_t nSize = 0;
            
            for (auto& p : args->getAllValues()) {
                if (p.first < ipc_args_pos_info::FuncName) {
                    ++nSize;
                }
            }
            
            return nSize;
        }
        
        /*!
         * @fn	void message_launcher::setValue(const int& nIndex, const any& val)
         *
         * @brief	Sets a value.
         *
         * @param	nIndex	The index.
         * @param	val   	The value.
         */
        
        void setValue(const int& nIndex, const any& val) {
            msg->getArgumentList()->setValue(nIndex, val);
        }
        
        /*!
         * @fn	void message_launcher::ClearValue(const int& nIndex)
         *
         * @brief	Clears the value described by nIndex.
         *
         * @param	nIndex	The index.
         */
        
        void clearValue(const int& nIndex) {
            msg->getArgumentList()->clearValue(nIndex);
        }
        
        /*!
         * @fn	void message_launcher::ClearValue(const int& nBegin, const int& nEnd)
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
         * @fn	void message_launcher::setipc_msg(std::shared_ptr<ipc_msg> ipcMessage)
         *
         * @brief	Sets ipc message.
         *
         * @param	ipcMessage	Message describing the ipc.
         */
        
        void setipc_msg(std::shared_ptr<ipc_msg> ipcMessage) {
            msg = ipcMessage;
        }
        
        ipc_msg::SmartType& getipc_msg() {
            return msg;
        }
        
    protected:
    
        /*! @brief	Identifier for the pipe. */
        int m_nPipeID;
        
        /*! @brief	Identifier for the frame. */
        int64_t m_nFrameID;
        
        /*! @brief	The message. */
        ipc_msg::SmartType msg;
    };
}



#endif //AMO_MESSAGE_LAUNCHER_D04CFECA_E60E_445E_BBC1_64A8CCD26812_HPP__