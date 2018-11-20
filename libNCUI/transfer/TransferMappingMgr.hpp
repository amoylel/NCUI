// Created by amoylel on 07/04/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_TRANSFERMAPPINGMGR_HPP__
#define AMO_TRANSFERMAPPINGMGR_HPP__

#include <unordered_map>
#include <memory>
#include <amo/singleton.hpp>
#include <amo/json.hpp>
#include "transfer/TransferMapping.hpp"
#include "transfer/ClassTransfer.hpp"


namespace amo {
    /*!
     * @struct	cefrefptr_hash
     *
     * @brief	自定义Hash TransferMappingMgr.
     *
     * @tparam	T	Generic type parameter.
     */
    template <class T>
    struct cefrefptr_hash {
        std::size_t operator()(const T& p) const {
            int addr = (int)&p;
            return std::hash<int>()(addr);
        }
    };
    
    template<typename T>
    bool static isSameCefRefPtr(const T a, const T b) {
        return false;
    }
    template<>
    bool static isSameCefRefPtr < CefRefPtr<CefBrowserHost> >(const
            CefRefPtr<CefBrowserHost> a,
            const CefRefPtr<CefBrowserHost> b) {
        if (a == NULL && b == NULL) {
            return true;
        }
        
        if (a == NULL || b == NULL) {
            return false;
        }
        
        return a->GetBrowser()->IsSame(b->GetBrowser());
    }
    
    template<>
    bool static isSameCefRefPtr < CefRefPtr<CefBrowser> > (const
            CefRefPtr<CefBrowser> a,
            const CefRefPtr<CefBrowser> b) {
        if (a == NULL && b == NULL) {
            return true;
        }
        
        if (a == NULL || b == NULL) {
            return false;
        }
        
        return a->IsSame(b);
    }
    
    template<>
    bool static isSameCefRefPtr < CefRefPtr<CefFrame> > (const CefRefPtr<CefFrame>
            a,
            const  CefRefPtr<CefFrame> b) {
        if (a == NULL && b == NULL) {
            return true;
        }
        
        if (a == NULL || b == NULL) {
            return false;
        }
        
        return a->GetIdentifier() == b->GetIdentifier();
    }
    
    template<typename T, typename P1, typename P2>
    class TransferMappingMgrBase : public
        amo::singleton<TransferMappingMgrBase<T, P1, P2> >  {
    public:
        typedef P1 transfer_type;
        typedef P2 cefrefptr_type;
    public:
    
        transfer_type findTransferByCefRefPtr(cefrefptr_type pCefRefPtr) {
            for (auto& p : m_oTransferMap) {
                if (isSameCefRefPtr<cefrefptr_type>(p.first, pCefRefPtr)) {
                    return p.second;
                }
            }
            
            return transfer_type();
        }
        /*!
        * @fn	transfer_type TransferMappingMgr::toTransfer(cefrefptr_type pCefRefPtr)
        *
        * @brief	将Cef类型转换为Transfer类型.
        *
        * @param	pCefRefPtr	Cef类型.
        *
        * @return	Transfer类型.
        */
        transfer_type toTransfer(cefrefptr_type pCefRefPtr) {
            transfer_type pTransfer;
            pTransfer = findTransferByCefRefPtr(pCefRefPtr);
            
            if (pTransfer) {
                return pTransfer;
            }
            
            // 创建一个新的Transfer并保存
            
            pTransfer = ClassTransfer::createTransfer<T>(pCefRefPtr);
            m_oTransferMap[pCefRefPtr] = pTransfer;
            return pTransfer;
        }
        
        /*!
        * @fn	amo::u8json TransferMappingMgr::toJson(cefrefptr_type pCefRefPtr)
        *
        * @brief	将Cef类型转换为JSON.
        *
        * @param	pCefRefPtr	The cef reference pointer.
        *
        * @return	pCefRefPtr as an amo::u8json.
        */
        amo::u8json toJson(cefrefptr_type pCefRefPtr) {
            transfer_type pTransfer = toTransfer(pCefRefPtr);
            return pTransfer->getFuncMgr().toJson();
        }
        
        /*!
        * @fn	cefrefptr_type TransferMappingMgr::formJson(amo::u8json& json)
        *
        * @brief	将JSON转换为Cef类型.
        *
        * @param 	json	The JSON.
        *
        * @return	A cefrefptr_type.
        */
        cefrefptr_type formJson(amo::u8json& json) {
            int nObjectID = json.get<int64_t>("id");
            
            for (auto& p : m_oTransferMap) {
                if (p.second->getObjectID() == nObjectID) {
                    return p.first;
                }
            }
            
            return NULL;
        }
        
        /*!
        * @fn	amo::u8json TransferMappingMgr::toSimplifiedJson(cefrefptr_type pCefRefPtr)
        *
        * @brief	将Cef类型转换为一个简单的JSON对象（只有ID和Name）.
        *
        * @param	pCefRefPtr	The cef reference pointer.
        *
        * @return	pCefRefPtr as an amo::u8json.
        */
        amo::u8json toSimplifiedJson(cefrefptr_type pCefRefPtr) {
            transfer_type pTransfer = toTransfer(pCefRefPtr);
            return pTransfer->getFuncMgr().toSimplifiedJson();
        }
        
        amo::u8json toSimplifiedJson(transfer_type pTransfer) {
            return pTransfer->getFuncMgr().toSimplifiedJson();
        }
        /*!
         * @fn	void TransferMappingMgrBase::removeMapping(cefrefptr_type pCefRefPtr)
         *
         * @brief	通过原始类移除映射.
         *
         * @param	pCefRefPtr	The cef reference pointer.
         */
        void removeMapping(cefrefptr_type pCefRefPtr) {
            transfer_type pTransfer = findTransferByCefRefPtr(pCefRefPtr);
            
            if (!pTransfer) {
                return;
            }
            
            ClassTransfer::removeTransfer(pTransfer->getObjectID());
            removeMapping(pTransfer);
        }
        /*!
         * @fn	void TransferMappingMgrBase::removeMapping(transfer_type pTransfer)
         *
         * @brief	通过映射类移除映射.
         *
         * @param	pTransfer	The transfer.
         */
        void removeMapping(transfer_type pTransfer) {
            for (auto& p : m_oTransferMap) {
                if (p.second == pTransfer) {
                    ClassTransfer::removeTransfer(p.second->getObjectID());
                    m_oTransferMap.erase(p.first);
                    return;
                }
            }
        }
    private:
        /*! @brief	对象管理器. */
        std::unordered_map<cefrefptr_type,
            transfer_type, cefrefptr_hash<cefrefptr_type> > m_oTransferMap;
    };
    
    
    template<typename T>
    class TransferMappingMgr
        : public TransferMappingMgrBase<T,
          std::shared_ptr<T>,
          typename TransferMapping<shared_ptr<T> >::value_type> {
    public:
        typedef std::shared_ptr<T> transfer_type;
        typedef typename TransferMapping<shared_ptr<T> >::value_type cefrefptr_type;
    public:
    
    };
    
    
    
    ///*!
    //* @class	TransferMappingMgr
    //*
    //* @brief	类型映射管理器.
    //*/
    //template<typename T>
    //class TransferMappingMgr : public amo::singleton < TransferMappingMgr<T> > {
    //public:
    //    typedef std::shared_ptr<T> transfer_type;
    //    typedef typename TransferMapping<shared_ptr<T> >::value_type cefrefptr_type;
    //public:
    //
    //    /*!
    //     * @fn	transfer_type TransferMappingMgr::toTransfer(cefrefptr_type pCefRefPtr)
    //     *
    //     * @brief	将Cef类型转换为Transfer类型.
    //     *
    //     * @param	pCefRefPtr	Cef类型.
    //     *
    //     * @return	Transfer类型.
    //     */
    //    transfer_type toTransfer(cefrefptr_type pCefRefPtr) {
    //        auto iter = m_oTransferMap.find(pCefRefPtr);
    //
    //        // 如果已经有映射的Transfer存在，直接返回
    //        if (iter != m_oTransferMap.end()) {
    //            return iter->second;
    //        }
    //
    //        // 创建一个新的Transfer并保存
    //        transfer_type pTransfer;
    //        pTransfer = ClassTransfer::createTransfer<T>(pCefRefPtr);
    //        m_oTransferMap[pCefRefPtr] = pTransfer;
    //        return pTransfer;
    //    }
    //
    //    /*!
    //     * @fn	amo::u8json TransferMappingMgr::toJson(cefrefptr_type pCefRefPtr)
    //     *
    //     * @brief	将Cef类型转换为JSON.
    //     *
    //     * @param	pCefRefPtr	The cef reference pointer.
    //     *
    //     * @return	pCefRefPtr as an amo::u8json.
    //     */
    //    amo::u8json toJson(cefrefptr_type pCefRefPtr) {
    //        transfer_type pTransfer = toTransfer(pCefRefPtr);
    //        return pTransfer->getFuncMgr().toJson();
    //    }
    //
    //    /*!
    //     * @fn	cefrefptr_type TransferMappingMgr::formJson(amo::u8json& json)
    //     *
    //     * @brief	将JSON转换为Cef类型.
    //     *
    //     * @param 	json	The JSON.
    //     *
    //     * @return	A cefrefptr_type.
    //     */
    //    cefrefptr_type formJson(amo::u8json& json) {
    //        int nObjectID = json.get<int64_t>("id");
    //
    //        for (auto& p : m_oTransferMap) {
    //            if (p.second->getObjectID() == nObjectID) {
    //                return p.first;
    //            }
    //        }
    //
    //        return NULL;
    //    }
    //
    //    /*!
    //     * @fn	amo::u8json TransferMappingMgr::toSimplifiedJson(cefrefptr_type pCefRefPtr)
    //     *
    //     * @brief	将Cef类型转换为一个简单的JSON对象（只有ID和Name）.
    //     *
    //     * @param	pCefRefPtr	The cef reference pointer.
    //     *
    //     * @return	pCefRefPtr as an amo::u8json.
    //     */
    //    amo::u8json toSimplifiedJson(cefrefptr_type pCefRefPtr) {
    //        transfer_type pTransfer = toTransfer(pCefRefPtr);
    //        return pTransfer->getFuncMgr().toSimplifiedJson();
    //    }
    //
    //    amo::u8json toSimplifiedJson(transfer_type pTransfer) {
    //        return pTransfer->getFuncMgr().toSimplifiedJson();
    //    }
    //
    //private:
    //    /*! @brief	对象管理器. */
    //    std::unordered_map<cefrefptr_type,
    //        transfer_type, cefrefptr_hash<cefrefptr_type> > m_oTransferMap;
    //};
    
}

#endif // AMO_TRANSFERMAPPINGMGR_HPP__
