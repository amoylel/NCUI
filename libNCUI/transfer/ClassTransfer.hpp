// Created by amoylel on 06/14/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_CLASSMESSAGETRANSFER_HPP__
#define AMO_CLASSMESSAGETRANSFER_HPP__

#include <memory>
#include <unordered_map>

#include <amo/stdint.hpp>
#include <amo/singleton.hpp>
#include "transfer/Transfer.hpp"
#include <amo/logger.hpp>
#include <mutex>



namespace amo {
    /*!
     * @class	Object
     *
     * @chapter api
     *
     * @brief	所有JS扩展对象所对应的C++类基类；该类不能被直接使用,
     * 			需要编写C++扩展时继承该类，并映射成JS类使用.
     */
    class ClassTransfer
        : public Transfer {
    public:
        class ClassTransferMap /*: public std::unordered_map < int64_t,
            std::shared_ptr<amo::ClassTransfer> >*/ {

        public:
            typedef std::unordered_map < int64_t,
                    std::shared_ptr<amo::ClassTransfer> > ContainerType;
                    
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
            ClassTransferMap() {
                int nSize = size();
            }
            ~ClassTransferMap() {
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
                 std::shared_ptr<amo::ClassTransfer> > ClassTransferMap;*/
    public:
    
        /*!
         * @fn	template<typename T, typename ... Args>
         * 		static std::shared_ptr<T>
         * 		 ClassTransfer::createTransfer(Args ... args)
         *
         * @ignore
         *
         * @brief	创建一个新的Transfer并注册Transfer的函数.
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
            pTransfer->setTransferObject(true); //这里创建的是对象
            pTransfer->registerFunction();
            
            addTransfer(pTransfer);
            
            return pTransfer;
        }
        
        /*!
         * @fn	template<typename T>
         * 		static std::shared_ptr<T> ClassTransfer::getUniqueTransfer()
         *
         * @ignore
         *
         * @brief	创建一个单件类，用来实例化其他对象，
         * 			 所有能调用些函数的Transfer都必须继承自Transfer和singleton.
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
            
            pTransfer->setTransferObject(false); //这里创建的是类
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
         * @fn	static std::shared_ptr<ClassTransfer> ClassTransfer::findTransfer(const int64_t& nID)
         *
         * @ignore
         *
         * @brief	通过ID查找 Transfer
         *
         * @param	nID	Transfer ID.
         *
         * @return	Transfer.
         */
        static std::shared_ptr<ClassTransfer> findTransfer(const int64_t& nID) {
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            auto ptr = getTransferMap();
            
            if (!ptr) {
                return  std::shared_ptr<ClassTransfer>();
            }
            
            auto iter = getTransferMap()->find(nID);
            
            if (iter == getTransferMap()->end()) {
                return std::shared_ptr<ClassTransfer>();
            }
            
            return iter->second;
        }
        
        /**
         * @fn	static std::shared_ptr<ClassTransfer>
         * 		ClassTransfer::findTransfer(const std::string& strObjectName)
         *
         * @ignore
         *
         * @brief	通过对象的名称查找Transfer,返回第一个成功匹配的Transfer.
         *
         * @param	strObjectName	Name of the object.
         *
         * @return	The found transfer.
         */
        
        static std::shared_ptr<ClassTransfer> findTransfer(
            const std::string& strObjectName) {
            
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            auto ptr = getTransferMap();
            
            if (!ptr) {
                return  std::shared_ptr<ClassTransfer>();
            }
            
            for (auto& p : *getTransferMap()) {
                if (p.second->getObjectName() == strObjectName) {
                    return p.second;
                }
            }
            
            return std::shared_ptr<ClassTransfer>();
        }
        
        /**
         * @fn	static std::vector<std::shared_ptr<ClassTransfer> >
         * 		 ClassTransfer::findAllTransfer( const std::string& strObjectName)
         *
         * @ignore
         *
         * @brief	通过对象的名称查找Transfer,返回所有成功匹配的Transfer..
         *
         * @param	strObjectName	Name of the object.
         *
         * @return	The found all transfer.
         */
        
        static std::vector<std::shared_ptr<ClassTransfer> > findAllTransfer(
            const std::string& strObjectName) {
            std::vector<std::shared_ptr<ClassTransfer> > vec;
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
        
        static std::shared_ptr<ClassTransfer> findClassTransfer(
            const std::string& strClassName) {
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            auto ptr = getTransferMap();
            
            if (!ptr) {
                return  std::shared_ptr<ClassTransfer>();
            }
            
            for (auto& p : *getTransferMap()) {
                if (p.second->isTransferOjbect()) {
                    continue;
                }
                
                if (p.second->transferName() == strClassName) {
                    return p.second;
                }
            }
            
            return std::shared_ptr<ClassTransfer>();
        }
        
        /**
         * @fn	static std::vector<std::shared_ptr<ClassTransfer> > ClassTransfer::findObjectTransferByClassName(const std::string& strClassName)
         *
         * @ignore
         *
         * @brief	通过类名查找对象.
         *
         * @param	strClassName	Name of the class.
         *
         * @return	The found object transfer by class name.
         */
        
        static std::vector<std::shared_ptr<ClassTransfer> >
        findObjectTransferByClassName(const std::string& strClassName) {
            std::vector<std::shared_ptr<ClassTransfer> > vec;
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            auto ptr = getTransferMap();
            
            if (!ptr) {
                return vec;
            }
            
            
            for (auto& p : *ptr) {
                if (!p.second->isTransferOjbect()) {
                    continue;
                }
                
                if (p.second->getObjectName().find("CLASS.") != -1) {
                    continue;
                }
                
                
                if (p.second->transferName() == strClassName) {
                    vec.push_back(p.second);
                }
            }
            
            return vec;
            
        }
        
        /*!
         * @fn	static void ClassTransfer::addTransfer(std::shared_ptr<ClassTransfer> transfer)
         *
         * @ignore
         *
         * @brief	添加一个Transfer到管理器中.
         *
         * @param	transfer	The transfer.
         */
        static void addTransfer(std::shared_ptr<ClassTransfer> transfer) {
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            auto ptr = getTransferMap();
            
            if (!ptr) {
                return;
            }
            
            (*ptr)[transfer->getObjectID()] = transfer;
        }
        
        
        /*!
         * @fn	static void ClassTransfer::removeTransfer(const int64_t& nID)
         *
         * @ignore
         *
         * @brief	通过ID移除一个管理器中的Transfer.
         *
         * @param	nID	The identifier.
         */
        static void removeTransfer(const int64_t& nID) {
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            auto ptr = getTransferMap();
            
            if (!ptr) {
                return ;
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
            
            /* std::vector<int64_t> vec;
            
             for (auto& p : *ptr) {
                 if (p.second->transferName() == name) {
                     vec.push_back(p.first);
                 }
             }
            
             for (auto& p : vec) {
                 removeTransfer(p);
             }*/
            
            
            for (auto iter = ptr->begin(); iter != ptr->end();) {
                if (iter->second->transferName() == name) {
                    iter->second->onBeforeRelease();
                    iter = ptr->erase(iter);
                } else {
                    ++iter;
                }
            }
            
        }
        
        static std::shared_ptr<ClassTransferMap> &getTransferMap() {
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            static std::shared_ptr<ClassTransferMap> oTransferMap(new ClassTransferMap());
            return oTransferMap;
        }
        
        static std::recursive_mutex& getMutex() {
            static std::recursive_mutex m_mutex;
            return m_mutex;
        }
        
        static void clearTransferMap() {
            std::unique_lock<std::recursive_mutex> lock(getMutex());
            
            auto classMap = ClassTransfer::getTransferMap();
            
            if (classMap) {
                for (auto iter = classMap->begin(); iter != classMap->end();) {
                    if (iter->second->transferName() != "Thread") {
                        $cdevel("release Transfer:{0}, {1} ",
                                iter->first,
                                iter->second->transferName());
                        iter->second->onBeforeRelease();
                        iter = classMap->erase(iter);
                    } else {
                        ++iter;
                    }
                }
                
                for (auto iter = classMap->begin(); iter != classMap->end();) {
                    $cdevel("release Transfer:{0}, {1}, {2} ",
                            iter->first,
                            iter->second->transferName(),
                            iter->second->getObjectName());
                    iter->second->onBeforeRelease();
                    iter = classMap->erase(iter);
                }
            }
        }
        
    public:
    
        ClassTransfer(const std::string& strName)
            : Transfer(strName) {
        }
        
        virtual std::string getClass() const override {
            return "Class";
        }
        
        virtual Transfer* getInterface(const std::string& name) override {
            if (name == ClassTransfer::getClass()) {
                return this;
            }
            
            return Transfer::getInterface(name);
        }
        
        /*!
         * @fn	virtual TransferType ClassTransfer::transferType()
         *
         *@ignore
         *
         * @brief	获取Transfer类型 .
         *
         * @return	A TransferType.
         */
        virtual TransferType transferType() {
            return TransferClass;
        }
        
        /*!
         * @fn	virtual void ClassTransfer::registerFunction()
         *
         * @ignore
         *
         * @brief	注册JS函数.
         */
        virtual void registerFunction() {
            //注册对象创建函数，这个函数与Transfer名称相同
            registerTransfer(transferName(),
                             std::bind(&ClassTransfer::createClass, this,
                                       std::placeholders::_1),
                             TransferMultiDisabled | TransferExecSync | TransferFuncConstructor);
                             
            registerTransfer("CLASS",
                             std::bind(&ClassTransfer::onGetClassObject, this,
                                       std::placeholders::_1),
                             TransferMultiDisabled | TransferFuncConstProperty | TransferExecSync);
                             
                             
            registerTransfer("fromObjectID", std::bind(&ClassTransfer::onFromObjectID, this,
                             std::placeholders::_1),
                             TransferMultiDisabled | TransferFuncStatic | TransferExecSync);
            registerTransfer("fromObjectName", std::bind(&ClassTransfer::onFromObjectName,
                             this,
                             std::placeholders::_1),
                             TransferMultiDisabled | TransferFuncStatic | TransferExecSync);
            registerTransfer("getObjectID", std::bind(&ClassTransfer::onGetObjectID,
                             this,
                             std::placeholders::_1),
                             TransferMultiDisabled | TransferFuncNormal | TransferExecSync);
                             
            registerTransfer("getObjectName", std::bind(&ClassTransfer::onGetObjectName,
                             this,
                             std::placeholders::_1),
                             TransferMultiDisabled | TransferFuncNormal | TransferExecSync);
            registerTransfer("setObjectName", std::bind(&ClassTransfer::onSetObjectName,
                             this,
                             std::placeholders::_1),
                             TransferMultiDisabled | TransferFuncNormal | TransferExecNormal);
            registerTransfer("release", std::bind(&ClassTransfer::onRelase, this,
                                                  std::placeholders::_1),
                             TransferMultiDisabled |  TransferFuncNormal | TransferExecNormal);
                             
            registerTransfer("notify", std::bind(&ClassTransfer::onNotify, this,
                                                 std::placeholders::_1),
                             TransferFuncNormal | TransferExecNormal);
            registerTransfer("getUserData", std::bind(&ClassTransfer::OnGetUserData, this,
                             std::placeholders::_1),
                             TransferMultiDisabled | TransferFuncNormal | TransferExecSync);
            registerTransfer("setUserData", std::bind(&ClassTransfer::OnSetUserData, this,
                             std::placeholders::_1),
                             TransferMultiDisabled | TransferFuncNormal | TransferExecNormal);
            registerTransfer("All", std::bind(&ClassTransfer::OnGetAll, this,
                                              std::placeholders::_1),
                             TransferMultiDisabled | TransferFuncStatic | TransferExecSync);
            return Transfer::registerFunction();
        }
        
        /*!
         * @fn	virtual Any ClassTransfer::onCreateClass(IPCMessage::SmartType msg)
         *
         * @ignore
         *
         * @brief	继承 此函数 创建类对象.
         *
         * @param	msg	The message.
         *
         * @return	Any.
         */
        virtual Any onCreateClass(IPCMessage::SmartType msg) {
            return Undefined();
        }
        
        
        virtual Any createClass(IPCMessage::SmartType msg) {
            Any val = this->onCreateClass(msg);
            
            if (val.is<amo::u8json>()) {
                amo::u8json json = val.As<amo::u8json>();
                int64_t nID = json.get<int64_t>("id");
                auto pTransfer = findTransfer(nID);
                
                if (pTransfer) {
                    pTransfer->setTriggerEventFunc(this->getTriggerEventFunc());
                    pTransfer->setDefaultTriggerEventFunc(this->getDefaultTriggerEventFunc());
                    pTransfer->setModuleName(this->getModuleName());
                }
            }
            
            return val;
        }
        /**
         * @fn	virtual void ClassTransfer::onBeforeRelease()
         *
         * @ignore
         *
         * @brief	释放资源.
         */
        
        virtual void onBeforeRelease() {
        
            $cdevel("release transfer :transferName = \"{0}\"， objectName = \"{1}\"， objectID = {2}",
                    transferName(), getObjectName(), getObjectID());
                    
                    
            /* this->isWorkOnRenderer() {
                 amo::DllManager<PID_RENDERER>::
             } else {
            
             }*/
            
            return;
        }
        
        /**
         * @fn	static constexpr property deprecated single Any CLASS(IPCMessage::SmartType msg)
         *
         * @tag static const property  single
         *
         * @brief	获取当前类所对象的对象，调用该对象的函数将会作用到类上面去.
         *
         * @return	#Object 类所对象的CLASS对象.
         * @example
         *
         ```
        	include('Demo');
        	console.assert(Demo.CLASS.staticTestID == 2, true);
        	Demo.CLASS.staticTestID = 32;
        	console.log(Demo.CLASS.staticTestID);
        	console.assert(Demo.CLASS.staticTestID != 2, true);
        
         ```
         */
        
        virtual Any onGetClassObject(IPCMessage::SmartType msg) {
            std::string strObjectName = "CLASS.";
            strObjectName += transferName();
            auto transfer = findTransfer(strObjectName);
            
            if (transfer) {
                return transfer->getFuncMgr().toSimplifiedJson();
            }
            
            return Nil();
        }
        
        /**
         * @fn	static Any fromObjectName(IPCMessage::SmartType msg)
         *
         * @tag static single sync
         *
         * @brief	通过对象名称查找对象.
         *
         * @param	#String 对象名称.
         *
         * @return	#Object 如果对象存在，返回该对象; 如果不存在，返回Undefined.
         *
         * @see fromObjectID=Object.fromObjectID
         * @example
         *
         ```
        	include('Demo');
        	var demo = new Demo();
        	demo.setObjectName('demo2');
        	var name = demo.getObjectName();
        	var demo2 = Demo.fromObjectName(name);
        	console.assert(demo.getObjectID() == demo2.getObjectID(), true);
         ```
         */
        
        virtual Any onFromObjectName(IPCMessage::SmartType msg) {
            std::string objectName = msg->getArgumentList()->getString(0);
            auto transfer = findTransfer(objectName);
            
            if (!transfer) {
                return Undefined();
            }
            
            return transfer->getFuncMgr().toSimplifiedJson();
        }
        
        /**
         * @fn	static Any fromObjectID(IPCMessage::SmartType msg)
         *
         * @tag static single sync
         *
         * @brief	通过对象ID查找对象.
         *
         * @param	#Int 对象ID.
         *
         * @return	#Object 如果对象存在，返回该对象; 如果不存在，返回Undefined.
         */
        
        /**
        * @fn  static Any fromObjectID(IPCMessage::SmartType msg)
        * @tag static sync single
        *
        * @brief	通过对象ID查找对象.
        *
        * @param	#String 对象ID.
        *
        * @return	#Object 如果对象存在，返回该对象; 如果不存在，返回Undefined.
        *
        * @see	fromObjectName=Object.fromObjectName
        *
        * @example
        *
         ```
        
        	include('Demo');
        	var demo = new Demo();
        	demo.setObjectName('demo1');
        	var id = demo.getObjectID();
        	var demo2 = Demo.fromObjectID(id);
        	console.assert(demo.getObjectName() == demo2.getObjectName(), true);
         ```
        */
        
        virtual Any onFromObjectID(IPCMessage::SmartType msg) {
            Any& val = msg->getArgumentList()->getValue(0);
            int64_t nObjectID = 0;
            
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
                return Undefined();
            }
            
            auto transfer = findTransfer(nObjectID);
            
            if (!transfer) {
                return Undefined();
            }
            
            return transfer->getFuncMgr().toSimplifiedJson();
            return Undefined();
        }
        
        /**
         * @fn	virtual Any setObjectName(IPCMessage::SmartType msg)
         *
         * 	@tag
         *
         * @brief	设置当前对象名称.
         *
         * @param	#String 对象名称.
         *
         * @return	无.
         *
         * @example
         *
         ```
        	include ('Demo');
        	var demo = new Demo();
        	demo.setObjectName('demo3');
        	console.log(demo.getObjectName());
         ```
         */
        
        virtual Any onSetObjectName(IPCMessage::SmartType msg) {
            std::string strName = msg->getArgumentList()->getString(0);
            this->setObjectName(strName);
            return Undefined();
        }
        
        /*!
         * @fn	virtual Any onGetObjectID(IPCMessage::SmartType msg)
         *
         * @brief	获取当前对象名称ID.ID为一个字符串
         *
         *
         * @return	#String.
         *
         * @see getObjectName=Object.getObjectName
         *
         * @example
         *
         ```
        	 include ('Demo');
        	 var demo = new Demo();
        	 demo.setObjectName('demo4');
        	 console.log(demo.getObjectName());
         ```
         */
        
        virtual Any onGetObjectID(IPCMessage::SmartType msg) {
            return std::to_string(this->getObjectID());
            
        }
        /**
         * @fn	virtual Any getObjectName(IPCMessage::SmartType msg)
         *
         * 	@tag  single sync
         *
         * @brief	获取当前对象名称.
         *
         *
         * @return	#String 对象名称.
         *
         * @see getObjectID=Object.getObjectID, setObjectName=Object.setObjectName
         *
         * @example
         *
         ```
        	include ('Demo');
        	var demo = new Demo();
        	demo.setObjectName('demo4');
        	console.log(demo.getObjectName());
         ```
         */
        
        virtual Any onGetObjectName(IPCMessage::SmartType msg) {
            return this->getObjectName();
        }
        
        /**
         * @fn	virtual Any ClassTransfer::notify(IPCMessage::SmartType msg)
         *
         * @brief	向自己发送一个通知，可以监听这个通知得到返回结果(当transfer在单独的线程上执行时，可以通过自己定义消息来监听执行进度).
         *
         * @param	#String 通知名称
         * @param	#Object 任务Javascript所支持的基本类型（Int Double String JsonObject Array），可以不填.
         *
         *
         * @return	无.
         *
         * @example
         *
         ```
        	 include ('Demo');
        	 var demo = new Demo();
        	 demo.on('Demo.notifyTest', function(){
        		console.log(arguments);
        	 });
        	 demo.notify('Demo.notifyTest');
         ```
         */
        
        virtual Any onNotify(IPCMessage::SmartType msg) {
            std::string strName = msg->getArgumentList()->getString(0);
            TransferEventInfo info;
            info.name = strName;
            info.toAll = true;
            Any& val = msg->getArgumentList()->getValue(1);
            
            if (val.isValid() && !val.is<Undefined>() && !val.is<Nil>()) {
                info.data = val;
            }
            
            triggerEvent(info);
            return Undefined();
        }
        
        /**
         * @fn	virtual Any relase(IPCMessage::SmartType msg) override
         * @tag single
         * @brief	释放当前对象所对应的C++对象，该函数调用后页面上对应的对象将无法使用
         *
         * @return	无.
         */
        
        virtual Any onRelase(IPCMessage::SmartType msg) override {
        
        
            removeTransfer(getObjectID());
            
            if (m_bReleased) {
                return Undefined();
            }
            
            m_bReleased = true;
            TransferEventInfo info;
            info.name = "transfer.release";
            info.toAll = true;
            triggerEvent(info);
            return Undefined();
        }
        
        
        /*!
        * @fn	Any AppTransfer::getUserData(IPCMessage::SmartType msg);
        *
        * @tag single sync
        *
        * @brief	获取用户保存的自定义数据.
        *
        * @return	#JsonObject 所有配置参数.
        */
        
        /*!
        * @fn	Any AppTransfer::getUserData(IPCMessage::SmartType msg);
        *
        * @brief	根据输入字段返回用户保存的自定义数据.
        *
        * @param	#String 需要获取的字段名称.
        *
        * @return	返回字段名所对应的数据。返回类型视配置参数类型而定，为JS所支持的基本数据类型.
        *
        * @see setUserData=Object.setUserData
        * @example
        *
        ```
        	include('Demo');
        	var demo = new Demo();
        	demo.setUserData({aaa: 123, bbb: 344});
        	console.assert(demo.getUserData('aaa')== 123, true);
        	console.log(demo.getUserData());
        ```
        */
        
        virtual Any OnGetUserData(IPCMessage::SmartType msg) {
            std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
            
            Any& val = args->getValue(0);
            
            if (val.type() == AnyValueType<Nil>::value) {
                // 返回所有设置
                return userData;
                
            } else  if (val.type() == AnyValueType<std::string>::value) {
                std::string strKey = args->getString(0);
                auto& json = userData;
                
                if (json.is_bool(strKey)) {
                    return json.getBool(strKey);
                } else if (json.is_int(strKey)) {
                    return json.getInt(strKey);
                } else if (json.is_string(strKey)) {
                    return json.getString(strKey);
                } else if (json.is_double(strKey)) {
                    return json.get<double>(strKey);
                } else if (json.is_uint(strKey)) {
                    return (int)json.getUint(strKey);
                } else if (json.is_object(strKey)) {
                    return json.getJson(strKey);
                } else {
                    return Undefined();
                }
                
                // 返回单项设置
            }
            
            return Undefined();
        }
        
        /*!
        * @fn	Any AppTransfer::setUserData(IPCMessage::SmartType msg);
        * @tag single
        * @brief	保存自定义数据.该函数并不要求一定要输入所支持的参数，
        * 			你也可以设置自定义参数，并在getUserData里面获取所设置的值与{@link getUserData=Object.getUserData}相对应.
        *
        * @param	#JsonObject 需要设置的参数，该值为一个JSON对象.
        *
        * @return	无.
        *
        * @see getUserData=Object.getUserData
        *
        * @example
        *
        ```
        	include('Demo');
        	var demo = new Demo();
        	demo.setUserData({aaa: 123, bbb: 344});
        	demo.setUserData({aaa:222});
        	demo.setUserData({ccc:444});
        	console.assert(demo.getUserData('aaa')== 222, true);
        	console.log(demo.getUserData());
        ```
        *
        */
        
        
        virtual Any OnSetUserData(IPCMessage::SmartType msg) {
            std::shared_ptr<AnyArgsList> args = msg->getArgumentList();
            
            Any& val = args->getValue(0);
            
            
            if (val.type() == AnyValueType<amo::u8json>::value) {
                // 更新AppSettings
                amo::u8json json = args->getJson(0);
                
                if (json.is_object()) {
                    userData.join(json);
                }
                
                return Undefined();
            }
            
            return Undefined();
        }
        
        /**
         * @fn	virtual Any All(IPCMessage::SmartType msg)
         * @tag single static sync
         * @brief	获取当前类的所有对象.
         *
         *
         * @return	#Array 包含当前类的所有对象.
         * @example
         *
         ```
        	include('Demo');
        	var arr = Demo.All();
        	console.log(arr);
         ```
         */
        
        virtual Any OnGetAll(IPCMessage::SmartType msg) {
            auto vec = ClassTransfer::findObjectTransferByClassName(transferName());
            std::vector<Any> ret;
            
            for (auto& p : vec) {
                ret.push_back(p->getFuncMgr().toSimplifiedJson());
            }
            
            return ret;
        }
        
        /*!
         * @fn	virtual Any ClassTransfer::onMessageTransfer(
         * 		IPCMessage::SmartType message) override
         *
         * @ignore
         *
         * @brief	执行消息.
         *
         * @param	message	The message.
         *
         * @return	Any.
         */
        virtual Any onMessageTransfer(IPCMessage::SmartType message) override {
            std::shared_ptr<AnyArgsList> args = message->getArgumentList();
            int nBrowserID = args->getInt(IPCArgsPosInfo::BrowserID);
            int64_t nID = args->getInt64(IPCArgsPosInfo::TransferID);
            // 查找Transfer
            std::shared_ptr<ClassTransfer> transfer = findTransfer(nID);
            
            // 如果Transfer不存在或者Transfer == this 执行
            if (!transfer || transfer == getDerivedClass<ClassTransfer>()) {
                return Transfer::onMessageTransfer(message);
            }
            
            std::string strObjectName = transfer->getObjectName();
            
            if (strObjectName.find("CLASS.") == 0) {
                // 重定向到类的Transfer中执行
                std::shared_ptr<ClassTransfer>  pClassTransfer;
                pClassTransfer = ClassTransfer::findClassTransfer(
                                     transfer->transferName());
                                     
                if (!pClassTransfer && (pClassTransfer->getObjectID() == getObjectID())) {
                    return Transfer::onMessageTransfer(message);
                }
                
                args->setValue(IPCArgsPosInfo::TransferID,
                               pClassTransfer->getObjectID());
                               
                return pClassTransfer->onMessageTransfer(message);
            }
            
            //调用transfer的OnMessaggeTransfer
            return transfer->onMessageTransfer(message);
        }
        
    };
    
    
    
    class TransferRegister {
    public:
        TransferRegister() {
            nBrowserID = -1;
            transferMap = ClassTransfer::getTransferMap();
            pLogger = amo::log::logger();
        }
        int nBrowserID;
        std::shared_ptr<amo::ClassTransfer::ClassTransferMap> transferMap;
        std::function<void(int, std::shared_ptr<amo::ClassTransfer>)> fnCallback;
        std::shared_ptr<spdlog::logger> pLogger;
        std::string moduleName;
        
    };
    
}

#endif // AMO_CLASSMESSAGETRANSFER_HPP__

