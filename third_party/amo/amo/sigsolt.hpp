// Created by amoylel on 22/08/2018.
// Copyright (c) 2018 amoylel All rights reserved.

#ifndef AGENTMGR_SIGSOLT_47323E38_C6B7_4F06_99E3_09ED27EB0A11_HPP__
#define AGENTMGR_SIGSOLT_47323E38_C6B7_4F06_99E3_09ED27EB0A11_HPP__




#include <set>
#include <amo/any.hpp>
#include <amo/functional.hpp>
#include <amo/stdint.hpp>
#include <amo/uid.hpp>
#include <amo/unordered_map.hpp>
#include <amo/signals2.hpp>
#include <amo/smart_ptr.hpp>
#include <amo/optional.hpp>
#include <amo/logger.hpp>


template< typename R, typename EventType, typename ... Args>
class emit_impl {
public:
    emit_impl(boost::any obj) {
        m_fn = obj;
    }
    
    amo::optional<R> emit(const EventType& event_id, Args ... args) {
        amo::optional<R> ret;
        
        try {
        
            ret = boost::any_cast<amo::shared_ptr<amo::signals2::signal<R(Args ...)> >>
                  (m_fn)->operator()(args...);
                  
            return ret;
        } catch (boost::bad_any_cast& e) {
            $cerr(e.what());
            return ret;
        }
        
        return ret;
    }
    boost::any m_fn;
};

template< typename EventType, typename ... Args>
class emit_impl<amo::nil, EventType, Args ...> {

public:
    emit_impl(boost::any obj) {
        m_fn = obj;
    }
    
    amo::optional<amo::nil> emit(const EventType& event_id, Args ... args) {
        amo::optional<amo::nil> ret;
        
        try {
        
            boost::any_cast<amo::shared_ptr<amo::signals2::signal<void(Args ...)> >>
                    (m_fn)->operator()(args...);
                    
            return ret;
        } catch (boost::bad_any_cast& e) {
            amo::cerr << __FUNCTION__ << e.what() << amo::endl;
            return ret;
        }
        
        return ret;
    }
public:
    boost::any m_fn;
};


namespace amo {
    template<typename EventType>
    class slot {
    public:
        typedef amo::unordered_map<EventType, boost::any> SignalMap;
        typedef amo::unordered_map<EventType, boost::any> SignalsType;
        
    public:
        template<typename Functor>
        void listen(const EventType& event_id, amo::function<Functor> slt) {
            SignalsType& signals_ = get_signals();
            typename  SignalsType::iterator iter = signals_.find(event_id);
            
            if (iter == signals_.end()) {
                amo::shared_ptr<amo::signals2::signal<Functor> > sig(new
                        amo::signals2::signal<Functor>());
                sig->connect(slt);
                signals_[event_id] = sig;
                boost::any_cast<amo::shared_ptr<amo::signals2::signal<Functor> >>
                        (signals_[event_id]);
            } else {
                try {
                    boost::any_cast<amo::shared_ptr<amo::signals2::signal<Functor> >>
                            (iter->second)->connect(slt);
                } catch (boost::bad_any_cast& e) {
                    amo::cerr << __FUNCTION__ << e.what() << amo::endl;
                    return;
                }
            }
        }
        
        SignalsType& get_signals() {
            return signals_;
        }
        
    private:
    
        SignalsType signals_;
    };
    
    
    template<typename EventType>
    class signal {
    public:
    
        typedef amo::unordered_map<EventType, boost::any> SignalMap;
        typedef amo::unordered_map<EventType, boost::any> SignalsType;
    public:
        void connect(slot<EventType>* slt) {
            if (slt == NULL) {
                return;
            }
            
            m_slt.insert(slt);
        }
        
        void disconnect(slot<EventType>* slt) {
            if (slt == NULL) {
                return;
            }
            
            m_slt.erase(slt);
        }
        
        template<typename R, typename ... Args>
        amo::optional<R> emit(const EventType& event_id, Args... args) {
        
            amo::optional<R> ret;
            
            for (typename std::set<slot<EventType>*>::iterator it = m_slt.begin();
                    it != m_slt.end(); ++it) {
                SignalsType& signals_ = (*it)->get_signals();
                typename SignalsType::iterator iter = signals_.find(event_id);
                
                if (iter == signals_.end()) {
                    return ret;
                }
                
                ret = emit_impl< R, EventType, Args ...>(iter->second).emit(event_id, args...);
            }
            
            return ret;
        }
        
    protected:
        std::set<slot<EventType>*> m_slt;
    };
    
    template<typename EventType>
    class sigslot : public signal<EventType>, public slot<EventType> {
    public:
    
    };
}


#endif //AGENTMGR_SIGSOLT_47323E38_C6B7_4F06_99E3_09ED27EB0A11_HPP__
