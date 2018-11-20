// Created by amoylel on 06/04/2017.
// Copyright (c) 2017 amoylel All rights reserved.

#ifndef AMO_HANDLERDELEGATE_H__
#define AMO_HANDLERDELEGATE_H__


#include <list>
#include "handler/CefHeader.hpp"

namespace amo {
    template<typename T>
    class HandlerDelegate {
    public:
        typedef std::list < T*> DelegateSet;
    public:
        HandlerDelegate() {}
        virtual ~HandlerDelegate() {}
        
        /*!
         * @fn	virtual bool HandlerDelegate::RegisterDelegate(T* delegate, int nIndex = -1)
         *
         * @brief	Registers the delegate.
         *
         * @param 	delegate	If non-null, the delegate.
         * @param	nIndex				(Optional) the index.
         *
         * @return	true if it succeeds, false if it fails.
         */
        
        virtual bool RegisterDelegate(T* delegate, int nIndex = -1) {
            for (auto& p : m_Delegates) {
                if (p == delegate) {
                    return true;
                }
            }
            
            if (nIndex < 0 || nIndex >= (int)m_Delegates.size()) {
                m_Delegates.push_back(delegate);
            } else {
                int nPos = 0;
                auto iter = m_Delegates.begin();
                
                while (nPos < nIndex) {
                    ++iter;
                }
                
                m_Delegates.insert(iter, delegate);
            }
            
            return true; //
            //return m_Delegates.insert(delegate).second;
        }
        
        /*!
         * @fn	virtual void HandlerDelegate::UnregisterDelegate(T* delegate)
         *
         * @brief	Unregisters the delegate described by delegate.
         *
         * @param 	delegate	If non-null, the delegate.
         */
        
        virtual void UnregisterDelegate(T* delegate) {
            auto iter = std::find(m_Delegates.begin(), m_Delegates.end(), delegate);
            m_Delegates.erase(iter);
        }
        
        /*!
         * @fn	DelegateSet& HandlerDelegate::GetDelegates()
         *
         * @brief	Gets the delegates.
         *
         * @return	The delegates.
         */
        
        DelegateSet& GetDelegates() {
            return m_Delegates;
        }
        
        /*!
         * @fn	void HandlerDelegate::CopyDelegates(const DelegateSet& delegates)
         *
         * @brief	Copies the delegates described by delegates.
         *
         * @param	delegates	The delegates.
         */
        
        void CopyDelegates(const DelegateSet& delegates) {
            DelegateSet::const_iterator it = delegates.begin();
            
            for (; it != delegates.end(); ++it) {
                RegisterDelegate(*it);
            }
        }
    protected:
        DelegateSet m_Delegates;
    };
    
}

#endif // AMO_HANDLERDELEGATE_H__