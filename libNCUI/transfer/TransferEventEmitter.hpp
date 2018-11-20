// Created by amoylel on 11/27/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_TRANSFEREVENTEMITTER_HPP__
#define AMO_TRANSFEREVENTEMITTER_HPP__

#include <memory>
#include <functional>
#include "transfer/TransferEventInfo.hpp"
#include <amo/logger.hpp>

#ifndef PROCESS_EVENT
#define PROCESS_EVENT(name, process)\
	m_data->collectProcessEvent(#name, process);\

#endif

#ifndef PROCESS_ERROR_EVENT
#define PROCESS_ERROR_EVENT(name)\
	m_data->collectProcessEvent(#name, 100, 1);\

#endif


namespace amo {
    class TransferEventEmitter
        : public std::enable_shared_from_this<TransferEventEmitter> {
    public:
        TransferEventEmitter(
            std::function<Any(const TransferEventInfo&)> fn = {},
            bool toAll = true)
            : m_fnEventCallback(fn)
            , m_bDefaultToAll(toAll) {
            
            if (m_bDefaultToAll) {
                m_bToAll.reset(new bool(true));
            }
        }
        
        
    public:
        virtual bool beforeTriggerEvent(TransferEventInfo& info) {
            $cdevel(info.toString().c_str());
            return false;
        }
        
        Any triggerEvent(TransferEventInfo& info) {
            if (m_bToAll) {
                info.toAll = true;
            }
            
            if (m_bSuspend) {
                info.suspend = true;
            }
            
            m_bToAll.reset();
            m_bSuspend.reset();
            
            
            if (m_bDefaultToAll) {
                m_bToAll.reset(new bool(true));
            }
            
            if (beforeTriggerEvent(info)) {
                return Undefined();
            }
            
            if (m_fnEventCallback) {
                return m_fnEventCallback(info);
            }
            
            return Undefined();
        }
        
        // status 0 正常
        // status 1 失败
        //
        Any collectProcessEvent(const amo::u8string& processName, int pec,
                                int status = 0) {
            TransferEventInfo info(amo::u8string("collect.process", true));
            amo::u8json json;
            json.put("name", processName.to_utf8());
            json.put("process", pec);
            json.put("status", status);
            info.setData(json);
            return triggerEvent(info);
        }
        
        template<typename T>
        Any triggerEvent(const amo::u8string& name, const T& data) {
            TransferEventInfo info(name.to_utf8());
            info.setData(data);
            return triggerEvent(info);
        }
        
        
        
        template<>
        Any triggerEvent<amo::u8string>(const amo::u8string& name,
                                        const amo::u8string& data) {
            TransferEventInfo info(name);
            info.setData(data.to_utf8());
            return triggerEvent(info);
        }
        
        
        
        template<typename ... Args>
        Any stringEvent(const amo::u8string& name, Args... args) {
            TransferEventInfo info(name);
            info.setData(catString(args...).to_utf8());
            return triggerEvent(info);
        }
        
        
        
        template<typename ... Args>
        amo::u8string catString(const amo::u8string& str, Args... args) {
            amo::u8string ret = str;
            ret += catString(args...);
            return ret;
        }
        
        template<>
        amo::u8string catString<>(const amo::u8string& str) {
            return str;
        }
        
        std::shared_ptr<TransferEventEmitter> all() {
            if (!m_bToAll) {
                m_bToAll.reset(new bool(true));
            }
            
            return shared_from_this();
        }
        
        std::shared_ptr<TransferEventEmitter> suspend() {
            if (!m_bSuspend) {
                m_bSuspend.reset(new bool(true));
            }
            
            return shared_from_this();
        }
        
        
    public:
        std::function<Any(const TransferEventInfo&)> m_fnEventCallback;
        bool m_bDefaultToAll;
        std::shared_ptr<bool> m_bToAll;		// to all
        std::shared_ptr<bool> m_bSuspend;	// 挂起线程
        
    };
}

#endif // AMO_TRANSFEREVENTEMITTER_HPP__