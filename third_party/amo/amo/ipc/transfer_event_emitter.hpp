// Created by amoylel on 14/02/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AMO_TRANSFER_EVENT_EMITTER_59DDA26A_1CD7_4C7D_930D_833C6F253780_HPP__
#define AMO_TRANSFER_EVENT_EMITTER_59DDA26A_1CD7_4C7D_930D_833C6F253780_HPP__




#include <memory>
#include <functional>
#include <amo/ipc/transfer_event_info.hpp>
#include <amo/logger.hpp>

namespace amo {
    class transfer_event_emitter
        : public std::enable_shared_from_this<transfer_event_emitter> {
    public:
        transfer_event_emitter(
            std::function<any(const transfer_event_info&)> fn = {},
            bool toAll = true)
            : m_fnEventCallback(fn)
            , m_bDefaultToAll(toAll) {
            
            if (m_bDefaultToAll) {
                m_bToAll.reset(new bool(true));
            }
        }
        
        
    public:
        virtual bool beforeTriggerEvent(transfer_event_info& info) {
            $cdevel(info.toString().c_str());
            return false;
        }
        
        any triggerEvent(transfer_event_info& info) {
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
                return undefined();
            }
            
            if (m_fnEventCallback) {
                return m_fnEventCallback(info);
            }
            
            return undefined();
        }
        
        
        
        
        template<typename T>
        any triggerEvent(const amo::string& name, const T& data) {
            transfer_event_info info(name.to_utf8());
            info.setData(data);
            return triggerEvent(info);
        }
        
        
        
        template<>
        any triggerEvent<amo::string>(const amo::string& name,
                                      const amo::string& data) {
            transfer_event_info info(name.to_utf8());
            info.setData(data.to_utf8());
            return triggerEvent(info);
        }
        
        
        
        template<typename ... Args>
        any stringEvent(const amo::string& name, Args... args) {
            transfer_event_info info(name.to_utf8());
            info.setData(catString(args...).to_utf8());
            return triggerEvent(info);
        }
        
        
        
        template<typename ... Args>
        amo::string catString(const amo::string& str, Args... args) {
            amo::string ret = str;
            ret += catString(args...);
            return ret;
        }
        
        template<>
        amo::string catString<>(const amo::string& str) {
            return str;
        }
        
        std::shared_ptr<transfer_event_emitter> all() {
            if (!m_bToAll) {
                m_bToAll.reset(new bool(true));
            }
            
            return shared_from_this();
        }
        
        std::shared_ptr<transfer_event_emitter> suspend() {
            if (!m_bSuspend) {
                m_bSuspend.reset(new bool(true));
            }
            
            return shared_from_this();
        }
        
        
    public:
        std::function<any(const transfer_event_info&)> m_fnEventCallback;
        bool m_bDefaultToAll;
        std::shared_ptr<bool> m_bToAll;		// to all
        std::shared_ptr<bool> m_bSuspend;	// 挂起线程
        
    };
}


#endif //AMO_TRANSFER_EVENT_EMITTER_59DDA26A_1CD7_4C7D_930D_833C6F253780_HPP__
