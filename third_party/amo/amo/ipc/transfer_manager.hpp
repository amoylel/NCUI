// Created by amoylel on 14/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_TRANSFERMGR_23AF15AB_A44A_4BB7_B9F7_29367A237738_HPP__
#define AMO_TRANSFERMGR_23AF15AB_A44A_4BB7_B9F7_29367A237738_HPP__



#include <memory>
#include <unordered_map>
#include <exception>
#include <amo/singleton.hpp>
#include <amo/json.hpp>

#include <amo/ipc/any.hpp>
#include <amo/ipc/transfer.hpp>



namespace amo {

    class transfer;
    
    /*!
     * @class	TransferMap
     *
     * @brief	保存一个浏览器的所有transfer类（这里只保存类，不保存类的实例）.
     */
    class transfer_map  {
    public:
        typedef  std::shared_ptr<std::function<any(ipc_msg::SmartType)> >
        smart_event_fn_type;
    public:
    
        /*!
         * @fn	void TransferMap::addTransfer(transfer* pTransfer)
         *
         * @brief	添加transfer.
         *
         * @param 	pTransfer	If non-null, the transfer.
         */
        void addTransfer(transfer* pTransfer) {
            pTransfer->setDefaultTriggerEventFunc(getDefaultTriggerEventFunc());
            m_oTransferMap[pTransfer->transferName()] = pTransfer;
        }
        
        /*!
         * @fn	void TransferMap::addTransfer(std::shared_ptr<transfer> pTransfer)
         *
         * @brief	添加transfer.
         *
         * @param	pTransfer	The transfer.
         */
        void addTransfer(std::shared_ptr<transfer> pTransfer) {
            pTransfer->setDefaultTriggerEventFunc(getDefaultTriggerEventFunc());
            m_oSmartMap.insert(std::make_pair(pTransfer->transferName(),
                                              pTransfer));
            m_oTransferMap[pTransfer->transferName()] = pTransfer.get();
        }
        
        /*!
         * @fn	void TransferMap::removeTransfer(const std::string& strName)
         *
         * @brief	通过名称移除transfer.
         *
         * @param	strName	transfer名.
         */
        void removeTransfer(const std::string& strName) {
            m_oTransferMap.erase(strName);
            m_oSmartMap.erase(strName);
        }
        
        void removeTransfer() {
            m_oTransferMap.clear();
            m_oSmartMap.clear();
        }
        
        
        /*!
         * @fn	transfer* TransferMap::FinTransfer(const std::string& strName)
         *
         * @brief	通过名称查找transfer.
         *
         * @param	strName	transfer名.
         *
         * @return	null if it fails, else a pointer to a transfer.
         */
        transfer* findTransfer(const std::string& strName) {
            auto iter = m_oTransferMap.find(strName);
            
            if (iter == m_oTransferMap.end()) {
                return NULL;
            }
            
            return iter->second;
        }
        
        any onDefaultMessageTransfer(ipc_msg::SmartType msg) {
            if (m_fnDefaultMsgFunc && *m_fnDefaultMsgFunc) {
                return (*m_fnDefaultMsgFunc)(msg);
            }
            
            return nothing();
        }
        /*!
         * @fn	any TransferMap::onMessageTransfer(ipc_msg::SmartType msg)
         *
         * @brief	遍历transfer并找到对应的函数执行.
         *
         * @param	msg	The message.
         *
         * @return	any.
         */
        any onMessageTransfer(ipc_msg::SmartType msg) {
            std::shared_ptr<any_args_list> args = msg->getArgumentList();
            int nBrowserID = args->getInt(ipc_args_pos_info::BrowserID);
            std::string messageName = msg->getName();
            
            // 只处理这三种消息
            if (messageName == IPC_NATIVE_EXECUTE
                    || messageName == IPC_NATIVE_SYNC_EXECUTE
                    || messageName == IPC_NATIVE_ASYNC_EXECUTE) {
                std::string funcName;
                funcName = args->getString(ipc_args_pos_info::TransferName);
                
                for (auto& p : m_oTransferMap) {
                    if (p.second->transferName() == funcName) {
                        any ret = p.second->onMessageTransfer(msg);
                        
                        if (ret.isValid()) {
                            // 如果返回值有效，直接返回结果
                            return ret;
                        }
                        
                        // 返回值无效，继续遍历（实际上应该直接返回）
                    }
                }
                
                return onDefaultMessageTransfer(msg);
            }
            
            // 返回无效结果
            return amo::nothing();
        }
        
        /*!
         * @fn	amo::json TransferMap::toJson()
         *
         * @brief	将对象转换为JSON（数组）.
         *
         * @return	This object as an amo::json.
         */
        amo::json toJson() {
            amo::json jsonArr;
            jsonArr.set_array();
            
            for (auto& transfer : m_oTransferMap) {
                auto& p = transfer.second;
                jsonArr.push_back(p->getFuncMgr().toJson());
            }
            
            return jsonArr;
        }
        /*!
         * @fn	std::unordered_map<std::string, transfer*>& TransferMap::transferMap()
         *
         * @brief	获取原始transfer map.
         *
         * @return	null if it fails, else a reference to a std::unordered_map&lt;std::string,
         * 			transfer*&gt;
         */
        std::unordered_map<std::string, transfer*>& transferMap() {
            return m_oTransferMap;
        }
        smart_event_fn_type getDefaultMsgFunc() const {
            return m_fnDefaultMsgFunc;
        }
        void setDefaultMsgFunc(smart_event_fn_type val) {
            m_fnDefaultMsgFunc = val;
        }
        void setDefaultTriggerEventFunc(
            std::function<void(ipc_msg::SmartType) >
            val) {
            m_fnDefaultTriggerEventFunc = val;
        }
        
        
        
        std::function<void(ipc_msg::SmartType) > getDefaultTriggerEventFunc() const {
            return m_fnDefaultTriggerEventFunc;
        }
        
    private:
        /*! @brief	保存通过普通指针添加的transfer. */
        std::unordered_map<std::string, transfer*> m_oTransferMap;
        /*! @brief	保存通过智能指针添加的transfer. */
        std::unordered_map<std::string, std::shared_ptr<transfer> > m_oSmartMap;
        
        /** @brief	默认消息处理函数，如果没有找到消息处理函数将触发该函数. */
        smart_event_fn_type m_fnDefaultMsgFunc;
        
        std::function<void(ipc_msg::SmartType)> m_fnDefaultTriggerEventFunc;
    };
    /*!
     * @class	TransferMgr
     *
     * @brief	管理所有的transfer.
     */
    class transfer_manager {
    public:
        typedef  std::shared_ptr<std::function<any(ipc_msg::SmartType)> >
        smart_event_fn_type;
    public:
    
        /*!
         * @fn	void TransferMgr::addTransfer(int nBrowserID,
         * 		transfer* pTransfer)
         *
         * @brief	添加一个transfer到指定的TransferMap.
         *
         * @param	nBrowserID		 	Identifier for the browser.
         * @param 	pTransfer	If non-null, the transfer.
         */
        void addTransfer(int nBrowserID, transfer* pTransfer) {
            getTransferMap(nBrowserID).addTransfer(pTransfer);
        }
        
        /*!
         * @fn	void TransferMgr::addTransfer(int nBrowserID,
         * 		 std::shared_ptr<transfer> pTransfer)
         *
         * @brief	添加一个transfer到指定的TransferMap.
         *
         * @param	nBrowserID	Identifier for the browser.
         * @param	pTransfer 	The transfer.
         */
        void addTransfer(int nBrowserID, std::shared_ptr<transfer> pTransfer) {
        
            getTransferMap(nBrowserID).addTransfer(pTransfer);
        }
        
        /*!
         * @fn	void TransferMgr::removeTransfer(int nBrowserID,
         * 		const std::string& strName)
         *
         * @brief	Removes the transfer.
         *
         * @param	nBrowserID	Identifier for the browser.
         * @param	strName   	The name.
         */
        void removeTransfer(int nBrowserID, const std::string& strName) {
            getTransferMap(nBrowserID).removeTransfer(strName);
        }
        
        void removeTransfer(int nBrowserID) {
            getTransferMap(nBrowserID).removeTransfer();
            m_oTransferMap.erase(nBrowserID);
        }
        
        /**
         * @fn	transfer* TransferMgr::findTransfer(int nBrowserID, const std::string& strName)
         *
         * @brief	Searches for the first transfer.
         *
         * @param	nBrowserID	Identifier for the browser.
         * @param	strName   	The name.
         *
         * @return	null if it fails, else the found transfer.
         */
        
        transfer* findTransfer(int nBrowserID, const std::string& strName) {
            return getTransferMap(nBrowserID).findTransfer(strName);
        }
        bool isEmpty() const {
            return m_oTransferMap.empty();
        }
        
        /*!
         * @fn	TransferMap& TransferMgr::getTransferMap(int nBrowserID)
         *
         * @brief	获取指定ID的TransferMap.
         *
         * @param	nBrowserID	Identifier for the browser.
         *
         * @return	The transfer map.
         */
        transfer_map& getTransferMap(int nBrowserID) {
            auto iter = m_oTransferMap.find(nBrowserID);
            
            if (iter != m_oTransferMap.end()) {
                return iter->second;
            }
            
            m_oTransferMap[nBrowserID] = transfer_map();
            m_oTransferMap[nBrowserID].setDefaultMsgFunc(getDefaultMsgFunc());
            return m_oTransferMap.insert(std::make_pair(nBrowserID,
                                         transfer_map())).first->second;
        }
        
        /*!
         * @fn	virtual void TransferMgr::returnSyncResult(int nBrowserID, amo::ipc_result& ret)
         *
         * @brief	返回同步调用结果.
         *
         * @exception	std::runtime_error	Raised when a runtime error condition occurs.
         *
         * @param	nBrowserID 	Identifier for the browser.
         * @param 	ret	The ret.
         */
        virtual void returnSyncResult(int nBrowserID, amo::ipc_result& ret) {
            throw std::runtime_error("unable return sync msg");
        }
        
        /*!
         * @fn	virtual void TransferMgr::returnAsyncResult(int nBrowserID,
         * 		 any& ret,
         * 		  int id,
         * 		  int frame_id)
         *
         * @brief	返回异步调用结果.
         *
         * @exception	std::runtime_error	Raised when a runtime error condition occurs.
         *
         * @param	nBrowserID 	浏览器ID.
         * @param 	ret			异步调用的执行结果.
         * @param	id		   	ipc_msg ID.
         * @param	frame_id   	Frame ID.
         */
        virtual void returnAsyncResult(int nBrowserID,
                                       any& ret,
                                       int id,
                                       int frame_id) {
            throw std::runtime_error("unable return async msg");
        }
        
        /*!
         * @fn	virtual void TransferMgr::registerClass(int nBrowserID)
         *
         * @brief	向所给浏览器注册可以实例化的类transfer.
         *
         * @param	nBrowserID	Identifier for the browser.
         */
        virtual void registerClass(int nBrowserID) {
        
        }
        
        /*!
         * @fn	any TransferMgr::onMessageTransfer(ipc_msg::SmartType msg)
         *
         * @brief	遍历transfer执行消息处理.
         *
         * @param	msg	The message.
         *
         * @return	any.
         */
        any onMessageTransfer(ipc_msg::SmartType msg) {
            std::shared_ptr<any_args_list> args = msg->getArgumentList();
            int nBrowserID = args->getInt(ipc_args_pos_info::BrowserID);
            any ret =  getTransferMap(nBrowserID).onMessageTransfer(msg);
            
            amo::u8string strMessageName(msg->getMessageName(), true);
            
            
            
            /*     $clog(amo::cdevel << func_orient << "处理消息:" << msg->toJson().to_string() << amo::endl;);
                 $clog(amo::cdevel << func_orient << "处理结果:" << ret.value() << amo::endl;);*/
            
            if (!ret.isValid()) {
                if (strMessageName == IPC_NATIVE_SYNC_EXECUTE) {
                    // 不应该返回无效, BUG??
                    ret = undefined();
                } else {
                    return ret;
                }
            }
            
            amo::ipc_result result;
            result.setID(args->getInt(ipc_args_pos_info::MessageID));
            result.setResult(ret);
            int nFrameID = args->getInt(ipc_args_pos_info::FrameID);
            
            if (strMessageName == IPC_NATIVE_EXECUTE) {
                // 普通调用不需要返回结果
            } else if (strMessageName == IPC_NATIVE_SYNC_EXECUTE)	{
                int nFlag = args->getInt(ipc_args_pos_info::CrossProcessFlag);
                
                if (nFlag != 1) {
                    // 通过管道向页面同步返回结果
                    returnSyncResult(nBrowserID, result);
                }
                
            } else if (strMessageName == IPC_NATIVE_ASYNC_EXECUTE) {
            
                int nFlag = args->getInt(ipc_args_pos_info::CrossProcessFlag);
                
                if (nFlag != 1) {
                
                    // 通过发送ipc_msg向页面异步返回结果
                    returnAsyncResult(nBrowserID,
                                      ret,
                                      args->getInt(ipc_args_pos_info::AsyncCallback),
                                      nFrameID);
                }
                
            }
            
            return ret;
            
        }
        
        smart_event_fn_type getDefaultMsgFunc() const {
        
        
            return m_fnDefaultMsgFunc;
        }
        void setDefaultMsgFunc(smart_event_fn_type val) {
        
        
        
            for (auto& p : m_oTransferMap) {
                smart_event_fn_type fn = p.second.getDefaultMsgFunc();
                
                
                if (m_fnDefaultMsgFunc == fn) {
                    p.second.setDefaultMsgFunc(val);
                }
            }
            
            
            m_fnDefaultMsgFunc = val;
        }
        
        void setDefaultTriggerEventFunc(
            std::function<void(ipc_msg::SmartType) >
            val) {
            m_fnDefaultTriggerEventFunc = val;
        }
        
        
        
        std::function<void(ipc_msg::SmartType) > getDefaultTriggerEventFunc() const {
            return m_fnDefaultTriggerEventFunc;
        }
        
        smart_event_fn_type getDefaultMsgFunc(int nBrowserID)  {
            return getTransferMap(nBrowserID).getDefaultMsgFunc();
        }
        void setDefaultMsgFunc(int nBrowserID,
                               smart_event_fn_type val) {
            getTransferMap(nBrowserID).setDefaultMsgFunc(val);
        }
    private:
        /*! @brief	保存各浏览器的TransferMap. */
        std::unordered_map<int, transfer_map> m_oTransferMap;
        
        /** @brief	默认消息处理函数，如果没有找到消息处理函数将触发该函数. */
        smart_event_fn_type m_fnDefaultMsgFunc;
        
        std::function<void(ipc_msg::SmartType)> m_fnDefaultTriggerEventFunc;
    };
    
    
    
    
}

#endif //AMO_TRANSFERMGR_23AF15AB_A44A_4BB7_B9F7_29367A237738_HPP__