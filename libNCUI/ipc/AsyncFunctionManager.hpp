// Created by amoylel on 05/21/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_ASYNCFUNCTIONMANAGER_HPP__
#define AMO_ASYNCFUNCTIONMANAGER_HPP__




#include <amo/singleton.hpp>
#include <unordered_map>
#include "handler/CefHeader.hpp"
#include <unordered_map>
#include <functional>

namespace amo {

    /*!
     * @class	AsyncFunctionWrapper
     *
     * @brief	js 异步调用C++结果回调函数.
     */
    
    class AsyncFunctionWrapper {
    public:
        AsyncFunctionWrapper(CefRefPtr<CefFrame> pFrame = NULL, CefRefPtr<CefV8Value> pFunction = NULL) {
            m_pFrame = pFrame;
            m_pFunction = pFunction;
        }
        CefRefPtr<CefV8Value> m_pFunction;
        CefRefPtr<CefFrame> m_pFrame;
    };
    
    template<cef_process_id_t>
    class AsyncFunctionManager;
    
    template<> class AsyncFunctionManager<PID_RENDERER>
        : public amo::singleton<AsyncFunctionManager<PID_RENDERER> > {
    public:
        AsyncFunctionManager() {
            m_nIndex = 0;
        }
        
        int registerCallabackFunction(CefRefPtr<CefFrame> pFrame,
                                      CefRefPtr<CefV8Value> ptr) {
            ++m_nIndex;
            m_cbMap[m_nIndex] = AsyncFunctionWrapper(pFrame, ptr);
            return m_nIndex;
        }
        
        void unRegisterCallbackFunction(const int& nKey) {
            m_cbMap.erase(nKey);
        }
        
        AsyncFunctionWrapper& get(const int& nKey) {
            auto iter = m_cbMap.find(nKey);
            
            if (iter == m_cbMap.end()) {
                return m_NoItem;
            }
            
            return iter->second;
        }
        
        
    private:
        std::unordered_map<int, AsyncFunctionWrapper > m_cbMap;
        int m_nIndex;
        AsyncFunctionWrapper m_NoItem;
    };
    
    template<> class AsyncFunctionManager<PID_BROWSER>
        : public amo::singleton <  AsyncFunctionManager<PID_BROWSER> > {
    public:
        typedef std::function<void(Any&)> FuncType;
    public:
        int registerCallabackFunction(FuncType fn) {
            int id = ++nId;
            m_oCallbackMap[id] = fn;
            return id;
        }
        
        FuncType get(int nID) {
            auto iter = m_oCallbackMap.find(nID);
            
            if (iter == m_oCallbackMap.end()) {
                return FuncType();
            }
            
            return iter->second;
        }
        
        void unRegisterCallbackFunction(int nID) {
            m_oCallbackMap.erase(nID);
        }
    private:
        /*! @brief	The callback map. */
        std::unordered_map<int, FuncType> m_oCallbackMap;
        /*! @brief	The identifier. */
        int nId;
    };
}

#endif // AMO_ASYNCFUNCTIONMANAGER_HPP__