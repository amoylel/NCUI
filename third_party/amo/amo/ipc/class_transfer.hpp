// Created by amoylel on 14/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_CLASS_TRANSFER_AFA36E60_1925_4265_8ABA_C47F519FBD6F_HPP__
#define AMO_CLASS_TRANSFER_AFA36E60_1925_4265_8ABA_C47F519FBD6F_HPP__



#include <memory>
#include <unordered_map>

#include <amo/stdint.hpp>
#include <amo/singleton.hpp>
#include <amo/ipc/transfer.hpp>
#include <amo/logger.hpp>
#include <mutex>



namespace amo {
    /*!
     * @class	class_transfer
     *
     * @brief	js 类 对应的C++类基类.
     */
    class class_transfer
        : public transfer {
    public:
        class class_transferMap /*: public std::unordered_map < int64_t,
            std::shared_ptr<amo::class_transfer> >*/ {

        public:
            typedef std::unordered_map < int64_t,
                    std::shared_ptr<amo::class_transfer> > ContainerType;
                    
            typedef ContainerType::hasher hasher;
            typedef ContainerType::key_type key_type;
            typedef ContainerType::mapped_type mapped_type;
            typedef ContainerType::key_equal key_equal;
            typedef ContainerType::key_compare key_compare;	// extra
            
            //	typedef typename _Mybase::value_compare value_compare;
            typedef  ContainerType::allocator_type allocator_type;
            typedef  ContainerType::size_type size_type;
            typedef  ContainerType::difference_type difference_type;
            typedef  ContainerType::pointer pointer;
            typedef  ContainerType::const_pointer const_pointer;
            typedef  ContainerType::reference reference;
            typedef  ContainerType::const_reference const_reference;
            typedef  ContainerType::iterator iterator;
            typedef  ContainerType::const_iterator const_iterator;
            typedef  ContainerType::value_type value_type;
            
            typedef  ContainerType::iterator local_iterator;
            typedef  ContainerType::const_iterator const_local_iterator;
            
        public:
            ~class_transferMap() {
                int nSize = size();
            }
            
            iterator begin() {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_oMap.begin();
            }
            
            const_iterator begin() const {
                //std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_oMap.begin();
            }
            iterator end() {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_oMap.end();
            }
            
            const_iterator end() const {
                //std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_oMap.end();
            }
            
            size_t size() const {
                //std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_oMap.size();
            }
            
            
            iterator erase(const_iterator _Plist) {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_oMap.erase(_Plist);
            }
            
            iterator erase(const_iterator _First, const_iterator _Last) {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_oMap.erase(_First, _Last);
            }
            
            size_type erase(const key_type& _Keyval) {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_oMap.erase(_Keyval);
            }
            
            void clear()   {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_oMap.clear();
            }
            
            iterator find(const key_type& _Keyval) {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_oMap.find(_Keyval);
            }
            
            const_iterator find(const key_type& _Keyval) const {
                //std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_oMap.find(_Keyval);
            }
            
            
            mapped_type& operator[](const key_type& _Keyval) {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_oMap[_Keyval];
            }
            
            mapped_type& at(const key_type& _Keyval) {
                std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_oMap.at(_Keyval);
            }
            
            const mapped_type& at(const key_type& _Keyval) const {
                //std::unique_lock<std::recursive_mutex> lock(m_mutex);
                return m_oMap.at(_Keyval);
            }
        private:
            ContainerType m_oMap;
            std::recursive_mutex m_mutex;
        };
        /* typedef std::unordered_map < int64_t,
                 std::shared_ptr<amo::class_transfer> > class_transferMap;*/
    public:
    
        /*!
         * @fn	template<typename T, typename ... Args>
         * 		static std::shared_ptr<T>
         * 		 class_transfer::createTransfer(Args ... args)
         *
         * @brief	创建一个新的transfer并注册transfer的函数.
         *
         * @tparam	T   	Generic type parameter.
         * @tparam	Args	Type of the arguments.
         * @param	args	Variable arguments providing the arguments.
         *
         * @return	The new transfer.
         */
        template<typename T, typename ... Args>
        static std::shared_ptr<T> createTransfer(Args ... args) {
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            std::shared_ptr<T> pTransfer(new T(args ...));
            pTransfer->setClassObject(true); //这里创建的是对象
            pTransfer->registerFunction();
            
            addTransfer(pTransfer);
            
            return pTransfer;
        }
        
        /*!
         * @fn	template<typename T>
         * 		static std::shared_ptr<T> class_transfer::getUniqueTransfer()
         *
         * @brief	创建一个单件类，用来实例化其他对象，
         * 			 所有能调用些函数的transfer都必须继承自transfer和singleton.
         *
         * @tparam	T	Generic type parameter.
         *
         * @return	The unique transfer.
         */
        template<typename T>
        static std::shared_ptr<T> getUniqueTransfer() {
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            std::shared_ptr<T> pTransfer = T::getInstance()->getDerivedClass<T>();
            
            if (pTransfer->isFuncRegistered()) {
                return pTransfer;
            }
            
            pTransfer->setClassObject(false); //这里创建的是类
            pTransfer->registerFunction();
            
            addTransfer(pTransfer);
            
            auto pAssitTransfer = createTransfer<T>();
            std::string strObjectName = "CLASS.";
            strObjectName += pTransfer->transferName();
            pAssitTransfer->setObjectName(strObjectName);
            return pTransfer;
        }
        
    public:
    
        // 静态函数
        
        /*!
         * @fn	static std::shared_ptr<class_transfer> class_transfer::findTransfer(const int64_t& nID)
         *
         * @brief	通过ID查找 transfer
         *
         * @param	nID	transfer ID.
         *
         * @return	transfer.
         */
        static std::shared_ptr<class_transfer> findTransfer(const int64_t& nID) {
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            auto ptr = getTransferMap();
            
            if (!ptr) {
                return  std::shared_ptr<class_transfer>();
            }
            
            auto iter = getTransferMap()->find(nID);
            
            if (iter == getTransferMap()->end()) {
                return std::shared_ptr<class_transfer>();
            }
            
            return iter->second;
        }
        
        /**
         * @fn	static std::shared_ptr<class_transfer>
         * 		class_transfer::findTransfer(const std::string& strObjectName)
         *
         * @brief	通过对象的名称查找transfer,返回第一个成功匹配的transfer.
         *
         * @param	strObjectName	Name of the object.
         *
         * @return	The found transfer.
         */
        
        static std::shared_ptr<class_transfer> findTransfer(
            const std::string& strObjectName) {
            
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            auto ptr = getTransferMap();
            
            if (!ptr) {
                return  std::shared_ptr<class_transfer>();
            }
            
            for (auto& p : *getTransferMap()) {
                if (p.second->getObjectName() == strObjectName) {
                    return p.second;
                }
            }
            
            return std::shared_ptr<class_transfer>();
        }
        
        /**
         * @fn	static std::vector<std::shared_ptr<class_transfer> >
         * 		 class_transfer::findAllTransfer( const std::string& strObjectName)
         *
         * @brief	通过对象的名称查找transfer,返回所有成功匹配的transfer..
         *
         * @param	strObjectName	Name of the object.
         *
         * @return	The found all transfer.
         */
        
        static std::vector<std::shared_ptr<class_transfer> > findAllTransfer(
            const std::string& strObjectName) {
            std::vector<std::shared_ptr<class_transfer> > vec;
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            auto ptr = getTransferMap();
            
            if (!ptr) {
                return vec;
            }
            
            for (auto& p : *getTransferMap()) {
                if (p.second->getObjectName() == strObjectName) {
                    vec.push_back(p.second);
                }
            }
            
            return vec;
        }
        
        static std::shared_ptr<class_transfer> findclass_transfer(
            const std::string& strClassName) {
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            auto ptr = getTransferMap();
            
            if (!ptr) {
                return  std::shared_ptr<class_transfer>();
            }
            
            for (auto& p : *getTransferMap()) {
                if (p.second->isClassOjbect()) {
                    continue;
                }
                
                if (p.second->transferName() == strClassName) {
                    return p.second;
                }
            }
            
            return std::shared_ptr<class_transfer>();
        }
        
        /*!
         * @fn	static void class_transfer::addTransfer(std::shared_ptr<class_transfer> transfer)
         *
         * @brief	添加一个transfer到管理器中.
         *
         * @param	transfer	The transfer.
         */
        static void addTransfer(std::shared_ptr<class_transfer> transfer) {
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            auto ptr = getTransferMap();
            
            if (!ptr) {
                return;
            }
            
            (*ptr)[transfer->getObjectID()] = transfer;
        }
        
        
        /*!
         * @fn	static void class_transfer::removeTransfer(const int64_t& nID)
         *
         * @brief	通过ID移除一个管理器中的transfer.
         *
         * @param	nID	The identifier.
         */
        static void removeTransfer(const int64_t& nID) {
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            auto ptr = getTransferMap();
            
            if (!ptr) {
                return;
            }
            
            auto iter = ptr->find(nID);
            
            if (iter != ptr->end()) {
                iter->second->onBeforeRelease();
                ptr->erase(iter);
            }
            
            return;
        }
        
        static void removeTransferByName(const std::string& name) {
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            auto ptr = getTransferMap();
            
            if (!ptr) {
                return;
            }
            
            for (auto iter = ptr->begin(); iter != ptr->end();) {
                if (iter->second->transferName() == name) {
                    iter->second->onBeforeRelease();
                    iter = ptr->erase(iter);
                } else {
                    ++iter;
                }
            }
            
        }
        
        static std::shared_ptr<class_transferMap> &getTransferMap() {
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            static std::shared_ptr<class_transferMap> oTransferMap(new class_transferMap());
            return oTransferMap;
        }
        
        static std::recursive_mutex& getMutex() {
            static std::recursive_mutex m_mutex;
            return m_mutex;
        }
        
        static void clearTransferMap() {
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            
            auto classMap = class_transfer::getTransferMap();
            
            if (classMap) {
                for (auto iter = classMap->begin(); iter != classMap->end();) {
                    if (iter->second->transferName() != "Thread") {
                        $cdevel("release transfer:{0}, {1} ",
                                iter->first,
                                iter->second->transferName());
                        iter->second->onBeforeRelease();
                        iter = classMap->erase(iter);
                    } else {
                        ++iter;
                    }
                }
                
                for (auto iter = classMap->begin(); iter != classMap->end();) {
                    $cdevel("release transfer:{0}, {1}, {2} ",
                            iter->first,
                            iter->second->transferName(),
                            iter->second->getObjectName());
                    iter->second->onBeforeRelease();
                    iter = classMap->erase(iter);
                }
            }
        }
        
    public:
    
        class_transfer(const std::string& strName)
            : transfer(strName) {
        }
        
        virtual std::string getClass() const override {
            return "Class";
        }
        
        virtual transfer* getInterface(const std::string& name) override {
            if (name == class_transfer::getClass()) {
                return this;
            }
            
            return transfer::getInterface(name);
        }
        
        /*!
         * @fn	virtual transfer_type class_transfer::transferType()
         *
         * @brief	获取transfer类型 .
         *
         * @return	A transfer_type.
         */
        virtual transfer_type transferType() {
            return transfer_class;
        }
        
        /*!
         * @fn	virtual void class_transfer::registerFunction()
         *
         * @brief	注册JS函数.
         */
        virtual void registerFunction() {
            //注册对象创建函数，这个函数与transfer名称相同
            registerTransfer(transferName(),
                             std::bind(&class_transfer::onCreateClass, this,
                                       std::placeholders::_1),
                             transfer_multi_disabled | transfer_exec_sync | transfer_func_constructor);
                             
            registerTransfer("CLASS",
                             std::bind(&class_transfer::onGetClassObject, this,
                                       std::placeholders::_1),
                             transfer_multi_disabled | transfer_func_const_property | transfer_exec_sync);
                             
                             
            registerTransfer("fromObjectID", std::bind(&class_transfer::onFromObjectID,
                             this,
                             std::placeholders::_1),
                             transfer_multi_disabled | transfer_func_static | transfer_exec_sync);
            registerTransfer("fromObjectName", std::bind(&class_transfer::onFromObjectName,
                             this,
                             std::placeholders::_1),
                             transfer_multi_disabled | transfer_func_static | transfer_exec_sync);
            registerTransfer("getObjectName", std::bind(&class_transfer::onGetObjectName,
                             this,
                             std::placeholders::_1),
                             transfer_func_normal | transfer_exec_sync);
            registerTransfer("setObjectName", std::bind(&class_transfer::onSetObjectName,
                             this,
                             std::placeholders::_1),
                             transfer_func_normal | transfer_exec_normal);
            registerTransfer("release", std::bind(&class_transfer::onRelase, this,
                                                  std::placeholders::_1),
                             transfer_multi_disabled |  transfer_func_normal | transfer_exec_normal);
                             
            registerTransfer("notify", std::bind(&class_transfer::onNotify, this,
                                                 std::placeholders::_1),
                             transfer_func_normal | transfer_exec_normal);
            return transfer::registerFunction();
        }
        
        /*!
         * @fn	virtual any class_transfer::onCreateClass(ipc_msg::SmartType msg)
         *
         * @brief	继承 此函数 创建类对象.
         *
         * @param	msg	The message.
         *
         * @return	any.
         */
        virtual any onCreateClass(ipc_msg::SmartType msg) {
            return undefined();
        }
        
        /**
         * @fn	virtual void class_transfer::onBeforeRelease()
         *
         * @brief	释放资源.
         */
        
        virtual void onBeforeRelease() {
            $cdevel("release transfer resource:transferName = \"{0}\"， objectName = \"{1}\"， objectID = {2}",
                    transferName(), getObjectName(), getObjectID());
            return;
        }
        
        virtual any onGetClassObject(ipc_msg::SmartType msg) {
            std::string strObjectName = "CLASS.";
            strObjectName += transferName();
            auto transfer = findTransfer(strObjectName);
            
            if (transfer) {
                return transfer->getFuncMgr().toSimplifiedJson();
            }
            
            return nil();
        }
        
        virtual any onFromObjectName(ipc_msg::SmartType msg) {
            std::string objectName = msg->getArgumentList()->getString(0);
            auto transfer = findTransfer(objectName);
            
            if (!transfer) {
                return undefined();
            }
            
            return transfer->getFuncMgr().toSimplifiedJson();
        }
        
        virtual any onFromObjectID(ipc_msg::SmartType msg) {
            any& val = msg->getArgumentList()->getValue(0);
            int nObjectID = 0;
            
            if (val.is<int64_t>()) {
                nObjectID = val.As<int64_t>();
            } else if (val.is<std::string>()) {
                std::string str = val.As<std::string>();
                
                if (!str.empty()) {
                    std::stringstream stream;
                    stream << str;
                    stream >> nObjectID;
                }
            }
            
            if (nObjectID == 0) {
                return undefined();
            }
            
            auto transfer = findTransfer(nObjectID);
            
            if (!transfer) {
                return undefined();
            }
            
            return transfer->getFuncMgr().toSimplifiedJson();
            return undefined();
        }
        
        virtual any onSetObjectName(ipc_msg::SmartType msg) {
            std::string strName = msg->getArgumentList()->getString(0);
            this->setObjectName(strName);
            return undefined();
        }
        
        virtual any onGetObjectName(ipc_msg::SmartType msg) {
            return this->getObjectName();
        }
        
        /**
         * @fn	virtual any class_transfer::onNotify(ipc_msg::SmartType msg)
         *
         * @brief	向自己发送一个通知，可以监听这个通知得到返回结果(当transfer在单独的线程上执行时，可以通过自己定义消息来监听执行进度).
         *
         * @param	msg	The message.
         *
         * @return	any.
         */
        
        virtual any onNotify(ipc_msg::SmartType msg) {
            std::string strName = msg->getArgumentList()->getString(0);
            transfer_event_info info;
            info.name = strName;
            info.toAll = true;
            any& val = msg->getArgumentList()->getValue(1);
            
            if (val.isValid() && !val.is<undefined>() && !val.is<nil>()) {
                info.data = val;
            }
            
            triggerEvent(info);
            return undefined();
        }
        
        virtual any onRelase(ipc_msg::SmartType msg) override {
        
            removeTransfer(getObjectID());
            
            if (m_bReleased) {
                return undefined();
            }
            
            m_bReleased = true;
            transfer_event_info info;
            info.name = "transfer.release";
            info.toAll = true;
            triggerEvent(info);
            return undefined();
        }
        /*!
         * @fn	virtual any class_transfer::onMessageTransfer(
         * 		ipc_msg::SmartType message) override
         *
         * @brief	执行消息.
         *
         * @param	message	The message.
         *
         * @return	any.
         */
        virtual any onMessageTransfer(ipc_msg::SmartType message) override {
            std::shared_ptr<any_args_list> args = message->getArgumentList();
            int nBrowserID = args->getInt(ipc_args_pos_info::BrowserID);
            int64_t nID = args->getInt64(ipc_args_pos_info::TransferID);
            // 查找transfer
            std::shared_ptr<class_transfer> transfer = findTransfer(nID);
            
            // 如果transfer不存在或者transfer == this 执行
            if (!transfer || transfer == getDerivedClass<class_transfer>()) {
                return transfer::onMessageTransfer(message);
            }
            
            std::string strObjectName = transfer->getObjectName();
            
            if (strObjectName.find("CLASS.") == 0) {
                // 重定向到类的transfer中执行
                std::shared_ptr<class_transfer>  pclass_transfer;
                pclass_transfer = class_transfer::findclass_transfer(
                                      transfer->transferName());
                                      
                if (!pclass_transfer && (pclass_transfer->getObjectID() == getObjectID())) {
                    return transfer::onMessageTransfer(message);
                }
                
                args->setValue(ipc_args_pos_info::TransferID,
                               pclass_transfer->getObjectID());
                               
                return pclass_transfer->onMessageTransfer(message);
            }
            
            //调用transfer的OnMessaggeTransfer
            return transfer->onMessageTransfer(message);
        }
        
    };
    
    
    
    class transfer_tegister {
    public:
        transfer_tegister() {
            nBrowserID = -1;
            transferMap = class_transfer::getTransferMap();
            pLogger = amo::log::logger();
        }
        int nBrowserID;
        std::shared_ptr<amo::class_transfer::class_transferMap> transferMap;
        std::function<void(int, std::shared_ptr<amo::class_transfer>)> fnCallback;
        std::shared_ptr<spdlog::logger> pLogger;
    };
    
}


#endif //AMO_CLASS_TRANSFER_AFA36E60_1925_4265_8ABA_C47F519FBD6F_HPP__


